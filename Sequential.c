// I will be using Sieve of Eratosthenes Algorithm for Prime Number counting.
/*The sieve of Eratosthenes is a method for finding prime numbers by crossing out multiples of prime numbers.
 
How it works 
1.Write down all the natural numbers in order
2.Cross out the number 1
3.Cross out every second number after 2
4.Cross out every third number after 3
5.Continue crossing out every nth number after n
6.The numbers that remain are prime
*/

//Without OpenMP

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

void sieve_of_eratosthenes(int n) {
    bool *is_prime = (bool *)malloc((n + 1) * sizeof(bool));
    for (int i = 0; i <= n; i++) {
        is_prime[i] = true;
    }
    is_prime[0] = is_prime[1] = false;

    for (int p = 2; p * p <= n; p++) {
        if (is_prime[p]) {
            for (int i = p * p; i <= n; i += p) {
                is_prime[i] = false;
            }
        }
    }

    int count = 0;
    for (int p = 2; p <= n; p++) {
        if (is_prime[p]) {
            count++;
        }
    }

    printf("Number of primes up to %d: %d\n", n, count);
    free(is_prime);
}

int main() {
    int n = 10000000; 
    clock_t start = clock();
    sieve_of_eratosthenes(n);
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Sequential execution time: %f seconds\n", time_spent);
    return 0;
}