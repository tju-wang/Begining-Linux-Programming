/*  In alarm.c, the first function, ding, simulates an alarm clock.  */

#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

static int alarm_fired = 0;

void ding(int sig)
{
    alarm_fired = 1;
}

/*  In main, we tell the child process to wait for five seconds
    before sending a SIGALRM signal to its parent.  */

int main()
{
    pid_t pid;

    printf("alarm application starting\n");

    pid = fork();
    switch(pid) {
    case -1:
      /* Failure */
      perror("fork failed");
      exit(1);
    case 0:
      /* child */
        sleep(5);
        kill(getppid(), SIGALRM);
        exit(0);//子进程到此处执行完毕  退出  相当于这部分就是子进程的代码
    }

/*  The parent process arranges to catch SIGALRM with a call to signal
    and then waits for the inevitable.  */
//只有父进程会等待子进程的alarm信号  并对信号做出相应
    printf("waiting for alarm to go off\n");
    (void) signal(SIGALRM, ding);

    pause();    //挂起等待
    if (alarm_fired)
        printf("Ding!\n");

    printf("done\n");
    exit(0);
}
