#include "include/config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAILLE_BUFFER 256

void charger_configuration(reseau_t *rs, const char *path)
{
  // Charger les données contenues dans le fichier de configuration se situant
  // dans path dans le réseau passé en argument
  FILE *fichier = fopen(path, "r");

  if (fichier == NULL)
{
  // Affiche un message d'erreur si le fichier ne peut pas être ouvert
  perror("fopen (charger_configuration)");
  // Libère la mémoire allouée au réseau 
  deinit_reseau_t(rs);
  // Arrête le programme car on ne peut pas continuer sans le fichier
  exit(EXIT_FAILURE);
}


  char ligne_buffer[TAILLE_BUFFER];

  // Lit une ligne depuis le fichier et la stocke dans ligne_buffer si tout se passe bien
  if (fgets(ligne_buffer, sizeof(ligne_buffer), fichier) != NULL)
  {
    // Parsing du nombre d'équipements et du nombre de liens
    // Analyse la ligne pour extraire le nombre d'équipements et le nombre de liens attendus
    if (sscanf(ligne_buffer, "%hu %hu", &rs->nb_equipements, &rs->nb_liens) != 2)
    {
      // Si le format n'est pas respecté, on affiche une erreur et on quitte
      fprintf(stderr, "Erreur : fichier de configuration malformé\n");
      deinit_reseau_t(rs);
      fclose(fichier);
      exit(EXIT_FAILURE);
    }

    // Boucle pour lire et ajouter tous les équipements (stations et switchs)
    for (size_t i = 0; i < rs->nb_equipements; i++)
    {
      // On lit une ligne contenant les infos d’un équipement
      if (fgets(ligne_buffer, sizeof(ligne_buffer), fichier) != NULL)
      {
        // On ajoute l’équipement au réseau à partir de la ligne lu recemenntt
        ajouter_equipement_t(rs, ligne_buffer);
      }
      else
      {
        // Si une ligne est manquante, on affiche une erreur et on quitte dans le cas ou il y a un nombre d'équipements incorrect allez kingkong
        fprintf(stderr, "Erreur : nombre de liens incorrect dans le fichier\n");
        deinit_reseau_t(rs);
        fclose(fichier);
        exit(EXIT_FAILURE);
      }
    }


    // Ajout des liens
    // Boucle pour lire et ajouter tous les liens entre les équipements
    for (size_t i = 0; i < rs->nb_liens; i++)
    {
      // Lecture d'une ligne contenant un lien
      if (fgets(ligne_buffer, sizeof(ligne_buffer), fichier) != NULL)
      {
        // Ajout du lien dans le réseau à partir des infos lues
        ajouter_lien_t(rs, ligne_buffer);
      }
      else
      {
        // Si une ligne est manquante, on affiche une erreur et on quitte
        fprintf(stderr, "Erreur : nombre de liens incorrect dans le fichier\n");
        deinit_reseau_t(rs);
        fclose(fichier);
        exit(EXIT_FAILURE);
      }
    }
  }

  fclose(fichier);
}
