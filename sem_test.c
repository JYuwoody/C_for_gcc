#include <semaphore.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <pthread.h>
//-----------------------------------------------------------------------------
#define MAX_I2C_BUS         14
#define I2C_SEM_KEY		    "skeyi2c-%d"
#define	MAX_SKEY_LEN	    16
#define	LOOP_NUMBER	        10
//gcc -o sem_test sem_test.c -lpthread
//-----------------------------------------------------------------------------
typedef unsigned char  INT8U;
void sem_wait_i2c(INT8U bus);
void sem_post_i2c(INT8U bus);
int Index = 0;

//-----------------------------------------------------------------------------
void sem_wait_i2c(INT8U bus)
{
	char sem_key[MAX_SKEY_LEN];
	snprintf(sem_key, MAX_SKEY_LEN, I2C_SEM_KEY, bus);
	sem_t *semaphore = sem_open(sem_key, O_CREAT | O_EXCL, 0666, 1);
	if(semaphore == SEM_FAILED)
	{
		if(errno == EEXIST)
		{
			semaphore = sem_open(sem_key, 0, 0666, 0);
			if(semaphore != SEM_FAILED)
			{
				//printf("sem_wait i2c bus %d\n",bus);
				sem_wait(semaphore);
			}
		}
	}

}

//-----------------------------------------------------------------------------
void sem_post_i2c(INT8U bus)
{
	char sem_key[MAX_SKEY_LEN];
	snprintf(sem_key, MAX_SKEY_LEN, I2C_SEM_KEY, bus);
	sem_t *semaphore = sem_open(sem_key, 0, 0666, 0);
	if(semaphore != SEM_FAILED)
	{
		//printf("sem_post i2c bus %d\n",bus);
		sem_post(semaphore);
	}

}



// child pthread
void* child(void* data) {
    int i, count = 0;
    INT8U bus = 1;    
    for (i = 0; i < LOOP_NUMBER; i++) {
        sem_wait_i2c(bus);
        usleep(300);
        printf("child leaving critical section c-%d Index=%d\n", i, ++Index);
        sem_post_i2c(bus);
        usleep(300);
    }
    pthread_exit(NULL); // exit child pthread
}

// main
int pthreadmain(void) {
    int i, count = 0;
    INT8U bus = 1;        
    pthread_t t; // declare pthread variable
    pthread_create(&t, NULL, child, "Child"); // create child pthread

    // main pthread 
    for (i = 0; i < LOOP_NUMBER; i++) {
        sem_wait_i2c(bus);
        //usleep(30*1000);
        usleep(300);
        printf("parent leaving critical section p-%d Index=%d\n", i, ++Index);
        sem_post_i2c(bus);
        usleep(300);
    }

    pthread_join(t, NULL); // Wait for child thread execution to complete
    return 0;
}


int mainfork(void) {
    int i, count = 0;
    INT8U bus = 1;
    pid_t pid;
    pid = fork();

    if (pid == 0) { // child process
        for (i = 0; i < LOOP_NUMBER; i++) {
            sem_wait_i2c(bus);
            usleep(300);
            printf("child leaving critical section c-%d Index=%d\n", i, ++Index);
            sem_post_i2c(bus);
            usleep(300);
        }
        exit(0);
    }

    // back to parent process
    for (i = 0; i < LOOP_NUMBER; i++) {
        sem_wait_i2c(bus);
        usleep(300);
        printf("parent leaving critical section p-%d Index=%d\n", i, ++Index);
        sem_post_i2c(bus);
        //usleep(1);
    }
    // wait child fork exit >>>
    int status;
    pid_t ret;
    ret = wait(&status);
    if(ret <0){
        perror("wait error");
        exit(EXIT_FAILURE);
    }
        printf("ret = %d pid = %d\n", ret, pid);
    if (WIFEXITED(status))
        printf("child exited normal exit status=%d\n", WEXITSTATUS(status));

    else if (WIFSIGNALED(status))
        printf("child exited abnormal signal number=%d\n", WTERMSIG(status));
    else if (WIFSTOPPED(status))
        printf("child stoped signal number=%d\n", WSTOPSIG(status));

    // wait child fork exit <<<

    return 0;
}


int main(int argc, char **argv) {
    //pthreadmain();
    mainfork();
    return 0;
}
