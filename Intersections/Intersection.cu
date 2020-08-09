#include <iostream>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>

// #include <stdio.h>

//set datafile separator ","
//plot 'test/testSmall.csv' using 1:2:($3-$1):($4-$2) with vectors nohead, "test/points.csv" pt 7 ps 2
//plot 'test/triangles.csv' using 1:2:($3-$1):($4-$2) with vectors nohead, "test/points.csv" pt 7 ps 2

// struct arg_struct
// {
//   std::string outputPath;
// };



struct Point
{
  double x;
  double y;
};

struct Line
{
  int id;
  Point left_point;
  Point right_point;
};

struct linePair
{
  Line l1;
  Line l2;
};


__device__
Line makeLine(int id, int x1, int y1, int x2, int y2);
// void printLine(Line passLine);
// // bool doIntersect(Point p1, Point q1, Point p2, Point q2);
__device__
bool get_line_intersection(Point p0, Point p1,
                           Point p2, Point p3, double *i_x, double *i_y);
// std::vector<std::string> split(const std::string &str, const std::string &delim);
// void *multiply(void *passed);
Line makeLineHost(int id, int x1, int y1, int x2, int y2);
__device__
linePair makeLinePair(Line l1,Line l2);
// //Share Fields
std::map<std::string, int> mymap;
// int sharedI = 0;

// __device__ int* mutex = 0;

__device__ void lock(int* mutex) {
    /* compare mutex to 0.
       when it equals 0, set it to 1
       we will break out of the loop after mutex gets set to 1 */
    while (atomicCAS(mutex, 0, 1) != 0) {
        /* do nothing */
    }
}

__device__ void unlock(int* mutex) {
    atomicExch(mutex, 0);
}






/////////////////!!!!!!!!!!!!!!!!!!!!!!!!!///////////////////////////////
//USE THE BLOCK AND THREAD to be your for loop INDEXER////////////////////
//////////////////////////////////////////////////////////////////////////
__global__
void runner(Line *myvector,int size,linePair* pairs,int* counter,int* mutex)//,int* points)//, std::string outputPath)
{
  if(blockIdx.x == 0 && threadIdx.x == 0)
  {
    printf("true");
    mutex = 0;
    counter = 0;
  }
  // printf("size : %i \n",size);
//   std::string temp;
  double x, y;
//   std::ofstream omyfile;
  int i = 0;
//   omyfile.open("test/points.csv", std::ios_base::app);

    i = (blockIdx.x *1024) + threadIdx.x;
    //atomicAdd(&counter, 1);
    if(i == 15000)

      printf("thread : %i\n",i );
    for (int j = 0; j < size; j++)
    {
      // if(j == 10)
      //   printf("j : %i\n",j);
      if (i != j)
      {
        if (get_line_intersection(myvector[i].left_point, myvector[i].right_point,
                                  myvector[j].left_point, myvector[j].right_point, &x, &y))
        {
            // (*points)++;
            
          // printf("counter : %i\n",counter);
          //cout << "intersection found." << endl;
          lock(mutex);
          // if (myvector[i].id < myvector[j].id)
          //   pairs[counter] = makeLinePair(myvector[i],myvector[j]);
          // else
          //   pairs[counter] = makeLinePair(myvector[j],myvector[i]);

            // atomicAdd(&counter, 1);
            (*counter)++;
            unlock(mutex);
        //   mtxMap.lock();
        //   if (mymap.find(temp) == mymap.end()) //O(log(n)) disable if you want more speed up I guess
        //   {
            // omyfile << std::to_string(x) << "," << std::to_string(y) << "\n";
            // mymap.insert(std::pair<std::string, int>(temp, 0));
        //   }
        //   mtxMap.unlock();
        }
      }
    }

//   omyfile.close();
}


// void *multiply(void *passed)
// {

//   return NULL;
// }

// Point makePoint(double x, double y)
// {
//   return {x, y};
// }

__device__
Line makeLine(int id, int x1, int y1, int x2, int y2)
{
  Point left_point;
  Point right_point;
  if (x1 < x2)
  {
    left_point.x = x1;
    left_point.y = y1;
    right_point.x = x2;
    right_point.y = y2;
  }
  else
  {
    left_point.x = x2;
    left_point.y = y2;
    right_point.x = x1;
    right_point.y = y1;
  }
  return {id, left_point, right_point};
}

__device__
linePair makeLinePair(Line l1,Line l2)
{
  return {l1,l2};
}

// void printPoint(Point passPoint)
// {
//   printf("x: %f y: %f \n",
//          passPoint.x, passPoint.y);
// }

// void printLine(Line passLine)
// {
//   printf("x1: %f y1: %f :: x2: %f y2: %f",
//          passLine.left_point.x, passLine.left_point.y,
//          passLine.right_point.x, passLine.right_point.y);
// }

// Returns 1 if the lines intersect, otherwise 0. In addition, if the lines
// intersect the intersection point may be stored in the doubles i_x and i_y.
__device__
bool get_line_intersection(Point p0, Point p1,
                           Point p2, Point p3, double *i_x, double *i_y)
{
  // cout << "intersect function" << endl;
  Point s1, s2;
  s1.x = p1.x - p0.x;
  s1.y = p1.y - p0.y;
  s2.x = p3.x - p2.x;
  s2.y = p3.y - p2.y;

  double s, t;
  s = (-s1.y * (p0.x - p2.x) + s1.x * (p0.y - p2.y)) / (-s2.x * s1.y + s1.x * s2.y);
  t = (s2.x * (p0.y - p2.y) - s2.y * (p0.x - p2.x)) / (-s2.x * s1.y + s1.x * s2.y);

  if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
  {
    // Collision detected
    if (i_x != NULL)
      *i_x = p0.x + (t * s1.x);
    if (i_y != NULL)
      *i_y = p0.y + (t * s1.y);
    return true;
  }

  return false; // No collision
}


Line makeLineHost(int id, int x1, int y1, int x2, int y2)
{
  Point left_point;
  Point right_point;
  if (x1 < x2)
  {
    left_point.x = x1;
    left_point.y = y1;
    right_point.x = x2;
    right_point.y = y2;
  }
  else
  {
    left_point.x = x2;
    left_point.y = y2;
    right_point.x = x1;
    right_point.y = y1;
  }
  return {id, left_point, right_point};
}