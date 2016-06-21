#include <iostream>
#include <vector>

using namespace std;

const int RANGE_X = 350;
const int RANGE_Y = 405;

const int GRID_X = 900;
const int GRID_Y = 1170;

const int BASE_X = 39787000;
const int BASE_Y = 116147000;

const double PI = 3.1415;
const double BIG180 = 180000000.0;
//const double EARTH_RADIUS = 6371.393;
const double EARTH_RADIUS = 6378.137;

const int MAX_POINT = 18887;

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
    int st;
    int en;
    void add(int x){points.push_back(x);}
};

struct Range
{
    vector<int> points;
    void add(int x){points.push_back(x);}
};

struct HashSet
{
    int* co;
    int standart;
    int ones;
    HashSet()
    {
        co = new int[MAX_POINT];
    }
    void reset(int n)
    {
        for(int i=0;i<MAX_POINT;i++)
        {
            co[i] = 0;
        }
        co[n] = -1;
        standart = -1;
        ones = 1;
    }
    inline int addStd()
    {
        int res = ones;
        standart++;
        ones = 0;
        return res;
    }
    inline void set(int n)
    {
        co[n]++;
        ones ++;
    }
    inline int check(int n)
    {
        return (co[n]==standart);
    }
    void print(int n)
    {
        for(int i=0;i<MAX_POINT;i++)
        {
            if(co[i] == standart)
                cout<<n<<" "<<i<<endl;
        }
    }
};

class Space
{
    //data
    vector<Point> global_points;
    vector<Track> global_tracks;
    Range ranges[RANGE_X][RANGE_Y];
    int size;

    HashSet hashset;

public:
    Space();
    void addFile(const char* filename);
    void buildAll();
    void checkAll();
    int check(int a,int b);
    int checkDis(int a,int b);
    int checkDis(int x1,int y1,int x2,int y2);
    int simDis(int a,int b);

    void printDebug();
};