
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#define MEM_SIZE 1000
struct Table{
int columns;
int row;
int *matrix;
};
void print(struct Table temp)
{
    for (int i=0; i<temp.row; i++){
            
        for (int j=0;j<temp.columns;j++){
                if(temp.matrix[i*temp.columns+j]==2){
                printf("|X ");
                            }
                else if(temp.matrix[i*temp.columns+j]==1){
                printf("|O ");
                            }
                else{
                printf("|  ");                     
                    }
                }
            printf("|\n__________\n");
        }
}
int check(struct Table temp){
for (int i=0;i<temp.columns;i++)
{
        if((temp.matrix[i]==temp.matrix[i+3])&&(temp.matrix[i+3]==temp.matrix[i+6])&&(temp.matrix[i+6]==2))
{
    return 1;
}
else if((temp.matrix[i]==temp.matrix[i+3])&&(temp.matrix[i+3]==temp.matrix[i+6])&&(temp.matrix[i+6]==1))
{
    return 2;
}
}
for (int i=0; i<=2*temp.row; i=i+3)
{
    if((temp.matrix[i]==temp.matrix[i+1])&&(temp.matrix[i+1]==temp.matrix[i+2]==2)&&(temp.matrix[i+2]==2))
    {
        return 1;
    }
else if((temp.matrix[i]==temp.matrix[i+1])&&(temp.matrix[i+1]==temp.matrix[i+2]==2)&&(temp.matrix[i+2]==1))
{
    return 2;
}
}
if((temp.matrix[0]==temp.matrix[4])&&(temp.matrix[4]==temp.matrix[8])&&(temp.matrix[8]==2)){
    return 1;
}
else if((temp.matrix[0]==temp.matrix[4])&&(temp.matrix[4]==temp.matrix[8])&&(temp.matrix[8]==1))
{
    return 2;
}
if((temp.matrix[2]==temp.matrix[4])&&(temp.matrix[4]==temp.matrix[6])&&(temp.matrix[6]==2)){
    return 1;
}
else if((temp.matrix[2]==temp.matrix[4])&&(temp.matrix[4]==temp.matrix[6])&&(temp.matrix[6]==1))
{
    return 2;
}

return 0;
}
void krzyzyk(struct Table temp){
while(1){
if(temp.matrix[9] % 2 == 0){
    if(check(temp)==2){
    printf("przegrales,koniec gry\n");
    print(temp);
    return;
    }
    if(temp.matrix[9]==9){
        printf("remis!\n");
        return;
    }
    printf("Twoj ruch!\n");
    print(temp);
    int move;
    while(1){
    scanf("%d",&move);
    if(temp.matrix[move-1]!=0)
    {
        printf("to pole jest juz zajete\n");
    }
    else break;
    }
    temp.matrix[move-1]=2;
    print(temp);
    temp.matrix[9]++;
    if(check(temp)==1){
        printf("Wygrales, koniec gry\n");
        return;    
    }
    if(temp.matrix[9]==9){
        printf("remis!\n");
        return;}
    
 }
else{
    printf("Kolej przeciwnika\n");
    sleep(5);
}}
}
void kolko(struct Table temp){
while(1){
if(temp.matrix[9] % 2 == 1){
    if(check(temp)==1){
    printf("przegrales,koniec gry\n");
    print(temp);
    return;
    }
    if(temp.matrix[9]==9){
        printf("remis!\n");
        return;
    }
    printf("Twoj ruch!\n");
    print(temp);
    int move;
    while(1){
    scanf("%d",&move);
    if(temp.matrix[move-1]!=0)
    {
        printf("to pole jest juz zajete\n");
    }
    else break;
    }
    temp.matrix[move-1]=1;
    print(temp);
    temp.matrix[9]++;
    if(check(temp)==2){
        printf("Wygrales, koniec gry\n");
        return;    
    }
    if(temp.matrix[9]==9){
        printf("remis!\n");
        return;}
    
 }
else{
    printf("Kolej przeciwnika\n");
    sleep(5);
}
}}

int main(int argc, char **argv){
if(argc !=2)
{
    fprintf(stderr,"invalid number of arguments\n");
    return 1;
}
key_t key=ftok(argv[1],'a');
if(key<0)
{
    fprintf(stderr, "Error creating key\n");
    return 2;
}
char role;
int schmid=shmget(key, MEM_SIZE,0666|IPC_CREAT|IPC_EXCL);
role='x';
if(schmid <0){
    schmid=shmget(key,MEM_SIZE,0666);
    role='o';
    if(schmid<0){    
    fprintf(stderr, "Error getting shared memory\n");
    return 3;}
}
struct Table temp;
temp.row=3;
temp.columns=3;
if(role=='x'){
temp.matrix=malloc(sizeof(int) *10);
temp.matrix=shmat(schmid,NULL,0);
for (int i=0;i<10;i++)
{
    temp.matrix[i]=0;
}
}
else if(role=='o'){
temp.matrix=shmat(schmid,NULL,0);
}
printf("Jestem %c\n",role);
if(role=='x')
{   
    krzyzyk(temp);
}
else if(role=='o')
{
    kolko(temp);
    shmdt(temp.matrix);
}

shmctl(schmid,IPC_RMID,NULL);
return 0;
}
