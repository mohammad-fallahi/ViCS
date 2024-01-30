// Mohammad Fallahinezhad
// 402106304
#include "commands.h"
#include "constants.h"

int validate_command(char* command) {
    char copy[500] = "";
    strcpy(copy, command);
    char* token = strtok(copy, " \n\0");
    if(strcmp(token, "vics")) return 0;
    token = strtok(NULL, " \n\0");
    if(token == NULL) return 1;
    for(int i = 0 ; i < COMMANDS_COUNT ; i++) {
        if(!strcmp(token, COMMANDS[i])) return 1;
    }
    return 0;
}

int main(int argc, char *argv[]) {

    if(argc == 1) {
        printf("Thanks for choosing ViCS!\n");
    }
    if(!strcmp(argv[1], "config")) {

        int error;
        int initialized = check_initial_dir_existence();
        if(!initialized) {
            printf("no ViCS project is initialized in this folder or its parent.\n");
            error = 1;
            goto FINAL;
        }
        if(strcmp(argv[2], "-global")) {

            if(strstr(argv[2], "user") == argv[2]) {
                error = config(argv[2], argv[3], 0);
            } else if(strstr(argv[2], "alias") == argv[2]) {
                error = !validate_command(argv[3]);
                if(!error) error = add_alias(argv[2], argv[3], 0);
                else printf("could not make alias: invalid alias command\n");
            } else {
                error = 1;
                printf("invalid command\n");
            }
            
        } else if(!strcmp(argv[2], "-global")) {

            if(strstr(argv[3], "user") == argv[3]) {
                error = config(argv[3], argv[4], 1);
            } else if(strstr(argv[3], "alias") == argv[3]) {
                error = !validate_command(argv[4]);
                if(!error) error = add_alias(argv[3], argv[4], 1);
                else printf("could not make alias: invalid alias command\n");
            } else {
                error = 1;
                printf("invalid command\n");
            }

        }

        FINAL:
        if(error) {
            printf("an error occured.\n");
        } else {
            printf("data updated successfully.\n");
        }
    }

    if(!strcmp(argv[1], "init")) {

        int error = init();
        if(error) printf("you are inside a vics project and can not initialize another project.\n");
        else printf("welcome to ViCS!\n");

    }

    return 0;
}