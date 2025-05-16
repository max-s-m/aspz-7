#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
    //перевірка на присутність потрібних аргументів
    if(argc != 3){
        printf("Need 2 args: word for search, file\n");
        exit(1);
    }
    char *searched = argv[1], *name = argv[2];

    FILE *file = fopen(name, "r");
    if(file == NULL){
        printf("Didn't open file\n");
        exit(1);
    }

    char line[100];
    while(fgets(line, sizeof(line), file) != NULL){
        //strstr шукає співпадіння searched у line
        if(strstr(line, searched) != NULL){
            //якщо слово є, друкується рядок
            printf("%s", line);
        }
    }
    fclose(file);
}
