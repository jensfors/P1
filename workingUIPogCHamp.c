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


int get_twitch_chat(FILE *chatfile, twitchchat chat[],  emotelist *emoticon, clock_t start_t, int amountofemotes);
void timer(int *hour, int *min, int *sec, clock_t start_t, clock_t end_t);
int count_line(FILE *chatfile);
/*void make_emote_struct(FILE *emotefile, twitchchat chat[], emotelist emotes[], int *amountofemotes);*/
void emote_counter(twitchchat chat[], emotelist *emoticon, int i, int amountofemotes);
void emote_highlight(twitchchat chat[], emotelist *emoticon, int i, int amountofemotes);
int find_questions(twitchchat test[], int n, twitchchat questions[]);
void print_emote_counter(emotelist *emoticon, int msg_nr, int amountofemotes);
int is_highlight();
void emote_to_file(FILE *emotefile, emotelist *emotedummy, int *amountofemotes);
void make_emote_struct(FILE *emotefile, emotelist *emoticon, int *amountofemotes);
void print_struct(emotelist *emoticon, int amountofemotes);
void emote_menu(emotelist *emoticon, emotelist *emotedummy);
void emote_option_function(emotelist *emoticon, emotelist *emotedummy, int amountofemotes);
void print_it_all(twitchchat chat[], twitchchat questions[], emotelist *emoticon, int msg_nr, int question_nr, int amountofemotes);
int choose_file_name(char savefile[]);
void save_to_file(twitchchat savestruct[], char savefile[], int numberoflines);
void add_txt_to_filename(char savefile[]);
void set_default_emotes(emotelist standard_emotes[]);
void chatfile_menu(char twitchchatfile[], int *loadchat);
void ctrl_f(twitchchat chat[], int numberoflines);
void get_offline_chat(twitchchat offlinechat[], FILE *chatfile);


int main(void){
    twitchchat *chat_line, questions[500], *offlinechat;
    int numberoflines = 0, msg_nr, amountofemotes = 0, question_nr, offlineinput, maininput, mainmenu, offlinemenu, onlinemenu, 
        savechat = 0, loadchat = 1, offline = 0, online = 0, onlineinput, offline_numberoflines = 0;
    char savechatfile[30], twitchchatfile[30], std_chatfile[30] = "twitchchat2.txt";
    emotelist *emoticon = (emotelist*)malloc(MAX_EMOTES * sizeof(emotelist));
    emotelist *emotedummy = (emotelist*)malloc(MAX_EMOTES * sizeof(emotelist));
    clock_t start_t;

    FILE *chatfile;
    FILE *emotefile;
    FILE *ifpemotefile;

    do{
        /* Main menu */
        printf("\n\nMain menu \n(1) Live options \n(2) Offline options \n(3) Go live \nEnter: ");
        scanf("%d", &maininput);
        switch(maininput){
            case 1: printf("\n\nShowing live settings"); online = 1; mainmenu = 0; break; 
            case 2: printf("\n\nShowing offline settings"); offline = 1; mainmenu = 0; break;
            case 3: printf("\n\nGoing Live \n\n");mainmenu = 1; start_t = clock(); break;
            default: printf("Unknown command, try again!\n"); mainmenu = 0; break;
        }

        /* Live instillinger */
        if(online == 1){
            do{
                printf("\n\nLive settings \n(1) Choose highlight emotes \n(2) Choose file to save chatlog to \n(3) Only show filtered chat \n(4) Only show questions" 
                       "\n(5) Only show highlights \n(6) Show filtered chat/questions/highlight \n(7) Go live \n(8) BACK TO MAIN MENU \nEnter: ");
                scanf("%d", &onlineinput);

                switch(onlineinput){
                    /* Case 1: Indskriv emotes til emotefil */
                    case 1: emote_menu(emoticon, emotedummy); onlinemenu = 0; break; 
                    /* Case 2: Gem chatloggen til en custom fil */
                    case 2: printf("Please type in the name of the .txt file the chatlog will be saved to"); 
                            savechat = choose_file_name(savechatfile);
                            printf("\nChat log will be saved to: %s\n", savechatfile); onlinemenu = 0; break;
                    /* Case 3: Viser kun filtreret chat */
                    case 3: printf("Showing filtered chat\n"); onlinemenu= 0; break;
                    /* Case 4: Viser kun spørgsmål */
                    case 4: printf("Showing questions\n"); onlinemenu= 0; break;
                    /* Case 5: Viser kun highlights */
                    case 5: printf("Showing highlights\n"); onlinemenu = 0; break;
                    /* Case 6: Viser filtreret chat/spørgsmål/highlight */
                    case 6: printf("Showing filtered chat/questions/highlight\n"); onlinemenu = 0; break;
                    /* Case 7: Starter streamchatten */
                    case 7: mainmenu = 1; online = 0; onlinemenu = 1; start_t = clock(); break;
                    /* Case 8: Gå tilbage til main menu */
                    case 8: onlinemenu = 1; online = 0; break;
                    default: printf("Unknown command, try again!\n"); onlinemenu = 0; break;
                }
            }
            while(onlinemenu == 0);
        }

        /* Offline indstillinger */
        if(offline == 1){
            do{
                printf("\n\nOffline settings \n(1) Choose highlight emotes \n(2) Choose chatfile \n(3) Search chat \n(4) Show all questions"
                       "\n(5) Show highlight timestamps \n(6) Go through chatfile \n(7) BACK TO MAIN MENU \nEnter: ");
                scanf("%d", &offlineinput);

                switch(offlineinput){
                    /* Case 1: Indskriv emotes til emotefil */
                    case 1: emote_menu(emoticon, emotedummy); offlinemenu = 0; break; 
                    /* Case 2: Vælg hvilken fil chatten skal læses fra */
                    case 2: chatfile_menu(twitchchatfile, &loadchat);
                    if(loadchat == 2){
                        printf("Loading chat from: %s\n", twitchchatfile);
                        chatfile = fopen(twitchchatfile, "r");
                    }
                    else if(loadchat == 1){
                        printf("Loading chat from: %s\n", std_chatfile);
                        chatfile = fopen(std_chatfile, "r");
                    }
                    offline_numberoflines = count_line(chatfile);
                    offlinechat = (twitchchat *)malloc(offline_numberoflines * sizeof(twitchchat));
                    get_offline_chat(offlinechat, chatfile);
                    offlinemenu = 0; break;
                    /* Case 3: Søg efter noget i chatten */
                    case 3: printf("Searching chat\n"); 
                    ctrl_f(offlinechat, offline_numberoflines);
                    offlinemenu = 0; break;
                    /* Case 4: Vis alle spørgsmål fra filen */
                    case 4: printf("Showing questions\n"); offlinemenu = 0; break;
                    /* Case 5: Vis alle steder hvor der var highlights */
                    case 5: printf("Showing highlight timestamps\n"); offlinemenu = 0; break;
                    /* Case 6: Start læsningen af filen */
                    case 6: mainmenu = 1; offlinemenu = 1; start_t = clock(); break;
                    /* Case 7: Gå tilbage til main menu */
                    case 7: offlinemenu = 1; offline = 0; offlinemenu = 1; break;
                    default: printf("Unknown command, try again!\n"); offlinemenu = 0; break;
                }
            }
            while(offlinemenu == 0);
        }
    }
    while(mainmenu == 0);



    if(loadchat == 2){
        printf("Loading chat from: %s\n", twitchchatfile);
        chatfile = fopen(twitchchatfile, "r");
    }
    else if(loadchat == 1){
        printf("Loading chat from: %s\n", std_chatfile);
        chatfile = fopen(std_chatfile, "r");
    }
  
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
            question_nr = find_questions(chat_line, msg_nr - 1, questions);
            print_it_all(chat_line, questions, emoticon, msg_nr, question_nr, amountofemotes);
        }
        /* Gemmer chatlogen til den angivet fil ellers dato.txt */
        if(savechat == 1){
            save_to_file(chat_line, savechatfile, numberoflines);
        }
        else if(savechat == 0){
            strcpy(savechatfile, chat_line[0].date);
            add_txt_to_filename(savechatfile);        
            save_to_file(chat_line, savechatfile, numberoflines);
        }


        fclose(chatfile);
        fclose(emotefile);
        free(chat_line);
    }
    else{
        printf("Can't open the file %s\n", std_chatfile);
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

void get_offline_chat(twitchchat offlinechat[], FILE *chatfile){
    char line[600];
    int i = 0;

    while(fgets(line, sizeof(line), chatfile) != NULL){
        sscanf(line, " [%s %d:%d:%d UTC] %[^:]: %600[^\n]",
               offlinechat[i].date,
               &offlinechat[i].hour,
               &offlinechat[i].min,
               &offlinechat[i].sec,
               offlinechat[i].username,
               offlinechat[i].text);
        //sscanf(line, " %[^\n]", dummystr);
        i++; 

    }
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

int find_questions(twitchchat test[], int n, twitchchat questions[]){
    static int j = 0;
    char curr[MAX];
    static char prev[MAX];
    strcpy(curr, test[n].text);
    if(!(prev == NULL || strcmp(curr, prev) == 0)){
        if(strstr(curr, "????")){
            return j;
        }
        else if(strchr(curr, '?')){
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

void emote_menu(emotelist *emoticon, emotelist *emotedummy){
    int amountofemotes = 0, input, breaker, i;
    emotelist standard_emotes[5];

    strcpy(standard_emotes[0].emote, "PogChamp");
    strcpy(standard_emotes[1].emote, "Kappa");
    strcpy(standard_emotes[2].emote, "LUL");
    strcpy(standard_emotes[3].emote, "monkaS");
    strcpy(standard_emotes[4].emote, "Jebaited");

    FILE *dummyfile;

    do{
        printf("(1) Delete all emotes and add new ones \n(2) Add extra emotes \n(3) Set emotes to standard emotes \n(4) Show standard emotes \n(5) BACK \nEnter: ");
        scanf("%d", &input); 

        switch(input){
            case 1: emote_option_function(emoticon, emotedummy, amountofemotes); 
                    breaker = 1; break;

            case 2: dummyfile = fopen("emotetwitch.txt", "r"); 
                    make_emote_struct(dummyfile, emotedummy, &amountofemotes);
                    emote_option_function(emoticon, emotedummy, amountofemotes);
                    breaker = 1; break;

            case 3: printf("\nSetting emotes to default\n");
                    set_default_emotes(standard_emotes);
                    breaker = 1; break;

            case 4: printf("\nStandard emotes are:\n");
                    for(i = 0; i < 5; i++){
                        printf("%s\n", standard_emotes[i].emote);
                    }
                    printf("\n"); breaker = 1; break;

            case 5: breaker = 0; break;

            default: printf("Unknown command, try again!\n"); breaker = 1; 
        }
    }
    while(breaker != 0);
}

void set_default_emotes(emotelist standard_emotes[]){
    int i;
    FILE *emotefile;
    emotefile = fopen("emotetwitch.txt", "w");

    for(i = 0; i < 5; i++){
        fprintf(emotefile, "%s\n", standard_emotes[i].emote);
    }
    fclose(emotefile);
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
    char emotename[MAX_EMOTES], emotelen[100];
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
    int res;

    printf("\nEnter name here: ");
    scanf("%s", savefile);

    /* Filnavnet får sat '.txt' på enden */
    add_txt_to_filename(savefile);
    res = 1;
    
    return res;
}

/* Menu til valg af den fil chatten skal læses fra */
void chatfile_menu(char twitchchatfile[], int *loadchat){
    int input, menu;

    printf("\nChoose what chatfile to load the chat from \n(1) Current streamfile \n(2) Choose custom file \nEnter: ");
    scanf("%d", &input);

    do{
        switch(input){
            case 1: *loadchat = 1; menu = 1; break;
            case 2: *loadchat = 2; menu = 1; 
                    printf("Please type in the name of the .txt file the chat will be loaded from"); 
                    choose_file_name(twitchchatfile);
                    printf("\nChat will be loaded from %s\n", twitchchatfile); break;
            default: menu = 0; break; 
        }
    }
    while(menu != 1);
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

void add_txt_to_filename(char savefile[]){
    int i, j;
    char txt[5] = ".txt";
    for(i = 0; savefile[i] != '\0'; i++);
    for(j = 0; txt[j] != '\0'; ++j, ++i)
    {
        savefile[i] = txt[j];
    }
    savefile[i] = '\0';
}

void print_it_all(twitchchat chat[], twitchchat questions[], emotelist emoticon[], int msg_nr, int question_nr, int amountofemotes){
    static int prev_msg_nr = 0, prev_question_nr = 0;
    int i, chatline, count, first_question, j = 0;
    static int question = 0;
    CONSOLE_SCREEN_BUFFER_INFO SBInfo; /* Somewhere to save screenbufferinfo, which includes the current position of the cursor */
    HANDLE hConsoleOut = GetStdHandle( STD_OUTPUT_HANDLE ); /* Something you have to have for some reason */
    COORD newPosition; /* windows.h struct that saves x and y position */

    if(msg_nr != prev_msg_nr){
        system("cls");
        count = msg_nr - 20 < 0 ? 0 : msg_nr - 20;
        for(i = count; i <= msg_nr - 1; i++){
            printf("%-3d[%-10s %-2d:%-2d:%-2d UTC] %s: %-40.40s\n", i, chat[i].date,
            chat[i].hour,
            chat[i].min,
            chat[i].sec,
            chat[i].username,
            chat[i].text); 
            if(strlen(chat[i].text) > 20){
                printf("%-60.60s\n", chat[i].text + 20);
            }
            if(strlen(chat[i].text) > 80){
                printf("%-60.60s\n", chat[i].text + 100);
            }
            printf("\n");
        }
        print_emote_counter(emoticon, msg_nr - 1, amountofemotes);

        GetConsoleScreenBufferInfo(hConsoleOut, &SBInfo);
        newPosition.X = 100;
        newPosition.Y = 10;
        SetConsoleCursorPosition(hConsoleOut, newPosition);

        printf("Questions:");
        first_question = question - 10 < 0 ? 0 : question - 10; 
        for(i = first_question; i < question; i++){
            newPosition.Y = 11 + j;
            SetConsoleCursorPosition(hConsoleOut, newPosition);
            printf("%-25s: %s", questions[i].username, questions[i].text);
            j++;
        }
        if(question_nr != prev_question_nr){
            question++;
        }
        prev_question_nr = question_nr;
        newPosition.X = SBInfo.dwCursorPosition.X;
        newPosition.Y = SBInfo.dwCursorPosition.Y; 
        SetConsoleCursorPosition(hConsoleOut, newPosition);
    }
    prev_msg_nr = msg_nr;
}

void print_emote_counter(emotelist *emoticon, int msg_nr, int amountofemotes){
    int i, j = 0;
    static int prev_msg_nr;

    CONSOLE_SCREEN_BUFFER_INFO SBInfo; /* Somewhere to save screenbufferinfo, which includes the current position of the cursor */
    HANDLE hConsoleOut = GetStdHandle( STD_OUTPUT_HANDLE ); /* Something you have to have for some reason */
    COORD newPosition; /* windows.h struct that saves x and y position */

    GetConsoleScreenBufferInfo(hConsoleOut, &SBInfo); /* Save the cursors current position */
    newPosition.X = 100;
    newPosition.Y = 0;
    SetConsoleCursorPosition(hConsoleOut, newPosition);

    printf("Emote Streaks:\n");

    for(i = 0; i < amountofemotes; i++){
        if(emoticon[i].emotecount > 1){ 
            printf("%-10s: %4d", emoticon[i].emote, emoticon[i].emotecount);
            newPosition.Y = 1 + j;
            SetConsoleCursorPosition(hConsoleOut, newPosition);
            j++;
        }           
    }
    
    prev_msg_nr = msg_nr;

    newPosition.X = SBInfo.dwCursorPosition.X;
    newPosition.Y = SBInfo.dwCursorPosition.Y; 
    SetConsoleCursorPosition(hConsoleOut, newPosition); 
}

void ctrl_f(twitchchat chat[], int offline_numberoflines){
    char searchstring[500];
    int matchfound = 0, i = 0, picknumber = 0;

    printf("INSTRUCTION: 1) Press a number. 2) Press space. 3) Enter keyword.\n");
    printf("Press (1) for username. (2) for text. Please enter your keyword: ");
    scanf("%d %s", &picknumber, &searchstring);

    if(picknumber == 1){
        for(i; i < offline_numberoflines; i++){
            if(strstr(chat[i].username, searchstring)){
                printf("[%s %d:%d:%d UTC] %s: %s\n", 
                    chat[i].date,
                    chat[i].hour,
                    chat[i].min,
                    chat[i].sec,
                    chat[i].username,
                    chat[i].text); 
                    matchfound = 1;
            }
        }
        if(matchfound == 0) {
                printf("No matches found.\n");
        }
    }
    else if(picknumber == 2){
        for(i; i < offline_numberoflines; i++){
            if(strstr(chat[i].text, searchstring)){
                printf("[%s %d:%d:%d UTC] %s: %s\n", 
                    chat[i].date,
                    chat[i].hour,
                    chat[i].min,
                    chat[i].sec,
                    chat[i].username,
                    chat[i].text); 
                    matchfound = 1;
            }
        }
        if(matchfound == 0) {
                printf("No matches found.\n");
        }
    }
    else{
        printf("Wrong input. Please try again.\n");
    }
}
