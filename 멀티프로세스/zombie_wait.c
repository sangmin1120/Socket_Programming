#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc,char* argv[]){

    int status;
    pid_t pid=fork();

    if (pid==0)
        return 3;
    else{
        printf("Child PID : %d \n",pid);
        pid=fork();
        if(pid==0){
            exit(7);
        }
        else{
            printf("Child PID : %d \n",pid);
            wait(&status); // 자식이 종료되면서 관련 정보는 status에 담기게 된다.
            if (WIFEXITED(status)) // 정상 종료 하였는가?
                printf("Child send one : %d \n",WEXITSTATUS(status)); // 그럼 자식의 반환값은?
            
            wait(&status);
            if (WIFEXITED(status))
                printf("Child send two : %d \n",WEXITSTATUS(status));
            sleep(30);
        }
    }

    return 0;
}