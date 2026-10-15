# TD1 — Introduction to Algorithms

ENSISA, Informatique et Réseaux, 1re année  
Algorithm and Data Structures  
Tom Zarpellon

Les deux exercices du TD sont traités ici : algorithmes, mise en œuvre en C, comparaison des temps avec `clock()`, et preuve de complexité du crible.

## Compilation

```bash
gcc -std=c11 -Wall -Wextra -O2 -o fibonacci fibonacci.c
gcc -std=c11 -Wall -Wextra -O2 -o premiers premiers.c
```

Avec `make` : `make`, puis `make test`.

```bash
./fibonacci          # comparaison pour n = 30, 35, 40, 45
./fibonacci 20       # une valeur, entre 0 et 93
./premiers           # comparaison pour n = 10^5, 10^6, 10^7
./premiers 1000      # une valeur, entre 0 et 100000000
```

Les deux programmes vérifient d'abord quelques valeurs connues. Ils s'arrêtent avec un code de retour non nul si un résultat est faux.

`clock()` est utilisée comme demandé. Sur cette machine, `CLOCKS_PER_SEC` vaut 1000 : les temps sont donc connus à 1 ms près. Un temps affiché `0.000000` signifie « inférieur à 1 ms ».

Les mesures ci-dessous ont été obtenues avec `gcc -O2`.

---

## Exercice 1 — Fibonacci

On pose F(0) = 0, F(1) = 1 et F(n) = F(n − 1) + F(n − 2).

### 1. Algorithme naïf

```
fibonacci_naif(n) :
    si n ≤ 1 :
        retourner n
    retourner fibonacci_naif(n − 1) + fibonacci_naif(n − 2)
```

Chaque appel lance deux appels, et les mêmes termes sont recalculés de nombreuses fois. Le nombre d'appels est proportionnel à F(n), donc en Θ(φⁿ), avec φ = (1 + √5) / 2 ≈ 1,618.

### 2. Programmation dynamique

F(i) ne dépend que des deux termes précédents. On les conserve dans un tableau : chaque terme est calculé une seule fois.

```
fibonacci_dynamique(n) :
    si n ≤ 1 :
        retourner n
    fib[0] ← 0
    fib[1] ← 1
    pour i de 2 à n :
        fib[i] ← fib[i − 1] + fib[i − 2]
    retourner fib[n]
```

Complexité temporelle Θ(n), complexité spatiale Θ(n).

Les résultats tiennent dans un entier non signé de 64 bits jusqu'à n = 93 inclus. Au-delà, F(n) déborde.

### 3. Comparaison des temps

| n | F(n) | naïf (s) | dynamique (s) |
|---:|---:|---:|---:|
| 30 | 832040 | 0,001 | < 0,001 |
| 35 | 9227465 | 0,007 | < 0,001 |
| 40 | 102334155 | 0,091 | < 0,001 |
| 45 | 1134903170 | 0,884 | < 0,001 |

De n = 40 à n = 45, le temps naïf est multiplié par environ 9,7. Comme φ⁵ ≈ 11,1, cette croissance suit bien Θ(φⁿ). Un appel dynamique reste sous la précision de `clock()`, soit moins de 1 ms.

---

## Exercice 2 — Nombres premiers

On compte les nombres premiers strictement inférieurs à un entier n.

### 1. Algorithme naïf, en O(n √n)

```
est_premier(k) :
    si k < 2 :
        retourner faux
    pour d de 2 à ⌊√k⌋ :
        si k est divisible par d :
            retourner faux
    retourner vrai

compter_premiers_naif(n) :
    compte ← 0
    pour k de 2 à n − 1 :
        si est_premier(k) :
            compte ← compte + 1
    retourner compte
```

Pour un entier k, la boucle interne effectue au plus ⌊√k⌋ divisions, que l'on s'arrête au premier diviseur ou non. Comme √k ≤ √n, chaque test coûte O(√n). On répète ce test pour moins de n entiers, donc le coût total est

O(n √n).

On peut aussi l'écrire avec une somme. Le coût est majoré par une constante fois

∑_{k=2}^{n−1} √k ≤ ∫₀ⁿ √x dx = (2/3) n^(3/2) = O(n √n).

### 2. Algorithme plus efficace : crible d'Ératosthène

Un multiple d'un nombre premier supérieur ou égal à ce premier n'est pas premier. On part d'un tableau où chaque entier ≥ 2 est considéré premier, puis on raye les multiples.

```
compter_premiers_crible(n) :
    est[0 .. n−1] ← vrai
    est[0] ← faux
    est[1] ← faux
    pour p de 2 à n − 1 :
        si est[p] est faux :
            continuer
        si p² ≥ n :
            arrêter la boucle
        pour m ← p², p² + p, p² + 2p, … tant que m < n :
            est[m] ← faux
    retourner le nombre de cases encore vraies
```

On commence le rayage à p², et non à 2p : les multiples plus petits ont déjà été rayés par un facteur premier inférieur à p. On s'arrête dès que p² ≥ n. Tout composé m < n possède un facteur premier au plus égal à √m, donc strictement inférieur à √n : il a déjà été rayé.

### 3. Preuve : le crible est en O(n log(log n))

On découpe le coût en trois parties.

1. Initialiser le tableau, puis compter les cases encore vraies, coûte Θ(n).
2. La boucle externe examine chaque entier une fois, ce qui coûte aussi Θ(n). Elle s'interrompt au plus tard lorsque p > √n.
3. Le coût restant est celui du rayage. Pour chaque premier p tel que p² < n, le nombre de multiples visités est au plus ⌊(n − 1) / p⌋, donc au plus n / p.

La somme de ces visites est majorée par la somme étendue à tous les premiers p ≤ n (ajouter des termes positifs ne peut qu'augmenter la borne) :

∑_{p premier, p ≤ n} n / p = n ∑_{p premier, p ≤ n} 1 / p.

L'indication du sujet donne

∑_{p premier, p ≤ n} 1 / p ≤ log(log(n + 1)) + M,

où M est la constante de Meissel–Mertens. Dans cette formule, log est le logarithme népérien : c'est avec lui que M ≈ 0,261497 est définie.

Le rayage coûte donc

O( n (log(log(n + 1)) + M) ).

M est une constante. De plus, changer la base du logarithme ne fait que multiplier log(log n) par une constante. On obtient

O(n log(log n)).

Enfin, log(log n) tend vers l'infini avec n, donc le Θ(n) de l'initialisation et du parcours est absorbé par O(n log(log n)).

La complexité temporelle du crible est O(n log(log n)). Sa complexité spatiale est Θ(n), à cause du tableau de booléens.

### 4. Comparaison des temps

| n | nombre de premiers | naïf (s) | crible (s) |
|---:|---:|---:|---:|
| 100 000 | 9 592 | 0,004 | < 0,001 |
| 1 000 000 | 78 498 | 0,097 | 0,001 |
| 10 000 000 | 664 579 | 2,393 | 0,022 |

Pour n = 10⁷, le crible est environ 100 fois plus rapide. Les deux algorithmes donnent le même nombre, et ce nombre coïncide avec les valeurs connues de π(n) : π(10) = 4, π(100) = 25, π(1 000) = 168, π(10⁶) = 78 498, π(10⁷) = 664 579.

L'écart correspond aux complexités démontrées : O(n √n) d'un côté, O(n log(log n)) de l'autre.
