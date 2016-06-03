#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

const int RANGE_X = 100;
const int RANGE_Y = 100;

struct Point
{
    double x;
    double y;
};

struct Track
{
    vector<int> points;
};

struct Range
{
    vector<int> points;
    vector<int> owners;
};

class Space
{
    //data
    vector<Track> tracks;
    Range ranges[RANGE_X][RANGE_Y];

public:
    Space();
    void addFile(const char* filename);
    void buildRange();
};