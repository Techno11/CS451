#include <stdio.h>
#include <pthread.h>

void thread_task(int i)
{
    // ...
    printf("A thread of %d i-id has been made!\n", i);
    pthread_exit(0); // this code returns to the corresponding pthread_join issue in main()
}

int main() {
    printf("Hello, World!\n");

    pthread_t thread_id[5];

    // The following code creates 5 threads:
    for (int i=0; i<5; i++)
    {
        // Will create a thread:
        pthread_create(&thread_id[i], NULL, (void *(*)(void *))thread_task,  (void *)i);
    }

    //The following code makes sure the main program waits until all threads have finished execution
    for (int i=0; i<5;i++)
    {
        pthread_join(thread_id[i], NULL);
    }

    return 0;
}
