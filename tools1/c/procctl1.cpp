#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc,char *argv[])
{
    //execl("/usr/bin/ls","/usr/bin/ls","-lt","/tmp",(char*)0); 
    //第二个参数可以不写目录 直接写ls
    //execl("/usr/bin/ls","ls","-lt","/tmp",(char*)0);

    char *pargv[argc];
    for(int ii=2;ii<argc;ii++){
        pargv[ii-2]=argv[ii];
    }
    pargv[argc-2]=NULL;
        
    //每隔10s调用一次
    while(true){
        if(fork()==0){
            //子进程
            //execl("/usr/bin/ls","ls","-lt","/tmp/yum.log",(char*)0);
            //按照预想的形式 ./procctl 5 /usr/bin/ls  -lt /tmp/yum.log
            //execl(argv[2],argv[2],argv[3],argv[4],(char*)0);
            
            //为了支持参数不确定的情况 采用execv
            execv(argv[2],pargv);
        }else{
            //父进程
            int status;
            wait(&status);
            //sleep(10); 改为参数
            sleep(atoi(argv[1]));
        }
    }


    
    return 0;
}