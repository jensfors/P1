#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 800
#define SUMTING 2880
#define MAX_EMOTES 30
#define SEC_PR_HOUR 3600
#define SEC_PR_MIN 60

typedef struct twitchchat {
    char date[MAX];
    int hour;
    int min;
    int sec;
    char username[MAX];
    char text[MAX];
} twitchchat;

typedef struct emoticon{
    char name[20];
    int counter;
} emoticon;

typedef struct message{
    int attime;
    int counter;
} message;

typedef struct cancer{
    char emote[20];
    int counter;
} cancer;

void get_twitch_chat(FILE *ifp, twitchchat test[]);
int count_line(FILE *ifp);
void print_to_file(FILE *ofp, message messages[], emoticon **emotelist, int amountofemotes);
void emote_to_file(FILE *emotefile);
void make_emote_struct(FILE *emotefile, emoticon **emotelist, int *amountofemotes);
void print_struct(emoticon **emotelist, int amountofemotes);


int main(void){
    twitchchat *chat_struct;
    int numberoflines = 0;
    int timer = 0, i = 0, j = 0, k, startsec = 0, totalsec = 0, LUL = 0, input, amountofemotes = 0;
    message *messages = (message*)malloc(SUMTING * sizeof(message));
    emoticon **emotelist = (emoticon**)malloc(MAX_EMOTES * sizeof(emoticon*));
    
    for (i = 0; i < MAX_EMOTES; i++){
        emotelist[i] = (emoticon*)malloc(SUMTING * sizeof(emoticon));
    }

    FILE *ifp;
    FILE *ofp;
    FILE *ifpemotefile;
    FILE *ofpemotefile;

    ifp = fopen("twitchchat2.txt", "r");
    ofp = fopen("outputtwitch.txt", "w");
    
    
    do{
        printf("Press 1 to type emote \nPress 2 To run through chat \nEnter: ");
        scanf("%d", &input);
        switch(input){
            /* Case 1: Indskriv emotes til emotefil */
            case 1: ofpemotefile = fopen("emotetwitch.txt", "w"); 
                    emote_to_file(ofpemotefile); 
                    fclose(ofpemotefile);
                    ifpemotefile = fopen("emotetwitch.txt", "r");
                    make_emote_struct(ifpemotefile, emotelist, &amountofemotes); 
                    print_struct(emotelist, amountofemotes); fclose(ifpemotefile); break; 
            /* Case 2: Kør resten af programmet */
            case 2: LUL = 1; break;
        }
    }
    while(LUL == 0);


    if(ifp != NULL){
        numberoflines = count_line(ifp);
        chat_struct = (twitchchat *)malloc(numberoflines * sizeof(twitchchat));

        /* Åbner filen med emotes */
        ifpemotefile = fopen("emotetwitch.txt", "r");
        make_emote_struct(ifpemotefile, emotelist, &amountofemotes); 
        printf("Highlight Emotes:\n");
        print_struct(emotelist, amountofemotes);

        if(chat_struct == NULL){
            printf("Allocation problems. Bye.\n");
            exit(EXIT_FAILURE);
        }

        printf("Printing emote use pr 30 sec in outputtwitch.txt");
        get_twitch_chat(ifp, chat_struct);
        for(j = 0; j < amountofemotes; j++){
            for(i = 0; i < SUMTING; i++){
                emotelist[j][i].counter = 0;
            }
        }

        i = 0;
        j = 0;

        startsec = chat_struct[i].hour * SEC_PR_HOUR + chat_struct[i].min * SEC_PR_MIN + chat_struct[i].sec;
        totalsec = startsec + 30;

        while(i < numberoflines){
            j = 0;
            while(startsec <= totalsec && i < numberoflines){
                for(k = 0; k < amountofemotes; k++){
                    if(strstr(chat_struct[i].text, emotelist[k][0].name)){
                        emotelist[k][timer].counter += 1;
                    }
                }
                j++;
                i++;
                startsec = chat_struct[i].hour * SEC_PR_HOUR + chat_struct[i].min * SEC_PR_MIN + chat_struct[i].sec;
            }
            messages[timer].counter = j;
            totalsec +=30;
            timer++;           
        }
        print_to_file(ofp, messages, emotelist, amountofemotes);
    }
    else{
        printf("Can't open the file%s\n", "twitchchat.txt");
    }
    
    fclose(ifp);
    fclose(ifpemotefile);
    free(chat_struct);
    free(emotelist);
    free(messages);

    return 0;


}

void get_twitch_chat(FILE *ifp, twitchchat test[]){
    int i = 0;
    char line[400];

    while(fgets(line, sizeof(line), ifp) != NULL){ 
        sscanf(line, " [%s %d:%d:%d UTC] %[^:]: %500[^\n]", 
        test[i].date,
        &test[i].hour,
        &test[i].min,
        &test[i].sec,
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

void print_to_file(FILE *ofp, message messages[], emoticon **emotelist, int amountofemotes){
    int i, j;
    for(i = 0; i < SUMTING; i++){
        fprintf(ofp, "%d\t", messages[i].counter);
        for(j = 0; j < amountofemotes; j++){
            fprintf(ofp, "%d\t", emotelist[j][i].counter);
        }
        fprintf(ofp,"\n");
    }
    fclose(ofp);
}

/* Funktion hvor brugeren kan skrive higlight emotes/ord */
void emote_to_file(FILE *emotefile){
    char emotename[MAX_EMOTES];
    do{
        printf("\nType emote or EXIT: ");
            scanf("%s", emotename);
            if(strcmp(emotename, "EXIT") != 0){
                /* Skriver emoten/teksten til filen */
                fprintf(emotefile, "%s\n", emotename);
            }
        }
        /* Kører indtil brugeren skriver EXIT */
        while(strcmp(emotename, "EXIT") != 0);
}

/* Funktion der indlæser emotesne fra emotefilen til en struct */
void make_emote_struct(FILE *emotefile, emoticon **emotelist, int *amountofemotes){
    int k = 0;
    char line[20];
    /* Indlæser linjer indtil der ikke er flere emotes */
    while(fgets(line, sizeof(line), emotefile) != NULL){
        sscanf(line, " %20[^\n]", emotelist[k][0].name);
        emotelist[k][0].counter = 0;
        k++;
    }
    *amountofemotes = k;
}

/* Funktion der kan udskrive de valgte emotes til higlight */
void print_struct(emoticon **emotelist, int amountofemotes){
    int i;
    for(i = 0; i < amountofemotes; i++){
        printf("%s\n", emotelist[i][0].name);
    }
}