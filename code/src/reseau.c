// Inclusion des fichiers d'en-tête nécessaires du projet
#include "include/reseau.h"
#include "include/mac.h"
#include "include/station.h"
#include "include/switch.h"
#include "include/table_commutation.h"
// Inclusion des bibliothèques standard
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Définit la capacité initiale pour les tableaux d'équipements et de liens lors de l'allocation dynamique.
#define CAPACITE_INITIALE 8
// Définit la taille du tampon pour certaines opérations (non utilisé directement dans ce fichier, mais peut être pertinent pour des fonctions appelées).
#define TAILLE_BUFFER 256

// Fonction pour initialiser une structure reseau_t.
// Alloue la mémoire initiale pour les listes d'équipements et de liens.
void init_reseau_t(reseau_t *rs)
{
  // Initialise les compteurs et capacités pour les équipements.
  rs->nb_equipements = 0;
  rs->index_equipement_actuel = 0;
  rs->capacite_equipements = CAPACITE_INITIALE;

  // Alloue de la mémoire pour le tableau d'équipements.
  rs->equipements =
      malloc(rs->capacite_equipements *
             sizeof(equipement_t)); // Pas de calloc() dans le code pour une
                                    // lecture plus intuitive
  if (rs->equipements == NULL)
  {
    // En cas d'échec de malloc, affiche une erreur et termine le programme.
    perror("malloc (init_reseau_t -> equipements) : ");
    exit(EXIT_FAILURE);
  }

  // Initialise les compteurs et capacités pour les liens.
  rs->nb_liens = 0;
  rs->index_lien_actuel = 0;
  rs->capacite_liens = CAPACITE_INITIALE;

  // Alloue de la mémoire pour le tableau de liens.
  rs->liens = malloc(rs->capacite_liens * sizeof(lien_t));
  if (rs->liens == NULL)
  {
    // En cas d'échec de malloc, libère la mémoire déjà allouée pour les équipements, affiche une erreur et termine.
    perror("malloc (init_reseau_t -> liens) : ");
    free(rs->equipements); // Libère la mémoire allouée précédemment pour les équipements.
    exit(EXIT_FAILURE);
  }
}

void deinit_reseau_t(reseau_t *rs)
{
  // Mise à zéro et libération de la mémoire allouée
  // Boucle sur tous les équipements pour libérer les ressources spécifiques à chaque type.
  for (size_t i = 0; i < rs->nb_equipements; i++)
  {
    // Si l'équipement est un switch, il faut désinitialiser sa table de commutation
    // et libérer la mémoire allouée pour l'état de ses ports.
    if (rs->equipements[i].type == SWITCH)
    {
      deinit_table_commutation_t(&rs->equipements[i].contenu.sw.tc);
      free(rs->equipements[i].contenu.sw.etat_ports);
      rs->equipements[i].contenu.sw.etat_ports = NULL; // Met le pointeur à NULL après libération.
    }
    // Pour les stations, il n'y a pas de mémoire allouée dynamiquement dans la structure station_t elle-même
    // (les adresses IP et MAC sont des structures, pas des pointeurs alloués ici).
  }

  // Réinitialise les capacités et compteurs.
  rs->capacite_equipements = 0;
  rs->capacite_liens = 0;
  rs->nb_equipements = 0;
  rs->nb_liens = 0;
  // Libère la mémoire des tableaux principaux d'équipements et de liens.
  free(rs->equipements);
  rs->equipements = NULL;
  free(rs->liens);
  rs->liens = NULL;
}

void afficher_reseau_t(const reseau_t *rs)
{
  printf("=== Réseau ===\n");
  printf("Nombre d'équipements : %hu\n", rs->nb_equipements);

  // Boucle pour afficher chaque équipement du réseau.
  for (size_t i = 0; i < rs->nb_equipements; i++)
  {
    afficher_equipement_t(&rs->equipements[i]);
  }

  printf("\n"); // Ajoute un saut de ligne pour la lisibilité.

  printf("Nombre de liens : %hu\n", rs->nb_liens);
  // Boucle pour afficher chaque lien du réseau.
  for (size_t i = 0; i < rs->nb_liens; i++)
  {
    afficher_lien_t(&rs->liens[i]);
  }
  printf("\n"); // Ajoute un saut de ligne pour la lisibilité.
}

// Fonction pour afficher les détails d'un équipement spécifique.
// L'affichage dépend du type de l'équipement (Station ou Switch).
void afficher_equipement_t(const equipement_t *eq)
{
  // Utilise une instruction switch pour gérer les différents types d'équipements.
  switch (eq->type)
  {
  case STATION:
    printf("ID : %hu\nType : Station\nAdresse IP : ", eq->id);
    afficher_ip_t(&eq->contenu.st.ip);
    printf("Adresse MAC : ");
    afficher_mac(&eq->contenu.st.mac);
    printf("\n\n"); // Deux sauts de ligne pour séparer les équipements.
    break;
  case SWITCH:
    printf("ID : %hu\nType : Switch\nAdresse MAC : ", eq->id);
    afficher_mac(&eq->contenu.sw.ma);
    printf(
        "\nNombre de ports : %hu\nPriorité STP : %hu\nTable de commutation :\n",
        eq->contenu.sw.nb_ports, eq->contenu.sw.priorite_stp);
    afficher_table_commutation(&eq->contenu.sw.tc);
    // Pas de saut de ligne supplémentaire ici car afficher_table_commutation en ajoute déjà.
    break;
  default:
    // Cas où le type d'équipement n'est pas reconnu.
    fprintf(stderr, "Erreur : type d'équipement inconnu\n");
    break;
  }
}

// Fonction pour afficher les détails d'un lien.
void afficher_lien_t(const lien_t *ln)
{
  printf("ID équipement 1 : %hu\nID équipement 2 : %hu\nPoids : %hu\n", ln->id1,
         ln->id2, ln->poids);
}

// Fonction pour ajouter un équipement au réseau à partir d'une chaîne de description.
// Gère la réallocation dynamique du tableau d'équipements si nécessaire.
void ajouter_equipement_t(reseau_t *rs, char *eq_desc)
{
  // Vérifie si la capacité actuelle du tableau d'équipements est atteinte.
  if (rs->index_equipement_actuel >= rs->capacite_equipements)
  {
    rs->capacite_equipements *= 2;
    rs->equipements = realloc(rs->equipements,
                              rs->capacite_equipements * sizeof(equipement_t));
    if (rs->equipements == NULL)
    {
      // En cas d'échec de realloc, affiche une erreur, libère les ressources et termine.
      perror("realloc (ajouter_equipement_t)");
      deinit_reseau_t(rs);
      exit(EXIT_FAILURE);
    }
  }

  // Variable statique pour pouvoir mettre un ID unique à chaque noeud du réseau
  // Cet ID correspondra également à l'index de l'équipement dans le tableau rs->equipements.
  static unsigned short id = 0;

  // Parsing de la ligne contenant l'équipement
  unsigned short eq_type_us; // Variable temporaire pour lire le type d'équipement sous forme d'entier.
  type_equipement_t eq_type; // Variable pour stocker le type d'équipement énuméré.

  // Tente de parser le premier champ de la chaîne (le type d'équipement).
  if (sscanf(eq_desc, "%hu", &eq_type_us) != 1)
  {
    perror("sscanf (ajouter_equipement_t - lecture type)");
    // Gérer l'erreur plus proprement ? Pour l'instant, on continue, ce qui peut mener à un comportement indéfini.
    // Il serait préférable de retourner une erreur ou d'arrêter si le type ne peut être lu.
  }

  // Convertit l'entier lu en type énuméré.
  if (eq_type_us == 1)
    eq_type = STATION;
  else if (eq_type_us == 2) // Utiliser else if pour éviter une affectation incorrecte si eq_type_us n'est ni 1 ni 2.
    eq_type = SWITCH;
  else {
    fprintf(stderr, "Type d'équipement non reconnu: %hu\n", eq_type_us);
    // Gérer l'erreur : ne pas ajouter l'équipement, retourner un code d'erreur, etc.
    return; // Arrête l'ajout si le type est inconnu.
  }

  equipement_t eq;    // Structure temporaire pour construire le nouvel équipement.
  unsigned short tmp; // Variable temporaire pour stocker le type lu par sscanf, car il est déjà dans eq_type.

  // Traitement spécifique en fonction du type d'équipement.
  switch (eq_type)
  {
  case STATION:
  {
    if (sscanf(eq_desc,
               // 1;54:d6:a6:82:c5:23;130.79.80.21
               "%hu;%hhx:%hhx:%hhx:%hhx:%hhx:%hhx;%hhu.%hhu.%hhu.%hhu", &tmp,
               &eq.contenu.st.mac.octet[0], &eq.contenu.st.mac.octet[1],
               &eq.contenu.st.mac.octet[2], &eq.contenu.st.mac.octet[3],
               &eq.contenu.st.mac.octet[4], &eq.contenu.st.mac.octet[5],
               &eq.contenu.st.ip.paquet[0], &eq.contenu.st.ip.paquet[1],
               &eq.contenu.st.ip.paquet[2], &eq.contenu.st.ip.paquet[3]) != 11)
    {
      // Si le parsing échoue, affiche un message d'erreur détaillé.
      fprintf(stderr,
              "sscanf (ajouter_equipement_t) : chaîne d'équipement mal "
              "formatée\nchaîne fautive : %s \n",
              eq_desc);
    }
    eq.type = eq_type;
    eq.contenu.st.ip.masque = 24;
    eq.id = id; // Assigne l'ID unique.
    rs->equipements[rs->index_equipement_actuel] = eq; // Ajoute l'équipement au tableau.
    rs->index_equipement_actuel++; // Incrémente l'index pour le prochain équipement.
    id++; // Incrémente l'ID global.
    break; // Fin du cas STATION.
  }
  case SWITCH:
    if (sscanf(eq_desc, "%hu;%hhx:%hhx:%hhx:%hhx:%hhx:%hhx;%hu;%hu", &tmp,
               &eq.contenu.sw.ma.octet[0], &eq.contenu.sw.ma.octet[1],
               &eq.contenu.sw.ma.octet[2], &eq.contenu.sw.ma.octet[3],
               &eq.contenu.sw.ma.octet[4], &eq.contenu.sw.ma.octet[5],
               &eq.contenu.sw.nb_ports, &eq.contenu.sw.priorite_stp) != 9)
    {
      // Si le parsing échoue, affiche un message d'erreur détaillé.
      fprintf(stderr,
              "sscanf (ajouter_equipement_t) : chaîne d'équipement mal "
              "formatée\nchaîne fautive : %s\n",
              eq_desc);
    }
    eq.type = eq_type;

    // Le commentaire original "Bug causé ici" pourrait se référer à l'utilisation de eq.contenu.sw.nb_ports
    // avant de vérifier si sscanf a réussi à lire toutes les valeurs. Si sscanf échoue, nb_ports pourrait être non initialisé.
    // Cependant, la vérification (!= 9) est faite avant. Si elle échoue, le message est affiché, mais le code continue.
    // Il serait plus robuste de retourner ou d'arrêter en cas d'échec de sscanf.

    eq.id = id; // Assigne l'ID unique.
    rs->equipements[rs->index_equipement_actuel] = eq; // Ajoute l'équipement (partiellement initialisé) au tableau.
    // Initialise la table de commutation pour le switch qui vient d'être ajouté au tableau.
    init_table_commutation_t(
        &rs->equipements[rs->index_equipement_actuel].contenu.sw.tc);
    // Alloue de la mémoire pour l'état des ports du switch.
    // La taille est basée sur nb_ports lu depuis le fichier de configuration.
    rs->equipements[rs->index_equipement_actuel].contenu.sw.etat_ports = malloc(
        rs->equipements[rs->index_equipement_actuel].contenu.sw.nb_ports *
        sizeof(etat_port_t)); // etat_port_t doit être défini ailleurs (probablement pour STP).
    if (rs->equipements[rs->index_equipement_actuel].contenu.sw.etat_ports == NULL && rs->equipements[rs->index_equipement_actuel].contenu.sw.nb_ports > 0) {
        perror("malloc (ajouter_equipement_t -> etat_ports)");
        // Gérer l'erreur : potentiellement défaire les initialisations précédentes pour ce switch.
    }
    rs->index_equipement_actuel++; // Incrémente l'index pour le prochain équipement.
    id++; // Incrémente l'ID global.
    break; // Fin du cas SWITCH.
  default:
    // Ce cas ne devrait pas être atteint si la conversion eq_type_us -> eq_type est correcte et gère les erreurs.
    fprintf(stderr, "Logique d'ajout d'équipement atteinte avec un type inconnu.\n");
    break;
  }
}

// Fonction pour ajouter un lien au réseau à partir d'une chaîne de description.
// Gère la réallocation dynamique du tableau de liens si nécessaire.
void ajouter_lien_t(reseau_t *rs, char *lien)
{
  lien_t ln; // Structure temporaire pour stocker les informations du lien.

  // Tente de parser la chaîne pour extraire les IDs des équipements connectés et le poids du lien.
  if (sscanf(lien, "%hu;%hu;%hu", &ln.id1, &ln.id2, &ln.poids) != 3)
  {
    // Si le parsing échoue, affiche une erreur, libère les ressources et termine.
    fprintf(stderr, "sscanf (ajouter_lien_t) : erreur de format dans la chaîne à parser\n");
    fprintf(stderr, "chaîne fautive : '%s'\n", lien);
    deinit_reseau_t(rs);
    exit(EXIT_FAILURE);
  }
  // Vérifie si la capacité actuelle du tableau de liens est atteinte.
  if (rs->index_lien_actuel == rs->capacite_liens)
  {
    rs->capacite_liens *= 2;
    rs->liens = realloc(rs->liens, sizeof(lien_t) * rs->capacite_liens);
    if (rs->liens == NULL)
    {
      // En cas d'échec de realloc, affiche une erreur, libère les ressources et termine.
      perror("realloc (ajouter_lien_t) ");
      deinit_reseau_t(rs);
      exit(EXIT_FAILURE);
    }
  }
  rs->liens[rs->index_lien_actuel] = ln;
  rs->index_lien_actuel++;
  // Note: rs->nb_liens (lu depuis l'en-tête du fichier) devrait correspondre à rs->index_lien_actuel à la fin du chargement.
}

// Fonction pour simuler l'envoi d'une trame par un switch vers un port spécifique.
// Le "port" ici est l'ID de l'équipement de destination connecté à ce switch.
void envoyer_trame_vers_port(reseau_t *rs, int id_switch, int port,
                             const trame_ethernet_t *trame)
{
  printf("Switch %d : Tentative d'envoi sur le port %d\n", id_switch, port);

  // Parcourt tous les liens du réseau.
  // Note: id_switch est l'index de l'équipement switch dans rs->equipements.
  // On suppose que rs->equipements[id_switch].id == id_switch.
  for (int i = 0; i < rs->nb_liens; i++)
  {
    // Vérifie si le lien part du switch actuel (id_switch) et va vers l'équipement 'port'.
    // Cette condition peut être simplifiée si les liens sont directionnels ou si l'on vérifie les deux sens.
    // Actuellement, on suppose que id1 est le switch source et id2 est la destination sur ce lien.
    if (rs->liens[i].id1 == rs->equipements[id_switch].id) // Utilise l'ID réel du switch.
    {
      // Vérifie si l'autre extrémité du lien est l'équipement 'port' de destination.
      if (port == rs->liens[i].id2)
      {
        printf("  -> Lien trouvé vers équipement %d\n", rs->liens[i].id2);

        // Trouve l'équipement connecté à l'autre bout du lien.
        // On pourrait directement utiliser 'port' comme ID pour chercher l'équipement,
        // mais cette boucle assure que l'équipement existe.
        for (int j = 0; j < rs->nb_equipements; j++)
        {
          if (rs->equipements[j].id == rs->liens[i].id2)
          {
            // Si l'équipement connecté est un autre switch, on lui transmet la trame pour traitement.
            if (rs->equipements[j].type == SWITCH)
            {
              printf("  -> Transmission vers switch %d\n",
                     rs->equipements[j].id);
              // Le 'port_entree' pour le switch récepteur est l'ID du switch actuel (l'expéditeur).
              // rs->equipements[j].id est l'ID du switch récepteur, qui est aussi son index (supposition).
              traiter_trame_switch(rs, rs->equipements[j].id /* index du switch récepteur */, trame,
                                   rs->equipements[id_switch].id /* ID du switch envoyeur comme port d'entrée */);
            }
            else if (rs->equipements[j].type == STATION)
            {
              // Si c'est une station, vérifie si la trame lui est destinée (adresse MAC correspond ou broadcast).
              int is_broadcast =
                  memcmp(&trame->dest, "\xFF\xFF\xFF\xFF\xFF\xFF", 6) == 0;
              int is_for_this_station =
                  memcmp(&rs->equipements[j].contenu.st.mac, &trame->dest,
                         sizeof(mac_address_t)) == 0;
              if (is_broadcast || is_for_this_station)
              {
                printf("  -> Trame acceptée par la station %d\n",
                       rs->equipements[j].id);
                afficher_trame(trame);
              }
              else
              {
                printf("  -> Trame ignorée par la station %d (MAC ne "
                       "correspond pas)\n",
                       rs->equipements[j].id);
              }
            } // Fin du if type STATION
            break;
          }
        }
      }
    }
  }
}

// Fonction pour traiter une trame Ethernet arrivant sur un switch.
// id_switch: index du switch dans le tableau rs->equipements.
// trame: la trame reçue.
// port_entree: ID de l'équipement depuis lequel la trame est arrivée sur ce switch.
void traiter_trame_switch(reseau_t *rs, int id_switch,
                          const trame_ethernet_t *trame, int port_entree)
{
  // Récupère un pointeur vers la structure de l'équipement switch.
  equipement_t *switch_eq = &rs->equipements[id_switch];

  // Phase 1: Apprentissage (Learning)
  // Associe l'adresse MAC source de la trame au 'port_entree'.
  // Ici, 'port_entree' est l'ID de l'équipement voisin par lequel la trame est arrivée.
  entree_table_commutation_t nouvelle_entree = {.port = port_entree,
                                                .ma = trame->src};

  // Vérifie si cette association MAC source <-> port_entree existe déjà dans la table.
  int existe = 0;
  for (unsigned int i = 0; i < switch_eq->contenu.sw.tc.nb_entree; i++)
  {
    if (memcmp(&switch_eq->contenu.sw.tc.entrees[i].ma, &trame->src,
               sizeof(mac_address_t)) == 0)
    {
      existe = 1;
      switch_eq->contenu.sw.tc.entrees[i].port = port_entree;
      break;
    }
  }

  // Si l'entrée n'existe pas, l'ajoute à la table de commutation.
  if (!existe)
  {
    ajouter_entree_table_commutation(&switch_eq->contenu.sw.tc,
                                     nouvelle_entree);
  }

  // Phase 2: Commutation (Forwarding/Filtering)
  // Vérifie si l'adresse MAC de destination est une adresse de broadcast.
  if (memcmp(&trame->dest, "\xFF\xFF\xFF\xFF\xFF\xFF", 6) == 0)
  {
    // Si c'est un broadcast, inonde la trame sur tous les ports (liens) du switch,
    // sauf celui par lequel la trame est arrivée.
    printf("Switch %d : Trame broadcast reçue. Inondation...\n", switch_eq->id);
    for (unsigned int i = 0; i < rs->nb_liens; i++)
    {
      // Vérifie si le lien part de ce switch.
      if (rs->liens[i].id1 == switch_eq->id)
      {
        // Ne renvoie pas la trame sur le lien d'où elle vient.
        if (rs->liens[i].id2 != (unsigned short)port_entree) {
          envoyer_trame_vers_port(rs, id_switch, rs->liens[i].id2, trame);
        }
      }
      // Gérer aussi le cas où le switch est id2 dans le lien (si les liens sont bidirectionnels et non ordonnés)
      else if (rs->liens[i].id2 == switch_eq->id) {
        if (rs->liens[i].id1 != (unsigned short)port_entree) {
          envoyer_trame_vers_port(rs, id_switch, rs->liens[i].id1, trame);
        }
      }
    }
    return; // Traitement terminé pour le broadcast.
  }

  // Si ce n'est pas un broadcast, cherche l'adresse MAC de destination dans la table de commutation.
  int port_dest = -1; // Initialisé à -1 (non trouvé).
  for (unsigned int i = 0; i < switch_eq->contenu.sw.tc.nb_entree; i++)
  {
    if (memcmp(&switch_eq->contenu.sw.tc.entrees[i].ma, &trame->dest, sizeof(mac_address_t)) == 0)
    {
      // MAC destination trouvée dans la table. Le 'port' stocké est l'ID de l'équipement voisin.
      port_dest = switch_eq->contenu.sw.tc.entrees[i].port;
      // Vérifie si le port de destination est le même que le port d'entrée.
      if (port_dest == port_entree) {
        printf("Switch %d : Trame pour MAC connue sur le port d'entrée %d. Filtrage.\n", switch_eq->id, port_entree);
        return; // Filtre la trame (ne la renvoie pas).
      }
      break; // Sort de la boucle une fois la destination trouvée.
    }
  }

  // Si l'adresse MAC de destination a été trouvée dans la table de commutation.
  if (port_dest != -1)
  {
    printf("Switch %d : MAC destination connue. Envoi vers équipement ID %d.\n", switch_eq->id, port_dest);
    envoyer_trame_vers_port(rs, id_switch, port_dest, trame);
  }
  else
  {
    // Si l'adresse MAC de destination n'est pas dans la table (Unknown Unicast),
    // inonde la trame sur tous les ports sauf celui d'entrée.
    printf("Switch %d : MAC destination inconnue. Inondation...\n", switch_eq->id);
    for (unsigned int i = 0; i < rs->nb_liens; i++)
    {
      if (rs->liens[i].id1 == switch_eq->id) {
        if (rs->liens[i].id2 != (unsigned short)port_entree) {
          envoyer_trame_vers_port(rs, id_switch, rs->liens[i].id2, trame);
        }
      } else if (rs->liens[i].id2 == switch_eq->id) {
         if (rs->liens[i].id1 != (unsigned short)port_entree) {
          envoyer_trame_vers_port(rs, id_switch, rs->liens[i].id1, trame);
        }
      }
    }
  }
}