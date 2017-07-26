#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double initialT = 0.0;
double finalT = 1.0;
double deltaT = .01;
double e = 1;
char continueC = 'y';

/* should be less than 1 percent */

double f( double u ){ return 1.0/e/pow(u,2); }
double df( double u ){ return -2.0/e/pow(u,3); }
double exact( double t ){ return cbrt( 1+3*t/e); }

double trapezoidal( double initT, double dt, double tMax);
double newtonRaphson(double u){ return u - ( f(u) )/( df(u) ); }

int main()
{
    printf("\tPROBLEM 5.1.5: TRAPEZOIDAL RULE\n\n");
    while (continueC == 'y'){
        printf("Enter dt: ");
        scanf("%lf", &deltaT);
        printf("Enter e: ");
        scanf("%lf", &e);

        trapezoidal(initialT, deltaT, finalT);

        printf("Continue: (y/n) \n");
        scanf(" %c", &continueC);
        printf("\n\n");
    }

    printf("\n\nHave a nice day!\n\n");
    return 0;
}

double trapezoidal( double initT, double dt, double tMax){
    double exac;
    double t = initT;
    double u=1, uNew, uStar;

    for( t=dt; t<tMax+dt; t+=dt ){
        uStar = newtonRaphson(u);
        uNew = u+dt/2*( f(u)+f(uStar) );
        exac = exact(t);

        u = uNew;
    }

    printf("Trapezoidal: u(%2.2lf)   \tApprox: %lf\tExact: %lf   \tRelError: %3.8lf percent\n", t-=dt,uNew,exac, fabs(exac-uNew)/exac*100 );

    return uNew;
}

