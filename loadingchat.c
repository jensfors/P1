#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 800

typedef struct twitchchat {
    char date[MAX];
    int hour;
    int min;
    int sec;
    char username[MAX];
    char text[MAX];
} twitchchat;

void get_twitch_chat(twitchchat test[], int *numberoflines);

int main(void){
    twitchchat test[MAX]; 
    int numberoflines = 0;

    get_twitch_chat(test, &numberoflines);

    printf("\n%d\n", numberoflines);

    return 0;

}

void get_twitch_chat(twitchchat test[], int *numberoflines){
    int i = 0;
    char line[400];

    FILE *ifp;
    ifp = fopen("twitchchat.txt", "r");
    while(fgets(line, sizeof(line), ifp) != NULL){ 
        sscanf(line, " [%s %d:%d:%d UTC] %[^:]: %500[^\n]", 
        test[i].date,
        &test[i].hour,
        &test[i].min,
        &test[i].sec,
        test[i].username,
        test[i].text);

        printf("[%s %d:%d:%d UTC] %s: %s\n", test[i].date,
        test[i].hour,
        test[i].min,
        test[i].sec,
        test[i].username,
        test[i].text);
        i++;
    }
    *numberoflines = i;
    fclose(ifp);
}