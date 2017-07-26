#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// BLOWS UP
// SHOULD BE KNOWN AS PROBLEM 2.1.4

#define dt .1
double end = 4*M_PI/dt;

int main()
{
    double x[(int)(end)+1], v[(int)(end)+1];
    double t=0;
    int count=2;

    // Initial Case t=0
    x[0] = 1; v[0] = 0;
    printf("Tao = (%2.2lf)\tX*: (Approx / Actual): %lf / %lf\tV*: (Approx / Actual): %lf / %lf\n\t Error: %lf", 0.0, x[0],cos(0), v[0],-sin(0) );
    // Case of index 1, or t+1dt
    x[1] = x[0]*cos(dt);
    v[1] = v[0] - dt*x[0];
    printf("Tao = (%2.2lf)\tX*: (Approx / Actual): %lf / %lf\tV*: (Approx / Actual): %lf / %lf\n", dt, x[1],cos(dt), v[1],-sin(dt) );

    count=2; // EXTRA: for added safety line
    for ( t=2*dt; t<end*dt; t+=dt ){
        x[count] = -v[count-2]*2*dt + 4*x[count-1] - 3*x[count-2];
        v[count] =  x[count-2]*2*dt + 4*v[count-1] - 3*v[count-2];
        printf("Tao = (%2.4lf)\tX*: (Approx / Actual): %lf / %lf\tV*: (Approx / Actual): %lf / %lf\n", t, x[count],cos(t), v[count],-sin(t) );
        count++;
    }


    return 0;
}
