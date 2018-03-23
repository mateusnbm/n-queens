//
//  n-queens-naive.c
//  queens
//
//  Created by Mateus Nunes de B Magalhaes on 3/22/18.
//  Copyright © 2018 mateusnbm. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#define MIN(A, B) ((A) > (B) ? (B) : (A))
#define MAX(A, B) ((A) > (B) ? (A) : (B))

void printBoard(int **board, int n) {
    
    int i, j;
    
    for (i = 0; i < n; i++) {
        
        for (j = 0; j < n; j++) {
            
            printf("%c ", (board[i][j] == 1 ? 'q' : '_'));
            
        }
        
        printf("\n");
        
    }
    
    printf("\n");
    
}

int canPlace(int **board, int n, int i, int j) {
    
    int ix, jx;
    int minI = 0, maxI = 0, minJ = 0, maxJ = 0;
    
    int kMin = MIN(i, j);
    int kMax = MAX(i, j);
    
    // Check the given position, column and row. If there is a queen
    // on one of this locations we cannot place the new one on the
    // specified coordinates.
    
    if (board[i][j] == 1)
        return 0;
    
    for (ix = 0; ix < n; ix++)
        if (board[ix][j] == 1)
            return 0;
    
    for (jx = 0; jx < n; jx++)
        if (board[i][jx] == 1)
            return 0;
    
    // Check the main diagonal.
    
    minI = i - kMin;
    maxI = i + (n - 1 - kMax);
    minJ = j - kMin;
    maxJ = j + (n - 1 - kMax);
    
    for (ix = minI, jx = minJ; ix <= maxI && jx <= maxJ; ix++, jx++)
        if (board[ix][jx] == 1)
            return 0;
    
    // Check the antidiagonal.
    
    minI = i - MIN(i, n-1-j);
    maxI = i + MIN(n-1-i, j);
    minJ = j - MIN(n-1-i, j);
    maxJ = j + MIN(i, n-1-j);
    
    for (ix = minI, jx = maxJ; ix <= maxI && jx >= minJ; ix++, jx--)
        if (board[ix][jx] == 1)
            return 0;
    
    return 1;
    
}

int putQueen(int **board, int n, int nth) {
    
    int i, j;
    int r = 0;
    
    for (i = 0; i < n; i++) {
        
        for (j = 0; j < n; j++) {
            
            if (canPlace(board, n, i, j)) {
                
                board[i][j] = 1;
                
                if (nth == n) {
                    
                    r = 1;
                    printBoard(board, n);
                    
                }else{
                    
                    if (putQueen(board, n, nth+1)) {
                        
                        i = n;
                        j = n;
                        
                    }
                    
                }
                
                board[i][j] = 0;
                
            }
            
        }
        
    }
    
    return r;
    
}

void queens(int n) {
    
    int i;
    int **board = malloc(n * sizeof(int *));
    
    for (i = 0; i < n; i++) {
        
        board[i] = malloc(n * sizeof(int));
        
    }
    
    putQueen(board, n, 1);
    
    for (i = 0; i < n; i++) {
        
        free(board[i]);
        
    }
    
    free(board);
    
}

int main(int argc, const char * argv[]) {
    
    queens(8);
    
    return 0;
    
}
