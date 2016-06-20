#include <iostream>
#include <vector>

using namespace std;

const int RANGE_X = 350;
const int RANGE_Y = 350;

const int GRID_X = 901;
const int GRID_Y = 1408;

const int BASE_X = 39787000;
const int BASE_Y = 116147000;

struct Point
{
    int no;
    int x;
    int y;
    Point(int no, int x, int y):no(no),x(x),y(y){}
};

struct Track
{
    vector<int> points;
    void add(int x){points.push_back(x);}
};

struct Range
{
    vector<int> points;
    void add(int x){points.push_back(x);}
};

class Space
{
    //data
    vector<Point> global_points;
    vector<Track> global_tracks;
    Range ranges[RANGE_X][RANGE_Y];

public:
    Space();
    void addFile(const char* filename);
    void buildAll();
};