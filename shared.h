#include <sys/mman.h>       
#include <fcntl.h>          
#include <semaphore.h>      
#include <pthread.h>        
#include <sys/stat.h>       
#include <stdio.h>          
#include <stdlib.h>        
#include <unistd.h>         

#define minBuff 2           
#define maxBuff 2         

// Struct defining shared memory for synchronization and buffer
struct sem {
    pthread_mutex_t mutex; // Mutex for mutual exclusion (protects access to shared buffer)
    sem_t empty;           // Semaphore to track empty slots in the buffer (initially empty)
    sem_t full;            // Semaphore to track full slots in the buffer (initially full)
    char buf[minBuff];     // Circular buffer with size defined by minBuff (2 items)
};
