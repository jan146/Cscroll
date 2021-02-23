#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include <unistd.h>

float delay = 0.1;
int len = 25;
int forceRotate = 0;
char* full;
char* strings[100];
int ctr = 0;

void removeNL(char* arr){
    if (arr[strlen(arr)-1] == '\n')
        arr[strlen(arr)-1] = '\0';
}

char* getStdout(char *command){

    FILE* fp;
    char path[100];

    fp = popen(command, "r");

    if (fp == NULL){
        printf("Command did not run properly\n");
        exit(1);
    }
    
    if (fgets(path, sizeof(path), fp) != NULL){
        char* ret = malloc(strlen(path) + 1);
        strcpy(ret, path);
        removeNL(ret);
        return (ret);
    }

    pclose(fp);

}

void printHelp(){
    printf(
        "Usage: cscroll [OPTIONS] [TEXT]\n\
        OPTIONS:\n\
            -h:         Display this page.\n\
            (--help)    \n\
            -d:         Delay in seconds\n\
            (--delay)   example: cscroll -d 0.5 [TEXT]\n\
                        this will rotate the text every\n\
                        half of a second (500 ms).\n\
                        Default: 0.1 (100 ms).\n\
            -l:         The maximum length of [TEXT]\n\
            (--length)  i. e. if the number of characters\n\
                        (in bytes) is equal or greater than\n\
                        this, it will rotate.\n\
                        Default: 25 characters.\n\
            -f:         Force the text to rotate,\n\
            (--force)   even if it is not too long\n\
                        (longer than -l argument).\n\
                        Default: off.\n\
            -c:         Place your desired command in double\n\
            (--command) quotes (\"\"), after the -c argument\n\
                        to add it to [TEXT].\n\
        \n\
        TEXT:\n\
            Add either strings or commands (-c) to the text\n\
            that you want to display.\n\
            Both have to be encapsulated by double quotes (\"\").\n\
        \n\
        Example (stupid, but probably necessary):\n\
        cscroll -d 0.8 -l 15 -f \"Today is \" -c \"date +%%A\" \".\"\n\
    \n");
}

void invalidArgs(char* reason){
    printf(
        "Invalid usage of cscroll: %s\
Run \"cscroll -h\" for help.\n\
    \n", reason);
    exit(1);
}

void setDelay(char* d){
    if (atof(d) <= 0){
        char* reason = (char*) malloc(30 * sizeof(char) + sizeof(d));
        sprintf(reason, "invalid delay parameter \"%s\"\n", d);
        invalidArgs(reason);
    }
    delay = atof(d);
}

void setLength(char* l){
    if (atoi(l) <= 0){
        char* reason = (char*) malloc(30 * sizeof(char) + sizeof(l));
        sprintf(reason, "invalid length parameter \"%s\"\n", l);
        invalidArgs(reason);
    }
    len = atoi(l);
}

void addCommand(char* c){
    if (ctr > sizeof(strings)/sizeof(char*))
        invalidArgs("too many arguments");
    else
        strings[ctr++] = getStdout(c);
}

void addString(char* s){
    if (ctr > sizeof(strings)/sizeof(char*))
         invalidArgs("too many arguments");
    else
        strings[ctr++] = s;
}

int validChar(char c){
    return isprint(c) || (c >= 0 && c <= 31);
}

int invalidCharsBefore(char* c){
    int counter = 0;
    c--;
    while (!validChar(*c)){
        counter++;
        c--;
    }
    return counter;
}

int invalidCharsAfter(char* c){
    int counter = 0;
    c++;
    while (!validChar(*c)){
        counter++;
        c++;
    }
    return counter;
}

void printArgs(int argc, char* argv[]){
    printf("%d: ", argc);
    for (int i = 0; i < argc; i++){
        printf("[%s] ", argv[i]);
    }
    printf("\n");
}

void parseArgs(int argc, char* argv[]){

    for (int i = 1; i < argc; i++){
        if (argv[i][0] == '-' && argv[i][1] == '-'){
            char* str = argv[i] + 2;
            if (strcmp(str, "help") == 0){
                printHelp();
                continue;
            }
            else if (strcmp(str, "delay") == 0){
                setDelay(argv[++i]);
                continue;   
            }
            else if (strcmp(str, "length") == 0){
                setLength(argv[++i]);
                continue;
            }
            else if (strcmp(str, "force") == 0){
                forceRotate = 1;
                continue;
            }
            else if (strcmp(str, "command") == 0){
                addCommand(argv[++i]);
                continue;
            }
            else{
                char* reason = (char*) malloc(30 * sizeof(char) + sizeof(str));
                sprintf(reason, "invalid argument \"--%s\"\n", str);
                invalidArgs(reason);
            }
        }
        else if (argv[i][0] == '-'){
            switch (argv[i][1]){
                case 'h':  printHelp();
                           break;
                case 'd':  setDelay(argv[++i]);
                           break;
                case 'l':  setLength(argv[++i]);
                           break;
                case 'f':  forceRotate = 1;
                           break;
                case 'c':  addCommand(argv[++i]);
                           break;
                default:{
                    char* reason = (char*) malloc(30 * sizeof(char) + sizeof(argv[i]));
                    sprintf(reason, "invalid argument \"%s\"\n", argv[i]);
                    invalidArgs(reason);
                }
            }
        }
        else
            addString(argv[i]);
    }

}

void rotateText(){
    char* ptr = full;
    for (int i = 0; 1; i++){
        for (int j = 0; j < len; j++){
            char* c = ptr+(i+j)%strlen(full);
            if (isprint(*c))
                printf("%c", *c);
            else {
                if (invalidCharsBefore(c) == invalidCharsAfter(c)){
                    char* first = c - invalidCharsBefore(c);
                    char* last = c + invalidCharsAfter(c); 
                    char wideChar[last - first + 1];
                    for (int i = 0; i < (last - first + 1); i++){
                        printf("%c", *(first+i));
                        wideChar[i] = *(first+i);
                    }
                }
                else
                    printf("%c", ' ');
            }
        }
        if (i >= strlen(full))  // prevent integer overflow
            i -= strlen(full);
        usleep(1000000*delay);
        printf("\n");
    }
}

int main(int argc, char* argv[]){

    setlocale(LC_ALL, "");

    //printArgs(argc, argv);
    parseArgs(argc, argv);
    full = (char*) malloc(200);

    for (int i = 0; i < ctr; i++)
        strcat(full, strings[i]);

    //printf("\"%s\" %d\n", full, strlen(full));

    if (strlen(full) > len || forceRotate)
        rotateText();
    else
        printf(full);

    return(0);

}
