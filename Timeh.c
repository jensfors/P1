#include <time.h>
#include <stdio.h>

#define START_TID 14

int main () {
    clock_t start_t, end_t, total_t;
    int i = 0, min=0, hour=0, sec = 0;

    start_t = clock();
    end_t = clock();
    total_t = (int)((end_t - start_t) / CLOCKS_PER_SEC);
    while(1){
        if(clock()/CLOCKS_PER_SEC == i) {
           end_t = clock();
           total_t = (double)((end_t - start_t) / CLOCKS_PER_SEC);
           sec = total_t % 60;
           min = total_t / 60;
           hour = min / 60;
           
           printf("%d:%d:%d\n", hour, min, sec);
           i++;
        }
    }

   return(0);
}