#include <iostream>
#include <vector>
#include <pthread.h>
using namespace std;

// struct Point{
//   double x;
//   double y;
// };
struct Point 
{ 
    double x; 
    double y; 
}; 

struct Line{
  Point left_point;
  Point right_point;
};

struct linePair{
  Line l1;
  Line l2;
};

Line makeLine(int x1,int y1,int x2,int y2);
Point makePoint(int x,int y);
void printPoint(Point passPoint);
void printLine(Line passLine);
bool doIntersect(Point p1, Point q1, Point p2, Point q2); 

int main()
{
  int numCores = 8;
  //temporarly using vector
  std::vector<Line> myvector;
  //struct arg_struct pass;

  myvector.push_back(makeLine(0,0,4,4));
  myvector.push_back(makeLine(0,4,4,0));
  myvector.push_back(makeLine(0,2,2,0));
  myvector.push_back(makeLine(3,0,5,2));

  int index = 0;
  for(int i = 0; i < 4;i++)
  {
    for(int j = 0; j < 4;j++)
    {
       if(i != j)
       {
         if(doIntersect(myvector.at(i).left_point,myvector.at(i).right_point,
             myvector.at(j).left_point,myvector.at(j).right_point))
             {
               printLine(myvector.at(i));printLine(myvector.at(j));
               printf("\n");
             }
       }
    }
  }
    

  

  // printPoint(lineIntersection(myvector.at(4),myvector.at(5)));

  // pthread_t threads[numCores];

  // for(int i=0;i<numCores;i++)
  //   {
  //     pthread_create(&threads[i],NULL,multiply,(void *)&pass);
  //   }

  // for(int i=0;i<numCores;i++)
  //   {
  //     pthread_join(threads[i],NULL);
  //   }

}

// void *multiply(void *passed) 
// {
//   struct arg_struct *pass = (struct arg_struct *)passed;
//   //pass->N

 //}


Point makePoint(double x,double y)
{
  return{x,y};
}    

Line makeLine(int x1,int y1,int x2,int y2)
{
  Point left_point;
  Point right_point;
  if(x1 < x2){
    left_point.x = x1;
    left_point.y = y1;
    right_point.x = x2;
    right_point.y = y2;
  }else{
    left_point.x = x2;
    left_point.y = y2;
    right_point.x = x1;
    right_point.y = y1;
  }
  return {left_point,right_point};
}

void printPoint(Point passPoint)
{
    printf("x: %f y: %f \n",
	 passPoint.x,passPoint.y);
}

void printLine(Line passLine)
{
  printf("x1: %f y1: %f :: x2: %f y2: %f",
	 passLine.left_point.x,passLine.left_point.y,
	 passLine.right_point.x,passLine.right_point.y);
}


  
// Given three colinear points p, q, r, the function checks if 
// point q lies on line segment 'pr' 
bool onSegment(Point p, Point q, Point r) 
{ 
    if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) && 
        q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y)) 
       return true; 
  
    return false; 
} 
  
// To find orientation of ordered triplet (p, q, r). 
// The function returns following values 
// 0 --> p, q and r are colinear 
// 1 --> Clockwise 
// 2 --> Counterclockwise 
int orientation(Point p, Point q, Point r) 
{ 
    // See https://www.geeksforgeeks.org/orientation-3-ordered-points/ 
    // for details of below formula. 
    int val = (q.y - p.y) * (r.x - q.x) - 
              (q.x - p.x) * (r.y - q.y); 
  
    if (val == 0) return 0;  // colinear 
  
    return (val > 0)? 1: 2; // clock or counterclock wise 
} 
  
// The main function that returns true if line segment 'p1q1' 
// and 'p2q2' intersect. 
bool doIntersect(Point p1, Point q1, Point p2, Point q2) 
{ 
    // Find the four orientations needed for general and 
    // special cases 
    int o1 = orientation(p1, q1, p2); 
    int o2 = orientation(p1, q1, q2); 
    int o3 = orientation(p2, q2, p1); 
    int o4 = orientation(p2, q2, q1); 
  
    // General case 
    if (o1 != o2 && o3 != o4) 
        return true; 
  
    // Special Cases 
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1 
    if (o1 == 0 && onSegment(p1, p2, q1)) return true; 
  
    // p1, q1 and q2 are colinear and q2 lies on segment p1q1 
    if (o2 == 0 && onSegment(p1, q2, q1)) return true; 
  
    // p2, q2 and p1 are colinear and p1 lies on segment p2q2 
    if (o3 == 0 && onSegment(p2, p1, q2)) return true; 
  
     // p2, q2 and q1 are colinear and q1 lies on segment p2q2 
    if (o4 == 0 && onSegment(p2, q1, q2)) return true; 
  
    return false; // Doesn't fall in any of the above cases 
} 

