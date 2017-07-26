#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double initialT = 0.0;
double initialR = .5;
double finalT = 10.0;
double deltaT = .01;
char continueP = 'y';

double vel( double v ){ return v; }
double dv( double r, double v ){ return  ((1.0/pow(r,3) - 1 - (3.0/2)*v*v )/ r); }

void adaptiveIntegration( double initT, double dt, double tMax);
void rungeKuttaCalc( double *r, double *v, double dt);


int main()
{
    while (continueP == 'y'){
        /* User inputing initial dt. */
        printf("Enter in the initial dt: ");
        scanf("%lf", &deltaT);
        /* Determining initial r offset. */
        printf("Enter in the initial R offset from 1: ");
        scanf("%lf", &initialR);
        /* Determining t max */
        printf("Enter in the final t value: (if you put in an insane value, I will find you...\n and I will end you)\n");
        scanf("%lf", &finalT);

        /* RUNGE KUTTA's FORMULA */
        adaptiveIntegration( initialT, deltaT, finalT );


        /* Run Again? Sequence */
        printf("\nContinue Again? (y/n) \n");
        scanf(" %c", &continueP);
    }

    printf("\nHave a nice day!\n");

    return 0;
}

void adaptiveIntegration( double initT, double dt, double tMax){
    double t = initT;

    double r = 1+initialR;
    double v=0;

    double rNew2Step, rNew1Step;
    double vNew2Step, vNew1Step;
    double dt2Step;
    int count2Step = 0; // Counter to call RungeKutta twice

    // Vars to change dt
    double delta;
    // Checking delta Desired
    double e;
    double deltaDes;
    double phi;
    // Changing dt
    double dtNew;

    /* Variables to Make File */
    FILE *f = fopen("Cavitration.csv", "w");
    fprintf(f,"time,r\n");
    fprintf(f,"%lf,%lf\n", 0.0, r);

    for( t=dt; t<tMax ; t+=dt ){

        /* One Step Runge Kutta */
        rNew1Step = r; vNew1Step = v;               // Have to initialize the 1 step news. I'm going to save r and v with the old values.
        rungeKuttaCalc(&rNew1Step,&vNew1Step, dt);

        /* Two Step Runge Kutta */
        rNew2Step = r; vNew2Step = v; dt2Step = dt/2;
        for( count2Step = 0; count2Step<2; count2Step++)
            rungeKuttaCalc(&rNew2Step,&vNew2Step, dt2Step);



        /* Variable dt */
        // Check Delta = abs( u two steps - u one step )
        delta = fabs(rNew2Step-rNew1Step);
        // Checking delta Desired
        e = pow(10,-10);
        deltaDes = e*( fabs(r) + dt*fabs(v) );
        phi = deltaDes/delta;

        // Changing dt
        if (delta > deltaDes){ dtNew = dt*pow(phi, .25);  }
        else if (delta < deltaDes){ dtNew = dt*pow(phi, .20); }
        else dtNew = dt;
        // Safety ensures that dt does go below 10^-9
        if ( dtNew<pow(10,-10) ) dtNew = pow(10, -10);


        /* Have to run calculation 1 more time with the correct dt */
        rungeKuttaCalc( &r, &v, dtNew);
        // Reseting values done because passed in pointer into function
        dt = dtNew;

        /* Final Check */
        double rDot = (2/pow(r,3)) * (log(r/(1+initialR))) - ((2.0/3.0) * (1-pow((1+initialR)/r,3)));
        /* Display Statement */
        printf("r(%2.2lf): %lf\tv(%2.2lf): %lf\tdt(%2.2lf): %.10lf\tv^2 = %lf\t R2 = %lf\n\n", t,r, t,v, t,dt, pow(v,2), rDot);
        fprintf(f, "%lf,%lf\n",t,r);
    }

    fclose(f);
}

void rungeKuttaCalc( double *r, double *v, double dt){

    double rNew, rStar, rStar2, rStar3;
    double vNew, vStar, vStar2, vStar3;

    rStar = *r + dt/2 * vel(*v);
        /* NOTE: Second Equation manipulates V */
    vStar = *v + dt/2 * dv(*r,*v);

    rStar2 = *r + dt/2 * vel(vStar);
    vStar2 = *v + dt/2 * dv(rStar,vStar);

    rStar3 = *r + dt * vel(vStar2);
    vStar3 = *v + dt * dv(rStar2,vStar2);

    rNew = *r+dt/6*( vel(*v) + 2*vel(vStar) + 2*vel(vStar2) + vel(vStar3) );
    vNew = *v+dt/6*( dv(*r,*v) + 2*dv(rStar,vStar) + 2*dv(rStar2,vStar2) + dv(rStar3,vStar3) );

    *r = rNew;
    *v = vNew;
}
