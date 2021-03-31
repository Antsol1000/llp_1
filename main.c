#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include "header.h"


int main(int argc, char **argv){

///GETTING THE ARGS
char* filename = argv[0];
char* expression = argv[1];
char* section1;
char* section2;
char* key1;
char* key2;
int operation;

//status - 0= no entry ever; 1- inside a section;2 - between sections


//FILE HANDLING
FILE* fp;
struct Section* head;
fp = fopen("3.txt", "r");
head = linked_list_creator(fp, head);

fclose(fp);
uroboros(head);
return 0;}


