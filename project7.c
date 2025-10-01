#include <stdio.h>

int main() {
    double num1, num2, num3;

    // Ask the user to enter three numbers
    printf("Enter the first number: ");
    scanf("%lf", &num1);

    printf("Enter the second number: ");
    scanf("%lf", &num2);

    printf("Enter the third number: ");
    scanf("%lf", &num3);

    // Determine and print the largest number
    if (num1 >= num2 && num1 >= num3) {
        printf("The largest number is %.2lf\n", num1);
    } else if (num2 >= num1 && num2 >= num3) {
        printf("The largest number is %.2lf\n", num2);
    } else {
        printf("The largest number is %.2lf\n", num3);
    }

    return 0;
}

