# Projet CHTI GONZALVEZ / DECHAMBE

Ce projet à pour but de créer un jeu de tir avec des pistolets laser sur différentes cibles. Il y a jusqu'à 6 joueurs, il faut donc pouvoir identifier les différents signaux provenant des pistolets sur les photodiodes.
On traitera ici de cette identification.

Plusieurs problématiques apparaissent :

* Comment isoler la fréquence de chaque joueur malgré le repliement des spectres des signaux ?
* Comment traiter le courant en sortie de la photodiode ?  
* Comment éviter d'avoir une tension négative en entrée du micro-controleur ? 

Dans une première partie on s'interessera à la gestion du signal puis à son implémentation en électronique.

## Partie Signal

Chacun des 6 pistolets va émettre une onde sinusoïdale pendant un temps maximal de 100ms, à une fréquence bien déterminée. La discrimination de chacun des 6 faisceaux va se faire par une DFT pour chacune
des 6 fréquences. Sous MATLAB, on utilisera la FFT 
(qui peut le plus peu le moins, dans le sens ou la 
FFT calcule les M points fréquentiels, alors que dans le projet, on ne calculera que 6 points fréquentiels, ceux qui
nous intéressent).
Les fréquences retenues sont :
* F1 =  85 kHz
* F2 =  90 kHz
* F3 = 95 kHz
* F4 = 100 kHz
* F5 = 115 kHz
* F6 = 120 kHz

On impose une fréquence d'échantillonage Fe = 320kHz.

On détermine donc :
* T = 1/5000 (l'écart minimum de fréquence entre 2 joueurs)
* M = T/Te avec Te = 1/Fe 

Le bruit a une amplitude de l'ordre de 10^-14 tandis que l'amplitude du signal des joueurs est de l'ordre de 10^1
Avec notre matériel, on estime qu'un bruit avec une amplitude inférieure à 10^0 est négligeable.

Tracé : 
![Figure 1 : Tracé pour fréquences parfaites](https://raw.githubusercontent.com/alexandregonzalvez/Projet-CHTI/master/images/f_parfaites.jpg)

En réalité, l'émetteur possède une imprécision sur les canaux suivants :
* F1 =  85005,9 Hz
* F3 =  94986,8 Hz
* F5 =  115015,9 Hz

Quand on trace les courbes, on obtient les résultats suivants :

![Figure 2 : Tracé pour fréquences imparfaites](https://raw.githubusercontent.com/alexandregonzalvez/Projet-CHTI/master/images/f_imparfaites.jpg)

Comme on peut le voir, le bruit est monté à 10^-1. Ceci est acceptable comme il reste en-dessous de 10^0.

Deuxième source d'erreur : 
Il se peut que la fenêtre d'analyse de la DFT soit à cheval sur le début d'une salve.
Dans ce cas la, on obtient le résultat suivant :

![Figure 3 : Tracé pour fenetre decalee](https://raw.githubusercontent.com/alexandregonzalvez/Projet-CHTI/master/images/fenetre_decalee.jpg)

On constate que c'est une catastrophe pour isoler le signal des joueurs.


En réalité, les signaux issus du pistolet ne sont pas sinusoïdaux mais carrés.
On trace la DFT d'un signal carré de fréquence 85kHz:

![Figure 3 : Tracé pour signal carre 85kHz](https://raw.githubusercontent.com/alexandregonzalvez/Projet-CHTI/master/images/carre_85kHz.jpg)

Comme on peut le voir sur le graphe, on a pleins de pics sur la DFT, ceci est dû au fait que la DFT d'un signal carré sort la fréquence du fondamental et de ses harmoniques impaires en décroissant hyperboliquement.
On observe sur la DFT l'ensemble de ces fréquences ainsi que celles dues au repliement.

On propose de placer en amont un filtre. Son rôle est de contourner le problème relevé précédemment c'est-à-dire de supprimer les harmoniques indésirables dues à la DFT du signal carré pour ne conserver que la fréquence fondamentale et également d'éviter le phénomène de repliement du spectre (filtre anti-repliement).
Ce filtre sera passe-bas d'ordre 4.

![Figure 4 : Tracé Bode (Chebychev)](https://raw.githubusercontent.com/alexandregonzalvez/Projet-CHTI/master/images/Chebychev.PNG)
Comme on peut le voir sur le diagramme ci-dessus, le gain pour la première harmonique indésirable (le pire des cas, car la plus basse fréquence indésirable de tous les joueurs) a un gain de -50 dB, ce qui est suffisant pour l'utilisation du signal que l'on souhaite faire.

![Figure 5 : Signal en sortie du filtre (Chebychev)](https://raw.githubusercontent.com/alexandregonzalvez/Projet-CHTI/master/images/sortieFiltre.jpg)
On voit bien que le signal en sortie du filtre est sinusoïdal. L'enveloppe du signal varie un peu au début : c'est le régime transitoire (il serait préférable de ne pas observer les premières périodes)

## Partie Electronique
Pour implémenter ce filtre passe-bas d'ordre 4, on va le diviser en 2 filtres passe-bas d'ordre 2 en série dont les fonctions de transfert ont les caractéristiques suivantes : 

Pour F1(p), on a :
* fn1=53,122kHz
* m1=0.46

Pour F2(p), on a :
* fn2=114,031kHz
* m2=0.09

Voici les diagrammes de bode de ces deux filtres dans l'ordre :

![Figure 6 : Tracé bode](https://raw.githubusercontent.com/alexandregonzalvez/Projet-CHTI/master/images/trace_bode_filtres.jpg)

On va maintenant déterminer les valeurs des composants des circuits RLC pour ces deux filtres d'ordre 2 afin d'obtenir les bonnes caractéristiques (fréquence de coupure et facteur d'amortissement).

Valeur théorique->	Valeur réelle

#### F1 :
* L1 = 220 uH	->	220 uH
* R1 = 68 Ohm	->	47 Ohm
* C1 = 40 nF	->	39 nF


#### F2 :
* L2 = 1 mH		->	1 mH
* R2 = 125 Ohm	->	120 ohm
* C2 = 2 nF		->	2.2 nF

On câble ces deux filtre sur la breadboard et on relève des valeurs afin de tracer un diagramme de bode du filtre réel.
Voici le diagramme obtenu :

![Figure 7 : Tracé bode expérimental](https://raw.githubusercontent.com/alexandregonzalvez/Projet-CHTI/master/images/gain_experimental_filtres.PNG)

On peut voir que le filtre fonctionne comme attendu (2 fréquences de coupures avec résonnance car facteur d'amortissement faible pour les 2 filtres).

Pour convertir le courant généré par la photodiode lorsqu'elle est soumise à un laser en tension, nous utiliserons un montage AOP dit "transimpédance" qui transforme le courant en tension, qui est une grandeur bien plus pratique à manipuler dans ce genre de circuit électronique.

### Schéma du montage transimpédance :
![Figure 8 : Montage Transimpédance](https://raw.githubusercontent.com/alexandregonzalvez/Projet-CHTI/master/images/Transimpédance.PNG)

On détermine la valeur de la résistance de ce montage pour avoir une tension de sortie aux alentours des 200 mV pic à pic:
* R = 364 ohm	->	390 ohm

Maintenant que le signal de la photodiode est récupéré sous forme de sinusoïde grâce au montage transimpédance et au filtre, il faut l'ajuster pour qu'il soit centré sur 1,65 V afin d'être sûr qu'il soit positif pour l'entrée dans le micro-contrôleur qui effectuera la DFT.
Cette fonction sera réalisée par un ensemble de 2 montages : un premier qui supprimera la composante continue du signal pour le centrer sur 0 V et un second qui ajoutera un offset de 1,65 V. 

Ce montage peut être réalisé de la façon suivante : 
![Figure 9 : Montage Offset](https://raw.githubusercontent.com/alexandregonzalvez/Projet-CHTI/master/images/montage_offset.PNG)

En appliquant le théorème de superposition, on peut isoler ces deux montages ici combinés en un seul qui sont un filtre passe haut (pour supprimer la composante continue) et un pont diviseur de tension pour ajouter l'offset de 1,65V.
Les valeurs des composants sont donc déterminées en fonction de ces caractéristiques :
* On veut une fréquence de coupure de 20 kHz pour le filtre passe-haut
* On veut une tension de sortie de 1,65 V pour le pont diviseur de tension

On choisit d'abord la valeur de R2 arbitrairement puis on détermine les valeurs des autres composants : 
* R2 = 4,7 kohm
* R1 = 20,955 kohm  -> 22 kohm
* C = 2,05 nF       -> 2,2 nF

Le signal de sortie peut maintenant être envoyé dans le microcontroleur dans lequel il sera traité.

En résumé, voici le diagramme du montage complet (des suiveurs ont été intercalés entre certains montage pour éviter des perturbations d'un montage sur l'autre):
![Figure 10 : Diagramme](https://raw.githubusercontent.com/alexandregonzalvez/Projet-CHTI/master/images/Diagramme.PNG)
