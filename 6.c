#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

int compare(const void *, const void *);
void print_dirs_sorted(const char *);

int main(int argc, char *argv[]){
    printf("Sub dirs sorted:\n");
    printf("------------\n");
    print_dirs_sorted(".");
}

int compare(const void *a, const void *b){
    const char **pa = (const char **)a;
    const char **pb = (const char **)b;
    return strcmp(*pa, *pb);
}

void print_dirs_sorted(const char *path){
    DIR *dir;
    struct dirent *item;
    char *dirs[100];
    int count = 0;
    if((dir = opendir(path)) == NULL){
        printf("Couldn't open directory %s\n", path);
        return;
    }
    //збираються всі каталоги
    while((item = readdir(dir)) != NULL){
        if(strcmp(item->d_name, ".") == 0 || strcmp(item->d_name, "..") == 0){
            continue;
        }
        if(item->d_type == DT_DIR){
            if(count >= 100){
                printf("Too many directories in %s\n", path);
                break;
            }
            dirs[count] = strdup(item->d_name);
            count++;
        }
    }
    closedir(dir);
    //сортування списку каталогів та вивід
    qsort(dirs, count, sizeof(char *), compare);
    for(int i = 0; i < count; i++){
        char full_path[100];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, dirs[i]);
        printf("%s\n", full_path);
        print_dirs_sorted(full_path); //рекурсивно обходяться піддиректорії
        free(dirs[i]);
    }
}
