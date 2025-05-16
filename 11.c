#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void exec(const char *);

int main(){
    char line[100];
    //історія команд в режимі додавання
    FILE *log = fopen(".replay_log.sh", "a+");
    if (!log){
        perror("Didn't open log\n");
        return 1;
    }
    //постійне зчитування вводу і виклик exec(), поки не введено exit
    printf("---Enter 'exit' to exit, 'replay' to replay---\n");
    while(1){
        printf("cmd> "); //імітація команд. рядка
        if(!fgets(line, sizeof(line), stdin)){
            break; //завеликий рядок
        }
        //видалення символу нового рядка
        line[strcspn(line, "\n")] = 0;
        //перевірка вводу
        if(strcmp(line, "exit") == 0){
            break;
        }
        else if(strcmp(line, "replay") == 0){ //користувач хоче відтворити команду
            fflush(log);
            //запис у шеллівський файл історії команд для майбутнього запуску заново
            FILE *replay = fopen(".replay_log.sh", "r");
            if(!replay){
                printf("Didn't read log\n");
                continue;
            }
            char replay_line[100];
            printf("-------Replaying-------\n");
            while(fgets(replay_line, sizeof(replay_line), replay)){
                replay_line[strcspn(replay_line, "\n")] = 0;
                printf("<<rewind_time<< %s\n", replay_line);
                exec(replay_line);
            }
            fclose(replay);
            printf("-------Ended replaying-------\n");
        }
        else{ //без відтворення чи виходу - ввід виконується
            fprintf(log, "%s\n", line);
            fflush(log);
            exec(line);
        }
    }
    fclose(log);
}

void exec(const char *cmd){
    int res = system(cmd);
    if(res == -1) {
       printf("Didn't execute command\n");
    }
}
