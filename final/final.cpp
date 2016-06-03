#include "final.h"
#include <fstream>

using namespace std;

Space::Space()
{

}

void Space::addFile(const char* filename)
{
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
    vector<Track> temp_track(no+1);
    global_tracks = temp_track;
    fin.close();
}

void Space::buildTrack()
{
    for(int i=0;i<global_points.size();i++)
    {
        global_tracks[global_points[i].no].add(i);
    }
}

void Space::buildRange()
{
    
}

void Space::buildAll()
{
    buildTrack();
    buildRange();
}