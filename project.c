#include <stdio.h>
#include <math.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>

void basekon(char matn[]){
    for(int i=0;i<strlen(matn);i++){
        matn[i]='\0';
    }
}

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

void rootdo (char direction[]){
    int n=strlen(direction);
    for(int i=0;i<=n;i++){
        direction[i]=direction[i+1];
    }
}

void filenamemaker(char direction[]){
    int s=0;
    char filename[100];
    for(int i=0;i<strlen(direction);i++){
        if(direction[i] == '/')
            s=i+1;
    }
    strcpy(filename , "root/mine/undo/");
    int t=15;
    for(int i=s;i<strlen(direction);i++){
        filename[t] = direction[i];
        t++;
    }
    filename[t]='\0';
    basekon(direction);
    strcpy(direction , filename);
    return;
}

void tcreatefile(char name[]){
    int n = strlen(name);
    if(name[0] == '"'){
        n-=2;
        for(int i=0;i<n;i++)
            name[i]=name[i+1];
        name[n] = '\0';
    }
    rootdo(name);
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
    rootdo(direction);
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

    char fana[100] , fanammatn[200];
    strcpy(fana , direction);
    filenamemaker(fana);
    FILE* file1 = fopen(direction , "r");
    FILE* file3 = fopen(fana , "w");
    while((fgets(fanammatn , 200 , file1)) != NULL)
        fputs(fanammatn , file3);
    fclose(file1);
    fclose(file3);

    file1 = fopen(direction , "r");
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

void tcat(char direction[]){
    if(!check_fileex(direction)){
        printf("This file doesn't exist!\n");
        return;
    }
    else{
        FILE* file1 = fopen(direction , "r");
        char matn[200];
        while((fgets(matn , 200 , file1)) != NULL){
            printf("%s",matn);
        }
        printf("\n");
    }
}

void tremovetstr(char direction[],int line,int place,int dsize,char dmode){
    rootdo(direction);
    char matn[100] ,matnkol[10000] ,v;
    int tchar=0 , mcounter=0;
    if(check_fileex(direction)){

        char fana[100] , fanammatn[200];
        strcpy(fana , direction);
        filenamemaker(fana);
        FILE* file1 = fopen(direction , "r");
        FILE* file3 = fopen(fana , "w");
        while((fgets(fanammatn , 200 , file1)) != NULL)
            fputs(fanammatn , file3);
        fclose(file1);
        fclose(file3);

        file1 = fopen(direction , "r");
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
    rootdo(direction);
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

void tpastestr(char direction[] , int line , int place){
    rootdo(direction);
    char cmatn[200] ,matn[100] ,v;
    if(!check_fileex(direction))
        printf("This file doesn't exist\n");
    else{

        char fana[100] , fanammatn[200];
        strcpy(fana , direction);
        filenamemaker(fana);
        FILE* file1 = fopen(direction , "r");
        FILE* file3 = fopen(fana , "w");
        while((fgets(fanammatn , 200 , file1)) != NULL)
            fputs(fanammatn , file3);
        fclose(file1);
        fclose(file3);

        FILE* file2 = fopen("root/mine/clipboard.txt" , "r");
        fgets(cmatn , 200 ,file2);
        fclose(file2);
        file1 = fopen(direction , "r");
        file2 = fopen("root/mine/helper.txt" , "w");
        while((fgets(matn , 100 ,file1)) != NULL){
            fputs(matn , file2);
        }
        fclose(file1);
        fclose(file2);
        file1 = fopen(direction , "w");
        file2 = fopen("root/mine/helper.txt" , "r");
        for(int i=0;i<line-1;i++){
            fgets(matn , 100 , file2);
            fprintf(file1 , "%s" , matn);
        }
        for(int i=0;i<place;i++){
            v=fgetc(file2);
            fputc(v , file1);
        }
        fprintf(file1 , "%s" , cmatn);
        while((fgets(matn , 100 ,file2)) != NULL){
            fprintf(file1 , "%s" ,matn);
        }
        fclose(file1);
        fclose(file2);
    }
}

void tfind(char direction[] , char kalame[] , int modde , int atr){
    if(!check_fileex(direction)){
        printf("This file doesn't exist\n");
        return;
    }
    char fmatn[20000] , matn[100] , buff[100] , c;
    basekon(fmatn);
    int t=0 ,i=0 ,tedad=0,founded[100];
    buff[strlen(kalame)] = '\0';
    FILE* file1 = fopen(direction , "r");

    while((fgets(matn , 100 , file1)) != NULL)
        strcat(fmatn , matn);
    fclose(file1);

    for(int i=0 ; i<=strlen(fmatn) - strlen(kalame) ; i++){
        for(int j=0 ; j<strlen(kalame) ; j++){
            buff[j] = fmatn[i+j];
        }
        if(!strcmp(kalame , buff) &&(i==0 || (fmatn[i-1] == 32 || fmatn[i-1] == '\n')) && (fmatn[i+strlen(kalame)] == 32 || fmatn[i+strlen(kalame)] == '\n' || (i+strlen(kalame) == strlen(fmatn)))){
            founded[tedad]=i;
            tedad++;
        }
    }
    if(modde == 1){
        printf("%d\n",tedad);
        return;
    }

    if(modde == 0){
        printf("%d\n",founded[0]+1);
        return;
    }

    if(modde==3){
        for(int k=0 ; k<i ; k++){
            if(fmatn[k] == 32)
                t++;
        }
        printf("%d\n" , t+1);
            return;
    }

    if(modde == 2){
        if(tedad < atr){
            printf("Couldn't find that many!\n");
            return;
        }
        printf("%d\n",founded[atr-1]+1);
        return;
    }

    if(modde == 4){
        printf("%d",founded[0]+1);
        for(int j=1;j<tedad;j++){
            printf(",%d",founded[j]+1);
        }
        printf("\n");
        return;
    }

    if(modde == 5){
        t=0;
        for(int k=0 ; k<founded[0] ; k++){
            if(fmatn[k] == 32)
                t++;
        }
        printf("%d",t+1);
        for(int j=1 ; j<tedad;j++){
            t=0;
            for(int k=0 ; k<founded[j] ; k++){
            if(fmatn[k] == 32)
                t++;
            }
            printf(",%d",t+1);
        }
        printf("\n");
        return;
    }

    printf("Your word didn't exist\n");
}

void treplace(char direction[] , char kalame[] , char matn2[] , int modde , int atr){
    if(!check_fileex(direction)){
        printf("This file doesn't exist\n");
        return;
    }

    char fana[100] , fanammatn[200];
    strcpy(fana , direction);
    filenamemaker(fana);
    FILE* file1 = fopen(direction , "r");
    FILE* file3 = fopen(fana , "w");
    while((fgets(fanammatn , 200 , file1)) != NULL)
        fputs(fanammatn , file3);
    fclose(file1);
    fclose(file3);

    char fmatn[20000] , matn[100] , buff[100] , c;
    basekon(fmatn);
    int t=0 ,i=0 ,tedad=0,founded[100] ,n;
    buff[strlen(kalame)] = '\0';
    file1 = fopen(direction , "r");

    while((fgets(matn , 100 , file1)) != NULL)
        strcat(fmatn , matn);
    fclose(file1);

    for(int i=0 ; i<=strlen(fmatn) - strlen(kalame) ; i++){
        for(int j=0 ; j<strlen(kalame) ; j++){
            buff[j] = fmatn[i+j];
        }
        if(!strcmp(kalame , buff) &&(i==0 || (fmatn[i-1] == 32 || fmatn[i-1] == '\n')) && (fmatn[i+strlen(kalame)] == 32 || fmatn[i+strlen(kalame)] == '\n' || (i+strlen(kalame) == strlen(fmatn)))){
            founded[tedad]=i;
            tedad++;
        }
    }
    if(tedad == 0){
        printf("Your word wasn't in the file!\n");
        return;
    }
    file1 = fopen(direction , "r");
    FILE* file2 = fopen("root/mine/helper.txt" , "w");
    if(modde == 0 || modde == 1){
            if(modde == 1)
                n=founded[atr-1];
            else
                n=founded[0];
        for(int i=0;i<n;i++){
            c = fgetc(file1);
            fputc(c , file2);
        }
        fputs(matn2 , file2);
        for(int i=0;i<strlen(kalame);i++)
            fgetc(file1);
        while((fgets(matn , 100 , file1)) != NULL)
            fputs(matn , file2);
        fclose(file1);
        fclose(file2);
        file1 = fopen(direction , "w");
        file2 = fopen("root/mine/helper.txt" , "r");
        while((fgets(matn , 100 , file2)) != NULL)
            fputs(matn , file1);
        fclose(file1);
        fclose(file2);
        return;
    }
    int nowcheck=0;
    if(modde == 2){
        basekon(fmatn);
        while((fgets(matn , 100 , file1)) != NULL)
            strcat(fmatn , matn);
        for(int i=0;i<strlen(fmatn);i++){
            if(i != founded[nowcheck]){
                fputc(fmatn[i] , file2);
            }
            else{
                fputs(matn2 , file2);
                i=i+strlen(kalame)-1;
                nowcheck++;
            }
        }
        fclose(file1);
        fclose(file2);
        file1 = fopen(direction , "w");
        file2 = fopen("root/mine/helper.txt" , "r");
        while((fgets(matn , 100 , file2)) != NULL)
            fputs(matn , file1);
        fclose(file1);
        fclose(file2);
        return;
    }
}

void tgrep(char direction[] , char kalame[] , int modde){
    char cdir[100];
    int s=0 ,cd=0 ,chap=0 , ted=0;
    char matn[100] , buff[100] , c;
    int t=0 ,tedad=0,founded[100] ,n;
    buff[6] = '\0';
    for(int i=0 ; i<=strlen(direction) - 6 ; i++){
        for(int j=0 ; j<6 ; j++){
            buff[j] = direction[i+j];
        }
        if(!strcmp("/root/" , buff)){
            founded[tedad]=i;
            tedad++;
        }
    }
    for(int qw=0;qw<tedad-1;qw++){
        cd=0;
        for(int i=s;i<founded[qw+1]-1;i++){
            cdir[cd]=direction[i];
            cd++;
        }
        cdir[cd]='\0';
        rootdo(cdir);
        if(!check_fileex(cdir)){
                printf("This file (/%s) doesn't exist\n",cdir);
            }
            else{
                buff[strlen(kalame)] = '\0';
                FILE* file1 = fopen(cdir , "r");

                while((fgets(matn , 100 , file1)) != NULL){
                    for(int qww=0;qww<=strlen(matn) - strlen(kalame) ; qww++){
                        for(int j=0 ; j<strlen(kalame) ; j++){
                            buff[j] = matn[qww+j];
                        }
                        if(!strcmp(kalame , buff)){
                            int n=strlen(matn);
                            if(matn[n-1]=='\n'){
                                matn[n-1]='\0';
                            }
                            if(modde == 0){
                                printf("/%s : %s\n",cdir,matn);
                                chap++;
                                break;
                            }
                            else if(modde == 1){
                                ted++;
                                break;
                            }
                            else{
                                printf("%s\n",cdir);
                                chap++;
                                t++;
                                break;
                            }
                        }
                    }
                    if(t != 0){
                        break;
                    }
                }
                if(modde == 0 ){
                    if(chap == 0){
                        printf("Couldn't find your word in /%s\n",cdir);
                    }
                }
                chap=0;
                t=0;
                fclose(file1);
            }
        s=founded[qw+1];
    }
    cd=0;
    for(int i=s;i<strlen(direction);i++){
        cdir[cd]=direction[i];
        cd++;
    }
    cdir[cd]='\0';
    rootdo(cdir);
    chap=0;
    if(!check_fileex(cdir)){
        printf("This file (/%s) doesn't exist\n",cdir);
    }
    else{
        buff[strlen(kalame)] = '\0';
        FILE* file1 = fopen(cdir , "r");

        while((fgets(matn , 100 , file1)) != NULL){
            for(int qww=0;qww<=strlen(matn) - strlen(kalame) ; qww++){
                for(int j=0 ; j<strlen(kalame) ; j++){
                    buff[j] = matn[qww+j];
                }
                if(!strcmp(kalame , buff)){
                    int n=strlen(matn);
                    if(matn[n-1]=='\n'){
                        matn[n-1]='\0';
                    }
                    if(modde == 0){
                        printf("/%s : %s\n",cdir,matn);
                        chap++;
                        break;
                    }
                    else if(modde == 1){
                        ted++;
                        break;
                    }
                    else{
                        printf("%s\n",cdir);
                        chap++;
                        t++;
                        break;
                    }
                }
            }
            if(t != 0){
                break;
            }
        }
        if(modde == 0){
            if(chap == 0){
                printf("Couldn't find your word in /%s\n",cdir);
            }
        }
        t=0;
    fclose(file1);
    }
    if(modde == 1){
        printf("%d\n",ted);
    }
}

void tundo(char direction[]){
    if(check_fileex(direction) == 0){
        printf("This file doesn't exist!\n");
        return;
    }
    char fana[100] , matn[200];
    strcpy(fana , direction);
    filenamemaker(fana);
    if(check_fileex(fana) == 0){
        printf("No recent action on this file!\n");
        return;
    }
    FILE* file2 = fopen("root/mine/helper.txt" , "w");
    FILE* file1 = fopen(direction , "r");
    while((fgets(matn , 200 ,file1)) != NULL)
        fputs(matn , file2);
    fclose(file1);
    fclose(file2);
    FILE* file3 = fopen(fana , "r");
    file1 = fopen(direction , "w");
    while((fgets(matn , 200 ,file3)) != NULL)
        fputs(matn , file1);
    fclose(file1);
    fclose(file3);
    file2 = fopen("root/mine/helper.txt" , "r");
    file3 = fopen(fana , "w");
    while((fgets(matn , 200 ,file2)) != NULL)
        fputs(matn , file3);
    fclose(file2);
    fclose(file3);
    return;
}

tcpairs(char direction[]){
    if(!check_fileex(direction)){
        printf("This file doesn't exist!\n");
        return;
    }

    char fana[100] , fanammatn[200];
    strcpy(fana , direction);
    filenamemaker(fana);
    FILE* file1 = fopen(direction , "r");
    FILE* file3 = fopen(fana , "w");
    while((fgets(fanammatn , 200 , file1)) != NULL)
        fputs(fanammatn , file3);
    fclose(file1);
    fclose(file3);

    file1 = fopen(direction , "r");
    FILE* file2 = fopen("root/mine/helper.txt" , "w");
    char c , perv='a';
    int tab=0;
    char matn[2000];
    fgets(matn , 2000 , file1);
    int n=strlen(matn);
    for(int k=0;k<n;k++){
    c=matn[k];
        if(perv == '}' && c != '}'){
            fputc('\n',file2);
        }
        if(c != '{' && c != '}'){
            fputc(c,file2);
            perv=c;
        }
        else if(c == '{'){
            while(c == '{'){
                if(perv == 32 || k == 0){
                    fputc(c,file2);
                    perv=c;
                }
                else if(perv != 32 && k != 0){
                    if(perv != '{')
                        fputc(' ' , file2);
                    fputc(c,file2);
                    perv=c;
                }
                tab+=4;
                fputc('\n',file2);
                if(matn[k+1] == '}'){
                    tab=tab-4;
                }
                for(int i=0;i<tab;i++){
                    fputc(' ' , file2);
                }
                if(matn[k+1] == '}')
                    tab+=4;
                k++;
                while((matn[k] == 32)){
                    k++;
                }
                c=matn[k];
            }
            fputc(c,file2);
            if(c == '}')
                tab-=4;
            perv=c;
        }
        else if(c == '}'){
            fputc('\n',file2);
            tab-=4;
            for(int i=0;i<tab;i++){
                fputc(' ' ,file2);
            }
            fputc(c,file2);
            perv=c;
        }
    }
    fclose(file1);
    fclose(file2);
    file1 = fopen(direction , "w");
    file2 = fopen("root/mine/helper.txt" , "r");
    while((fgets(matn , 2000 , file2)) != NULL){
        fputs(matn , file1);
    }
    fclose(file1);
    fclose(file2);
    return;
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

        else if(!strcmp(vo , "cat")){
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
                rootdo(vo);
                tcat(vo);
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
                tcopystr(direction,line,place,csize,c);
            }
            }
        }
        else if(!strcmp(vo , "cutstr")){
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
                tcopystr(direction,line,place,csize,c);
                tremovetstr(direction,line,place,csize,c);
            }
            }
        }
        else if(!strcmp(vo , "pastestr")){
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
                tpastestr(direction,line,place);
            }
            }
        }

        else if(!strcmp(vo , "find")){
            char c , matn[200];
            int i=0 ,cv=0,m=0,atr=0;
            getchar();
            scanf("%s" , vo);
            if(!strcmp(vo , "-count")){
                m=1;
            }

            else if(!strcmp(vo , "-at")){
                m=2;
                scanf("%d" , &atr);
            }

            else if(!strcmp(vo , "-byword")){
                m=3;
            }

            else if(!strcmp(vo , "-all")){
                getchar();
                scanf("%s" , vo);
                if(!strcmp(vo , "-byword")){
                    m=5;
                }
                else if(!strcmp(vo , "--str")){
                    m=4;
                }
                else{
                    fgets(vo,100,stdin);
                    printf("Invalid Input\n");
                    cv++;
                }
            }

            else if(strcmp(vo , "--str")){
                fgets(vo,100,stdin);
                printf("Invalid Input\n");
                cv++;
            }

            if(m!=0 && m!=4){
                getchar();
                scanf("%s",vo);
                if(strcmp(vo , "--str")){
                    fgets(vo,100,stdin);
                    printf("Invalid Input\n");
                    cv++;
                }
            }
            if(cv==0){
                getchar();
                matn[i]=getchar();
                i++;
                if(matn[i-1] == '"'){
                    i--;
                    matn[i]=getchar();
                    i++;
                    while((matn[i]=getchar()) != '"')
                        i++;
                    matn[i]='\0';
                    getchar();
                }
                else{
                    while((matn[i]=getchar()) != 32)
                        i++;
                    matn[i]='\0';
                }
                scanf("%s" , vo);
                if(strcmp(vo , "--file")){
                fgets(vo,100,stdin);
                printf("Invalid Input\n");
                }
                else{
                    getchar();
                    fgets(vo , 100 ,stdin);
                    vo[strlen(vo)-1]='\0';
                    if(vo[0] == '"'){
                        int n = strlen(vo);
                        n-=2;
                        for(int i=0;i<n;i++)
                        vo[i]=vo[i+1];
                        vo[n] = '\0';
                    }
                    rootdo(vo);
                    tfind(vo , matn , m , atr);
                }
            }
        }

        else if(!strcmp(vo , "replace")){
            char c , matn1[200] , matn2[200];
            int i=0 ,cv=0,m=0,atr=0;
            getchar();
            scanf("%s" , vo);
            if(!strcmp(vo , "-at")){
                m=1;
                scanf("%d",&atr);
            }

            else if(!strcmp(vo , "-all")){
                m=2;
            }

            else if(strcmp(vo , "--str1")){
                fgets(vo,100,stdin);
                printf("Invalid Input\n");
                cv++;
            }

            if(m!=0){
                getchar();
                scanf("%s",vo);
                if(strcmp(vo , "--str1")){
                    fgets(vo,100,stdin);
                    printf("Invalid Input\n");
                    cv++;
                }
            }
            if(cv==0){
                getchar();
                matn1[i]=getchar();
                i++;
                if(matn1[i-1] == '"'){
                    i--;
                    matn1[i]=getchar();
                    i++;
                    while((matn1[i]=getchar()) != '"')
                        i++;
                    matn1[i]='\0';
                    getchar();
                }
                else{
                    while((matn1[i]=getchar()) != 32)
                        i++;
                    matn1[i]='\0';
                }
                scanf("%s",vo);
                if(strcmp(vo , "--str2")){
                    fgets(vo,100,stdin);
                    printf("Invalid Input\n");
                }
                else{
                    i=0;
                    getchar();
                    matn2[i]=getchar();
                    i++;
                    if(matn2[i-1] == '"'){
                        i--;
                        matn2[i]=getchar();
                        i++;
                        while((matn2[i]=getchar()) != '"')
                            i++;
                        matn2[i]='\0';
                        getchar();
                    }
                    else{
                        while((matn2[i]=getchar()) != 32)
                            i++;
                        matn2[i]='\0';
                    }
                    scanf("%s" , vo);
                    if(strcmp(vo , "--file")){
                    fgets(vo,100,stdin);
                    printf("Invalid Input\n");
                    }
                    else{
                        getchar();
                        fgets(vo , 100 ,stdin);
                        vo[strlen(vo)-1]='\0';
                        if(vo[0] == '"'){
                            int n = strlen(vo);
                            n-=2;
                            for(int i=0;i<n;i++)
                            vo[i]=vo[i+1];
                            vo[n] = '\0';
                        }
                        rootdo(vo);
                        treplace(vo , matn1 , matn2 , m , atr);
                    }
                }
            }
        }

        else if(!strcmp(vo , "grep")){
            char c , matn[200];
            int i=0 ,cv=0,m=0,atr=0;
            getchar();
            scanf("%s" , vo);
            if(!strcmp(vo , "-c")){
                m=1;
            }

            else if(!strcmp(vo , "-l")){
                m=2;
            }

            else if(strcmp(vo , "--str")){
                fgets(vo,100,stdin);
                printf("Invalid Input\n");
                cv++;
            }

            if(m!=0){
                getchar();
                scanf("%s",vo);
                if(strcmp(vo , "--str")){
                    fgets(vo,100,stdin);
                    printf("Invalid Input\n");
                    cv++;
                }
            }
            if(cv==0){
                getchar();
                matn[i]=getchar();
                i++;
                if(matn[i-1] == '"'){
                    i--;
                    matn[i]=getchar();
                    i++;
                    while((matn[i]=getchar()) != '"')
                        i++;
                    matn[i]='\0';
                    getchar();
                }
                else{
                    while((matn[i]=getchar()) != 32)
                        i++;
                    matn[i]='\0';
                }
                scanf("%s" , vo);
                if(strcmp(vo , "--files")){
                fgets(vo,100,stdin);
                printf("Invalid Input\n");
                }
                else{
                    getchar();
                    fgets(vo , 100 ,stdin);
                    vo[strlen(vo)-1]='\0';
                    if(vo[0] == '"'){
                        int n = strlen(vo);
                        n-=2;
                        for(int i=0;i<n;i++)
                        vo[i]=vo[i+1];
                        vo[n] = '\0';
                    }
                    tgrep(vo , matn ,m);
                }
            }
        }

        else if(!strcmp(vo , "undo")){
            getchar();
            scanf("%s",vo);
            if(strcmp(vo , "--file")){
                fgets(vo,100,stdin);
                printf("Invalid Input\n");
            }
            else{
                getchar();
                fgets(vo , 100 ,stdin);
                vo[strlen(vo)-1]='\0';
                if(vo[0] == '"'){
                    int n = strlen(vo);
                    n-=2;
                    for(int i=0;i<n;i++)
                        vo[i]=vo[i+1];
                    vo[n] = '\0';
                }
            }
            rootdo(vo);
            tundo(vo);
        }

        else if(!strcmp(vo , "auto-indent")){
            getchar();
            scanf("%s",vo);
            if(strcmp(vo , "--file")){
                fgets(vo,100,stdin);
                printf("Invalid Input\n");
            }
            else{
                getchar();
                fgets(vo , 100 ,stdin);
                vo[strlen(vo)-1]='\0';
                if(vo[0] == '"'){
                    int n = strlen(vo);
                    n-=2;
                    for(int i=0;i<n;i++)
                        vo[i]=vo[i+1];
                    vo[n] = '\0';
                }
                rootdo(vo);
                tcpairs(vo);
            }
        }

        else{
            printf("Invalid Input\n");
            fgets(vo,100,stdin);
        }

    }
    return 0;
}
