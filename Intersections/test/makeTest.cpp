#include <iostream>
#include <fstream>
using namespace std;

//set datafile separator ","
//plot 'test/testSmall.csv' using 1:2:($3-$1):($4-$2) with vectors nohead, "test/points.csv" pt 7 ps 2


string generateTriangle(int triangleNumber);

int main()
{
    string path = "triangles.csv";
    cout << path;

    ofstream omyfile;
    omyfile.open(path,ofstream::out | ofstream::trunc);
    omyfile.close();
    omyfile.open(path, ios_base::app);
    for(int i = 0; i < 2000; i++)
    {
        omyfile << generateTriangle(i);
    }
    omyfile << "0,0,0,0";
    omyfile.close();
}

string generateTriangle(int triangleNumber)
{
    int i = triangleNumber;
    string line1 = to_string((i*3)+1) + "," + to_string(1) + "," + to_string((i*3)+2) + "," + to_string(2) + "\n";
    string line2 = to_string((i*3)+1) + "," + to_string(1) + "," + to_string((i*3)+3) + "," + to_string(1) + "\n";
    string line3 = to_string((i*3)+2) + "," + to_string(2) + "," + to_string((i*3)+3) + "," + to_string(1) + "\n";
    return line1 + line2 + line3 ;
}