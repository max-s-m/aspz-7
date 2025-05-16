#include <stdio.h>
#include <stdlib.h>

int main(){
    char buff[1000];
    //rwho потребує демону rwhod, який в мене відсутній та через pkg install не встановлюється
    //він має виводити залогінених в систему користувачів (кілька рядків тексту, подібно до who чи w)
    //more - для переглядання тексту по рядкам (enter) та сторінкам (пробіл). ps aux видає купу тексту,
    //що є непоганою демонстрацією функціоналу more, тому rwho замінено на ps aux
    //ps aux - виводить інформцію про всі активні процеси
    //popen відкриває новий процес, створює pipe ("трубу"), яка під'єднує процес з переданою командою
    //pipe тільки в один напрямок - треба зазначити r (read) чи w (write)
    FILE *fp = popen("ps aux", "r");
    if(fp == NULL){
        printf("Didn't rwho\n");
        exit(1);
    }
    //pipe до more для запису (туди записується вивід з ps aux)
    FILE *more = popen("more", "w");
    if(more == NULL){
        printf("Didn't more\n");
        pclose(fp);
        exit(1);
    }
    //зчитування з rwho і передача у more
    while(fgets(buff, sizeof(buff), fp) != NULL){
        fputs(buff, more);
    }
    pclose(fp);
    pclose(more);
}
