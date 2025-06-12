// Inclusion du fichier d'en-tête correspondant (config.h)
#include "include/config.h"
// Inclusion de la bibliothèque standard pour les entrées/sorties (FILE, fopen, fclose, fgets, printf, perror)
#include <stdio.h>
// Inclusion de la bibliothèque standard pour les fonctions utilitaires (exit, EXIT_FAILURE)
#include <stdlib.h>
// Inclusion de la bibliothèque standard pour la manipulation de chaînes de caractères (non utilisée directement ici, mais souvent utile avec les fichiers)
#include <string.h>

// Définit la taille maximale du tampon utilisé pour lire les lignes du fichier de configuration.
#define TAILLE_BUFFER 256

// Fonction pour charger la configuration du réseau à partir d'un fichier.
// Prend en argument un pointeur vers la structure reseau_t à remplir et le chemin vers le fichier de configuration.
void charger_configuration(reseau_t *rs, const char *path)
{
  // Déclaration d'un pointeur de fichier pour manipuler le fichier de configuration.
  FILE *fichier = fopen(path, "r");

  // Vérifie si l'ouverture du fichier a échoué.
  if (fichier == NULL)
  {
    // Affiche un message d'erreur système décrivant l'échec de fopen.
    perror("fopen (charger_configuration)");
    // Libère les ressources potentiellement allouées dans la structure réseau avant de quitter.
    deinit_reseau_t(rs);
    // Termine le programme avec un code d'erreur.
    exit(EXIT_FAILURE);
  }


  // Déclaration d'un tampon pour stocker chaque ligne lue depuis le fichier.
  char ligne_buffer[TAILLE_BUFFER];


  // Tente de lire la première ligne du fichier, qui doit contenir le nombre d'équipements et de liens.
  if (fgets(ligne_buffer, sizeof(ligne_buffer), fichier) != NULL)
  {
    // Tente de parser la première ligne pour extraire le nombre d'équipements et le nombre de liens.
    // sscanf retourne le nombre d'éléments assignés avec succès.
    if (sscanf(ligne_buffer, "%hu %hu", &rs->nb_equipements, &rs->nb_liens) !=
        2)
    {
      // Si le parsing échoue (ne trouve pas 2 nombres), le fichier est malformé.

      fprintf(stderr, "Erreur : fichier de configuration malformé\n");
      // Libère les ressources réseau.
      deinit_reseau_t(rs);
      // Ferme le fichier.
      fclose(fichier);
      // Termine le programme.
      exit(EXIT_FAILURE);
    }


    // Boucle pour lire et ajouter chaque équipement.
    // Le nombre d'itérations est déterminé par rs->nb_equipements lu précédemment.
    for (size_t i = 0; i < rs->nb_equipements; i++)
    {
      // Tente de lire la ligne suivante (description de l'équipement).
      if (fgets(ligne_buffer, sizeof(ligne_buffer), fichier) != NULL)
      {
        // Appelle la fonction pour ajouter l'équipement au réseau à partir de la ligne lue.

        ajouter_equipement_t(rs, ligne_buffer);
      }
      else
      {

        // Si fgets échoue, cela signifie qu'il y a moins de lignes d'équipement que spécifié.
        fprintf(stderr, "Erreur : nombre d'équipements incorrect ou fin de fichier inattendue en lisant les équipements\n");

        deinit_reseau_t(rs);
        fclose(fichier);
        exit(EXIT_FAILURE);
      }
    }

    // Boucle pour lire et ajouter chaque lien.
    // Le nombre d'itérations est déterminé par rs->nb_liens lu précédemment.
    for (size_t i = 0; i < rs->nb_liens; i++)
    {
      // Tente de lire la ligne suivante (description du lien).
      if (fgets(ligne_buffer, sizeof(ligne_buffer), fichier) != NULL)
      {
        // Appelle la fonction pour ajouter le lien au réseau à partir de la ligne lue.

        ajouter_lien_t(rs, ligne_buffer);
      }
      else
      {

        // Si fgets échoue, cela signifie qu'il y a moins de lignes de lien que spécifié.
        fprintf(stderr, "Erreur : nombre de liens incorrect ou fin de fichier inattendue en lisant les liens\n");

        deinit_reseau_t(rs);
        fclose(fichier);
        exit(EXIT_FAILURE);
      }
    }
  }

  // Ferme le fichier de configuration après avoir lu toutes les informations nécessaires.
  fclose(fichier);
}
