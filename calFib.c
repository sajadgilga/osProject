#include <stdio.h>
#include <stdlib.h>



int calculate_fib(int number) {
    int fib[number + 2];
    fib[0] = 0;
    fib[1] = 1;
    int i;
    for (i = 2; i <= number; i++) {
        fib[i] = fib[i - 1] + fib[i - 2];
    }
    return fib[number];
}

int main() {
    int number, result;
    scanf("%d", &number);
    result = calculate_fib(number);
    // needs to be filled
    printf("Result is: %d\n", result);
    return 0;
}
