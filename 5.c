#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

void print_files(const char *);

int main(int argc, char *argv[]){
    printf("Files in this catalog and daughter catalogs:\n");
    printf("------------\n");
    print_files("."); //цей каталог (поточний)
}

void print_files(const char *path){
    DIR *dir;
    if((dir = opendir(path)) == NULL){
        printf("Didn't open folder %s\n", path);
        return;
    }
    //отримання вмісту папки
    struct dirent *item;
    while((item = readdir(dir)) != NULL){
        //поточний "." та батьківський ".." каталоги скіпаються
        if(strcmp(item->d_name, ".") == 0 || strcmp(item->d_name, "..") == 0) {
            continue;
        }
        //отримується повний шлях (щоб бачити в якій підпапці лежить)
        char path_buff[100];
        int len = snprintf(path_buff, sizeof(path_buff), "%s/%s", path, item->d_name);
        if(len >= (int)sizeof(path_buff)){
            printf("Too big path %s/%s\n", path, item->d_name);
            continue;
        }
        printf("%s\n", path_buff);
        //перевірка типу, каталог - рекурсія цієї ф-ції
        if(item->d_type == DT_DIR){
            print_files(path_buff);
        }
    }
    closedir(dir);
}
