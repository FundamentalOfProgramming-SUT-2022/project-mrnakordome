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

void tremovetstr(char direction[],int line,int place,int dsize,char dmode){
    char matn[100] ,matnkol[10000] ,v;
    int tchar=0 , mcounter=0;
    if(check_fileex(direction)){
        FILE* file1 = fopen(direction , "r");
        FILE* file2 = fopen("root/mine/helper.txt" , "w");
        if(dmode == 'f'){
            for(int i=0;i<line-1;i++){
                fgets(matn , 100 ,file1);
                fprintf(file2, "%s" , matn);
            }
            for(int i=0;i<place-1;i++){
                v=fgetc(file1);
                fputc(v,file2);
            }
            for(int i=0;i<dsize;i++)
                v=fgetc(file1);
            while((fgets(matn , 100 , file1)) != NULL)
                fprintf(file2 , "%s" ,matn);
            fclose(file1);
            fclose(file2);
            file1 = fopen(direction , "w");
            file2 = fopen("root/mine/helper.txt" , "r");
            while((fgets(matn , 100 , file2)) != NULL)
                fprintf(file1 , "%s" ,matn);
            fclose(file1);
            fclose(file2);
        }
        else{
            FILE* file1 = fopen(direction , "r");
            for(int i=0;i<line-1;i++){
                fgets(matn , 100 ,file1);
                tchar+=strlen(matn);
            }
            tchar+=place;
            fclose(file1);
            file1 = fopen(direction , "r");
            while((fgets(matn , 100 , file1)) != NULL){
                strcat(matnkol , matn);
            }
            fclose(file1);
            file1 = fopen(direction , "w");
            mcounter=0;
            for(int i=0;i<tchar-dsize;i++){
                fputc(matnkol[mcounter] , file1);
                mcounter++;
            }
            mcounter=tchar;
            while(matnkol[mcounter] != '\0'){
                fputc(matnkol[mcounter] , file1);
                mcounter++;
            }
            fclose(file1);
        }
    }
    else
        printf("This file doesn't exist\n");
}

void tcopystr(char direction[] , int line , int place , int csize , char mode){
    char matn[100] , v;
    int tchar=0;
    FILE* file2 = fopen("root/mine/clipboard.txt" , "w");
    if(!check_fileex(direction)){
        printf("This file doesn't exist\n");
    }
    else{
        FILE* file1 = fopen(direction , "r");
        if(mode == 'f'){
            for (int i=0;i<line-2;i++){
                fgets(matn , 100 , file1);
            }
            for(int i=0;i<place-1;i++){
                fgetc(file1);
            }
            for(int i=0;i<csize;i++){
                v=fgetc(file1);
                fputc(v , file2);
            }
            fclose(file1);
            fclose(file2);
        }
        if(mode == 'b'){
            for(int i=0;i<line-1;i++){
                fgets(matn , 100 , file1);
                tchar+=strlen(matn);
            }
            tchar+=place;
            fclose(file1);
            file1 = fopen(direction , "r");
            for(int i=0;i<tchar-csize;i++){
                fgetc(file1);
            }
            for(int i=0; i< csize ; i++){
                v=fgetc(file1);
                fputc(v , file2);
            }
            fclose(file1);
            fclose(file2);
        }
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

        else if(!strcmp(vo , "removetstr")){
            int line , place , dsize;
            char c;
            getchar();
            scanf("%s",vo);
            if(strcmp(vo,"--file")){
                fgets(vo,100,stdin);
                printf("Invalid Input\n");
            }
            else{
                getchar();
                char direction[100];
                int dcounter=1;
                direction[0]=getchar();
                if(direction[0] == '"'){
                    while((direction[dcounter]=getchar()) != '"')
                        dcounter++;
                    for(int i=0;i<dcounter-1;i++){
                        direction[i]=direction[i+1];
                    }
                    direction[dcounter-1]='\0';
                    getchar();
                    scanf("%s",vo);
                    if(!strcmp(vo , "--pos")){
                        getchar();
                        scanf("%d%c%d",&line,&c,&place);
                        getchar();
                        scanf("%s",vo);
                        if(strcmp(vo , "-size")){
                            fgets(vo,100,stdin);
                            printf("Invalid Input\n");
                        }
                        else{
                            getchar();
                            scanf("%d",&dsize);
                            getchar();
                            scanf("%s",vo);
                            if(!strcmp(vo , "-b"))
                                c='b';
                            else if(!strcmp(vo , "-f"))
                                c='f';
                            else{
                                fgets(vo,100,stdin);
                                printf("Invalid Input\n");
                            }
                        }
                    }
                    else{
                        fgets(vo,100,stdin);
                        printf("Invalid Input\n");
                    }
                }
                else{
                    while((direction[dcounter]=getchar()) != 32)
                        dcounter++;
                    direction[dcounter]='\0';
                    scanf("%s",vo);
                    if(!strcmp(vo , "--pos")){
                        getchar();
                        scanf("%d%c%d",&line,&c,&place);
                        getchar();
                        scanf("%s",vo);
                        if(strcmp(vo , "-size")){
                            fgets(vo,100,stdin);
                            printf("Invalid Input\n");
                        }
                        else{
                            getchar();
                            scanf("%d",&dsize);
                            getchar();
                            scanf("%s",vo);
                            if(!strcmp(vo , "-b"))
                                c='b';
                            else if(!strcmp(vo , "-f"))
                                c='f';
                            else{
                                fgets(vo,100,stdin);
                                printf("Invalid Input\n");
                            }
                        }
                    }
                    else{
                        fgets(vo,100,stdin);
                        printf("Invalid Input\n");
                    }
                }
                tremovetstr(direction,line,place,dsize,c);
            }
        }

        else if(!strcmp(vo , "copystr")){
            int line , place , csize;
            char c;
            getchar();
            scanf("%s",vo);
            if(strcmp(vo , "--file")){
                fgets(vo,100,stdin);
                printf("Invalid Input\n");
            }
            else{
                getchar();
                char direction[100];
                int dcounter=1;
                direction[0]=getchar();
                if(direction[0] == '"'){
                    while((direction[dcounter]=getchar()) != '"')
                        dcounter++;
                    for(int i=0;i<dcounter-1;i++){
                        direction[i]=direction[i+1];
                    }
                    direction[dcounter-1]='\0';
                    getchar();
                }
            else{
                while((direction[dcounter]=getchar()) != 32)
                    dcounter++;
                direction[dcounter]='\0';
            }
            scanf("%s",vo);
            if(strcmp(vo , "--pos")){
                fgets(vo,100,stdin);
                printf("Invalid Input\n");
            }
            else{
                getchar();
                scanf("%d%c%d",&line,&c,&place);
                getchar();
                scanf("%s",vo);
                if(strcmp(vo , "-size")){
                    fgets(vo,100,stdin);
                    printf("Invalid Input\n");
                }
                else{
                    getchar();
                    scanf("%d",&csize);
                    getchar();
                    scanf("%s",vo);
                    if(!strcmp(vo , "-b"))
                        c='b';
                    else if(!strcmp(vo , "-f"))
                        c='f';
                    else{
                        fgets(vo,100,stdin);
                        printf("Invalid Input\n");
                    }
                }
                //printf("direction : %s line : %d place : %d size : %d mode : %c\n",direction,line,place,csize,c);
                tcopystr(direction,line,place,csize,c);
            }
            }
        }
        else{
            printf("Invalid Input\n");
            fgets(vo,100,stdin);
        }
    }
    return 0;
}
