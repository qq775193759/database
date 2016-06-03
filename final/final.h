#include <iostream>
#include <vector>

using namespace std;

const int RANGE_X = 100;
const int RANGE_Y = 100;

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
};

struct Range
{
    vector<int> points;
    vector<int> owners;
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
    void buildTrack();
    void buildRange();
    void buildAll();
};