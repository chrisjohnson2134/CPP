#include <iostream>
#include <vector>
#include <map>
#include <pthread.h>
#include <fstream>
using namespace std;

//set datafile separator ","
//plot 'test/testSmall.csv' using 1:2:($3-$1):($4-$2) with vectors nohead, "test/points.csv" pt 7 ps 2
//plot 'test/triangles.csv' using 1:2:($3-$1):($4-$2) with vectors nohead, "test/points.csv" pt 7 ps 2


// struct Point{
//   double x;
//   double y;
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

Line makeLine(int id, int x1, int y1, int x2, int y2);
Point makePoint(int x, int y);
void printPoint(Point passPoint);
void printLine(Line passLine);
bool doIntersect(Point p1, Point q1, Point p2, Point q2);
bool get_line_intersection(Point p0, Point p1,
                           Point p2, Point p3, double *i_x, double *i_y);
vector<string> split(const string &str, const string &delim);

int main()
{
  int numCores = 8;
  //temporarly using vector
  std::vector<Line> myvector;
  //struct arg_struct pass;
  int lineNumber = 0;

  string line;
  string path = "test/triangles.csv";
  ifstream myfile(path);
  cout<<path;
  ofstream omyfile;
  omyfile.open("test/points.csv",ofstream::out | ofstream::trunc);
  omyfile.close();
  vector<string> tempSplit;
  if (myfile.is_open())
  {
    while (getline(myfile, line))
    {
      tempSplit = split(line, ",");
      //cout<<tempSplit.at(0)<<endl;
      myvector.push_back(makeLine(lineNumber,
                                  stod(tempSplit.at(0)), stod(tempSplit.at(1)),
                                  stod(tempSplit.at(2)), stod(tempSplit.at(3))));
      lineNumber++;
    }
    myfile.close();
  }
  // myvector.push_back(makeLine(0,0,2,1,0));
  // myvector.push_back(makeLine(1,0,0,4,4));
  // myvector.push_back(makeLine(2,0,4,3,0));
  // myvector.push_back(makeLine(3,0,1,3,4));

  string temp;
  map<string, int> mymap;
  double x, y;

  int index = 0;
  int counter  = 0;
  for (int i = 0; i < myvector.size(); i++)
  {
    for (int j = 0; j < myvector.size(); j++)
    {
      if (i != j)
      {counter++;
        //  if(doIntersect(myvector.at(i).left_point,myvector.at(i).right_point,
        //      myvector.at(j).left_point,myvector.at(j).right_point))
        //      {
        //        if(myvector.at(i).id < myvector.at(j).id)
        //         temp = to_string(myvector.at(i).id) + ":"+ to_string(myvector.at(j).id);
        //        else
        //         temp = to_string(myvector.at(j).id) + ":"+ to_string(myvector.at(i).id) ;

        //         mymap.insert(pair<string,int>(temp,0));
        //          //cout<< temp << endl;
        //      }

         //cout << "preintersection test" << endl;
        if (get_line_intersection(myvector.at(i).left_point, myvector.at(i).right_point,
                                  myvector.at(j).left_point, myvector.at(j).right_point, &x, &y))
        {
           //cout << "intersection found." << endl;
          if (myvector.at(i).id < myvector.at(j).id)
            temp = to_string(myvector.at(i).id) + ":" + to_string(myvector.at(j).id);
          else
            temp = to_string(myvector.at(j).id) + ":" + to_string(myvector.at(i).id);
          // cout << "temp made" << endl;
          mymap.insert(pair<string, int>(temp, 0));

          

          // cout << "appended to file" << endl;
          omyfile.open("test/points.csv", ios_base::app);
          omyfile << to_string(x)<< "," << to_string(y) << "\n";
          omyfile.close();
          //cout<< temp << endl;
        }
      }
    }
  }
  cout<<"\n"<<counter<<"\n";
  int i = 0;
  for (auto it = mymap.cbegin(); it != mymap.cend(); ++it)
  {
    i++;
    //cout<<it->first<<endl;
  }
  cout << i;

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

// // Given three colinear points p, q, r, the function checks if
// // point q lies on line segment 'pr'
// bool onSegment(Point p, Point q, Point r)
// {
//   if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
//       q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
//     return true;

//   return false;
// }

// // To find orientation of ordered triplet (p, q, r).
// // The function returns following values
// // 0 --> p, q and r are colinear
// // 1 --> Clockwise
// // 2 --> Counterclockwise
// int orientation(Point p, Point q, Point r)
// {
//   // See https://www.geeksforgeeks.org/orientation-3-ordered-points/
//   // for details of below formula.
//   int val = (q.y - p.y) * (r.x - q.x) -
//             (q.x - p.x) * (r.y - q.y);

//   if (val == 0)
//     return 0; // colinear

//   return (val > 0) ? 1 : 2; // clock or counterclock wise
// }

// // The main function that returns true if line segment 'p1q1'
// // and 'p2q2' intersect.
// bool doIntersect(Point p1, Point q1, Point p2, Point q2)
// {
//   // Find the four orientations needed for general and
//   // special cases
//   int o1 = orientation(p1, q1, p2);
//   int o2 = orientation(p1, q1, q2);
//   int o3 = orientation(p2, q2, p1);
//   int o4 = orientation(p2, q2, q1);

//   // General case
//   if (o1 != o2 && o3 != o4)
//     return true;

//   // Special Cases
//   // p1, q1 and p2 are colinear and p2 lies on segment p1q1
//   if (o1 == 0 && onSegment(p1, p2, q1))
//     return true;

//   // p1, q1 and q2 are colinear and q2 lies on segment p1q1
//   if (o2 == 0 && onSegment(p1, q2, q1))
//     return true;

//   // p2, q2 and p1 are colinear and p1 lies on segment p2q2
//   if (o3 == 0 && onSegment(p2, p1, q2))
//     return true;

//   // p2, q2 and q1 are colinear and q1 lies on segment p2q2
//   if (o4 == 0 && onSegment(p2, q1, q2))
//     return true;

//   return false; // Doesn't fall in any of the above cases
// }

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

vector<string> split(const string &str, const string &delim)
{
  vector<string> tokens;
  size_t prev = 0, pos = 0;
  do
  {
    pos = str.find(delim, prev);
    if (pos == string::npos)
      pos = str.length();
    string token = str.substr(prev, pos - prev);
    if (!token.empty())
      tokens.push_back(token);
    prev = pos + delim.length();
  } while (pos < str.length() && prev < str.length());
  return tokens;
}
