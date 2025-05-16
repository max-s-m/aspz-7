#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

int main(){
    DIR *dir = opendir(".");
    if(dir == NULL){
        printf("Didn't open this dir'\n");
        exit(1);
    }

    struct dirent *item;
    while((item = readdir(dir)) != NULL){
        //пропуск каталогів та спец. елементи
        if(item->d_type != DT_REG){
            continue;
        }
        //запит користувача про видалення файлу
        char choice[10];
        printf("Delete \"%s\"? (y/n): ", item->d_name);
        if(fgets(choice, sizeof(choice), stdin) != NULL){
            if(choice[0] == 'y' || choice[0] == 'Y'){
                if(remove(item->d_name) == 0){
                    printf("File \"%s\" deleted\n", item->d_name);
                }
                else{
                    printf("Didn't delete\n");
                }
            }
        }
    }
    closedir(dir);
}

