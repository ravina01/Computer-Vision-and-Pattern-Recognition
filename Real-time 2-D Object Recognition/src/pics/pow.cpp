// #include <iostream>
// #include <stdio.h>
// #include <sys/time.h>
// #include <math.h>

// using namespace std;

// int main(int argc, char *argv[]) {

//     Timer clock; // Timer<milliseconds, steady_clock>
//     Timer clock1; // Timer<milliseconds, steady_clock>

//     int x = 8;

//     clock.tick();
//     int square = x * x;
//     clock.tock();

//     cout << "Run time = " << clock.duration().count() << " ms\n";

//     clock1.tick();
//     pow(x, 2);
//     clock1.tock();

//     cout << "Run time = " << clock1.duration().count() << " ms\n";

// }

#include<time.h> // for clock
#include<math.h> // for fmod
#include<cstdlib> //for system
#include <stdio.h> //for delay
#include <iostream>

using namespace std;

int main()
{


   clock_t t1,t2;

   t1=clock(); // first time capture

   // Now your time spanning loop or code goes here
   // i am first trying to display time elapsed every time loop runs

   int ddays=0; // d prefix is just to say that this variable will be used for display
   int dhh=0;
   int dmm=0;
   int dss=0;

   int loopcount = 1000 ; // just for demo your loop will be different of course

   for(float count=1;count<loopcount;count++)
   {

     t2=clock(); // we get the time now

     float difference= (((float)t2)-((float)t1)); // gives the time elapsed since t1 in milliseconds

    // now get the time elapsed in seconds

    float seconds = difference/1000; // float value of seconds
    if (seconds<(60*60*24)) // a day is not over
    {
        dss = fmod(seconds,60); // the remainder is seconds to be displayed
        float minutes= seconds/60;  // the total minutes in float
        dmm= fmod(minutes,60);  // the remainder are minutes to be displayed
        float hours= minutes/60; // the total hours in float
        dhh= hours;  // the hours to be displayed
        ddays=0;
    }
    else // we have reached the counting of days
    {
        float days = seconds/(24*60*60);
        ddays = (int)(days);
        float minutes= seconds/60;  // the total minutes in float
        dmm= fmod(minutes,60);  // the rmainder are minutes to be displayed
        float hours= minutes/60; // the total hours in float
        dhh= fmod (hours,24);  // the hours to be displayed

    }

    cout<<"Count Is : "<<count;


    // the actual working code here,I have just put a delay function
    //delay(1000);
    system("cls");

 } // end for loop

}// end of main 
