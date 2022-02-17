#  Pour tester les taux de convergence:

Commentez les lignes 146 à 155 et décommentez les lignes 135 à 143 dans *PbEF2d.hpp*

``make test && ./test``

# Pour tester la simulation sur les appartements:

Décommentez les lignes 146 à 155 et commentez les lignes 135 à 143 dans *PbEF2d.hpp*

Dans main.cpp vous pouvez choisir entre **"appart.msh"** et **"appart2.msh"**

``make && ./main``
