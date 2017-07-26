#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double initialT = 0.0;
double finalT = 2.0;
double deltaT = .000001;
char flag = 'a';
char continueP = 'y';

double f0( double t ){
    return 2;
}
double f1( double t ){
    return 4;
}

double fa( double u, double t ){ return pow(t,3)*u - 3.0/2.0*u; }
double faExact( double t ){ return exp(pow(t,4)/4 - 3.0/2*t); }

double fb( double u, double t ){ return (1 + 2*t)*sqrt(u); }
double fbExact( double t ){ return .25*pow( (2+t+t*t), 2); }

double eulerFunction( double initT, double dt, double tMax, char flag);
double midPoint( double initT, double dt, double tMax, char flag);
double huen( double initT, double dt, double tMax, char flag);
double rungeKutta( double initT, double dt, double tMax, char flag);



int main()
{
    while (continueP == 'y'){
        /* Determining user input precision. */
        printf("Enter in dt: ");
        scanf("%lf", &deltaT);

        /* EULER FORWARD */
        flag = 'a';
        printf("\nPart A (Euler): Equation A\n");
        eulerFunction( initialT, deltaT, finalT, flag );
        finalT = 1; flag = 'b'; printf("Part A (Euler): Equation B\n");
        eulerFunction( initialT, deltaT, finalT, flag );

        /* MIDPOINT FORMULA */
        printf("\nPart B (Midpoint): Equation A\n");
        finalT = 2; flag = 'a';
        midPoint( initialT, deltaT, finalT, flag );
        finalT = 1; flag = 'b'; printf("Part B (Midpoint): Equation B\n");
        midPoint( initialT, deltaT, finalT, flag );

        /* HEUN's FORMULA */
        printf("\nPart C (Huen's): Equation A\n");
        finalT = 2; flag = 'a';
        huen( initialT, deltaT, finalT, flag );
        finalT = 1; flag = 'b'; printf("Part C (Huen's): Equation B\n");
        huen( initialT, deltaT, finalT, flag );

        /* RUNGE KUTTA's FORMULA */
        printf("\nPart D (RUNGE KUTTA's): Equation A\n");
        finalT = 2; flag = 'a';
        rungeKutta( initialT, deltaT, finalT, flag );
        finalT = 1; flag = 'b'; printf("Part D (RUNGE KUTTA's): Equation B\n");
        rungeKutta( initialT, deltaT, finalT, flag );

        printf("\nContinue Again? (y/n) \n");
        scanf(" %c", &continueP);
    }

    printf("\nHave a nice day!\n");

    return 0;
}



double eulerFunction( double initT, double dt, double tMax, char flag){
    double exact;
    double t=initT;
    double u = 1, uNew;

    for( t=dt; t<tMax+dt; t+=dt ){
        if (flag == 'a'){
            uNew = u+dt*fa(u,t);
            exact = faExact(t);
        }
        else {
            uNew = u+dt*fb(u,t);
            exact = fbExact(t);
        }

        u = uNew;
    }

    printf("Eueler: u(%2.2lf)   \tApprox: %lf\tExact: %lf   \tRelError: %3.8lf percent\n", t-=dt,uNew,exact, fabs(exact-uNew)/exact*100 );

    return uNew;
}


double midPoint( double initT, double dt, double tMax, char flag){
    double exact;
    double t = initT;
    double u=1, uNew, uStar;

    for( t=dt; t<tMax+dt; t+=dt ){
        if (flag == 'a'){
            uStar = u + dt/2*fa(u,t);
            uNew = u+dt*fa(uStar,t+dt/2);
            exact = faExact(t);
        }
        else {
            uStar = u + dt/2*fb(u,t);
            uNew = u+dt*fb(uStar,t+dt/2);
            exact = fbExact(t);
        }

        u = uNew;
    }

    printf("Midpoint: u(%2.2lf)   \tApprox: %lf\tExact: %lf   \tRelError: %3.8lf percent\n", t-=dt,uNew,exact, fabs(exact-uNew)/exact*100 );

    return uNew;
}

double huen( double initT, double dt, double tMax, char flag){
    double exact;
    double t = initT;
    double u=1, uNew, uStar;

    for( t=dt; t<tMax+dt; t+=dt ){
        if (flag == 'a'){
            uStar = u + dt*fa(u,t);
            uNew = u+dt/2*( fa(uStar,t+dt)+fa(u,t) );
            exact = faExact(t);
        }
        else {
            uStar = u + dt*fb(u,t);
            uNew = u+dt/2*( fb(uStar,t+dt)+fb(u,t) );
            exact = fbExact(t);
        }

        u = uNew;
    }

    printf("Huen: u(%2.2lf)   \tApprox: %lf\tExact: %lf   \tRelError: %3.8lf percent\n", t-=dt,uNew,exact, fabs(exact-uNew)/exact*100 );

    return uNew;
}

double rungeKutta( double initT, double dt, double tMax, char flag){
    double exact;
    double t = initT;
    double u=1, uNew, uStar, uStar2, uStar3;

    for( t=dt; t<tMax+dt; t+=dt ){
        if (flag == 'a'){
            uStar = u + dt/2*fa(u,t);
            uStar2 = u + dt/2*fa(uStar,t+.5*dt);
            uStar3 = u + dt/2*fa(uStar2,t+.5*dt);
            uNew = u+dt/6*( fa(u,t) + 2*fa(uStar, t+.5*dt) + 2*fa(uStar2, t+.5*dt) + fa(uStar3, t) );
            exact = faExact(t);
        }
        else {
            uStar = u + dt/2*fb(u,t);
            uStar2 = u + dt/2*fb(uStar,t+.5*dt);
            uStar3 = u + dt/2*fb(uStar2,t+.5*dt);
            uNew = u+dt/6*( fb(u,t) + 2*fb(uStar, t+.5*dt) + 2*fb(uStar2, t+.5*dt) + fb(uStar3, t) );
            exact = fbExact(t);
        }

        u = uNew;

    }
    printf("R-K: u(%2.2lf)   \tApprox: %lf\tExact: %lf   \tRelError: %3.8lf percent\n", t-=dt,uNew,exact, fabs(exact-uNew)/exact*100 );

    return uNew;
}

