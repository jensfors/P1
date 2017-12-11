#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define MAX 1000
#define STARTHOUR 0
#define STARTMIN 0
#define STARTSEC 0

typedef struct twitchchat{
    char date[MAX];
    int hour;
    int min;
    int sec;
    char username[MAX];
    char text[MAX];
} twitchchat;

typedef struct emotelist {
    char emote[20];
    int emotecount;
} emotelist;

int get_twitch_chat(FILE *chatfile, FILE *emotefile, twitchchat chat[],  emotelist emotes[], clock_t start_t, int amountofemotes);
void timer(int *hour, int *min, int *sec, clock_t start_t, clock_t end_t);
int count_line(FILE *chatfile);
void make_emote_struct(FILE *emotefile, twitchchat chat[], emotelist emotes[], int *amountofemotes);
void emote_counter(twitchchat chat[], emotelist emotes[], int i, int amountofemotes);
void emote_highlight(twitchchat chat[], emotelist emotes[], int i, int emotenumber);

void highlight_options(){
    printf("meme");
} 

int main(void){
    /* twitchchat chat[]; */
    twitchchat *chat_line;
    emotelist *emotes;
    int numberoflines = 0, msg_nr, amountofemotes = 0, choice;
    const clock_t start_t = clock();
    char highlightemotes[10];
    highlightemotes[0] = 2;
    highlightemotes[1] = 0;
    

    FILE *chatfile;
    chatfile = fopen("twitchchat2.txt", "r");
    
    FILE *emotefile;
    emotefile = fopen("emotes.txt", "r");

    /*
    do{
        printf("Enter number: ");
        scanf("%d", &choice);

        switch(choice){
            case 1: highlight_options();
            case 2: */
            if(chatfile != NULL){
                numberoflines = count_line(chatfile);
                chat_line = (twitchchat *)malloc(numberoflines * sizeof(twitchchat));
                emotes = (emotelist *)malloc(20 * sizeof(emotelist));
        
                make_emote_struct(emotefile, chat_line, emotes, &amountofemotes);
        
                if (chat_line == NULL){
                    printf("Allocation problems. Bye.\n");
                    exit(EXIT_FAILURE);
                }
                while(1){
                    msg_nr = get_twitch_chat(chatfile, emotefile, chat_line, emotes, start_t, amountofemotes);
                }
                fclose(chatfile);
                fclose(emotefile);
                printf("\n%d\n", numberoflines);
        
                free(chat_line);
            }
            else{
                printf("Can't open the file%s\n", "twitchchat.txt");
            }
            /*
        }
    }
    while(choice != 0);
*/
    return 0;
}

int get_twitch_chat(FILE *chatfile, FILE *emotefile, twitchchat chat[], emotelist emotes[], clock_t start_t, int amountofemotes){
    /* emotenumber = what emote to use for highlight in emotes.txt, 0 = Kappa, 3 = LUL */
    static int i = 0, hour = 0, min = 0, sec = 0, emotenumber = 0;
    char line[500],
        dummystr[500];
    static clock_t end_t;
    end_t = clock();
    timer(&hour, &min, &sec, start_t, end_t);

    Sleep(20);
    /*printf("%d:%d:%d  ", hour, min, sec); */
    if(fgets(line, sizeof(line), chatfile) != NULL){
        sscanf(line, " [%s %d:%d:%d UTC] %[^:]: %500[^\n]",
               chat[i].date,
               &chat[i].hour,
               &chat[i].min,
               &chat[i].sec,
               chat[i].username,
               chat[i].text);
        sscanf(line, " %[^\n]", dummystr);
        /* printf("HELLO: %d %d %d\n", chat[i].hour, chat[i].min, chat[i].sec);*/

        if(chat[i].hour == hour && chat[i].min == min && chat[i].sec == sec){
            printf("%-8d", i);
            if(strstr(chat[i].text, emotes[emotenumber].emote)){
                emote_highlight(chat, emotes, i, emotenumber);
            } 
            emote_counter(chat, emotes, i, amountofemotes);
            /*printf("%s: %-4d  ", emotes[2].emote, emotes[2].emotecount);*/
            printf("[%s %d:%d:%d UTC] %s: %s\n", chat[i].date,
                   chat[i].hour,
                   chat[i].min,
                   chat[i].sec,
                   chat[i].username,
                   chat[i].text);
            i++;
        }
        else{
            fseek(chatfile, -(strlen(dummystr) + 2), SEEK_CUR); /* seek backward from the current pos */
        }
        /*printf("%d\n", strlen(dummystr));
        printf("%ld\n", ftell(chatfile));*/
    }

    return i;
}

void timer(int *hour, int *min, int *sec, clock_t start_t, clock_t end_t){
    static int i = 0;
    clock_t total_t;

    if(end_t / CLOCKS_PER_SEC == i){
        end_t = clock();
        total_t = (double)((end_t - start_t) / CLOCKS_PER_SEC) + STARTSEC + STARTMIN * 60 + STARTHOUR * 60 * 60;
        *sec = total_t % 60;
        *min = (total_t / 60) % 60;
        *hour = total_t / 3600;
        i++;
    }
}

int count_line(FILE *chatfile){
    int i = 0;
    char line[400];
    while(fgets(line, sizeof(line), chatfile) != NULL){
        i++;
    }
    rewind(chatfile);
    return i;
}


void make_emote_struct(FILE *emotefile, twitchchat chat[], emotelist emotes[], int *amountofemotes){
    int k = 0;
    char line[20];
    while(fgets(line, sizeof(line), emotefile) != NULL){
        sscanf(line, " %20[^\n]", emotes[k].emote);
        emotes[k].emotecount = 0;
        k++;
    }
    *amountofemotes = k;
}

void emote_counter(twitchchat chat[], emotelist emotes[], int i, int amountofemotes){
    int j;
    for(j = 0; j < amountofemotes; j++){
        if(strstr(chat[i].text, emotes[j].emote)){  
            emotes[j].emotecount = emotes[j].emotecount + 1;
            printf("%-10s: %-4d", emotes[j].emote, emotes[j].emotecount);
        }
    }
}

/* Mathias has copyright on this function Jebaited */
void emote_highlight(twitchchat chat[], emotelist emotes[], int i, int emotenumber){
    twitchchat emotetester[5];
    /* secback = sec you go back to check for emotes */
    /* triggernumber = how many emotes in secback to make highlight */
    int j = 0, k, n = 0, emotecounter = 0, secback = 30, startsec, totalsec, triggernumber = 40;
    
    totalsec = (chat[i].hour * 3600) + (chat[i].min * 60) + (chat[i].sec);
    startsec = (totalsec - secback) < 0 ? 0 : (totalsec - secback);
 
    emotetester[0].hour = startsec / 3600;
    emotetester[0].min = (startsec / 60) % 60;
    emotetester[0].sec = startsec % 60;

    for(j = 0; j < i; j++){
        if((chat[j].hour < emotetester[0].hour) || (chat[j].min < emotetester[0].min) || (chat[j].sec < emotetester[0].sec)){
            n++;
        }
    }
    for(k = n; k <= i; k++){
        if(strstr(chat[k].text, emotes[emotenumber].emote)){
            emotecounter++; 
        }
    }

    if(emotecounter >= triggernumber){
        printf("-----HIGHLIGHT-----   ");
    }
    printf("Counter: %d  ", emotecounter);
}
