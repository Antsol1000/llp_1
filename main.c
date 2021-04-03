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

//FILE HANDLING
FILE* fp;
struct Section* head;
fp = fopen(filename, "r");
head = linked_list_creator(fp);

//READING THE ARGUMENTS - whole expression
int len_exp = strlen(argv[argc - 1]);
expression = malloc((len_exp+1)*sizeof(char));
strcpy(expression, argv[argc - 1]);
expression[len_exp] = '\0';

//tutaj to typowe szukañsko
if(argc == 3){

    struct Name_returner* r = malloc(sizeof(struct Name_returner*));
    r = getting_section_key_name(len_exp,expression, r);
    section1 = r->section;
    key1 = r->key;

    if( extended_validity_checker(section1, key1)== 1){
    val1 = klucznik(section1, key1,head);
    if(val1){printf("The value: %s\n", val1);}
    }
    //freeing
    free(key1); free(section1); free(r);
}
else if(argc == 4){//tutaj to specjalne
    struct Name_returner* r1 = malloc(sizeof(struct Name_returner*));
    struct Name_returner* r2 = malloc(sizeof(struct Name_returner*));
    char* expression1; char* expression2;
    char* operation; char* output;

    char* section2; char* key2;
    expression1 = strtok(expression, " ");
    operation = strtok(NULL, " ");
    expression2 = strtok(NULL, " ");

    r1 = getting_section_key_name(strlen(expression1),expression1, r1);
    section1 = r1->section;
    key1 = r1->key;

    r2 = getting_section_key_name(strlen(expression2),expression2, r2);
    section2 = r2->section;
    key2 = r2->key;

    output = operations(section1, key1, operation, section2, key2);
    printf("the outcome: %s\n", output);

    //freeing the memory
    free(key1); free(key2); free(section1); free(section2); free(r1); free(r2);

}

fclose(fp);
free(expression);
uroboros(head);

return 0;}


