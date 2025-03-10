# Producer-Consumer Problem

## Overview
This project implements the classic Producer-Consumer problem using shared memory and semaphores in C++. In this solution:

- The **Producer** generates random integer values and places them in a shared buffer.
- The **Consumer** removes these values from the buffer and displays them.
- A shared memory segment is used to store the buffer that both processes access.
- Semaphores are used to synchronize access to the shared buffer, ensuring proper coordination between the producer and consumer.

The implementation includes three key semaphores:
- `mutex`: Ensures mutual exclusion when accessing the buffer
- `empty`: Tracks how many empty slots are available in the buffer
- `full`: Tracks how many filled slots are in the buffer

## Directory Structure
```
├── Include/
│   ├── consumer.hpp
│   ├── producer.hpp
│   └── memorybuff.hpp
├── src/
│   ├── consumer.cpp
│   └── producer.cpp
└── Makefile
```

## How to Build
To compile both the producer and consumer programs, run:

```bash
make
```

This will create two executable files: `producer` and `consumer`.

## How to Run
To run both the producer and consumer simultaneously in the background:

```bash
./producer & ./consumer &
```

This will start both processes. The producer will create items and place them in the shared buffer, while the consumer will remove and display them.

## How to Stop
To terminate both programs, use:

```bash
killall producer consumer
```

This will send a termination signal to both processes, allowing them to clean up resources before exiting.

## Implementation Details
- The programs use POSIX shared memory (`shm_open`, `mmap`) for inter-process communication.
- POSIX semaphores are used for synchronization between the producer and consumer.
- The buffer size is defined by `TABLESIZE` in the memorybuff.hpp header file.
- Each program handles signals to allow for graceful termination.

## Tech Stack
- C++
- Makefile
- gcc

## Author
Ethan Hicks
