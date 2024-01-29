// Mohammad Fallahinezhad
// 402106304
#include<stdio.h>
#include<dirent.h>

char* COMMANDS[] = {"config", "init", "add", "reset", "status", "commit", 
                    "log", "branch", "checkout", "revert", "tag", "tree", 
                    "stash", "pre-commit", "grep", "diff", "merge"};
int COMMANDS_COUNT = 17;