#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double initialT = 0.0;
double finalT = 10.0;
double deltaT = .01;
char continueP = 'y';

double sigma = 10.0;
double B = 8.0/3;
double p = 28.0;

double dx( double x, double y, double z ){ return sigma*( y-x ); }
double dy( double x, double y, double z ){ return x*(p-z) - y;   }
double dz( double x, double y, double z ){ return x*y - B*z;     }

void adaptiveIntegration( double initT, double dt, double tMax);
void rungeKuttaCalc(double *x, double *y, double *z, double dt);


int main()
{
    while (continueP == 'y'){
        deltaT = .01;
        /* Determining sigma, B and phi. */
        printf("Enter in sigma: "); scanf("%lf", &sigma);
        //sigma = 10.0;
        printf("Enter in beta: ");  scanf("%lf", &B);
        //B = 8.0/3;
        printf("Enter in phi: ");   scanf("%lf", &p);
        //p = 28.0;


        /* Determining t max */
        printf("Enter in the final t value: (if you put in an insane value, I will find you...\n and I will end you)\n");
        scanf("%lf", &finalT);

        /* RUNGE KUTTA's FORMULA */
        adaptiveIntegration( initialT, deltaT, finalT );



        /* Running Again Sequence */
        printf("\nContinue Again? (y/n) \n");
        scanf(" %c", &continueP);
    }

    printf("\nHave a nice day!\n");

    return 0;
}

void adaptiveIntegration( double initT, double dt, double tMax){
    double t = initT;
        /* NOTE: Need 3 sets of variables to work the 3 equations */
    double x = 1, xNew1Step, xNew2Step;
    double y = 1, yNew1Step, yNew2Step;
    double z = 1, zNew1Step, zNew2Step;
    int count = 0; // Runs the 2 step u twice
    double dt2Step = dt/2;

        /* NOTE: Variables below help change dt */
    double delta, e, deltaDes, phi, dtNew;

        /* NOTE: File Manipulation Variables */
    FILE *f = fopen("LorenzModel.csv","w");
    fprintf(f,"x,y,z\n");
    fprintf(f,"1,1,1\n");

    for( t=dt; t<tMax ; t+=dt ){
        // Checking 1 step Runge Kutta
        xNew1Step = x; yNew1Step = y; zNew1Step = z;
        rungeKuttaCalc(&xNew1Step,&yNew1Step,&zNew1Step, dt);

        // Checking 2 steps
        xNew2Step = x; yNew2Step = y; zNew2Step = z;
        dt2Step = dt/2;
        for( count=0; count<2; count++ )
            rungeKuttaCalc(&xNew2Step,&yNew2Step,&zNew2Step, dt2Step);


        /*Variable dt */
        // Check Delta = abs( u two steps - u one step )
        delta = fabs(xNew2Step-xNew1Step);
        // Checking delta Desired
        e = pow(10,-8);
        deltaDes = e*( fabs(xNew2Step) );
        phi = deltaDes/delta;
        // Changing dt
        dtNew = dt;
        if (delta > deltaDes) dtNew = dt*pow(phi, .25);
        else if (delta < deltaDes) dtNew = dt*pow(phi, .20);
        else dtNew = dt;
        // Safety ensures that dt does go below 10^-10
        if ( dtNew<pow(10,-10) ) dtNew = pow(10, -10);


        /* Have to run calculation 1 more time with the correct dt */
        rungeKuttaCalc(&x,&y,&z, dtNew);
        // Reseting values for next iteration. NOTE x,y,z does not need to updated since they are changed within the function (Pointer Logic)
        dt = dtNew;

        printf("x(%2.5lf): %lf\ty(%2.5lf): %lf\tz(%2.5lf): %lf\tdt(%2.2lf): %.5lf\n", t,x, t,y, t,y, t,dt);
        fprintf(f,"%lf,%lf,%lf\n",x,y,z);
    }
    fclose(f);
}

void rungeKuttaCalc(double *x, double *y, double *z, double dt){
    double xS, xS2, xS3, xNew;
    double yS, yS2, yS3, yNew;
    double zS, zS2, zS3, zNew;

    xS = *x + dt/2 * dx(*x,*y,*z);
    yS = *y + dt/2 * dy(*x,*y,*z);
    zS = *z + dt/2 * dz(*x,*y,*z);

    xS2 = *x + dt/2 * dx(xS,yS,zS);
    yS2 = *y + dt/2 * dy(xS,yS,zS);
    zS2 = *z + dt/2 * dz(xS,yS,zS);

    xS3 = *x + dt * dx(xS2,yS2,zS2);
    yS3 = *y + dt * dy(xS2,yS2,zS2);
    zS3 = *z + dt * dz(xS2,yS2,zS2);

    xNew = *x+dt/6*( dx(*x,*y,*z) + 2*dx(xS,yS,zS) + 2*dx(xS2,yS2,zS2) + dx(xS3,yS3,zS3) );
    yNew = *y+dt/6*( dy(*x,*y,*z) + 2*dy(xS,yS,zS) + 2*dy(xS2,yS2,zS2) + dy(xS3,yS3,zS3) );
    zNew = *z+dt/6*( dz(*x,*y,*z) + 2*dz(xS,yS,zS) + 2*dz(xS2,yS2,zS2) + dz(xS3,yS3,zS3) );

    *x = xNew;
    *y = yNew;
    *z = zNew;
}
