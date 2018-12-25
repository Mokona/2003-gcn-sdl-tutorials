Affichage de sprites avec la SDL, partie 3
==========================================

Introduction
------------

Dans la partie précédente, nous avions obtenu à l'écran des sprites
énumérant les chiffres de 0 à 9, en sprites, se déplaçant de façons
aléatoires et indépendantes à l'écran.

Dans cette troisième partie, nous allons un peu changer la structure du
programme pour accueillir des sprites animés différemment en fonction de
leur direction.

Avant de commencer la lecture de cette partie, je vous conseille de lire
bien évidemment les deux premières parties de cette série d'articles.

Les trois programmes sont disponibles dans l'archive fournie avec cet
article. Afin de les compiler, vous devez avoir la librairie SDL
installée et savoir comment compiler un programme avec cette librairie.
Vous devrez aussi avoir installé la librairie `SDL_image`. Si vous
utilisez gcc sous un environnement Linux/Bash, vous compilerez très simplement
chaque programme de la manière suivante :
``gcc -o sprites7 sprites7.c `pkg-config sdl --cflags -libs` ``.
                                                               
Si vous utilisez `fish`: `gcc -o sprites7 sprites7.c (pkg-config sdl --cflags --libs | string split " ")`

Et de même pour les sources `sprites8.c` et `sprites9.c`, en y ajoutant `SDL_Image`.


Première phase
--------------

### La réécriture

Dans `sprites6.c`, l'animation était gérée par deux biais : le sprite
contenait le numéro de son image d'animation (`numStep`), mis à jour par
une fonction à chaque tour de boucle, et ce numéro d'image était traduit
en `SDL_Rect` source pour afficher la partie de la surface correspondant
à cette image.

Avec cette méthode, à chaque affichage, on recalcule le bon `SDL_Rect`
sur la bonne surface. Comme souvent en informatique, il y a un choix à
faire entre le recalcule systématique pour un gain de place en mémoire
et le pré-calcul pour un gain de vitesse (et une perte en place
mémoire).

Afin de gagner en vitesse ainsi qu'en simplicité, et parce que les PCs
actuels n'ont pas vraiment de problèmes de mémoire, nous allons opter
pour le pré-calcul. L'idée est donc d'avoir tous les `SDL_Rect` prêts et
associés à leur `SDL_Surface` avant la boucle d'affichage.

Une structure nommée `stFrame` est donc ajoutée.

On y trouve un pointeur sur une surface, qui sera la surface qui
contient l'image de l'animation, et un `SDL_Rect` afin de définir la
zone de cette surface qui contient l'image que l'on veut afficher.

On y trouve aussi un champ `nextFrame`, pointeur sur une `Frame`. Nous
allons utiliser le principe des **listes chaînées**. Plutôt que d'avoir le
numéro de l'image d'animation courante que nous mettrions à jour avec un
test de dépassement, nous allons construire la liste des images
d'animation sous forme de liste. Le champ `nextFrame`, comme son nom
l'indique, sera un pointeur sur l'image suivante. Ainsi, la mise à jour
de l'image d'animation en cours se résumera à lire ce champ.

Comme nous avons encapsulé la surface dans une frame, la fonction
`create_random_animated_sprite_with_speed` change de paramètre et
prend à présent une `Frame` qui représentera la première frame
d'animation. La fonction alloue la structure de sprite comme avant, choisi
au hasard les coordonnées, la vitesse et le déplacement initial et assigne le
paramètre à la structure.

Les frames sont elles créées par la fonction `create_frame` qui prend la
surface et les coordonnées dans cette surface pour cette étape
d'animation.

Grâce à cette nouvelle façon de manipuler les différentes frames de
l'animation, la mise à jour de la structure du sprite est beaucoup plus
simple. Pas de test à faire, la mise à jour consiste tout simplement à
remplacer le pointeur sur la frame courante par celui contenu dans le
champ `nextFrame`.

Ce qui ce limite à cela :

```C
oneSprite->currentFrame = oneSprite->currentFrame->nextFrame;
```

De la même façon, l'affichage du sprite est simplifié car il est inutile
de calculer les paramètres d'affichage, qui sont déjà dans la frame. Il
suffit de les passer à la fonction de blit.

Voyons à présent les changements apportés au programme principal. Nous
devons ajouter un tableau pour stocker les frames,
`Frame* arrayFrames[10];`, en plus de celui déjà présent pour
stocker les sprites, qui ne change pas.

La surface contenant toutes les images est chargée de la même façon
qu'avant. Mais à présent, elle est utilisée pour créer les frames plutôt
que les sprites. Pour cela, une boucle suffit à créer les frames car
elles sont toutes prises dans la même surface et ont la même taille.

Après avoir créé les frames, il faut les lier entre elles. Comme les
frames sont dans l'ordre dans l'image et ont été créées dans ce même
ordre, la frame suivante est tout simplement la prochaine dans le
tableau. Sauf pour la dernière frame, qui est liée à la première.

Le plus difficile est passé. Les sprites sont crées en passant en
paramètre la première frame, qui est pour tous `arrayFrames[0]`.

Le reste ne change pas. À noter que nous sommes revenus à la version où
l'écran est effacé à chaque fois.

### Résumé

Le résultat de ce programme est le même que dans `sprites6.c`, nous
n'avons fait que changer un peu la façon de traiter les données. Cela
donnera plus de flexibilité pour la suite.

Deuxième phase
--------------

### Hélicoptère

Dans `sprites8.c` nous allons ajouter un type de sprite dont l'animation
dépend de la direction dans laquelle il se dirige. C'est très courant
dans les jeux, que ce soit d'aventure, plate-forme, action,... Les
sprites ont généralement beaucoup d'animations qui dépendent de l'action
qu'ils sont en train de représenter.

Dans ce programme, nous différencierons les animations pour les quatre
direction : haut, droite, bas et gauche. Chaque animation a quatre
frames.

![](data/helico_HAUT_001.png) ![](data/helico_HAUT_002.png)
![](data/helico_HAUT_003.png) ![](data/helico_HAUT_004.png)

![](data/helico_DROIT_001.png) ![](data/helico_DROIT_002.png)
![](data/helico_DROIT_003.png) ![](data/helico_DROIT_004.png)

![](data/helico_BAS_001.png) ![](data/helico_BAS_002.png) ![](data/helico_BAS_003.png)
![](data/helico_BAS_004.png)

![](data/helico_GAUCHE_001.png) ![](data/helico_GAUCHE_002.png)
![](data/helico_GAUCHE_003.png) ![](data/helico_GAUCHE_004.png)

Comme la question sera posée, autant y répondre tout de suite : les
sprites ont été créés avec
[KPovModeler](https://fr.wikipedia.org/wiki/KPovModeler)
(n'existe plus) et rendu avec
[PovRay](http://www.povray.org/). L'hélicoptère est assez stylisé,
c'était juste histoire d'avoir une animation et vous pouvez tout à fait
penser que cela ne ressemble pas du tout à un hélicoptère.

Ce sont aussi des images au format PNG. La SDL ne charge pas de base
autre chose que les BMP. La librairie qui s'en charge est `SDL_image`,
que vous devrez donc installer pour compiler les exemples suivants.
N'oubliez pas de prévenir votre compilateur que vous voulez lier
`SDL_image` à ces programmes lorsque vous les compilez.

La liste des fichiers à charger est initialisée au début en dur dans le
tableau `helicopterFiles`.

### Sprites différenciés

Le premier changement dans la structure du sprite est l'ajout d'un type
de sprite. En effet, nous aurons un traitement différent suivant le type
du sprite : soit l'ancien système avec une animation, soit le nouveau
avec quatre animations, dépendant de la direction du sprite. Le second
changement est d'ajouter à la structure un pointeur sur l'animation du
sprite pour chacune des quatre directions. Bien entendu, pour un sprite
ancien système, ces quatre pointeurs ne servent à rien. Nous les gardons
pour le moment, nous réglerons ce problème plus tard.

Les anciens sprites étaient crées avec une vitesse aléatoire. Les
nouveaux sprites auront une vitesse fixe. On ajoute donc un constructeur
de sprite qui crée un sprite avec une vitesse donnée :
`create_random_animated_sprite_with_speed`. L'ancien constructeur tire une vitesse
aléatoire et appel le nouveau constructeur.

Dans ce nouveau constructeur, nous initialisons à zéro le type du sprite
et les quatre pointeurs sur les animations de direction sont mis à
`NULL`.

La mise à jour nécessite aussi des aménagements. La mise à jour était
quelque chose de très simple. Les coordonnées étaient directement mises
à jour à partir des informations de déplacement du sprite. À présent,
ceci n'est plus valable que pour l'ancien type de sprite.

La mise à jour contient donc un `switch` sur le `type` du sprite. Dans
le cas d'un sprite de type 0, l'ancien code est exécuté. Dans le cas
d'un sprite de type 1, c'est un peu plus compliqué.

En premier lieu, les nouveaux sprites ne se déplacent qu'horizontalement
et verticalement, c'est un choix. Nous déplaçons le sprite en fonction
du déplacement le plus grand. C'est le but du test sur les valeurs
absolues au début du `case`. Il faut bien comprendre ici que ce
comportement n'est pas une méthode particulière ou une recette de code,
c'est juste un choix que j'ai fait pour le déplacement de ces sprites.
Un choix direct de déplacements strictement horizontaux ou verticaux
aurait été probablement plus simple. Vous pouvez modifier le programme
pour avoir un tel comportement comme exercice.

Après le test, nous changeons le comportement du sprite dans 10% des
cas, grâce à un nombre aléatoire.

Si le test passe, c'est à dire statistiquement 1 fois sur 10, le
déplacement en X et Y du sprite est retiré aléatoirement. Comme ce type
de sprite a une animation dépendante de la direction qu'il prend, la
frame d'animation courante doit être mise à jour en fonction. Les
valeurs absolues de déplacement sont testées de la même façon que pour
la mise à jour du déplacement afin de savoir si le sprite se déplace
horizontalement ou verticalement. Puis le signe du déplacement est testé
pour connaître le sens du déplacement : haut ou bas pour le vertical,
droite ou gauche pour l'horizontal.

En fonction de tous ces tests, le champ `currentFrame` est mis à jour
avec la valeur d'un des quatre pointeurs sur les premières frames
d'animation (`firstRightFrame`, `firstLeftFrame`, `firstDownFrame` ou
`firstUpFrame`).

Voilà pour les changements dans les fonctions. À présent, regardons les
modifications du programme principal.

### Fonctionnement général

Tout d'abord, nous remplaçons le pointeur `loadedPicture` par
`loadedDigitPicture` et nous ajoutons un tableau de pointeurs sur
`SDL_Surface` : `loadedHelicopterPictures[16]`. Le premier pointeur
contient la même chose qu'avant : l'image de l'animation des chiffres.
Le tableau de pointeurs est là pour stocker les 16 images de l'animation
de l'hélicoptère.

En passant, le chargement des frames des chiffres est extrait dans
une fonction à part. 

Ces deux animations
sont représentées par deux techniques différentes : toutes les frames
d'animation des chiffres sont contenues dans une image alors que les
frames de l'hélicoptère sont dans 16 images différentes. Mais grâce aux
aménagements faits dans `sprites7.c`, cela importe peu, nous le verrons
lors du chargement des images et de la construction des `stFrame`.

La taille du tableau qui contient les sprites est doublée. En effet,
nous affichons `NB_SPRITES` chiffres, et `NB_SPRITES` hélicoptères.

Le tableau des frames de `sprites7.c` est remplacé par
`digitFrames`, juste pour être plus clair. Un tableau de frames est
ajouté pour l'animation de l'hélicoptère avec le nom `helicopterFrames`,
de taille 16 bien entendu, puisqu'il y a 16 images.

Le chargement de l'image `32sprites.bmp` est fait de la même façon
qu'avant, seul le nom du pointeur a changé. Les 16 images de
l'hélicoptère sont chargées avec la fonction `IMG_Load` mise à
disposition par la librairie `SDL_image` et affectées au tableau
`loadedHelicopterPictures`.

Un message d'erreur avertira en cas de problème au chargement de l'image
PNG. Si ce message d'erreur appairait à l'exécution, deux grandes causes
peuvent se présenter. La première est évidemment l'absence des fichiers
dans le répertoire d'exécution. La seconde est que votre librairie
`IMG_image` n'a pas été compilée avec le support des PNG. Dans ce cas,
vous devrez vous procurer ou recompiler une `IMG_image` correcte.

La création des frames des chiffres ne change pas (mis à part le nouveau
nom du pointeur). La création des frames d'hélicoptère est similaire. On
appelle 16 fois la fonction `create_frame` avec les surfaces chargées,
toutes de la même taille et aux mêmes coordonnées :

À partir de ces frames, les animations sont crées avec un chaînage comme
pour les frames d'animation des chiffres. Ici, cependant, nous avons
quatre animations de quatre frames à chaque fois. Pour faire simple, on
fait une première boucle qui chaîne toutes les frames sauf la dernière,
comme pour les chiffres. Puis nous affectons aux dernières frames de
chacune des quatre animations le pointeur sur la première frame de leur
animation.

```C
/* Liens généraux entre les frames */
for (int i = 0; i < 15; i++)
{
    helicopterFrames[i]->nextFrame = helicopterFrames[i + 1];
}

/* Correction des liens */
helicopterFrames[3]->nextFrame = helicopterFrames[0];
helicopterFrames[7]->nextFrame = helicopterFrames[4];
helicopterFrames[11]->nextFrame = helicopterFrames[8];
helicopterFrames[15]->nextFrame = helicopterFrames[12];
```    

Nos quatre animations commencent donc respectivement avec les frames
`helicopterFrames[0]`, `helicopterFrames[4]`, `helicopterFrames[8]` et
`helicopterFrames[12]`.

Les frames sont créées, il faut à présent créer les sprites. La création
des sprites pour les chiffres ne change pas beaucoup : le nom de la
fonction a été modifié, et le type du sprite (`0`), est affecté après
chaque création.

Les sprites d'hélicoptère sont créés de manière similaire. On commence
par un appel à `create_random_animated_sprite_with_speed()`, puisque la vitesse est
fixe, et avec l'affectation du type du sprite (`1`). Tous les accès au
tableau sont décalés de `NB_SPRITES` car le choix est d'avoir les
sprites "chiffres" de `0` à `NB_SPRITES-1` et les sprites
« hélicoptères » de `NB_SPRITES` à `(NB_SPRITES*2)-1` dans le tableau.

Puis, les animations sont affectées suivant les directions :

```C    
sprites[i + NB_SPRITES]->firstDownFrame = helicopterFrames[0];
sprites[i + NB_SPRITES]->firstLeftFrame = helicopterFrames[4];
sprites[i + NB_SPRITES]->firstUpFrame = helicopterFrames[8];
sprites[i + NB_SPRITES]->firstRightFrame = helicopterFrames[12];
```    

**Note** : ce genre d'affectations sauvages de type et d'animation, ou plus
généralement des champs d'une structure, en plein milieu du programme
principal n'est pas très propre. Dans le cadre de ces articles, l'idée
est de faire des changements simples à chaque nouveau programme. Dans
l'absolu, une fonction de gestion de la création des sprites devrait
s'occuper de cette maintenance. Nous verrons ça plus loin.

Reste à modifier la boucle de mise à jour et la boucle d'affichage des
sprites avec le nouveau nombre de sprites à afficher (`NB_SPRITES*2`) et
à modifier la libération de mémoire à la fin du programme.

### Résumé

En plus des chiffres animés qui bougent aléatoirement, nous avons à
présent des hélicoptères (ou quelque chose qui y ressemble vaguement) se
déplacement soit verticalement, soit horizontalement, et animés en
fonction de la direction.

Il y a une petite erreur graphique dans l'animation des hélicoptères. Je
ne la distingue pas vraiment même en sachant qu'elle existe. Cependant,
vous pouvez en exercice chercher à la corriger.

L'erreur vient de la reinitialisation à la première frame d'animation à
chaque changement de direction de l'hélicoptère. Cela devrait créer un
saut dans l'animation, qui peut passer de la frame 1 d'un animation
directement à la frame 0 de l'autre animation, esquivant ainsi 2 images.
L'idée de la correction est donc de se replacer dans la nouvelle
animation en fonction de la dernière frame affichée.

Troisième phase
---------------

Dans `sprites9.c`, nous allons gagner un peu de place en mémoire en
ajoutant une structure et nous allons afficher le nombre de FPS (frames
par secondes, c'est-à-dire le nombre d'images par seconde affichées à
l'écran) en bas à droite de l'écran.

### Un peu de nettoyage

Dans `sprites8.c`, nous avions ajouté à la structure `Sprite` quatre
pointeurs vers les premières images des animations, un pointeur par
direction possible. Cependant, nous avions deux types de sprites, l'un
qui utilisait ces pointeurs, et l'autre non. Il est dommage d'avoir des
pointeurs qui ne servent à rien et prennent de la place pour tous les
sprites qui n'en ont pas besoin (les sprites de type 0).

Il y a plusieurs manières d'alléger le sprite. Nous allons utiliser une
manière simple qui est de déplacer ces quatre pointeurs dans une
structure `Animation` et de remplacer dans `Sprite` les quatre pointeurs
par un seul pointeur vers une structure `Animation`.

Voilà un peu de place gagnée.

Comme nous avons fait jusqu'à maintenant à chaque fois, puisqu'il y a
une structure à créer, nous lui joignons une fonction pour construire
des instances de cette structure. La fonction `create_animation()` prend en
paramètre les quatre frames des animations et alloue une instance de
`Animation` qu'elle renvoie remplie.

Dans la création du sprite, on enlève les affectations des quatre
pointeurs à `NULL` et on les remplace par l'initialisation du nouveau
pointeur, `associatedAnimation` à `NULL`.

Au passage, toutes les mentions à ces quatre pointeurs dans les
différentes fonctions sont remplacées par des mentions aux quatre
pointeurs de l'`associatedAnimation`.

Dans le programme principal, on ajoute un pointeur sur `Animation` qui
contiendra l'animation de l'hélicoptère, que nous créons avec un appel à
`create_animation` après la création des frames d'animations. Et plutôt que
d'affecter au sprite les quatre pointeurs sur ces animations, on lui
affecte `animHelicopter`.

Voilà pour le nettoyage, qui allège aussi un peu l'écriture dans le
programme principal.

### Affichage des FPS

C'est un grand classique dans les jeux : afficher sa vitesse,
c'est-à-dire sa performance. Nous verrons si notre programme est
efficace et donnera un jeu « jouable" ». Il est généralement considéré
qu'en dessous de 30 FPS, le jeu n'est plus agréable. À partir de 50 ou 60
(ce qui correspond à la fréquence d'une télévision), l'affichage
commence à être vraiment agréable.

Comment calculer le FPS ? Le FPS est une fréquence : la méthode généralement
utilisée est de calculer la fréquence instantanée.
Son inconvénient est que la fréquence n'est pas vraiment
juste. Elle est calculée directement à partir de la période entre les
deux derniers affichages, et ce nombre risque bien d'être différent à
chaque affichage, ne facilitant pas la lecture.

On peut adoucir ce dernier résultat en calculant la valeur moyenne de la
fréquence sur quelques images.

Dans `sprites9.c`, nous utiliserons la fréquence instantanée, mais je
vous conseille d'expérimenter en exercice le lissage de valeur.

Pour cela, nous mesurons la différence d'heure entre deux rendus. On
retient à chaque boucle l'« heure » actuelle avec
`SDL_GetTicks()`. On ajoute une variable dans le programme principal
dans laquelle on stock l'ancienne valeur avant d'acquérir la nouvelle.
Ces deux valeurs sont utilisées par une fonction `compute_fps()`.

Cette fonction fait la différence entre l'heure actuelle et l'ancienne.
Si cette différence n'est pas nulle, alors on passe de la période à la
fréquence multipliée par 1000. La multiplication par 1000 vient du fait
que l'heure est en millisecondes. Le test sur la différence de zéro est
pour éviter une division par zéro, tout simplement.

Reste à afficher cette valeur. La fonction qui aura cette charge se
nomme `display_fps()`. Cette fonction prend en paramètres une surface
cible (l'écran pour nous), le tableau sur les dix sprites des chiffres
et la valeur que l'on veut afficher.

L'affichage se fera à un endroit précis : l'unité sera en (620,450).
Nous spécifions ces coordonnées en variables locales. Ce ne sont pas des
constantes car nous allons modifier `destX`. La valeur ne changera pas
pour `destY` car nous n'écrivons que sur une ligne. Cela aurait pu être
une constante.

Puis vient une boucle que nous n'allons exécuter que si FPS est
différent de zéro. À chaque boucle, on récupère le chiffre le plus à
droite (en base 10) de la valeur actuelle des FPS. Ce chiffre est le
chiffre que l'on va afficher. Malheureusement, l'image des chiffres est
dans l'ordre suivant : 1, 2, 3, 4, 5, 6, 7, 8, 9 puis 0. On ajuste donc
le chiffre : si on a un zéro, on le change en 10. Puis on réduit d'une
unité car un tableau en C commence par l'indice 0.

Ainsi, nous avons l'indice du sprite à afficher dans le tableau qui a
été passé en paramètre. L'affichage se fait tout simplement avec un
appel à `display_animated_sprite()`.

On termine la boucle en divisant le nombre de FPS par 10, pour
"éliminer" l'unité. On décrémente aussi les coordonnées de 16 pixels.

Lorsque le nombre sera entièrement affiché, la dernière division entière
par 10 donnera 0, ce qui fera sortir de la boucle, et nous aurons
affiché le nombre de FPS en partant de la "fin".

### Résumé

À présent, en plus des hélicoptères et des chiffres baladeurs, le
programme affiche à un endroit précis le nombre de FPS. Peut-être
allez-vous bondir aux valeurs qui sont affichées d'ailleurs. Et
peut-être pas. Cela dépend de votre machine, de la profondeur
d'affichage de votre bureau,... et d'autres choses.

Quatrième phase
---------------

Il existait dans les articles originaux une quatrième phase et un
fichier `sprite10.c` pour discuter de performances.

Cependant, depuis l'écriture, le matériel a changé, et la discussion
n'est plus entièrement d'actualité. Cette partie a donc été retirée.

Au final
--------

Nos sprites bougent de différentes façons, les animations peuvent
dépendre de la direction qu'ils prennent, nous affichons un indicateur
(simpliste) de performances.

Dans la prochaine partie, nos sprites se déplaceront sur un fond.

À bientôt.

Crédits 
--------

- Tutorial écrit par [Mokona](https://www.puupuu.org) écrit pour Prografix.
- Date de publication : 6 septembre 2003
