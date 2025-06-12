#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/config.h"
#include "include/reseau.h"
#include "include/trame.h"

int main(void)
{
  reseau_t rs;
  init_reseau_t(&rs);
  charger_configuration(&rs, "configurations/lan1.lan");

  printf("\n=== Configuration initiale du réseau ===\n");
  afficher_reseau_t(&rs);

  // Test 1: Trame Broadcast
  printf("\n=== Test 1: Envoi d'une trame broadcast ===\n");
  trame_ethernet_t trame;
  mac_address_t src = {{0x00, 0x1A, 0x2B, 0x3C, 0x4D, 0x5E}};
  mac_address_t dest = {{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}}; // Broadcast

  init_trame_ethernet(&trame, &src, &dest);
  trame.type = htons(0x0800); // IPv4
  const char *message = "Message broadcast de test";
  memcpy(trame.donnees, message, strlen(message));
  trame.taille_donnees = strlen(message);

  printf("\nEnvoi de la trame broadcast depuis port externe 1:\n");
  printf("------------------------------------------------\n");
  afficher_trame(&trame);
  printf("\nPropagation de la trame dans le réseau...\n");
  traiter_trame_switch(&rs, 0, &trame, 1);

  // Test 2: Trame Unicast vers Station 1
  printf("\n=== Test 2: Envoi d'une trame unicast vers Station 1 ===\n");
  mac_address_t dest_station1 = {{0x54, 0xD6, 0xA6, 0x82, 0xC5, 0x23}};
  init_trame_ethernet(&trame, &src, &dest_station1);
  trame.type = htons(0x0800); // IPv4
  const char *message2 = "Message uniquement pour Station 1";
  memcpy(trame.donnees, message2, strlen(message2));
  trame.taille_donnees = strlen(message2);

  printf("\nEnvoi de la trame unicast depuis port externe 1:\n");
  printf("---------------------------------------------\n");
  afficher_trame(&trame);
  printf("\nPropagation de la trame dans le réseau...\n");
  traiter_trame_switch(&rs, 0, &trame, 1);

  // Test 3: Vérification de l'apprentissage
  printf("\n=== Test 3: Vérification de la table de commutation ===\n");
  printf("État de la table de commutation du switch 0:\n");
  printf("------------------------------------------\n");
  afficher_table_commutation(&rs.equipements[0].contenu.sw.tc);

  deinit_reseau_t(&rs);
  return EXIT_SUCCESS;
}