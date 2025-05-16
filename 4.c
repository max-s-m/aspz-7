#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void process_file(const char *);

int main(int argc, char *argv[]){
    if(argc < 2){
        printf("More arg: file1, file2...\n");
        exit(1);
    }
    //запитуємо гортання, поки файли не скінчаться
    for(int i=1; i<argc; i++){
        process_file(argv[i]);
        if(i < argc-1){
            printf("---Enter to next file---\n");
            getchar();
        }
    }
}
//обробка та виведення одного файлу
void process_file(const char *name){
    char line[100];
    int page_lines = 0; //рядків на сторінці виведено
    int file_lines = 0; //рядків у файлі виведено
    int curr_char;
    int cont = 1; //умова продовження циклу

    FILE *file = fopen(name, "r");
    if(file == NULL){
        printf("Didn't open %s\n", name);
        return;
    }
    printf("-----\n%s\n-----\n", name);
    //читання файлу
    while(cont && fgets(line, sizeof(line), file) != NULL){
        printf("%s", line);
        page_lines++;
        file_lines++;
        //максимум 5 рядків на сторінку
        if(page_lines >= 5){
            printf("---Enter to continue, 'q' to exit file, 'n' to next file---");
            fflush(stdout);
            curr_char = getchar();
            //очищення решти буфера
            if(curr_char != '\n' && curr_char != EOF){
                int temp_char;
                while((temp_char = getchar()) != '\n' && temp_char != EOF);
            }
            //очищення рядка з підказкою
            printf("\r                                                             \r");
            fflush(stdout);

            if(curr_char == 'q' || curr_char == 'Q'){
                printf("...Closing %s...\n", name);
                cont = 0;
            }
            else if(curr_char == 'n' || curr_char == 'N'){
                printf("...Going to next file...\n");
                cont = 0;
            }
            else {
                //для інших клавіш - анулювання лічильника та продовження
                page_lines = 0;
            }
        }
    }
    if(file_lines > 0){
         printf("\n");
    }
}
