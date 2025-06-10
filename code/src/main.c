#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

// Headers

#include "include/config.h"
#include "include/reseau.h"
#include "include/trame.h"

int main(void)
{
  reseau_t rs;

  init_reseau_t(&rs);

  charger_configuration(&rs, "configurations/lan1.lan");

  printf("Configuration chargée !\n");

  afficher_reseau_t(&rs);

  trame_ethernet_t trame;
  mac_address_t src = {{0x00, 0x1A, 0x2B, 0x3C, 0x4D, 0x5E}};
  mac_address_t dest = {{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}}; // Broadcast
    
  init_trame_ethernet(&trame, &src, &dest);
  trame.type = htons(0x0800); // IPv4
    
  // Ajouter des données de test
  const char *message = "Hello Ethernet!";
  memcpy(trame.donnees, message, strlen(message));
  trame.taille_donnees = strlen(message);
    
  afficher_trame(&trame);
  printf("\n");
  afficher_trame_hex(&trame);

  deinit_reseau_t(&rs);

  return EXIT_SUCCESS;
}
