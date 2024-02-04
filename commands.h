// Mohammad Fallahinezhad
// 402106304
#include<stdio.h>
#include<string.h>
#include<dirent.h>
#include<windows.h>
#include<sys/stat.h>
#include<unistd.h>
#include<time.h>
#include "constants.h"

int is_file(char *path) {
    struct stat s;
    if (stat(path, &s) == 0) 
    {
        if (s.st_mode & S_IFDIR) 
            return 0;
        else if (s.st_mode & S_IFREG) 
            return 1;
    }
    else
        return -1;
}

int check_initial_dir_existence() {
    char current_path[500];
    getcwd(current_path, sizeof(current_path));
    int found = 0, root = 0;
    do {
        root = 1;
        DIR* cwd = opendir(".");
        struct dirent* entry;
        while(entry = readdir(cwd)) {
            if(!strcmp(entry->d_name, ".vics")) {
                found = 1; break;
            }
            if(!strcmp(entry->d_name, "..")) root = 0;
        }
        closedir(cwd);
        chdir("..");
        if(found) break;
        
    } while(!root);
    chdir(current_path);
    return found;
}

void get_vics_folder(char* buffer) {
    char current_path[500];
    getcwd(current_path, sizeof(current_path));
    int found = 0, root = 0;
    do {
        root = 1;
        DIR* cwd = opendir(".");
        struct dirent* entry;
        while(entry = readdir(cwd)) {
            if(!strcmp(entry->d_name, ".vics")) {
                found = 1; break;
            }
            if(!strcmp(entry->d_name, "..")) root = 0;
        }
        closedir(cwd);
        chdir("..");
        if(found) break;
        strcat(buffer, "../");
    } while(!root);
    chdir(current_path);
    strcat(buffer, ".vics");
}

void get_staging_folder(char* buffer) {
    get_vics_folder(buffer);
    strcat(buffer, "/staging/");
}

void get_name(char* res) {
    char info_path[500] = ""; get_vics_folder(info_path);
    strcat(info_path, "/user-info.txt");
    FILE* info = fopen(info_path, "r");
    int found = 0;
    if(info != NULL) {
        char buffer[100];
        while(fgets(buffer, sizeof(buffer), info)) {
            int pos = strstr(buffer, ":") - buffer;
            if(!strncmp(buffer, "user.name", pos)) {
                strcpy(res, buffer + pos + 1);
                if(res[strlen(res)-1] == '\n') res[strlen(res)-1] = 0;
                found = 1;
                break;
            }
        }
    } 
    if(!found) {
        info = fopen("D:/uni/fop/project/ViCS/global/user-info.txt", "r");
        if(info != NULL) {
            char buffer[100];
            while(fgets(buffer, sizeof(buffer), info)) {
                int pos = strstr(buffer, ":") - buffer;
                if(!strncmp(buffer, "user.name", pos)) {
                    strcpy(res, buffer + pos + 1);
                    if(res[strlen(res)-1] == '\n') res[strlen(res)-1] = 0;
                    break;
                }
            }
        }
    }
    fclose(info);
}

void get_email(char* res) {
    char info_path[500] = ""; get_vics_folder(info_path);
    strcat(info_path, "/user-info.txt");
    FILE* info = fopen(info_path, "r");
    int found = 0;
    if(info != NULL) {
        char buffer[100];
        while(fgets(buffer, sizeof(buffer), info)) {
            int pos = strstr(buffer, ":") - buffer;
            if(!strncmp(buffer, "user.email", pos)) {
                strcpy(res, buffer + pos + 1);
                if(res[strlen(res)-1] == '\n') res[strlen(res)-1] = 0;
                found = 1;
                break;
            }
        }
    }
    if(!found) {
        info = fopen("D:/uni/fop/project/ViCS/global/user-info.txt", "r");
        if(info != NULL) {
            char buffer[100];
            while(fgets(buffer, sizeof(buffer), info)) {
                int pos = strstr(buffer, ":") - buffer;
                if(!strncmp(buffer, "user.email", pos)) {
                    strcpy(res, buffer + pos + 1);
                    if(res[strlen(res)-1] == '\n') res[strlen(res)-1] = 0;
                    break;
                }
            }
        }
    }
    fclose(info);
}

int config(char* info, char* value, int global) {
    char main_file_path[500], tmp_file_path[500];
    if(!global) {
        char vics_folder_path[500] = "";
        get_vics_folder(vics_folder_path);

        strcpy(main_file_path, vics_folder_path); strcat(main_file_path, "/user-info.txt");
        strcpy(tmp_file_path, vics_folder_path); strcat(tmp_file_path, "/tmp.txt");
    } else {
        strcpy(main_file_path, "D:/uni/fop/project/ViCS/global/user-info.txt");
        strcpy(tmp_file_path, "D:/uni/fop/project/ViCS/global/tmp.txt");
    }

    FILE* config_file = fopen(main_file_path, "r");
    if(config_file == NULL) {
        config_file = fopen(main_file_path, "w");
    }
    FILE* tmp = fopen(tmp_file_path, "w");

    char buffer[500];
    int found = 0;
    while(fgets(buffer, sizeof(buffer), config_file)) {
        if(buffer[strlen(buffer)-1] == '\n') buffer[strlen(buffer)-1] = 0;
        if(strstr(buffer, info) != NULL) {
            strcpy(buffer, info);
            strcat(buffer, ":");
            strcat(buffer, value);
            found = 1;
        }
        fprintf(tmp, "%s\n", buffer);
    }
    if(!found) {
        strcpy(buffer, info);
        strcat(buffer, ":");
        strcat(buffer, value);
        fprintf(tmp, "%s\n", buffer);
    }
    fclose(tmp); fclose(config_file);

    config_file = fopen(main_file_path, "w");
    tmp = fopen(tmp_file_path, "r");
    while(fgets(buffer, sizeof(buffer), tmp)) {
        if(buffer[strlen(buffer)-1] == '\n') buffer[strlen(buffer)-1] = 0;
        fprintf(config_file, "%s\n", buffer);
    }
    fclose(tmp); fclose(config_file);
    remove(tmp_file_path);
    return 0;
}

int add_alias(char* info, char* value, int global) {
    char main_file_path[500], tmp_file_path[500];
    if(!global) {
        char vics_folder_path[500] = "";
        get_vics_folder(vics_folder_path);

        strcpy(main_file_path, vics_folder_path); strcat(main_file_path, "/aliases.txt");
        strcpy(tmp_file_path, vics_folder_path); strcat(tmp_file_path, "/tmp.txt");
    } else {
        strcpy(main_file_path, "D:/uni/fop/project/ViCS/global/aliases.txt");
        strcpy(tmp_file_path, "D:/uni/fop/project/ViCS/global/tmp.txt");
    }

    FILE* config_file = fopen(main_file_path, "r");
    if(config_file == NULL) {
        config_file = fopen(main_file_path, "w");
    }
    FILE* tmp = fopen(tmp_file_path, "w");

    char buffer[500];
    int found = 0;
    while(fgets(buffer, sizeof(buffer), config_file)) {
        if(buffer[strlen(buffer)-1] == '\n') buffer[strlen(buffer)-1] = 0;
        if(strstr(buffer, info) != NULL) {
            strcpy(buffer, info);
            strcat(buffer, ":");
            strcat(buffer, value);
            found = 1;
        }
        fprintf(tmp, "%s\n", buffer);
    }
    if(!found) {
        strcpy(buffer, info);
        strcat(buffer, ":");
        strcat(buffer, value);
        fprintf(tmp, "%s\n", buffer);
    }
    fclose(tmp); fclose(config_file);

    config_file = fopen(main_file_path, "w");
    tmp = fopen(tmp_file_path, "r");
    while(fgets(buffer, sizeof(buffer), tmp)) {
        if(buffer[strlen(buffer)-1] == '\n') buffer[strlen(buffer)-1] = 0;
        fprintf(config_file, "%s\n", buffer);
    }
    fclose(tmp); fclose(config_file);
    remove(tmp_file_path);
    return 0;
}

int init() {
    int error;
    error = check_initial_dir_existence();
    if(error) return 1;
    else {
        // creating .vics:
        CreateDirectory(".vics", NULL);
        SetFileAttributes(".vics", FILE_ATTRIBUTE_HIDDEN);

        // creating "staging" inside .vics:
        chdir(".vics");
        CreateDirectory("staging", NULL);
        chdir("..");

        return 0;
    }
}

int check_equality(char* path1, char* path2) {
    FILE* a = fopen(path1, "r");
    FILE* b = fopen(path2, "r");
    if(a == NULL || b == NULL) return 0;

    char c1, c2;
    int equal = 1;
    do {
        c1 = fgetc(a), c2 = fgetc(b);
        if(c1 != c2) {
            equal = 0; break;
        }
    } while(c1 != EOF && c2 != EOF);

    fclose(a); fclose(b);
    return equal;
}

void copy_file(char* path, char* dest) {
    FILE* from = fopen(path, "r"); 
    FILE* to = fopen(dest, "w");
    char ch;
    do {
        ch = fgetc(from);
        if(ch != EOF) fputc(ch, to);
    } while(ch != EOF);
    fclose(from); fclose(to);
}

void extract_file_name(char* result, char* path) {
    strcpy(result, path);
    
    strrev(result);
    char* pos = strstr(result, "/");
    if(pos == NULL) pos = strstr(result, "\\");
    if(pos == NULL) {
        strrev(result);
    } else {
        result[pos - result] = '\0';
        strrev(result);
    }
}

int stage_file(char* path) {
    char dest[500] = "";
    char tmp[500] = ""; extract_file_name(tmp, path);

    get_staging_folder(dest); strcat(dest, tmp);

    int equal = check_equality(path, dest);
    if(equal) return 0;

    copy_file(path, dest);
    return 1;
}

int unstage_file(char* path) {

    char tmp[500] = ""; extract_file_name(tmp, path);
    char dest[500] = ""; get_staging_folder(dest); strcat(dest, tmp);
    
    if(is_file(dest) == -1) return 0;
    remove(dest);
    return 1;
}

int info_incomplete() {

    char name[100] = "", email[100] = "";
    get_name(name); get_email(email);
    return !(strlen(name) && strlen(email));

}

void commit(char* message, char* result_path) {
    last_commit++;
    char commit_path[500] = "", cur_path[500]; getcwd(cur_path, sizeof(cur_path));
    get_vics_folder(commit_path); strcat(commit_path, "/commits/");
    chdir(commit_path);
    char lstcmt[10]; sprintf(lstcmt, "%d", last_commit);
    mkdir(lstcmt);
    strcat(commit_path, lstcmt); chdir(lstcmt);
    mkdir("contents");
    
    FILE* cmt = fopen("info.txt", "w");

    fprintf(cmt, "message:%s\n", message);
    time_t t = time(NULL);
    struct tm tyme = *localtime(&t);
    fprintf(cmt, "date-time:%d.%02d.%02d - %02d:%02d:%02d\n", tyme.tm_year + 1900, tyme.tm_mon+1, tyme.tm_mday, tyme.tm_hour, tyme.tm_min, tyme.tm_sec);
    fprintf(cmt, "id:%03d%03d\n", cur_branch, last_commit);

    char name[100] = "", email[100] = "";
    get_name(name); get_email(email);
    fprintf(cmt, "author:%s - %s\n", name, email);

    // TODO: commiting :)
    fclose(cmt);

    chdir(cur_path);
}