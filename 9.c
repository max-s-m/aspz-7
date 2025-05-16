#include <stdio.h>
#include <unistd.h>
#include <time.h>

void garbage();

int main(){
    clock_t start, end;
    double total;
    //посаток рахування
    start = clock();
    //функція - затримка (сегмент коду, що вимірюється)
    garbage();
    //кінець
    end = clock();
    total = ((double)(end - start) / CLOCKS_PER_SEC) * 1000;
    printf("Time taken: %.2f ms\n", total);

}
//функція, яка займає певний час для виконання
void garbage(){
    long long sum = 0;
    for(long long i = 0; i < 200000000; ++i) {
        sum += i;
    }
}
