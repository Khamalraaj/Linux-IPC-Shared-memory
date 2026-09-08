#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>

#define TEXT_SZ 2048

struct shared_use_st {
    int written;
    char some_text[TEXT_SZ];
};

int main() {
    int shmid;
    struct shared_use_st *shared_stuff;

    shmid = shmget(IPC_PRIVATE, sizeof(struct shared_use_st),
                   0666 | IPC_CREAT);

    shared_stuff = shmat(shmid, NULL, 0);
    shared_stuff->written = 0;

    pid_t pid = fork();

    if (pid == 0) {
        while (1) {
            while (shared_stuff->written == 0)
                sleep(1);

            printf("Consumer received: %s", shared_stuff->some_text);

            if (strncmp(shared_stuff->some_text, "end", 3) == 0)
                break;

            shared_stuff->written = 0;
        }

        shmdt(shared_stuff);
    }
    else {
        char buffer[TEXT_SZ];

        while (1) {
            printf("Enter Some Text: ");
            fgets(buffer, TEXT_SZ, stdin);

            strcpy(shared_stuff->some_text, buffer);
            shared_stuff->written = 1;

            printf("Producer sent: %s", shared_stuff->some_text);

            if (strncmp(buffer, "end", 3) == 0)
                break;

            while (shared_stuff->written == 1)
                sleep(1);
        }

        wait(NULL);
        shmdt(shared_stuff);
        shmctl(shmid, IPC_RMID, NULL);
    }

    return 0;
}
