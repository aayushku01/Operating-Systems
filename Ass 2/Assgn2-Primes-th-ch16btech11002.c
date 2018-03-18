#include <time.h>

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <pthread.h>

int **arr;
int k;
long n;
void *prime_cal(void *buffer);


int main(){

clock_t t;
t = clock();

void *buffer;

printf("Enter Number to find primes\n");
scanf("%ld",&n);
printf("Enter Number of Threads\n");
scanf("%d",&k);

int **arr = malloc(k*sizeof(int*));

for (int i = 0; i < k; ++i)
{
	arr[i] = malloc((n/k)*sizeof(int));
}

for (int i = 0; i < k; ++i)
{
	for (int j = 0; j < n/k; ++j)
	{
		arr[i][j] = -1;
	}
}


pthread_t tid[k]; /* the thread identifier */
pthread_attr_t attr; /* set of thread attributes */


const char *name = "Prime_Numbers";		// shared memory name
const int SIZE = (n/k)*(sizeof(long));		// file size 

char *ptr;		// shm_base is fixed, ptr is movable

FILE *file;
file=fopen("out_1.txt","w");

pthread_attr_init(&attr);


for (int i = 0; i < k; i++){
	pthread_create(&tid[i],&attr,prime_cal,(void*)i);

}

for (int i = 0; i < k; i++){
	pthread_join(tid[i], NULL);	
}

for (int i = 0; i < k; ++i)
{
	for (int j = 0; j < n/k; ++j)
	{
		if (arr[i][j] != -1)
		{
			printf("%d ", arr[i][j] );
		}
	}
}


fclose(file);

t = clock() - t;
double time_taken = ((double)t)/CLOCKS_PER_SEC;

printf("Time Taken = %f\n", time_taken);

}

void *prime_cal(void *buffer){

int flag = 0;
long long row = (long long) buffer;
int prime_in_thread = 0;

printf("%ld\n", n );


for (int j = row; j < n; )
{
	flag = 0;
	printf("121\n");
	for (int l = 2; l*l <= j; l++)
	{
		if (j%l == 0)
		{
			flag = 1;
			break;
		}
	}
	printf("asdasf\n");
	printf("%d \n", row );

	if (flag == 0 )
	{
		arr[row][prime_in_thread] = j;
		printf("%d\n", j);
		prime_in_thread++;
	}
	j += k;
}


pthread_exit(NULL);

}