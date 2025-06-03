#!/bin/bash

# Programme qui exécute le formatage automatique du code selon le fichier .clang-format
# Actuellement bugué (à fix)

echo "Formatage en cours"
find . -type f \( -name "*.c" -o -name "*.h" \) | xargs clang-format -i
echo "Formatage terminé"
