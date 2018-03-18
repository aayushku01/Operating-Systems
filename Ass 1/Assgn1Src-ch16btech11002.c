#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/mman.h>
//#include <sys/types.h>

int main()
{
	
	long n=0;
	const char *name = "Collatz_Number";		// shared memory name
	const int SIZE = 4096*(sizeof(long));		// file size 

	int shm_fd;		// file descriptor
	char *shm_base;		// base address
	char *ptr;		// shm_base is fixed, ptr is movable


	while(n<=0){
		printf("Enter A Number : ");
		scanf("%ld",&n);
	}
	shm_fd = shm_open(name,O_CREAT | O_RDWR, 0666);
	if (shm_fd == -1) {
		printf("Shared memory failed \n");
		exit(1);
	}
	ftruncate(shm_fd, SIZE);
	shm_base = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (shm_base == MAP_FAILED) {
		printf("Map failed\n");

		exit(1);
	}
	/* fork a child process */
	pid_t pid;
	pid = fork();
	

	if (pid < 0) { /* error occurred */
		printf("Fork Failed");
		return 1;
	}else if (pid == 0) { /* child process */
		//printf("%d ",n);
		ptr = shm_base;
		ptr += sprintf(ptr, "%ld", n);
		while(n>1){
			if(n%2 == 0){
				n = n/2;
				//printf("%d ",n);
				ptr += sprintf(ptr, ",%ld", n);
			}else{
				n = 3*n + 1;
				//printf("%d ",n);
				ptr += sprintf(ptr, ",%ld", n);
			}
		}

	}else { /* parent process */
		wait(NULL);
		//printf("Child Complete\n");
		//printf("Collatz Number(s) : ");
		printf("%s \n", shm_base);
	}

	if (munmap(shm_base, SIZE) == -1){
		printf("Unmap failed\n");
		exit(1);
	}
	if (close(shm_fd) == -1) {
		printf("Close failed: \n");
		exit(1);
	}

	return 0;
}
