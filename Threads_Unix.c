/*
============================================================================
Name        : Threads_Assigment.c
Author      : moti gadian 302490263

============================================================================
*/


#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <signal.h>
#include "Queue.h"
#include "TimeFunc.h"
///////////// Queue Functions//////////////////
/*int pop(int* T,int *size)
{

	int val=T[*size] ,i;
	for (i = 1; i <*size; i++)
		T[i-1]=T[i];

	*size-=1;


return val;

}
int* push(int* T,int *size,int value)
{
    T[*size]=value;
    *size+=1;

    return T;
}*/
//////////////////////////////////////////////

//////////////Memory/////////////////////////////
void* SharedMemory(int size, int C)
{
	void *ptr;
	key_t MemKey;
	int MemoryId;

	MemKey = ftok(".", C);
	if ((MemoryId = shmget(MemKey, size, IPC_CREAT | IPC_EXCL | 0666)) == -1)
	{
		printf("Shared memory segment exists - opening as client\n");
		if ((MemoryId = shmget(MemKey, size, 0)) == -1)
		{
			perror(" bad shmget"); exit(1);
		}
	}
	else
	{
		printf("Creating new shared memory segment\n");
	}
	if ((ptr = shmat(MemoryId, NULL, 0)) == (void*)-1)

	{
		perror("shmat");   exit(1);
	}

	return ptr;

}
//////////////Time Founction////////////////////
/*double GetTime(struct timeval start, struct timeval now)
{
	return (double)(now.tv_usec - start.tv_usec) / CLOCKS_PER_SEC + (double)(now.tv_sec - start.tv_sec);
}
float nextTime(float rateParameter)
{
	return -logf(1.0f - (float)rand() / (float)(RAND_MAX + 1)) / rateParameter;
}
struct timeval UpdateTime(struct timeval T)
{
	gettimeofday(&T, NULL);
	return T;
}*/
//////////////////////////////////////////////////

/////////////////Hendlers///////////////////////////////
void Quitpid()
{
	printf("procces KILLED!");
	exit(0);
}
void EndWash()
{
	signal(SIGUSR1,EndWash);
}

//////////////////////////////////////////

/////////////global var///////
int *size;
int *cars;
int *washedcars;
double *timeWash;
/////////////////////////////

int main(void) {


	int N, run ; float avgC ;

	struct timeval startT, nowT, Wasingtime;
	sem_t WashMachine, Queue;

	int  pid = getpid(), CarId;

    printf("enter num of wash station(1-5):\nnum of run time program(in seconds)\nnum of avg arrival car to station:\n");
	scanf("%d%d%f", &N,&run,&avgC);


	 sem_init(&Queue, 1, 1);
	 sem_init(&WashMachine, 1, N);

	/////////////////Shared memory inition/////////////////////
	cars = (int*)SharedMemory(sizeof(int), 7);
	size = (int*)SharedMemory(sizeof(int), 5);
	washedcars = (int*)SharedMemory(sizeof(int), 51);
	timeWash = (double*)SharedMemory(sizeof(double), 75);
	(*size) = 0;
	*washedcars = 0;
	*timeWash = 0;
	//////////////////////////////////////////////////////////

	gettimeofday(&startT, NULL);
	//------------------START---------------------------------
	while ( GetTime(startT, UpdateTime(nowT))<=run)
	{

		if (pid == getpid())
		{
			if ((CarId=fork())!= 0)///case father- enter new car to Queue
			{

                sem_wait(&Queue);
				push(cars, size, CarId);
				printf("\nIm: %d {{Enterd to Queue}} in time: %lf\n", CarId, GetTime(startT, UpdateTime(nowT)));
				sem_post(&Queue);

				usleep((int)(pow(10,6)*nextTime(1.0/avgC))*-1);

            }
			else///case son-exit himself from Queue and enter to wash
			{

				signal(SIGQUIT, Quitpid);
				signal(SIGUSR1, EndWash);

                    sem_wait(&Queue);
                    if (cars[0] == getpid() )
					{

						sem_wait(&WashMachine);////////
                        pop(cars, size);//////////////  critical section
						sem_post(&Queue);/////////////
						gettimeofday(&Wasingtime, NULL);
						printf("Im: %d Enter to wash in time: %lf\n", getpid(), GetTime(startT, UpdateTime(nowT)));
						usleep((int)(pow(10, 6) * nextTime(1.0 / 3.0)*-1));
						sem_post(&WashMachine);
						printf("Im: %d Finish wash in time: %lf\n", getpid(), GetTime(startT, UpdateTime(nowT)));
						*timeWash += GetTime(Wasingtime, UpdateTime(nowT));
						*washedcars += 1;
						if(*size>0)
						kill(CarId, SIGUSR1);
						kill(getpid(), SIGQUIT);


                    }
		         else
					{
						sem_post(&Queue);
						pause();
					}
			}

		 }
		else
			break;

    }

	if(GetTime(startT, UpdateTime(nowT))>run+0.5)
	{
		printf("\n--------------------------After Time Ended---------------------\n");
		while((*size)>0)
		{
			 int temp=pop(cars, size);

	    gettimeofday(&Wasingtime, NULL);
		printf("Im: %d Enter to wash in time: %lf\n", temp, GetTime(startT, UpdateTime(nowT)));
        usleep((int) (pow(10, 6) * nextTime(1.0 / 3.0))*-1);
		printf("Im: %d Finish wash in time: %lf\n", temp, GetTime(startT, UpdateTime(nowT)));
		*timeWash += GetTime(Wasingtime, UpdateTime(nowT));
	    *washedcars += 1;

		}


    printf("\n\n{End program}\n");
	printf("car washed--> [%d] in Total run Time: %lf\n", *washedcars, GetTime(startT, UpdateTime(nowT)));
	printf("avg wash time: %lf(s)\n", *timeWash / (*washedcars));
	}



	return EXIT_SUCCESS;
}
