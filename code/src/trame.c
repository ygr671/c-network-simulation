#include "include/trame.h"
#include <string.h>

void init_trame_ethernet_t(trame_ethernet_t * trame, mac_address_t * src, mac_address_t * dest)
{
  // Remplissage du préambule (01010101 en binaire)
  for (unsigned short i = 0 ; i < 7 ; i++)
    trame->preambule[i] = 0x55;

  trame->src = *src;
  trame->dest = *dest;
}

void deinit_trame_ethernet_t(trame_ethernet_t *trame)
{
  memset(trame->preambule, 0, sizeof(trame->preambule));
}
