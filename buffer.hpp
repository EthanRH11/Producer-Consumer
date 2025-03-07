// Ethan Hicks
// 03/06/2025

#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <semaphore.h>

// Shared Memory Name
const char *shm_name = "/producer_consumer_buffer";

// Semaphore Names
const char *sem_empty = "/pc_empty_sem";
const char *sem_full = "/pc_full_sem";

// Buffer Size
const static int bufferSize = 2;

struct Buffer
{
    int bufferTable[bufferSize];
};

#endif // BUFFER_HPP