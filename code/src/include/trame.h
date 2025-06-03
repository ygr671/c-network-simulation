#pragma once

#include "mac.h"

#define TAILLE_PREAMBULE 7
#define TAILLE_SFD 1
#define TAILLE_MAC 6
#define TAILLE_TYPE 2
#define TAILLE_MIN_DONNEES 46
#define TAILLE_MAX_DONNEES 1500
#define TAILLE_FCS 4

typedef struct
{
  unsigned char preambule[TAILLE_PREAMBULE]; // Généralement 0x55uint8_t
  unsigned char sfd;                         // Généralement 0xD5
  mac_address_t dest;                        // MAC destination
  mac_address_t src;                         // MAC source
  unsigned short type;           // Type Ethernet (ex: 0x0800 pour IPv4)
  unsigned short taille_donnees; // Taille réelle des données
  unsigned char donnees[TAILLE_MAX_DONNEES];  // Données (payload)
  unsigned char bourrage[TAILLE_MIN_DONNEES]; // Bourrage si <46 octets
  unsigned int fcs;                           // CRC (factice ici)
} trame_t;

void init_trame_t(trame_t *trame, mac_address_t *src, mac_address_t *dest);

void deinit_trame_ethernet_t(trame_t *trame);
