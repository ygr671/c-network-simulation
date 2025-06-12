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
  charger_configuration(&rs, "configurations/lan1.lan");

  // Affichage d'un titre pour la section de la configuration initiale
  printf("\n=== Configuration initiale du réseau ===\n");
  // Affichage de l'état actuel du réseau (liste des équipements et des liens)
  afficher_reseau_t(&rs);

  // Test 1: Trame Broadcast
  // Ce bloc de code simule l'envoi d'une trame de diffusion (broadcast) sur le réseau.
  // Une trame de broadcast est destinée à tous les équipements sur le même segment de réseau.
  printf("\n=== Test 1: Envoi d'une trame broadcast ===\n");
  // Déclaration d'une variable pour stocker la trame Ethernet
  trame_ethernet_t trame;
  // Définition de l'adresse MAC source de la trame
  mac_address_t src = {{0x00, 0x1A, 0x2B, 0x3C, 0x4D, 0x5E}};
  // Définition de l'adresse MAC destination comme étant l'adresse de broadcast (tous les bits à 1)
  mac_address_t dest = {{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}}; // Broadcast

  // Initialisation des champs de la trame Ethernet (adresses MAC, type, données)
  init_trame_ethernet(&trame, &src, &dest);
  // Définition du champ "type" de la trame. htons convertit 0x0800 (IPv4) de l'ordre des octets de l'hôte vers l'ordre des octets du réseau.
  trame.type = htons(0x0800); // IPv4
  // Définition du contenu (payload) de la trame
  const char *message = "Message broadcast de test";
  // Copie du message dans le champ de données de la trame
  memcpy(trame.donnees, message, strlen(message));
  // Mise à jour de la taille des données dans la trame
  trame.taille_donnees = strlen(message);

  // Affichage d'informations sur la trame envoyée
  printf("\nEnvoi de la trame broadcast depuis port externe 1:\n");
  printf("------------------------------------------------\n");
  // Affichage détaillé de la trame
  afficher_trame(&trame);
  printf("\nPropagation de la trame dans le réseau...\n");
  // Simulation du traitement de la trame par un switch (ici, le switch avec l'ID 0) sur un port d'entrée donné (ici, port 1)
  traiter_trame_switch(&rs, 0, &trame, 1);

  // Test 2: Trame Unicast vers Station 1
  printf("\n=== Test 2: Envoi d'une trame unicast vers Station 1 ===\n");
  mac_address_t dest_station1 = {{0x54, 0xD6, 0xA6, 0x82, 0xC5, 0x23}};
  init_trame_ethernet(&trame, &src, &dest_station1);
  trame.type = htons(0x0800); // IPv4
  const char *message2 = "Message uniquement pour Station 1";
  memcpy(trame.donnees, message2, strlen(message2));
  trame.taille_donnees = strlen(message2);

  // Affichage d'informations sur la trame unicast envoyée
  printf("\nEnvoi de la trame unicast depuis port externe 1:\n");
  printf("---------------------------------------------\n");
  // Affichage détaillé de la trame
  afficher_trame(&trame);
  printf("\nPropagation de la trame dans le réseau...\n");
  // Simulation du traitement de cette trame unicast par le même switch (ID 0) sur le même port d'entrée (port 1)
  // Le switch devrait utiliser sa table de commutation pour acheminer la trame si l'adresse MAC destination est connue.
  traiter_trame_switch(&rs, 0, &trame, 1);

  // Test 3: Vérification de l'apprentissage
  // Ce bloc vérifie l'état de la table de commutation du switch après les envois de trames.
  // Les switchs apprennent les adresses MAC sources des trames qu'ils reçoivent et les associent aux ports d'entrée.
  printf("\n=== Test 3: Vérification de la table de commutation ===\n");
  printf("État de la table de commutation du switch 0:\n");
  printf("------------------------------------------\n");
  // Affichage de la table de commutation du switch avec l'ID 0.
  // On s'attend à y trouver l'adresse MAC `src` associée au port 1, car des trames avec cette source ont été traitées.
  afficher_table_commutation(&rs.equipements[0].contenu.sw.tc);

  // Libération de toute la mémoire allouée dynamiquement pour la structure du réseau
  deinit_reseau_t(&rs);
  // Retourne EXIT_SUCCESS pour indiquer que le programme s'est terminé sans erreur.
  return EXIT_SUCCESS;
}