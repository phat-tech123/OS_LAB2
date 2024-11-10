#include <pthread.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>

/*
    Filename: SEND 
*/

int main(int argc, char * argv[]){
    const char* file_path = "test.txt";
    const size_t file_size = 4096; // Initializing file size to $kb.
 
    // We open the file for read/write operation
    FILE * file = fopen(file_path, "w+");
    if (file == 0) {
        perror("hehe");
        return 1;
    };
    if (ftruncate(fileno(file), file_size) == -1) {
        perror("ftruncate hehe");
        close(fileno(file));
        return 1;
    };

    void* file_memory = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED,fileno(file), 0);
    if (file_memory == MAP_FAILED) {
        perror("mmap");
        close(fileno(file));
        return 1;
    }
    char textBuffer[1024];
    printf("Type message, ^D to quit!\n");
    while(fgets(textBuffer, sizeof textBuffer, stdin) != NULL){
        strncpy(file_memory,textBuffer, strlen(textBuffer));
    };
    munmap(file_memory, file_size);
    fclose(file);    
    return 0;
};

