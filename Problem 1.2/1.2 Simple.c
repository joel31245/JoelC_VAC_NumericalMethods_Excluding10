#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double h = .0001;

// RUNTIME ACCURACY ERROR FOR THE NEW STUFF

int main()
{
    double xStr[(int)(4*M_PI/h)+1];
    double velStr[(int)(4*M_PI/h)+1];

    velStr[0] = 0;
    xStr[0] = 1;

    double currentTao=0;
    int count=1;
    for( currentTao=h; currentTao<=4*M_PI; currentTao+=h ){
        xStr[count] = ( xStr[count-1] + h*velStr[count-1] );
        //xStr[count] = ( velStr[count-1] - h*xStr[count-1])/(1+h*h);
        velStr[count] = ( velStr[count-1] - h*xStr[count] );
        //velStr[count] = ( xStr[count-1] + h*velStr[count-1])/(1+h*h);
        printf("Tao(%lf)\tApproximate/Real X*: %lf / %lf\t\tApproximate/Real V*: %lf / %lf\n", currentTao, xStr[count], cos(currentTao), velStr[count], -sin(currentTao) );
        count++;
    }

    return 0;
}
