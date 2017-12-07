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

void get_twitch_chat(FILE *ifp, twitchchat test[]);
int count_line(FILE *ifp);

int main(void){
    //twitchchat test[]; 
    twitchchat *pointertest;
    int numberoflines = 0;
    
    FILE *ifp;
    ifp = fopen("twitchchat.txt", "r");

    if (ifp != NULL){
        numberoflines = count_line(ifp);
        pointertest = (twitchchat *)malloc(numberoflines * sizeof(twitchchat));

        if (pointertest == NULL){
            printf("Allocation problems. Bye.\n");
            exit(EXIT_FAILURE);
        }

        get_twitch_chat(ifp, pointertest);
        printf("\n%d\n", numberoflines);
        
        fclose(ifp);
        free(pointertest);
    }
    else{
        printf("Can't open the file%s\n", "twitchchat.txt");
    }
    

    return 0;

}

void get_twitch_chat(FILE *ifp, twitchchat test[]){
    int i = 0;
    char line[400];

    /*FILE *ifp;
    ifp = fopen("twitchchat.txt", "r"); */
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
    fclose(ifp);
}

int count_line(FILE *ifp){
    int i = 0;
    char line[400];
    while(fgets(line, sizeof(line), ifp) != NULL){ 
        i++;
    }
    rewind(ifp);
    return i;
}