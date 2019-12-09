#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
using namespace std;
pid_t p1;
pid_t t1;
void parent_func(int sig_no)
{
    if (sig_no == SIGINT)
    {
        kill(p1, SIGUSR1);
        kill(t1, SIGUSR1);
    }
}
void child_func(int sig_no)
{
    if (sig_no == SIGUSR1)
    {
        cout << "\nChild Process " << getpid() << " is Killed by Parent!" << endl;
        exit(0);
    }
}
int main()
{

    int pipefd[2] = {0, 0};
    if (pipe(pipefd) != 0)
    {
        perror("pipe");
        exit(1);
    }
    p1 = fork();
    // 子进程1 - 发送消息
    if (p1 == 0)
    {
        char temp[100];
        int i = 1;
        signal(SIGINT, SIG_IGN);
        signal(SIGUSR1, child_func);
        while (1)
        {
            sprintf(temp, "I send you %d times\n", i);
            int m = write(pipefd[1], temp, 100);
            sleep(1);
            i++;
        }
        exit(0);
    }
    t1 = fork();
    // 子进程2 - 接受消息
    if (t1 == 0)
    {
        char temp[100];
        int i = 1;
        signal(SIGINT, SIG_IGN);
        signal(SIGUSR1, child_func);
        while (1)
        {
            int m = read(pipefd[0], temp, 100);
            cout << "recv from p1:" << temp;
            i++;
        }
        exit(0);
    }
    if (t1 != 0 && p1 != 0)
    {
        int status_1;
        int status_2;
        signal(SIGINT, parent_func);
        pid_t pid_1 = waitpid(p1, &status_1, 0);
        pid_t pid_2 = waitpid(t1, &status_2, 0);
        close(pipefd[1]);
        close(pipefd[0]);
        // cout<<"Wait success for child\n"<<endl;
        // cout<<"p1:Exit code: "<<((status_1>>8)&0xff)<<",Exit signal: "<<(status_1&0xff)<<endl;
        // cout<<"t1:Exit code: "<<((status_2>>8)&0xff)<<",Exit signal: "<<(status_2&0xff)<<endl;
        cout << "Parent process is killed" << endl;
    }
    exit(0);
}