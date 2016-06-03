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
    fin.close();
}

void Space::buildTrack()
{
    for(auto it=global_points.begin();it!=global_points.end();i++)
    {
        it->no;
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