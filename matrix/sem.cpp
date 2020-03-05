// This example uses POSIX anonymous semaphores for synchronization
// of two threads.

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

using namespace std;

void *char_count(void *arg);
sem_t bin_sem;

#define WORK_SIZE 1024
static char work_area[WORK_SIZE];

int main() {
    int res;
    pthread_t thread_id;
    void *thread_result;
    string work_str;

    sem_init(&bin_sem, 0, 0);
    pthread_create(&thread_id, NULL, char_count, NULL);
    work_area[0] = '\0';
    
    cout << "Input some text. Enter 'end' to finish\n";
    while(strncmp("end", work_area, 3) != 0) {
        getline(cin, work_str);
        strncpy(work_area, work_str.c_str(), WORK_SIZE);
        sem_post(&bin_sem);
    }
    cout << "\nWaiting for thread to finish...\n";
    pthread_join(thread_id, &thread_result);
    cout << "Thread joined\n";
    sem_destroy(&bin_sem);

    exit(EXIT_SUCCESS);
}

void *char_count(void *arg) {
    sem_wait(&bin_sem);
    while(strncmp("end", work_area, 3) != 0) {
      cout << "You input " << strlen(work_area) << " characters\n";
      cout << work_area << endl;
      sem_wait(&bin_sem);
    }
    pthread_exit(NULL);
}

