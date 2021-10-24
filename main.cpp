#include <iostream>
#include "silver.h"

void funb(){
    printf("im fun b(%llx)\n", funb);
}


void funa(){
    printf("im fun a(%llx)\n", funa);
    funb();
}

int main(){
    silver_init(1);

    printf("im fun main(%llx)\n", main);

    funa();

    return 0;
}
