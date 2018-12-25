Première application avec SDL
=============================

Introduction
------------

La librairie **SDL** ([Simple DirectMedia
Layer](http://www.libsdl.org/))
est un moyen pratique d'accéder aux ressources multimédia d'un
ordinateur tout en restant portable. La SDL permet des accès au système
graphique, au son, permet de gérer les évènements et d'autres choses
encore.

Dans ce tutoriel, nous allons créer une première application graphique
en **C** utilisant la SDL.

Ce tutoral nécessite un minimum de connaissances en C (syntaxe générale,
conversion de types, structures et unions, pointeurs).

Le programme 
-------------

Pour cette première approche de la SDL, nous allons afficher une fenêtre
à l'écran. Dans cette fenêtre, nous afficherons un serpentin de points
suivant les déplacements de la souris. Nous verrons ainsi comment
initialiser le système graphique de la SDL et comment gérer les
évènements souris.

L'architecture du programme est très simple : une boucle principale
attend les évènements et les interpréte lorsqu'ils arrivent. À chaque
tour de boucle, la fonction d'affichage est appelée. La fonction
d'affichage se charge d'effacer l'écran et d'afficher les vingt
dernières positions prises par la souris.

Initialisation
--------------

Au début du programme, on trouve les `#include` nécessaires. Mise à
part la librairie standard (`stdlib.h`) et les types booléens
(`stdbool.h`), on a besoin de `SDL.h` afin d'accéder aux prototypes de
la SDL.

```C
int main(int argc, char** argv)

{
 SDL_Surface* screen = sdl_init();
 main\_loop(screen);
 return 0;
}
```

Le point d’entrée d’un programme en C est la fonction main(). Celle-ci
se décompose en deux étapes : l’initialisation du programme, qui
renverra un pointeur vers la surface d’affichage de l’écran, puis la
boucle principale.

Avant toute chose, SDL doit être initialisée avec les composants dont
nous aurons besoin. Dans ce programme, nous avons besoin de la partie
vidéo : `SDL_Init(SDL_INIT_VIDEO)`. Si la valeur de retour est
inférieure à zéro, il y a eu un problème et le programme s'arrête.

Le `atexit(SDL\_Quit);` est une sécurité. La fonction d'arrêt de la SDL,
`SDL\_Quit()`, sera appelée à la sortie du programme.

Nous n'entrons pas dans les subtilités des résolutions d'écran. Il est
en effet possible d'interroger le matériel afin de savoir quels modes
sont disponibles. Dans notre exemple, nous demandons directement le mode
que nous voulons : 640x480 en 16 bits. SDL fera au mieux et renverra un
pointeur sur la surface représentant l'écran (structure `SDL_Surface*`).
Si ce pointeur est nul, quelque chose s'est passé de
travers, et on s’arrête là.

La fonction `SDL_SetVideoMode()` accepte des paramétrés en plus de la
résolution voulue. Ici, nous lui passons `SDL_SWSURFACE` et
`SDL_DOUBLEBUF`. `SDL_SWSURFACE` précise que la mémoire vidéo que l'on
demande sera allouée dans la mémoire système (par opposition à la
mémoire vidéo de la carte). C'est souvent plus lent, mais c'est le plus
simple et notre programme est un début. `SDL_DOUBLEBUF` précise que
nous voulons travailler en double buffer.

En mode double buffer, le dessin se fait sur une buffer caché. Lorsque
l'affichage est terminé, on échange le buffer affiché et le buffer
caché. Ainsi, ce qu'on a affiché s'affiche d'un coup et on écrit dans
l'ancien buffer affiché. Et ainsi de suite. L'intérêt principal est de
ne pas voir l'affichage se réaliser directement, c'est peu esthétique.

`SDL\_WM\_SetCaption()` sert à déterminer le titre de la fenêtre (WM
comme Window Manager).

La fonction a pour valeur de retour la surface d’affichage que l’on a
initialisé avec `SDL_SetVideoMode()`.

Cette valeur sera a son tour passée à la boucle principale.

```C
enum
{
    MAX_MOUSE_POS = 20
};

struct MousePositions
{
    uint16_t mouse_x[MAX_MOUSE_POS];
    uint16_t mouse_y[MAX_MOUSE_POS];
    uint16_t mouse_pos;
    uint16_t size;
} mouse_positions = { {0}, {0}, 0, MAX_MOUSE_POS};
```

Afin de retenir les vingt dernière positions de la souris, deux tableaux
sont déclarés (un pour x, l'autre pour y). Un index sur la position à
enregistrer dans le buffer est initialisé à zéro. Le tout est placé dans
une structure contenant aussi le nombre d’éléments des tableaux.

La boucle principale (main\_loop)
---------------------------------

C'est `SDL_PollEvent()` qui permet d'interroger la file d'attente des
évènements. Les évènements peuvent être de plusieurs types :
l'utilisateur veut fermer l'application, la souris a bougé, une touche a
été appuyée,...

Ces évènements du système sont mis dans une file d'attente et le
programme doit aller lire cette file afin d'y répondre de façon
adéquate. Un évènement est de type `SDL\_Event`, qui est en fait une
`union` de structures de tous les types d'évènements possibles. Toutes
ces structures partagent leur premier champ : leur type. Le `switch` lit
ce type et réagit en fonction.

Si c'est un type `SDL_QUIT`, alors on active le drapeau qui arrête la
boucle principale.

Si c'est un type `SDL_MOUSEMOTION`, c'est que la souris a changé de
position. La structure réelle d'un évènement mouvement de souris est
`SDL_MouseMotionEvent`. On modifie le type de `SDL_Event` en
`SDL_MouseMotionEvent` afin de lire les coordonnées `x` et `y` de la
souris. Ces coordonnées sont placées dans le buffer de positions. Ces
coordonnées sont fournies en absolues par rapport à la fenêtre, ce qui
est bien pratique.

La fonction d'affichage (draw)
------------------------------

La fonction d'affichage commence par effacer l'écran. Pour cela, on
demande l'affichage d'un rectangle plein de couleur noire prenant tout
l'espace d'affichage.

Afin d'obtenir la couleur noire, nous utilisons la fonction `SDL_MapRGB
(screen->format, 0, 0, 0);`. `SDL_MapRGB()` permet d'obtenir un
entier sur 32 bits représentant la couleur décrite suivant ses
composantes rouge verte et bleue en fonction de la profondeur
d'affichage courante.

Ainsi, pas besoin de se tracasser à savoir dans quel mode d'affichage on
est, SDL s'occupe de tout. Noir en RGB, c'est `(0,0,0)`. Le premier
argument précise le format d'affichage de la surface écran que l'on a
initialisée.

`SDL_FillRect()` permet d'afficher un rectangle sur la surface avec une
couleur donnée. Nous connaissons la couleur et la surface. Afin que le
rectangle prenne tout l'espace, le deuxième argument est tout simplement
mis à `NULL`.

Nous demandons ensuite une nouvelle couleur : bleu, en RGB `(0,0,255)`.
Les plus perspicaces d'entre vous auront remarqué que nous pourrions
calculer ces deux couleurs une fois pour toute au début du programme.
C'est vrai. Mais ici, on fait simple et direct ; ça sera votre premier
exercice de modification de ce programme.

La boucle examine toutes les positions de la souris enregistrées dans le
buffer et affichage une petit rectangle à cet endroit. Afin de décrire
le rectangle à afficher, nous devons remplir une structure de type
`SDL_Rect`. Les champs `w` et `h` sont respectivement la largeur (*width*)
et la hauteur (*height*) du rectangle à afficher, les champs `x` et `y`
sont les coordonnés en absolue par rapport à l'origine de la surface
affichée.

Dans notre cas, on positionne les coordonnées en fonction des
coordonnées enregistrées de la souris, et notre carré fait cinq pixels
de côté.

`SDL_FillRect()` prend la variable de type `SDL_Rect` en deuxième
paramétré, à la place du `NULL` de tout à l'heure.

Enfin, maintenant que tout l'affichage est fait, on demande la
permutation des deux buffers grâce à `SDL_Flip()` en précisant la
surface concernée.

Conclusion 
-----------

Voilà un premier programme, très simple, qui décrit brièvement comment
accéder à l'affichage grâce à la SDL. Ce programme a été compilé avec
succès avec gcc 7.3.0 sous Linux/Ubuntu 18.04.

Je vous invite fortement à prendre le source de ce programme et de vous
amuser avec, d'y ajouter des choses, de changer les couleurs, la forme
des rectangles affichés, de changer le nombre de coordonnées
enregistrées, de faire cycler les couleurs et pourquoi pas tout à la
fois.

Crédits 
--------

- Tutorial écrit par [Mokona](https://www.puupuu.org) écrit pour Prografix.
- Date de publication : 17 juillet 2002
