#include <time.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

int main(){

clock_t t;
t = clock();

long int n,j;
int k;
char buffer[10];
int i,flag=0;

printf("Enter Number to find primes\n");
scanf("%ld",&n);
printf("Enter Number of child process\n");
scanf("%d",&k);


const char *name = "Prime_Numbers";		// shared memory name
const int SIZE = (n/k)*(sizeof(long));		// file size 

int shm_fd[k];		// file descriptor
char *shm_base[k];		// base address
char *ptr;		// shm_base is fixed, ptr is movable

FILE *file;
file=fopen("out.txt","w");

pid_t pid;

for (int i = 0; i < k; ++i){
	shm_fd[i] = shm_open("i",O_CREAT | O_RDWR, 0666);
	if (shm_fd[i] == -1) {
		printf("Shared memory failed \n");
		exit(1);
	}
	ftruncate(shm_fd[i], SIZE);
	shm_base[i] = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, shm_fd[i], 0);
	if (shm_base[i] == MAP_FAILED) {
		printf("Map failed\n");
		exit(1);
	}
}

if (pid < 0) { /* error occurred */
	printf("Fork Failed\n");
	return 1;
}
else{
	for (int i = 0; i < k;i++){
		pid = fork();
		ptr = shm_base[i];
		
		if (pid == 0){
			/*printf("%d : %d\n", getpid(),i+1 );
			sprintf(buffer, "%d", i+1);*/
			for (j = i; j < n; )
			{
				flag = 0;
				for (int l = 2; l*l <= j; l++)
				{
					if (j%l == 0)
					{
						flag = 1;
						break;
					}
				}
				if (flag == 0 && (j !=0 && j != 1))
				{
					//printf("%ld\n", j );
					ptr += sprintf(ptr, "%ld ", j);

				}
				j += k;
			}
			return 0;
		}else if (pid > 0){
			wait(NULL);
			//i++;
		}
	}
}

for (int i = 0; i < k; i++)
{
	fprintf(file,"%s", shm_base[i]);
}


for (int i = 0; i < k; ++i){
	if (munmap(shm_base[i], SIZE) == -1){
		printf("Unmap failed\n");
		exit(1);
	}
	if (close(shm_fd[i]) == -1) {
		printf("Close failed: \n");
		exit(1);
	}
}

fclose(file);

t = clock() - t;
double time_taken = ((double)t)/CLOCKS_PER_SEC;

printf("Time Taken = %f\n", time_taken);


}