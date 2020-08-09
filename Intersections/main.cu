#include <iostream>
#include <vector>
#include <map>
#include <pthread.h>
#include <fstream>
#include "Intersection.cu"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>

using namespace std;

vector<string> split(const string &str, const string &delim);



int main(){

  //struct arg_struct pass;
  int lineNumber = 0;
  vector<Line> localVector;

  string line;
  //string path = "test/testSmall.csv";
  string path = "test/triangles.csv";
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
      localVector.push_back(makeLineHost(lineNumber,
                                  stod(tempSplit.at(0)), stod(tempSplit.at(1)),
                                  stod(tempSplit.at(2)), stod(tempSplit.at(3))));
      lineNumber++;
    }
    myfile.close();
  }
  string temp;

  // cout << "myvector size : " << localVector.capacity() << "\n";
  localVector.shrink_to_fit();
  cout << "myvector size shrunk: " << localVector.capacity() << "\n";

  //int i = 0;

  const int count = localVector.size();
  const int countLinePair = 40000;
  const int size = count * sizeof(Line);
  const int sizeLinePair = countLinePair * sizeof(linePair);
  printf("pairs : %i\n",sizeLinePair);
  printf("lines : %i\n",size);
  Line *dLines;
  linePair* dpairs;
  cudaMalloc(&dpairs,sizeLinePair);
  cudaMalloc(&dLines,size);

  Line* hLines = &localVector[0];
  
  cudaMemcpy(dLines,hLines,size,cudaMemcpyHostToDevice);
//   cout<<hLines[0].left_point.x<<"\n";
//   cout<<hLines2[0].left_point.x<<"\n";
  // cout<<"Blocks : "<<(count/1024)+1;
  int* counter = 0;
  int* mutex = 0;
  runner<<<(count/1024)+1,1024>>>(dLines,count,dpairs,counter,mutex);//,"test/points.csv");

  cudaDeviceSynchronize();

  printf("host quite!!");
  cudaFree(dLines);
  cudaFree(dpairs);
  free(hLines);
  localVector = vector<Line>();
  //i = runner(localVector,"test/points.csv",numCores);
//   int* c = 0;
//   cudaMemcpy(counter,c,sizeof(counter),cudaMemcpyDeviceToHost);

//   cout << "Number of Intersections Found : " << c << "\n";
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

