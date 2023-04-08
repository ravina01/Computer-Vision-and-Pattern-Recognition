/* This program does a timimg comparison of 
using C pow function to square 2 values versus 
using simple multiplication also calculate relative cost
*/
#include <iostream>
#include <stdio.h>
#include <sys/time.h>
#include <math.h>

using namespace std;

//Utility function to capture current time
double getTime() {
  struct timeval cur;

  gettimeofday( &cur, NULL );
  return( cur.tv_sec + cur.tv_usec / 1000000.0 );
}

int main()
{
    // calculate time difference of square operation using multipication
    int num = 10;
    double time1 = getTime();

    for(int i =0; i<100000; i++){
        int square = num * num;
    }
    double time2 = getTime();

    double timeDiff_square = time2 - time1;

    cout<<"Execuation time taken by square operation using multiplication = "<<timeDiff_square<<endl;


    // calculate time difference of square operation using inbuilt pow function
    double time3 = getTime();

    for(int i =0; i<100000; i++){
        int square = pow(num, 2);
    }
    double time4 = getTime();
    double timeDiff_pow = time4 - time3;

    cout<<"Execuation time taken by inbuilt pow function = "<<timeDiff_pow<<endl;

    double computaional_cost = timeDiff_pow/timeDiff_square;
    cout<<"The relative computational cost = "<<computaional_cost<<endl;



    //cases where inbuilt pow is better than simple multiplication

    cout<<"\nCase where inbuilt pow function is better, when we are taking decimals as exponent"<<endl;
    
    float x = 6.8;
    float y = 11.2;
    
    double time5 = getTime();

    for(int i =0; i<100000; i++){
        float square = exp(y*log(x));
    }
    double time6 = getTime();
    double timeDiff_decimalexponent = time6 - time5;

    cout<<"Execuation time taken by Decimal exponent without pow function = "<<timeDiff_decimalexponent<<endl;


    double time7 = getTime();
    for(int i =0; i<100000; i++){
        float square = pow(x,y);
    }
    double time8 = getTime();
    double timeDiff_decimalpow = time8 - time7;

    cout<<"Execuation time taken by Decimal exponent using pow function = "<<timeDiff_decimalpow<<endl;

    return 0;

}