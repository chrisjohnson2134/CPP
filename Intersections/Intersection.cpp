#include <iostream>
#include <vector>
#include <map>
#include <mutex>
#include <pthread.h>
#include <fstream>

//set datafile separator ","
//plot 'test/testSmall.csv' using 1:2:($3-$1):($4-$2) with vectors nohead, "test/points.csv" pt 7 ps 2
//plot 'test/triangles.csv' using 1:2:($3-$1):($4-$2) with vectors nohead, "test/points.csv" pt 7 ps 2

struct arg_struct
{
  std::string outputPath;
};

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

std::mutex mtx;
std::mutex mtxMap;

Line makeLine(int id, int x1, int y1, int x2, int y2);
void printLine(Line passLine);
bool doIntersect(Point p1, Point q1, Point p2, Point q2);
bool get_line_intersection(Point p0, Point p1,
                           Point p2, Point p3, double *i_x, double *i_y);
std::vector<std::string> split(const std::string &str, const std::string &delim);
void *multiply(void *passed);

//Share Fields
std::vector<Line> myvector;
std::map<std::string, int> mymap;
int sharedI = 0;

int runner(std::vector<Line> &pVector, std::string outputPath, int cores)
{
  int numCores = cores;
  myvector = pVector;

  int index = 0;
  int counter = 0;

  pthread_t threads[numCores];
  struct arg_struct *pass;
  for (int i = 0; i < numCores; i++)
  {
    pthread_create(&threads[i], NULL, multiply, (void *)&pass);
  }

  for (int i = 0; i < numCores; i++)
  {
    pthread_join(threads[i], NULL);
  }

  int i = 0;
  std::cout << "mymap size : " << sizeof(mymap) << "\n";
  for (auto it = mymap.cbegin(); it != mymap.cend(); ++it)
  {
    i++;
  }
  std::cout << "Number of Intersections Found : " << i << "\n";
  return i;
}

void *multiply(void *passed)
{
  std::string temp;
  double x, y;
  std::ofstream omyfile;
  int i = 0;
  omyfile.open("test/points.csv", std::ios_base::app);
  while (sharedI < myvector.size())
  {
    mtx.lock();
    i = sharedI;
    sharedI++;
    mtx.unlock();

    for (int j = 0; j < myvector.size(); j++)
    {
      if (i != j)
      {
        if (get_line_intersection(myvector.at(i).left_point, myvector.at(i).right_point,
                                  myvector.at(j).left_point, myvector.at(j).right_point, &x, &y))
        {
          //cout << "intersection found." << endl;
          if (myvector.at(i).id < myvector.at(j).id)
            temp = std::to_string(myvector.at(i).id) + ":" + std::to_string(myvector.at(j).id);
          else
            temp = std::to_string(myvector.at(j).id) + ":" + std::to_string(myvector.at(i).id);

          mtxMap.lock();
          if (mymap.find(temp) == mymap.end()) //O(log(n)) disable if you want more speed up I guess
          {
            omyfile << std::to_string(x) << "," << std::to_string(y) << "\n";
            mymap.insert(std::pair<std::string, int>(temp, 0));
          }
          mtxMap.unlock();
        }
      }
    }
  }
  omyfile.close();
  return NULL;
}

Point makePoint(double x, double y)
{
  return {x, y};
}

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

void printPoint(Point passPoint)
{
  printf("x: %f y: %f \n",
         passPoint.x, passPoint.y);
}

void printLine(Line passLine)
{
  printf("x1: %f y1: %f :: x2: %f y2: %f",
         passLine.left_point.x, passLine.left_point.y,
         passLine.right_point.x, passLine.right_point.y);
}

// Returns 1 if the lines intersect, otherwise 0. In addition, if the lines
// intersect the intersection point may be stored in the doubles i_x and i_y.
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
