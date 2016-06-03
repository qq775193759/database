#include "final.h"
#include <fstream>

using namespace std;

Space::Space()
{

}

void Space::addFile(const char* filename)
{
    ifstream fin(filename);
    int no;
    double x,y;
    while(fin>>no)
    {
        fin>>x>>y;
        if(no == 26350)
            cout<<no<<x<<y<<endl;
    }
}

void Space::buildRange()
{
    
}