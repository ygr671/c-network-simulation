# SAÉ Reseau S21-M23

## Membres du groupe
- [Omar Farouk LASFAR](https://github.com/olasfar)
- [Muhammed Emin ERDAL](https://github.com/ygr671)
- [Ramzan ABDOULAEV](https://github.com/BorZa-45)

## Objectifs
- Représenter et simuler un réseau configuré dans un fichier chargé en amont
- Faire transiter des messages entre les différents équipements d'un réseau
- Faire une simulation du protocole Ethernet et à terme, du protocole STP (Spanning Tree Protocol)

## Objectifs atteints
- [x] : Création de toutes les structures de données nécessaires pour la représentation du réseau
    - NB : Il manque les éléments pour la transmission, le stockage et la simulation de messages + protocole STP
- [x] : Chargement du fichier de configuration
    - NB : Il manque l'ajout des liens dans les tables de commutation des switches reliés aux équipements correspondants
    - EDIT 03-06-2025 : la création du réseau représentant l'état initial à l'instant t = 0, la table de commutation est par définition vide car aucune donnée n'y transite en premier lieu.
