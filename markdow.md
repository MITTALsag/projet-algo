

## Question 6

Si $i \neq j$, alors il n’y a pas collision car  
$$
\text{pass}_{x,i+1} = (h \circ r_i)(\text{pass}_{x,i}) \quad \text{et} \quad \text{pass}_{x,j+1} = (h \circ r_j)(\text{pass}_{x,j})
$$  
En général (sauf si $r_i(\text{pass}_{x,i}) = r_j(\text{pass}_{y,j})$), on a donc $\text{pass}_{x,i+1} \neq \text{pass}_{x,j+1}$, et par conséquent les suites des chaînes restent différentes.

En revanche, si $i = j$, il y a bien collision, et dès lors les chaînes à partir de $i$ et $j$ pour $x$ et $y$ coïncident jusqu’à $L$.  
Ce cas est problématique pour les mêmes raisons que celles évoquées dans la partie 2 : si $inputhash$ se situe avant la collision, il se peut que le candidat obtenu ne corresponde pas au résultat attendu.


## Question 7

### Analyse de la probabilité de collision

Pour que $\text{pass}_{x,L} = \text{pass}_{y,L}$, il faut qu'à un certain moment les deux chaînes se rejoignent, c'est-à-dire qu'il existe un indice $i$ tel que $\text{pass}_{x,i} = \text{pass}_{y,i}$.

En effet, par déterminisme des fonctions $h$ et $r$ :
- Si $\text{pass}_{x,i} = \text{pass}_{y,i}$, alors les chaînes restent identiques jusqu'à la fin : $\text{pass}_{x,L} = \text{pass}_{y,L}$.
- Si $\text{hash}_{x,j} = \text{hash}_{y,j}$, alors $\text{pass}_{x,j+1} = r(\text{hash}_{x,j}) = r(\text{hash}_{y,j}) = \text{pass}_{y,j+1}$, et on retombe sur le cas précédent.

**Simplification** : On peut donc se limiter au calcul des collisions sur les mots de passe, car toute collision sur les hash entraîne nécessairement une collision sur les mots de passe à l'étape suivante.

Soit $X = C^M$ le nombre de mots de passe possibles.

Pour des $\text{pass}_{x,0}$ et $\text{pass}_{y,0}$ choisis aléatoirement et différents, cherchons la probabilité qu'elles convergent à un moment donné de la chaîne.

### Calcul de la probabilité de collision

**Approche récursive**

Soit $A_i$ l'événement : "les deux chaînes ont le même mot de passe à l'étape $i$".

On veut calculer $P(A_L)$.

- **Étape initiale** : $P(A_0) = 0$ (les mots de passe initiaux sont différents par construction)

- **Étape 1** : 
  $$
  P(A_1) = P(\text{pass}_{x,1} = \text{pass}_{y,1}) = \frac{1}{X}
  $$
  car la fonction de réduction $r$ appliquée aux hashs produit une sortie uniforme.

- **Étape 2** :
  - Si $A_1$ est vrai (probabilité $\frac{1}{X}$), alors $P(A_2 | A_1) = 1$ (les chaînes restent fusionnées)
  - Si $A_1$ est faux (probabilité $1 - \frac{1}{X}$), alors $P(A_2 | \overline{A_1}) = \frac{1}{X}$
  
  Donc par la formule des probabilités totales :
  $$
  P(A_2) = P(A_1) \cdot 1 + P(\overline{A_1}) \cdot \frac{1}{X} = \frac{1}{X} + \left(1 - \frac{1}{X}\right) \cdot \frac{1}{X} = 1 - \left(1 - \frac{1}{X}\right)^2
  $$

- **Formule générale** : Par récurrence, on montre que :
  $$
  P(A_i) = 1 - \left(1 - \frac{1}{X}\right)^i
  $$

**Preuve par récurrence** :

- **Initialisation** : Pour $i=1$, $P(A_1) = \frac{1}{X} = 1 - (1 - \frac{1}{X})^1$ 

- **Hérédité** : Supposons $P(A_i) = 1 - (1 - \frac{1}{X})^i$

$
P(A_{i+1}) = P(A_i) \cdot 1 + P(\overline{A_i}) \cdot \frac{1}{X}
    = P(A_{i}) + (1 - P(A_{i})) \cdot \frac{1}{X}
    = 1 - \left(1 - \frac{1}{X} \right)^i + \left(1 - \frac{1}{X} \right)^i \cdot \frac{1}{X}
    = 1 - \left(1 - \frac{1}{X} \right)^i \left(1 - \frac{1}{X} \right)
    = 1 - \left(1 - \frac{1}{X} \right)^{i+1}
$

D'où la récurrence.


**Donc la probabilité exacte** que deux chaînes distinctes aient le même mot de passe final est :
$$
P(\text{pass}_{x,L} = \text{pass}_{y,L}) = 1 - \left(1 - \frac{1}{X}\right)^L
$$



### Ordre de grandeur du nombre de chaînes

Considérons $N$ chaînes avec des $\text{pass}_{x,0}$ tous distincts. On veut estimer combien de chaînes on peut insérer avant que les collisions deviennent probables.

**Approche par construction séquentielle conditionnelle** :

On construit les chaînes une par une, en vérifiant à chaque étape que la nouvelle chaîne ne crée pas de collision avec les chaînes déjà présentes.

Soit $E_N$ l'événement : "les $N$ premières chaînes ont toutes des $\text{pass}_{x,L}$ distincts".

La probabilité d'obtenir $N$ chaînes sans aucune collision se calcule récursivement :

- $P(E_1) = 1$ (une seule chaîne, aucune collision possible)
- $P(E_2) = P(E_1) \times P(\text{chaîne 2 réussit} | E_1) = 1 \times \left(1 - \frac{1}{X}\right)^L$
- $P(E_3) = P(E_2) \times P(\text{chaîne 3 réussit} | E_2) = \left(1 - \frac{1}{X}\right)^L \times \left(1 - \frac{1}{X}\right)^{2L}$
- ...

En généralisant :
$$
P(E_N) = \prod_{k=1}^{N-1} \left(1 - \frac{1}{X}\right)^{kL} = \left(1 - \frac{1}{X}\right)^{L \cdot \sum_{k=1}^{N-1} k} = \left(1 - \frac{1}{X}\right)^{\frac{LN(N-1)}{2}} = exp{\left(\frac{LN(N-1)}{2} \cdot ln(1-\frac{1}{X}) \right)} = exp{\left(-\frac{LN(N-1)}{2X} \right)} = exp{\left(-\frac{LN^2}{2X} \right)}
$$
(car Pour $ N(N-1) \approx N^2 \quad \text{et} \quad ln(1-\frac{1}{X}) \approx - \frac{1}{X} \quad \text{Pour N grand et X grand}$)


**Seuil de difficulté** :

On considère qu'il devient "difficile" d'ajouter des chaînes sans collision lorsque $P(E_N)$ devient trop faible. Si on fixe un seuil de $P(E_N) > 0.5$ :

$$
P(E_N) > 0.5
$$
$$
\Leftrightarrow exp{\left(-\frac{LN^2}{2X} \right)} > 0.5
$$
$$
\Leftrightarrow -\frac{LN^2}{2X} > \ln(0.5)
$$
$$
\Leftrightarrow -\frac{LN^2}{2X}> -\ln(2)
$$
$$
\Leftrightarrow N^2 < \frac{2X}{L} \ln(2)
$$
$$
\Leftrightarrow N < \sqrt{\frac{2X}{L} \ln(2)}
$$

**Application numérique** pour $M = 6$ :
- $X = 26^6 \approx 3.09 \times 10^8$
- $L = 1000$
- $N_{\text{max}} \approx \sqrt{\frac{2 \times 3.09 \times 10^8}{1000} \times 0.693} \approx \sqrt{428,000} \approx 654$

**Conclusion** :

L'approche par construction séquentielle nous ramène au même ordre de grandeur que l'approche par paires : on peut insérer environ 600-700 chaînes dans une rainbow table avant que les collisions ne deviennent probables.

Cette analyse confirme que pour les paramètres de l'énoncé ($M=6$, $L=1000$, $N=100,000$), il serait très difficile de créer une seule table avec 100,000 chaînes sans collisions fréquentes, justifiant ainsi l'utilisation de multiples tables ($R=10$).