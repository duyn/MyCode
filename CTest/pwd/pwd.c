#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>

#define WRONG_CNT_MAX 3
#define WRONG_INT_TIME 20

int iWrongCnt = 0;
int iTimeCnt = 0;
int isLock = 0;
int iInputLog[WRONG_CNT_MAX] = {0};
int iLockTimeCnt = 0;
int iLockTime = 10;

void ResetInputLog()
{
    iTimeCnt = 0;
    iWrongCnt = 0;
    memset(iInputLog, 0, WRONG_CNT_MAX);
}

void timeout_info(int signo)
{
    //printf("timeout!\n");
    if (isLock)
    {
        printf("lock time:%d\n", iLockTime);
        iLockTime--;
        if (0 == iLockTime)
        {
            isLock = 0;
            ResetInputLog();
        }
    }

    if ((iWrongCnt && iTimeCnt - iInputLog[iWrongCnt-1] > WRONG_INT_TIME))
    {
        iLockTimeCnt = 0;
        ResetInputLog();
    }

    if (!iWrongCnt && iTimeCnt > WRONG_INT_TIME)
    {
        printf("reset time\n");
        iTimeCnt = 0;
        iLockTimeCnt = 0;
    }

    iTimeCnt++;
}

void init_sigaction()
{
    struct sigaction act;

    act.sa_handler = timeout_info;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    sigaction(SIGPROF, &act, NULL);
}

void init_time()
{
    struct itimerval val, oval;

    signal(SIGALRM, timeout_info);
    signal(SIGVTALRM, timeout_info);

    val.it_value.tv_sec = 1;
    val.it_value.tv_usec = 0;
    val.it_interval = val.it_value;

    setitimer(ITIMER_REAL, &val, &oval);
}

void CheckLock()
{
    printf("%d, %d, %d\n", iInputLog[0], iInputLog[1], iInputLog[2]);
    if (iInputLog[2] - iInputLog[0] < WRONG_INT_TIME)
    {
        iLockTimeCnt += 10;
        iLockTime = iLockTimeCnt;
        isLock = 1;
    }
    else
    {
        iInputLog[0] = iInputLog[1];
        iInputLog[1] = iInputLog[2];
        iInputLog[2] = 0;
        iWrongCnt--;
    }
}

int main()
{
    char init[] = "1234";
    char input[5] = {0};

    init_time();

    while(1)
    {
        if (isLock)
        {
            //printf("input lock.\n");
            continue;
        }

        scanf("%s", input);
        if (0 == strcmp(input, init))
        {
            iLockTimeCnt = 0;
            ResetInputLog();
            printf("ok.\n");
            //break;
        }
        else if (0 == strcmp(input, "quit"))
        {
            printf("bye.\n");
            break;
        }
        else
        {
            iInputLog[iWrongCnt] = iTimeCnt;
            iWrongCnt++;
            if (WRONG_CNT_MAX <= iWrongCnt)
            {
                CheckLock();
            }
            printf("error(%d).\n", iWrongCnt);
        }
    }
    return 0;
}
