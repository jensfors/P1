#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#define MAX 5000
#define MIN_SIMIL 0.45

typedef struct cmp{
    char word[100];
    int word_nr;
} cmp;

typedef struct twitchchat{
    char date[10];
    int hour;
    int min;
    int sec;
    char username[100];
    char text[500];
    int combo_count;
    int print;
    int ref;
    int msg_nr;
} twitchchat;

/*Beregning af .ref, .combo og .print*/
void select_source_target(char *line1, char *line2, char *ssource, char *ttarget);
void string_to_words(cmp *source, cmp *target, char *ssource, char *ttarget, int *source_word_count, int *target_word_count);
int ComputeLevenshteinDistance(char *source, int source_length, char *target, int target_length);
int cmp_word(const void *v1, const void *v2);
double avg_lev(int word_lev, int source_word_count);
double calc_similar(double avg_lev, int target_word_count);

/*Indlæsning af chat*/
void get_twitch_chat(FILE *chatfile, twitchchat chat[]);
int count_line(FILE *chatfile);


int main(void){              
    twitchchat *chat_line;
    int numberoflines = 0, k = 0;
  
    FILE *chatfile;
    chatfile = fopen("twitchchat2.txt", "r");
    
    if(chatfile != NULL){
        numberoflines = count_line(chatfile);
        chat_line = (twitchchat *)malloc(numberoflines * sizeof(twitchchat));
        
        while(k<20){
            get_twitch_chat(chatfile, chat_line);
            printf("[%d:%d:%d UTC] %s: Msg nr: %d, Ref: %d, Combo: %d, Print: %d\n", 
                   chat_line[k].hour,
                   chat_line[k].min,
                   chat_line[k].sec,
                   chat_line[k].username,
                   chat_line[k].msg_nr,
                   chat_line[k].ref,
                   chat_line[k].combo_count,
                   chat_line[k].print);  
            k++;
        }
        fclose(chatfile);
        free(chat_line);
                    
    }   
    else{
        printf("Can't open the file%s\n", "twitchchat.txt");
    }  
    return 0;
}

void get_twitch_chat(FILE *chatfile, twitchchat chat[]){
    static int i = 0;
    int source_word_count=0, target_word_count=0, word_lev=0, k, j;
    double similar = 0;
        
    char line[600], dummystr[500];
    char *ssource = malloc(2000), *ttarget = malloc(2000);
    cmp *source = malloc(5000), *target = malloc(5000);    
    if(fgets(line, sizeof(line), chatfile) != NULL){
        sscanf(line, " [%s %d:%d:%d UTC] %[^:]: %500[^\n]",
               chat[i].date,
               &chat[i].hour,
               &chat[i].min,
               &chat[i].sec,
               chat[i].username,
               chat[i].text);
        //sscanf(line, " %[^\n]", dummystr); 
        chat[0].msg_nr = 0;
        chat[i].msg_nr = (chat[i-1].msg_nr + 1);
        chat[i].ref = chat[i].msg_nr;
        chat[i].print = 1;
        
        /*Assigner chat[i].text, chat[i-1].text til ssource, ttarget, baseret på størrelsen af strlen*/
        if(chat[i].text == NULL){
            select_source_target(chat[i].text, chat[i-1].text, ssource, ttarget);
            string_to_words(source, target, ssource, ttarget, &source_word_count, &target_word_count);
        
            /* Beregner Levensthtein, hvis de x første bogstaver i ordenen er ens */
            for(k=0; k < source_word_count; k++){
                for(j = 0; j <= target_word_count; j++){
                    if(strncmp(source[k].word, target[j].word, 2) == 0){
                        word_lev += ComputeLevenshteinDistance(source[k].word, strlen(source[k].word), target[j].word, strlen(target[j].word));
                        printf("wordlev %d\n", word_lev);
                    }
                }
                
            }
        
            /*Når for(k)løkken når til sidste ord i source, beregnes beskedernes similar*/
            if(k == source_word_count){
                similar = calc_similar(avg_lev(word_lev, source_word_count), target_word_count);
                printf("similar %f\n", similar);
            }   
            if(similar >= MIN_SIMIL){
                chat[i].ref = chat[i-1].ref;
                chat[i].print = 0;
                chat[i-1].combo_count += 1;
            }
            source_word_count = 0;
            target_word_count = 0;
            word_lev = 0;
            similar = 0;
           
        }  
    } 
    else{
        fseek(chatfile, -(strlen(dummystr) + 2), SEEK_CUR); /* seek backward from the current pos */
    }
    i++;        
}

int count_line(FILE *chatfile){
    int i = 0;
    char line[500];
    while(fgets(line, sizeof(line), chatfile) != NULL){
        i++;
    }
    rewind(chatfile);
    return i;
}

void select_source_target(char *line1, char *line2, char *ssource, char *ttarget){
    int len1=0, len2=0; 
    len1 = strlen(line1);
    len2 = strlen(line2);
    /*printf("line1: %s\nline2: %s\n\n", line1, line2);*/

    if((len1) && (len2)){   
        if(len1 > len2){
            strcpy(ttarget,line1);
            strcpy(ssource,line2);
        }
        else{
            strcpy(ssource,line1);
            strcpy(ttarget,line2);
        }
    }
}


/*Tager de to chatbeskeder, og splitter dem op ord i hvert deres array*/
void string_to_words(cmp *source, cmp *target, char *ssource, char *ttarget, int *source_word_count, int *target_word_count){ 
    char *source_tok, *target_tok;
    int k=0, j=0;

    /*printf("Ssource: %s\n", ssource);
    printf("Ttarget: %s\n", ttarget);*/
  
    source_tok = strtok(ssource, " ");
    while(source_tok != NULL){
        strcpy(source[k].word, source_tok);
        *source_word_count +=1;
        /*printf("Source: %s\n", source[k].word);*/
        source_tok = strtok(NULL, " ");
        k++;
        /*qsort(source[k].word, *source_word_count, sizeof(cmp), cmp_word);*/
    }
        
    target_tok = strtok(ttarget, " ");
    while(target_tok !=NULL){
        strcpy(target[j].word, target_tok);
        *target_word_count +=1;
        /*printf("Target: %s\n", target[j].word);*/
        target_tok = strtok(NULL, " ");
        j++;
        /*qsort(target[j].word, *target_word_count, sizeof(cmp), cmp_word);*/
    }

}    

int cmp_word(const void *v1, const void *v2){
    const cmp *p1 = (cmp *)v1;
    const cmp *p2 = (cmp *)v2;

    return strncmp(p1->word,p2->word, 2);    
}

int ComputeLevenshteinDistance(char *source, int source_length, char *target, int target_length){
    int a, b, c, ls, lt;
    ls=source_length;
    lt=target_length;
        
        /* if either string is empty, difference is inserting all chars 
           from the other
         */
    if (!source_length) return target_length;
    if (!target_length) return source_length;
 
        /* if last letters are the same, the difference is whatever is
           required to edit the rest of the strings
         */
    if (source[ls] == target[ls])
        return ComputeLevenshteinDistance(source, ls - 1, target, lt - 1);
 
        /* else try:
               changing last letter of s to that of t; or
               remove last letter of s; or
               remove last letter of t,
               any of which is 1 edit plus editing the rest of the strings
         */
    a = ComputeLevenshteinDistance(source, ls - 1, target, lt - 1);
    b = ComputeLevenshteinDistance(source, ls, target, lt - 1);
    c = ComputeLevenshteinDistance(source, ls - 1, target, lt);
 
    if (a > b) a = b;
    if (a > c) a = c;
 
    return a + 1;
}    
    
double avg_lev(int word_lev, int source_word_count){
    return (word_lev/source_word_count);
}    

double calc_similar(double avg_lev, int target_word_count){
    return (1.0 - ((double)avg_lev / (double)target_word_count));
}


