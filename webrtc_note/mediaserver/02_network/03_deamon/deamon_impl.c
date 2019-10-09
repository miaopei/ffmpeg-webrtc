#include <unistd.h>
#include <signal.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <syslog.h>

void init_daemon(void)
{
    int pid;
    int i;
    if(pid=fork())
        exit(0);//是父进程，结束父进程
    else if(pid< 0)
        exit(1);//fork失败，退出
    //是第一子进程，后台继续执行
    setsid();//第一子进程成为新的会话组长和进程组长
    //并与控制终端分离
    if(pid=fork())
        exit(0);//是第一子进程，结束第一子进程
    else if(pid< 0)
        exit(1);//fork失败，退出
    //是第二子进程，继续
    //第二子进程不再是会话组长
    for(i=0;i< NOFILE;++i)//关闭打开的文件描述符
        close(i);
    //chdir("/tmp");//改变工作目录到/tmp
    umask(0);//重设文件创建掩模
    return;
}

void sig_term(int signo)
{
    if(signo==SIGTERM) /*catchedsignalsentbykill(1)command*/
    {
        syslog(LOG_INFO,"log4j-agent program terminated.");
        closelog();
        exit(0);
    }
}

int main(int argc, char** argv)
{
    extern int errno;
    if (argc>1) 
    {
        init_daemon();//初始化为Daemon
        openlog("log4j-agent", LOG_PID, LOG_USER);
        syslog(LOG_INFO, "log4j-agent started.");
        signal(SIGTERM,sig_term);/*arrangetocatchthesignal*/
        char *p[argc + 1];
        int j=0;
        for(j=0; j<argc+1; j++){
            p[j] = 0;
        }
        int i=1;
        for(i=1; i<argc; i++){
            p[i-1] = argv[i];
        }
        for(j=0; j<argc+1; j++){
            printf("%s \n", p[j]);
        }
        int ret = execvp(argv[1], p);
        if (ret<0){
            //syslog(LOG_ERR, "execl failed:%s");
            fprintf(stderr,"execl failed:%s", strerror(errno));
            return -1;
        }
    }else{
        printf("usage: ./daemon commond [argv] \n");
        printf("su as: ./daemon java -jar log4j-agent.jar -f conf/log4j.xml -p 4561 \n");
    }

}
