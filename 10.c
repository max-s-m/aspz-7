#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double rand_0_1();
double rand_0_n(double n);

int main(){
    //сім'ячко - поточний час
    srand(time(NULL));
    int nums = 7; //к-ть рандомних чисел
    //від 0 до 1
    printf("Random from 0 to 1:\n");
    for(int i=0; i<nums; i++){
        printf("%.4f\n", rand_0_1());
    }
    printf("\n");
    //ввід n
    printf("Enter n: ");
    double n;
    if(scanf("%lf", &n) != 1){
        printf("Enter real number\n");
        exit(1);
    }
    //від 0 до n
    printf("\nRandom from 0 to %.2f:\n", n);
    for(int i=0; i<nums; i++){
        printf("%.4f\n", rand_0_n(n));
    }
    printf("\n");
}

double rand_0_1(){
    //дасть дробове число від 0 до 1
    return (double)rand() / (double)RAND_MAX;
}

double rand_0_n(double n){
    //число від 0 до 1, помножити на n = число від 0*n=0 до 1*n=n
    return rand_0_1() * n;
}
