#include <cstdlib>
#include <unistd.h>
#include <pthread.h>
#include <iostream>

using namespace std;

void *xmpl_thread (void *ptr);

int main ()
{
  pthread_t thread;
  // The cygwin implementation of pthreads will not allow access
  // to another thread's stack.  This example will not work under
  // cygwin if the keyword static is removed from the fillowing line.
  static char message[] = "Thread 1";
  int iret;

  // Create  a new thread
  iret = pthread_create (
             &thread,
             NULL, 
             xmpl_thread, 
             (void *) message);

  // Wait until thread is complete before main continues. Unless we 
  // wait we run the risk of executing an exit which will terminate
  // the process and all threads before the threads have completed.
  int ret_val;
  pthread_join (thread, (void **)&ret_val);

  cout << "Thread 1 has completed." << endl;
  cout << "Return value is " << ret_val << endl;

  exit (0);
}

void *xmpl_thread (void *ptr)
{
  char *message;
  message = (char *) ptr;
  for(int i = 0; i < 5; i++) {
    cout << message << endl;
    // sleep is thread-safe, only putting the thread to sleep,
    // not the entire process.
    sleep(1);
  }
  pthread_exit((void *)25);
}
