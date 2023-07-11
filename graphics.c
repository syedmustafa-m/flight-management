
#include<stdio.h>
#include<conio.h>
#include<ctype.h>

void main(void) {

    int sample;

    printf("Enter a number");
    while(!isdigit(scanf("%d", &sample))) {
        printf("Invalid input");
            break;
    }

    printf("You entered the number %d", sample);

}
