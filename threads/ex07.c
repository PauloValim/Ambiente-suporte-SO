//
//  main.c
//  Exemplos
//
//  Created by Paulo Roberto Oliveira Valim on 23/03/17.
//  Copyright © 2017 Paulo Roberto Oliveira Valim. All rights reserved.
//
//Description
/*
 
Exemplo demonstrando o paralelismo de execução. 
Exercicio: execute o código e anote o tempo de de execução. Modifique o código de modo a não usar mais threads e realizar o for quatro
           vezes. Execute e anote o tempo de execução.
         ??? não gerou o resultado esperado  
 
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define NUM_THREADS 4
#define NUM_ITERACOES 1000000000

int variavel_compartilhada = 0;

void *funcao_thread(void *arg) {
    int i;
    
    for (i = 0; i < NUM_ITERACOES; i++) {
        variavel_compartilhada++;
    }

    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int i;

    clock_t inicio = clock();

    for (i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, funcao_thread, NULL);
    }

    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_t fim = clock();
    double tempo_execucao = (double)(fim - inicio) / CLOCKS_PER_SEC;

    printf("Variável compartilhada após as threads modificarem: %d\n", variavel_compartilhada);
    printf("Tempo de execução: %.2f segundos\n", tempo_execucao);

    return 0;
}


