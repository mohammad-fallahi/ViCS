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

void get_commits_folder(char* buffer) {
    get_vics_folder(buffer);
    strcat(buffer, "/commits/");
}

void get_tags_folder(char* buffer) {
    get_vics_folder(buffer); strcat(buffer, "/tags/");
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

int get_last_commit() {
    int last_commit;
    char tmp[500] = ""; get_commits_folder(tmp); strcat(tmp, "number-of-commits.txt");
    FILE* f = fopen(tmp, "r");
    fscanf(f, "%d", &last_commit);
    fclose(f);
    return last_commit;
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
        chdir("staging");
        CreateDirectory("file-paths", NULL);
        chdir("..");
        chdir("..");

        // creating "commits" inside .vics:
        chdir(".vics");
        CreateDirectory("commits", NULL);
        FILE* file = fopen("commits/number-of-commits.txt", "w");
        fprintf(file, "0\n");
        fclose(file);
        chdir("commits");
        CreateDirectory("0", NULL);
        chdir("0");
        CreateDirectory("contents", NULL);
        chdir("..");
        chdir("..");
        chdir("..");

        // creating "tags" inside .vics:
        chdir(".vics");
        CreateDirectory("tags", NULL);
        chdir("..");

        // creating branches file:
        chdir(".vics");
        FILE* br = fopen("branches.txt", "w");
        fprintf(br, "master:0\n");
        fclose(br);
        chdir("..");

        // creating currents file:
        chdir(".vics");
        FILE* curs = fopen("currents.txt", "w");
        fprintf(curs, "commit:0\n");
        fprintf(curs, "branch:0\n");
        fclose(curs);
        chdir("..");

        // creating hooks file:
        chdir(".vics");
        FILE* hooks = fopen("hooks.txt", "w");
        fprintf(hooks, "todo-check:0\n");
        fprintf(hooks, "eof-blank-space:0\n");
        fprintf(hooks, "format-check:0\n");
        fprintf(hooks, "balance-braces:0\n");
        fprintf(hooks, "indentation-check:0\n");
        fprintf(hooks, "static-error-check:0\n");
        fprintf(hooks, "file-size-check:0\n");
        fprintf(hooks, "character-limit:0\n");
        fprintf(hooks, "time-limit:0\n");
        fclose(hooks);

        return 0;
    }
}

void load_initials() {
    char branches[500] = ""; get_vics_folder(branches); strcat(branches, "/");
    strcat(branches, "branches.txt");
    FILE* branch_list = fopen(branches, "r");
    char buffer[50];
    while(fgets(buffer, sizeof(buffer), branch_list)) {
        if(buffer[strlen(buffer)-1] == '\n') buffer[strlen(buffer)-1] = 0;
        char* pos = strstr(buffer, ":");
        buffer[pos-buffer] = 0;
        strcpy(BRANCHES[branch_count], buffer);
        branch_count++;
    }
    fclose(branch_list);

    char currents[500] = ""; get_vics_folder(currents); strcat(currents, "/currents.txt");
    FILE* curs = fopen(currents, "r");
    fgets(buffer, sizeof(buffer), curs);
    char* pos = strstr(buffer, ":");
    sscanf(pos+1, "%d", &cur_commit);
    fgets(buffer, sizeof(buffer), curs);
    pos = strstr(buffer, ":");
    sscanf(pos+1, "%d", &cur_branch);

    fclose(curs);
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

void super_copy(char* file, char* dest) {
    char cur_path[500]; getcwd(cur_path, sizeof(cur_path));
    char dest_copy[500]; strcpy(dest_copy, dest);
    for(int i = strlen(dest_copy)-1 ; i >= 0 ; i--) {
        if(dest_copy[i] == '\\' || dest_copy[i] == '/') {
            dest_copy[i] = 0; break;
        }
    }
    char* token = strtok(dest_copy, "/\\");
    while(token) {
        if(is_file(token) == 0 || is_file(token) == -1) {
            if(chdir(token)) {
                mkdir(token);
                chdir(token);
            }
        }
        token = strtok(NULL, "/\\");
    }
    chdir(cur_path);
    copy_file(file, dest);
}

void recursive_copy(char* root) {

    char root_name[100];
    int i;
    for(i = strlen(root)-1 ; i >= 0 ; i--) {
        if(root[i] == '\\' || root[i] == '/') {
            strcpy(root_name, root+i+1); break;
        }
    }

    if(i == -1) strcpy(root_name, root);
    if(is_file(root) == 1) {

        copy_file(root, root_name);
        return;
    }
    if(is_file(root) == 0) {

        mkdir(root_name);
        DIR* dir = opendir(root);
        struct dirent* entry;
        while(entry = readdir(dir)) {
            if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) continue;

            chdir(root_name);
            char new_root[500]; strcpy(new_root, "../"); strcat(new_root, root); strcat(new_root, "/"); strcat(new_root, entry->d_name);
            recursive_copy(new_root);
            chdir("..");
        }
        closedir(dir);
    }

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

void go_to(char* path) {
    char copy[500]; strcpy(copy, path);
    char* token = strtok(copy, "\\/");
    while(token) {
        if(is_file(token) == 0) {
            chdir(token);
        }
        token = strtok(NULL, "\\/");
    }
}

int stage_file(char* path) {
    char dest[500] = "";
    char tmp[500] = ""; extract_file_name(tmp, path);

    get_staging_folder(dest); strcat(dest, tmp);

    int equal = check_equality(path, dest);
    if(equal) return 0;

    copy_file(path, dest);

    char cur_path[500]; getcwd(cur_path, sizeof(cur_path));
    go_to(path);
    char file_absolute_path[500]; getcwd(file_absolute_path, sizeof(file_absolute_path)); 
    strcat(file_absolute_path, "\\");
    strcat(file_absolute_path, tmp);
    char filenames[500] = ""; get_staging_folder(filenames);
    strcat(filenames, "file-paths/"); strcat(filenames, tmp);
    FILE* f = fopen(filenames, "w");
    fprintf(f, "%s\n", file_absolute_path);
    fclose(f);
    chdir(cur_path);

    return 1;
}

int unstage_file(char* path) {

    char tmp[500] = ""; extract_file_name(tmp, path);
    char dest[500] = ""; get_staging_folder(dest); strcat(dest, tmp);
    
    if(is_file(dest) == -1) return 0;
    remove(dest);
    char ch[500]; get_staging_folder(ch); strcat(ch, "file-paths/"); strcat(ch, tmp);
    remove(ch);
    return 1;
}

int info_incomplete() {

    char name[100] = "", email[100] = "";
    get_name(name); get_email(email);
    return !(strlen(name) && strlen(email));

}

int first_commit(int branch_code) {
    char branches[500] = ""; get_vics_folder(branches);
    strcat(branches, "/branches.txt");

    FILE* branch_list = fopen(branches, "r");
    char buffer[100];
    int res = -1;
    while(fgets(buffer, sizeof(buffer), branch_list)) {
        char* pos = strstr(buffer, ":");
        if(!strncmp(BRANCHES[branch_code], buffer, pos-buffer)) {
            int cmt; sscanf(pos+1, "%d", &cmt);
            res = cmt; break;
        }
    }
    fclose(branch_list);
    return res;
}

int find_head(int branch_code) {

    char commits_folder[500] = ""; get_commits_folder(commits_folder);
    DIR* commits = opendir(commits_folder);
    struct dirent* entry;
    int res = -1;
    while(entry = readdir(commits)) {
        if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) continue;
        char commit_path[500]; sprintf(commit_path, "%s%s", commits_folder, entry->d_name);
        if(is_file(commit_path) == 0) {
            strcat(commit_path, "/info.txt");
            FILE* info = fopen(commit_path, "r");
            if(info == NULL) {
                if(first_commit(branch_code) == 0) res = 0;
                continue;
            }
            char buffer[100];
            while(fgets(buffer, sizeof(buffer), info)) {
                char* pos = strstr(buffer, ":");
                if(!strncmp(buffer, "id", pos-buffer)) {
                    char id[20]; sscanf(pos+1, "%s", id);
                    int br = 100*(id[0]-'0') + 10*(id[1]-'0') + (id[2]-'0');
                    int cmt = 100*(id[3]-'0') + 10*(id[4]-'0') + (id[5]-'0');
                    if(br == branch_code || first_commit(branch_code) == cmt) {
                        int number; sscanf(entry->d_name, "%d", &number);
                        if(number > res) res = number;
                    }
                    break;
                }
            }
            fclose(info);
        }
    }
    closedir(commits);

    return res;
}

void get_relative_path(char* abs, char* res) {
    
    char file_name[100];
    for(int i = strlen(abs)-1 ; i >= 0 ; i--) {
        if(abs[i] == '\\' || abs[i] == '/') {
            strcpy(file_name, abs+i+1);
            abs[i] = 0;  break;
        }
    }
    strcpy(res, file_name);

    char cur_path[500]; getcwd(cur_path, sizeof(cur_path));
    chdir(abs);


    while(check_initial_dir_existence()) {
        char last_dir[100];
        for(int i = strlen(abs)-1 ; i >= 0 ; i--) {
            if(abs[i] == '\\' || abs[i] == '/') {
                strcpy(last_dir, abs+i+1);
                abs[i] = 0; break;
            }
        }
        strcat(last_dir, "/"); strcat(last_dir, res);
        strcpy(res, last_dir);

        chdir(abs);
    }
    char* pos = strstr(res, "/");
    strcpy(res, pos+1);

    chdir(cur_path);
}

void clear_stage() {

    char staging_folder[500] = ""; get_staging_folder(staging_folder);
    char file_paths_folder[500]; strcpy(file_paths_folder, staging_folder); strcat(file_paths_folder, "file-paths/");
    DIR* staging_area = opendir(staging_folder);
    struct dirent* entry;
    while(entry = readdir(staging_area)) {
        char entry_path[500]; strcpy(entry_path, staging_folder); strcat(entry_path, entry->d_name);
        if(is_file(entry_path) == 1) {
            remove(entry_path);
        }
    }
    closedir(staging_area);

    DIR* file_paths = opendir(file_paths_folder);
    while(entry = readdir(file_paths)) {
        char entry_path[500]; strcpy(entry_path, file_paths_folder); strcat(entry_path, entry->d_name);
        if(is_file(entry_path) == 1) {
            remove(entry_path);
        }
    }
    closedir(file_paths);
}

int commit(char* message, char* result_path) {

    int head = find_head(cur_branch);
    if(head != cur_commit) {
        printf("commit failed: this is not the head of this branch. commits are only allowed in heads.\n");
        clear_stage();
        return 1;
    }

    int last_commit;
    char tmp[500] = ""; get_commits_folder(tmp); strcat(tmp, "number-of-commits.txt");
    FILE* f = fopen(tmp, "r");
    fscanf(f, "%d", &last_commit);
    fclose(f);
    last_commit++;
    f = fopen(tmp, "w");
    fprintf(f, "%d\n", last_commit);
    fclose(f);

    char curs_path[500] = ""; get_vics_folder(curs_path); strcat(curs_path, "/currents.txt");
    FILE* curs = fopen(curs_path, "w");
    fprintf(curs, "commit:%d\n", last_commit);
    fprintf(curs, "branch:%d\n", cur_branch);
    fclose(curs);

    char commit_path[500] = "", cur_path[500]; getcwd(cur_path, sizeof(cur_path));
    get_commits_folder(commit_path);
    chdir(commit_path);
    char lstcmt[10]; sprintf(lstcmt, "%d", last_commit);
    mkdir(lstcmt);
    strcat(commit_path, lstcmt); chdir(lstcmt);
    
    FILE* cmt = fopen("info.txt", "w");

    fprintf(cmt, "message:%s\n", message);
    time_t t = time(NULL);
    struct tm tyme = *localtime(&t);
    fprintf(cmt, "date-time:%d.%02d.%02d - %02d:%02d:%02d\n", tyme.tm_year + 1900, tyme.tm_mon+1, tyme.tm_mday, tyme.tm_hour, tyme.tm_min, tyme.tm_sec);
    fprintf(cmt, "id:%03d%03d\n", cur_branch, last_commit);

    char name[100] = "", email[100] = "";
    get_name(name); get_email(email);
    fprintf(cmt, "author:%s - %s\n", name, email);

    char prev_cmt_contents[500] = ""; get_commits_folder(prev_cmt_contents);
    char prev_cmt[10] = ""; sprintf(prev_cmt, "%d", cur_commit); 
    strcat(prev_cmt_contents, prev_cmt); strcat(prev_cmt_contents, "/contents");

    recursive_copy(prev_cmt_contents);
    char staging_folder[500] = ""; get_staging_folder(staging_folder);
    char file_paths[500]; strcpy(file_paths, staging_folder); strcat(file_paths, "file-paths/");
    DIR* staging_area = opendir(staging_folder);
    struct dirent* entry;
    int number_of_files_commited = 0;
    while(entry = readdir(staging_area)) {
        char cur_file[500]; strcpy(cur_file, staging_folder); strcat(cur_file, entry->d_name);
        if(is_file(cur_file) == 1) {

            char absolute_file_path[500];
            char cur_file_path[500]; strcpy(cur_file_path, file_paths); strcat(cur_file_path, "/"); strcat(cur_file_path, entry->d_name);
            FILE* f = fopen(cur_file_path, "r");
            fscanf(f, "%s", absolute_file_path);
            fclose(f);

            char relative_file_path[500] = "", tmp[500] = ""; 
            get_relative_path(absolute_file_path, tmp);
            strcat(relative_file_path, "contents/");
            strcat(relative_file_path, tmp);
            super_copy(cur_file, relative_file_path);
            number_of_files_commited++;
        }
    }
    closedir(staging_area);

    fprintf(cmt, "files commited:%d\n", number_of_files_commited);
    fclose(cmt);

    char ttt[500]; getcwd(ttt, sizeof(ttt));
    strcpy(result_path, ttt);
    strcat(result_path, "/info.txt");

    chdir(cur_path);

    clear_stage();
    return 0;
}

void no_condition_log() {

    char commits_folder[500] = ""; get_commits_folder(commits_folder);
    int last_commit = get_last_commit();
    if(last_commit == 0) {
        printf("there is no commits yet.\n");
        return;
    }

    for(int i = last_commit ; i > 0 ; i--) {
        char num[15] = ""; sprintf(num, "%d/info.txt", i);
        char commit_info_path[500]; strcpy(commit_info_path, commits_folder); strcat(commit_info_path, num);
        FILE* commit_info = fopen(commit_info_path, "r");
        char id[50], message[50], tyme[50], author[50], branch[50], cnt[50];
        char buffer[100];
        while(fgets(buffer, sizeof(buffer), commit_info)) {
            char* pos = strstr(buffer, ":");
            if(!strncmp(buffer, "message", pos-buffer)) {
                sprintf(message, "%s", pos+1);
                if(message[strlen(message)-1] == '\n') message[strlen(message)-1] = 0;
            }
            if(!strncmp(buffer, "date-time", pos-buffer)) {
                sprintf(tyme, "%s", pos+1);
                if(tyme[strlen(tyme)-1] == '\n') tyme[strlen(tyme)-1] = 0;
            }
            if(!strncmp(buffer, "id", pos-buffer)) {
                sprintf(id, "%s", pos+1);
                if(id[strlen(id)-1] == '\n') id[strlen(id)-1] = 0;
                sprintf(branch, "%s", BRANCHES[100*(id[0]-'0') + 10*(id[1]-'0') + (id[2]-'0')]);
            }
            if(!strncmp(buffer, "author", pos-buffer)) {
                sprintf(author, "%s", pos+1);
                if(author[strlen(author)-1] == '\n') author[strlen(author)-1] = 0;
            }
            if(!strncmp(buffer, "files commited", pos-buffer)) {
                sprintf(cnt, "%s", pos+1);
                if(cnt[strlen(cnt)-1] == '\n') cnt[strlen(cnt)-1] = 0;
            }
        }
        fclose(commit_info);

        printf("commit with commit id: %s at %s\n", id, tyme);
        printf("\tauthor: %s\n", author);
        printf("\tmessage: %s\n", message);
        printf("\ton branch: %s\n", branch);
        printf("\t%s files commited.\n", cnt);
        printf("***************************************\n");
    }

}

void numbered_log(int n) {

    char commits_folder[500] = ""; get_commits_folder(commits_folder);
    int last_commit = get_last_commit();
    if(last_commit == 0) {
        printf("there is no commits yet.\n");
        return;
    }

    for(int i = last_commit ; i > 0 && i > last_commit - n ; i--) {
        char num[15] = ""; sprintf(num, "%d/info.txt", i);
        char commit_info_path[500]; strcpy(commit_info_path, commits_folder); strcat(commit_info_path, num);
        FILE* commit_info = fopen(commit_info_path, "r");
        char id[50], message[50], tyme[50], author[50], branch[50], cnt[50];
        char buffer[100];
        while(fgets(buffer, sizeof(buffer), commit_info)) {
            char* pos = strstr(buffer, ":");
            if(!strncmp(buffer, "message", pos-buffer)) {
                sprintf(message, "%s", pos+1);
                if(message[strlen(message)-1] == '\n') message[strlen(message)-1] = 0;
            }
            if(!strncmp(buffer, "date-time", pos-buffer)) {
                sprintf(tyme, "%s", pos+1);
                if(tyme[strlen(tyme)-1] == '\n') tyme[strlen(tyme)-1] = 0;
            }
            if(!strncmp(buffer, "id", pos-buffer)) {
                sprintf(id, "%s", pos+1);
                if(id[strlen(id)-1] == '\n') id[strlen(id)-1] = 0;
                sprintf(branch, "%s", BRANCHES[100*(id[0]-'0') + 10*(id[1]-'0') + (id[2]-'0')]);
            }
            if(!strncmp(buffer, "author", pos-buffer)) {
                sprintf(author, "%s", pos+1);
                if(author[strlen(author)-1] == '\n') author[strlen(author)-1] = 0;
            }
            if(!strncmp(buffer, "files commited", pos-buffer)) {
                sprintf(cnt, "%s", pos+1);
                if(cnt[strlen(cnt)-1] == '\n') cnt[strlen(cnt)-1] = 0;
            }
        }
        fclose(commit_info);

        printf("commit with commit id: %s at %s\n", id, tyme);
        printf("\tauthor: %s\n", author);
        printf("\tmessage: %s\n", message);
        printf("\ton branch: %s\n", branch);
        printf("\t%s files commited.\n", cnt);
        printf("***************************************\n");
    }
}

void branched_log(char* br) {
    char commits_folder[500] = ""; get_commits_folder(commits_folder);
    int last_commit = get_last_commit();
    if(last_commit == 0) {
        printf("there is no commits yet.\n");
        return;
    }

    for(int i = last_commit ; i > 0 ; i--) {
        char num[15] = ""; sprintf(num, "%d/info.txt", i);
        char commit_info_path[500]; strcpy(commit_info_path, commits_folder); strcat(commit_info_path, num);
        FILE* commit_info = fopen(commit_info_path, "r");
        char id[50], message[50], tyme[50], author[50], branch[50], cnt[50];
        char buffer[100];
        while(fgets(buffer, sizeof(buffer), commit_info)) {
            char* pos = strstr(buffer, ":");
            if(!strncmp(buffer, "message", pos-buffer)) {
                sprintf(message, "%s", pos+1);
                if(message[strlen(message)-1] == '\n') message[strlen(message)-1] = 0;
            }
            if(!strncmp(buffer, "date-time", pos-buffer)) {
                sprintf(tyme, "%s", pos+1);
                if(tyme[strlen(tyme)-1] == '\n') tyme[strlen(tyme)-1] = 0;
            }
            if(!strncmp(buffer, "id", pos-buffer)) {
                sprintf(id, "%s", pos+1);
                if(id[strlen(id)-1] == '\n') id[strlen(id)-1] = 0;
                sprintf(branch, "%s", BRANCHES[100*(id[0]-'0') + 10*(id[1]-'0') + (id[2]-'0')]);
            }
            if(!strncmp(buffer, "author", pos-buffer)) {
                sprintf(author, "%s", pos+1);
                if(author[strlen(author)-1] == '\n') author[strlen(author)-1] = 0;
            }
            if(!strncmp(buffer, "files commited", pos-buffer)) {
                sprintf(cnt, "%s", pos+1);
                if(cnt[strlen(cnt)-1] == '\n') cnt[strlen(cnt)-1] = 0;
            }
        }
        fclose(commit_info);
        if(strcmp(br, branch)) continue;

        printf("commit with commit id: %s at %s\n", id, tyme);
        printf("\tauthor: %s\n", author);
        printf("\tmessage: %s\n", message);
        printf("\ton branch: %s\n", branch);
        printf("\t%s files commited.\n", cnt);
        printf("***************************************\n");
    }
}

void named_log(char* name) {
    char commits_folder[500] = ""; get_commits_folder(commits_folder);
    int last_commit = get_last_commit();
    if(last_commit == 0) {
        printf("there is no commits yet.\n");
        return;
    }

    for(int i = last_commit ; i > 0 ; i--) {
        char num[15] = ""; sprintf(num, "%d/info.txt", i);
        char commit_info_path[500]; strcpy(commit_info_path, commits_folder); strcat(commit_info_path, num);
        FILE* commit_info = fopen(commit_info_path, "r");
        char id[50], message[50], tyme[50], author[50], branch[50], cnt[50];
        char buffer[100];
        while(fgets(buffer, sizeof(buffer), commit_info)) {
            char* pos = strstr(buffer, ":");
            if(!strncmp(buffer, "message", pos-buffer)) {
                sprintf(message, "%s", pos+1);
                if(message[strlen(message)-1] == '\n') message[strlen(message)-1] = 0;
            }
            if(!strncmp(buffer, "date-time", pos-buffer)) {
                sprintf(tyme, "%s", pos+1);
                if(tyme[strlen(tyme)-1] == '\n') tyme[strlen(tyme)-1] = 0;
            }
            if(!strncmp(buffer, "id", pos-buffer)) {
                sprintf(id, "%s", pos+1);
                if(id[strlen(id)-1] == '\n') id[strlen(id)-1] = 0;
                sprintf(branch, "%s", BRANCHES[100*(id[0]-'0') + 10*(id[1]-'0') + (id[2]-'0')]);
            }
            if(!strncmp(buffer, "author", pos-buffer)) {
                sprintf(author, "%s", pos+1);
                if(author[strlen(author)-1] == '\n') author[strlen(author)-1] = 0;
            }
            if(!strncmp(buffer, "files commited", pos-buffer)) {
                sprintf(cnt, "%s", pos+1);
                if(cnt[strlen(cnt)-1] == '\n') cnt[strlen(cnt)-1] = 0;
            }
        }
        fclose(commit_info);
        char author_name[50];
        sscanf(author, "%s", author_name);
        if(strcmp(author_name, name)) continue;

        printf("commit with commit id: %s at %s\n", id, tyme);
        printf("\tauthor: %s\n", author);
        printf("\tmessage: %s\n", message);
        printf("\ton branch: %s\n", branch);
        printf("\t%s files commited.\n", cnt);
        printf("***************************************\n");
    }

}

void search_log(int n, char words_list[10][20]) {

    char commits_folder[500] = ""; get_commits_folder(commits_folder);
    int last_commit = get_last_commit();
    if(last_commit == 0) {
        printf("there is no commits yet.\n");
        return;
    }

    for(int i = last_commit ; i > 0 ; i--) {
        char num[15] = ""; sprintf(num, "%d/info.txt", i);
        char commit_info_path[500]; strcpy(commit_info_path, commits_folder); strcat(commit_info_path, num);
        FILE* commit_info = fopen(commit_info_path, "r");
        char id[50], message[50], tyme[50], author[50], branch[50], cnt[50];
        char buffer[100];
        while(fgets(buffer, sizeof(buffer), commit_info)) {
            char* pos = strstr(buffer, ":");
            if(!strncmp(buffer, "message", pos-buffer)) {
                sprintf(message, "%s", pos+1);
                if(message[strlen(message)-1] == '\n') message[strlen(message)-1] = 0;
            }
            if(!strncmp(buffer, "date-time", pos-buffer)) {
                sprintf(tyme, "%s", pos+1);
                if(tyme[strlen(tyme)-1] == '\n') tyme[strlen(tyme)-1] = 0;
            }
            if(!strncmp(buffer, "id", pos-buffer)) {
                sprintf(id, "%s", pos+1);
                if(id[strlen(id)-1] == '\n') id[strlen(id)-1] = 0;
                sprintf(branch, "%s", BRANCHES[100*(id[0]-'0') + 10*(id[1]-'0') + (id[2]-'0')]);
            }
            if(!strncmp(buffer, "author", pos-buffer)) {
                sprintf(author, "%s", pos+1);
                if(author[strlen(author)-1] == '\n') author[strlen(author)-1] = 0;
            }
            if(!strncmp(buffer, "files commited", pos-buffer)) {
                sprintf(cnt, "%s", pos+1);
                if(cnt[strlen(cnt)-1] == '\n') cnt[strlen(cnt)-1] = 0;
            }
        }
        fclose(commit_info);

        int flag = 0;
        for(int i = 0 ; i < n ; i++) {
            if(strstr(message, words_list[i]) != NULL) {
                flag = 1; break;
            }
        }
        if(!flag) continue;

        printf("commit with commit id: %s at %s\n", id, tyme);
        printf("\tauthor: %s\n", author);
        printf("\tmessage: %s\n", message);
        printf("\ton branch: %s\n", branch);
        printf("\t%s files commited.\n", cnt);
        printf("***************************************\n");
    }
}

void since_log(char* date) {
    char commits_folder[500] = ""; get_commits_folder(commits_folder);
    int last_commit = get_last_commit();
    if(last_commit == 0) {
        printf("there is no commits yet.\n");
        return;
    }

    for(int i = last_commit ; i > 0 ; i--) {
        char num[15] = ""; sprintf(num, "%d/info.txt", i);
        char commit_info_path[500]; strcpy(commit_info_path, commits_folder); strcat(commit_info_path, num);
        FILE* commit_info = fopen(commit_info_path, "r");
        char id[50], message[50], tyme[50], author[50], branch[50], cnt[50];
        char buffer[100];
        while(fgets(buffer, sizeof(buffer), commit_info)) {
            char* pos = strstr(buffer, ":");
            if(!strncmp(buffer, "message", pos-buffer)) {
                sprintf(message, "%s", pos+1);
                if(message[strlen(message)-1] == '\n') message[strlen(message)-1] = 0;
            }
            if(!strncmp(buffer, "date-time", pos-buffer)) {
                sprintf(tyme, "%s", pos+1);
                if(tyme[strlen(tyme)-1] == '\n') tyme[strlen(tyme)-1] = 0;
            }
            if(!strncmp(buffer, "id", pos-buffer)) {
                sprintf(id, "%s", pos+1);
                if(id[strlen(id)-1] == '\n') id[strlen(id)-1] = 0;
                sprintf(branch, "%s", BRANCHES[100*(id[0]-'0') + 10*(id[1]-'0') + (id[2]-'0')]);
            }
            if(!strncmp(buffer, "author", pos-buffer)) {
                sprintf(author, "%s", pos+1);
                if(author[strlen(author)-1] == '\n') author[strlen(author)-1] = 0;
            }
            if(!strncmp(buffer, "files commited", pos-buffer)) {
                sprintf(cnt, "%s", pos+1);
                if(cnt[strlen(cnt)-1] == '\n') cnt[strlen(cnt)-1] = 0;
            }
        }
        fclose(commit_info);

        int Y, M, D, y, m, d;
        sscanf(date, "%d.%d.%d", &Y, &M, &D);
        char commit_date[30]; sscanf(tyme, "%s", commit_date);
        sscanf(commit_date, "%d.%d.%d", &y, &m, &d);

        if(y < Y) continue;
        if(y == Y && m < M) continue;
        if(y == Y && m == M && d < D) continue;

        printf("commit with commit id: %s at %s\n", id, tyme);
        printf("\tauthor: %s\n", author);
        printf("\tmessage: %s\n", message);
        printf("\ton branch: %s\n", branch);
        printf("\t%s files commited.\n", cnt);
        printf("***************************************\n");
    }
}

void before_log(char* date) {
    char commits_folder[500] = ""; get_commits_folder(commits_folder);
    int last_commit = get_last_commit();
    if(last_commit == 0) {
        printf("there is no commits yet.\n");
        return;
    }

    for(int i = last_commit ; i > 0 ; i--) {
        char num[15] = ""; sprintf(num, "%d/info.txt", i);
        char commit_info_path[500]; strcpy(commit_info_path, commits_folder); strcat(commit_info_path, num);
        FILE* commit_info = fopen(commit_info_path, "r");
        char id[50], message[50], tyme[50], author[50], branch[50], cnt[50];
        char buffer[100];
        while(fgets(buffer, sizeof(buffer), commit_info)) {
            char* pos = strstr(buffer, ":");
            if(!strncmp(buffer, "message", pos-buffer)) {
                sprintf(message, "%s", pos+1);
                if(message[strlen(message)-1] == '\n') message[strlen(message)-1] = 0;
            }
            if(!strncmp(buffer, "date-time", pos-buffer)) {
                sprintf(tyme, "%s", pos+1);
                if(tyme[strlen(tyme)-1] == '\n') tyme[strlen(tyme)-1] = 0;
            }
            if(!strncmp(buffer, "id", pos-buffer)) {
                sprintf(id, "%s", pos+1);
                if(id[strlen(id)-1] == '\n') id[strlen(id)-1] = 0;
                sprintf(branch, "%s", BRANCHES[100*(id[0]-'0') + 10*(id[1]-'0') + (id[2]-'0')]);
            }
            if(!strncmp(buffer, "author", pos-buffer)) {
                sprintf(author, "%s", pos+1);
                if(author[strlen(author)-1] == '\n') author[strlen(author)-1] = 0;
            }
            if(!strncmp(buffer, "files commited", pos-buffer)) {
                sprintf(cnt, "%s", pos+1);
                if(cnt[strlen(cnt)-1] == '\n') cnt[strlen(cnt)-1] = 0;
            }
        }
        fclose(commit_info);

        int Y, M, D, y, m, d;
        sscanf(date, "%d.%d.%d", &Y, &M, &D);
        char commit_date[30]; sscanf(tyme, "%s", commit_date);
        sscanf(commit_date, "%d.%d.%d", &y, &m, &d);

        if(y > Y) continue;
        if(y == Y && m > M) continue;
        if(y == Y && m == M && d > D) continue;

        printf("commit with commit id: %s at %s\n", id, tyme);
        printf("\tauthor: %s\n", author);
        printf("\tmessage: %s\n", message);
        printf("\ton branch: %s\n", branch);
        printf("\t%s files commited.\n", cnt);
        printf("***************************************\n");
    }
}

void add_branch(char* name) {

    char branches[500] = ""; get_vics_folder(branches); strcat(branches, "/");
    strcat(branches, "branches.txt");
    FILE* branch_list = fopen(branches, "a");
    fprintf(branch_list, "%s:%d\n", name, cur_commit);
    fclose(branch_list);
    strcpy(BRANCHES[branch_count++], name);

    printf("new branch created successfully.\n");
}

int recursive_compare(char* lhs, char* rhs) {
    if(is_file(lhs) == 1) {
        return check_equality(lhs, rhs);
    }
    strcat(lhs, "/");
    strcat(rhs, "/");

    DIR* dir = opendir(lhs);
    struct dirent* entry;
    while(entry = readdir(dir)) {
        if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) continue;
        strcat(lhs, entry->d_name);
        strcat(rhs, entry->d_name);
        if(!recursive_compare(lhs, rhs)) return 0;
        for(int i = strlen(lhs)-1 ; i >= 0 ; i--) if(lhs[i] == '/' || lhs[i] == '\\') {
            lhs[i+1] = 0; break;
        }
        for(int i = strlen(rhs)-1 ; i >= 0 ; i--) if(rhs[i] == '/' || rhs[i] == '\\') {
            rhs[i+1] = 0; break;
        }
    }
    closedir(dir);
    lhs[strlen(lhs)-1] = 0;
    rhs[strlen(rhs)-1] = 0;
    return 1;
}

void recursive_clear(char* path) {
    if(is_file(path) == 1) {
        remove(path); return;
    }
    strcat(path, "/");
    DIR* dir = opendir(path);
    struct dirent* entry;
    while(entry = readdir(dir)) {
        if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) continue;
        strcat(path, entry->d_name);
        recursive_clear(path);
        for(int i = strlen(path)-1 ; i >= 0 ; i--) if(path[i] == '/' || path[i] == '\\') {
            path[i+1] = 0; break;
        }
    }
    closedir(dir);
    path[strlen(path)-1] = 0;
    rmdir(path);
}

int checkout_commit(char* commit_id) {
    if(!strcmp(commit_id, "HEAD")) {

        int head = find_head(cur_branch);
        char res[20]; sprintf(res, "%03d%03d", cur_branch, head);
        return checkout_commit(res);
    }


    int commit_branch = 100*(commit_id[0]-'0') + 10*(commit_id[1]-'0') + (commit_id[2]-'0');
    int commit_number = 100*(commit_id[3]-'0') + 10*(commit_id[4]-'0') + (commit_id[5]-'0');
    char cur_path[500]; getcwd(cur_path, sizeof(cur_path));

    char tmp[500] = ""; get_vics_folder(tmp);
    while(tmp[0] == '.' && tmp[1] == '.') {
        chdir("..");
        strcpy(tmp, ""); get_vics_folder(tmp);
    }

    DIR* root = opendir(".");
    char rhs[500]; sprintf(rhs, ".vics/commits/%d/contents/", cur_commit);
    char lhs[500] = "";
    struct dirent* entry;
    int no_change = 1;
    while(entry = readdir(root)) {
        if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..") || !strcmp(entry->d_name, ".vics")) continue;

        sprintf(lhs, "%s", entry->d_name);
        strcat(rhs, entry->d_name);
        if(!recursive_compare(lhs, rhs)) {
            no_change = 0; break;
        }
        for(int i = strlen(rhs)-1 ; i >= 0 ; i--) if(rhs[i] == '/' || rhs[i] == '\\') {
            rhs[i+1] = 0; break;
        }
    }

    if(!no_change && find_head(cur_branch) == cur_commit) {
        printf("checkout is not allowed: there exists changes that are not commited yet.\n");
        return 1;
    }
    closedir(root);

    root = opendir(".");
    char file_path[500] = "";
    while(entry = readdir(root)) {
        if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..") || !strcmp(entry->d_name, ".vics")) continue;
        
        strcat(file_path, entry->d_name);
        if(is_file(file_path) == 1) {
            remove(file_path);
        } else if(is_file(file_path) == 0) {
            recursive_clear(file_path);
        }
        strcpy(file_path, "");

    }
    closedir(root);

    char to_be_copied[500]; sprintf(to_be_copied, ".vics/commits/%d/contents/", commit_number);
    DIR* src = opendir(to_be_copied);
    while(entry = readdir(src)) {
        if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) continue;

        strcat(to_be_copied, entry->d_name);
        recursive_copy(to_be_copied);
        for(int i = strlen(to_be_copied)-1 ; i >= 0 ; i--) if(to_be_copied[i] == '/' || to_be_copied[i] == '\\') {
            to_be_copied[i+1] = 0; break;
        }
    }
    closedir(src);

    cur_branch = commit_branch;
    cur_commit = commit_number;
    FILE* curs = fopen(".vics/currents.txt", "w");
    fprintf(curs, "commit:%d\n", cur_commit);
    fprintf(curs, "branch:%d\n", cur_branch);
    fclose(curs);

    chdir(cur_path);
    return 0;
}

int checkout_branch(char* branch_name) {

    char commits_folder[500] = ""; get_commits_folder(commits_folder);
    DIR* commits = opendir(commits_folder);

    struct dirent* entry;
    int head = -1;
    while(entry = readdir(commits)) {
        if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) continue;
        if(is_file(entry->d_name) == 1) continue;

        char commit_info_path[500]; strcpy(commit_info_path, commits_folder); strcat(commit_info_path, entry->d_name); strcat(commit_info_path, "/info.txt");
        FILE* info = fopen(commit_info_path, "r");
        if(info == NULL) continue;
        char commit_id[10], buffer[100];
        while(fgets(buffer, sizeof(buffer), info)) {
            int pos = strstr(buffer, ":") - buffer;
            if(!strncmp(buffer, "id", pos)) {
                strcpy(commit_id, buffer + pos + 1); break;
            }
        }
        fclose(info);

        int commit_branch = 100*(commit_id[0]-'0') + 10*(commit_id[1]-'0') + (commit_id[2]-'0');
        if(!strcmp(BRANCHES[commit_branch], branch_name)) {
            int num; sscanf(entry->d_name, "%d", &num);
            if(num > head) head = num;
        }
    }
    closedir(commits);

    int branch_idx;
    for(int i = 0 ; i < branch_count ; i++) {
        if(!strcmp(BRANCHES[i], branch_name)) {
            branch_idx = i; break;
        }
    }

    if(head == -1) {
        char branch_list[500] = ""; get_vics_folder(branch_list);
        strcat(branch_list, "/branches.txt");
        FILE* branches = fopen(branch_list, "r");
        char buffer[70];
        int par_cmt = 0;
        while(fgets(buffer, sizeof(buffer), branches)) {
            char* pos = strstr(buffer, ":");
            if(!strncmp(buffer, branch_name, pos-buffer)) {
                sscanf(pos+1, "%d", &par_cmt); 
                break;
            }
        }
        fclose(branches);
        cur_commit = par_cmt;
        cur_branch = branch_idx;
        char curs[500] = ""; get_vics_folder(curs);
        strcat(curs, "/currents.txt");
        FILE* currents = fopen(curs, "w");
        fprintf(currents, "commit:%d\n", cur_commit);
        fprintf(currents, "branch:%d\n", cur_branch);
        fclose(currents);
        return 0;
    }

    char res[10];
    sprintf(res, "%03d%03d", branch_idx, head);
    return checkout_commit(res);
}

void show_file_status(char* file) {
    char file_name[100] = ""; extract_file_name(file_name, file);

    char file_in_commit[500] = ""; sprintf(file_in_commit, ".vics/commits/%d/contents/%s", cur_commit, file);
    if(check_equality(file, file_in_commit)) return;

    printf("%s: ", file);

    if(is_file(file_in_commit) == -1) printf("A");
    else printf("M");

    char file_in_stage[500]; sprintf(file_in_stage, ".vics/staging/%s", file_name);
    if(check_equality(file, file_in_stage)) printf("+\n");
    else printf("-\n");
}

void recursively_show_status(char* path) {
    if(is_file(path) == 1) {
        show_file_status(path);
        return;
    }

    strcat(path, "/");
    DIR* dir = opendir(path);

    struct dirent* entry;
    while(entry = readdir(dir)) {
        if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) continue;
        strcat(path, entry->d_name);
        recursively_show_status(path);
        for(int i = strlen(path)-1 ; i >= 0 ; i--) if(path[i] == '/' || path[i] == '\\') {
            path[i+1] = 0; break;
        }
    }

    closedir(dir);

    path[strlen(path)-1] = 0;
}

void show_status() {
    char cur_path[500]; getcwd(cur_path, sizeof(cur_path));
    char tmp[500] = ""; get_vics_folder(tmp);
    while(tmp[0] == '.' && tmp[1] == '.') {
        chdir("..");
        strcpy(tmp, ""); get_vics_folder(tmp);
    }

    char file_path[500] = "";
    DIR* root = opendir(".");

    struct dirent* entry;
    while(entry = readdir(root)) {
        if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..") || !strcmp(entry->d_name, ".vics")) continue;

        strcat(file_path, entry->d_name);
        recursively_show_status(file_path);
        strcpy(file_path, "");
    }

    closedir(root);

    chdir(cur_path);
}

int run_alias(char* command) {
    char alias_path[500] = ""; get_vics_folder(alias_path); strcat(alias_path, "/aliases.txt");
    FILE* aliases = fopen(alias_path, "r");
    if(aliases == NULL) aliases = fopen("D:/uni/fop/project/ViCS/global/aliases.txt", "r");
    if(aliases == NULL) {
        return 0;
    }

    char tmp[100]; strcpy(tmp, "alias.");
    strcat(tmp, command);

    char buffer[100];
    int success = 0;
    while(fgets(buffer, sizeof(buffer), aliases)) {
        int pos = strstr(buffer, ":") - buffer;
        if(!strncmp(buffer, tmp, pos)) {
            char alias_value[100];
            strcpy(alias_value, buffer+pos+1);
            system(alias_value);
            success = 1;
            break;
        }
    }
    fclose(aliases);
    if(success) return 1;

    aliases = fopen("D:/uni/fop/project/ViCS/global/aliases.txt", "r");
    if(aliases == NULL) return 0;
    while(fgets(buffer, sizeof(buffer), aliases)) {
        int pos = strstr(buffer, ":") - buffer;
        if(!strncmp(buffer, tmp, pos)) {
            char alias_value[100];
            strcpy(alias_value, buffer+pos+1);
            system(alias_value);
            success = 1;
            break;
        }
    }
    fclose(aliases);
    
    return success;
}

int create_tag(char* tag_name, char* tag_message, char* commit_id, int overwrite) {
    char tags_folder[500] = ""; get_tags_folder(tags_folder);

    char tag_path[500]; sprintf(tag_path, "%s%s.txt", tags_folder, tag_name);
    if(is_file(tag_path) == 1 && !overwrite) {
        printf("a tag with this name already exists.\n");
        return 1;
    }

    char author_name[50] = ""; get_name(author_name);
    char author_email[50] = ""; get_email(author_email);
    if(!strlen(author_email) || !strlen(author_name)) {
        printf("please set your user name and email with 'vics config' and try again.\n");
        return 1;
    }


    FILE* tag = fopen(tag_path, "w");

    fprintf(tag, "name: %s\n", tag_name);
    fprintf(tag, "commit-id: %s\n", commit_id);
    fprintf(tag, "author: %s - %s\n", author_name, author_email);
    time_t t = time(NULL);
    struct tm tyme = *localtime(&t);
    fprintf(tag, "date-time: %d.%02d.%02d - %02d:%02d:%02d\n", tyme.tm_year + 1900, tyme.tm_mon+1, tyme.tm_mday, tyme.tm_hour, tyme.tm_min, tyme.tm_sec);
    fprintf(tag, "message: %s\n", tag_message);

    fclose(tag);
}

void list_tags() {
    printf("all tags:\n");
    char tags_folder[500] = ""; get_tags_folder(tags_folder);
    DIR* all_tags = opendir(tags_folder);
    struct dirent* entry;
    while(entry = readdir(all_tags)) {
        if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) continue;
        char no_extension[50]; strcpy(no_extension, entry->d_name);
        for(int i = strlen(no_extension)-1 ; i >= 0 ; i--) if(no_extension[i] == '.') no_extension[i] = 0;
        printf("%s\n", no_extension);
    }
    closedir(all_tags);
}

int show_tag(char* tag_name) {
    char tag_path[500] = ""; get_tags_folder(tag_path);
    strcat(tag_path, tag_name); strcat(tag_path, ".txt");

    FILE* tag = fopen(tag_path, "r");
    if(tag == NULL) {
        printf("no such tag.\n");
        return 1;
    }

    char buffer[100];
    while(fgets(buffer, sizeof(buffer), tag)) {
        printf("%s", buffer);
    }

    fclose(tag);
    return 0;
}

int grep(char* file, char* word, char* commit_id, int another_commit, int line_number) {

    char absolute[500]; getcwd(absolute, sizeof(absolute));
    strcat(absolute, "\\"); strcat(absolute, file);
    char relative_path[500] = ""; get_relative_path(absolute, relative_path);

    int commit_number = 100*(commit_id[3]-'0') + 10*(commit_id[4]-'0') + (commit_id[5]-'0');
    char contents_path[500]; sprintf(contents_path, "%d/contents/%s", commit_number, relative_path);
    char file_in_commit[500] = ""; get_commits_folder(file_in_commit);
    strcat(file_in_commit, contents_path);
    
    if(!another_commit) strcpy(file_in_commit, file);

    FILE* target = fopen(file_in_commit, "r");
    if(target == NULL) {
        printf("file path is invalid.\n");
        return 1;
    }

    char buffer[1000];
    int line = 1;
    while(fgets(buffer, sizeof(buffer), target)) {
        if(strstr(buffer, word) != NULL) {
            if(line_number) printf("%3d. ", line);
            printf("%s", buffer);
        }
        line++;
    }

    return 0;
}

int merge(char* branch_name) {
    int branch_idx = -1;
    for(int i = 0 ; i < branch_count ; i++) {
        if(!strcmp(BRANCHES[i], branch_name)) {
            branch_idx = i; break;
        }
    }
    
    int head = find_head(branch_idx);
    if(head == -1) {
        printf("this branch have no commits or doesn't exist.\n");
        return 1;
    }

    char commits_folder[500] = ""; get_commits_folder(commits_folder);
    int last_commit;
    char number_of_commits[500]; sprintf(number_of_commits, "%s/number-of-commits.txt", commits_folder);
    FILE* f = fopen(number_of_commits, "r");
    fscanf(f, "%d", &last_commit);
    fclose(f);

    last_commit++;

    f = fopen(number_of_commits, "w");
    fprintf(f, "%d\n", last_commit);
    fclose(f);

    char curs[500] = ""; get_vics_folder(curs); strcat(curs, "/currents.txt");
    f = fopen(curs, "w");
    fprintf(f, "commit:%d\n", last_commit);
    fprintf(f, "branch:%d\n", cur_branch);
    fclose(f);

    char cur_path[500]; getcwd(cur_path, sizeof(cur_path));

    chdir(commits_folder);

    char new_commit[10]; sprintf(new_commit, "%d", last_commit);
    mkdir(new_commit);
    chdir(new_commit);

    char head_contents[50]; sprintf(head_contents, "%d/contents", head);
    char head_path[500] = ""; get_commits_folder(head_path);
    strcat(head_path, head_contents);

    recursive_copy(head_path);

    chdir("contents");
    char par_commit_path[500]; sprintf(par_commit_path, "../../%d/contents", cur_commit);
    DIR* par_commit = opendir(par_commit_path);
    struct dirent* entry;
    while(entry = readdir(par_commit)) {
        if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) continue;
        char file_path[500]; sprintf(file_path, "../../%d/contents/%s", cur_commit, entry->d_name);
        recursive_copy(file_path);
    }
    closedir(par_commit);
    chdir("..");

    FILE* info = fopen("info.txt", "w");
    fprintf(info, "message:branch %s merged into branch %s\n", branch_name, BRANCHES[cur_branch]);
    time_t t = time(NULL);
    struct tm tyme = *localtime(&t);
    fprintf(info, "date-time:%d.%02d.%02d - %02d:%02d:%02d\n", tyme.tm_year + 1900, tyme.tm_mon+1, tyme.tm_mday, tyme.tm_hour, tyme.tm_min, tyme.tm_sec);
    fprintf(info, "id:%03d%03d\n", cur_branch, last_commit);

    char name[100] = "", email[100] = "";
    get_name(name); get_email(email);
    fprintf(info, "author:%s - %s\n", name, email);
    fprintf(info, "files commited:0");
    fclose(info);

    chdir(cur_path);
    char HEAD[10]; strcpy(HEAD, "HEAD");
    checkout_commit(HEAD);
    return 0;
}

void list_all_hooks() {
    printf("all hooks:\n");

    char hooks_path[500] = ""; get_vics_folder(hooks_path);
    strcat(hooks_path, "/hooks.txt");
    FILE* hooks = fopen(hooks_path, "r");

    char buffer[50];
    while(fgets(buffer, sizeof(buffer), hooks)) {
        int pos = strstr(buffer, ":") - buffer;
        buffer[pos] = 0;
        printf("%s\n", buffer);
    }

    fclose(hooks);
}

void list_applied_hooks() {
    printf("applied hooks:\n");

    char hooks_path[500] = ""; get_vics_folder(hooks_path);
    strcat(hooks_path, "/hooks.txt");
    FILE* hooks = fopen(hooks_path, "r");

    char buffer[50];
    while(fgets(buffer, sizeof(buffer), hooks)) {
        int pos = strstr(buffer, ":") - buffer;
        int applied; sscanf(buffer+pos+1, "%d", &applied);
        buffer[pos] = 0;
        if(applied) printf("%s\n", buffer);
    }

    fclose(hooks);
}

void add_hook(char* hook) {
    char hooks_path[500] = ""; get_vics_folder(hooks_path);
    char tmp_path[500] = ""; get_vics_folder(tmp_path);
    strcat(hooks_path, "/hooks.txt");
    strcat(tmp_path, "/tmp.txt");
    FILE* hooks = fopen(hooks_path, "r");
    FILE* tmp = fopen(tmp_path, "w");

    char buffer[50];
    while(fgets(buffer, sizeof(buffer), hooks)) {
        int pos = strstr(buffer, ":") - buffer;
        if(!strncmp(buffer, hook, pos)) {
            fprintf(tmp, "%s:1\n", hook);
        } else {
            fprintf(tmp, "%s", buffer);
        }
    }
    fclose(tmp);
    fclose(hooks);

    tmp = fopen(tmp_path, "r");
    hooks = fopen(hooks_path, "w");

    while(fgets(buffer, sizeof(buffer), tmp)) {
        fprintf(hooks, "%s", buffer);
    }
    fclose(tmp); fclose(hooks);
    remove(tmp_path);

    printf("hook applied successfully.\n");
} 

void remove_hook(char* hook) {
    char hooks_path[500] = ""; get_vics_folder(hooks_path);
    char tmp_path[500] = ""; get_vics_folder(tmp_path);
    strcat(hooks_path, "/hooks.txt");
    strcat(tmp_path, "/tmp.txt");
    FILE* hooks = fopen(hooks_path, "r");
    FILE* tmp = fopen(tmp_path, "w");

    char buffer[50];
    while(fgets(buffer, sizeof(buffer), hooks)) {
        int pos = strstr(buffer, ":") - buffer;
        if(!strncmp(buffer, hook, pos)) {
            fprintf(tmp, "%s:0\n", hook);
        } else {
            fprintf(tmp, "%s", buffer);
        }
    }
    fclose(tmp);
    fclose(hooks);

    tmp = fopen(tmp_path, "r");
    hooks = fopen(hooks_path, "w");

    while(fgets(buffer, sizeof(buffer), tmp)) {
        fprintf(hooks, "%s", buffer);
    }
    fclose(tmp); fclose(hooks);
    remove(tmp_path);

    printf("hook removed successfully.\n");
}

int hook_char_limit(char* file_path) {
    char file_name[30] = ""; extract_file_name(file_name, file_path);
    char* pos = strstr(file_name, ".");
    if(pos == NULL || (strcmp(pos, ".txt") && strcmp(pos, ".c") && strcmp(pos, ".cpp"))) {
        return -1;
    }
    FILE* file = fopen(file_path, "r");
    int cnt = 0;
    char c;
    do {
        c = fgetc(file);
        cnt++;
    } while(c != EOF);
    fclose(file);
    return (cnt <= 20000);
}

int balance_braces(char* file_path) {
    char file_name[30] = ""; extract_file_name(file_name, file_path);
    char* pos = strstr(file_name, ".");
    if(pos == NULL || (strcmp(pos, ".txt") && strcmp(pos, ".c") && strcmp(pos, ".cpp"))) {
        return -1;
    }
    FILE* file = fopen(file_path, "r");
    int a = 0, b = 0, c = 0;
    char ch;
    int res = 1;
    do {
        ch = fgetc(file);
        if(ch == '[') a++;
        if(ch == ']') a++;
        if(ch == '{') b--;
        if(ch == '}') b--;
        if(ch == '(') c--;
        if(ch == ')') c--;

        if(a < 0 || b < 0 || c < 0) {
            res = 0; break;
        }

    } while(ch != EOF);
    fclose(file);

    if(a > 0 || b > 0 || c > 0) res = 0;
    return res;
}

int format_check(char* file_path) {
    char file_name[30] = ""; extract_file_name(file_name, file_path);
    char* pos = strstr(file_name, ".");
    if(pos == NULL || (strcmp(pos, ".txt") && strcmp(pos, ".c") && strcmp(pos, ".cpp")) && strcmp(pos, ".py") && strcmp(pos, ".html") && strcmp(pos, ".css") && strcmp(pos, ".js") && strcmp(pos, ".tex")) {
        return 0;
    }
    return 1;
}

int todo_check(char* file_path) {
    char file_name[30] = ""; extract_file_name(file_name, file_path);
    char* pos = strstr(file_name, ".");
    if(pos == NULL || (strcmp(pos, ".txt") && strcmp(pos, ".c") && strcmp(pos, ".cpp"))) {
        return -1;
    }
    FILE* file = fopen(file_path, "r");

    char buffer[1000];
    int res = 1;
    while(fgets(buffer, sizeof(buffer), file)) {
        if(!strcmp(pos, ".txt")) {
            if(strstr(buffer, "TODO") != NULL) {
                res = 0; break;
            }
        } else {
            if(strstr(buffer, "//TODO") != NULL || strstr(buffer, "// TODO") != NULL) {
                res = 0; break;
            }
        }
    }

    fclose(file);
    return res;
}

int pre_commit() {

    int final_result = 1;
    char staging_path[500] = ""; get_staging_folder(staging_path);
    DIR* staging_area = opendir(staging_path);
    if(staging_area == NULL) printf("%s\n", staging_path);
    struct dirent* entry;
    while(entry = readdir(staging_area)) {
        char file_path[500]; sprintf(file_path, "%s%s", staging_path, entry->d_name);
        if(is_file(file_path) == 1) {
            printf("%s:\n", entry->d_name);

            char hooks_path[500] = ""; get_vics_folder(hooks_path);
            strcat(hooks_path, "/hooks.txt");
            FILE* hooks = fopen(hooks_path, "r");

            char buffer[50];
            while(fgets(buffer, sizeof(buffer), hooks)) {
                char* pos = strstr(buffer, ":");
                char hook_id[30]; strncpy(hook_id, buffer, pos-buffer); hook_id[pos-buffer] = 0;
                int apply; sscanf(pos+1, "%d", &apply);
                if(!apply) continue;
                if(!strcmp(hook_id, "character-limit")) {
                    printf("%s", hook_id);
                    for(int i = 0 ; i < 40-strlen(hook_id) ; i++) printf(".");
                    int res = hook_char_limit(file_path);
                    if(res == 1) printf("PASSED\n");
                    if(res == -1) printf("SKIPPED\n");
                    if(res == 0) {
                        printf("FAILED\n");
                        final_result = 0;
                    }
                }
                if(!strcmp(hook_id, "balance-braces")) {
                    printf("%s", hook_id);
                    for(int i = 0 ; i < 40-strlen(hook_id) ; i++) printf(".");
                    int res = balance_braces(file_path);
                    if(res == 1) printf("PASSED\n");
                    if(res == -1) printf("SKIPPED\n");
                    if(res == 0) {
                        printf("FAILED\n");
                        final_result = 0;
                    }
                }
                if(!strcmp(hook_id, "format-check")) {
                    printf("%s", hook_id);
                    for(int i = 0 ; i < 40-strlen(hook_id) ; i++) printf(".");
                    int res = format_check(file_path);
                    if(res == 1) printf("PASSED\n");
                    if(res == -1) printf("SKIPPED\n");
                    if(res == 0) {
                        printf("FAILED\n");
                        final_result = 0;
                    }
                }
                if(!strcmp(hook_id, "todo-check")) {
                    printf("%s", hook_id);
                    for(int i = 0 ; i < 40-strlen(hook_id) ; i++) printf(".");
                    int res = todo_check(file_path);
                    if(res == 1) printf("PASSED\n");
                    if(res == -1) printf("SKIPPED\n");
                    if(res == 0) {
                        printf("FAILED\n");
                        final_result = 0;
                    }
                }
            }

            fclose(hooks);
        }
    }

    closedir(staging_area);
    return final_result;
}