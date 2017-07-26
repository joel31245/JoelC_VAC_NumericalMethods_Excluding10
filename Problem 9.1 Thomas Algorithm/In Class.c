#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 4
#define N2 6

void initializeMat( double[N][3]);
void initializeA( double[N][3], double[N] );
void initializeF( double[N] );

int main()
{
    double u[N];
    double a[N];
    double f[N];
    double mat[N][3];

    // Setting up the two known matrices
    initializeMat( mat );
    initializeF( f );
    // Using seperate variable for a to keep track of it.
    initializeA( mat, a );

    // Setting initial values for a and f
    a[0] = mat[0][0];
    f[0] = mat[0][0]*

    // Finding the alphas
    int j;
    for( j=1; j<N; j++ ){
        a[j] = a[j] - ( mat[j][1]*mat[j-1][2]/a[j-1] );
        f[j] = f[j] - ( mat[j][1]/a[j-1] )*f[j-1] ;
        printf("a[%d] = %lf\tf[%d] = %lf\n", j,a[j], j,f[j]);
    }

    // Assigning the u values
    u[N-1] = f[N-1]/a[N-1];
    int k;
    for( k=N-2; k>-1; k-- ){
        u[k] = f[k] - mat[j][2]*u[k+1]/a[k];
    }

    // Error Calc and printing. Also decided to recycle variable j
    double error;
    for( j=0; j<N; j++ ){
        error = (mat[j][1]*u[j-1] + mat[j][0]*u[j] + mat[j][2]*u[j+1] - f[j]) / ( fabs(mat[j][1]*u[j-1]) + fabs(mat[j][0]*u[j]) + fabs(mat[j][2]*u[j+1]) + fabs(f[j]) );
        printf("u[%d] = %lf\tError = %lf\n", j,u[j], error);
    }


    return 0;
}

void initializeMat( double mat[N][3] ){
    /* A is first Col (the main diaganol for a real Matrix),
       B is second Col (the diagonal just left of a in a real matrice,
       C is third Col (the diagonal just right of a in a real matrice)
    */
    // All Rows except for first and last
    int i;
    for( i=1; i<N-1; i++ ){
        mat[i][0] = 2.04;
        mat[i][1] = -1;
        mat[i][2] = -1;
    }
    // First Row
    mat[0][0] = 2.04;
    mat[0][1] = 0;
    mat[0][2] = -1;
    // Last Row
    mat[N-1][0] = 2.04;
    mat[N-1][1] = -1;
    mat[N-1][2] = 0;

}
void initializeF( double f[N]){
    f[0] = 40.8;
    f[1] = 0.8;
    f[2] = 0.8;
    f[3] = 200.8;
}
void initializeA( double mat[N][3], double a[N] ){
    int i;
    for ( i=0; i<N; i++ ){
        a[i] = mat[i][0];
    }
}
