#include <stdio.h>
#include <string.h>
#include <getHashFunc.h>

int getHash(char text[20]){
    int counter = 0;
    
    for(int i = 0; i < strlen(text); i++){
        counter = counter + text[i];
    }
    return counter % 50;
}