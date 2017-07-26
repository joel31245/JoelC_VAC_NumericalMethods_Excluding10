#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void errorCheck2ndDer( double N, double k );
void errorCheck1stDer1Order( double N, double k );
void errorCheck1stDer2ndOrder( double N, double k );

// 2nd DERIVITIVE CHECK NOT WORKING - LARGE ERROR

int main()
{
    double N = 10.0;
    double k=1.0;

    printf(" - CASE A: N=10, k=1\n");
    errorCheck2ndDer(N,k);


    printf("\n\n - CASE B: N=10, k=5, 10, 20\n");
    k=5;                errorCheck2ndDer(N,k);
    k=10; printf("\n"); errorCheck2ndDer(N,k);
    k=20; printf("\n"); errorCheck2ndDer(N,k);


    printf("\n\n - CASE C: k=20, N=100, N=500\n");
    k=20; N=100;        errorCheck2ndDer(N,k);
    N=500; printf("\n"); errorCheck2ndDer(N,k);


    printf("\n\n - CASE D: First Derivitive, k=1, N=100, N=10^4, N=10^6, N=10^9\n");
    printf(" 1st Order \t 2nd Order\n");
    k=1; N=pow(10,2);          errorCheck1stDer1Order(N,k); printf("\t"); errorCheck1stDer2ndOrder(N,k);
    N=pow(10,3); printf("\n"); errorCheck1stDer1Order(N,k); printf("\t"); errorCheck1stDer2ndOrder(N,k);
    N=pow(10,4); printf("\n"); errorCheck1stDer1Order(N,k); printf("\t"); errorCheck1stDer2ndOrder(N,k);
    N=pow(10,6); printf("\n"); errorCheck1stDer1Order(N,k); printf("\t"); errorCheck1stDer2ndOrder(N,k);
    printf("\nCalculating. Will take some time. \n(Maybe 300 seconds.) Kindly wait patiently... ");
    N=pow(10,9); printf("\n"); errorCheck1stDer1Order(N,k); printf("\t"); errorCheck1stDer2ndOrder(N,k);

    return 0;
}

void errorCheck2ndDer( double N, double k ){
    double error = 0.0, exact = 0.0, approx = 0.0;
    double j=0.0; // loop counter
    double h=0.0;

    h = (double) 1.0/ (double) N;

    for( j=0; j<=N; j++ ){
        double x = (double)j*h;
//        exact = -k*k*sin(k*x);
//        approx = ( (sin(k*(x+h))) - (2*sin(k*x)) + (sin(k*(x-h))) / (h*h));
//        // manual absolute value
//        if (exact > approx) error+= exact-approx;
//        else                error+= approx-exact;
        error+= (double) fabs((-k*k*sin(k*x)) - ((sin(k*(h+x)) - (2*sin(k*x)) + sin(k*(x-h)))/(h*h)));
    }

    error=( error/(double)(k*k) );

    printf("Error: %lf", error);
}
// RECREATE AS MANY ERRORCHECKS AS THERE ARE DIFFERENT EQUATIONS - I THINK YOULL NEED 3
// 1st Derivitive 1st Order
void errorCheck1stDer1Order( double N, double k ){
    double error = 0.0, exact = 0.0, approx = 0.0;
    double j=0; // loop counter
    double h=0.0;

    h = (double) 1.0/ (double) N;

    for( j=0; j<=N; j++ ){
        double x = j*h;
        exact = k*cos(k*x);
        approx = ( sin(k*(x+h)) - sin(k*x) )/ (h);
        // manual absolute value
        if (exact > approx) error+= exact-approx;
        else                error-= exact-approx;
    }

    error*=( 1/(k*k) );

    printf("Error: %lf", error);
}
// 1st Derivitive 2nd Order
void errorCheck1stDer2ndOrder( double N, double k ) {
    double error = 0.0, exact = 0.0, approx = 0.0;
    double j=0; // loop counter
    double h=0.0;

    h = (double) 1.0/ (double) N;

    for( j=0; j<=N; j++ ){
        double x = j*h;
        exact = k*cos(k*x);
        approx = ( 4*sin(k*(x+h)) - 3*sin(k*x) - sin(k*(x+2*h)) )/ (2*h);
        // manual absolute value
        if (exact > approx) error+= exact-approx;
        else                error-= exact-approx;
    }

    error*=( 1/(k*k) );

    printf("Error: %lf", error);
}
