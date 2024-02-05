// Mohammad Fallahinezhad
// 402106304
#include<stdio.h>
#include<dirent.h>
#include<string.h>

char* COMMANDS[] = {"config", "init", "add", "reset", "status", "commit", 
                    "log", "branch", "checkout", "revert", "tag", "tree", 
                    "stash", "pre-commit", "grep", "diff", "merge"};
int COMMANDS_COUNT = 17;

// char* BRANCHES[1000] = {"master"}; // TODO: correct it :((
// int branch_count = 1, cur_branch = 0;
char BRANCHES[1000][20];
int branch_count = 0, cur_branch = 0;