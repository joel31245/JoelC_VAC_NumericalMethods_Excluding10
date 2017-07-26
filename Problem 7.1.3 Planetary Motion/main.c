#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "structBody.c"


/* General Loop and Boundary Control Variables */
double initialT = 0.0;
double finalT = 200000.0;
double deltaT = 600;
char continueP = 'y';
char advancedMenu = 'n';
char fileName[100] = "";

/* Problem Specific CONSTANTS */
struct body Sun;
struct body planet;
double G = 6.67*pow(10,-11);
double M1 = .08*1.989*pow(10,30);
double M2 = .85*5.9722*pow(10,24);
// Advanced Menu Variables
double initialSunX=0, initialSunY=0, initialSunVX=0, initialSunVY=0;
double initialPX=1.9889*pow(10,8), initialPY=-1.651*pow(10,9), initialPVX=79348.8, initialPVY=9078.256;
// These functions help control the menu
void advancedMenuReset(); void advancedMenuRun();


/* BIG Functions Crunching and Grinding */
void adaptiveIntegration( double initT, double dt, double tMax);
void rungeKuttaCalc(double );
/* SMALL Functions are Problem Specific (The Physics Equations that require integration */
double r12( double rx1, double ry1, double rx2, double ry2){ return pow(  pow(rx1-rx2,2) + pow(ry1-ry2,2) ,0.5 ); }
/* The f1 f2 and f3 functions of the Runge-Kutta */
double dr( double v ){ return v; }
// The f2 of the RungeKutta. Finds acceleration due to gravity. Split into components of x and y
double dv1X( double r1x,double r1y,  double r2x,double r2y ){ return -G*M2*( r1x-r2x )/ pow( r12(r1x,r1y, r2x,r2y), 3); }
double dv1Y( double r1x,double r1y,  double r2x,double r2y ){ return -G*M2*( r1y-r2y )/ pow( r12(r1x,r1y, r2x,r2y), 3); }
double dv2X( double r1x,double r1y,  double r2x,double r2y ){ return -G*M1*( r2x-r1x )/ pow( r12(r1x,r1y, r2x,r2y), 3); }
double dv2Y( double r1x,double r1y,  double r2x,double r2y ){ return -G*M1*( r2y-r1y )/ pow( r12(r1x,r1y, r2x,r2y), 3); }



int main()
{
    while (continueP == 'y'){
        deltaT = .01;


        /* Enter in the fileName with .csv at end */
        printf("Enter in the file name with .csv at the end: ");
        gets(fileName);

        /* Ask for Advanced Input Settings */
        printf("Do you want to go to the advanced Settings? (y/n)\n(Or hit r to reset to default values): ");
        scanf(" %c", &advancedMenu);

        while (advancedMenu ==  'r') advancedMenuReset();
        while (advancedMenu == 'y')  advancedMenuRun();

        /* Enter in the amount of days to rotate */
        printf("Enter in the amount of revolutions: ");
        scanf("%lf",&finalT); finalT=finalT*86400*1.509;

        /* Enter in the dt */
        printf("Enter in the change in t value (in seconds): ");
        scanf("%lf",&deltaT);

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

    // Initial X and Y and vX and vY
    Sun.x = initialSunX; Sun.y = initialSunY; Sun.vX = initialSunVX; Sun.vY = initialSunVY;
    planet.x = initialPX; planet.y = initialPY; planet.vX = initialPVX; planet.vY = initialPVY;

        /* NOTE: Adaptive Variation Variables */
    double e=pow(10,-4), dtNew;
    double deltaSunX, deltaDesSunX, phiSunX;
    double deltaSunY, deltaDesSunY, phiSunY;
    double deltaPX, deltaDesPX, phiPX;
    double deltaPY, deltaDesPY, phiPY;
    double vSun_x_1step, vSun_y_1step, vP_x_1step, vP_y_1step;
    double vSun_x_2step, vSun_y_2step, vP_x_2step, vP_y_2step, dt2Step;
    double xSun_x_2step, xSun_y_2step, xP_x_2step, xP_y_2step;
    int countTwice=0;

        /* NOTE: File Manipulation Variables */
    FILE *f = fopen(fileName, "w");
    fprintf(f,"SunX,SunY,PlanetX,PlanetY,zComp\n");
    fprintf(f,"%lf,%lf,%lf,%lf,0\n",Sun.x,Sun.y,  planet.x,planet.y);

    for( t=dt; t<tMax ; t+=dt ){
        /* One Step Vs */
        rungeKuttaCalc(dt);
        vSun_x_1step = Sun.vXNew; vSun_y_1step = Sun.vYNew;
        vP_x_1step = planet.vXNew; vP_y_1step = planet.vYNew;

        /* Two Step Vs */
        dt2Step = dt/2;
        for( countTwice=0; countTwice<2; countTwice++)
            rungeKuttaCalc(dt2Step);
        vSun_x_2step = Sun.vXNew; vSun_y_2step = Sun.vYNew;
        vP_x_2step = planet.vXNew; vP_y_2step = planet.vYNew;
        xSun_x_2step = Sun.xNew;  xSun_y_2step = Sun.yNew;
        xP_x_2step = planet.xNew; xP_y_2step = planet.yNew;


        /* Variable/Changing dt */
        // Check Delta = abs( u two steps - u one step )
        deltaSunX = fabs(vSun_x_2step-vSun_x_1step); deltaSunY = fabs(vSun_y_2step-vSun_y_1step);
        deltaPX = fabs(vP_x_2step-vP_x_1step);       deltaPY = fabs(vP_y_2step-vP_y_1step);
        // Checking delta Desired
        e = pow(10,-4);
        deltaDesSunX = e*( fabs(xSun_x_2step)+dt*fabs( vSun_x_2step ) );
        deltaDesSunY = e*( fabs(xSun_y_2step)+dt*fabs( vSun_y_2step ) );
        deltaDesPX = e*( fabs(xP_x_2step)+dt*fabs( vP_x_2step ) );
        deltaDesPY = e*( fabs(xP_y_2step)+dt*fabs( vP_y_2step ) );
        // Creating phi
        phiSunX = deltaDesSunX/deltaSunX; phiSunY = deltaDesSunY/deltaSunY;
        phiPX = deltaDesPX/deltaPX;       phiPY = deltaDesPY/deltaPY;
        // Changing dt
        dtNew = dt;
        if (phiSunX<phiSunY && phiSunX<phiPX && phiSunX<phiPY){
            if( deltaSunX>deltaDesSunX )     dtNew = dt*pow(phiSunX, .20);
            else if( deltaSunX<deltaDesSunX )dtNew = dt*pow(phiSunX, .25);
            else                dtNew = dt;
            //printf("SUN X\n");
        }
        else if (phiSunY<phiSunX && phiSunY<phiPX && phiSunY<phiPY){
            if( deltaSunY>deltaDesSunY )     dtNew = dt*pow(phiSunY, .20);
            else if( deltaSunY<deltaDesSunY )dtNew = dt*pow(phiSunY, .25);
            else                dtNew = dt;
            //printf("\tSUN Y\n");
        }
        else if (phiPX<phiSunX && phiPX<phiSunY && phiPX<phiPY){
            if( deltaPX>deltaDesPX )       dtNew = dt*pow(phiPX, .20);
            else if( deltaPX<deltaDesPX )  dtNew = dt*pow(phiPX, .25);
            else                dtNew = dt;
            //printf("\t\tPLANET X\n");
        }
        else {
            if( deltaPY>deltaDesPY )       dtNew = dt*pow(phiPY, .20);
            else if( deltaPY<deltaDesPY )  dtNew = dt*pow(phiPY, .25);
            else                dtNew = dt;
            //printf("\t\t\tPLANET Y\n");
        }
        // Safety ensures that dt does go below 10^-10
        if ( dtNew<pow(10,-10) ) dtNew = pow(10, -10);


        /* Run the Runge Kutta with the updated dt */
        rungeKuttaCalc(dtNew);

        /* Update Vars for Next Iteration */
        Sun.x = Sun.xNew; Sun.y = Sun.yNew; Sun.vX = Sun.vXNew; Sun.vY = Sun.vYNew;
        planet.x = planet.xNew; planet.y = planet.yNew; planet.vX = planet.vXNew; planet.vY = planet.vYNew;
        dt = dtNew;

        /* Printing all the Variables */
        printf("SUN: (%2.5lf): X=%e\tY=%e\tR=%e\tdt=%lf\n", t,Sun.x,Sun.y, r12(Sun.x,Sun.y, planet.x,planet.y), dt);
        printf("PLANET: (%2.5lf): X=%e\tY=%e\tR=%e\n", t,planet.x,planet.y, r12(Sun.x,Sun.y, planet.x,planet.y));
        fprintf(f,"%lf,%lf,%lf,%lf,0\n",Sun.x,Sun.y, planet.x,planet.y);
    }

    fclose(f);
}


void rungeKuttaCalc(double dt){

   /* Runge Kutta Step 1. r* v* */
            /* Setting Sun's R Star */
            Sun.xS = Sun.x + dt/2 * dr(Sun.vX);
            Sun.yS = Sun.y + dt/2 * dr(Sun.vY);
            /* Setting planet's R Star */
            planet.xS = planet.x + dt/2 * dr(planet.vX);
            planet.yS = planet.y + dt/2 * dr(planet.vY);
            /* Setting Sun's V Star */
            Sun.vXS = Sun.vX + dt/2 * dv1X(Sun.x,Sun.y,  planet.x,planet.y);
            Sun.vYS = Sun.vY + dt/2 * dv1Y(Sun.x,Sun.y,  planet.x,planet.y);
            /* Setting planet's V Star */
            planet.vXS = planet.vX + dt/2 * dv2X(Sun.x,Sun.y,  planet.x,planet.y);
            planet.vYS = planet.vY + dt/2 * dv2Y(Sun.x,Sun.y,  planet.x,planet.y);
        /* End of Runge Kutta Step 1. */

        /* Runge Kutta Step 2. r* v* */
            /* Setting Sun's R Star */
            Sun.xS2 = Sun.x + dt/2 * dr(Sun.vXS);
            Sun.yS2 = Sun.y + dt/2 * dr(Sun.vYS);
            /* Setting planet's R Star */
            planet.xS2 = planet.x + dt/2 * dr(planet.vXS);
            planet.yS2 = planet.y + dt/2 * dr(planet.vYS);
            /* Setting Sun's V Star */
            Sun.vXS2 = Sun.vX + dt/2 * dv1X(Sun.xS,Sun.yS,  planet.xS,planet.yS);
            Sun.vYS2 = Sun.vY + dt/2 * dv1Y(Sun.xS,Sun.yS,  planet.xS,planet.yS);
            /* Setting planet's V Star */
            planet.vXS2 = planet.vX + dt/2 * dv2X(Sun.xS,Sun.yS,  planet.xS,planet.yS);
            planet.vYS2 = planet.vY + dt/2 * dv2Y(Sun.xS,Sun.yS,  planet.x,planet.yS);
        /* End of Runge Kutta Step 2. */

        /* Runge Kutta Step 3. r* v* */
            /* Setting Sun's R Star */
            Sun.xS3 = Sun.x + dt * dr(Sun.vXS2);
            Sun.yS3 = Sun.y + dt * dr(Sun.vYS2);
            /* Setting planet's R Star */
            planet.xS3 = planet.x + dt * dr(planet.vXS2);
            planet.yS3 = planet.y + dt * dr(planet.vYS2);
            /* Setting Sun's V Star */
            Sun.vXS3 = Sun.vX + dt * dv1X(Sun.xS2,Sun.yS2,  planet.xS2,planet.yS2);
            Sun.vYS3 = Sun.vY + dt * dv1Y(Sun.xS2,Sun.yS2,  planet.xS2,planet.yS2);
            /* Setting planet's V Star */
            planet.vXS3 = planet.vX + dt * dv2X(Sun.xS2,Sun.yS2,  planet.xS2,planet.yS2);
            planet.vYS3 = planet.vY + dt * dv2Y(Sun.xS2,Sun.yS2,  planet.xS2,planet.yS2);
        /* End of Runge Kutta Step 3. */

        /* Runge Kutta Step Last. r* v* */
            /* Setting Sun's R Star */
            Sun.xNew = Sun.x + dt/6 *( dr(Sun.vX) + 2*dr(Sun.vXS) + 2*dr(Sun.vXS2) + dr(Sun.vXS3) );
            Sun.yNew = Sun.y + dt/6 *( dr(Sun.vY) + 2*dr(Sun.vYS) + 2*dr(Sun.vYS2) + dr(Sun.vYS3) );
            /* Setting planet's R Star */
            planet.xNew = planet.x + dt/6 *( dr(planet.vX) + 2*dr(planet.vXS) + 2*dr(planet.vXS2) + dr(planet.vXS3) );
            planet.yNew = planet.y + dt/6 *( dr(planet.vY) + 2*dr(planet.vYS) + 2*dr(planet.vYS2) + dr(planet.vYS3) );
            /* Setting Sun's V Star */
            Sun.vXNew = Sun.vX + dt/6 *( dv1X(Sun.x,Sun.y, planet.x,planet.y) + 2*dv1X(Sun.xS,Sun.yS, planet.xS,planet.yS) + 2*dv1X(Sun.xS2,Sun.yS2, planet.xS2,planet.yS2) + dv1X(Sun.xS3,Sun.yS3, planet.xS3,planet.yS3) );
            Sun.vYNew = Sun.vY + dt/6 *( dv1Y(Sun.x,Sun.y, planet.x,planet.y) + 2*dv1Y(Sun.xS,Sun.yS, planet.xS,planet.yS) + 2*dv1Y(Sun.xS2,Sun.yS2, planet.xS2,planet.yS2) + dv1Y(Sun.xS3,Sun.yS3, planet.xS3,planet.yS3) );
            /* Setting planet's V Star */
            planet.vXNew = planet.vX + dt/6 *( dv2X(Sun.x,Sun.y, planet.x,planet.y) + 2*dv2X(Sun.xS,Sun.yS, planet.xS,planet.yS) + 2*dv2X(Sun.xS2,Sun.yS2, planet.xS2,planet.yS2) + dv2X(Sun.xS3,Sun.yS3, planet.xS3,planet.yS3) );
            planet.vYNew = planet.vY + dt/6 *( dv2Y(Sun.x,Sun.y, planet.x,planet.y) + 2*dv2Y(Sun.xS,Sun.yS, planet.xS,planet.yS) + 2*dv2Y(Sun.xS2,Sun.yS2, planet.xS2,planet.yS2) + dv2Y(Sun.xS3,Sun.yS3, planet.xS3,planet.yS3) );
        /* End of Runge Kutta Last. */

}


void advancedMenuReset(){
    M1 = .08*1.989*pow(10,30);
    M2 = .85*5.9722*pow(10,24);
    deltaT = 600;
    finalT = 1.509*86400; //(1 revolution around the sun)
    // Reseting the initial positions.
    initialSunX=0; initialSunY=0; initialSunVX=0; initialSunVY=0;
    initialPX=1.9889*pow(10,8); initialPY=-1.651*pow(10,9); initialPVX=79348.8; initialPVY=9078.256;
    advancedMenu = 'n';
}
void advancedMenuRun(){
        // Masses setting in the advanced menu
    printf("\n\tMASSES OF THE BODIES: \n");
    printf("Enter in the Mass of the first astro-body: "); scanf("%lf", &M1);
    printf("Enter in the Mass of the second astro-body: "); scanf("%lf", &M2);
        // Initial Positions
    printf("\n\tINITIAL POSITIONS OF THE BODIES: \n");
    printf("Enter in the initial xPosition of the first astro-body: "); scanf("%lf", &initialSunX);
    printf("Enter in the initial yPosition of the first astro-body: "); scanf("%lf", &initialSunY);
    printf("Enter in the initial xPosition of the second astro-body: "); scanf("%lf", &initialPX);
    printf("Enter in the initial yPosition of the second astro-body: "); scanf("%lf", &initialPY);
        // Initial Velocities
    printf("\n\tINITIAL VELOCITES OF THE BODIES: \n");
    printf("Enter in the initial xVelocity of the first astro-body: "); scanf("%lf", &initialSunVX);
    printf("Enter in the initial yVelocity of the first astro-body: "); scanf("%lf", &initialSunVY);
    printf("Enter in the initial xVelocity of the second astro-body: "); scanf("%lf", &initialPVX);
    printf("Enter in the initial yVelocity of the second astro-body: "); scanf("%lf", &initialPY);
    advancedMenu = 'n';
}
