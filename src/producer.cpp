/*Ethan Hicks
  03/10/2025
  Producer Implementation
*/

#include "../Include/producer.hpp"
#include "../Include/memorybuff.hpp"
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <signal.h>

bool running = true;

// Signal handler for termination
void signal_handler(int sig)
{
    std::cout << "Producer received termination signal. Exiting..." << std::endl;
    running = false;
}

// Main Function
int main(int argc, char *argv[])
{
    int fd;
    const char *shmpath = "/producer_consumer_buffer";
    struct sharedMemoryBuff *prodmem;

    // Set up signal handler
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    // random number generator
    srand(time(NULL));

    // Open shared memory
    fd = shm_open(shmpath, O_CREAT | O_RDWR, 0666);
    if (fd == -1)
    {
        perror("shm_open failed");
        return 1;
    }

    // Set size of the shared memory
    if (ftruncate(fd, sizeof(sharedMemoryBuff)) == -1)
    {
        perror("ftruncate failed");
        shm_unlink(shmpath);
        return 1;
    }

    // Map shared memory
    prodmem = static_cast<sharedMemoryBuff *>(mmap(NULL, sizeof(sharedMemoryBuff),
                                                   PROT_READ | PROT_WRITE,
                                                   MAP_SHARED, fd, 0));
    if (prodmem == MAP_FAILED)
    {
        perror("mmap failed");
        shm_unlink(shmpath);
        return 1;
    }

    // Initialize semaphores
    if (sem_init(&(prodmem->mutex), 1, 1) == -1 ||
        sem_init(&(prodmem->full), 1, 0) == -1 ||
        sem_init(&(prodmem->empty), 1, TABLESIZE) == -1)
    {
        perror("sem_init failed");
        munmap(prodmem, sizeof(sharedMemoryBuff));
        shm_unlink(shmpath);
        return 1;
    }

    // Initialize buffer indices
    prodmem->in = 0;
    prodmem->out = 0;

    std::cout << "Producer started. Press Ctrl+C to terminate." << std::endl;

    // run continuously until signal received
    while (running)
    {
        if (sem_wait(&prodmem->empty) == -1)
        {
            if (errno == EINTR)
                continue; // Interrupted by signal
            perror("sem_wait on empty failed");
            break;
        }

        if (sem_wait(&prodmem->mutex) == -1)
        {
            if (errno == EINTR)
            {
                sem_post(&prodmem->empty);
                continue;
            }
            perror("sem_wait on mutex failed");
            sem_post(&prodmem->empty);
            break;
        }

        // Produce an item
        int prod_val = rand() % 1000;
        prodmem->table[prodmem->in] = prod_val;

        std::cout << "Produced: " << prod_val << " at position " << prodmem->in << std::endl;

        // update index
        prodmem->in = (prodmem->in + 1) % TABLESIZE;

        sem_post(&prodmem->mutex);
        sem_post(&prodmem->full);

        // Sleep for a random time (1-10 seconds)
        sleep(1 + (rand() % 10));
    }

    // Cleanup
    std::cout << "Producer cleaning up..." << std::endl;

    sem_destroy(&prodmem->mutex);
    sem_destroy(&prodmem->full);
    sem_destroy(&prodmem->empty);

    munmap(prodmem, sizeof(sharedMemoryBuff));
    close(fd);

    std::cout << "Producer terminated." << std::endl;
    return 0;
}