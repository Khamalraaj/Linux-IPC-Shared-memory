# Linux-IPC-Shared-memory
Ex06-Linux IPC-Shared-memory

# AIM:
To Write a C program that illustrates two processes communicating using shared memory.

# DESIGN STEPS:

### Step 1:

Navigate to any Linux environment installed on the system or installed inside a virtual environment like virtual box/vmware or online linux JSLinux (https://bellard.org/jslinux/vm.html?url=alpine-x86.cfg&mem=192) or docker.

### Step 2:

Write the C Program using Linux Process API - Shared Memory

### Step 3:

Execute the C Program for the desired output. 

# PROGRAM:

## Write a C program that illustrates two processes communicating using shared memory.


```py
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
```


## OUTPUT:

![img](img.png)

# RESULT:
The program is executed successfully.
