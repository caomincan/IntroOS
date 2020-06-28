#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int data = 0; //shared data
int num = 0; // number of readers
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c_write = PTHREAD_COND_INITIALIZER;

void *reader(void *param);
void *writer(void *param);

int main(int argc, char *argv[]){
	pthread_t readers[5];
	pthread_t writers[5];
	
	time_t t;
	srand((unsigned) time(&t));
	
	for(int i = 0; i< 5; i++){
		if(pthread_create(&readers[i], NULL, reader, NULL) != 0){
			fprintf(stderr, "Unable to create reader thread\n");
			exit(1);
		}
	}
	
	for(int i = 0; i< 5; i++){
		if(pthread_create(&writers[i], NULL, writer, NULL) != 0) {
			fprintf(stderr, "Unable to create writer thread\n");
			exit(1);
		}
	}
	
	for(int i=0;i<5;i++){
		pthread_join(readers[i], NULL);
		pthread_join(writers[i], NULL);
		printf("Parent quiting\n");
	}
	return 0;
}

void *reader(void *param) {
	for(int i =0;i<=20;i++){
		sleep(rand()%4); //sleep random time
		int value, n;
		pthread_mutex_lock(&m);
			if(num > 5 || num < 0){ // over flow
				exit(1);
			}
			n = num+1;
			num++;
			value = data;
			num--;
		pthread_mutex_unlock(&m);
		pthread_cond_signal(&c_write);
		
		printf("Read value: %d Current Readers: %d\n", value, n); fflush(stdout);
	}
}

void *writer(void *param) {
	for(int i =0;i<=20;i++){
		int value, n;
		sleep(rand()%4); //sleep random time
		pthread_mutex_lock(&m);
			while(num > 0) {
				pthread_cond_wait(&c_write, &m);
			}
			value = rand()%100;
			n = num;
			data = value;
		pthread_mutex_unlock(&m);
		printf("Writer writes: %d Reader number: %d\n", value, n); fflush(stdout);
	}
}
