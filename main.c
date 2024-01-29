// Mohammad Fallahinezhad
// 402106304
#include<stdio.h>
#include<dirent.h>
#include "commands.h"

int main(int argc, char *argv[]) {

    if(argc == 1) {
        printf("Thanks for choosing ViCS!\n");
    }
    if(!strcmp(argv[1], "config")) {
        int error;
        if(strcmp(argv[2], "-global")) {
            if(strcmp(argv[2], "user.name") && strcmp(argv[2], "user.email")) {
                printf("invalid command\n");
                error = 1;
            }
            else error = config(argv[2], argv[3], 0);
        } else {
            if(strcmp(argv[3], "user.name") && strcmp(argv[3], "user.email")) {
                printf("invalid command\n");
                error = 1;
            }
            else error = config(argv[3], argv[4], 1);
        }
        if(error) {
            printf("an error occured.\n");
        } else {
            printf("data updated successfully.\n");
        }
    }

    return 0;
}