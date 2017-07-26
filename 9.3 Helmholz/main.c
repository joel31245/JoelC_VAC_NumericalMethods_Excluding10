#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define Q 10
#define N 100
#define L 1.0
#define UL 0
#define U0 0
#define lam 1

void initializeG( double[N], double );
void initializeMat( double[N], double[N], double[N], double h );

int main()
{
    /* STEP 1: INITIALIZE THE MATRIX AND ELEMENTS */
    double h = L/Q;
    // Initializing G
    double g[N];
    initializeG(g,h);
    // Initializing the initial matrix
    double a[N]; double b[N]; double c[N];
    initializeMat(a, b, c, h);

    /* STEP 2: FIND THE ALPHAS */
    // Finding the alphas (stored into the original a's array
    int j=1;
    for( j=1; j<N; j++ ){
        a[j] = a[j] - ( b[j]*c[j-1]/a[j-1] );
        g[j] = g[j] - ( b[j]/a[j-1] ) * g[j-1];
    }

    /* STEP 3: FIND THE U's */
    // Finding all the u's
    double u[N];
    u[N-1] = g[N-1]/a[N-1];
    for( j=N-2; j>-1; j-- ){
        u[j] = ( g[j] - c[j+1]*u[j+1] )/a[j];
    }


    double x, integral, exact, error;
    double integralL = ( -exp(-2*Q*L) * (( lam*cosh(lam*L) + Q*sinh(lam*L) - lam)/ (lam*lam - Q*Q)) );// - ( exp(-Q*0) * (( lam*cosh(lam*0) + Q*sinh(lam*0) )/ (lam*lam - Q*Q)) );
    for( j=0; j<N; j++ ){

        /* STEP 4: FIND THE EXACT AND ERROR */
        x = (double)j*h;
        integral = ( -exp(-2*Q*x) * (( lam*cosh(lam*x) + Q*sinh(lam*x) - lam)/ (lam*lam - Q*Q) ));//  -  ( exp(-Q*0) * (( lam*cosh(lam*0) + Q*sinh(lam*0) )/ (lam*lam - Q*Q)) );
//        exact = U0* sinh(lam*(L-x))/sinh(lam*L);
//        exact-= 1.0/lam*integral;
//        exact+= (UL + 1.0/lam*integralL)*( sinh(lam*x)/sinh(lam*L) );
        exact = Q/(lam*(Q*Q-lam*lam)*sinh(lam*L)) * ( sinh(lam*(x-L)) - exp(-Q*x)*(sinh(lam*L) - sinh(lam*x)) );
        error = fabs(exact - u[j])/exact*100;

        /* STEP 5: PRINT RESULTS */
        printf("u[%2d] = %e\t Exact: %e\t Error: %3.3lf percent\n", j+1,u[j], exact, error);

    }

    return 0;
}


void initializeG( double g[N], double h ){
    int i;
    double x;
    for( i=0; i<N; i++ ){
        x = i*h;
        g[i] = h*h*exp(-Q*x);
    }
}
void initializeMat( double a[N], double b[N], double c[N], double h ){
    // All rows
    int k;
    for( k=0; k<N; k++ ){
        a[k] = 2+lam*lam*h*h;
        b[k] = -1;
        c[k] = -1;
    }
}
