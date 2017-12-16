#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define MAX 1000
#define STARTHOUR 0
#define STARTMIN 0
#define STARTSEC 0
#define MAX_LINES 63
#define MAXLINE 500
#define MAX_EMOTES 30
#define SUMTING 2880

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

typedef struct message{
    int attime;
    int counter;
} message;

int get_twitch_chat(FILE *chatfile, twitchchat chat[],  emotelist *emoticon, clock_t start_t, int amountofemotes);
void timer(int *hour, int *min, int *sec, clock_t start_t, clock_t end_t);
int count_line(FILE *chatfile);
/*void make_emote_struct(FILE *emotefile, twitchchat chat[], emotelist emotes[], int *amountofemotes);*/
void emote_counter(twitchchat chat[], emotelist *emoticon, int i, int amountofemotes);
void emote_highlight(twitchchat chat[], emotelist *emoticon, int i, int amountofemotes);
int find_questions(twitchchat test[], int n, twitchchat questions[]);
void print_emote_counter(emotelist *emoticon, int msg_nr, int amountofemotes);
int is_highlight();

void print_to_file(FILE *ofp, message messages[], emotelist *emoticon, int amountofemotes);
void emote_to_file(FILE *emotefile, emotelist *emotedummy, int *amountofemotes);
void make_emote_struct(FILE *emotefile, emotelist *emoticon, int *amountofemotes);
void print_struct(emotelist *emoticon, int amountofemotes);
void emote_option(emotelist *emoticon, emotelist *emotedummy);
void emote_option_function(emotelist *emoticon, emotelist *emotedummy, int amountofemotes);

int choose_file_name(char savefile[]);
void save_to_file(twitchchat savestruct[], char savefile[], int numberoflines);


int main(void){
    twitchchat *chat_line, questions[500];
    emotelist *emotes;
    int numberoflines = 0, msg_nr, amountofemotes = 0, question_nr_curr, question_nr_prev, prev_msg_nr, input, menu, savechat = 0, i, j;
    char savechatfile[30], txt[5] = ".txt";
    emotelist *emoticon = (emotelist*)malloc(MAX_EMOTES * sizeof(emotelist));
    emotelist *emotedummy = (emotelist*)malloc(MAX_EMOTES * sizeof(emotelist));
    message *messages = (message*)malloc(SUMTING * sizeof(message));
    clock_t start_t;

    FILE *chatfile;
    FILE *emotefile;
    FILE *ifpemotefile;

    chatfile = fopen("twitchchat2.txt", "r");

    do{
        printf("\nPress (1) to choose highlight emote \nPress (2) To save the chatlog to a txt file \nPress (3) To start live chat \nEnter: ");
        scanf("%d", &input);

        switch(input){
            /* Case 1: Indskriv emotes til emotefil */
            case 1: emote_option(emoticon, emotedummy); menu = 0; break; 
            /* Case 2: Indskriv filnavnet hvor chatloggen skal gemmes på*/
            case 2: savechat = choose_file_name(savechatfile);
                    printf("Chat log will be saved to: %s\n", savechatfile); menu = 0; break;
            /* Case 3: Kør resten af programmet */
            case 3: menu = 1; start_t = clock(); break;
            default: menu = 0; break;
        }
    }
    while(menu == 0);
  
    if(chatfile != NULL){

        /* Åbner filen med emotes */
        ifpemotefile = fopen("emotetwitch.txt", "r");
        make_emote_struct(ifpemotefile, emoticon, &amountofemotes); 
        printf("Highlight Emotes:\n");
        print_struct(emoticon, amountofemotes);
        printf("\n");

        numberoflines = count_line(chatfile);
        chat_line = (twitchchat *)malloc(numberoflines * sizeof(twitchchat));
    

        if(chat_line == NULL){
            printf("Allocation problems. Bye.\n");
            exit(EXIT_FAILURE);
        }
        while(1){
            msg_nr = get_twitch_chat(chatfile, chat_line, emoticon, start_t, amountofemotes);
            emote_highlight(chat_line, emoticon, msg_nr - 1, amountofemotes);
            if(msg_nr != prev_msg_nr){
                print_emote_counter(emoticon, msg_nr - 1, amountofemotes);
                printf("%d\t[%s %d:%d:%d UTC] %s: %s\n", 
                   msg_nr - 1, 
                   chat_line[msg_nr - 1].date,
                   chat_line[msg_nr - 1].hour,
                   chat_line[msg_nr - 1].min,
                   chat_line[msg_nr - 1].sec,
                   chat_line[msg_nr - 1].username,
                   chat_line[msg_nr - 1].text);   
            }
            prev_msg_nr = msg_nr;                   

            /*question_nr_curr = find_questions(chat_line, msg_nr, questions);
            if(question_nr_curr != question_nr_prev && question_nr_curr){
                printf("%s\n", questions[question_nr_curr - 1].text);
            }
            question_nr_prev = question_nr_curr;*/
        }
        if(savechat == 1){
            save_to_file(chat_line, savechatfile, numberoflines);
        }
        else if(savechat == 0){
            strcpy(savechatfile, chat_line[0].date);
            for(i = 0; savechatfile[i] != '\0'; i++);
            for(j = 0; txt[j] != '\0'; ++j, ++i)
            {
                savechatfile[i] = txt[j];
            }
            savechatfile[i] = '\0';
            save_to_file(chat_line, savechatfile, numberoflines);
        }


        fclose(chatfile);
        fclose(emotefile);
        free(chat_line);
    }
    else{
        printf("Can't open the file %s\n", "twitchchat2.txt");
    }
   
    return 0;
}

int get_twitch_chat(FILE *chatfile, twitchchat chat[], emotelist *emoticon, clock_t start_t, int amountofemotes){
    /* emotenumber = what emote to use for highlight in emotes.txt, 0 = Kappa, 3 = LUL */
    static int i = 0, hour = 0, min = 0, sec = 0;
    char line[500],
        dummystr[500];
    static clock_t end_t;
    end_t = clock();
    timer(&hour, &min, &sec, start_t, end_t); 

    /*printf("i = %d ", i);
    printf("%d:%d:%d  ", hour, min, sec);*/
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
            /*printf("%s: %-4d  ", emotes[2].emote, emotes[2].emotecount);*/
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
    
    /*if(end_t / CLOCKS_PER_SEC == i){ */
        end_t = clock();
        total_t = (double)((end_t - start_t) / CLOCKS_PER_SEC) + STARTSEC + STARTMIN * 60 + STARTHOUR * 60 * 60;
        *sec = total_t % 60;
        *min = (total_t / 60) % 60;
        *hour = total_t / 3600;
        i++;
    
}

int count_line(FILE *chatfile){
    int i = 0;
    char line[MAXLINE];
    while(fgets(line, sizeof(line), chatfile) != NULL){
        i++;
    }
    rewind(chatfile);
    return i;
}
/*
void make_emote_struct(FILE *emotefile, twitchchat chat[], emotelist emotes[], int *amountofemotes){
    int k = 0;
    char line[20];
    while(fgets(line, sizeof(line), emotefile) != NULL){
        sscanf(line, " %20[^\n]", emotes[k].emote);
        emotes[k].emotecount = 1;
        k++;
    }
    *amountofemotes = k;
}
*/
/*void emote_counter(twitchchat chat[], emotelist emotes[], int i, int amountofemotes){
    int j;
    for(j = 0; j < amountofemotes; j++){
        if(strstr(chat[i].text, emotes[j].emote)){  
            emotes[j].emotecount = emotes[j].emotecount + 1;
            printf("%-10s: %-4d", emotes[j].emote, emotes[j].emotecount);
        }
    }
}*/

/* Mathias has copyright on this function Jebaited */
void emote_highlight(twitchchat chat[], emotelist *emoticon, int i, int amountofemotes){
    /* secback = sec you go back to check for emotes */
    /* triggernumber = how many emotes in secback to make highlight */
    int j = 0, k, n = 0, secback = 20, startsec, totalsec, emotefound = 0,
        emotenumber, hour, min, sec, dummy, prev_msg_nr;
    
    totalsec = (chat[i].hour * 3600) + (chat[i].min * 60) + (chat[i].sec);
    startsec = (totalsec - secback) < 0 ? 0 : (totalsec - secback);
 
    hour = startsec / 3600;
    min = (startsec / 60) % 60;
    sec = startsec % 60;

    if(i != prev_msg_nr){
        
        for(j = 0; j < i; j++){
            if((chat[j].hour < hour) || (chat[j].min < min) || (chat[j].sec < sec)){
                n++;
            }
        }
        for(emotenumber = 0; emotenumber < amountofemotes; emotenumber++){
            emotefound = 0;
            if(strstr(chat[i].text, emoticon[emotenumber].emote)){
                for(k = n; k < i; k++){
                    if(strstr(chat[k].text, emoticon[emotenumber].emote)){
                        emotefound = 1;
                    }
                }
                if(!emotefound){
                    emoticon[emotenumber].emotecount = 1;
                }
                else{
                    dummy = emoticon[emotenumber].emotecount;
                    emoticon[emotenumber].emotecount = dummy + 1;
                }
            }
        }
    }
    prev_msg_nr = i;

    is_highlight();

}

void print_emote_counter(emotelist *emoticon, int msg_nr, int amountofemotes){
    int i;
    static int prev_msg_nr;

    if(msg_nr != prev_msg_nr){
        for(i = 0; i < amountofemotes; i++){
            if(emoticon[i].emotecount > 20){
                printf("%s: %d\t", emoticon[i].emote, emoticon[i].emotecount);
            }
        }
    }
    prev_msg_nr = msg_nr;

}

int find_questions(twitchchat test[], int n, twitchchat questions[]){
    static int j = 0;
    char curr[MAX];
    static char prev[MAX];
    strcpy(curr, test[n].text);
    if(!(prev == NULL || strcmp(curr, prev) == 0)){
        if(strchr(curr, '?')){
            questions[j] = test[n];
            j++;
        }
    }
    strcpy(prev,test[n].text);

    return j;
}

int is_highlight(){
    return 1;
}

void emote_option(emotelist *emoticon, emotelist *emotedummy){
    int amountofemotes = 0, input, breaker;

    FILE *dummyfile;

    do{
        printf("Hello, you want to delete previous emotes (1) or add extra (2): ");
        scanf("%d", &input); 

        switch(input){
            case 1: emote_option_function(emoticon, emotedummy, amountofemotes); 
                    breaker = 0; break;

            case 2: dummyfile = fopen("emotetwitch.txt", "r"); 
                    make_emote_struct(dummyfile, emotedummy, &amountofemotes);
                    emote_option_function(emoticon, emotedummy, amountofemotes);
                    breaker = 0; break;

            default: printf("Unknown command, try again!\n"); breaker = 1; 
        }
    }
    while(breaker != 0);
}

void emote_option_function(emotelist *emoticon, emotelist *emotedummy, int amountofemotes){
    FILE *ifpemotefile;
    FILE *ofpemotefile;
    
    ofpemotefile = fopen("emotetwitch.txt", "w"); 
    emote_to_file(ofpemotefile, emotedummy, &amountofemotes); 
    fclose(ofpemotefile);
    ifpemotefile = fopen("emotetwitch.txt", "r");
    make_emote_struct(ifpemotefile, emoticon, &amountofemotes); 
}

/* Funktion hvor brugeren kan skrive higlight emotes/ord */
void emote_to_file(FILE *emotefile, emotelist *emotedummy, int *amountofemotes){
    int i;
    char emotename[MAX_EMOTES], meme[100];
    while(fgets(meme, strlen(meme), emotefile) != NULL);
    for(i = 0; i < *amountofemotes; i++){
        fprintf(emotefile, "%s\n", emotedummy[i].emote);
    }
    do{
        printf("\nType emotename or EXIT: ");
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
void make_emote_struct(FILE *emotefile, emotelist *emoticon, int *amountofemotes){
    int k = 0;
    char line[20];
    /* Indlæser linjer indtil der ikke er flere emotes */
    while(fgets(line, sizeof(line), emotefile) != NULL){
        sscanf(line, " %20[^\n]", emoticon[k].emote);
        emoticon[k].emotecount = 0;
        k++;
    }
    *amountofemotes = k;
}

/* Funktion der kan udskrive de valgte emotes til higlight */
void print_struct(emotelist *emoticon, int amountofemotes){
    int i;
    for(i = 0; i < amountofemotes; i++){
        printf("%s\n", emoticon[i].emote);
    }
}

/* Funktion hvor man vælger den fil chatlogen skal gemmes på */
int choose_file_name(char savefile[]){
    int i, j, res;
    char txt[5] = ".txt";

    printf("Please type in the name of the .txt file the chatlog will be saved to. \nEnter name here: ");
    scanf("%s", savefile);

    /* Filnavnet for sat '.txt' på enden */
    for(i = 0; savefile[i] != '\0'; i++);
    for(j = 0; txt[j] != '\0'; ++j, ++i)
    {
        savefile[i] = txt[j];
    }
    savefile[i] = '\0';
    res = 1;
    
    return res;
}

/* Funktion der udskriver chatlogen til den valgte fil af brugeren */
void save_to_file(twitchchat savestruct[], char savefile[], int numberoflines){
    int i;
    FILE *ofp;
    ofp = fopen(savefile, "w");

    for(i = 0; i < numberoflines; i++){
        fprintf(ofp, "[%s %d:%d:%d UTC] %s: %s\n", 
        savestruct[i].date,
        savestruct[i].hour,
        savestruct[i].min,
        savestruct[i].sec,
        savestruct[i].username,
        savestruct[i].text);    
    }
}

