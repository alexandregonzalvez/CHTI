# Projet CHTI GONZALVEZ / DECHAMBE
Pour connaître l'avancée du projet en assembleur/C voir assembleur/readme.md

Pour la partie 1 du Projet, voir PROJET.md
## Partie 1 : Prise en main de Matlab / Simulink

On va tracer plusieurs courbes avec les paramètres suivants :
* Fenêtre d'analyse T = 1s
* Nombre de points d'échantillonage M = 32

#### Tracé des courbes  pour Fsin = 3hz
```Q4```
![Figure 1 : Tracé pour 3Hz](/images/plot_3hz.jpg)

Sur le graphe de la transformée de Fourier, on observe 2 pics de fréquence :
* 3Hz : Fsin
* 29Hz : M - Fsin

#### Tracé des courbes pour Fsin = 15hz
```Q5```
![Figure 2 : Tracé pour 15Hz](/images/plot_15hz.jpg)

Sur le graphe de la transformée de Fourier, on observe 2 pics de fréquence :
* 15Hz : Fsin
* 17Hz : M - Fsin

On constate que les deux pics de fréquences se rapprochent beaucoup par rapport au tracé pour 3Hz, on se rapproche du repliement du spectre.
En augmentant la fréquence du sinus (Fsin), on peut être amené à déterminer de fausses fréquences sur le spectre de Fourier à cause de ce phénomène.

#### Exemple avec Fsin = 29Hz

![Figure 3 : Tracé pour 29Hz](/images/plot_29hz.jpg)

On observe 2 pics:
* 3Hz : M - Fsin
* 29Hz : Fsin

Si on ne sait pas que la fréquence du sinus est 29Hz, on déduit du spectre de Fourier que la fréquence est de 3Hz : c'est le phénomène de repliement du spectre.

#### Fsin = 5Hz
```Q6```
![Figure 4 : Tracé pour 5Hz](/images/plot_5hz.jpg)

Sur le graphe de la transformée de Fourier, on observe 2 pics de fréquence :
* 5Hz : Fsin
* 27Hz : M - Fsin

Rien d'inhabituel par rapport aux derniers tracés.

#### Fsin = 5,5Hz

![Figure 5 : Tracé pour 5.5Hz](/images/plot_55hz.jpg)

```Q8```
Ici, 5.5 n'est pas un multiple de 1/T c'est pour ça qu'on obtient un spectre déformé. Pour régler le problème, il suffit de modifier T pour que 1/T soit un diviseur de 5.5 (par exemple T=2).

Avec T=2:

![Figure 6 : Bon tracé pour 5.5Hz](/images/plot_55hz_good.jpg)

On en conclut qu'il faut impérativement avoir 1/T qui divise Fsin pour avoir une analyse correcte.


