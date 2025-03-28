#include "shared.h"

int main() {
    // Open the shared memory object, creating it if it doesn't exist
    int shm_fd = shm_open("/shared", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    
    // Map the shared memory object to a pointer
    struct sem *shared_mem = mmap(NULL, sizeof(*shared_mem), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    
    // Set the size of the shared memory object
    ftruncate(shm_fd, sizeof(struct sem));

    // Initialize the semaphores for synchronization
    sem_init(&shared_mem->empty, 1, 0);  // Semaphore for empty buffer slots
    sem_init(&shared_mem->full, 1, 0);   // Semaphore for full buffer slots

    // Initialize the character to be sent and buffer index
    char item_to_send = 'a';
    int buffer_index = 0;

    // Send maxBuff items to the shared buffer
    for (int i = 0; i < maxBuff; ++i) {
        // Wait until there is space in the buffer
        sem_wait(&shared_mem->empty);

        // Place the item in the buffer and print the operation
        shared_mem->buf[buffer_index] = item_to_send;
        printf("\033[1;34mProducer sent item on index %d: %c\033[0m\n", buffer_index, item_to_send);

        // Update the buffer index in a circular manner
        buffer_index = (buffer_index + 1) % minBuff;

        // Increment the character to be sent
        ++item_to_send;

        // Signal that a new item is available (full slot)
        sem_post(&shared_mem->full);
    }

    // Unlink the shared memory object
    shm_unlink("/shared");

}
