#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
void print_perms(mode_t);

int main(){
    DIR *dir;
    struct dirent *curr_file;
    struct stat file_stat;
    char timebuf[100];
    //відкриття поточної папки
    dir = opendir(".");
    if(!dir){
        printf("Didn't open this folder\n");
        exit(1);
    }
    //вивід інформації кожного файлу по черзі
    while((curr_file = readdir(dir)) != NULL){
        //"." - поточна папка, ".." - батьківська папка, які ls -l не виводить, тому тут пропускаються
        //lstat отримує інформацію про файл, якщо не вийшло (файл пошкоджений чи видалився), то він не виводиться
        if(!strcmp(curr_file->d_name, ".") || !strcmp(curr_file->d_name, "..") || lstat(curr_file->d_name, &file_stat) == -1){
            continue;
        }
        //права доступу (рядки типу -rw-r--r--)
        print_perms(file_stat.st_mode);
        //кількість hard link-ів (жорстке посилання - по суті асоціація назви з файлом у системі)
        printf("%3ld ", (long) file_stat.st_nlink);
        //власник та група
        struct passwd *pw = getpwuid(file_stat.st_uid);
        struct group  *gr = getgrgid(file_stat.st_gid);
        printf("%-8s %-8s ", pw ? pw->pw_name : "?", gr ? gr->gr_name : "?");
        //розмір
        printf("%8lld ", (long long) file_stat.st_size);
        //час останнього редагування
        strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", localtime(&file_stat.st_mtime));
        printf("%s ", timebuf);
        //назва
        printf("%s\n", curr_file->d_name);
    }
    closedir(dir);
}
//принтування прав доступу
void print_perms(mode_t mode){
    char perms[11] = "----------";
    //тип файлу (перша буква - посилання, директорія, сокет, чи ін)
    if(S_ISDIR(mode)) perms[0] = 'd';
    else if(S_ISLNK(mode)) perms[0] = 'l';
    else if(S_ISCHR(mode)) perms[0] = 'c';
    else if(S_ISBLK(mode)) perms[0] = 'b';
    else if(S_ISSOCK(mode)) perms[0] = 's';
    else if(S_ISFIFO(mode)) perms[0] = 'p';
    //права доступу власника (читання, запис, виконання)
    if(mode & S_IRUSR) perms[1] = 'r';
    if(mode & S_IWUSR) perms[2] = 'w';
    if(mode & S_IXUSR) perms[3] = 'x';
    //права для групи
    if(mode & S_IRGRP) perms[4] = 'r';
    if(mode & S_IWGRP) perms[5] = 'w';
    if(mode & S_IXGRP) perms[6] = 'x';
    //права для інших
    if(mode & S_IROTH) perms[7] = 'r';
    if(mode & S_IWOTH) perms[8] = 'w';
    if(mode & S_IXOTH) perms[9] = 'x';
    printf("%s ", perms);
}

