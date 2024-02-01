// Mohammad Fallahinezhad
// 402106304
#include<stdio.h>
#include<string.h>
#include<dirent.h>
#include<windows.h>
#include<sys/stat.h>
#include<unistd.h>

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

int stage_file(char* path) {
    char dest[500] = "";
    char tmp[500] = ""; strcpy(tmp, path);
    
    strrev(tmp);
    char* pos = strstr(tmp, "/");
    if(pos == NULL) pos = strstr(tmp, "\\");
    if(pos == NULL) {
        strrev(tmp);
    } else {
        tmp[pos - tmp] = '\0';
        strrev(tmp);
    }

    get_vics_folder(dest); strcat(dest, "/staging/"); strcat(dest, tmp);

    int equal = check_equality(path, dest);
    if(equal) return 0;

    copy_file(path, dest);
    return 1;
}