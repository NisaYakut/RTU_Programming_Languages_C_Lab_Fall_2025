#include <stdio.h>

int sum_to_n(int n) {
    int s = 0;
    for (int i = 1; i <= n; i++) s += i;
    return s;
}

int main(void) {
    int n;
    printf("Enter n: ");
    scanf("%d", &n);

    if (n < 1) {
        printf("Error: n must be >= 1\n");
        return 0;
    }

    printf("Sum 1..%d = %d\n", n, sum_to_n(n));
    return 0;
}
