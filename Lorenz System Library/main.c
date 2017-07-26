#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


/* BROKEN: FILE CREATION NOT WORKING. REST OF CODE IS GOOD. */
/* TODO: MAKE FUNCTION THAT CAN ACCESS EACH OF THESE FILES */


/* Control Variables (Control the iterations and when to stop) */
double initialT = 0.0;
double finalT = 10.0;
double deltaT = .01;
char xyzRange = 'n';
double xLoop, yLoop, zLoop;

/* Library Manipulation/Creation */
char fileName[100] = "";
double libraryStart = 0;
double libraryEnd = 0;
double libraryStepSize = 1.0;
int loopCounter=0;

/* Initial Conditions */
double sigma = 10.0;
double B = 8.0/3;
double p = 28.0;
double xInitial = 1;
double yInitial = 1;
double zInitial = 1;
double xStart = 1, xEnd = 1, xStep = 1;
double yStart = 1, yEnd = 1, yStep = 1;
double zStart = 1, zEnd = 1, zStep = 1;
void xyzRangeF();


/* Functions for the Runga-Kutta Approximations */
double dx( double x, double y, double z ){ return sigma*( y-x ); }
double dy( double x, double y, double z ){ return x*(p-z) - y;   }
double dz( double x, double y, double z ){ return x*y - B*z;     }
/* Adaptive Integration to Speed Things Up and Runga-Kutta */
void adaptiveIntegration( double initT, double dt, double tMax);
void rungeKuttaCalc(double *x, double *y, double *z, double dt);


int main()
{
    /*______STEP 1: USER INPUT______*/
        /* User Input for Library Parameters */
        printf("Enter the number you want sigma, beta, and phi to START: "); scanf("%lf",&libraryStart);
        printf("Enter the number you want sigma, beta, and phi to STOP: "); scanf("%lf",&libraryEnd);
        printf("Enter the step size between START and STOP: "); scanf("%lf",&libraryStepSize);
        printf("\nRange for x, y, and z? (y/n) "); scanf(" %c", &xyzRange);
        if (xyzRange == 'y') xyzRangeF();
        /* User Input for Final T */
        printf("\nEnter in the final t value: ");
        scanf("%lf", &finalT);

    /*______STEP 2: CREATE LIBRARY GOING THROUGH THE VALUES______*/
        printf("\nCreating Library. Please Wait...\n\n");
        double amtOfFiles = pow( (libraryEnd-libraryStart+1)/libraryStepSize, 3)  * ((xEnd-xStart+1)/xStep) * ((yEnd-yStart+1)/yStep) * ((zEnd-zStart+1)/zStep);
        for( sigma=libraryStart; sigma<=libraryEnd; sigma+=libraryStepSize )
        for( B=libraryStart; B<=libraryEnd; B+=libraryStepSize )
        for( p=libraryStart; p<=libraryEnd; p+=libraryStepSize ){
                        for( xLoop=xStart; xLoop<=xEnd; xLoop+=xStep )
                        for( yLoop=yStart; yLoop<=yEnd; yLoop+=yStep )
                        for( zLoop=yStart; zLoop<=zEnd; zLoop+=zStep )
                        {
                            sprintf(fileName, "Sigma(%2.5lf) B(%2.5lf) p(%2.5lf) :: x(%2.5lf) y(%2.5lf) z(%2.5lf).csv", sigma,B,p, xLoop,yLoop,zLoop);
                            xInitial = xLoop; yInitial=yLoop; zInitial=zLoop;
                            adaptiveIntegration( initialT, deltaT, finalT );
                            loopCounter++;
                        }
                        printf("\tFile %d/%2.lf Created.\t", loopCounter,amtOfFiles);
        }

    printf("\n\nLibrary Created. Have a nice day!\n");

    return 0;
}

void adaptiveIntegration( double initT, double dt, double tMax){
    double t = initT;
        /* NOTE: Need 3 sets of variables to work the 3 equations */
    double x = xInitial, xNew1Step, xNew2Step;
    double y = yInitial, yNew1Step, yNew2Step;
    double z = zInitial, zNew1Step, zNew2Step;
    int count = 0; // Runs the 2 step u twice
    double dt2Step = dt/2;

        /* NOTE: Variables below help change dt */
    double delta, e, deltaDes, phi, dtNew;

        /* NOTE: File Manipulation Variables */
    FILE *f = fopen(fileName,"w");
    fprintf(f,"x,y,z\n");
    fprintf(f,"%lf,%lf,%lf\n",x,y,z);

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

void xyzRangeF(){
    printf("Enter in where X STARTs: "); scanf("%lf",&xStart);
    printf("Enter in where X ENDs: "); scanf("%lf",&xEnd);
    printf("Enter in the step size for X: "); scanf("%lf",&xStep);
    printf("Enter in where Y STARTs: "); scanf("%lf",&yStart);
    printf("Enter in where Y ENDs: "); scanf("%lf",&yEnd);
    printf("Enter in the step size for Y: "); scanf("%lf",&yStep);
    printf("Enter in where Z STARTs: "); scanf("%lf",&zStart);
    printf("Enter in where Z ENDs: "); scanf("%lf",&zEnd);
    printf("Enter in the step size for Z: "); scanf("%lf",&zStep);

    xyzRange = 'n';
}
