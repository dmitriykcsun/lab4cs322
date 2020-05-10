#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<semaphore.h>
#include<errno.h>
#include<signal.h>
#include<pthread.h>

sem_t *chopstick_1;
sem_t *chopstick_2;
int tot_cycle = 0;
int sigbool=1;
int iseat;
int iposition;
int sigproc=0;

void handler(int num){
	sigbool = 0;
	sigproc = 1;
	fprintf(stderr,"Philosopher #%d has completed %d cycles\n",iposition,tot_cycle);

}

void eat(void){
	printf("philosopher #%d eating\n",iposition);
	sleep((rand()%(3+1-1)+1)); 
	return;
}
void think(void){
	printf("philosopher #%d thinking\n",iposition);
	sleep((rand()%(3+1-1)+1));
	return; 
}
void think2(void){
	printf("philosopher #%d thinking\n",iposition);
	tot_cycle++;
	sleep((rand()%(3+1-1)+1)); 
	return;
}

int main(int argc, char** argv){

	int valuechk1,valuechk2;

	signal(SIGTERM,handler);

	char* seats = argv[1];
	char* position = argv[2];
	char *temp;
	long strseat;
	long strpos;

	strseat = strtol(seats,&temp,10);
	strpos = strtol(position,&temp,10);

	iseat = (int) strseat;
	iposition = (int) strpos;
	iposition++;

	char chop_1[10];
	char chop_2[10];

	snprintf(chop_1,10,"/c%d",iposition);
	if(iposition==iseat){
		snprintf(chop_2,10,"/c1");
	}
	else{
		snprintf(chop_2,10,"/c%d",iposition+1);
	}


	chopstick_1 = sem_open(chop_1,O_CREAT,0666,1);
	chopstick_2 = sem_open(chop_2,O_CREAT,0666,1);

	printf("philosopher#%d has pid = %d\n",iposition,getpid());

	while(sigbool!=0){
		sem_getvalue(chopstick_1, &valuechk1);
		sem_getvalue(chopstick_2, &valuechk2);
		if(valuechk1 == 1 && valuechk2 == 1){
			sem_wait(chopstick_1);
			sem_wait(chopstick_2);
			eat();
			sem_post(chopstick_1);
			sem_post(chopstick_2);
			think2();
		}
		else{
			think();
		}
		if(tot_cycle==10){
			sigbool=0;
		}
	} 

	if(sigproc==0){
		fprintf(stderr,"Philosopher #%d has completed %d cycles\n",iposition,tot_cycle);
	}


	sem_close(chopstick_1);
	sem_close(chopstick_2);
	sem_unlink(chop_1);
	sem_unlink(chop_2);

	return 0;
}