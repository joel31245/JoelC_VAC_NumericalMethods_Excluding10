#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double h = .001;

// RUNTIME ACCURACY ERROR FOR THE NEW STUFF

int main()
{
    double th;
    double mu;

    th = 0.1;
    mu = 0;
    int B = 1;
    int O = 2;

    double currentTao=0;
    for( currentTao=h; currentTao<=4*M_PI; currentTao+=h ){
        //th[count] = ( th[count-1] + h*mu[count-1] );
        th = th + h*mu;
        //mu[count] = ( mu[count-1] - h*( 1+B*cos(O*currentTao) )*sin(th[count]) );
        mu = mu - h*( 1+B*cos(O*currentTao) )*sin(th);

        printf("Tao(%lf)\t Theta*: %lf \t\t Mu*: %lf \n", currentTao, th, mu );
    }

    return 0;
}
