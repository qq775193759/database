#include "final.h"
#include <ctime>
#include <fstream>

using namespace std;

Space::Space()
{

}

void Space::addFile(const char* filename)
{
    time_t start_t,end_t;
    start_t = time(&start_t);
    cout<<"begin: readfile"<<endl;
    ifstream fin(filename,ios::in);
    int no;
    int x,y;
    while(fin>>no)
    {
        fin>>x>>y;
        Point p(no,x,y);
        global_points.push_back(p);
    }
    cout<<"end:readfile  all points: "<<global_points.size()<<endl;
    end_t = time(&end_t);
    cout <<"readfile use time:"<< end_t - start_t <<endl;
    vector<Track> temp_track(no+1);
    global_tracks = temp_track;
    fin.close();
}

void Space::buildAll()
{
    time_t start_t,end_t;
    start_t = time(&start_t);
    for(int i=0;i<global_points.size();i++)
    {
        int x=global_points[i].x;
        int y=global_points[i].y;
        int x_rank = (x-BASE_X)/GRID_X;
        int y_rank = (y-BASE_Y)/GRID_Y;
        int no=global_points[i].no;
        global_tracks[no].add(i);
        for(int r=-1;r<=1;r++)
            for(int s=-1;s<=1;s++)
                ranges[x_rank+r][y_rank+s].add(i);
    }
    end_t = time(&end_t);
    cout <<"buildall use time:"<< end_t - start_t <<endl;
}

void Space::checkAll()
{
    time_t start_t,end_t;
    start_t = time(&start_t);
    
    end_t = time(&end_t);
    cout <<"checkall use time:"<< end_t - start_t <<endl;
}

int Space::checkDis(int a,int b)
{
    int x1 = global_points[a].x;
    int y1 = global_points[a].y;
    int x2 = global_points[b].x;
    int y2 = global_points[b].y;
    return 1;
}