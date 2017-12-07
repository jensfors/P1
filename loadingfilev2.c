#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define MAX 1000
#define STARTHOUR 3
#define STARTMIN 4
#define STARTSEC 20

typedef struct twitchchat {
    char date[10];
    int hour;
    int min;
    int sec;
    char username[50];
    char text[450];
    int chats;
} twitchchat;

void get_twitch_chat(twitchchat test[], int *numberoflines);
void timer(int *hour, int *min, int *sec, clock_t start_t, clock_t end_t);

int main(void){
    twitchchat test[MAX], anothertest[MAX], endtime[MAX]; 
    int numberoflines = 0, i, j, k = 0, breaker = 0;

    get_twitch_chat(test, &numberoflines);
    printf("\nChat messages: %d\n", numberoflines);

    for(i = 0; i < numberoflines; i++){
        for(j = 0; j <= k; j++){
            if((strcmp(test[i].username, anothertest[j].username) == 0)){
                anothertest[j].chats += 1;
                breaker = 1; 
            }
        }
          
        if(breaker != 1){
            anothertest[k+1] = test[i];
            anothertest[k+1].chats = 1;
            k += 1;
        }          
        breaker = 0;
    }

    endtime[0].hour = test[numberoflines-1].hour - test[0].hour;
    endtime[0].min = test[numberoflines-1].min - test[0].min;
    endtime[0].sec = test[numberoflines-1].sec - test[0].sec;

    printf("Users: %d\n", k);

    for(i = 0, breaker = 0; i < k && breaker == 0; i++){
        if(strcmp(anothertest[i].username, "frosty_nipple_god") == 0){
            printf("\nUser %s chats %d times in the time frame %d:%d:%d\n", 
            anothertest[i].username, anothertest[i].chats, endtime[0].hour, endtime[0].min, endtime[0].sec);
            breaker = 1;
        }
    }

    return 0;

}

void get_twitch_chat(twitchchat test[], int *numberoflines){
    int i = 0, hour = 0, min = 0, sec = 0;
    char line[500],
    dummystr[500];
    fpos_t pos;
    const clock_t start_t = clock();
    clock_t end_t;

    FILE *ifp;
    ifp = fopen("twitchchat.txt", "r");

    while(1){
        end_t = clock();
        timer(&hour, &min, &sec, start_t, end_t);
        Sleep(250);
        printf("\n%d:%d:%d\n", hour, min, sec);
        fgetpos(ifp, &pos);
        if(fgets(line, sizeof(line), ifp) != NULL){ 
            sscanf(line, " [%s %d:%d:%d UTC] %[^:]: %500[^\n]", 
            test[i].date,
            &test[i].hour,
            &test[i].min,
            &test[i].sec,
            test[i].username,
            test[i].text);
            sscanf(line, " %[^\n]", dummystr);
            printf("HELLO: %d %d %d\n", test[i].hour, test[i].min, test[i].sec);

            if(test[i].hour == (hour+STARTHOUR) && test[i].min == (min+STARTMIN) && test[i].sec == (sec+STARTSEC)){
                printf("[%s %d:%d:%d UTC] %s: %s\n", test[i].date,
                test[i].hour,
                test[i].min,
                test[i].sec,
                test[i].username,
                test[i].text);
                i++;
            }
            else{
                fseek(ifp, -(strlen(dummystr) + 2), SEEK_CUR); /* seek backward from the current pos */
            }
            printf("%d\n", strlen(dummystr));
            printf("%ld\n", ftell(ifp));
        }
    }
    *numberoflines = i;
    fclose(ifp);
}

void timer(int *hour, int *min, int *sec, clock_t start_t, clock_t end_t){
    static int i = 0;
    clock_t total_t;

    if(end_t/CLOCKS_PER_SEC == i) {
        end_t = clock();
        total_t = (double)((end_t - start_t) / CLOCKS_PER_SEC);
        *sec = total_t % 60;
        *min = *sec / 60;
        *hour = *min / 60;
        i++;
    }

}
