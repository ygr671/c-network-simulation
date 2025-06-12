// Inclusion des bibliothèques standard nécessaires
#include <arpa/inet.h> // Pour la fonction htons (conversion d'entiers courts de l'ordre des octets de l'hôte à l'ordre des octets du réseau)
#include <stdio.h>     // Pour les fonctions d'entrée/sortie standard (printf, etc.)
#include <stdlib.h>    // Pour les fonctions utilitaires standard (EXIT_SUCCESS, malloc, free, etc.)
#include <string.h>    // Pour les fonctions de manipulation de chaînes de caractères (memcpy, strlen)

// Inclusion des fichiers d'en-tête du projet
#include "include/config.h"
#include "include/reseau.h"
#include "include/trame.h"

// Fonction principale du programme
int main(void)
{
  // Déclaration d'une variable de type reseau_t pour représenter le réseau
  reseau_t rs;
  // Initialisation de la structure du réseau (allocation de mémoire, mise à zéro des compteurs, etc.)
  init_reseau_t(&rs);
  // Chargement de la configuration du réseau à partir d'un fichier spécifié.
  // Le fichier "configurations/lan1.lan" contient la description de la topologie du réseau (équipements et liens).
  charger_configuration(&rs, "configurations/lan_hasard.lan");

  // Affichage d'un titre pour la section de la configuration initiale
  printf("\n=== Configuration initiale du réseau ===\n");
  // Affichage de l'état actuel du réseau (liste des équipements et des liens)
  afficher_reseau_t(&rs);

  // Vérifions qu'il y a assez d'équipements pour les tests
  // et que les équipements ciblés sont du bon type.
  if (rs.nb_equipements < 3) {
    fprintf(stderr, "Pas assez d'équipements dans la configuration pour les tests prévus.\n");
    deinit_reseau_t(&rs);
    return EXIT_FAILURE;
  }

  // Index du switch que nous allons tester (Switch ID 1 dans lan_hasard.lan)
  int switch_test_index = 1;
  // ID du port d'entrée sur le switch (Station ID 0 dans lan_hasard.lan)
  int port_entree_id = 0; 

  if ((unsigned int)switch_test_index >= rs.nb_equipements || rs.equipements[switch_test_index].type != SWITCH) {
    fprintf(stderr, "L'équipement à l'index %d n'est pas un switch ou n'existe pas.\n", switch_test_index);
    deinit_reseau_t(&rs);
    return EXIT_FAILURE;
  }
  if ((unsigned int)port_entree_id >= rs.nb_equipements || rs.equipements[port_entree_id].type != STATION) {
    fprintf(stderr, "L'équipement d'entrée (ID %d) n'est pas une station ou n'existe pas.\n", port_entree_id);
    deinit_reseau_t(&rs);
    return EXIT_FAILURE;
  }

  // Utiliser la MAC de la station connectée (Station ID 0) comme source
  mac_address_t mac_source_test = rs.equipements[port_entree_id].contenu.st.mac;

  // Test 1: Trame Broadcast
  // Ce bloc de code simule l'envoi d'une trame de diffusion (broadcast) sur le réseau.
  // Une trame de broadcast est destinée à tous les équipements sur le même segment de réseau.
  printf("\n=== Test 1: Envoi d'une trame broadcast ===\n");
  // Déclaration d'une variable pour stocker la trame Ethernet
  trame_ethernet_t trame;
  // Définition de l'adresse MAC destination comme étant l'adresse de broadcast (tous les bits à 1)
  mac_address_t mac_dest_broadcast = {{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}}; // Broadcast

  // Initialisation des champs de la trame Ethernet (adresses MAC, type, données)
  init_trame_ethernet(&trame, &mac_source_test, &mac_dest_broadcast);
  // Définition du champ "type" de la trame. htons convertit 0x0800 (IPv4) de l'ordre des octets de l'hôte vers l'ordre des octets du réseau.
  trame.type = htons(0x0800); // IPv4
  // Définition du contenu (payload) de la trame
  const char *message = "Message broadcast de test";
  // Copie du message dans le champ de données de la trame
  memcpy(trame.donnees, message, strlen(message));
  // Mise à jour de la taille des données dans la trame
  trame.taille_donnees = strlen(message);

  // Affichage d'informations sur la trame envoyée
  printf("\nEnvoi de la trame broadcast depuis Station ID %d vers Switch ID %d:\n", port_entree_id, rs.equipements[switch_test_index].id);
  printf("------------------------------------------------\n");
  // Affichage détaillé de la trame
  afficher_trame(&trame);
  printf("\nPropagation de la trame dans le réseau...\n");
  // Simulation du traitement de la trame par le switch spécifié, arrivant via l'équipement port_entree_id
  traiter_trame_switch(&rs, switch_test_index, &trame, port_entree_id);

  // Test 2: Trame Unicast vers Station 1
  printf("\n=== Test 2: Envoi d'une trame unicast vers Station 1 ===\n");
  // Utiliser la MAC de la Station ID 2 comme destination
  mac_address_t mac_dest_unicast = rs.equipements[2].contenu.st.mac; 
  init_trame_ethernet(&trame, &mac_source_test, &mac_dest_unicast);
  trame.type = htons(0x0800); // IPv4
  const char *message2 = "Message uniquement pour Station 1";
  memcpy(trame.donnees, message2, strlen(message2));
  trame.taille_donnees = strlen(message2);

  // Affichage d'informations sur la trame unicast envoyée
  printf("\nEnvoi de la trame unicast depuis Station ID %d vers Station ID %d (via Switch ID %d):\n", port_entree_id, rs.equipements[2].id, rs.equipements[switch_test_index].id);
  printf("---------------------------------------------\n");
  // Affichage détaillé de la trame
  afficher_trame(&trame);
  printf("\nPropagation de la trame dans le réseau...\n");
  // Le switch devrait utiliser sa table de commutation pour acheminer la trame si l'adresse MAC destination est connue.
  traiter_trame_switch(&rs, switch_test_index, &trame, port_entree_id);

  // Test 3: Vérification de l'apprentissage
  // Ce bloc vérifie l'état de la table de commutation du switch après les envois de trames.
  // Les switchs apprennent les adresses MAC sources des trames qu'ils reçoivent et les associent aux ports d'entrée.
  printf("\n=== Test 3: Vérification de la table de commutation ===\n");
  printf("État de la table de commutation du switch ID %d (Index %d):\n", rs.equipements[switch_test_index].id, switch_test_index);
  printf("------------------------------------------\n");
  // Affichage de la table de commutation du switch avec l'ID 0.
  // On s'attend à y trouver l'adresse MAC `src` associée au port 1, car des trames avec cette source ont été traitées.
  afficher_table_commutation(&rs.equipements[switch_test_index].contenu.sw.tc);

  // Libération de toute la mémoire allouée dynamiquement pour la structure du réseau
  deinit_reseau_t(&rs);
  // Retourne EXIT_SUCCESS pour indiquer que le programme s'est terminé sans erreur.
  return EXIT_SUCCESS;
}