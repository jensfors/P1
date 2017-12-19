#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

#define MAX 1000
#define STARTHOUR 0
#define STARTMIN 0
#define STARTSEC 0
#define MAX_LINE 600
#define STR_LEN 30
#define MAX_EMOTES 30
#define HIGHLIGHT_THRESHOLD 0
#define SEC_PR_MIN 60
#define SEC_PR_HOUR 3600

/* Struct til chatbeskeder */
typedef struct twitchchat{
    char date[STR_LEN];
    int hour;
    int min;
    int sec;
    char username[MAX];
    char text[MAX];
} twitchchat;

/* Struct til emotes */
typedef struct emotelist {
    char emote[MAX_EMOTES];
    int emotecount;
} emotelist;

/* Prototyper */
int get_twitch_chat(FILE *chatfile, twitchchat chat[],  emotelist *emotes, clock_t start_t, int amountofemotes);
void timer(int *hour, int *min, int *sec, clock_t start_t, clock_t end_t);
int count_line(FILE *chatfile);
void emote_counter(twitchchat chat[], emotelist *emotes, int i, int amountofemotes);
void emote_streak(twitchchat chat[], emotelist *emotes, int i, int amountofemotes);
int find_questions(twitchchat test[], int n, twitchchat questions[]);
void print_emote_counter(emotelist *emotes, int msg_nr, int amountofemotes, int input);
int auto_highlight(twitchchat chat[], emotelist emotes[], twitchchat highlights[], int msg_nr, int numberoflines, int amountofemotes);
void emote_to_file(FILE *emotefile, emotelist *emotedummy, int *amountofemotes);
void make_emote_struct(FILE *emotefile, emotelist *emotes, int *amountofemotes);
void print_struct(emotelist *emotes, int amountofemotes);
void emote_menu(emotelist *emotes, emotelist *emotedummy, emotelist *standard_emotes, int amount_std_emote);
void choose_emotes(emotelist *emotes, emotelist *emotedummy, int amountofemotes);
void print_it_all(twitchchat chat[], twitchchat questions[], emotelist *emotes, twitchchat highlights[], int msg_nr, int question_nr, int amountofemotes, int highlight_nr, int input);
int choose_file_name(char savefile[]);
void save_to_file(twitchchat savestruct[], char savefile[], int numberoflines);
void add_txt_to_filename(char savefile[]);
void chatfile_menu(char twitchchatfile[], int *loadchat);
void get_offline_chat(twitchchat offlinechat[], FILE *chatfile);
void ctrl_f(twitchchat chat[], int numberoflines);
void startup_default_emotes(emotelist standard_emotes[], int *amount_std_emote);
void print_emotes_from_file();
void print_questions(twitchchat questions[], int question_nr, int input);
void set_default_emotes(emotelist standard_emotes[], int amount_std_emote);
void print_highlights(twitchchat highlights[], int highlight_nr, int input);
void print_chat(twitchchat chat[], int msg_nr, int input);
void print_offline_questions(twitchchat offlinechat[], twitchchat questions[], int offline_numberoflines);
void print_offline_highlights(twitchchat offlinechat[], twitchchat highlights[], int offline_numberoflines, emotelist emotes[], int amountofemotes);

/* Main */
int main(void){
    twitchchat highlights[100];
    twitchchat *livechat, questions[MAX_LINE], *offlinechat;
    int numberoflines = 0, msg_nr, amountofemotes = 0, question_nr, offlineinput, maininput, mainmenu, offlinemenu, onlinemenu, 
        savechat = 0, loadchat = 1, offline = 0, online = 0, onlineinput, offline_numberoflines = 0, amount_std_emote, highlight_nr = 0, what_to_print = 0, done = 0;
    char savechatfile[STR_LEN], twitchchatfile[STR_LEN], std_chatfile[STR_LEN] = "twitchchat2.txt";
    emotelist *emotes = (emotelist*)malloc(MAX_EMOTES * sizeof(emotelist));
    emotelist *emotedummy = (emotelist*)malloc(MAX_EMOTES * sizeof(emotelist));
    emotelist *standard_emotes = (emotelist*)malloc(MAX_EMOTES * sizeof(emotelist));
    clock_t start_t;

    FILE *chatfile;
    FILE *emotefile;
    FILE *ifpemotefile;

    /* Indlæser standard emotes til emote structen */
    emotefile = fopen("emotetwitch.txt", "w");
    startup_default_emotes(standard_emotes, &amount_std_emote);
    set_default_emotes(standard_emotes, amount_std_emote);
    make_emote_struct(emotefile, emotes, &amount_std_emote);
    fclose(emotefile);

    do{
        /* Main menu */
        printf("Main menu \n(1) Live options \n(2) Offline options \n(3) Go live \nEnter: ");
        scanf("%d", &maininput);
        switch(maininput){
            case 1: online = 1; mainmenu = 0; system("cls"); break; 
            case 2: offline = 1; mainmenu = 0; system("cls"); break;
            case 3: mainmenu = 1; what_to_print = 1; break;
            default: printf("Unknown command, try again!\n"); mainmenu = 0; break;
        }

        /* Live instillinger */
        if(online == 1){
            do{
                printf("Live settings \n(1) Choose highlight emotes \n(2) Choose file to save chatlog to \n(3) Only show filtered chat \n(4) Only show questions" 
                       "\n(5) Only show highlights \n(6) Show filtered chat/questions/highlight \n(7) BACK TO MAIN MENU \nEnter: ");
                scanf("%d", &onlineinput);

                switch(onlineinput){
                    /* Case 1: Indskriv emotes til emotefil */
                    case 1: system("cls"); emote_menu(emotes, emotedummy, standard_emotes, amount_std_emote); onlinemenu = 0; break; 
                    /* Case 2: Gem chatloggen til en custom fil */
                    case 2: system("cls"); printf("Please type in the name of the .txt file the chatlog will be saved to"); 
                            savechat = choose_file_name(savechatfile);
                            printf("\nChat log will be saved to: %s\n", savechatfile); onlinemenu = 0; break;
                    /* Case 3: Viser kun filtreret chat */
                    case 3: printf("Showing filtered chat\n"); mainmenu = 1; onlinemenu= 1; what_to_print = 2; break;
                    /* Case 4: Viser kun spørgsmål */
                    case 4: printf("Showing questions\n"); mainmenu = 1; onlinemenu= 1; what_to_print = 3; break;
                    /* Case 5: Viser kun highlights */
                    case 5: printf("Showing highlights\n"); mainmenu = 1; onlinemenu = 1; what_to_print = 4; break;
                    /* Case 6: Viser filtreret chat/spørgsmål/highlight */
                    case 6: printf("Showing filtered chat/questions/highlight\n"); mainmenu = 1; onlinemenu = 1; what_to_print = 1; break;
                    /* Case 7: Gå tilbage til main menu */
                    case 7: system("cls"); onlinemenu = 1; online = 0; break;
                    default: printf("Unknown command, try again!\n"); onlinemenu = 0; break;
                }
            }
            while(onlinemenu == 0);
        }

        /* Offline indstillinger */
        if(offline == 1){
            /* Prompter brugeren til at vælge chatfil */
            chatfile_menu(twitchchatfile, &loadchat);
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
            offlinemenu = 0; 
            
            do{
                printf("Offline settings \n(1) Choose highlight emotes \n(2) Search chat \n(3) Show all questions"
                       "\n(4) Show highlight timestamps \n(5) Go through chatfile \n(6) BACK TO MAIN MENU \nEnter: ");
                scanf("%d", &offlineinput);

                switch(offlineinput){
                    /* Case 1: Indskriv emotes til emotefil */
                    case 1: system("cls"); emote_menu(emotes, emotedummy, standard_emotes, amount_std_emote); offlinemenu = 0; system("cls"); break; 
                    /* Case 2: Søg efter noget i chatten */
                    case 2: system("cls");
                            ctrl_f(offlinechat, offline_numberoflines);
                            offlinemenu = 0; printf("\n"); break;
                    /* Case 3: Vis alle spørgsmål fra filen */
                    case 3: system("cls"); printf("Showing questions\n"); offlinemenu = 0;
                            print_offline_questions(offlinechat, questions, offline_numberoflines); break;
                    /* Case 4: Vis alle steder hvor der var highlights */
                    case 4: system("cls"); printf("Showing highlight timestamps\n"); offlinemenu = 0; 
                            print_offline_highlights(offlinechat, highlights, offline_numberoflines, emotes, amountofemotes); break;
                    /* Case 5: Start læsningen af filen */
                    case 5: mainmenu = 1; offlinemenu = 1; start_t = clock(); system("cls"); break;
                    /* Case 6: Gå tilbage til main menu */
                    case 6: offlinemenu = 1; offline = 0; offlinemenu = 1; system("cls"); break;
                    default: system("cls"); printf("Unknown command, try again!\n"); offlinemenu = 0; break;
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
  
    /* Ser om der er en twitchchat i chatfilen */
    if(chatfile != NULL){
        
        /* Åbner filen med de valgte emotes */
        ifpemotefile = fopen("emotetwitch.txt", "r");
        make_emote_struct(ifpemotefile, emotes, &amountofemotes); 
        printf("Highlight Emotes:\n");
        print_struct(emotes, amountofemotes);
        printf("\n");

        numberoflines = count_line(chatfile);
        livechat = (twitchchat *)malloc(numberoflines * sizeof(twitchchat));
    
        start_t = clock();

        if(livechat == NULL){
            printf("Allocation problems. Bye.\n");
            exit(EXIT_FAILURE);
        }
        /* Går igennem hele chatten, en chatbesked af gangen */
        while(!done){
            msg_nr = get_twitch_chat(chatfile, livechat, emotes, start_t, amountofemotes);
            emote_streak(livechat, emotes, msg_nr - 1, amountofemotes);
            question_nr = find_questions(livechat, msg_nr - 1, questions);
            highlight_nr = auto_highlight(livechat, emotes, highlights, msg_nr - 1, numberoflines, amountofemotes);
            print_it_all(livechat, questions, emotes, highlights, msg_nr - 1, question_nr, amountofemotes, highlight_nr, what_to_print);
            if(kbhit()) done = 1; 
        }
        /* Gemmer chatlogen til den angivet fil ellers dato.txt */
        if(savechat == 1){
            save_to_file(livechat, savechatfile, numberoflines);
        }
        else if(savechat == 0){
            strcpy(savechatfile, livechat[0].date);
            add_txt_to_filename(savechatfile);        
            save_to_file(livechat, savechatfile, numberoflines);
        }

        fclose(chatfile);
        fclose(emotefile);
        free(livechat);
    }
    /* Chatfilen kan ikke åbnes */
    else{
        printf("Can't open the file %s\n", std_chatfile);
    }
   
    return 0;
}
/* Funktion der indlæser en chatbesked og ser om beskeden passer med den nuværende tid */
int get_twitch_chat(FILE *chatfile, twitchchat chat[], emotelist *emotes, clock_t start_t, int amountofemotes){
    static int i = 0, hour = 0, min = 0, sec = 0;
    char line[MAX_LINE], dummystr[MAX_LINE];
    static clock_t end_t;
    end_t = clock();
    timer(&hour, &min, &sec, start_t, end_t); 
    
    /* Indlæser en chatlinje med alt info */
    if(fgets(line, sizeof(line), chatfile) != NULL){
        sscanf(line, " [%s %d:%d:%d UTC] %[^:]: %500[^\n]",
               chat[i].date,
               &chat[i].hour,
               &chat[i].min,
               &chat[i].sec,
               chat[i].username,
               chat[i].text);
        sscanf(line, " %[^\n]", dummystr);

        /* Ser om tiden for beskeden passer med den nuværende tid og i blir talt op */
        if(chat[i].hour == hour && chat[i].min == min && chat[i].sec == sec){
            i++;
        }
        /* Hvis ikke beskedtiden passer med den nuværende tid, filpointeren rykkes en linje tilbage */
        else{
            fseek(chatfile, -(strlen(dummystr) + 2), SEEK_CUR); 
        }
    }
    return i;
}

/* Funktion hvor den nuværende tid i sekunder bliver omregnet til timer, min, sek */
void timer(int *hour, int *min, int *sec, clock_t start_t, clock_t end_t){
    static int i = 0;
    clock_t total_t;
    
    end_t = clock();
    total_t = (double)((end_t - start_t) / CLOCKS_PER_SEC) + STARTSEC + STARTMIN * SEC_PR_MIN + STARTHOUR * SEC_PR_MIN * SEC_PR_MIN;
    *sec = total_t % SEC_PR_MIN;
    *min = (total_t / SEC_PR_MIN) % SEC_PR_MIN;
    *hour = total_t / SEC_PR_HOUR;
    i++;
}

/* Funktion der tæller antallet af linjer i en chatfil*/
int count_line(FILE *chatfile){
    int i = 0;
    char line[MAX_LINE];
    while(fgets(line, sizeof(line), chatfile) != NULL){
        i++;
    }
    rewind(chatfile);
    return i;
}

/* Funktion der optæller emotestreaks for emotes i et givent tidsinterval */
void emote_streak(twitchchat chat[], emotelist *emotes, int i, int amountofemotes){
    /* secback = sekunder der bliver set tilbage */
    int j = 0, k, n = 0, secback = 10, startsec, totalsec, emotefound = 0,
        emotenumber, hour, min, sec, dummy, prev_msg_nr;
    
    /* Omregner timer, min, sek til sekunder -> går 10 sek tilbage -> omregnes tilbage igen */
    totalsec = (chat[i].hour * SEC_PR_HOUR) + (chat[i].min * SEC_PR_MIN) + (chat[i].sec);
    startsec = (totalsec - secback) < 0 ? 0 : (totalsec - secback);
    hour = startsec / SEC_PR_HOUR;
    min = (startsec / SEC_PR_MIN) % SEC_PR_MIN;
    sec = startsec % SEC_PR_MIN;

    if(i != prev_msg_nr){
        
        /* Optæller antallet af linjer op til 10 sek tilbage */
        for(j = 0; j < i; j++){
            if((chat[j].hour < hour) || (chat[j].min < min) || (chat[j].sec < sec)){
                n++;
            }
        }
        /* Gennemgår alle emotes og ser om de er skrevet i de 10 sek */
        for(emotenumber = 0; emotenumber < amountofemotes; emotenumber++){
            emotefound = 0;
            if(strstr(chat[i].text, emotes[emotenumber].emote)){
                for(k = n; k < i; k++){
                    if(strstr(chat[k].text, emotes[emotenumber].emote)){
                        emotefound = 1;
                    }
                }
                /* Hvis emoten ikke findes bliver den nulstillet */
                if(!emotefound){
                    emotes[emotenumber].emotecount = 1;
                }
                /* Emoten bliver optællet, hvis den er fundet*/
                else{
                    dummy = emotes[emotenumber].emotecount;
                    emotes[emotenumber].emotecount = dummy + 1;
                }
            }
        }
    }
    prev_msg_nr = i;

}

/* Funktion der finder spørgsmål i chatten */
int find_questions(twitchchat test[], int n, twitchchat questions[]){
    static int j = 0;
    char curr[MAX];
    static char prev[MAX];
    strcpy(curr, test[n].text);
    if(!(prev == NULL || strcmp(curr, prev) == 0)){
        if(strstr(curr, "????")){
            return j;
        }
        else if (strcmp(curr, "?") == 0){
            return j;
        }
        else if (strcmp(curr, "??") == 0){
            return j;
        }
        else if (strcmp(curr, "???") == 0){
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

/* Funktion der vurderer om der er et highlight udfra mængden af chatbeskeder og emotes */
int auto_highlight(twitchchat chat[], emotelist emotes[], twitchchat highlights[], int msg_nr, int numberoflines, int amountofemotes){
    int i, j = 0, k, totalsec, startsec, emotecounter = 0, res = 0, time_output, offset = 10;
    static int prev_startsec = 0, prev_emotecounter = 0, prev_messages = 0, highlight_counter = 0;

    printf("cnacer\n");

    /* Går 10 sek tilbage og gennemgår alle beskeder derfra til nu */
    startsec = chat[msg_nr].hour * SEC_PR_HOUR + chat[msg_nr].min * SEC_PR_MIN + chat[msg_nr].sec;
    if(startsec >= prev_startsec + offset){
        totalsec = startsec - offset < 0 ? 0 : startsec - offset;
        i = msg_nr;
        j = 0;
        while(startsec > totalsec){
            /* Emote optælleren tælles op hvis der er et match */
            for(k = 0; k < amountofemotes; k++){
                if(strstr(chat[i].text, emotes[k].emote)){
                    emotecounter++;
                }
            }
            j++;
            i--;
            startsec = chat[i].hour * SEC_PR_HOUR + chat[i].min * SEC_PR_MIN + chat[i].sec;
        }
        /* Vurdere om der er et highlight udfra antallet af beskeder/emotes fra de forrige 10 sek til nu */
        if(emotecounter > prev_emotecounter * HIGHLIGHT_THRESHOLD && j > prev_messages * HIGHLIGHT_THRESHOLD){
            res = 1;
            time_output = prev_startsec; 
        }
        prev_startsec = chat[msg_nr].hour * SEC_PR_HOUR + chat[msg_nr].min * SEC_PR_MIN + chat[msg_nr].sec;
        prev_emotecounter = emotecounter;
        prev_messages = j;
    }
    /* Hvis der er et highlight bliver tidspunktet gemt i structen 'highlights' */
    if(res){
        highlights[highlight_counter].sec = time_output % SEC_PR_MIN;
        highlights[highlight_counter].min = (time_output / SEC_PR_MIN) % SEC_PR_MIN;
        highlights[highlight_counter].hour = time_output / SEC_PR_HOUR;
        highlight_counter++;
    }
    return highlight_counter;
}

/* Funktion der er menu til de forskellige indstiller for emotes */
void emote_menu(emotelist *emotes, emotelist *emotedummy, emotelist *standard_emotes, int amount_std_emote){
    int amountofemotes = 0, input, emotemenu, i = 0;

    FILE *dummyfile;
    system("cls");

    do{
        printf("(1) Delete all emotes and add new ones \n(2) Add extra emotes \n(3) Set emotes to standard emotes" 
               "\n(4) Show standard emotes \n(5) Show current emotes \n(6) BACK \nEnter: ");
        scanf("%d", &input); 

        switch(input){
            /* Case 1: Slet forrige highlight emotes og/eller tilføje nye */
            case 1: system("cls"); choose_emotes(emotes, emotedummy, amountofemotes); 
                    emotemenu = 1; break;
            /* Case 2: Tilføj ekstra emotes uden at slette de forrige */
            case 2: system("cls");
                    dummyfile = fopen("emotetwitch.txt", "r"); 
                    make_emote_struct(dummyfile, emotedummy, &amountofemotes);
                    choose_emotes(emotes, emotedummy, amountofemotes);
                    emotemenu = 1; break;
            /* Case 3: Gør alle emotes til standardemotes */
            case 3: system("cls");
                    printf("Setting emotes to default\n");
                    set_default_emotes(standard_emotes, amount_std_emote);
                    emotemenu = 1; break;
            /* Case 4: Udskriver hvilke emotes der er standard emotes*/
            case 4: system("cls");
                    printf("Standard emotes are:\n");
                    for(i = 0; i < 5; i++){
                        printf("%s\n", standard_emotes[i].emote);
                    }
                    printf("\n"); emotemenu = 1; break;
            /* Case 5: Udskriver de nuværende valgte emotes */
            case 5: system("cls");
                    printf("Showing current emotes:\n"); 
                    print_emotes_from_file(); emotemenu = 1; break;
            /* Case 6: Gå tilbage til forrige menu */     
            case 6: system("cls"); emotemenu = 0; break;
            default: system("cls"); printf("Unknown command, try again!\n"); emotemenu = 1; 
        }
    }
    while(emotemenu != 0);
}

/* Funktion der gør emotes til standard emotes */
void set_default_emotes(emotelist standard_emotes[], int amount_std_emote){
    int i;

    FILE *emotefile;
    emotefile = fopen("emotetwitch.txt", "w");

    /* Udskriver i emotetwitch.txt alle standard emotes*/
    for(i = 0; i < amount_std_emote; i++){
        fprintf(emotefile, "%s\n", standard_emotes[i].emote);
    }
    fclose(emotefile);
}

/* Funktion hvor du kan inskrive emotes til emotetwitch.txt og indlæser dem i en struct */
void choose_emotes(emotelist *emotes, emotelist *emotedummy, int amountofemotes){
    FILE *ifpemotefile;
    FILE *ofpemotefile;
    
    ofpemotefile = fopen("emotetwitch.txt", "w"); 
    emote_to_file(ofpemotefile, emotedummy, &amountofemotes); 
    fclose(ofpemotefile);
    ifpemotefile = fopen("emotetwitch.txt", "r");
    make_emote_struct(ifpemotefile, emotes, &amountofemotes); 
}

/* Funktion hvor brugeren kan skrive higlight emotes/ord */
void emote_to_file(FILE *emotefile, emotelist *emotedummy, int *amountofemotes){
    int i;
    char emotename[MAX_EMOTES];
    for(i = 0; i < *amountofemotes; i++){
        fprintf(emotefile, "%s\n", emotedummy[i].emote);
    }
    /* Brugeren kan indtaste emotes*/
    do{
        printf("\nType emotename or (0) to exit): ");
        scanf("%s", emotename);
        if(strcmp(emotename, "0") != 0){
            /* Skriver emoten/teksten til filen */
            fprintf(emotefile, "%s\n", emotename);
        }
    }
    /* Kører indtil brugeren skriver EXIT */
    while(strcmp(emotename, "0") != 0);
}

/* Funktion der indlæser emotes fra emotefilen til en emotelist struct */
void make_emote_struct(FILE *emotefile, emotelist *emotes, int *amountofemotes){
    int k = 0;
    char line[20];
    /* Indlæser linjer indtil der ikke er flere emotes */
    while(fgets(line, sizeof(line), emotefile) != NULL){
        sscanf(line, " %20[^\n]", emotes[k].emote);
        emotes[k].emotecount = 0;
        k++;
    }
    *amountofemotes = k;
}

/* Funktion der kan udskrive de valgte emotes til higlight */
void print_struct(emotelist *emotes, int amountofemotes){
    int i;
    for(i = 0; i < amountofemotes; i++){
        printf("%s\n", emotes[i].emote);
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

    printf("Choose what chatfile to load the chat from \n(1) Current streamfile \n(2) Choose custom file \nEnter: ");
    scanf("%d", &input);

    /* Vælg mellem 'live' filen eller indskriv navnet på en fil til indlæsning  */
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

/* Funktion der tilføjer .txt på filnavnet */
void add_txt_to_filename(char savefile[]){
    int i = 0, j = 0;
    char txt[5] = ".txt";

    for(i = 0; savefile[i] != '\0'; i++);
    for(j = 0; txt[j] != '\0'; ++j, ++i){
        savefile[i] = txt[j];
    }
    savefile[i] = '\0';
}

/* Funktion der udskriver chatten, emote streaks, highlight og spørgsmål */
void print_it_all(twitchchat chat[], twitchchat questions[], emotelist *emotes, twitchchat highlights[], int msg_nr, int question_nr, int amountofemotes, int highlight_nr, int input){
    static int prev_msg_nr;

    if(msg_nr != prev_msg_nr){
        system("cls");
        print_chat(chat, msg_nr, input);
        print_emote_counter(emotes, msg_nr, amountofemotes, input);
        print_questions(questions, question_nr, input);
        print_highlights(highlights, highlight_nr, input);
    }
    prev_msg_nr = msg_nr;
}

/* Funktion der udskriver emote streaks */
void print_emote_counter(emotelist *emotes, int msg_nr, int amountofemotes, int input){
    int i, j = 0;
    CONSOLE_SCREEN_BUFFER_INFO SBInfo; /* Somewhere to save screenbufferinfo, which includes the current position of the cursor */
    HANDLE hConsoleOut = GetStdHandle( STD_OUTPUT_HANDLE ); 
    COORD newPosition; /* windows.h struct that saves x and y position */

    if(input == 1 || input == 2){
        GetConsoleScreenBufferInfo(hConsoleOut, &SBInfo); /* Save the cursors current position */
        newPosition.X = 100;
        newPosition.Y = 0;
        SetConsoleCursorPosition(hConsoleOut, newPosition);
    
        printf("Emote Streaks:\n");

        for(i = 0; i < amountofemotes; i++){
            if(emotes[i].emotecount > 1){ 
                newPosition.Y = 1 + j;
                SetConsoleCursorPosition(hConsoleOut, newPosition);
                printf("%-10s: %4d", emotes[i].emote, emotes[i].emotecount);
                j++;
            }           
        }

        newPosition.X = SBInfo.dwCursorPosition.X;
        newPosition.Y = SBInfo.dwCursorPosition.Y; 
        SetConsoleCursorPosition(hConsoleOut, newPosition); 
    }
}

/* Funktion der udskriver spørgsmål */
void print_questions(twitchchat questions[], int question_nr, int input){
    int i, j = 0, first_question;
    static int prev_question_nr = 0;

    CONSOLE_SCREEN_BUFFER_INFO SBInfo; /* Somewhere to save screenbufferinfo, which includes the current position of the cursor */
    HANDLE hConsoleOut = GetStdHandle( STD_OUTPUT_HANDLE ); 
    COORD newPosition; /* windows.h struct that saves x and y position */

    if(input == 1){
        GetConsoleScreenBufferInfo(hConsoleOut, &SBInfo);
        newPosition.X = 100;
        newPosition.Y = 10;
        SetConsoleCursorPosition(hConsoleOut, newPosition);
    }

    if(input == 3 || input == 1){
        printf("Questions:\n");
        first_question = question_nr - 15 < 0 ? 0 : question_nr - 15; 
        for(i = first_question; i < question_nr; i++){
            if(input == 1){
                newPosition.Y = 11 + j;
                SetConsoleCursorPosition(hConsoleOut, newPosition);
            }
            printf("[%-2d:%-2d:%-2d] %-25s: %s\n", questions[i].hour, questions[i].min, questions[i].sec, questions[i].username, questions[i].text);
            j++;
        }
    }
    if(input == 1){
        prev_question_nr = question_nr;
        newPosition.X = SBInfo.dwCursorPosition.X;
        newPosition.Y = SBInfo.dwCursorPosition.Y; 
        SetConsoleCursorPosition(hConsoleOut, newPosition);
    }
}

/* Funktion der udskriver highlight tider */
void print_highlights(twitchchat highlights[], int highlight_nr, int input){
    static int prev_highlight_nr, highlight;
    int i, j = 0, first_highlight;
    CONSOLE_SCREEN_BUFFER_INFO SBInfo; /* Somewhere to save screenbufferinfo, which includes the current position of the cursor */
    HANDLE hConsoleOut = GetStdHandle( STD_OUTPUT_HANDLE ); 
    COORD newPosition; /* windows.h struct that saves x and y position */

    if(input == 1){
        GetConsoleScreenBufferInfo(hConsoleOut, &SBInfo);
        newPosition.X = 100;
        newPosition.Y = 40;
        SetConsoleCursorPosition(hConsoleOut, newPosition);
    }
    
    if(input == 4 || input == 1){
        printf("Highlights:\n");
        first_highlight = highlight_nr - 20 < 0 ? 0 : highlight_nr - 20; 
        for(i = first_highlight; i < highlight_nr; i++){
            if(input == 1){
                newPosition.Y = 41 + j;
                SetConsoleCursorPosition(hConsoleOut, newPosition);
            }
            printf("Timestamp: [%-2d:%-2d:%-2d]\n", highlights[i].hour, highlights[i].min, highlights[i].sec);
            j++;
        }
        if(highlight_nr != prev_highlight_nr){
            highlight++;
        }
        prev_highlight_nr = highlight_nr;
    }

    if(input == 1){
        newPosition.X = SBInfo.dwCursorPosition.X;
        newPosition.Y = SBInfo.dwCursorPosition.Y; 
        SetConsoleCursorPosition(hConsoleOut, newPosition);
    }
}

/* Funktionen der udskriver chatbeskeder */
void print_chat(twitchchat chat[], int msg_nr, int input){
    int count, i;

    if(input == 1 || input == 2){
        count = msg_nr - 20 < 0 ? 0 : msg_nr - 20;
        for(i = count; i <= msg_nr ; i++){
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
    }
}

/* Funktion der gør det muligt at søge efter tekst/brugere i en chatfil */
void ctrl_f(twitchchat chat[], int offline_numberoflines){
    char searchstring[500];
    int matchfound = 0, i = 0, picknumber = 0;

    printf("INSTRUCTION: 1) Press a number. 2) Press space. 3) Enter keyword.\n");
    printf("Press (1) for username. (2) for text. Please enter your keyword: ");
    scanf("%d %s", &picknumber, &searchstring);

    /* Søger igennem hele chatfilen for det valgte brugernavn */
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
    /* Søger igennem hele chatfilen for den valgte tekst */
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

/* Funktion der gør alle emotes standard ved start af program */
void startup_default_emotes(emotelist standard_emotes[], int *amount_std_emote){
    int i = 0;
    char line[MAX_EMOTES];

    FILE *standard_emotefile;

    /* Skriver standard emotes til en emotelist struct */
    standard_emotefile = fopen("standard_emotes.txt", "r");
    while(fgets(line, sizeof(line), standard_emotefile) != NULL){
        sscanf(line, "%s", standard_emotes[i].emote);
        i++;
    }
    *amount_std_emote = i;
}

/* Funktion der kan indlæse en hel chatfil til en struct */
void get_offline_chat(twitchchat offlinechat[], FILE *chatfile){
    char line[MAX_LINE];
    int i = 0;

    /* Gennemgår hele filen til der ikke er mere tekst*/
    while(fgets(line, sizeof(line), chatfile) != NULL){
        sscanf(line, " [%s %d:%d:%d UTC] %[^:]: %600[^\n]",
               offlinechat[i].date,
               &offlinechat[i].hour,
               &offlinechat[i].min,
               &offlinechat[i].sec,
               offlinechat[i].username,
               offlinechat[i].text);
        i++; 
    }
}

/* Funktion der kan udskrive emotes fra filen til skærmen */
void print_emotes_from_file(){
    char line[MAX_EMOTES];
    FILE *emote;
    emote = fopen("emotetwitch.txt", "r");

    while(fgets(line, sizeof(line), emote) != NULL){
        sscanf("%s", line);
        printf("%s", line);
    }
    printf("\n");
}

void print_offline_questions(twitchchat offlinechat[], twitchchat questions[], int offline_numberoflines){
    int i, question_nr, prev_question_nr = 0;

    for(i = 0; i < offline_numberoflines; i++){
        question_nr = find_questions(offlinechat, i, questions);
        if(question_nr != prev_question_nr){
            printf("[%-2d:%-2d:%-2d] %-25s: %s\n", questions[question_nr - 1].hour, questions[question_nr - 1].min, questions[question_nr - 1].sec, questions[question_nr - 1].username, questions[question_nr - 1].text);
        }
        prev_question_nr = question_nr; 
    }
}

/*void print_offline_highlights(twitchchat offlinechat[], twitchchat highlights[], int offline_numberoflines, emotelist emotes[], int amountofemotes){
    int i, highlight_nr, prev_highlight_nr = 0;

    for(i = 0; i < offline_numberoflines; i++){
        highlight_nr = auto_highlight(offlinechat, emotes, highlights, i, offline_numberoflines, amountofemotes);
        if(highlight_nr != prev_highlight_nr){
            printf("Timestamp: [%-2d:%-2d:%-2d]\n", highlights[highlight_nr - 1].hour, highlights[highlight_nr - 1].min, highlights[highlight_nr - 1].sec);
        }
        prev_highlight_nr = highlight_nr;
    }
}*/
