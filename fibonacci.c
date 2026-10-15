#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * TD1 — Exercice 1 : suite de Fibonacci
 *
 * F(0) = 0, F(1) = 1, F(n) = F(n-1) + F(n-2) pour n >= 2.
 *
 * 1. Algorithme naïf
 *    On traduit la définition récursive telle quelle.
 *    Chaque appel produit deux appels, et les mêmes termes sont
 *    recalculés un grand nombre de fois.
 *    Le nombre d'appels est Theta(phi^n), avec phi = (1 + sqrt(5)) / 2.
 *
 * 2. Programmation dynamique (version ascendante)
 *    F(i) ne dépend que de F(i-1) et F(i-2). On range ces résultats
 *    dans un tableau et chaque terme n'est calculé qu'une fois.
 *    Complexité temporelle Theta(n), complexité spatiale Theta(n).
 *
 * Les valeurs sont exactes pour n <= 93 : F(93) tient dans un
 * entier non signé de 64 bits, F(94) non.
 */

#define FIB_MAX 93
#define FIB_NAIF_MAX 45

unsigned long long fibonacci_naif(int n) {
    if (n <= 1) {
        return (unsigned long long)n;
    }

    return fibonacci_naif(n - 1) + fibonacci_naif(n - 2);
}

unsigned long long fibonacci_dynamique(int n) {
    if (n <= 1) {
        return (unsigned long long)n;
    }

    unsigned long long fib[n + 1];

    fib[0] = 0;
    fib[1] = 1;

    for (int i = 2; i <= n; i++) {
        fib[i] = fib[i - 1] + fib[i - 2];
    }

    return fib[n];
}

static double mesurer_naif(int n, unsigned long long *resultat) {
    clock_t debut = clock();
    *resultat = fibonacci_naif(n);
    return (double)(clock() - debut) / CLOCKS_PER_SEC;
}

static double mesurer_dynamique(int n, unsigned long long *resultat) {
    clock_t debut = clock();
    *resultat = fibonacci_dynamique(n);
    return (double)(clock() - debut) / CLOCKS_PER_SEC;
}

static int verifier(void) {
    const int n[] = {0, 1, 2, 3, 10, 20, 30};
    const unsigned long long attendu[] = {
        0ULL, 1ULL, 1ULL, 2ULL, 55ULL, 6765ULL, 832040ULL
    };
    int nb = (int)(sizeof(n) / sizeof(n[0]));

    for (int i = 0; i < nb; i++) {
        unsigned long long a = fibonacci_naif(n[i]);
        unsigned long long b = fibonacci_dynamique(n[i]);

        if (a != attendu[i] || b != attendu[i]) {
            fprintf(stderr,
                    "Echec : F(%d) naif=%llu dynamique=%llu attendu=%llu\n",
                    n[i], a, b, attendu[i]);
            return 0;
        }
    }

    return 1;
}

static int lire_n(const char *texte, int *n) {
    char *fin = NULL;
    long valeur = strtol(texte, &fin, 10);

    if (fin == texte || *fin != '\0' || valeur < 0 || valeur > FIB_MAX) {
        return 0;
    }

    *n = (int)valeur;
    return 1;
}

static void afficher_ligne(int n, int avec_naif) {
    unsigned long long resultat_dyn = 0;
    double temps_dyn = mesurer_dynamique(n, &resultat_dyn);

    if (!avec_naif) {
        printf("%2d | %20llu | %12s | %12.6f\n",
               n, resultat_dyn, "trop long", temps_dyn);
        return;
    }

    unsigned long long resultat_naif = 0;
    double temps_naif = mesurer_naif(n, &resultat_naif);

    if (resultat_naif != resultat_dyn) {
        fprintf(stderr, "Echec : les deux algorithmes different pour n=%d\n", n);
        exit(1);
    }

    printf("%2d | %20llu | %12.6f | %12.6f\n",
           n, resultat_dyn, temps_naif, temps_dyn);
}

static void mode_emploi(const char *programme) {
    fprintf(stderr,
            "Usage : %s [n]\n"
            "  sans argument : comparaison pour plusieurs valeurs de n\n"
            "  n             : entier entre 0 et %d\n"
            "L'algorithme naif n'est chronometre que pour n <= %d.\n",
            programme, FIB_MAX, FIB_NAIF_MAX);
}

int main(int argc, char **argv) {
    printf("Exercice 1 — Fibonacci\n");
    printf("Temps mesures avec clock(), en secondes.\n\n");

    if (!verifier()) {
        return 1;
    }

    printf("Verification : F(0), F(1), F(10)=55, F(20), F(30) sont corrects.\n\n");
    printf(" n |                 F(n) |    naif (s) | dynam. (s)\n");
    printf("---+----------------------+-------------+------------\n");

    if (argc == 1) {
        const int valeurs[] = {30, 35, 40, 45};
        int nb = (int)(sizeof(valeurs) / sizeof(valeurs[0]));

        for (int i = 0; i < nb; i++) {
            afficher_ligne(valeurs[i], valeurs[i] <= FIB_NAIF_MAX);
        }
    } else if (argc == 2) {
        int n;

        if (!lire_n(argv[1], &n)) {
            mode_emploi(argv[0]);
            return 1;
        }

        afficher_ligne(n, n <= FIB_NAIF_MAX);
    } else {
        mode_emploi(argv[0]);
        return 1;
    }

    printf("\nNaif      : Theta(phi^n) appels recursifs, phi = (1+sqrt(5))/2.\n");
    printf("Dynamique : Theta(n) additions, chaque terme calcule une seule fois.\n");
    return 0;
}
