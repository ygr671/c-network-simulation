#include "include/trame.h"
#include <arpa/inet.h> // Pour htons()
#include <stdio.h>
#include <string.h>

void init_trame_ethernet(trame_ethernet_t *trame, const mac_address_t *src,
                         const mac_address_t *dest)
{
  //la fonction memcpy copie les octets de l'adresse MAC source et destination
  //dans la structure trame_ethernet_t, en utilisant sizeof(mac_address_t) pour
  //déterminer la taille exacte de l'adresse MAC.
  memcpy(&trame->src, src, sizeof(mac_address_t));
  memcpy(&trame->dest, dest, sizeof(mac_address_t));
  trame->type = 0;
  trame->taille_donnees = 0;
  //memset est utilisée pour initialiser le tableau de données à zéro.
  //Cela garantit que toutes les données de la trame sont initialisées à zéro,
  memset(trame->donnees, 0, TAILLE_MAX_DONNEES);
}

void afficher_trame(const trame_ethernet_t *trame)
{
  printf("Adresse mac destination : ");
  afficher_mac(&trame->dest);
  printf("\nAdresse mac source : ");        // adresse mac dest
  afficher_mac(&trame->src);                // adresse mac src
  printf("\ntype : 0x%04x\n", trame->type); // adresse type
  printf("\nTaille données: %u octets\n", trame->taille_donnees);
  for (int i = 0; i < trame->taille_donnees; i++)
  {
    printf("%02X ", trame->donnees[i]);
    if ((i + 1) % 16 == 0)
      printf("\n");
  }
  printf("\n");
}

void afficher_trame_hex(const trame_ethernet_t *trame)
{
  // Afficher l'en-tête (dest + src + type)
  printf("En-tête (14 octets):\n");

  // MAC destination
  for (int i = 0; i < 6; i++)
  {
    //%02X affiche chaque octet de l'adresse MAC destination en hexadécimal
    // avec deux chiffres, en ajoutant un zéro devant si nécessaire.
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
  for (int i = 0; i < trame->taille_donnees; i++)
  {
    printf("%02X ", trame->donnees[i]);

    // Affiche les données en hexadécimal, 16 octets par ligne
    // %02X affiche chaque octet en hexadécimal avec deux chiffres.
    if ((i + 1) % 16 == 0)
      printf("\n");
  }
  printf("\n");
}