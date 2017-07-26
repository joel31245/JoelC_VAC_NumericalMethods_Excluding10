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

double rungeKutta( double initT, double dt, double tMax);


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
        rungeKutta( initialT, deltaT, finalT );



        /* Running Again Sequence */
        printf("\nContinue Again? (y/n) \n");
        scanf(" %c", &continueP);
    }

    printf("\nHave a nice day!\n");

    return 0;
}

double rungeKutta( double initT, double dt, double tMax){
    double t = initT;
        /* NOTE: Need 2 sets of variables to work the 2 equations */
    double r = 1+initialR, rNew, rStar, rStar2, rStar3;
    double v = 0, vNew, vStar, vStar2, vStar3;

    int t1;int count=0;
        double r1=r, r1Star, r1Star2, r1Star3, r1New;
        double v1=v, v1Star, v1Star2, v1Star3, v1New;
        double dt1 = dt/2;
    double delta;
        // Checking delta Desired
        double e;
        double deltaDes;
        double phi;
        // Changing dt
        double dtNew;
    for( t=dt; t<tMax ; t+=dt ){
            /* NOTE: First Equation manipulates R */
        rStar = r + dt/2 * vel(v);
            /* NOTE: Second Equation manipulates V */
        vStar = v + dt/2 * dv(r,v);

        rStar2 = r + dt/2 * vel(vStar);
        vStar2 = v + dt/2 * dv(rStar,vStar);

        rStar3 = r + dt * vel(vStar2);
        vStar3 = v + dt * dv(rStar2,vStar2);

        rNew = r+dt/6*( vel(v) + 2*vel(vStar) + 2*vel(vStar2) + vel(vStar3) );
        vNew = v+dt/6*( dv(r,v) + 2*dv(rStar,vStar) + 2*dv(rStar2,vStar2) + dv(rStar3,vStar3) );



        // Checking 2 steps
        count=0;
         r1=r, r1Star, r1Star2, r1Star3, r1New;
         v1=v, v1Star, v1Star2, v1Star3, v1New;
         dt1 = dt;
        for( t1=0; t1<2; t1++ ){
                /* NOTE: First Equation manipulates R */
            r1Star = r1 + dt1/2 * vel(v1);
                /* NOTE: Second Equation manipulates V */
            v1Star = v1 + dt1/2 * dv(r1,v1);

            r1Star2 = r1 + dt1/2 * vel(v1Star);
            v1Star2 = v1 + dt1/2 * dv(r1Star,v1Star);

            r1Star3 = r1 + dt1 * vel(v1Star2);
            v1Star3 = v1 + dt1 * dv(r1Star2,v1Star2);

            r1New = r1+dt1/6*( vel(v1) + 2*vel(v1Star) + 2*vel(v1Star2) + vel(v1Star3) );
            v1New = v1+dt1/6*( dv(r1,v1) + 2*dv(r1Star,v1Star) + 2*dv(r1Star2,v1Star2) + dv(r1Star3,v1Star3) );

            r1=r1New;
            v1=v1New;
            count++;
        }

        printf("\nr1New = %lf   rNew = %lf\n", r1New, rNew);
        printf("\ndt1 = %lf\n", dt1);

        // Check Delta = abs( u two steps - u one step )
         delta = fabs(r1New-rNew); printf("\ndelta(%2.15lf)= %lf", t, delta);
        // Checking delta Desired
        e = pow(10,-2);
        deltaDes = e*( fabs(r) + dt*fabs(v) );
        phi = deltaDes/delta;
        printf("\n phi = %lf\n", phi);
        // Changing dt


        if (delta > deltaDes){ dtNew = dt*pow(phi, .25);  }
        else if (delta < deltaDes){ dtNew = dt*pow(phi, .20); printf("\ndt= %lf\n", dt); }
        else dtNew = dt;
        printf("\n dtNew = %lf\n", dtNew);
        // Safety ensures that dt does go below 10^-9
        if ( dtNew<pow(10,-10) ) dtNew = pow(10, -10);


        /* Have to run calculation 1 more time with the correct dt */
        rStar = r + dtNew/2 * vel(v);
            /* NOTE: Second Equation manipulates V */
        vStar = v + dtNew/2 * dv(r,v);

        rStar2 = r + dtNew/2 * vel(vStar);
        vStar2 = v + dtNew/2 * dv(rStar,vStar);

        rStar3 = r + dtNew * vel(vStar2);
        vStar3 = v + dtNew * dv(rStar2,vStar2);

        rNew = r+dtNew/6*( vel(v) + 2*vel(vStar) + 2*vel(vStar2) + vel(vStar3) );
        vNew = v+dtNew/6*( dv(r,v) + 2*dv(rStar,vStar) + 2*dv(rStar2,vStar2) + dv(rStar3,vStar3) );
        // Reseting values for next iteration
        r = rNew;
        v = vNew;
        dt = dtNew;

//        r = rNew;
//        v = vNew;

        double rDot = (2/pow(rNew,3)) * (log(rNew/(1+initialR))) - ((2.0/3.0) * (1-pow((1+initialR)/rNew,3)));

        printf("r(%2.2lf): %lf\tv(%2.2lf): %lf\tdt(%2.2lf): %.10lf\tv^2 = %lf\t R2 = %lf\n", t,r, t,v, t,dt, pow(v,2), rDot);

    }



    return rNew;
}
