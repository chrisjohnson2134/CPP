#include <iostream>
#include <mutex>
#include <pthread.h>
using namespace std;

struct arg_struct{
  float* mat_a;
  float* mat_b;
  float* mat_c;
  int N;
};

mutex mtx;
//for now use N
//#define N 10
int CURRENT_ROW=0;

//#define ij(i,j) (N*i+j)//superfast macro to lookup on 1d array

void *multiply(void *passed) 
{
  struct arg_struct *pass = (struct arg_struct *)passed;
  int thread_row=0;
  
  mtx.lock();
  thread_row=CURRENT_ROW;
  CURRENT_ROW++;
  mtx.unlock();

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

void threadRipper(float* mat_a,float* mat_b,float* mat_c,int N)
{
  struct arg_struct pass;// = (struct arg_struct*)sizeof(struct arg_struct);
  pass.mat_a = mat_a;
  pass.mat_b = mat_b;
  pass.mat_c = mat_c;
  pass.N = N;
  
  int num_cores = 1;
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

int main()
{
  int i,j;
  int N = 500;
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
	  pass->mat_a[N*i+j] = i+1;
	  pass->mat_b[N*i+j] = i+1;
	  pass->mat_c[N*i+j] = 0;
	}
    }

  threadRipper(pass->mat_a,pass->mat_b,pass->mat_c,N);
  
  
  cout << "Result matrix is \n"; 
  for (i = 0; i < N; i++) 
    { 
      for (j = 0; j < N; j++) 
	cout << pass->mat_c[N*i+j] << " "; 
      cout << "\n"; 
    }

  free(pass->mat_a);
  free(pass->mat_b);
  free(pass->mat_c);
  return 0;
}
