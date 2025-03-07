#include "buffer.hpp"

#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <semaphore.h>

int main()
{
    int shared = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    if (shared == -1)
    {
        perror("smh_open");
        exit(EXIT_FAILURE);
    }

    ftruncate(shared, sizeof(Buffer));
    Buffer *sharedMemory = (Buffer *)mmap(NULL, sizeof(Buffer), PROT_READ | PROT_WRITE, MAP_SHARED, shared, 0);
    if (sharedMemory == MAP_FAILED)
    {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    sem_t *full = sem_open("/pc_full_sem", O_CREAT, 0666, 0);
    sem_t *empty = sem_open("/pc_empty_sem", O_CREAT, 0666, bufferSize);

    if (full == SEM_FAILED || empty == SEM_FAILED)
    {
        perror("semaphore");
        exit(EXIT_FAILURE);
    }
}