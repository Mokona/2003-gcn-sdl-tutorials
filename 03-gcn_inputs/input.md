Traitement des entrées avec la SDL
==================================

Introduction
------------

Cet article va examiner comment gérer les entrées clavier, souris et
joysticks avec la SDL.

Je présenterai une méthode basée sur les évènements envoyés par la `SDL`
afin d'être cohérent avec les articles sur les sprites.

Pour comprendre l'article, vous devez connaître un minimum le C. Le
programme est livré avec cet article et peut vous servir comme base dans
une gestion des entrées. Afin de compiler ce programme, vous devez avoir
la librairie `SDL` installée et savoir comment compiler un programme avec
librairie. Si vous utilisez « gcc » sous un environnement Linux/Bash,
vous compilerez très simplement
chaque programme de la manière suivante :
``gcc -o input input.c `pkg-config sdl --cflags -libs` ``.
                                                               
Si vous utilisez `fish`: `gcc -o input input.c (pkg-config sdl --cflags --libs | string split " ")`

Initialisation
--------------

Afin de pouvoir traiter les entrées, nous devons créer un squelette
d'application SDL. Commençons donc par regarder la fonction `sdl_init()`.

      if (SDL_Init (SDL_INIT_VIDEO|SDL_INIT_JOYSTICK) < 0)

La librairie est intialisée avec deux flags. `SDL_INIT_VIDEO` est
nécessaire car SDL ne peut traiter les entrées claviers sans un contexte
vidéo. Cela peut sembler étrange, mais c'est un comportement inhérent
aux plateformes sur lesquelles tourne SDL (et si ce n'est pas toutes les
plateformes, cela concerne au moins une grande majorité). Cela peut se
comprendre : dans un système fenêtré, le systéme dirige les entrées vers
la fenêtre active (on dit qu'elle a le focus).

`SDL_INIT_JOYSTICK` est utile si l'on veut gérer les joysticks.

`SDL_Init()` ne suffit pas, il faut aussi créer une fenêtre avec
`SDL_SetVideoMode()`. La fenêtre créée est toute petite. Lors de mes
tests, il m'arrivait souvent que cette fenêtre recouvre le terminal
depuis lequel je lançais le programme en m'empêchant de voir les
résultats.

On utilise `atexit()` pour appeler une fonction qui fermera proprement
SDL avant de sortir du programme.

Passons sur les initialisations de l'UNICODE et des Joysticks pour
arriver à la boucle principale. C'est la boucle standard d'attente des
évènements dans un programme qui utilise SDL.

La boucle se déroule tant que la valeur de `quitProgram` est fausse. À
chaque tour de boucle, on examine tous les évènements de la file
d'évènements de SDL.

Un petit mot sur les évènements. La SDL examine toute une série de
« choses » qui peuvent se passer dans le système. Lorsqu'une de ces
« choses » arrivent, la SDL construit un évènement qui décrit ce qui
s'est passé, et met cet évènement dans une file d'attente. Le programme
peut donc lire ces évènements et réagir en fonction de ce qui s'est
passé dans le système depuis le dernier examen.

SDL gère les évènements suivant :

-   Activation/Désactivation de la fenêtre de l'application ;
-   Utilisation du clavier ;
-   Utilisation de la souris ;
-   Utilisation d'un joystick ;
-   Modification de la taille de la fenêtre ;
-   Altération extérieure du contenu de la fenêtre ;
-   Demande de terminaison du programme ;
-   Évènements dépendant du Window Manager ;
-   Évènements utilisateurs.

Ceux qui nous intéresse ici sont ceux qui avertissent d'une activité
clavier, souris ou joystick. Ce type est filtré par un `switch`.

C'est tout pour le cœur du programme.

Gestion du clavier
------------------

Les évènements examinés pour la gestion du clavier sont :

- `SDL_KEYDOWN` : une touche a été appuyée ;
- `SDL_KEYUP` : une touche a été relâchée.

Dans ces deux cas, on appelle une fonction `keyboard_event()` avec
en paramètre l'évènement transtypé en `SDL_KeyboardEvent *`. Les
évènements de type `SDL_Event` sont en effet une union de tous les
évènements possibles. Il faut donc déterminer le type réel de la
variable `event` avant d'y piocher les informations correspondantes à
l'évènement.

Dans cette fonction, nous allons examiner l'évènement reçu et afficher
sur la sortie standard des informations le concernant.

**Note** : sous Windows, la bibliothèque SDL est généralement compilée pour
que les affichage sur la sortie standard soient redirigés vers un
fichier. Si vous ne voyons rien apparaître, cela est donc normal, les
résultats sont dans un fichier nommé `stdout.txt` qui se trouve dans le
répertoire de l'exécutable.

La première chose que la fonction vérifie est le `type` de l'évènement.
En effet, j'appelle la même fonction pour les deux évènements
`SDL_KEYDOWN` et `SDL_KEYUP`. Appeler deux fonctions différents pour ces
deux évènements est une autre manière de faire. Cela dépend de votre
programme d'une manière plus large.

### Touche appuyée

Si l'évènement est `SDL_KEYDOWN`, une touche vient d'être appuyée. Nous
allons afficher laquelle. Le premier test sur la valeur UNICODE n'a de
sens que si vous avez appelé auparavant la fonction
`SDL_EnableUNICODE()` avec `VRAI (1)`pour paramètre. C'est fait dans ce
programme. La SDL va alors transformer ce qui a été tapé au clavier en
valeur UNICODE. Cela n'a aussi du sens que pour les touches qui sont
associés à des caractères. Appuyer sur la flèche droite du clavier ne
donnera pas de résultat intéressant.

Pour rester simple, nous n'affichons la touche traduite que si la valeur
UNICODE est comprise entre 32 et 127. En effet, sur cette plage de
valeurs, les normes UNICODE et ASCII coïncident. En fait, elles
coïncident aussi sur la plage 0 à 31, mais ces valeurs sont des
« caractères non affichables ».

Dans le cas où la valeur n'est pas dans cette plage, on affiche que la
touche enfoncée n'a pas été traduite.

**Note** : cette traduction est à utiliser avec prudence. En effet, il n'est
pas simple, avec la méthode utilisée par la SDL pour transcrire la
touche en valeur UNICODE, de faire la différence entre les différents
claviers sur certains systèmes d'exploitation. Ainsi, il peut y avoir
des problèmes de touche « A » traduite en « Q », ainsi que tous les
problèmes classiques du genre. Ayant moi même un clavier QWERTY, je ne
peux pas vraiment explorer ce problème. Il est cependant expliqué de
temps en temps sur la liste de diffusion de la SDL.

On affiche ensuite de toute façon le nom SDL de la touche à partir du
numéro SDL de la touche. Ce numéro se trouve dans le champ `sym` de la
structure `SDL_keysym` dont la valeur se trouve dans l'évènement. La
fonction `SDL_GetKeyName()` est une fonction qui permet d'afficher
quelque chose de plus compréhensible qu'un numéro.

En plus de la valeur UNICODE et de la valeur symbolique, la structure
`SDL_keysym` contient un champ `mod` qui retient la valeur des touches
de contrôles, comme SHIFT, CONTROL, ALT,... Je ne les traite pas dans
cet article. Il est à noter qu'un appuie sur ces touches génère aussi un
évènement. SDL retient cependant leur état car dans beaucoup de cas, on
veut connaître leur état simultanément à l'appuie d'une autre touche.
Par exemple pour différencier l'appuie sur « a » de l'appuie sur
« SHIFT+a ».

### Touche relâchée

Dans le cas où la touche est relâchée, on affiche seulement le nom SDL
de la touche. On ne peut pas afficher la valeur UNICODE car elle n'a
alors pas de sens et SDL ne la calcule pas.

### Résultat

Voici ce qui s'affiche dans le terminal lorsque je tape « Keyboard »
puis la touche `ENTRÉE` et les quatre flèches.

    Touche enfoncée non traduite
    Nom SDL de la touche : left shift
    Touche enfoncée : K
    Nom SDL de la touche : k
    Touche relevée : k
    Touche relevée : left shift
    Touche enfoncée : e
    Nom SDL de la touche : e
    Touche relevée : e
    Touche enfoncée : y
    Nom SDL de la touche : y
    Touche relevée : y
    Touche enfoncée : b
    Nom SDL de la touche : b
    Touche enfoncée : o
    Nom SDL de la touche : o
    Touche relevée : b
    Touche enfoncée : a
    Nom SDL de la touche : a
    Touche relevée : o
    Touche relevée : a
    Touche enfoncée : r
    Nom SDL de la touche : r
    Touche relevée : r
    Touche enfoncée : d
    Nom SDL de la touche : d
    Touche relevée : d
    Touche enfoncée non traduite
    Nom SDL de la touche : return
    Touche relevée : return
    Touche enfoncée non traduite
    Nom SDL de la touche : left
    Touche relevée : left
    Touche enfoncée non traduite
    Nom SDL de la touche : up
    Touche relevée : up
    Touche enfoncée non traduite
    Nom SDL de la touche : right
    Touche relevée : right
    Touche enfoncée non traduite
    Nom SDL de la touche : down
    Touche relevée : down

Gestion de la souris
--------------------

Pour la souris, en plus des boutons, il faut gérer le déplacement. La
gestion des boutons est relativement similaire à celle du clavier.

### Les boutons de la souris

Les noms des évènements changent (`SDL_MOUSEBUTTONDOWN` et
`SDL_MOUSEBUTTONUP`) ainsi que le type de l'évènement
(`SDL_MouseButtonEvent`) mais la fonction appelée
(`mouse_button_event()`) est similaire.

Il est intéressant de noter que l'évènement porteur de l'information du
bouton porte aussi avec lui les coordonnées absolues de la souris. De la
même manière que pour les modificateurs lors de l'appuie d'une touche,
la SDL considère que c'est une information importante dont la plupart
des programmes auront besoin à ce moment là.

Les boutons de la souris sont désignés par des numéros. Les numéros
classiques ont des symboles SDL (`SDL_BUTTON_LEFT`, `SDL_BUTTON_MIDDLE`
et `SDL_BUTTON_RIGHT`).

La molette est gérée comme deux boutons, généralement `4` et `5`.

### Les déplacements de la souris

L'évènement à surveiller est `SDL_MOUSEMOTION`. Dans ce programme, on
appel à sa réception la fonction `mouse_button_event()`

La fonction est très simple. Elle affiche en premier lieu les
informations de déplacement de la souris. Ces informations sont de deux
natures : les coordonnées absolues et les coordonnées relatives.

*Coordonnées absolues* : ce sont les coordonnées du pointeur souris dans
la fenêtre. Le (0,0) se situe en haut à gauche de la fenêtre.

*Coordonnées relatives* : c'est le déplacement du pointeur souris depuis
le dernier évènement du même type. Par exemple, si un premier évènement
est reçu avec pour coordonnées absolues (10,20) et un deuxième évènement
avec (15,10), les coordonnées relatives du deuxième évènement seront
(5,-10). Le premier mouvement relatif est identique aux coordonnées
absolues.

Les coordonnées absolues servent à savoir où se trouve la souris. Le
programme peut donc calculer ce qui est sous la souris, ce que l'on va
sélectionner, sur quoi on va cliquer,...

Les coordonnées relatives servent plutôt lors de déplacement à partir de
la souris, comme la direction d'un vaisseau dans un shoot'em up, ou bien
le changement de direction de la caméra dans un FPS.

Toujours dans un soucis pratique, SDL fourni avec les informations de
déplacement l'état des boutons de la souris lors de ce déplacement. La
fonction affiche l'état des trois premiers boutons.

### Résultat

Voici ce qui se passe lorsque je bouge la souris dans la fenêtre en
cliquant successivement sur les trois boutons.
    
    Mouvement de la souris (85,127) . Coordonnées dans la fenêtres (85,127)
    Mouvement de la souris (-1,-1) . Coordonnées dans la fenêtres (84,126)
    Mouvement de la souris (-1,-1) . Coordonnées dans la fenêtres (83,125)
    Mouvement de la souris (0,-1) . Coordonnées dans la fenêtres (83,124)
    Mouvement de la souris (-1,-1) . Coordonnées dans la fenêtres (82,123)
    Mouvement de la souris (0,-1) . Coordonnées dans la fenêtres (82,122)
    Mouvement de la souris (-1,-1) . Coordonnées dans la fenêtres (81,121)
    Mouvement de la souris (-1,-1) . Coordonnées dans la fenêtres (80,120)
    Mouvement de la souris (0,-1) . Coordonnées dans la fenêtres (80,119)
    Mouvement de la souris (-1,-1) . Coordonnées dans la fenêtres (79,118)
    Mouvement de la souris (0,-1) . Coordonnées dans la fenêtres (79,117)
    Mouvement de la souris (0,-1) . Coordonnées dans la fenêtres (79,116)
    Mouvement de la souris (-1,-1) . Coordonnées dans la fenêtres (78,115)
    Mouvement de la souris (0,-1) . Coordonnées dans la fenêtres (78,114)
    Mouvement de la souris (0,-1) . Coordonnées dans la fenêtres (78,113)
    Mouvement de la souris (0,-1) . Coordonnées dans la fenêtres (78,112)
    Mouvement de la souris (-1,-1) . Coordonnées dans la fenêtres (77,111)
    Mouvement de la souris (0,-1) . Coordonnées dans la fenêtres (77,110)
    Mouvement de la souris (0,-1) . Coordonnées dans la fenêtres (77,109)
    Mouvement de la souris (-1,-1) . Coordonnées dans la fenêtres (76,108)
    Mouvement de la souris (0,-1) . Coordonnées dans la fenêtres (76,107)
    Mouvement de la souris (0,-1) . Coordonnées dans la fenêtres (76,106)
    Mouvement de la souris (0,-1) . Coordonnées dans la fenêtres (76,105)
    Mouvement de la souris (-1,-1) . Coordonnées dans la fenêtres (75,104)
    Mouvement de la souris (0,-1) . Coordonnées dans la fenêtres (75,103)
    Mouvement de la souris (-1,-1) . Coordonnées dans la fenêtres (74,102)
    Mouvement de la souris (0,-1) . Coordonnées dans la fenêtres (74,101)
    Mouvement de la souris (0,-1) . Coordonnées dans la fenêtres (74,100)
    Mouvement de la souris (0,-1) . Coordonnées dans la fenêtres (74,99)
    Mouvement de la souris (0,-1) . Coordonnées dans la fenêtres (74,98)
    Mouvement de la souris (-1,0) . Coordonnées dans la fenêtres (73,98)
    Mouvement de la souris (0,-1) . Coordonnées dans la fenêtres (73,97)
    Bouton 1 appuyé aux coordonnées (73,97)
    Mouvement de la souris (0,-1) . Coordonnées dans la fenêtres (73,96) - B1
    Mouvement de la souris (-1,-1) . Coordonnées dans la fenêtres (72,95) - B1
    Mouvement de la souris (-1,-1) . Coordonnées dans la fenêtres (71,94) - B1
    Mouvement de la souris (0,-1) . Coordonnées dans la fenêtres (71,93) - B1
    Mouvement de la souris (-1,0) . Coordonnées dans la fenêtres (70,93) - B1
    Mouvement de la souris (-1,-2) . Coordonnées dans la fenêtres (69,91) - B1
    Mouvement de la souris (-1,-1) . Coordonnées dans la fenêtres (68,90) - B1
    Mouvement de la souris (-1,-1) . Coordonnées dans la fenêtres (67,89) - B1
    Mouvement de la souris (0,-1) . Coordonnées dans la fenêtres (67,88) - B1
    Mouvement de la souris (0,-1) . Coordonnées dans la fenêtres (67,87) - B1
    Mouvement de la souris (0,-1) . Coordonnées dans la fenêtres (67,86) - B1
    Mouvement de la souris (0,-1) . Coordonnées dans la fenêtres (67,85) - B1
    Mouvement de la souris (0,-1) . Coordonnées dans la fenêtres (67,84) - B1
    Mouvement de la souris (-1,0) . Coordonnées dans la fenêtres (66,84) - B1
    Bouton 1 relaché aux coordonnées (66,84)
    Mouvement de la souris (0,-1) . Coordonnées dans la fenêtres (66,83)
    Mouvement de la souris (1,0) . Coordonnées dans la fenêtres (67,83)
    Bouton 4 appuyé aux coordonnées (67,83)
    Bouton 4 relaché aux coordonnées (67,83)
    Mouvement de la souris (1,-1) . Coordonnées dans la fenêtres (68,82)
    Mouvement de la souris (-1,-1) . Coordonnées dans la fenêtres (67,81)
    Mouvement de la souris (0,-1) . Coordonnées dans la fenêtres (67,80)
    Bouton 2 appuyé aux coordonnées (67,80)
    Mouvement de la souris (0,-1) . Coordonnées dans la fenêtres (67,79) - B2
    Mouvement de la souris (0,-1) . Coordonnées dans la fenêtres (67,78) - B2
    Mouvement de la souris (1,-1) . Coordonnées dans la fenêtres (68,77) - B2
    Mouvement de la souris (0,-1) . Coordonnées dans la fenêtres (68,76) - B2
    Mouvement de la souris (1,-1) . Coordonnées dans la fenêtres (69,75) - B2
    Mouvement de la souris (0,-1) . Coordonnées dans la fenêtres (69,74) - B2
    Mouvement de la souris (0,-1) . Coordonnées dans la fenêtres (69,73) - B2
    Mouvement de la souris (1,-1) . Coordonnées dans la fenêtres (70,72) - B2
    Mouvement de la souris (0,-1) . Coordonnées dans la fenêtres (70,71) - B2
    Mouvement de la souris (1,-1) . Coordonnées dans la fenêtres (71,70) - B2
    Mouvement de la souris (1,-1) . Coordonnées dans la fenêtres (72,69) - B2
    Mouvement de la souris (0,-1) . Coordonnées dans la fenêtres (72,68) - B2
    Mouvement de la souris (1,-1) . Coordonnées dans la fenêtres (73,67) - B2
    Mouvement de la souris (1,-1) . Coordonnées dans la fenêtres (74,66) - B2
    Mouvement de la souris (0,-1) . Coordonnées dans la fenêtres (74,65) - B2
    Mouvement de la souris (1,-1) . Coordonnées dans la fenêtres (75,64) - B2
    Mouvement de la souris (1,0) . Coordonnées dans la fenêtres (76,64) - B2
    Mouvement de la souris (1,-1) . Coordonnées dans la fenêtres (77,63) - B2
    Mouvement de la souris (1,-1) . Coordonnées dans la fenêtres (78,62) - B2
    Mouvement de la souris (1,-1) . Coordonnées dans la fenêtres (79,61) - B2
    Mouvement de la souris (1,-1) . Coordonnées dans la fenêtres (80,60) - B2
    Mouvement de la souris (1,0) . Coordonnées dans la fenêtres (81,60) - B2
    Mouvement de la souris (1,-1) . Coordonnées dans la fenêtres (82,59) - B2
    Mouvement de la souris (1,-1) . Coordonnées dans la fenêtres (83,58) - B2
    Mouvement de la souris (1,-1) . Coordonnées dans la fenêtres (84,57) - B2
    Mouvement de la souris (1,0) . Coordonnées dans la fenêtres (85,57) - B2
    Mouvement de la souris (1,-1) . Coordonnées dans la fenêtres (86,56) - B2
    Mouvement de la souris (0,-1) . Coordonnées dans la fenêtres (86,55) - B2
    Mouvement de la souris (1,0) . Coordonnées dans la fenêtres (87,55) - B2
    Bouton 2 relaché aux coordonnées (87,55)
    Mouvement de la souris (0,-1) . Coordonnées dans la fenêtres (87,54)
    Mouvement de la souris (1,0) . Coordonnées dans la fenêtres (88,54)
    Bouton 2 appuyé aux coordonnées (88,54)
    Mouvement de la souris (1,-1) . Coordonnées dans la fenêtres (89,53) - B2
    Mouvement de la souris (0,-1) . Coordonnées dans la fenêtres (89,52) - B2
    Bouton 2 relaché aux coordonnées (89,52)
    Bouton 3 appuyé aux coordonnées (89,52)
    Mouvement de la souris (1,1) . Coordonnées dans la fenêtres (90,53) - B3
    Mouvement de la souris (4,2) . Coordonnées dans la fenêtres (94,55) - B3
    Mouvement de la souris (3,1) . Coordonnées dans la fenêtres (97,56) - B3
    Mouvement de la souris (1,1) . Coordonnées dans la fenêtres (98,57) - B3
    Mouvement de la souris (1,1) . Coordonnées dans la fenêtres (99,58) - B3
    Mouvement de la souris (1,1) . Coordonnées dans la fenêtres (100,59) - B3
    Mouvement de la souris (1,1) . Coordonnées dans la fenêtres (101,60) - B3
    Mouvement de la souris (1,0) . Coordonnées dans la fenêtres (102,60) - B3
    Mouvement de la souris (0,1) . Coordonnées dans la fenêtres (102,61) - B3
    Mouvement de la souris (1,1) . Coordonnées dans la fenêtres (103,62) - B3
    Mouvement de la souris (1,3) . Coordonnées dans la fenêtres (104,65) - B3
    Mouvement de la souris (1,1) . Coordonnées dans la fenêtres (105,66) - B3
    Mouvement de la souris (1,1) . Coordonnées dans la fenêtres (106,67) - B3
    Mouvement de la souris (1,1) . Coordonnées dans la fenêtres (107,68) - B3
    Mouvement de la souris (1,2) . Coordonnées dans la fenêtres (108,70) - B3
    Mouvement de la souris (0,1) . Coordonnées dans la fenêtres (108,71) - B3
    Mouvement de la souris (0,1) . Coordonnées dans la fenêtres (108,72) - B3
    Mouvement de la souris (1,1) . Coordonnées dans la fenêtres (109,73) - B3
    Mouvement de la souris (0,1) . Coordonnées dans la fenêtres (109,74) - B3
    Mouvement de la souris (0,1) . Coordonnées dans la fenêtres (109,75) - B3
    Mouvement de la souris (0,1) . Coordonnées dans la fenêtres (109,76) - B3
    Mouvement de la souris (0,1) . Coordonnées dans la fenêtres (109,77) - B3
    Mouvement de la souris (0,1) . Coordonnées dans la fenêtres (109,78) - B3
    Mouvement de la souris (0,1) . Coordonnées dans la fenêtres (109,79) - B3
    Mouvement de la souris (0,1) . Coordonnées dans la fenêtres (109,80) - B3
    Mouvement de la souris (0,1) . Coordonnées dans la fenêtres (109,81) - B3
    Mouvement de la souris (0,1) . Coordonnées dans la fenêtres (109,82) - B3
    Mouvement de la souris (0,1) . Coordonnées dans la fenêtres (109,83) - B3
    Mouvement de la souris (0,1) . Coordonnées dans la fenêtres (109,84) - B3
    Mouvement de la souris (0,1) . Coordonnées dans la fenêtres (109,85) - B3
    Mouvement de la souris (1,2) . Coordonnées dans la fenêtres (110,87) - B3
    Bouton 3 relaché aux coordonnées (110,87)
    Mouvement de la souris (1,1) . Coordonnées dans la fenêtres (111,88)
    Mouvement de la souris (0,1) . Coordonnées dans la fenêtres (111,89)
    Mouvement de la souris (1,0) . Coordonnées dans la fenêtres (112,89)
    Mouvement de la souris (1,1) . Coordonnées dans la fenêtres (113,90)
    Mouvement de la souris (1,1) . Coordonnées dans la fenêtres (114,91)
    Mouvement de la souris (1,1) . Coordonnées dans la fenêtres (115,92)
    Mouvement de la souris (0,2) . Coordonnées dans la fenêtres (115,94)
    Mouvement de la souris (0,1) . Coordonnées dans la fenêtres (115,95)
    Mouvement de la souris (0,1) . Coordonnées dans la fenêtres (115,96)
    Mouvement de la souris (0,1) . Coordonnées dans la fenêtres (115,97)
    Mouvement de la souris (0,1) . Coordonnées dans la fenêtres (115,98)
    Mouvement de la souris (0,2) . Coordonnées dans la fenêtres (115,100)
    Mouvement de la souris (0,1) . Coordonnées dans la fenêtres (115,101)
    Mouvement de la souris (0,1) . Coordonnées dans la fenêtres (115,102)
    Mouvement de la souris (0,1) . Coordonnées dans la fenêtres (115,103)        

### Gestion des joysticks

La gestion des joysticks est un peu différente de la gestion de la
souris et du clavier. Nous avons déjà vu qu'il fallait initialiser la
SDL spécifiquement avec `SDL_INIT_JOYSTICK`. Mais ça n'est pas tout. Un
joystick a besoin d'être ouvert avant d'être utilisé.
Pour cela, j'ai écrit une fonction d'initialisation `init_joysticks()`.

#### Initialisation

L'initialisation des joysticks dans ce programme est limité aux quatre
premiers joysticks trouvés. La fonction est très facilement extensible
au nombre de joysticks que vous voulez supporter. Quatre me semble pas
mal et de toute façon, je n'en ai que deux pour tester.


La fonction commence par demander à SDL combien de joysticks ont été
trouvés. Pour chaque joystick, on affiche son nom avec
`SDL_JoystickName()`, puis ou l'ouvre avec `SDL_JoystickOpen()`.
À partir de ce moment, le joystick est géré par SDL.

Pour tester quelques autres fonctions, on affiche le nombre d'axes
et de boutons trouvés. La SDL gère aussi les « chapeaux » et
les « trackballs » de certains joysticks.

Le pointeur récupéré par l'ouverture du joystick est stocké dans
une table globale. Il sera utilisé pour fermer proprement
le joystick lors de la sortie du programme, avec l'appel
à la fonction `close_joysticks()`.

Dans un programme plus conséquent, ce stockage dans un tableau global est
une mauvaise idée. Le changement à faire est laissé comme exercice.

#### Boutons d'un joystick

La gestion des boutons d'un joystick n'offre pas de problème.
Elle est très similaire à la gestion des boutons de la souris
et des touches du clavier.

Les deux évènements à surveiller sont `SDL_JOYBUTTONDOWN` et
`SDL_JOYBUTTONUP`, le type de la structure d'évènement est
`SDL_JoyButtonEvent`. Dans ce programme, je traite ces
évènements dans la fonction `joystick_button_event()`.

Le bouton est identifié par un numéro stocké dans le champ button de
l'évènement, son état dans le champ `state` et le joystick est
identifié par son numéro dans le champ `which`.

Ce qui est et n'est pas un bouton dépend du joystick. C'est entre
autre pour cela que la plupart des jeux permettent de changer les
actions liées aux boutons.

#### Axes d'un joystick

Les axes d'un joystick sont les boutons qui donnent une direction.
Sur un joypad numérique, il s'agit de la croix directionnelle,
sur un joystick analogique, il s'agit de la direction du joystick.

**Note** : il arrive que les crois directionnelles soient traitées
sous forme de `hat`.

L'évènement à surveiller est `SDL_JOYAXISMOTION`, qui appelle dans ce
programme `joystick_axis_event()`.

La fonction appelée est minimaliste. J'y affiche le numéro du joystick
source de l'évènement (champ which), le numéro de l'axe
(champ `axis`) et sa valeur (champ `value`).

Les axes sont désignés par des numéros, on ne sait pas là non plus
quel axe représente quelle direction, cela dépend des joysticks.
Il faut donc permettre à l'utilisateur de choisir ses axes. Le plus
souvent, l'axe 0 est l'axe horizontal et l'axe 1 l'axe vertical.

La valeur est comprise entre `-32767` et `32767`. Pour un joystick
numérique, ce sont ces valeurs qui seront envoyées.
Pour un joystick analogique, les valeurs seront envoyées dans tout
cette plage de valeurs, le 0 étant l'état « centre ».

Par exemple, sur un joypad que j'ai testé, la valeur -1014 sur
l'axe 3 correspond à : axe vertical du mini-joystick droite,
un peu vers le bas.

À noter aussi que sur les joypads analogique/numérique,
l'affectation des axes peut changer suivant si le pad est en mode
numérique ou analogique. Par exemple, toujours sur le même testé,
je trouve 6 axes dans un mode. Deux axes pour chaque des deux
mini-joysticks et deux axes pour la croix directionnelle.
Lorsque je change de mode, le mini-joystick gauche ne génère plus
d'évènements, et ses numéros d'axes (0 et 1) sont utilisés par la
croix directionnelle (qui dans le premier mode utilisait les
numéros 4 et 5).


#### Résultat

Voici ce que donne la détection et l'utilisation de deux joypads. On remarquera que juste après la détection, des évènements sont générés par les périphériques alors que je ne touche à rien (on ne peut pas le constater sur la copie de la sortie). Il s'agit de l'état initial des boutons et des axes. Quand je change de mode sur le joypad numérique/analogique, la valeur des axes changés est aussi envoyée.

    Nombre de joysticks detectes : 2
    Nom du joystick 0 : Mega World USB 2-Axis 8-Button Gamepad
    Nombre d'axes : 2 
    Nombre de boutons : 8 
    ... joystick ouvert 
    Nom du joystick 1 : THRUSTMASTER FireStorm Dual Power
    Nombre d'axes : 7 
    Nombre de boutons : 13 
    ... joystick ouvert
    Bouton 0 du joystick 0 : relache
    Bouton 1 du joystick 0 : relache
    Bouton 2 du joystick 0 : relache
    Bouton 3 du joystick 0 : relache
    Bouton 4 du joystick 0 : relache
    Bouton 5 du joystick 0 : relache
    Bouton 6 du joystick 0 : relache
    Bouton 7 du joystick 0 : relache
    Axe 0 du joystick 0 : 0
    Axe 1 du joystick 0 : 0
    Bouton 0 du joystick 1 : relache
    Bouton 1 du joystick 1 : relache
    Bouton 2 du joystick 1 : relache
    Bouton 3 du joystick 1 : relache
    Bouton 4 du joystick 1 : relache
    Bouton 5 du joystick 1 : relache
    Bouton 6 du joystick 1 : relache
    Bouton 7 du joystick 1 : relache
    Bouton 8 du joystick 1 : relache
    Bouton 9 du joystick 1 : relache
    Bouton 10 du joystick 1 : relache
    Bouton 11 du joystick 1 : relache
    Bouton 12 du joystick 1 : relache
    Axe 0 du joystick 1 : 0
    Axe 1 du joystick 1 : -676
    Axe 2 du joystick 1 : 0
    Axe 3 du joystick 1 : 0
    Axe 4 du joystick 1 : -32767
    Axe 5 du joystick 1 : 0
    Axe 6 du joystick 1 : 0
    Axe 1 du joystick 1 : -1352
    == FIN DE L'INITIALISATION ==
    Axe 0 du joystick 0 : -32767
    Axe 0 du joystick 0 : 0
    Axe 1 du joystick 0 : -32767
    Axe 1 du joystick 0 : 0
    Axe 1 du joystick 0 : 32767
    Axe 1 du joystick 0 : 0
    Bouton 1 du joystick 0 : appuye
    Bouton 1 du joystick 0 : relache
    Bouton 3 du joystick 0 : appuye
    Bouton 3 du joystick 0 : relache
    Bouton 0 du joystick 0 : appuye
    Bouton 0 du joystick 0 : relache
    Axe 1 du joystick 1 : -676
    Axe 3 du joystick 1 : -6081
    Axe 3 du joystick 1 : -13513
    Axe 2 du joystick 1 : 1351
    Axe 3 du joystick 1 : -20607
    Axe 2 du joystick 1 : 6756
    Axe 3 du joystick 1 : -26688
    Axe 2 du joystick 1 : 13512
    Axe 3 du joystick 1 : -32767
    Axe 2 du joystick 1 : 16215
    Axe 2 du joystick 1 : 16890
    Axe 2 du joystick 1 : 20268
    Axe 2 du joystick 1 : 24998
    Axe 3 du joystick 1 : -32431
    Axe 2 du joystick 1 : 29727
    Axe 3 du joystick 1 : -27701
    Axe 2 du joystick 1 : 32767
    Axe 3 du joystick 1 : -21283
    Axe 3 du joystick 1 : -10473
    Axe 3 du joystick 1 : -3041
    Axe 3 du joystick 1 : 0
    Axe 2 du joystick 1 : 0
    Bouton 1 du joystick 1 : appuye
    Bouton 1 du joystick 1 : relache
    Bouton 3 du joystick 1 : appuye
    Bouton 3 du joystick 1 : relache
    Bouton 8 du joystick 1 : appuye
    Bouton 8 du joystick 1 : relache
    Joystick 0 ferme
    Joystick 1 ferme
        

Évènements et actions
---------------------

Cet article aborde l'aspect technique de la lecture des périphériques
avec la SDL. Cependant, lire les périphériques n'est que le premier pas,
le second est de les associer à des actions dans le programme. Pour
prendre un exemple : détecter que la flèche droite a été appuyé est la
détection de l'évènement. Faire bouger le personnage à l'écran vers la
droite est l'action.

Il y a de nombreuses façons de faire, et ce n'est pas le sujet de
l'article. Je vais néanmoins donner quelques pistes.

Je conseille tout d'abord de bien marquer cette séparation. Ainsi, les
associations seront plus faciles à changer, et il sera plus facile de
programmer un paramétrage utilisateur des touches. Il vaut mieux donc
éviter d'appeler les fonctions d'action directement dans le `switch()`
qui surveille les évènements, il est préférable d'appeler des fonctions
qui, elles se chargeront de traduire la touche enfoncée en action pour
le programme.

C'est là que les possibilités sont multiples. Vous pouvez, dans les
fonctions de traitement d'évènements, avoir des tests en dur sur
certaines touches pour appelées les fonctions d'action. Vous pouvez
aussi aller chercher dans une table d'indirection un type d'action à
appeler, ou même directement un pointeur sur une fonction à appeler,...

Comme on peut le constater, les méthodes sont multiples et dépendent de
la complexité du programme et des possibilités que l'on veut avoir.

Conclusion
----------

Cet article a présenté les différents périphériques pouvant être gérés
par la SDL et une manière de les gérer, afin de rendre un programme
interactif.

Il n'y a rien de bien compliqué, il faut surtout penser à bien dissocier
les différents éléments : la lecture des évènements, l'appel de
différentes fonctions pour traiter ces évènements et l'appel des
traitements des actions déduites des entrées.

Bonne programmation.

Crédits 
--------

- Tutorial écrit par [Mokona](https://www.puupuu.org) écrit pour Prografix.
- Date de publication : 1 mars 2004
