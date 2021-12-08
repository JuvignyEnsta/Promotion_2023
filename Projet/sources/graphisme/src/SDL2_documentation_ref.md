# SDL2-C++ : encapsulation C++ de la bibliothèque SDL 2

****
###### Instructions
**Flèches du haut et du bas ou bouton souris gauche ou droit** pour défiler ligne à ligne
**Pagedown et Pageup** pour défiler rapidement
**home** pour revenir à la première ligne
**end** pour afficher les vingt-cinq dernières lignes
****

## Introduction
====

Simple Directmedia Layer (SDL) est une bibliothèque écrite en C qui permet un accès *bas niveau* au son, au clavier, à la souris, au joystick, et au hardware graphique via OpenGL et Direct3D. Cette bibliothèque est utilisée par les logiciels permettant de visualiser de la vidéo, les émulateurs, des jeux (en particuliers ceux publiés par Valve utilisent SDL...). 

SDL est supportée officiellement par Windows, Mac OS X, Linux, iOS et androïd. D'autres plateformes sont également plus ou moins supportées.

SDL est écrit en langage C, est supporté nativement par le C++ et il existe des api pour divers langages dont C# et Python.

La version 2.0 de cette librairie est sous license zlib, c'est à dire qu'on peut l'utiliser gratuitement dans n'importe quel logiciel.

Vous trouverez un wiki sur cette bibliothèque sur le lien suivant : https://github.com/libsdl-org/SDL/wiki

La bibliothèque proposée ici se repose sur SDL 2.0 et profite de l'avantage offert par le C++ pour simplifier l'appel à SDL. De plus, il a été rajouté certaines fonctionnalités non présentes dans SDL 2 (dont le texte multi-ligne avec justification) pour faciliter la mise au point du projet.

Cette bibliothèque est amenée à évoluer, et si vous voulez proposer **une nouvelle fonctionnalité ou une amélioration de cette bibliothèque**, vous êtes les bienvenues !

## Présentation de SDL2-C++
====

### Initialisation et finalisation
----

Afin d'initialiser certains paramètres correctement ainsi que la bibliothèque SDL 2, il est nécessaire d'appeler une fonction d'initialisation. De même, afin de nettoyer correctement la mémoire prise par le programme, il est conseillé de finaliser la session graphique. Cela se fait à l'aide de deux fonctions qu'on trouve dans le header sdl2.hpp

....
> sdl2::init(argc, args) : 
permet d'initialiser le contexte SDL 2. Prends les argc et argv de la fonction main.
> sdl2::finalise()       : 
finalise la session en nettoyant proprement la mémoire
....

**Attention**, il faut s'assurer que tous les objets SDL 2 créés pendant la session graphique soient détruits avant l'appel de la fonction finalise, sous peine d'obtenir des erreurs mémoires. La solution la plus simple pour cela est d'initialiser sdl 2 et le finaliser dans la fonction main puis gérer les objets sdl 2 dans des fonctions, mais surtout pas dans la fonction main elle-même !

Ainsi, le fichier contenant la fonction main() ressemblera probablement aux lignes suivantes :

....
> # include "sdl2/sdl2.hpp"
> // début gère l'affichage
> void début()
> {
>     ...
> }
> // Fonction principale : que init, début et finalize d'appelés !
> int main(int argc, char* args[])
> {
>     sdl2::init(argc, args);
>     début();
>     sdl2::finalize();
>     return EXIT_SUCCESS;
> }
....

### Création d'une fenêtre graphique
----
Pour créer ensuite une fenêtre graphique, il suffit de la créer de la manière suivante :

....
> sdl2::window fenêtre(titre, dimension[, position, à afficher])
....

où les paramètres entre [ et  ] sont optionnels. Le titre est le texte qui sera affiché dans la barre de la fenêtre (et non dans la fenêtre). Quant à la dimension, c'est un tableau à deux valeurs entières donnant respectivement le nombre de pixel en largeur et en hauteur. Ainsi, on pourra créer une fenêtre contenant 800 × 600 pixels comme suit :

....
> window fenêtre("Ceci est le titre de ma fenêtre", {800,600});
....

Il est possible d'interroger une fenêtre pour connaître sa dimension :

....
> auto [w,h] = fenêtre.dimensions();
....

où w recevra le nombre de pixel en longueur et h le nombre de pixel en hauteur.

Enfin, on peut effacer tout le contenu d'une fenêtre avec la fonction cls (CLear Screen) qui par défaut rendra un fond blanc. On peut donner à la fonction cls une autre couleur de fond en lui précisant la couleur sous la forme de quatre entiers variant entre 0 et 255, donnant respectivement les composants rouge, vert, bleu et alpha (ce dernier servant à la transparence). Par exemple, pour effacer la fenêtre avec un fond noir :

....
> fenêtre.cls({0,0,0,255});
....

En ce qui concerne l'affichage des divers types d'objets sur la fenêtre, on gère tout cela au travers d'un flux d'entrée sur la fenêtre, un peut comme les std::ostream. Les objets peuvent être créés au vol ou bien à l'avance. Un exemple, dont les divers composants seront plus clairs est le suivant :

....
> fenêtre << sdl2::rectangle({10,10},{20,10}, {0xFF,0xF0,0,0xFF}, true)
>         << sdl2::line({60,100}, {20,150}, {0,0,0xFF,0xFF}) << sdl2::flush;
....

##### Remarque 1 

Une fenêtre graphique ne possède pas un repère direct comme en mathématiques. En particuliers, le point (0,0) se trouve en haut à gauche de la fenêtre et l'axe des y est dirigé vers le bas (l'axe des x est dirigé vers la droite). L'origine de cette convention provient des écrans à tube cathodique dont le canon à électron balayait l'écran du haut vers le bas. 

##### Remarque 2

Lorsque vous commencerez à vouloir afficher des "choses" sur la fenêtre, vous serez peut-être surpris de constater que rien ne s'affiche. C'est que vous avez probablement oublié de mettre à jour la fenêtre grâce à sdl2::flush qui se gère sur le flot d'entrée de la fenêtre (voir l'exemple plus haut).

### Primitives graphiques
----

Quelques primitives graphiques sont proposées par SDL 2 : segments de droites, rectangles pleins ou non, images, texte multi-lignes formatés ou non. 

#### Les segments de droites 
____

On peut définir et afficher un segment de droite en lui donnant les deux points d'extrémités et la couleur du segment. Par exemple :

....
> sdl2::line segment({0,0},{255,255}, {0xFF,0,0,0xFF});
....

définit un segment de droite allant de la coordonnée (0,0) à la cordonnée (255,255) et dont la couleur est ici rouge.

Pour afficher le segment, il suffit de le mettre dans le flux d'entrée de la fenêtre de la manière suivante :

....
> fenêtre << segment;
....

On peut bien entendu afficher un segment créé au vol, au moment de l'affichage :

....
> fenêtre << sdl2::line({0,0},{255,255}, {0xFF,0,0,0xFF});
....

#### Les rectangles
____

Le rectangle est défini par la position de son coin supérieur gauche (celui ayant les coordonnées minimales), sa dimension (nombre de pixel en largeur et hauteur), sa couleur et si il est plein ou non. Pour ce dernier paramètre, il est possible de l'omettre, et dans ce cas le rectangle sera plein.

Par exemple :

....
> // Rectangle plein en (0,0) de dimension (256,128) et de couleur cyan
> sdl2::rectange r1({0,0}, {256,128}, {0x00,0xFF,0xFF,0xFF});
> // Rectangle creux en (256,128) de dimension (128,64) et de couleur verte
> sdl2::rectangle r2({256,128}, {128,64}, {0x00, 0xFF, 0x00, 0xFF});
....

Pour l'afficher, on utilise également le flux d'entrée :

....
> // On affiche les deux rectangle dans la fenêtre
> fenêtre << r1 << r2;
....

Il est également possible de créer les rectangles au vol durant l'affichage par le flux d'entrée.

#### Les images
____

On peut charger des images aux formats png  (*Portable Netword Graphic*) et jpg (*Joint Photographic Experts Group*) et les afficher à l'écran. Il est néanmoins, pour des raisons propres à la bibliothèque SDL (utilisée pour cette bibliothèque) nécessaire de préciser la fenêtre pour laquelle l'image est destinée.

Ainsi, pour charger une image en png ou jpg (la bibliothèque reconnait automatiquement le format), on écrira :
....
> sdl2::image homer("data/homer.jpg", fenêtre);
....

Il est possible de définir une couleur comme couleur pour la transparence en paramètre supplémentaire. Par exemple, on aurait pu préciser que la couleur noire est en fait une couleur transparente de la manière suivante :

....
> sdl2::image homer("data/homer.jpg", fenêtre, {0x00,0x00,0x00,0x00});
....

Une fois l'image chargée, il est possible de positionner l'image sur la fenêtre (toujours par rapport au coin supérieur gauche de l'image), de redimensionner l'image, d'effectuer une rotation dont le centre et l'angle est précisé, de renverser l'image par symétrie horizontale ou verticale ou bien de simplement connaître la dimension de l'image.

Par exemple :
....
> // On positionne homer en (310,110)
> homer.at(310,110);
> // On redimensionne l'image pour être de taille 128 x 128 (déformation de l'image)
> homer.stretch({128,128});
> // On effectue une symétrie axiale selon l'axe Oy
> homer.flip(sdl2::image::horizontal);
> // On effectue une rotation de centre (64,64) (le milieu de l'image) et d'angle 45°
> homer.rotate({64,64}, 45);
....

Enfin, il est possible de rajouter un filtre de couleur pour *teinter* l'image :
....
> // Teinte l'image d'une couleur jaunie
> homer.color_filter({0xFF,0xF0,0xA0,0xFF});
....

En fait, le filtre consiste à effectuer une opération binaire **and** sur chaque composant couleur.

Enfin, l'affichage se fait toujours à l'aide du flux d'entrée :
....
> fenêtre << homer;
....

Bien sûr, il est également possible de créer, modifier et afficher au vol une image dans le flux d'entrée. Par exemple :
....
> fenêtre << sdl2::image("data/homer.jpg", fenêtre).at(310,110).stretch({128,128});
....

#### Les textes
____

Les textes sont en fait des images constituée à partir de chaînes de caractères et de fontes vectorielles. Ces chaînes supportent l'unicode et il est donc possible d'utiliser l'UTF 8 (accents compris donc) pour afficher du texte. Puisque les textes sont des images, *il est possible d'effectuer toutes les manipulations qu'on peut faire sur une image avec un texte*. Ainsi, on peut effectuer une rotation, un filtre, redimensionner, etc.

Mais avant de pouvoir afficher du texte, il va falloir préalablement charger des fontes vectorielles. Avec la bibliothèque est livrée (leur copyright le permet) des fontes compatibles avec l'UTF 8 (elles ne le sont pas toutes). Pour charger une fonte, il suffit de charger le fichier contenant la fonte et préciser la taille voulue avec cette fonte (n'oubliez pas que c'est du vectoriel).

Ainsi, pour charger par exemple deux fontes avec des taille différentes :
....
> // Charge une fonte "grasse" avec une taille de 28 pixels
> sdl2::font fonte_titre("./data/Lato-Bold.ttf", 28);
> // Charge une fonte "maigre" avec une taille de 28 pixels.
> sdl2::font fonte_texte("./data/Lato-Thin.ttf", 18);
....

Une fois une fonte (ou plusieurs fontes chargées), on peut définir du texte à afficher. Trois façons de faire sont disponibles.

**Si votre texte est court et tient sur une ligne**, il suffit de donner le texte, la fonte utilisée, la fenêtre destinataire( comme pour les images ) et l'encre utilisée pour écrire le texte :

Exemple pour un texte court, avec divers traitement issue de l'image :
....
> sdl2::texte titre ("Console d'application élaborée", fonte_titre, fenêtre, {0x00,0xFF,0xFF,0xFF});
> // Positionnement du texte sur la fenêtre
> titre.at(10,310);
> // La dimension permet de calculer le centre de l'image
> auto [wph, hph] = titre.dimension();
> // Rotation d'angle 30° dont le centre est au milieu du texte
> titre.rotate({wph/2,hph/2}, 30);
....

**Si votre texte est long et que vous souhaitez l'afficher sur plusieurs lignes**, deux choix s'offrent à vous.
Soit vous laissez la bibliothèque s'adapter à la hauteur de la fenêtre est afficher sur une certaine largeur votre texte sur plusieurs lignes, soit vous contrôlez la dimension occupée par votre texte. Dans les deux cas, vous pouvez contrôler la couleur du papier et de l'encre, la justification du texte (gauche, droite ou centrée), ainsi que la marge à gauche et à droite:

....
> sdl2::texte(const std::string& phrase, const font& fnt, window& win, int maxwidth, int maxheight,
>             const color& encre, const color& papier, std::int32_t justification = left_justify,
>             int marge_width = 5, int marge_height = 5) :
Affiche le texte contenu dans *phrase* avec la fonte *fnt* à destination de la fenêtre *win*, avec une largeur maximale de *maxwidth* et une hauteur maximale de *maxheight* pixel pour l'affichage. Le texte sera affiché sur fond *papier* avec une couleur *encre* et sera justifié selon la valeur de *justification* (à gauche par défaut). La marge à gauche et à droite sont définis par *marge_width* et *marge_height* (valant cinq par défaut)
> sdl2::texte(const std::string& phrase, const font& fnt, window& win, int maxwidth,
>             const color& encre, const color& papier, std::int32_t justification = left_justify,
>             int marge_width = 5, int marge_height = 0 )
Même principe que pour le constructeur précédent, sauf qu'on n'impose par une hauteur maximale (qui sera du coup le nombre de pixels en hauteur contenus dans la fenêtre).
....

Par exemple :
....
> sdl2::texte doc("Ceci est de la documentation très poussée. On peut faire des sauts de ligne, etc.\n"
>                 "Comme on peut le voir ici !", fonte_texte, fenêtre, 320, 200, {0xFF,0xFF,0x00,0xFF},
>                 {0x00,0x00,0xFF,0xFF});
> doc.at(400, 360);
> // Remarque , on connait la dimension de l'image = (maxwidth x maxheight) = (320 x 200)
> doc.rotate({160,100}, 5);
> fenêtre << doc;
....

Remarquons que les retours à la ligne sont pris en compte dans le texte passé en argument.

#### Texte formaté

Si l'affichage de texte proposé ci-dessus est suffisant dans bien des cas, il montrera très vite ses limites si on veut afficher un texte long qui ne peut être contenu sur la fenêtre entière, possédant divers formats au sein du texte (titre, chapître, italique, gras, etc.) et dont on veut afficher qu'une partie à l'écran.

C'est ici qu'intervient les textes formatés. Les textes formatés, sont des textes lus dans un fichier dont le format texte est largement inspiré du standard **markdown**. Il ne prétend pas être aussi général que lui mais permet déjà une belle présentation du texte. Ce que vous voyez à l'écran actuellement est affiché avec un texte formaté défini dans la bibliothèque.

Un fichier texte au format lu par la bibliothèque utilisera les balises suivantes :
....
Un simple dièse suivi d'un espace  et d'un texte désigne un titre (heading1)
> # Je suis un titre
# Je suis un titre
Un double dièses suivi d'un espace  et d'un texte désigne un chapître (heading2)
> ## Je suis un chapître
## Je suis un chapître
Un triple dièse suivi d'un espace et d'un texte désigne une section (heading3)
> ### Je suis une section
### Je suis une section
Un quadruple dièse suivi d'un espace désigne une sous-section (heading4)
> #### Je suis une sous-section
#### Je suis une sous-section
Avec cinq dièses suivi d'un espace, une paragraphe (ou autre si on veut)
> ##### Je suis un paragraphe
##### Je suis un paragraphe
Avec six dièses suivi d'un espace, un sous-paragraphe
> ###### Je suis un sous-paragraphe
###### Je suis un sous-paragraphe
Avec un symbole supérieur suivi d'un espace, une ligne de code :
> > #include <iostream> // Ceci est du code
> #include <iostream> // Ceci est du code
Une partie du texte commençant par un simple astérisque (collé au premier mot) et se terminant par un simple astérisque (collé au dernier mot) désigne une section du texte en *italique*
> *Ceci est en italique*
*Ceci est en italique*
De même, si on met deux astérisques, on aura une section du code en **gras** :
> **Partie du texte en gras**
**Partie du texte en gras**
Et enfin avec trois astérisques, on sera en ***gras et italique*** :
> ***Partie du texte en gras et en italique***
***Partie du texte en gras et en italique***
Une ligne avec quatre astérisques seules désigne une ligne épaisse de couleur rouge)
> En dessous, une ligne rouge épaisse
> ****
****
Une ligne avec quatre symboles = désigne une ligne un peu moins épaisse et jaune
> Un ligne jaune un peu moins épaisse :
> ====
====
Une ligne avec quatre symboles - désigne une ligne encore moins épaisse et cyan
> Une ligne assez fine et cyan :
> ----
----
Une ligne avec quatre symboles _ désigne une ligne fine et blanche :
> Une ligne fine et blanche :
> ____
____
Enfin, une ligne avec quatre points désigne une ligne fine et grise :
> Une ligne fine et grise :
> ....
....
....

On peut définir plusieurs textes formatés (avec des styles différents si on veut). Comme pour un texte, on peut donner soit la fenêtre, soit une sous-zone d'une fenêtre pour afficher le texte formaté. Il faut également préciser le répertoire (absolu ou relatif) où se trouvent les fontes à charger :

....
> sdl2::formated_text( const window& win, const std::string& path_for_font)
Défini un texte formaté qui s'affichera sur toute la fenêtre et qui ira charger ses fontes dans le path donné par *path_for_font*.
> sdl2::formated_text( const std::array<std::int32_t, 4>& z, const std::string& path_for_font)
Défini un texte formaté qui s'affichera dans une sous-zone d'une fenêtre (à préciser dans une autre fonction), dont les coordonnées supérieurs gauches sont (z[0],z[1]) et dont la dimension est (z[2],z[3]).
....

Par exemple :
...
> sdl2::formated_text fmt_text( fenêtre, "./data");
...

Par défaut, un formateur de texte formatera le texte avec des styles et des fontes par défaut (que vous voyez actuellement à l'écran), mais il est possible de modifier ces styles. Tout d'abord, chaque formateur de texte contient son propre manager de fontes qui permet de charger dans un dictionnaire les différentes fontes utilisées par les styles :
....
> fmt_text.font_manager.load_and_add(const std::string &key, 
>                                    const std::string &fonte, std::uint8_t size)
chargera en tant que *key* la fonte issue du fichier *fonte* avec la taille *size*.
....

Par exemple :
....
> fmt_text.font_manager.load_and_add("italic", "./data/Lato-Italic.ttf", 16);
va charger la fonte *Lato-Italic.ttf* en tant que clef *italic* avec une taille de 16.
....

Il est bien sûr tout à fait possible d'écraser d'anciennes fontes avec de nouvelles en donnant la même clef.

Enfin, on peut modifier certaines parties de certains style à loisir. Pour accéder au format d'un style, on utiliser la méthode *get_format* :
....
> auto& fmt = fmt_text.get_format(formated_text::style::italic);
> // Définition d'une fonte pour le style italic
> fmt.pt_fonte = fmt.font_manager["italic"];
> // Définition de la couleur du fond pour le style italique (orange):
> fmt.papier   = sdl2::color{0xFF,0xFF,0x00,0xFF};
> // Définition de l'encre pour le style italique (gris clair):
> fmt.encre    = sdl2::color{0xA0,0xA0,0xA0,0xFF};
....

Les différents styles sont : texte, italic, bold, italic_bold, heading_1, heading_2, heading_3, heading_4, heading_5, heading_6 et code

Une fois le style défini (mais ce n'est pas obligatoire, on peut garder le style par défaut), il faut alors charger le fichier contenant le texte formaté (comme expliqué ci-dessus) puis préciser  à quelle ligne on veut commencer :
....
> fmt_text.load_simplified_markdown("SDL2_documentation.md");
> fmt_text.begin_at_line(fenêtre, 0);
....

Il ne reste plus qu'à afficher le résultat dans la fenêtre à l'aide des flots d'entrée :
....
> // Remarque : le flush n'est nécessaire que si on n'a rien d'autre à afficher
> //            Il doit être la dernière commande après qu'on est tout "afficher"
> //            afin de mettre à jour la fenêtre à l'écran
> fenêtre << fmt_text << sdl2::flush;
....

Et si on veut changer de ligne, il suffit de rappeler la méthode *begin_at_line* :
....
> // Pour afficher les dix dernières lignes
> fmt_text.begin_at_line(fenêtre, fmt_text.nb_lines()-10);
....

puis de reafficher le texte dans la fenêtre (après avoir fait un cls...)
....
> fenêtre.cls({0x00,0x00,0x00,0xFF});
> fenêtre << fmt_text << sdl2::flush;
....

### Les événements

un événement est la souris qui se déplace, une touche du clavier enfoncée, un bouton de la souris pressée, etc.

Les événements sous SDL sont gérés à l'aide d'une queue où sont empilées tous les événements ayant eu lieu depuis la dernière fois où des tests ont été exécutés sur les événements.

Pour gérer les événements, il faut créer une queue d'événement :
....
> sdl2::event_queue queue;
....

Puis créer une boucle "infinie" où on va gérer les événements en les analysant un par un :
....
> bool quitter = false;
> while (not quitter)
> {
>     auto events = queue.pull();
>     for (const auto& e : events )
>     {
>         ...
>     }
> }
....

Dans la boucle, on s'occupera bien sûr des événéments mais aussi de l'affichage si besoin, du son si il y en a, etc.

Pour les événements, on a donc une boucle parcourant tous les événements qui ont eu lieu depuis le dernier appel à la fonction *pull* et en vérifiant quel est le type d'événement :
....
> ...
> for ( const auto&  e : events )
> {
>      // Si l'utilisateur a cliqué sur la croix pour fermer la fenêtre
>      if (e->kind_of_event() == sdl2::event::quit) quitter = true;
>      if (e->kind_of_event() == sdl2::event::key_down)
>      {
>           ...
>      }
> }
....

À chaque type d'événement correspond une classe qui hérite d'un type générique *event*

Les différents types d'événements et la classe correspondante sont :

**sdl2::event::quit** : A lieu quand l'utilisateur clique sur la croix pour fermer la fenêtre. La classe qui lui correspond est *sdl2::event_quit*, qui ne possède aucune méthode spécifique...

**sdl2::event::mouse_motion** : a lieu quand l'utilisateur bouge la souris (dans la fenêtre). La classe qui lui correspond est *sdl2::event_mouse_motion*. On peut interroger un objet de cette classe pour avoir le mouvement relatif de la souris depuis la dernière position lue ou bien la position de la souris dans la fenêtre. Par exemple :
....
> if (e->kind_of_event() == sdl2::event::mouse_motion)
> {
>      auto& mouse_mouv = dynamic_cast<sdl2::event_mouse_motion&>(*e);
>      // Retourne le déplacement de la souris depuis la dernière lecture de sa position
>      auto [dx,dy] = mouse_mouv.get_relative_movement();
>      // Retourne la position de la souris dans la fenêtre
>      auto [px,py] = mouse_mouv.position();
> }
....

**sdl2::event::mouse_button** : a lieu quand l'utilisateur appuie sur/relève son doigt d'un bouton de la souris. La classe correspondante est *sdl2::mouse_button*. Elle permet de savoir quel bouton a été appuyé ou relevé, son état (appuyé ou relevé), la position de la souris lorsque l'action a été faite sur le bouton. Par exemple :
....
> if (e->kind_of_event() == sdl2::event::mouse_button)
> {
>      auto& bouton = dynamic_cast<sdl2::event_mouse_button&>(*e);
>      // Si on a cliqué sur le bouton gauche (left)
>      if (bouton.button() == sdl2::event_mouse_button::left)
>      {
>          // A quelle position on a actionné le bouton gauche de la souris
>          auto [px,py] = bouton.position();
>          if (bouton.state() == sdl2::event_mouse_button::pressed) 
>          {
>             // Nombre de fois où le bouton a été appuyé rapidement (double, triple, ... cliques )    
>             auto nombre_de_clics = bouton.number_of_clicks();
>              ...    
>          }
>          // Si on a retiré le doigt du bouton :
>          if (bouton.state() == sdl2::event_mouse_button::released)
>          ...
>      }
> }
....
Les autres boutons possibles sont : *right*, *middle*, *x1* et *x2* (pour les souris de joueurs). Attention à la porté de ces valeurs (elles sont définies dans *sdl2::event_mouse_button*).

**sdl2::event::mouse_wheel** : a lieu quand l'utilisateur action la roue se trouvant au centre de la souris. La classe n'a pas encore été programmé. Sûrement dans une prochaine livraison.

**sdl2::event::key** : Cet événement a lieu quand une touche du clavier change d'état (devient appuyée ou non appuyée). La classe correspondante est *sdl2::event_keyboard*. On peut interroger cette classe pour savoir le **code** de la touche du clavier concerné (si elle n'a pas d'équivalent ascii en lettre, chiffre ou symbole) ou bien avoir le code ascii si celui-ci existe ! Comme pour la souris, on peut également savoir combien de fois une touche a été frappée rapidement et si la touche a été pressée ( *pressed* ) ou relachée ( *released* ).
Par exemple :
....
> if (e->kind_of_event() == sdl2::event::key)
> {
>      auto& key = dynamic_cast<sdl2::event_keyboard&>(*e);
>      if (key.state() == sdl2::event_keyboard::pressed)
>      {
>          if ( (key.ascii_code() == 'q') quitter = true;
>          // Si l'utilisateur a actionné la flèche du bas :
>          if (key.code() == sdl2::event_keyboard::down)
>          {
>               auto nombre_fois_frappé = key.repeat();
>               ...
>          }
>      }    
> }
....
Les autres touches dont il n'existe pas de symbole ascii et définis dans la bibliothèque sont (attention à la porté avec *sdl2::event_keyboard*) : F1, F2, F3, F4, F5, F6, F7, ..., F12, printscreen, scrolllock, insert, home, pageup, pagedown, end, right, left, down et up.

**sdl2::event::touch_finger** : Quand l'utilisateur bouge ou appuie son doigt sur l'écran tactile (si il y en a un). L'événement créée est de type *sdl2::event_touch*. On peut interroger une instance de cette classe pour connaître le déplacement normalisé du doigt (entre -1. et 1.) ou bien la position normalisé du doigt par rapport à la fenêtre (entre 0 et 1.). Remarquez que toutes ces valeurs sont des réels simple précision. On peut également connaître la pression exercée par le doigt par une valeur normalisée entre 0 et 1 (là encore une réel simple précision).
Il est possible de connaître le type d'événement associer au périphérique tactile grâce à la méthode *type_of_event()* : cela peut être de type sdl2::event::finger_motion, sdl2::event::finger_down, sdl2::event::finger_up.
Exemple :
....
> if (e->kind_of_event() == sdl2::event::touch_finger)
> {
>     auto& touch_ev = dynamic_cast<sdl2::event_touch&>(*e);
>     if (touch_ev.pression() > 0.f)
>     {
>          auto [dx,dy] = touch_ev.displacement();
>          auto [px,py] = touch_ev.where();
>          ....
>     }
....

## Pour finir

N'hésitez pas à regarder les deux exemples fournis (le lecteur de document et le test) ainsi que les sources pour vous approprier le code !