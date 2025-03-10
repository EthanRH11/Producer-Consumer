/*Ethan Hicks
  03/10/2025
  Consumer Implementation
*/

#include "../Include/consumer.hpp"
#include "../Include/memorybuff.hpp"
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <signal.h>

bool running = true;

// Signal handler for termination
void signal_handler(int sig)
{
    std::cout << "Consumer received termination signal. Exiting..." << std::endl;
    running = false;
}

// Main Function
int main(int argc, char *argv[])
{
    int fd;
    const char *shmpath = "/producer_consumer_buffer";
    struct sharedMemoryBuff *consmem;

    // Set up signal handler
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    // Open existing shared memory
    fd = shm_open(shmpath, O_RDWR, 0666);
    if (fd == -1)
    {
        perror("shm_open failed - make sure producer is running first");
        return 1;
    }

    // Map shared memory
    consmem = static_cast<sharedMemoryBuff *>(mmap(NULL, sizeof(sharedMemoryBuff),
                                                   PROT_READ | PROT_WRITE,
                                                   MAP_SHARED, fd, 0));
    if (consmem == MAP_FAILED)
    {
        perror("mmap failed");
        return 1;
    }

    std::cout << "Consumer started. Press Ctrl+C to terminate." << std::endl;

    // Consumption loop
    while (running)
    {
        if (sem_wait(&(consmem->full)) == -1)
        {
            if (errno == EINTR)
                continue;
            perror("sem_wait on full semaphore failed");
            break;
        }

        if (sem_wait(&(consmem->mutex)) == -1)
        {
            if (errno == EINTR)
            {
                sem_post(&(consmem->full));
                continue;
            }
            perror("sem_wait on mutex semaphore failed");
            sem_post(&(consmem->full));
            break;
        }

        // Consume an item
        int con_item = consmem->table[consmem->out];
        std::cout << "Consumed: " << con_item << " from position " << consmem->out << std::endl;

        // Update index
        consmem->out = (consmem->out + 1) % TABLESIZE;

        sem_post(&(consmem->mutex));
        sem_post(&(consmem->empty));

        // Sleep for a random time (1-10 seconds)
        sleep(1 + (rand() % 10));
    }

    // Cleanup
    std::cout << "Consumer cleaning up..." << std::endl;

    munmap(consmem, sizeof(sharedMemoryBuff));
    close(fd);

    shm_unlink(shmpath);

    std::cout << "Consumer terminated." << std::endl;
    return 0;
}