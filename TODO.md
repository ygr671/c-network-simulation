# TODO

   - [x] `init_reseau_t(reseau_t * rs)`
   - [x] `deinit_reseau_t(reseau_t * rs)`
   - [x] `ajouter_station_t(reseau_t * rs, station_t st)`
   - [x] `ajouter_lien_t(reseau_t * rs, lien_t lien)`
   - [x] `afficher_reseau(const reseau_t * rs)`
   - [ ] Utiliser `init_switch_t` dans `config.c` au lieu de manuellement initialiser le `switch_t`.
   - [ ] Les fonctions `ajouter_station_t` et `ajouter_lien_t` doivent mettre à jour les compteurs `nb_stations` et `nb_liens` du réseau.
   - [ ] Ajouter vérification que la capacité n’est pas dépassée avant ajout de stations ou de liens.
   - [ ] Compléter `ajouter_entree_table_commutation` (incomplète : partie après le `*= 2;` absente).

Ajouter une macro de debug légère pour faciliter le suivi de l’exécution.
