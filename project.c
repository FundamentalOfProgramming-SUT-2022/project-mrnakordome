#include <stdio.h>
#include <math.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdbool.h>

int check_direx(char name[]){
    DIR* dir = opendir(name);
    if(dir){
        closedir(dir);
        return 1;
    }
    else
        return 0;
}

int check_fileex(char name[]){
    FILE* file = fopen(name, "r");
    if(file){
        fclose(file);
        return 1;
    }
    else
        return 0;
}

int main(){

    return 0;
}







