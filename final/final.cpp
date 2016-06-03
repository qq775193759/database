#include "final.h"
#include <fstream>

using namespace std;

Space::Space()
{

}

void Space::addFile(const char* filename)
{
    ifstream fin(filename,ios::in);
    int no;
    int x,y;
    while(fin>>no)
    {
        fin>>x>>y;
    }
}

void Space::buildRange()
{
    
}