#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ctype.h>

void process_c(const char *, const struct stat *);
void find_user_c(const char *);

int main(int argc, char *argv[]){
    const char *path = ".";
    printf("Searching C files...\n-------\n");
    find_user_c(path);
    printf("-------\n");
}

void process_c(const char *path, const struct stat *file_stat){
    const char *extension = strrchr(path, '.'); //остання крапка (перед типом файлу)
    if(extension == NULL || strcmp(extension, ".c") != 0){
        return; //не c файл
    }
    if(file_stat->st_uid != getuid()){
        return; //файл не належить користувачу
    }
    printf("Your C file at %s\n", path);
    //чи є вже дозвіл на читання іншим
    if(file_stat->st_mode & S_IROTH){
        printf("  Access already given\n");
    }
    else{
        printf("  Give access for others? (y/n): ");
        int choice = getchar();
        int temp;
        while((temp = getchar()) != '\n' && temp != EOF); //очистка рядка
        if(tolower(choice) == 'y'){
            mode_t new_mode = file_stat->st_mode | S_IROTH; //дається дозвіл
            if(chmod(path, new_mode) == 0){
                printf("  Access for others given");
            }
            else{
                printf("  Didn't change access\n");
            }
        }
        else{
            printf("  Accesses not changed\n");
        }
    }
}
//пошук с файлів користувача
void find_user_c(const char *path){
    char path_buff[100];
    DIR *dir;
    if((dir = opendir(path)) == NULL){
        return;
    }
    struct dirent *item;
    while((item = readdir(dir)) != NULL){
        if(strcmp(item->d_name, ".") == 0 || strcmp(item->d_name, "..") == 0){
            continue; //батьківська і поточна - пропуск
        }
        int len = snprintf(path_buff, sizeof(path_buff), "%s/%s", path, item->d_name);
        //якщо буфер переповниться
        if(len < 0 || len >= (int)sizeof(path_buff)){
            printf("Too big path\n");
            continue;
        }
        //lstat повертає дані про файл
        struct stat stat_info;
        if(lstat(path_buff, &stat_info) == -1){
            continue; //файли, з яких не отримується інформація, пропускаються
        }
        if(S_ISDIR(stat_info.st_mode)){
            find_user_c(path_buff); //рекурсія для підпапок
        }
        else if (S_ISREG(stat_info.st_mode)){ //перевірка, що це файл
            process_c(path_buff, &stat_info);
        }
    }
    closedir(dir);
}
