

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

Soit $A_i$ l'événement : "les deux chaînes n'ont pas le même mot de passe à l'étape $i$".

On veut calculer $P(A_L)$.

- **Étape initiale** : $P(A_0) = 1$ (les mots de passe initiaux sont différents par construction)

- **Étape k** : 
  $$
  P(A_k) = P(\text{pass}_{x,k} \neq \text{pass}_{y,k}) = \left(\frac{X-1}{X}\right)^k = \left(1 - \frac{1}{X}\right)^k
  $$
  car la fonction de réduction $r$ appliquée aux hashs produit une sortie uniforme.  
  Et il faut "éviter" une valeur pour chaque étape $i$ où $0 \leq i \leq k$.


**Donc la probabilité exacte** que deux chaînes distinctes n'aient pas le même mot de passe final est :
$$
P(\text{pass}_{x,L} \neq \text{pass}_{y,L}) = \left(1 - \frac{1}{X}\right)^L
= exp\left(L \cdot ln(1-\frac{1}{X}) \right) \approx e^{-L/X} \approx 1 - \frac{L}{X}
$$

Car $ \quad ln(1-\frac{1}{X}) \approx -\frac{1}{X} \quad \text{et} \quad  e^{-L/X} \approx 1 - \frac{L}{X}$ pour $L \ll X$.  
($L$ négligeable devant $X$).

**Donc la probabilité exacte** que deux chaînes distinctes aient le même mot de passe final est :
$$
P(\text{pass}_{x,L} = \text{pass}_{y,L}) \approx \frac{L}{X}
$$


### Cas générale avec N chaine.
On suppose qu'on a déjà $k$ chaîne dans la table. On génère une nouvelle chaîne $k+1$.
Soit maintenant $i \leq k$.  
Par le raisonnement de la Partie 1, la probabilité que la nouvelle chaîne ($k+1$) entre en collision avec la chaîne $i$ est :  

$$P(\text{collision avec la chaîne i}) \approx L/X $$

La nouvelle chaîne doit éviter chacune des k chaînes existantes.  
Si on note $B_i$ : "collision avec la chaîne i".   
On a alors :
$$
P(\text{collision}) = P(B_1 ∪ B_2 ∪ ... ∪ B_k) \approx Σ_{i=1}^{k} P(B_i) \approx k \cdot \frac{L}{X}
$$

Ainsi si on a N-1 chaînes et que l'on en insère une N-ème,  
On a :
$$
P(\text{collision}) \approx (N-1) \cdot \frac{L}{X} \approx \frac{N \cdot L}{X}
$$

Car $ N-1 \approx N $ pour N grand.


**Application numérique** (M=6, L=1000) :  

$$
P(\text{collision}) \leq 0.5 \Leftrightarrow N \leq 0.5 \cdot \frac{X}{L}
$$

Pour $X = 26^6 \quad L = 1000$  
On a : 
$$
P(\text{collision}) \leq 0.5 \Leftrightarrow N \leq 154,000$$

L'ordre de grandeur du nombre de chaînes qu'on peut facilement insérer est de N ≈ 150,000 chaînes

Avec $N = 100,000 \quad L = 1000 \quad X = 26^6$.  
$P(collision) \approx 100,000 × 1,000 / 26^6 \approx 0.32$

Interprétation : Vers la fin du remplissage de la table, environ 1 tentative sur 3 sera rejetée car le pass_{x,L} sera déjà présent.
