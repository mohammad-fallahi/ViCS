// Mohammad Fallahinezhad
// 402106304
#include "commands.h"

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

int check_date_format(char* date) {
    if(strlen(date) != 10) return 0;
    if(date[4] != '.' || date[7] != '.') return 0;
    for(int i = 0 ; i < 10 ; i++) if(i != 4 && i != 7 && !(date[i] >= '0' && date[i] <= '9')) return 0;
    return 1;
}


int main(int argc, char *argv[]) {

    if(argc == 1) {
        printf("Thanks for choosing ViCS!\n");
        return 0;
    }
    

    if(!strcmp(argv[1], "init")) {

        int error = init();
        if(error) printf("you are inside a vics project and can not initialize another project.\n");
        else printf("welcome to ViCS!\n");

        return 0;
    }


    if(!check_initial_dir_existence()) {
        printf("no ViCS project is initialized in this folder or its parent.\n");
        return 0;
    }

    load_initials();

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
        return 0;
    }


    if(!strcmp(argv[1], "add")) {
        int error = 0;
        int initialized = check_initial_dir_existence();
        if(!initialized) {
            printf("no ViCS project is initialized in this folder or its parent.\n");
            error = 1;
            goto ADD_END;
        }
        if(!strcmp(argv[2], "-n")) {
            char staging_folder_path[500] = ""; get_staging_folder(staging_folder_path);

            DIR* dir = opendir(".");
            struct dirent* entry;
            while(entry = readdir(dir)) {
                if(is_file(entry->d_name) == 1) {
                    DIR* staging_area = opendir(staging_folder_path);
                    int is_staging = 0;
                    struct dirent* staged;
                    while(staged = readdir(staging_area)) {
                        if(is_file(staged->d_name) == 1) is_staging |= !strcmp(staged->d_name, entry->d_name);
                    }
                    closedir(staging_area);
                    if(is_staging) printf("%s: is staged.\n", entry->d_name);
                    else printf("%s: is not staged.\n", entry->d_name);
                }
            }
            closedir(dir);

        } else {
            for(int i = 2 + (!strcmp(argv[2], "-f")) ; i < argc ; i++) {
                int result;
                if(is_file(argv[i]) == 1) {
                    result = stage_file(argv[i]);
                    if(result) printf("%s: added successfully.\n", argv[i]);
                    else printf("%s: adding was unsuccessful.\n", argv[i]);
                } else if(is_file(argv[i]) == 0){
                    DIR* dir = opendir(argv[i]);
                    char dir_path[500]; strcpy(dir_path, argv[i]);
                    int files_added = 0;
                    struct dirent* entry;
                    while(entry = readdir(dir)) {
                        char file_path[500]; strcpy(file_path, dir_path); strcat(file_path, "/"); strcat(file_path, entry->d_name);
                        if(is_file(file_path) == 1) {
                            files_added += stage_file(file_path);
                        }
                    }
                    closedir(dir);
                    printf("%s: %d files added successfully.\n", argv[i], files_added);
                } else {
                    printf("%s: no such file or directory.\n", argv[i]);
                }   
            }
        }


        ADD_END:
        if(error) printf("an error occured.\n");
        return 0;
    }

    if(!strcmp(argv[1], "reset")) {
        int error = 0;
        int initialized = check_initial_dir_existence();
        if(!initialized) {
            printf("no ViCS project is initialized in this folder or its parent.\n");
            error = 1;
            goto RESET_END;
        }

        for(int i = 2 + (!strcmp(argv[2], "-f")) ; i < argc ; i++) {
            if(is_file(argv[i]) == 1) {
                int result = unstage_file(argv[i]);
                if(result) printf("%s: unstaged successfully.\n", argv[i]);
                else printf("%s: unstaging was unsuccessful.\n", argv[i]);
            } else if(is_file(argv[i]) == 0) {
                DIR* dir = opendir(argv[i]);
                char dir_path[500]; strcpy(dir_path, argv[i]);
                int files_unstaged = 0;
                struct dirent* entry;
                while(entry = readdir(dir)) {
                    char file_path[500]; strcpy(file_path, dir_path); strcat(file_path, "/"); strcat(file_path, entry->d_name);
                    if(is_file(file_path) == 1) {
                        files_unstaged += unstage_file(file_path);
                    }
                }
                closedir(dir);
                printf("%s: %d files unstaged successfully.\n", argv[i], files_unstaged);
            } else {
                printf("%s: no such file or directory.\n", argv[i]);
            }
        }

        RESET_END:
        if(error) printf("an error occured.\n");
        return 0;
    }

    if(!strcmp(argv[1], "commit")) {
        int error = 0;
        int initialized = check_initial_dir_existence();
        if(!initialized) {
            printf("no ViCS project is initialized in this folder or its parent.\n");
            error = 1;
            goto COMMIT_END;
        }
        
        if(argc > 4) {
            printf("invalid usage of commit command.\n");
            error = 1; goto COMMIT_END;
        }
        if(argc <= 3) {
            printf("please enter commit message.\n");
            error = 1; goto COMMIT_END;
        }
        if(strcmp(argv[2], "-m")) {
            printf("invalid usage of commit command.\n");
            error = 1; goto COMMIT_END;
        }

        if(strlen(argv[3]) > 72) {
            printf("long commit message. commit message should be at most 72 characters long.\n");
            error = 1;
            goto COMMIT_END;
        }

        if(info_incomplete()) {
            printf("please set your user name and email with 'vics config' and try again.\n");
            error = 1;
            goto COMMIT_END;
        }

        char commit_file_path[500] = "";
        error = commit(argv[3], commit_file_path);
        if(error) goto COMMIT_END;
        printf("commited successfully.\n");

        FILE* commit_file = fopen(commit_file_path, "r");
        char buffer[100];
        while(fgets(buffer, sizeof(buffer), commit_file)) {
            int pos = strstr(buffer, ":") - buffer;
            if(!strncmp(buffer, "id", pos)) printf("%s", buffer);
            if(!strncmp(buffer, "message", pos)) printf("%s", buffer);
            if(!strncmp(buffer, "date-time", pos)) printf("%s", buffer);
        }
        fclose(commit_file);

        COMMIT_END:
        if(error) printf("an error occured.\n");
        return 0;
    }

    if(!strcmp(argv[1], "log")) {
        int error = 0;
        int initialized = check_initial_dir_existence();
        if(!initialized) {
            printf("no ViCS project is initialized in this folder or its parent.\n");
            error = 1;
            goto LOG_END;
        }

        if(argc == 2) {
            no_condition_log();
        } else {

            if(!strcmp(argv[2], "-n")) {
                int c; sscanf(argv[3], "%d", &c);
                numbered_log(c);
            }
            if(!strcmp(argv[2], "-branch")) {
                int is_valid = 0;
                for(int i = 0 ; i < branch_count ; i++) {
                    if(!strcmp(BRANCHES[i], argv[3])) {
                        is_valid = 1; break;
                    }
                }
                if(!is_valid) {
                    printf("no such branch.\n");
                    error = 1; goto LOG_END;
                } else {
                    branched_log(argv[3]);
                }
            }
            if(!strcmp(argv[2], "-author")) {
                named_log(argv[3]);
            }
            if(!strcmp(argv[2], "-search")) {
                char words[10][20];
                for(int i = 3 ; i < argc ; i++) strcpy(words[i-3], argv[i]);
                search_log(argc-3, words);
            }
            if(!strcmp(argv[2], "-since")) {
                if(!check_date_format(argv[3])) {
                    printf("wrong date format. please input date in format YYYY.MM.DD\n");
                    error = 1; goto LOG_END;
                }
                since_log(argv[3]);
            }
            if(!strcmp(argv[2], "-before")) {
                if(!check_date_format(argv[3])) {
                    printf("wrong date format. please input date in format YYYY.MM.DD\n");
                    error = 1; goto LOG_END;
                }
                before_log(argv[3]);
            }
        }

        LOG_END:
        if(error) printf("an error occured.\n");
        return 0;
    }

    if(!strcmp(argv[1], "branch")) {
        int error = 0;

        if(argc == 2) {
            printf("all branches:\n");
            for(int i = 0 ; i < branch_count ; i++) {
                if(i == cur_branch) printf("*%s\n", BRANCHES[i]);
                else printf(" %s\n", BRANCHES[i]);
            }
        } else {
            int exists = 0;
            for(int i = 0 ; i < branch_count ; i++) {
                if(!strcmp(argv[2], BRANCHES[i])) {
                    exists = 1; break;
                }
            }
            if(exists) {
                printf("a branch with this name already exists.\n");
                error = 1; goto BRANCH_END;
            }
            add_branch(argv[2]);
        }

        BRANCH_END:
        if(error) printf("an error occured.\n");
        return 0;
    }

    if(!strcmp(argv[1], "checkout")) {
        int error = 0;

        if(argc == 2) {
            printf("invalid usage of command.\n");
            error = 1; goto CHECKOUT_END;
        }

        int is_commit = 1;
        for(int i = 0 ; argv[2][i] ; i++) {
            if(argv[2][i] < '0' || argv[2][i] > '9') {
                is_commit = 0; break;
            }
        }
        if(!strcmp(argv[2], "HEAD")) is_commit = 1;

        if(!is_commit) {
            int exists = 0;
            for(int i = 0 ; i < branch_count ; i++) {
                if(!strcmp(BRANCHES[i], argv[2])) {
                    exists = 1; break;
                }
            }
            if(!exists) {
                printf("there is no branch with this name.\n");
                error = 1; goto CHECKOUT_END;
            }
            error = checkout_branch(argv[2]);
            if(!error) {
                printf("checkout complete.\n");
            }
        } else {
            error = checkout_commit(argv[2]);
            if(!error) {
                printf("checkout complete.\n");
            }
        }

        CHECKOUT_END:
        if(error) printf("an error occured.\n");
        return 0;
    }

    if(!strcmp(argv[1], "status")) {
        printf("status of changes:\n");
        show_status();
        return 0;
    }

    if(!strcmp(argv[1], "tag")) {
        int error = 0;

        if(argc == 2) {
            list_tags();
            goto TAG_END;
        }
        if(argc == 4 && !strcmp(argv[2], "show")) {
            error = show_tag(argv[3]);
            goto TAG_END;
        }


        if(argc < 4 || strcmp(argv[2], "-a")) {
            printf("invalid usage of command.\n");
            error = 1;
            goto TAG_END;
        }
        char tag_message[100] = "";
        char commit_id[20]; sprintf(commit_id, "%03d%03d", cur_branch, cur_commit);
        int overwrite = 0;
        if(argc >= 6 && !strcmp(argv[4], "-m")) {
            strcpy(tag_message, argv[5]);
        }
        if(argc >= 6 && !strcmp(argv[4], "-c")) {
            strcpy(commit_id, argv[5]);
        }
        if(argc >= 8 && !strcmp(argv[6], "-c")) {
            strcpy(commit_id, argv[7]);
        }
        overwrite = !strcmp(argv[argc-1], "-f");

        error = create_tag(argv[3], tag_message, commit_id, overwrite);
        if(!error) printf("tag created successfully.\n");


        TAG_END:
        if(error) printf("an error occured.\n");
        return 0;
    }

    if(!strcmp(argv[1], "grep")) {
        int error = 0;
        if(argc < 6 || strcmp(argv[2], "-f") || strcmp(argv[4], "-p")) {
            printf("invalid usage of the command.\n");
            error = 1;
            goto GREP_END;
        }

        char commit_id[20]; sprintf(commit_id, "%03d%03d", cur_branch, cur_commit);
        int another_commit = 0;
        if(argc >= 8) {
            strcpy(commit_id, argv[7]);
            another_commit = 1;
        }
        int line_number = !strcmp(argv[argc-1], "-n");

        error = grep(argv[3], argv[5], commit_id, another_commit, line_number);

        GREP_END:
        if(error) printf("an error occured.\n");
        return 0;
    }

    if(!strcmp(argv[1], "merge")) {
        int error;
        if(argc != 3) {
            printf("invalid usage of command.\n");
            error = 1; goto MERGE_END;
        }

        error = merge(argv[2]);
        if(!error) printf("branch %s merged into current branch(%s) successfully.\n", argv[2], BRANCHES[cur_branch]);

        MERGE_END:
        if(error) printf("an error occured.\n");
        return 0;
    }

    if(!strcmp(argv[1], "pre-commit")) {

        if(argc > 2) {
            if(!strcmp(argv[2], "hooks")) {
                list_all_hooks();
            }
            if(!strcmp(argv[2], "applied")) {
                list_applied_hooks();
            }
            if(!strcmp(argv[2], "add")) {
                add_hook(argv[4]);
            }
            if(!strcmp(argv[2], "remove")) {
                remove_hook(argv[4]);
            }
            
        } else {
            pre_commit();
        }

        return 0;
    }

    if(!run_alias(argv[1])) {
        printf("there is no command '%s'.\n", argv[1]);
    }

    return 0;
}