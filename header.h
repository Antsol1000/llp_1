#ifndef _HEADER_H_
#define _HEADER_H_
#include <stdbool.h>

// FUNCTIONAL STRUCTURES

struct Entry {
    char* key;
    char* value;
    struct Entry* next;
};

struct Section {
    char* name;
    struct Entry* first;
    struct Section* next;
};

struct Returner {
    struct Entry* entry;
    struct Section* section;
};

struct Name_returner {
    char* section;
    char* key;
};

// HELPER FUNCTIONS

void endstartend(int* arr, char* string) {
    int i = 0;
    while(string[i] != ' ') {
        i++;
    }
    arr[0] = i-1;
    i=i+2;
    arr[1] = i;
    while(string[i] != '\0') {
        i++;
    }
    arr[2] = i-1;
}

int dot_finder(char* my_string) {
    int i = 1;
    while(my_string[i]!='.') {
        i++;
    }
    return i;
}


int isvalid(char* str, int len) {
    //printf("The string %s, len %d", str, len);
    int i = 0;
    while(((isalnum(str[i])!=0) || str[i] == '-') && i<(len)) {
        i++;
    }
    if(i == (len)) {
        return 1;
    } else {
        return 0;
    }
}
int is_a_num(char* str) {
    int i = 0;
    int len = strlen(str);

    while(('0'<=str[i] && str[i]<= '9') && i<len) {
        i++;
    }
    if(len == i) {
        return 1;
    }
    return 0;
}

int op_check(char* str) {
    if(str[0] == '+') {
        return 1;
    } else if(str[0] == '-') {
        return 2;
    } else if(str[0] == '*') {
        return 3;
    } else {
        return 4;
    }
}
int extended_validity_checker(char* section1, char* key1) {
    int sv = isvalid(section1, strlen(section1));
    int kv = isvalid(key1, strlen(key1));

    if((sv+kv) <2) {
        if((sv + kv) ==0) {
            printf("Name of key and section of the record: [%s] \"%s\" is invalid.\n", section1, key1);
        } else {
            if(sv==0) {
                printf("Section name of the record: [%s] \"%s\" is invalid.\n", section1, key1);
            } else {
                printf("Key name of the record: [%s] \"%s\" is invalid.\n", section1, key1);
            }
        }
        return 1;
    }
    return 0;
}

//*******************************************************
//******************ARGUMENT HANDLING********************
//*******************************************************

void getting_section_key_name(int len_exp, char* expression, struct Name_returner* r) {
    int len_of_key;
    int len_of_section;

    len_of_section = dot_finder(expression);
    len_of_key = len_exp-len_of_section-1;

    char* section1 = malloc((len_of_section +1)*sizeof(char));
    char* key1 = malloc((len_of_key +1)*sizeof(char));

    strncpy(section1, expression,len_of_section);
    strncpy(key1, expression + len_of_section + 1,len_of_key);

    key1[len_of_key] = '\0';
    section1[len_of_section] = '\0';

    r->key = &key1[0];
    r->section =  section1;
}
//*******************************************************
//***************** INSERTING FUNCTIONS *****************
//*******************************************************
// - r zwraca obecny pointer na entry(key-value i obecna strukture


struct Returner* add_entry(char* key, char* value, struct Returner* r) {

    struct Entry* new_entry = malloc(sizeof(struct Entry));
    new_entry->value = value;
    new_entry->key = key;
    new_entry->next = NULL;
    if(r->entry) {
        r->entry->next = new_entry;
    } else {
        r->section->first = new_entry;
    }
    r->entry = new_entry;
    return r;
}

struct Returner* add_section(char* name, struct Returner* r) {
    struct Section* new_section =malloc(sizeof(struct Section));
    new_section->name = name;
    new_section->first = NULL;
    new_section->next = NULL;
    if(r->section) {
        r->section->next = new_section;
    }
    r->section = new_section;
    r->entry = new_section->first;
    return r;
}

//*******************************************************
//****** FUNCTION  HANDLING THE INSERTION OF STUFF ******
//*******************************************************

struct Returner* insert(int* state, char* line, struct Returner* r) {
    //status 2 or 0
    if(line[0] == '[') {
        int len = strlen(line) - 2;
        char* name = malloc((len + 1)*(sizeof(char)));
        name = strncpy(name,line+1,len);
        name[len] = '\0';
        r = add_section(name, r);
        *state = 1;
    } else if(*state == 1) {
        //check if out of section or comment
        if(line[0] == ';' || strlen(line)<=1) {
            *state = 2;
        } else {
            int arr[3];
            int size2;
            int size1;//end-start-end
            endstartend(arr, line);
            size2 = arr[2] - arr[1];
            size1 = arr[0] + 1;
            char * key = malloc((size1+1)*sizeof(char));
            char * value = malloc((size2+1)*(sizeof(char)));
            key = strncpy(key,line,size1);
            value = strncpy(value,line + arr[1]+1,size2);
            key[size1] = '\0';
            value[size2] = '\0';
            //printf("val= %s length of it:%d\n", value ,size2);
            r = add_entry(key, value, r);
        }
    }
    return r;
}

//*******************************************************
//*************** TWORZENIE LINKED LISTY*****************
//*******************************************************

struct Section* linked_list_creator(FILE* fp) {
    struct Section* head;
    struct Returner ret;
    ret.entry = NULL;
    ret.section = NULL;
    struct Returner* r = &ret;

    int state = 0;
    int* ptrst = &state;

    size_t size = 5;
    int cur = fgetc(fp);
    while (!feof(fp)) {
        char * buf = malloc(size);
        size_t bsize = size;
        size_t i = 0;
        while((int)cur!=10 && !feof(fp) && (int)cur!=0) {
            if(bsize-1<=i) {
                bsize += size;
                buf = realloc(buf,bsize);
            }
            buf[i] = (char) cur;
            cur = fgetc(fp);
            i++;
        }
        buf[i]='\0';
        //puts(buf);
        if(r->section) {
            r=insert(ptrst,buf,r);
        } else {
            r=insert(ptrst,buf,r);
            head = r->section;
        }
        free(buf);
        cur = fgetc(fp);
    }
    return head;
}

//*******************************************************
//***************** S E A R C H I N G********************
//*******************************************************

char* klucznik(char* sname, char* ename, struct Section* head) {
    struct Section* curs; //current section
    struct Entry* cure; //current entry
    curs = head;
    while(strcmp(curs->name,sname) != 0) {
        //printf("name:%s of len: is not %s \n",curs->name, sname);
        if(curs->next) {
            curs = curs->next;
        } else {
            printf("Failed to find section [%s]\n", sname);
            return NULL;
        }
    }
    cure = curs->first;
    while(strcmp(cure->key,ename)!=0) {
        //printf("name key :%s is not %s\n",cure->key, ename);
        if(cure->next) {
            cure = cure->next;
        } else {
            printf("Failed to find key \"%s\" in section [%s]\n", ename,sname);
            return NULL;
        }
    }
    return cure->value;
}

//*******************************************************
//************** O P E R A T I O N S ********************
//*******************************************************
/// for the grade of 5

void op_num(char* section1, char* key1, char* val1, char* operation, char* section2, char*key2, char* val2) {
    long long int nval1 = atoll(val1);
    long long int nval2 = atoll(val2);
    long long int result;

    switch(op_check(operation)) {
    case 1:
        result = nval1 + nval2;
        break;
    case 2:
        result = nval1 - nval2;
        break;
    case 3:
        result = nval1 * nval2;
        break;
    case 4:
        result = nval1 / nval2;
        break;
    }
    printf("%s.%s %s %s.%s  = %lld\n", section1, key1, operation, section2, key2, result);
}

void op_str(char* section1, char* key1, char* val1, char* operation, char* section2, char*key2, char* val2) {

    if(op_check(operation) == 1) {
        int leng_w = strlen(val1) + strlen(val2);
        char* result = malloc(leng_w +1);
        strcpy(result, val1);
        strcat(result, val2);
        result[leng_w] = '\0';
        printf("%s.%s + %s.%s  = %s\n", section1, key1, section2, key2, result);
        free(result);
    } else {
        printf("%s.%s %s %s.%s is an invalid operation\n", section1, key1,operation, section2, key2);
    }

}

void operations(struct Section* head,char* section1, char* key1, char* operation, char* section2, char*key2) {

    char* value1 = klucznik(section1,key1, head);
    char* value2 = klucznik(section2,key2, head);
    if(value1!=NULL && value2!= NULL) {
        if(is_a_num(value1) && is_a_num(value2)) {
            op_num(section1,key1,value1,operation, section2,key2, value2);
        }
        else if (is_a_num(value1) ^ is_a_num(value2)) {
            printf("It's invalid operation!");
        }
        else {
            op_str(section1,key1,value1,operation, section2,key2, value2);
        }
    } else {
        printf("Therefore not able to execute operation!\n");
    }
}

//*******************************************************
//************** TOTALNE ZNISCZENIE *********************
//*******************************************************
// wypis jako sprawdzenie
void uroboros(struct Section* head) {
    struct Section* curs; // obecna sekcja
    struct Entry* cure; // obecne entry
    struct Section* temps; // temporary var to free memory
    struct Entry* tempe; // -||-

    curs = head;
    while(curs) {
        cure = curs->first;
        while(cure) {
            free(cure->value);
            free(cure->key);
            tempe = cure->next;
            free(cure);
            cure = tempe;
        }
        free(curs->name);
        temps = curs->next;
        free(curs);
        curs = temps;
    }

//printf("MEMORY SUCCESFULLY FREED!\n"); // to tylko do sprawdzania od razu czy na pewno nic siê nie wywala
}
void inspector(struct Section* head) {
    struct Section* curs; // obecna sekcja
    struct Entry* cure; // obecne entry
    int i =0;
    curs = head;
    while(curs) {
        cure = curs->first;
        while(cure) {
            i+= extended_validity_checker(curs->name,cure->key);
            cure = cure->next;
        }
        curs = curs->next;
    }
    printf("I have found %d records with invalid identifiers.\n", i);
}
#endif // _HEADER_H_
