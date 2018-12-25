Fondus enchainés avec la SDL
============================

Introduction
------------

Ce petit article présente une méthode pour faire une apparition
progressive d'image ainsi qu'un enchaînement de deux images (un fondu
enchaîné)

Avant de commencer la lecture de celui-ci, je vous conseille de lire
« Première application en C avec la SDL » sur ce même site, qui explique
les bases de l'initialisation d'un programme en SDL.

L'article est accompagné de deux sources. Le premier fait apparaître et
disparaître une image progressivement, le second enchaîne l'affichage
progressif de deux images.

Afin de les compiler, vous devez avoir la librairie `SDL` installée et
savoir comment compiler un programme avec cette librairie. Si vous
utilisez gcc sous un environnement Linux/Bash,
vous compilerez très simplement
chaque programme de la manière suivante :
``gcc -o fade1 fade1.c `pkg-config sdl --cflags -libs` ``. Et
de même pour `fade2.c`.

Si vous utilisez `fish`: `gcc -o fade1 fade1.c (pkg-config sdl --cflags --libs | string split " ")`


Apparition d'une image
----------------------

Nous gardons du squelette d'une application SDL l'initialisation avec
`SDL_Init()` et la gestion des évènements avec la boucle
`while (!quitProgram) {}` qui gère le traitement des évènements avec
`SDL_PollEvent()`. Pour plus de détails, voir `01-sdl_premiere_application`. 

L'affichage est initialisé avec `SDL_SetVideoMode()` en double buffer.
La boucle se termine donc par la commande `SDL_Flip()`.

Les images sont chargées avec `SDL_LoadBMP()`. Faires attention au lancement
du programme de placer le répertoire de travail là où sont les images. Dans
le cas contraire, le programme sortira avec le code 3.

### Afficher l'image progressivement

Le cœur du problème. Nous allons utiliser pour cela la fonction
`SDL_SetAlpha()`. L'alpha gère l'opacité d'une surface. Avec une valeur
de `0` (zéro, ou encore `SDL_ALPHA_TRANSPARENT`), l'image est
transparente ; avec une valeur égale à `SDL_ALPHA_OPAQUE` (255), l'image
est opaque, comme son nom l'indique.

Les trois paramètres de `SDL_SetAlpha()` sont :

- `SDL_Surface *surface` : la surface sur laquelle nous modifions le
  niveau de transparence ;
- `Uint32 flag` : il y a plusieurs possibilités ici, mais nous ne nous
  occuperons qu'une d'une valeur dans cet article, `SDL_SRCALPHA`, qui
  nous permet de selectionner la transparence de la surface ;
- Uint8 alpha` : la valeur de transparence elle-même.

Une fois mis en place la transparence, on utilise `SDL_BlitSurface()`
normalement. Comme nous affichons une image en plein écran, les deux
`SDL_Rect *` sont à NULL.

Grâce à deux variables, `alpha` et `delta_alpha`, on fait varier la
transparence. On commence à zéro et on ajoute à chaque tour de boucle la
valeur de `delta_alpha`. Lorsque l'on atteint une borne, on prend
l'opposé de `delta_alpha` pour nouvelle valeur. La valeur d'`alpha`
oscille donc entre 0 et 255 jusqu'à arrêt du programme.

**Attention !** il faut prendre garde à effacer l'écran avant d'afficher
l'image transparente. Je vous propose d'enlever la ligne
`SDL_FillRect(sdlMainScreen,NULL,0);`. Que se passe-t-il ? L'image
devient opaque et... le reste. En effet, une fois l'image opaque
affichée, on affiche par dessus des images transparentes, ce qui ne
change rien.

Voilà donc une image qui s'affiche progessivement.

### Fondu enchaîné

Nous allons maintenant légèrement modifier le programme `fade1.c` pour
donner `fade2.c`. Peu de modifications. Nous commençons bien entendu par
charger deux images plutôt qu'une, que nous stockons dans deux surfaces.
Le code d'oscillation de la valeur d'`alpha` ne change pas.

Dernière petite modification, plutôt que d'effacer l'écran, on affiche
la première image de manière opaque et on y ajoute une surface plus ou
moins transparente. Et c'est tout, cela suffit. Si la valeur d'`alpha`
est à 0 (transparent), alors la première image est affichée, alors que
si la valeur d'`alpha` est à 255 (opaque), la deuxième image recouvre
complètement la première.

Conclusion
----------

Nous avons vu une façon assez simple de faire un fondu et un fondu
enchaîné avec la SDL grâce à la fonction `SDL_SetAlpha()`. À vous de
jouer.

Crédits 
--------

- Tutorial écrit par [Mokona](https://www.puupuu.org) écrit pour Prografix.
- Date de publication : 4 juin 2003
