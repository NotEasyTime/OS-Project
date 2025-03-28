# OS-Project
# Overview of the Code:

##    Shared Memory and Semaphore Initialization:

        The shared.h header file defines the shared memory structure, which contains:

            A pthread_mutex_t (mutex) to protect access to the shared buffer.

            Two semaphores: empty to track empty slots in the buffer and full to track full slots.

            A circular buffer buf with a size defined by minBuff (2 items).

##    Producer Program:

        The Producer opens (or creates) a shared memory object (/shared) using shm_open().

        It maps the shared memory to a pointer using mmap().

        The Producer initializes two semaphores:

            empty (set to 0): Represents the number of empty slots in the buffer.

            full (set to 0): Represents the number of full slots in the buffer.

        The Producer sends items (characters) to the buffer in a loop, waiting for an empty slot using sem_wait(&shared_mem->empty) before placing an item into the buffer.

        After placing an item in the buffer, the Producer signals the Consumer that an item is available using sem_post(&shared_mem->full).

        The buffer index is updated in a circular manner to handle the wrap-around of the buffer.

        Once the maximum buffer capacity is reached (maxBuff), the Producer exits.

##    Consumer Program:

        The Consumer opens the shared memory object (/shared) with read-write access.

        It maps the shared memory to a pointer using mmap().

        The Consumer initially signals that an empty slot is available for the Producer to write by posting to the empty semaphore.

        The Consumer consumes items from the buffer in a loop, waiting for a full slot using sem_wait(&shared_mem->full) before retrieving an item.

        Once an item is consumed, the Consumer updates the buffer index in a circular manner.

        The Consumer signals the Producer that an empty slot is available using sem_post(&shared_mem->empty).

        After consuming items for maxBuff iterations, the Consumer exits.

##    Synchronization:

        The Producer and Consumer are synchronized using two semaphores:

            empty semaphore tracks available space for the Producer to write into the buffer.

            full semaphore tracks available items for the Consumer to read from the buffer.

        These semaphores prevent the Producer from writing to the buffer when it's full and the Consumer from reading from the buffer when it's empty.

        The mutex (pthread_mutex_t mutex) is declared in the shared memory structure but is not used in this specific implementation. It could be useful for protecting access to the buffer if needed, but the semaphores alone are sufficient in this case.

    Shared Memory Cleanup:

        Both the Producer and Consumer unlink the shared memory object using shm_unlink("/shared") when they are done with it to free system resources.
