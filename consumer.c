#include "shared.h"

int main() {
    // Open the shared memory object (read-write access)
    int shm_fd = shm_open("/shared", O_RDWR, 0);

    // Map the shared memory object to a pointer
    struct sem *shared_mem = mmap(NULL, sizeof(*shared_mem), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // Signal that a slot is available for the producer to write to (post empty semaphore)
    sem_post(&shared_mem->empty);

    // Initialize the buffer index to 0
    int buffer_index = 0;

    // Consume items from the shared buffer for maxBuff iterations
    for (int i = 0; i < maxBuff; ++i) {
        // Wait until there is at least one item available in the buffer (full semaphore)
        sem_wait(&shared_mem->full);

        // Retrieve the item from the buffer
        char item_received = shared_mem->buf[buffer_index];
        
        // Print the item that was consumed (output in red)
        printf("\033[1;31mConsumer got item on index %d: %c\033[0m\n", buffer_index, item_received);

        // Update the buffer index in a circular manner
        buffer_index = (buffer_index + 1) % minBuff;

        // Signal that a slot is now available in the buffer for the producer to write (post empty semaphore)
        sem_post(&shared_mem->empty);
    }

    // Unlink the shared memory object
    shm_unlink("/shared");
}
