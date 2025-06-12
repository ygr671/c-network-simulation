// Inclusion du fichier d'en-tête correspondant (station.h) qui définit la structure station_t.
#include "include/station.h"
// Inclusion des fichiers d'en-tête pour les structures et fonctions relatives aux adresses IP (ip.h) et MAC (mac.h).
#include "include/ip.h"
#include "include/mac.h"
// Inclusion de la bibliothèque standard pour les opérations d'entrée/sortie (comme printf).
#include <stdio.h>
// Inclusion de la bibliothèque standard pour les utilitaires généraux (comme EXIT_SUCCESS, EXIT_FAILURE).
#include <stdlib.h>

// Fonction pour afficher les informations d'une station.
// Prend en argument un pointeur constant vers une structure station_t.
void afficher_station(const station_t *st)
{
  // Affiche le libellé "IP : ".
  printf("IP : ");
  // Appelle la fonction afficher_ip_t pour afficher l'adresse IP de la station.
  afficher_ip_t(&st->ip);
  // Affiche un saut de ligne suivi du libellé "MAC : ".
  printf("\nMAC : ");
  // Appelle la fonction afficher_mac pour afficher l'adresse MAC de la station.
  afficher_mac(&st->mac);
}

// Fonction pour initialiser une structure station_t avec une adresse IP et une adresse MAC à partir de chaînes de caractères.
// Prend en argument un pointeur vers la structure station_t à initialiser,
// et des pointeurs constants vers les chaînes de caractères représentant l'IP et la MAC.
// La logique de retour de cette fonction est spécifique :
// - Elle retourne EXIT_FAILURE si l'initialisation de l'IP via init_ip_address_t réussit.
// - Elle retourne EXIT_FAILURE si l'initialisation de la MAC via init_mac_address_t réussit.
// - Elle retourne EXIT_SUCCESS uniquement si les deux initialisations (IP et MAC) échouent.
int init_station_t(station_t *st, const char *ip, const char *mac)
{
  // Structures temporaires pour stocker les adresses IP et MAC à parser
ip_address_t tmp_ip;
mac_address_t tmp_ma;

// On essaye d'initialiser tmp_ip à partir de la chaîne 'ip'
// Si ça échoue, la fonction retourne une valeur non nulle
if (init_ip_address_t(&tmp_ip, ip))
{
  // Si l'init échoue, tmp_ip est mise à zéro, on l'affecte quand même à la station
  st->ip = tmp_ip;
}
else // Sinon, on considère que l'IP est invalide
{
  // On met manuellement une IP vide dans la station
  st->ip = (ip_address_t){{0, 0, 0, 0}, 0};
  
  // Et on retourne un échec
  return EXIT_FAILURE;
}


  // Tente d'initialiser tmp_ma à partir de la chaîne 'mac'.
  // Le bloc 'if' est exécuté si init_mac_address_t retourne une valeur non nulle (échec).
  if (init_mac_address_t(&tmp_ma, mac))
  {
    // En cas d'échec de init_mac_address_t, tmp_ma est mise à zéro par init_mac_address_t elle-même.
    // La structure st->mac est alors assignée avec cette version mise à zéro de tmp_ma.
    st->mac = tmp_ma;
  }
  else // Le bloc 'else' est exécuté si init_mac_address_t retourne 0 (succès).
  {
    // Si l'initialisation de tmp_ma a réussi, st->mac est explicitement mise à zéro ici.
    // L'initialiseur {{0,0,0,0}} initialise les premiers octets de st->mac.octet à zéro.
    // Pour une structure mac_address_t (6 octets), les octets restants sont implicitement mis à zéro
    // par les règles d'initialisation des agrégats en C, mais cela peut générer des avertissements
    // avec des options de compilation très strictes (ex: -Wmissing-field-initializers).
    st->mac = (mac_address_t){{0, 0, 0, 0}};
    // La fonction retourne EXIT_FAILURE, indiquant un échec global pour init_station_t malgré le succès de init_mac_address_t.
    return EXIT_FAILURE;
  }

  // Cette instruction n'est atteinte que si les deux appels précédents (init_ip_address_t et init_mac_address_t)
  // ont tous les deux retourné une valeur non nulle (c'est-à-dire, ont tous les deux signalé un échec).
  // Dans ce cas, st->ip et st->mac ont été assignées avec les versions mises à zéro de tmp_ip et tmp_ma.
  // La fonction init_station_t retourne alors EXIT_SUCCESS.
  return EXIT_SUCCESS;
}
