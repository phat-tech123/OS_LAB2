#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SHM_KEY 0x123

void sum(const char* filename, double *total_rating, double* count){
	FILE *file = fopen(filename, "r");
	if(!file){
		perror("fail to open file\n");
		exit(1);
	}
	
    	int userID, movieID, timestamp;
	double rating;
	while(fscanf(file, "%d\t%d\t%lf\t%d\t", &userID, &movieID, &rating, &timestamp) != EOF){
		*total_rating += rating;
		(*count)++;
	}
	fclose(file);
}

int main(int argc, char* argv[]){
	int shmid;
	double* shm;
	shmid = shmget(SHM_KEY, 2 * sizeof(double) * 2, 0644 | IPC_CREAT);
	if(shmid < 0){
		perror("shmget");
		return 1;
	}else {
		printf("shared memory ID: %d\n", shmid);
	}
	
   	shm = (double *)shmat(shmid, NULL, 0);
	if(shm == (double*) - 1){
		perror("shmat");
		exit(1);
	}else{
		printf("share memory mm: %p\n", shm);
	}
	shm[0] = 0;
	shm[1] = 0;
	shm[2] = 0;
	shm[3] = 0;

	pid_t pid1 = fork();
	if(pid1 < 0){
		printf("fork failed\n");
		return 1;
	}
	else if(pid1 == 0){
		double total_rating = 0, count = 0;
		sum("movie-100k_1.txt", &total_rating, &count);
		shm[0] = total_rating;
		shm[1] = count;
	}
	else{
		wait(NULL);
		pid_t pid2 = fork();
		if(pid2 < 0){
			printf("fork failed\n");
			return 1;
		}
		else if(pid2 == 0){
			double total_rating = 0, count = 0;
			sum("movie-100k_2.txt", &total_rating, &count);
			shm[2] = total_rating;
			shm[3] = count;
		}
		else{
			wait(NULL);
			double avg_rating = (shm[0] + shm[2])/(shm[1] + shm[3]);
			printf("%lf\n", avg_rating);
			if(shmdt(shm) == -1){
				perror("shmdt\n");
				return 1;
			}
			if(shmctl(shmid, IPC_RMID, 0) == -1){
				perror("shmctl");
				return 1;
			}
			return 0;
		}
	}
	

}

