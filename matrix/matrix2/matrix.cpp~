#include <iostream>
#include <mutex>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
using namespace std;

//passed arguments for threads structure
struct arg_struct{
  float* mat_a;
  float* mat_b;
  float* mat_c;
  int N;
};

mutex mtx;

//only edited/saved under a mutex lock
int CURRENT_ROW=0;

//The multiply function is passed the args structure by reference
//the function saves the current row that needs to be processed by the
//program and then increments it so that other threads won't do computations
//on the same row.
void *multiply(void *passed) 
{
  struct arg_struct *pass = (struct arg_struct *)passed;
  int thread_row=0;

  //the row must be saved and edited under a mutex
  //so that the two threads aren't doing the same
  //row at the same time.
  mtx.lock();
  thread_row=CURRENT_ROW;
  CURRENT_ROW++;
  mtx.unlock();

  //the function will not quite until all rows have been computed
  while(thread_row < pass->N)
    {
      for (int j = 0; j < pass->N; j++) 
	{ 
	  for (int k = 0; k < pass->N; k++)
	    {
	      pass->mat_c[thread_row*pass->N+j] += pass->mat_a[thread_row*pass->N+k] *  
		pass->mat_b[k*pass->N+j];
	    }
	}
      
      mtx.lock();
      thread_row=CURRENT_ROW;
      CURRENT_ROW++;
      mtx.unlock();
    }
  

}

//the threadRipper function spawns the number of threads asked for by the user
//and passing the reference of each of the matrices into the threads for
//computation.
void threadRipper(float* mat_a,float* mat_b,float* mat_c,int N,int cores)
{
  struct arg_struct pass;
  pass.mat_a = mat_a;
  pass.mat_b = mat_b;
  pass.mat_c = mat_c;
  pass.N = N;
  
  int num_cores = cores;
  //cout<<cores<<endl;
  pthread_t threads[num_cores];
  
  for(int i=0;i<num_cores;i++)
    {
      pthread_create(&threads[i],NULL,multiply,(void *)&pass);
    }

  for(int i=0;i<num_cores;i++)
    {
      pthread_join(threads[i],NULL);
    }
}

int main(int argc,char *argv[])
{
  int i,j;
  int N = atoi(argv[2]);
  srand(time(NULL));
  struct arg_struct* pass = (struct arg_struct*)malloc(sizeof(struct arg_struct));
  pass->mat_a = (float*)malloc(N*N*sizeof(float));
  pass->mat_b = (float*)malloc(N*N*sizeof(float));
  pass->mat_c = (float*)malloc(N*N*sizeof(float));
  pass->N = N;
  
  //intialize the matrice's
  for (i = 0; i < N; i++) 
    { 
      for (j = 0; j < N; j++)
	{
	  pass->mat_a[N*i+j] = i+1;//rand()%10;//i+1
	  pass->mat_b[N*i+j] = i+1;//rand()%10;//i+1
	   pass->mat_c[N*i+j] = 0;
	}
    }

  if(strcmp(argv[1],"m")==0)
    threadRipper(pass->mat_a,pass->mat_b,pass->mat_c,N,4);
  else if(strcmp(argv[1],"s")==0)
    {
      for(int i = 0; i < N; i++)
	{
	  for (int j = 0; j < N; j++) 
	    { 
	      for (int k = 0; k < N; k++)
		{
		  pass->mat_c[i*N+j] += pass->mat_a[i*N+k] *  
		    pass->mat_b[k*N+j];
		}
	    }
	}
    }
  
  //to check the correctness of the matrix operation
  //we can print out the computed matrix.
  // cout << "Result matrix is \n"; 
  // for (i = 0; i < N; i++) 
  //   { 
  //     for (j = 0; j < N; j++) 
  // 	cout << pass->mat_c[N*i+j] << " "; 
  //     cout << "\n"; 
  //   }

  free(pass->mat_a);
  free(pass->mat_b);
  free(pass->mat_c);
  return 0;
}
