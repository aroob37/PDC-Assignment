
// With OpenMP 

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <omp.h>
#include <string.h>

void sieve_of_eratosthenes_parallel(int n, int num_threads, int chunk_size) {
    char *is_prime = (char *)malloc((n + 1) * sizeof(char));
    memset(is_prime, true, (n + 1) * sizeof(char));

    double start, end;
    omp_set_num_threads(num_threads);
    start = omp_get_wtime();

    is_prime[0] = is_prime[1] = false;

    #pragma omp parallel
    {
        int p;
        for (p = 2; p * p <= n; p++) {
            if (is_prime[p]) {
                 /*Dynamic scheduling is ideal for tasks where the workload is uneven or unpredictable.
             the number of multiples of smaller primes is much larger than the number of multiples of larger primes.
             Dynamic scheduling ensures that threads are not left idle while others are still working.*/
                #pragma omp for schedule(dynamic, chunk_size)
                for (int i = p * p; i <= n; i += p) {
                    is_prime[i] = false;
                }
            }
        }
    }

    int count = 0;
     /*Static scheduling is ideal for tasks where the workload is evenly distributed across iterations. 
     Here its checking if is_prime[p] is true and incrementing a counter*/

     /*reduction clause eliminates race conditions by creating a private copy of the variable for each thread.
      Each thread works on its private copy, and at the end of the parallel region.
      The results from all threads are combined and reduced into a single value.*/
    #pragma omp parallel for reduction(+:count) schedule(static)
    for (int p = 2; p <= n; p++) {
        if (is_prime[p]) {
            count++;
        }
    }

    end = omp_get_wtime();
    printf("Threads: %d | Chunk Size: %d \nNumber of primes up to %d: %d \nTime: %.6f sec\n\n",
           num_threads, chunk_size, n, count, end - start);
    free(is_prime);
}

int main() {
    int n = 100000000;  
    int thread_counts[] = {1, 4, 8}; 
    int chunk_sizes[] = {1000, 10000, 100000};

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            sieve_of_eratosthenes_parallel(n, thread_counts[i], chunk_sizes[j]);
        }
    }

    return 0;
}
