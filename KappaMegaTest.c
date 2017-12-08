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
    int emotenumber;
} emotelist;

int get_twitch_chat(FILE *ifp, twitchchat test[],  emotelist emotes[], clock_t start_t);
void timer(int *hour, int *min, int *sec, clock_t start_t, clock_t end_t);
int count_line(FILE *ifp);
int emote_counter(twitchchat test[], emotelist emotes[], int i, int j);

int main(void){
    /*twitchchat test[];*/
    twitchchat *pointertest;
    emotelist *emotes;
    int numberoflines = 0, msg_nr;
    const clock_t start_t = clock();

    FILE *ifp;
    ifp = fopen("twitchchat2.txt", "r");

    if(ifp != NULL){
        numberoflines = count_line(ifp);
        pointertest = (twitchchat *)malloc(numberoflines * sizeof(twitchchat));
        emotes = (emotelist *)malloc(20 * sizeof(emotelist));

        if (pointertest == NULL){
            printf("Allocation problems. Bye.\n");
            exit(EXIT_FAILURE);
        }
        emotes[2].emotenumber = 0;
        while(1){
            msg_nr = get_twitch_chat(ifp, pointertest, emotes, start_t);
        }
        fclose(ifp);
        printf("\n%d\n", numberoflines);

        fclose(ifp);
        free(pointertest);
    }
    else{
        printf("Can't open the file%s\n", "twitchchat.txt");
    }

    return 0;
}

int get_twitch_chat(FILE *ifp, twitchchat test[], emotelist emotes[], clock_t start_t){
    static int i = 0, hour = 0, min = 0, sec = 0, j = 2;
    char line[500],
        dummystr[500];
    static clock_t end_t;

    end_t = clock();
    timer(&hour, &min, &sec, start_t, end_t);
    Sleep(20);
    /* printf("\n%d:%d:%d  ", hour, min, sec); */
    if(fgets(line, sizeof(line), ifp) != NULL){
        sscanf(line, " [%s %d:%d:%d UTC] %[^:]: %500[^\n]",
               test[i].date,
               &test[i].hour,
               &test[i].min,
               &test[i].sec,
               test[i].username,
               test[i].text);
        sscanf(line, " %[^\n]", dummystr);
        /*printf("HELLO: %d %d %d\n", test[i].hour, test[i].min, test[i].sec);*/

        if(test[i].hour == hour && test[i].min == min && test[i].sec == sec){
            emote_counter(test, emotes, i, j);
            printf("%-8d", i);
            printf("%s: %-4d  ", emotes[j].emote, emotes[j].emotenumber);
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
        /*printf("%d\n", strlen(dummystr));
        printf("%ld\n", ftell(ifp));*/
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

int count_line(FILE *ifp){
    int i = 0;
    char line[400];
    while(fgets(line, sizeof(line), ifp) != NULL){
        i++;
    }
    rewind(ifp);
    return i;
}


int emote_counter(twitchchat test[], emotelist emotes[], int i, int j){
  int isEmote = 0, k = 0;
  char line[20];
  FILE *meme;
  meme = fopen("emotes.txt", "r");

  while(fgets(line, sizeof(line), meme) != NULL){
        sscanf(line, " %20[^\n]", emotes[k].emote);
        k++;
  }

  if(strstr(test[i].text, emotes[j].emote)){  
      emotes[j].emotenumber += 1;
  }
  fclose(meme);
  return isEmote;
}

















