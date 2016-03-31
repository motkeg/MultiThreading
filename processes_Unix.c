#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <signal.h>
#include "TimeFunc.h"

//#define SIZE 1000
//////////////////////////////////Global Var///////////////////////////
int *washedcars;
int *machine;
double *timeWash;
int *flag;

pthread_mutex_t mutex;
pthread_cond_t IsMachine;
struct timeval startT, nowT;
//////////////////////////////////////////////////
//////////// Run function for thread////////////////////////////////
int RunThread(void* id)
{

	int ID=(int)id;
   struct timeval  Wasingtime;

    pthread_mutex_lock(&mutex);
    if(*machine<=0)
    	pthread_cond_wait(&IsMachine,&mutex);
    *machine-=1;               ///// critical section -reduce washing machine
    pthread_mutex_unlock(&mutex);//
    gettimeofday(&Wasingtime, NULL);
	printf("Im: %d Enter to wash in time: %lf\n", ID, GetTime(startT, UpdateTime(nowT)));
	usleep((int)(pow(10, 6) * nextTime(1.0 / 3.0)*-1));
    printf("Im: %d Finish wash in time: %lf\n", ID, GetTime(startT, UpdateTime(nowT)));
    pthread_mutex_lock(&mutex);
	*timeWash += GetTime(Wasingtime, UpdateTime(nowT));//// critical section
	*washedcars += 1;
	*machine+=1;
	pthread_cond_signal(&IsMachine);
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}
void UserStop(){
	signal(SIGINT,UserStop);
	*flag=1;}

////////////////////////////////////////////////////////

int main()
{
    int t=1;
    int N, run ; float avgC ;
    void *status;
   pthread_t *Cars;
   pthread_attr_t attr;

    printf("enter num of wash station(1-5):\nnum of run time program(in seconds)\nnum of avg arrival car to station:\n");
   	scanf("%d%d%f", &N,&run,&avgC);
/////////////////////// inition pointers//////////////////////////////////
    washedcars=(int *)calloc(1,sizeof(int));
    machine=(int *)calloc(1,sizeof(int));
    timeWash=(double *)calloc(1,sizeof(double));
    flag=(int*)calloc(1,sizeof(int));
    Cars=(pthread_t *)calloc(1,sizeof(pthread_t));

    *machine=N;///////  number of washing machine
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&IsMachine,NULL);
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	gettimeofday(&startT, NULL);
		//------------------START---------------------------------//
		while ( GetTime(startT, UpdateTime(nowT))<=run && *flag==0)
		{
			signal(SIGINT,UserStop);
			pthread_create(&Cars[t],&attr,RunThread,(void*)t);

			printf("FFF%d",*flag);
			printf("\nIm: %d {{Enterd to Queue}} in time: %lf\n", t, GetTime(startT, UpdateTime(nowT)));
			t++;
			usleep((int)(pow(10,6)*nextTime(1.0/avgC))*-1);

        }



		printf("\n--------------------------After Time Ended-------------------------------\n\n");
//////////////////////////////////////  waiting for end of all threads
		      int i;
				for (i = t-1; i >=1; i--) {
					pthread_join(Cars[i],&status);
				}
     ////////////////////////////////////////// End program////////////
		printf("\n\n{///////////End program////////////}\n");
		printf("car washed--> [%d], in Total run Time: %lf(s)\n", *washedcars, GetTime(startT, UpdateTime(nowT)));
		printf("avg wash time: %lf(s)\n", *timeWash / (*washedcars));
		//////////////////////////////////////////////////

		//////////////// free memory and destroy mutex and attributes
		free(washedcars);
		free(machine);
		free(timeWash);
		pthread_attr_destroy(&attr);
		pthread_mutex_destroy(&mutex);
		pthread_exit(NULL);


	return EXIT_SUCCESS;
}

