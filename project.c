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

void tcreatefile(char name[]){
    int n = strlen(name);
    if(name[0] == '"'){
        n-=2;
        for(int i=0;i<n;i++)
            name[i]=name[i+1];
        name[n] = '\0';
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
        printf("File already exist\n");
    else{
        FILE* file;
        file = fopen(name , "w");
        fclose(file);
    }
}

char poscheck(char name[] ,int  ncounter){
    char poscheck[100];
    for(int i=0;i<5;i++){
        poscheck[i]=name[ncounter];
        ncounter++;
    }
    poscheck[5]= '\0';
    return !strcmp(poscheck , "--pos");
}

void tinsert(char name[]){
    int n = strlen(name) , ncounter=0 , dcounter = 0 , mcounter = 0;
    char direction[100];
    while(name[ncounter] != '-'){
        direction[dcounter]=name[ncounter];
        ncounter++;
        dcounter++;
    }
    direction[dcounter-1]='\0';
    if(direction[0] == '"'){
        dcounter-=3;
        for(int i=0;i<n;i++)
            direction[i]=direction[i+1];
        direction[dcounter] = '\0';
    }
    char matn[100] ,checker[100];
    int ccounter=0;
    while(name[ncounter+ccounter] != 32){
        checker[ccounter]=name[ncounter+ccounter];
        ccounter++;
    }
    checker[ccounter]='\0';
    if(strcmp(checker , "--str")){
        printf("Invalid Input\n");
        return;
    }
    checker[6]='\0';
    if(!check_fileex(direction)){
        printf("This file doesn't exist\n");
        return;
    }
    ncounter+=6;
    int check =0;
    int mstart = ncounter , mend;
    while(ncounter < n-6){
        if(poscheck(name , ncounter)){
            check++;
            break;
        }
        ncounter++;
    }
    if (check == 0){
        printf("Invalid Input\n");
        return;
    }
    mend=ncounter-2;
    for(int i=0; i<mend-mstart+1;i++){
        matn[i]=name[mstart+i];
    }
    matn[mend-mstart+1]='\0';
    if(matn[0] == '"'){
        for(int i=0;i<mend-mstart+1;i++){
            matn[i]=matn[i+1];
        }
        matn[mend-mstart-1]= '\0';
    }
    ncounter+=6;
    int khat=0 , ja=0 ,donogh =0;
    while(ncounter < n){
        if(name[ncounter] == ':'){
            donogh=1;
        }
        else if(donogh == 0){
            khat+=name[ncounter] - 48;
            khat*=10;
        }
        else if(donogh == 1){
            ja+=name[ncounter] -48;
            ja*=10;
        }
        ncounter++;
    }
    if(donogh == 0){
        printf("Invalid Input\n");
        return;
    }
    ja/=10;
    khat/=10;
    FILE* file1 = fopen(direction , "r");
    FILE* file2 = fopen("root/mine/helper.txt" , "w");
    char vorodi[100];
    for(int i=0;i<khat-1;i++){
        fgets(vorodi , 100 , file1);
        fprintf(file2 , "%s" , vorodi);
    }
    char c;
    for(int i=0;i<ja;i++){
        c=fgetc(file1);
        fprintf(file2 , "%c" , c);
    }
    int h=0;
    for(int i=0;i<100;i++){
        h=0;
        if(matn[i] == '\0')
            break;
        if(matn[i] == '\\' && matn[i+1] == 'n'){
            fputc('\n' , file2);
            h=1;

        }
        else if(matn[i] == '\\' && matn[i+1] == '\\' &&matn[i+2] == 'n'){
            fputc('\\' , file2);
            fputc('n' , file2);
            h=2;
        }
        else if(matn[i] == '\\' && matn[i+1] == '"'){
            fputc('"' , file2);
            h=1;
        }
        else if(matn[i] == '\\' && matn[i+1] == '\\' && matn[i+2] == '"'){
            fputc('\\' , file2);
            fputc('"' , file2);
            h=2;
        }
        else{
            fputc(matn[i] , file2);
        }
        if(h==1)
            i++;
        if(h==2)
            i+=2;
    }
    while(fgets(vorodi , 100 , file1) != NULL){
        fprintf(file2 , "%s" , vorodi);
    }
    fclose(file2);
    fclose(file1);
    file1=fopen(direction , "w");
    file2=fopen("root/mine/helper.txt" , "r");
    while(fgets(vorodi , 100 , file2) != NULL){
        fprintf(file1 , "%s" , vorodi);
    }
    fclose(file2);
    fclose(file1);
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
                tcreatefile(vo);
            }
        }
        else if(!strcmp(vo , "insertstr")){
            getchar();
            scanf("%s",vo);
            if(strcmp(vo , "--file")){
                fgets(vo,100,stdin);
                printf("Invalid Input\n");
            }
            else{
                fgets(vo,100,stdin);
                vo[strlen(vo)-1] = '\0';
                for(int i=0;i<strlen(vo)-1;i++)
                    vo[i]=vo[i+1];
                vo[strlen(vo)-1] = '\0';
                tinsert(vo);
            }
        }
        else{
            printf("Invalid Input\n");
            fgets(vo,100,stdin);
        }
    }
    return 0;
}
