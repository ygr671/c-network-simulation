// Inclusion du fichier d'en-tête correspondant (ip.h) qui contient la définition de la structure ip_address_t
#include "include/ip.h"
// Inclusion de la bibliothèque standard pour les entrées/sorties (printf, fprintf, stderr)
#include <stdio.h>
// Inclusion de la bibliothèque standard pour les fonctions utilitaires (EXIT_FAILURE, EXIT_SUCCESS)
#include <stdlib.h>

// Fonction pour afficher une adresse IP au format lisible par l'homme.
// Prend en argument un pointeur constant vers une structure ip_address_t.
void afficher_ip_t(const ip_address_t *ip)
{
  // Boucle pour parcourir les 4 octets de l'adresse IP.
  for (unsigned int i = 0; i < 4; i++)
  {
    // Affiche l'octet courant sous forme d'entier décimal.
    printf("%d", ip->paquet[i]);
    // Si ce n'est pas le dernier octet, affiche un point "." comme séparateur.
    if (i < 3)
      printf(".");
  }
  // Affiche le masque de sous-réseau après un "/" et un saut de ligne.
  printf("/%d\n", ip->masque);
}

// Fonction pour initialiser une structure ip_address_t à partir d'une chaîne de caractères.
// Prend en argument un pointeur vers la structure ip_address_t à initialiser et une chaîne de caractères représentant l'adresse IP (ex: "192.168.1.1/24") <- c' est juste exemlple.
// Retourne EXIT_SUCCESS en cas de succès, EXIT_FAILURE en cas d'erreur.
int init_ip_address_t(ip_address_t *ip, const char *ip_char)
{
  // Déclaration d'une structure temporaire pour stocker les valeurs parsées.
  ip_address_t tmp;
  // Tente de parser la chaîne de caractères `ip_char` pour extraire les 4 octets de l'IP et le masque.
  // Le format attendu est "o1.o2.o3.o4/masque".
  // `sscanf` retourne le nombre d'éléments correctement assignés.
  if (sscanf(ip_char, "%hhu.%hhu.%hhu.%hhu/%hhu", &tmp.paquet[0],
             &tmp.paquet[1], &tmp.paquet[2], &tmp.paquet[3], &tmp.masque) != 5)

  {
    // Si le parsing échoue (pas 5 éléments assignés), affiche un message d'erreur sur la sortie d'erreur standard.
    fprintf(stderr,
            "Erreur dans le sscanf() de init_ip_address_t() : mise à 0\n");
    // En cas d'échec, initialise tous les octets de l'adresse IP à 0.
    for (size_t i = 0; i < 4; i++)
    {
      ip->paquet[i] = 0;
    }
    // Initialise le masque à 0.
    ip->masque = 0;

    // Retourne un code d'échec.
    return EXIT_FAILURE;
  }
  // Vérifie si la valeur du masque est valide (entre 1 et 32 inclus).
  if (tmp.masque > 32 || tmp.masque < 1)
  {
    // Si le masque n'est pas valide, affiche un message d'erreur.
    fprintf(stderr, "Erreur : le masque doit être valide, mise à 0\n");
    // En cas d'échec, initialise tous les octets de l'adresse IP à 0.
    for (size_t i = 0; i < 4; i++)
    {
      ip->paquet[i] = 0;
    }
    // Initialise le masque à 0.
    ip->masque = 0;
    // Retourne un code d'échec.
    return EXIT_FAILURE;
  }

  // Si le parsing et la validation du masque ont réussi, copie les valeurs de la structure temporaire `tmp`
  // vers la structure `ip` passée en argument.
  for (size_t i = 0; i < 4; i++)
  {
    ip->paquet[i] = tmp.paquet[i];
  }
  ip->masque = tmp.masque;

  // Retourne un code de succès.
  return EXIT_SUCCESS;
}
