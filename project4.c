#include <stdio.h>

int main() {
    int choice;
    float temp, converted;
    
    printf("Temperature Conversion Menu:\n");
    printf("1. Celsius to Fahrenheit\n");
    printf("2. Fahrenheit to Celsius\n");
    printf("Enter your choice (1 or 2): ");
    scanf("%d", &choice);
    
    if (choice == 1) {
        printf("Enter temperature in Celsius: ");
        scanf("%f", &temp);
        converted = (temp * 9/5) + 32;
        printf("In Fahrenheit: %.1f\n", converted);
    }
    else if (choice == 2) {
        printf("Enter temperature in Fahrenheit: ");
        scanf("%f", &temp);
        converted = (temp - 32) * 5/9;
        printf("In Celsius: %.1f\n", converted);
    }
    else {
        printf("Invalid choice! Please select 1 or 2.\n");
    }
    
    return 0;
}
