#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include "header.h"


int main( int argc, char **argv)  {
    printf("f\n");
    char* filename;
    filename = malloc((strlen(argv[1])+1)*(sizeof(char)));
    char* expression;
    char* section1;
    char* key1;
    char* val1;
    strcpy(filename,argv[1]);

//FILE HANDLING
    FILE* fp;
    struct Section* head;
    fp = fopen(filename, "r");
    printf("h\n");
    head = linked_list_creator(fp);

//READING THE ARGUMENTS - whole expression
    int len_exp = strlen(argv[argc - 1]);
    expression = malloc((len_exp+1)*sizeof(char));
    strcpy(expression, argv[argc - 1]);
    expression[len_exp] = '\0';

//tutaj to typowe szukañsko
    if(argc == 3) {

        struct Name_returner* r;
        r = malloc(sizeof(struct Name_returner));
        r->key = NULL;
        r->section = NULL;
        getting_section_key_name(strlen(expression),expression, r);
        section1 = r->section;
        key1 = r->key;

        val1 = klucznik(section1, key1, head);
        if(val1) {
            printf("The value: %s\n", val1);
        }
        //freeing
        free(key1);
        free(section1);
        free(r);
    } else if(argc == 4) { //tutaj to specjalne
        struct Name_returner* r1 = malloc(sizeof(struct Name_returner));
        struct Name_returner* r2 = malloc(sizeof(struct Name_returner));
        char* expression1;
        char* expression2;
        char* operation;

        char* section2;
        char* key2;
        expression1 = strtok(expression, " ");
        operation = strtok(NULL, " ");
        expression2 = strtok(NULL, " ");

        getting_section_key_name(strlen(expression1),expression1, r1);
        section1 = r1->section;
        key1 = r1->key;

        getting_section_key_name(strlen(expression2),expression2, r2);
        section2 = r2->section;
        key2 = r2->key;

        operations(head, section1, key1, operation, section2, key2);

        //freeing the memory
        free(key1);
        free(key2);
        free(section1);
        free(section2);
        free(r1);
        free(r2);

    }
    printf("\n Checking validity of identifiers . . . \n");
    inspector(head);
    fclose(fp);
    free(expression);
    uroboros(head);
    free(filename);
    return 0;
}
