#include <stdio.h>
#include <math.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>

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

void createfile(char name[]){
    int n = strlen(name) , counter=0;
    if(name[0] == '"'){
        printf("%s %d\n",name,n);
        n-=2;
        for(int i=0;i<n;i++)
            name[i]=name[i+1];
        name[n] = '\0';
        printf("%s %d\n",name,n);
    }
    char direction[100] ;
    for(int i=0;i<n;i++){
        if(name[i] != '/'){
            direction[i]=name[i];
        }
        else{
            direction[i] = '\0';
            if(check_direx(direction)){
                direction[i] = '/';
            }
            else{
                mkdir(direction);
                direction[i] = '/';
            }
        }
    }
    if(check_fileex(name))
        printf("File already exists\n");
    else{
        FILE* file;
        file = fopen(name , "w");
        fclose(file);
    }
}


int main(){
    char vo[100] , c;
    while(1){
        scanf("%s",vo);
        if(!strcmp(vo , "createfile")){
            getchar();
            scanf("%s",vo);
            if(strcmp(vo,"--file")){
                fgets(vo,100,stdin);
                printf("Invalid Input\n");
            }
            else{
                fgets(vo,100,stdin);
                vo[strlen(vo)-1] = '\0';
                for(int i=0;i<strlen(vo)-1;i++)
                    vo[i]=vo[i+1];
                vo[strlen(vo)-1] = '\0';
                createfile(vo);
            }
        }
        else{
            printf("Invalid Input\n");
            fgets(vo,100,stdin);
        }
    }
    return 0;
}







