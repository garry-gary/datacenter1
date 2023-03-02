#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc,char *argv[])
{
    if(argc<3){
        printf("Using:./procctl timetvl program argv ...\n");
        printf("Example:/project/tools1/bin/procctl 5 /usr/bin/ls -lt /tmp/yum.log \n\n");

        printf("本程序是服务程序的调度程序,周期性启动服务程序或shell脚本。\n");
        printf("timetvl 运行周期，单位：秒。被调度的程序运行结束后，在timetvl秒后会被procctl重新启动。\n");
        printf("program 被调度的程序名，必须使用全路径。\n");
        printf("argvs   被调度的程序的参数。\n");
        printf("注意，本程序不会被kill杀死，但可以用kill -9强行杀死。\n\n\n");

        exit(0);
    }

    //关闭信号和IO，本程序不希望被打扰
    for(int ii=0;ii<64;ii++){
        signal(ii,SIG_IGN); close(ii);
    }

    //生成子进程，父进程退出，让程序运行在后台，由系统1号进程托管。
    if(fork()!=0) exit(0);

    //启用SIGCHLD信号，让父进程可以wait子进程退出的状态。
    signal(SIGCHLD,SIG_DFL);

    char *pargv[argc];
    for(int ii=2;ii<argc;ii++){
        pargv[ii-2]=argv[ii];
    }
    pargv[argc-2]=NULL;
        
    //每隔10s调用一次
    while(true){
        if(fork()==0){
            //子进程
            execv(argv[2],pargv);
            exit(0); //这行代码在execv成功时不会执行，失败后会执行。
        }else{
            //父进程
            int status;
            wait(&status);
            sleep(atoi(argv[1]));
        }
    }

    return 0;
}