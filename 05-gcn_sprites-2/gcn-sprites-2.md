Affichage de sprites avec la SDL, partie 2
==========================================

Introduction
------------

Dans la partie précédente, nous avons vu comment afficher des sprites
animés à l'écran avec des vitesses indépendantes. Les sprites ne se
déplaçaient cependant pas.

Dans cette deuxième partie, nous allons voir en trois étapes comment
faire bouger ces sprites de manières indépendantes.

Avant de commencer la lecture de cette partie, je vous conseille de
lire bien évidemment la première partie de cette série d'articles. 

Les trois programmes sont disponibles dans l'archive fournie avec cet
article. Afin de les compiler, vous devez avoir la librairie SDL installée
et savoir comment compiler un programme avec cette librairie. Si vous
utilisez gcc sous un environnement Linux/Bash, vous compilerez très simplement
chaque programme de la manière suivante :
``gcc -o sprites4 sprites4.c `pkg-config sdl --cflags -libs` ``.
                                                               
Si vous utilisez `fish`: `gcc -o sprites4 sprites4.c (pkg-config sdl --cflags --libs | string split " ")`

Et de même pour les sources sprites5.c et sprites6.c. 

Première phase 
--------------

### Le déplacement 

Nous reprenons le programme là où nous nous étions arrêtés dans l'article
précédent. Nous voulons à présent que les sprites se déplacent à
l'écran. Puisque ces sprites peuvent chacun avoir leur propre déplacement,
leur vitesse est un attribut propre à chacun d'eux. Nous ajoutons
donc cette information dans la structure `Sprite`.

Les deux champs que nous ajoutons sont `int dispX` et `int dispY`,
comment « déplacement en X » et « déplacement en Y ».
À chaque tour de boucle, les coordonnées propres du sprite seront
modifiées par ces deux valeurs de déplacements afin d'obtenir
les nouvelles coordonnées.

Bien sûr, il faut initialiser ces deux nouveaux champs avec des
valeurs. La fonction `create_random_sprite()` est là pour cela. 

La fonction utilitaire `get_random()` nous sert à nouveau pour
initialiser les deux champs avec des valeurs aléatoires.
Pour `dispX`, une valeur négative signifie un déplacement vers
la gauche et une valeur positive un déplacement vers la droite.
Pour `dispY`, une valeur négative signifie un déplacement vers le
haut et une valeur positive un déplacement vers le bas. Une valeur
de 0 signifie que le sprite ne bougera pas sur l'axe concerné.

Tout le code de mise à jour de l'animation du sprite se trouve dans
`update_sprite()`. Et on y ajoute le déplacement à l'écran du sprite.

Le déplacement se fait en deux parties. Tout d'abord, on modifie
les coordonnées du sprite en fonction de informations de
déplacement sur les axes. 


**Attention**, nous faisons ici un déplacement très basique,
avec une addition toute simple. Souvenez-vous que pour être
indépendant de la vitesse d'affichage, nous avions utilisé
un timer, dont la valeur est `currentTime` dans cette fonction.
Nous devrions en toute logique effectuer des déplacements de
sprite indépendants de la vitesse d'affichage. Pour le moment,
nous nous en passerons.
               
La deuxième partie de la mise à jour des coordonnées gère les bords de
la fenêtre et les rebonds. Nos sprites vont en effet rebondir sur les
bords de la fenêtre.
               
Pour cela, nous vérifions tout simplement les coordonnées en X et en
Y de la nouvelle position du sprite. Si on est sorti des bornes de
la fenêtre, on recale le sprite à l'intérieur et on prend une nouvelle
vitesse aléatoire sur cet axe. Ce n'est donc pas un « vrai » déplacement
basé sur une simulation physique quelconque que nous faisons.
On déplace juste des sprites de manière aléatoire. 

### Résumé

Les sprites bougent à présent dans tous les sens, tout en étant animés comme
avant. On constate cependant deux choses (si vous ne le voyez pas tout de
suite, attendez un peu, ou augmentez le nombre de sprites affichés).

Tout d'abord, il y a un problème que nous avions déjà constaté avec
`sprite3.c` : les sprites qui se chevauchent s'effacent à cause du
carré noir de leur fond. C'est beaucoup plus visible maintenant
qu'ils se déplacent.

Ensuite, on peut voir que certains sprites laissent une trainée.
C'est particulièrement visible lorsqu'ils ont un grand déplacement.
Là encore, vous pouvez jouer avec la vitesse de déplacement
des sprites en l'augmentant pour bien voir ce que cela donne.
C'est normal, la fenêtre n'est jamais effacée. Si le déplacement
est petit, le carré noir du fond du sprite va effacer les traces
de l'ancienne position, mais pas si le déplacement est important.
De plus, lorsque nous voudrons régler le problème précédent,
le carré noir n'effacera plus la position précédente.

Passons donc à l'étape suivante. 

Deuxième phase
--------------

### Couleur clé et effacement

Le programme décrit est à présent `sprites5.c`, qui est basé sur `sprites4.c`.
Je rappelle que nous voulons deux choses : éviter les traces résiduelles des
sprites et que le fond noir du sprite n'efface pas ce qui est déjà à l'écran.

Pour cette partie, effaçons l'écran tout simplement avec
`SDL_FillRect(sdlMainScreen,NULL,0);`. Cette fonction remplis un rectangle
de surface décrit par un `SDL_Rect` avec la couleur spécifiée.
Ici, le rectangle est à `NULL` pour désigner l'ensemble de la surface,
et la couleur est 0, c'est à dire noire.

Voilà, c'est fait, il n'y a plus de trace résiduelle d'affichage des sprites.

Pour que le fond noir des sprites n'effacent pas ce qui est déjà dans
la surface, nous utilisons la fonction `SDL_SetColorKey()` qui permet de
désigner une couleur comme transparente. Cette transparence est du
tout ou rien. Lors de la copie de cette surface avec
`SDL_BlitSurface()`, tout pixel de la couleur spécifiée ne sera pas
recopié, tous les autres le sont.

`SDL_SetColorKey()` prend trois paramètres :

- `SDL_Surface *surface` : désigne la surface spécifiée. Ici, il s'agit
  de l'image chargée `loadedPicture` ;
- `Uint32 flag` : nous ne nous occuperons pas ici des différentes possibilités
  de ces options. Nous utilisons `SDL_SRCCOLORKEY` pour spécifier que nous
  allons donner une couleur transparente ;
- `Uint32 key` : la couleur voulue. Normalement, on devrait demander la
  valeur exacte de la couleur à la SDL, car elle change en fonction du
  format de la surface. Cependant, noir est toujours 0 (zéro) du moment
  que la surface n'est pas associée à une palette. Nous mettons donc
  directement zéro. 

Cette opération est faite une fois pour toute sur la surface, donc avant
la boucle principale.

Et voilà, les sprites ne s'effacent plus les uns les autres. 
Et voil�, les sprites ne s'effacent plus les uns les autres.

Troisième phase
---------------

### Le contexte

Pourquoi effacer tout l'écran ? Si l'écran est recouvert de sprites, pourquoi
pas, mais dans les cas les plus vraisemblables, il n'y a que quelques
sprites d'affichés. Alors pourquoi ne pas effacer uniquement les endroits
que l'on sait avoir été altéré par un affichage de sprite ?

C'est ce que nous faisons dans `sprites6.c` en modifiant légèrement `sprites5.c`.

L'idée est donc, avant de lancer l'affichage de tous les sprites,
d'effacer des surfaces de 32 par 32 pixels aux coordonnées qu'occupaient
les sprites à l'étape d'avant. Il faut donc retenir cette position précédente.

Mais **attention** ! Nous sommes en double buffer d'affichage. Qu'est-ce
que cela signifie ? 

Prenons un tour de boucle quelconque, que nous nommons frame 0. À la frame 0,
un sprite donné est affiché aux coordonnées (x0,y0) sur le buffer d'affichage 0.

À la frame 1, la suivante, le sprite est affiché aux coordonnées (x1,y1)
sur le buffer d'affichage 1, car les buffers ont été changés. Nous ne nous
occupons pas encore de l'effacement de l'ancienne position.

Nouvelle frame, les buffers permutent et nous écrivons le sprite aux coordonnées
(x2,y2). Mais c'est sur le buffer 0 que nous affichons, et à cet endroit,
le sprite a été affiché aux coordonnées (x0,y0).

On peut continuer comme cela. On comprend rapidement que lorsqu'on est sur
le point d'afficher un sprite, le sprite a éte affiché sur ce buffer deux
frames avant. Il faut donc conserver constamment les deux dernières
coordonnées auxquelles ont été affiché le sprite. Puis, avant d'afficher
le sprite à la nouvelle position, il faut effacer l'ancien, qui est à
la position la plus anciennement conservée.

Adaptons le programme. Comme la structure du Sprite est partagée avec les
autres programmes, je ne la touche pas. D'ailleurs, retenir ses anciennes
positions n'est pas vraiment de sa responsabilité. Je lui adjoins donc
une série de deux tableaux contenue dans la structure `Positions`.

Ces deux membres, `int destX[2]` et `int destY[2]` servirons à garder
une trace des anciennes coordonnées. 

Une autre modification se trouve dans `update_sprite()`.
Avant de calculer les nouvelles coordonnées du sprite, on sauve les
anciennes coordonnées. Les positions historiques d'indice 1 prennent
les valeurs des positions d'indice 0 et celles-ci prennent les
positions du sprite avant sa mise à jour.

Une nouvelle fonction fait son apparition : `erase_sprite()`.
Cette fonction prend une surface d'écran et une structure d'historique
de positions du sprite en paramètres. Elle efface sur le buffer donné une
zone de 32 par 32 pixels aux coordonnées sauvées dans le tableau
des coordonnées en indice 1. 

### Mise en œuvre

Nous avons donc maintenant une fonction qui met à jour la structure du sprite,
une fonction qui efface le sprite dans la position enregistrée deux frames
avant et une fonction pour afficher le sprite aux coordonnées
actuelles. Mettons tout ça ensemble après avoir enlevé l'effacement
d'écran que nous avions dans `sprites5.c`.

Une erreur serait de tout mettre dans la boucle qui traite tous les
sprites. L'erreur est tentante, donc nous allons l'examiner.
Cela donnerait ceci :

    for (int i = 0; i < NB_SPRITES; i++)
    {
        update_sprite(arraySprites[i], spriteOldPositions[i], currentTime);
        erase_sprite(screen, spriteOldPositions[i]);
        display_sprite(screen, arraySprites[i]);
    }

Cela fait : mise à jour du sprite, effacement de l'ancienne position,
affichage à la nouvelle position.

Dit comme ça, cela semble bon. Pourtant, cela ne fonctionne pas,
certain sprites effacent d'autres sprites ! Étrange, nous avons
utilisé `SDL_SetColorKey()`; pour éviter ce problème !
Écartons tout de suite l'idée qui nous soufflerait que la SDL ne
fonctionne pas. Que reste-t-il ? L'effacement des sprites tout simplement.

Il faut voir la boucle des sprites dans sa globalité. Le premier
sprite est effacé, puis affiché à la nouvelle position.
Le second sprite est effacé, puis affiché à la nouvelle position.
Le troisième sprite est effacé,...

**Stop !** 

Que se passe-t-il si l'ancienne position d'un de ces sprites coincide avec
la nouvelle position d'un des sprites déjà affiché ? Gagné ! On efface
un sprite nouvellement affiché. Ça ne va pas.

Il faut donc séparer la boucle en deux boucles de sprites. Dans la
première, on met à jour les structure et on efface les anciennes
positions. Dans la seconde, on affiche tous les sprites. Entre
les deux boucles, on sait donc que le buffer est complètement effacé,
et on peut afficher tous les sprites de manière sereine. 

Au final
--------

Ça y est, nos sprites bougent dans tous les sens. Un peu de façon
anarchique d'ailleurs.

Nous verrons dans les prochaines parties comment mettre un peu d'ordre
dans tout ça, et comment faire que l'animation jouée dépende de la
direction du sprite.

À bientôt. 

Crédits 
--------

- Tutorial écrit par [Mokona](https://www.puupuu.org) écrit pour Prografix.
- Date de publication : 16 juin 2003
