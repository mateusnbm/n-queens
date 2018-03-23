//
//  n.queens.c
//  queens
//
//  Created by Mateus Nunes de B Magalhaes on 3/22/18.
//  Copyright © 2018 mateusnbm. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

void printBoard(int *indexes, int n) {
    
    int i, j;
    
    for (i = 0; i < n; i++) {
        
        for (j = 0; j < n; j++) {
            
            printf("%c ", (indexes[j] == i ? 'Q' : '_'));
            
        }
        
        printf("\n");
        
    }
    
    printf("\n");
    
}

int canPlace(int *indexes, int i, int j) {
    
    int jx = 0;
    
    while( (i != indexes[jx]) && (abs(i-indexes[jx]) != (j-jx)) && jx < j) {
        
        jx++;
        
    }
    
    return jx == j ? 1 : 0;
    
}

void putQueen(int *indexes, int n, int nth) {
    
    int i;
    static int sth = 0;
    
    for (i = 0; i < n; i++) {
        
        if (canPlace(indexes, i, nth-1)) {
            
            indexes[nth-1] = i;
            
            if (nth == n) {
                
                printf("Solution %i: \n\n", ++sth);
                printBoard(indexes, n);
                
            }else{
                
                putQueen(indexes, n, nth+1);
                
            }
            
        }
        
    }
    
}

void queens(int n) {
    
    int *indexes = calloc(n, sizeof(int));
    
    putQueen(indexes, n, 1);
    
    free(indexes);
    
}

int main(int argc, const char * argv[]) {
    
    queens(8);
    
    return 0;
    
}

