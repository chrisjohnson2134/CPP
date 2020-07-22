#include <iostream>
#include <mutex>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
using namespace std;

//passed arguments for threads structure
struct arg_struct
{
  float *mat_a;
  float *mat_b;
  float *mat_c;
  int Rows;
};

mutex mtx;

//only edited/saved under a mutex lock
int CURRENT_ROW = 0;

//The multiply function is passed the args structure by reference
//the function saves the current row that needs to be processed by the
//program and then increments it so that other threads won't do computations
//on the same row.
void *multiply(void *passed)
{
  struct arg_struct *pass = (struct arg_struct *)passed;
  int thread_row = 0;
  //the row must be saved and edited under a mutex
  //so that the two threads aren't doing the same
  //row at the same time.
  mtx.lock();
  thread_row = CURRENT_ROW;
  CURRENT_ROW++;
  mtx.unlock();

  //the function will not quite until all rows have been computed
  while (thread_row < pass->Rows)
  {
    for (int j = 0; j < pass->Rows; j++)
    {
      for (int k = 0; k < pass->Rows; k++)
      {
        pass->mat_c[thread_row * pass->Rows + j] += pass->mat_a[thread_row * pass->Rows + k] *
                                                    pass->mat_b[k * pass->Rows + j];
      }
    }

    mtx.lock();
    thread_row = CURRENT_ROW;
    CURRENT_ROW++;
    mtx.unlock();
  }
}

//the threadRipper function spawns the number of threads asked for by the user
//and passing the reference of each of the matrices into the threads for
//computation.
void threadRipper(arg_struct *pass, int cores)
{
  pthread_t threads[cores];
  for (int i = 0; i < cores; i++)
  {
    pthread_create(&threads[i], NULL, multiply, (void *)pass);
  }

  for (int i = 0; i < cores; i++)
  {
    pthread_join(threads[i], NULL);
  }
}

//parameter guide
//1 -> m - multi core , S - single core
//2 -> size matrix
//3 -> number of cores
int main(int argc, char *argv[])
{
  int i, j;
  int Rows = atoi(argv[2]);
  int cores = 0;
  if(argc == 4)
    cores = atoi(argv[3]);
    
  srand(time(NULL));
  struct arg_struct *pass = (struct arg_struct *)malloc(sizeof(struct arg_struct));
  pass->mat_a = (float *)malloc(Rows * Rows * sizeof(float));
  pass->mat_b = (float *)malloc(Rows * Rows * sizeof(float));
  pass->mat_c = (float *)malloc(Rows * Rows * sizeof(float));
  pass->Rows = Rows;

  //intialize the matrice's
  for (i = 0; i < Rows; i++)
  {
    for (j = 0; j < Rows; j++)
    {
      pass->mat_a[Rows * i + j] = i + 1; //rand()%10;//i+1
      pass->mat_b[Rows * i + j] = i + 1; //rand()%10;//i+1
      pass->mat_c[Rows * i + j] = 0;
    }
  }

  if (strcmp(argv[1], "m") == 0)
    threadRipper(pass, cores);
  else if (strcmp(argv[1], "s") == 0)
  {
    for (int i = 0; i < Rows; i++)
    {
      for (int j = 0; j < Rows; j++)
      {
        for (int k = 0; k < Rows; k++)
        {
          pass->mat_c[i * Rows + j] += pass->mat_a[i * Rows + k] *
                                    pass->mat_b[k * Rows + j];
        }
      }
    }
  }

  free(pass->mat_a);
  free(pass->mat_b);
  free(pass->mat_c);
  return 0;
}
