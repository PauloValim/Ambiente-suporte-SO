/*Matrix multiplication using dynamic memory allocation*/
#include <stdio.h>
#include<stdlib.h>
#include <assert.h>



void matrix_mult(int **A, int **B, int **C, int N){
    int x, y, z;
    for(x=0; x < N; x++)
    {
        for(y=0; y < N; y++)
        {
            for(z=0; z<N; z++)
                C[x][y] = C[x][y] + A[x][z] * B[z][y];
        }
    }


}




/* Main Function */
int main(int argc, char **argv){
    
    int **ptr1, **ptr2, **ptr3;
    /* Declaring indexes. */
    int i, j, k, n, is_ok;
    char *awnser[] = { "bad", "ok" };
    
    double t_a, t_b, elapsed;
    
    if(argc != 2){
        fprintf(stderr, "Usage: %s <size>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    n = atoi(argv[1]);
    
    
 //   n = n % 8 == 0 ? n : n + (8 - n % 8);
    
    printf("number of rows x cols: %i\n", n);
    

    
    /* Allocating memory for three matrix rows. */
    ptr1 = (int **) malloc(sizeof(int *) * n);
    ptr2 = (int **) malloc(sizeof(int *) * n);
    ptr3 = (int **) malloc(sizeof(int *) * n);
    /* Allocating memory for the col of three matrices. */
    for(i=0; i<n; i++)
        ptr1[i] = (int *)malloc(sizeof(int) * n);
    for(i=0; i<n; i++)
        ptr2[i] = (int *)malloc(sizeof(int) * n);
    for(i=0; i<n; i++)
        ptr3[i] = (int *)malloc(sizeof(int) * n);

    
    for(i=0; i< n; i++)
    {
        for(j=0; j< n; j++)
        {
           ptr1[i][j]=1;
           ptr2[i][j]=1;
           ptr3[i][j]=0;
        }
    }


    
    matrix_mult(ptr1, ptr2, ptr3, n);
    

    
    
    is_ok = 1;
    for(i=0; i< n; i++)
    {
        for(j=0; j< n; j++)
        {
            if (ptr3[i][j] != n){
                is_ok = 0;
                i = n;
                j = n;
            }
        }
    }

    
    printf("multiply values are %s\ntime: %.3f seconds\n", awnser[is_ok], elapsed);
    

    return(0);
    
    
    
 }