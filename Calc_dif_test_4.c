#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define pist(a,b) (sqrt((a-0)*(a-0)+(0-b)*(0-b)))
#define max(a, b) ((a < b) ? b : a)
#define min(a, b) ((a < b) ? a : b)

/*
void select_20_lines(chatfile){
}
*/
/*void choose_2message(struct lines_20)*/



typedef struct data{
    char *strings;
}data;




int main (void){
    
struct data source, target;

        int ComputeLevenshteinDistance(data source, int source_length, data target, int target_length);
        double calc_similar(data source, int source_length, data target, int target_length);
    
    
  int source_length, target_length;
  double similarity;

    
    source.strings = "bugter";
    target.strings = "pugter";
        
    /*char data[1].source = *word; lines_20[i].messages;*/
    source_length = 7; /*strlen(lines_20[i].messages);*/

    /*char data[1].target = *data; lines_20[i+1].messages;*/
    target_length = 7; /*strlen(lines_20[i+1].messages);*/

    similarity = calc_similar(source, source_length, target, target_length);

    printf("\n\nMain Hello %d %s, %d %s Similarity: %f\n", source_length, source.strings, target_length, target.strings, similarity);



return 0;
}

int ComputeLevenshteinDistance(data source, int source_length, data target, int target_length){
     int a, b, c, ls, lt;
     char s[20];
     char t[20];
        
        ls=source_length;
        lt=target_length;
        strcpy(s, source.strings);
        strcpy(t, target.strings);
        /* if either string is empty, difference is inserting all chars 
           from the other
         */
        if (!source_length) return target_length;
        if (!target_length) return source_length;
 
        /* if last letters are the same, the difference is whatever is
           required to edit the rest of the strings
         */
        if (s[ls] == t[ls])
                return ComputeLevenshteinDistance(source, ls - 1, target, lt - 1);
 
        /* else try:
               changing last letter of s to that of t; or
               remove last letter of s; or
               remove last letter of t,
               any of which is 1 edit plus editing the rest of the strings
         */
        a = ComputeLevenshteinDistance(source, ls - 1, target, lt - 1);
        b = ComputeLevenshteinDistance(source, ls,     target, lt - 1);
        c = ComputeLevenshteinDistance(source, ls - 1, target, lt    );
 
        if (a > b) a = b;
        if (a > c) a = c;
 
        return a + 1;
}
 




double calc_similar(data source, int source_length, data target, int target_length){

printf("Hello 222 %d %s, %d %s\n", source_length, source.strings, target_length, target.strings);

if (strcmp(source.strings, target.strings)==0){   
return 1.0;
}

int stepsToSame = ComputeLevenshteinDistance(source, source_length, target, target_length);

printf("Steps to same: %d", stepsToSame); 

return (1.0 - ((double)stepsToSame / (double)max(source_length, target_length)));
    
}