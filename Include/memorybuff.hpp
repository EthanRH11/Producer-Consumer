/*Ethan Hicks
  03/10/2025
  Consumer Header File
*/

#ifndef MEMORYBUFF_HPP
#define MEMORYBUFF_HPP

#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <semaphore.h>

static const int TABLESIZE = 2;

struct sharedMemoryBuff
{

    int table[TABLESIZE];

    int in;
    int out;

    sem_t mutex;
    sem_t empty;
    sem_t full;
};

#endif // MEMORYBUFF_HPP
