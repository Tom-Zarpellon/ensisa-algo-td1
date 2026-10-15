#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * TD1 — Exercice 2 : nombres premiers strictement inférieurs à n
 *
 * 1. Algorithme naïf, complexité O(n * sqrt(n))
 *    Pour chaque k de 2 à n-1, on cherche un diviseur entre 2 et sqrt(k).
 *    Chaque test coûte O(sqrt(k)) divisions, et sqrt(k) <= sqrt(n), donc
 *    le total est majoré par n * O(sqrt(n)) = O(n * sqrt(n)).
 *
 * 2. Crible d'Ératosthène
 *    Les multiples d'un nombre premier ne sont pas premiers : on les raye.
 *    Voir README.md pour la preuve que cet algorithme est en O(n log(log n)).
 */

static int est_premier(int k) {
    if (k < 2) {
        return 0;
    }

    for (int d = 2; (long long)d * d <= k; d++) {
        if (k % d == 0) {
            return 0;
        }
    }

    return 1;
}

int compter_premiers_naif(int n) {
    int compte = 0;

    for (int k = 2; k < n; k++) {
        if (est_premier(k)) {
            compte++;
        }
    }

    return compte;
}

int compter_premiers_crible(int n) {
    if (n <= 2) {
        return 0;
    }

    bool *est_premier_k = malloc((size_t)n * sizeof(*est_premier_k));
    if (est_premier_k == NULL) {
        fprintf(stderr, "Allocation impossible pour n=%d.\n", n);
        exit(1);
    }

    for (int i = 0; i < n; i++) {
        est_premier_k[i] = true;
    }
    est_premier_k[0] = false;
    est_premier_k[1] = false;

    for (int p = 2; p < n; p++) {
        if (!est_premier_k[p]) {
            continue;
        }

        /* p*p calculé en 64 bits : au-delà, les multiples sont déjà >= n. */
        long long multiple = (long long)p * (long long)p;
        if (multiple >= n) {
            break;
        }

        for (; multiple < n; multiple += p) {
            est_premier_k[multiple] = false;
        }
    }

    int compte = 0;
    for (int i = 2; i < n; i++) {
        if (est_premier_k[i]) {
            compte++;
        }
    }

    free(est_premier_k);
    return compte;
}

static int verifier(void) {
    const int n[] = {0, 1, 2, 3, 10, 100, 1000, 10000};
    const int attendu[] = {0, 0, 0, 1, 4, 25, 168, 1229};
    int nb = (int)(sizeof(n) / sizeof(n[0]));

    for (int i = 0; i < nb; i++) {
        int a = compter_premiers_naif(n[i]);
        int b = compter_premiers_crible(n[i]);

        if (a != attendu[i] || b != attendu[i]) {
            fprintf(stderr,
                    "Echec : pi(%d) naif=%d crible=%d attendu=%d\n",
                    n[i], a, b, attendu[i]);
            return 0;
        }
    }

    if (compter_premiers_crible(100000) != 9592 ||
        compter_premiers_crible(1000000) != 78498) {
        fprintf(stderr, "Echec : le crible ne retrouve pas pi(10^5) ou pi(10^6).\n");
        return 0;
    }

    return 1;
}

static int lire_n(const char *texte, int *n) {
    char *fin = NULL;
    long valeur = strtol(texte, &fin, 10);

    if (fin == texte || *fin != '\0' || valeur < 0 || valeur > 100000000L) {
        return 0;
    }

    *n = (int)valeur;
    return 1;
}

static void afficher_ligne(int n) {
    clock_t debut;
    int naif;
    int crible;
    double temps_naif;
    double temps_crible;

    debut = clock();
    naif = compter_premiers_naif(n);
    temps_naif = (double)(clock() - debut) / CLOCKS_PER_SEC;

    debut = clock();
    crible = compter_premiers_crible(n);
    temps_crible = (double)(clock() - debut) / CLOCKS_PER_SEC;

    if (naif != crible) {
        fprintf(stderr, "Echec : naif=%d et crible=%d pour n=%d\n", naif, crible, n);
        exit(1);
    }

    printf("%8d | %8d | %12.6f | %12.6f\n", n, crible, temps_naif, temps_crible);
}

static void mode_emploi(const char *programme) {
    fprintf(stderr,
            "Usage : %s [n]\n"
            "  sans argument : comparaison pour n = 10^5, 10^6 et 10^7\n"
            "  n             : entier entre 0 et 100000000\n",
            programme);
}

int main(int argc, char **argv) {
    printf("Exercice 2 — Nombres premiers strictement inferieurs a n\n");
    printf("Temps mesures avec clock(), en secondes.\n\n");

    if (!verifier()) {
        return 1;
    }

    printf("Verification : pi(10)=4, pi(100)=25, pi(1000)=168, pi(10^6)=78498.\n\n");
    printf("       n |  nombre |    naif (s) |  crible (s)\n");
    printf("---------+---------+-------------+-------------\n");

    if (argc == 1) {
        afficher_ligne(100000);
        afficher_ligne(1000000);
        afficher_ligne(10000000);
    } else if (argc == 2) {
        int n;

        if (!lire_n(argv[1], &n)) {
            mode_emploi(argv[0]);
            return 1;
        }

        afficher_ligne(n);
    } else {
        mode_emploi(argv[0]);
        return 1;
    }

    printf("\nNaif   : O(n * sqrt(n)), un test de divisibilite par entier.\n");
    printf("Crible : O(n * log(log n)), preuve dans README.md.\n");
    return 0;
}
