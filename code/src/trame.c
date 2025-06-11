#include "include/trame.h"
#include <arpa/inet.h> // Pour htons()
#include <stdio.h>
#include <string.h>

void init_trame_ethernet(trame_ethernet_t *trame, const mac_address_t *src,
                         const mac_address_t *dest)
{
  memcpy(&trame->src, src, sizeof(mac_address_t));
  memcpy(&trame->dest, dest, sizeof(mac_address_t));
  trame->type = 0;
  trame->taille_donnees = 0;
  memset(trame->donnees, 0, TAILLE_MAX_DONNEES);
}

void afficher_trame(const trame_ethernet_t *trame)
{
  printf("=== Trame Ethernet Simplifiée ===\n");

  printf("Destination: ");
  for (int i = 0; i < 6; i++)
  {
    printf("%02X", trame->dest.octet[i]);
    if (i < 5)
      printf(":");
  }

  printf("\nSource:      ");
  for (int i = 0; i < 6; i++)
  {
    printf("%02X", trame->src.octet[i]);
    if (i < 5)
      printf(":");
  }

  printf("\nType:        0x%04X", ntohs(trame->type));
  printf("\nTaille données: %u octets\n", trame->taille_donnees);
}

void afficher_trame_hex(const trame_ethernet_t *trame)
{
  // Afficher l'en-tête (dest + src + type)
  printf("En-tête (14 octets):\n");

  // MAC destination
  for (int i = 0; i < 6; i++)
  {
    printf("%02X ", trame->dest.octet[i]);
  }

  // MAC source
  for (int i = 0; i < 6; i++)
  {
    printf("%02X ", trame->src.octet[i]);
  }

  // Type
  printf("%02X %02X", (ntohs(trame->type) >> 8) & 0xFF,
         ntohs(trame->type) & 0xFF);

  // Données
  printf("\n\nDonnées (%d octets):\n", trame->taille_donnees);
  for (int i = 0; i < trame->taille_donnees; i++)
  {
    printf("%02X ", trame->donnees[i]);
    if ((i + 1) % 16 == 0)
      printf("\n");
  }
  printf("\n");
}
