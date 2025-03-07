//With OpenMp

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <omp.h>

void sieve_of_eratosthenes_parallel(int n) {
    bool *is_prime = (bool *)malloc((n + 1) * sizeof(bool));
    #pragma omp parallel for
    for (int i = 0; i <= n; i++) {
        is_prime[i] = true;
    }
    is_prime[0] = is_prime[1] = false;

    for (int p = 2; p * p <= n; p++) {
        if (is_prime[p]) {
            #pragma omp parallel for schedule(static)
            for (int i = p * p; i <= n; i += p) {
                is_prime[i] = false;
            }
        }
    }

    int count = 0;
    #pragma omp parallel for reduction(+:count)
    for (int p = 2; p <= n; p++) {
        if (is_prime[p]) {
            count++;
        }
    }

    printf("Number of primes up to %d: %d\n", n, count);
    free(is_prime);
}

int main() {
    int n = 1000;
    double start = omp_get_wtime();
    sieve_of_eratosthenes_parallel(n);
    double end = omp_get_wtime();
    printf("Parallel execution time: %f seconds\n", end - start);
    return 0;
}