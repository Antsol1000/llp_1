#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include "header.h"

int main( int argc, char *argv[] )  {
char* filename;
char* expression;
char* section1;char* key1;
char* val1;

filename = argv[1];

if(argc == 3){
    int len_exp = strlen(argv[2]);
    expression = malloc((len_exp+1)*sizeof(char));
    strcpy(expression, argv[2]);
    expression[len_exp] = '\0';

    int len_of_section = dot_filnder(expression);
    int len_of_key = len_exp-len_of_section-1;

    section1 = malloc((len_of_section +1)*sizeof(char));
    key1 = malloc((len_of_key +1)*sizeof(char));
    strncpy(section1, expression,len_of_section);
    strncpy(key1, expression+ len_of_section + 1,len_of_key);
    key1[len_of_key] = '\0';
    section1[len_of_section] = '\0';
}
//status - 0= no entry ever; 1- inside a section;2 - between sections

//FILE HANDLING
FILE* fp;
struct Section* head;
fp = fopen(filename, "r");
head = linked_list_creator(fp, head);

//SEARCHING
val1 = klucznik(section1, key1,head);
printf("THE VALUE: %s\n", val1);
//CLOSING
fclose(fp);
uroboros(head);


return 0;}


