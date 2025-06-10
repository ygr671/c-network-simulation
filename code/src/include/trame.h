#pragma once

#include "mac.h"
#include <stdint.h>

#define TAILLE_MIN_DONNEES 46
#define TAILLE_MAX_DONNEES 1500

typedef struct {
    mac_address_t dest;                   // MAC destination (6 octets)
    mac_address_t src;                    // MAC source (6 octets)
    uint16_t type;                        // Type Ethernet (2 octets)
    uint8_t donnees[TAILLE_MAX_DONNEES];  // Payload (jusqu'à 1500 octets)
    uint16_t taille_donnees;              // Taille réelle des données
} trame_ethernet_t;

/**
 * Initialise une trame Ethernet
 * trame     Pointeur vers la trame à initialiser
 * src       Adresse MAC source
 * dest      Adresse MAC destination
 */
void init_trame_ethernet(trame_ethernet_t *trame, 
                        const mac_address_t *src, 
                        const mac_address_t *dest);

/**
 * Affiche la trame en format lisible
 * trame     La trame à afficher
 */
void afficher_trame(const trame_ethernet_t *trame);

/**
 * Affiche le contenu brut de la trame en hexadécimal
 * trame     La trame à afficher
 */
void afficher_trame_hex(const trame_ethernet_t *trame);
