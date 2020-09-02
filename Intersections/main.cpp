#include <iostream>
#include <vector>
#include <map>
#include <mutex>
#include <pthread.h>
#include <fstream>
#include "Intersection.cpp"

using namespace std;

vector<string> split(const string &str, const string &delim);

int main(){
  
  int numCores = 8;
  //temporarly using vector

  //struct arg_struct pass;
  int lineNumber = 0;

  vector<Line> localVector;

  string line;
  string path = "test/testSmall.csv";
  // string path = "test/triangles.csv";
  ifstream myfile(path);
  cout << "Lines File Path: " << path << "\n";
  ofstream omyfile;
  omyfile.open("test/points.csv", ofstream::out | ofstream::trunc);
  omyfile.close();
  vector<string> tempSplit;
  if (myfile.is_open())
  {
    while (getline(myfile, line))
    {
      tempSplit = split(line, ",");
      localVector.push_back(makeLine(lineNumber,
                                  stod(tempSplit.at(0)), stod(tempSplit.at(1)),
                                  stod(tempSplit.at(2)), stod(tempSplit.at(3))));
      lineNumber++;
    }
    myfile.close();
  }
  string temp;

  // cout << "myvector size : " << localVector.capacity() << "\n";
  localVector.shrink_to_fit();
  // cout << "myvector size shrunk: " << localVector.capacity() << "\n";


  int i = 0;

  i = runner(localVector,"test/points.csv",numCores);

  cout << "Number of Intersections Found : " << i << "\n";
}


vector<string> split(const string &str, const string &delim)
{
  std::vector<std::string> tokens;
  size_t prev = 0, pos = 0;
  do
  {
    pos = str.find(delim, prev);
    if (pos == std::string::npos)
      pos = str.length();
    std::string token = str.substr(prev, pos - prev);
    if (!token.empty())
      tokens.push_back(token);
    prev = pos + delim.length();
  } while (pos < str.length() && prev < str.length());
  return tokens;
}