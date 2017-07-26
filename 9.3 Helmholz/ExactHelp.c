#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define Q 10
#define N 10
#define L 1.0
#define UL 0
#define U0 0
#define lam 1

void initializeG( double[N], double );
void initializeMat( double[N], double[N], double[N], double h );

int main()
{
    /* STEP 1: INITIALIZE THE MATRIX AND ELEMENTS */
    double h = L/N;
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
        u[j] = ( g[j] - c[j]*u[j+1] )/a[j];
    }


    double x, integral, exact, error;
    double integralL = ( exp(-Q*1) * (( lam*cosh(lam*1) + Q*sinh(lam*1) )/ (lam*lam - Q*Q)) ) - ( exp(-Q*0) * (( lam*cosh(lam*0) + Q*sinh(lam*0) )/ (lam*lam - Q*Q)) );
    for( j=0; j<N; j++ ){

        /* STEP 4: FIND THE ERROR */ // WHY COMMENTED OUT NOT WORKING BUT WITH "-lam" WORKING
        x = (double)j*h;
        integral = ( exp(-Q*x) * (( lam*cosh(lam*x) + Q*sinh(lam*x) -lam)/ (lam*lam - Q*Q) ));//  -  ( exp(-Q*0) * (( lam*cosh(lam*0) + Q*sinh(lam*0) )/ (lam*lam - Q*Q)) );
        exact = U0* sinh(lam*(L-x))/sinh(lam*L);
        exact-= 1.0/lam*integral; printf("%lf\n\n", exact );
        exact+= (UL + 1.0/lam*integralL)*( sinh(lam*x)/sinh(lam*L) );
        error = fabs(exact - u[j])/exact*100;

        /* STEP 5: PRINT RESULTS */
        printf("u[%2d] = %lf\t Exact: %lf\t Error: %3.2lf percent\n", j+1,u[j], exact, error);

    }

    return 0;
}


void initializeG( double g[N], double h ){
    int i;
    double x;
    for( i=0; i<N; i++ ){
        x = 0+i*h;
        g[i] = h*h*exp(-Q*x);
    }
}
void initializeMat( double a[N], double b[N], double c[N], double h ){
    // First Row
    a[0] = 2+lam*lam*h*h;
    c[0] = -1;
    // All other rows
    int k;
    for( k=1; k<N-1; k++ ){
        a[k] = 2+lam*lam*h*h;
        b[k] = -1;
        c[k] = -1;
    }
    // Last Row
    a[N-1] = 2+lam*lam*h*h;
    b[N-1] = -1;
}
