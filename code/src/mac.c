// Inclusion du fichier d'en-tête correspondant (mac.h) qui contient la définition de la structure mac_address_t
#include "include/mac.h"
// Inclusion de la bibliothèque standard pour les entrées/sorties (printf, fprintf, stderr)
#include <stdio.h>
// Inclusion de la bibliothèque standard pour les fonctions utilitaires (EXIT_FAILURE, EXIT_SUCCESS)
#include <stdlib.h>

// Fonction pour afficher une adresse MAC au format lisible par l'homme (ex: 00:1A:2B:3C:4D:5E).
// Prend en argument un pointeur constant vers une structure mac_address_t.
void afficher_mac(const mac_address_t *ma)
{
  // Boucle pour parcourir les 6 octets de l'adresse MAC.
  for (unsigned int i = 0; i < 6; i++)
  {
    // Affiche l'octet courant en format hexadécimal sur deux chiffres, avec un zéro en préfixe si nécessaire (ex: 0A, FF).
    printf("%02X", ma->octet[i]);
    // Si ce n'est pas le dernier octet, affiche un ":" comme séparateur.
    if (i < 5)
      printf(":");
  }
}

// Fonction pour initialiser une structure mac_address_t à partir d'une chaîne de caractères.
// Prend en argument un pointeur vers la structure mac_address_t à initialiser et une chaîne de caractères représentant l'adresse MAC (ex: "00:1A:2B:3C:4D:5E").
// Retourne EXIT_SUCCESS en cas de succès, EXIT_FAILURE en cas d'erreur.
int init_mac_address_t(mac_address_t *ma, const char *mac_char)
{
  // Déclaration d'une structure temporaire pour stocker les valeurs parsées.
  mac_address_t tmp;
  // Tente de parser la chaîne de caractères `mac_char` pour extraire les 6 octets de l'adresse MAC.
  // Le format attendu est "XX:XX:XX:XX:XX:XX" où X est un chiffre hexadécimal.
  // `sscanf` retourne le nombre d'éléments correctement assignés. `%hhx` est utilisé pour lire un `unsigned char` en hexadécimal.
  if (sscanf(mac_char, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &tmp.octet[0],
             &tmp.octet[1], &tmp.octet[2], &tmp.octet[3], &tmp.octet[4],
             &tmp.octet[5]) != 6)
  {
    // Si le parsing échoue (pas 6 éléments assignés), affiche un message d'erreur sur la sortie d'erreur standard.
    fprintf(stderr,
            "Erreur dans le sscanf() de init_mac_address_t() : mise à 0\n");
    // En cas d'échec, initialise tous les octets de l'adresse MAC `ma` à 0.
    for (size_t i = 0; i < 6; i++)
    {
      ma->octet[i] = 0;
    }
    // Retourne un code d'échec.
    return EXIT_FAILURE;
  }
  // Si le parsing a réussi, copie les octets de la structure temporaire `tmp`
  // vers la structure `ma` passée en argument.
  for (size_t i = 0; i < 6; i++)
  {
    ma->octet[i] = tmp.octet[i];
  }
  // Retourne un code de succès.
  return EXIT_SUCCESS;
}
