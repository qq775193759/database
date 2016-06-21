#include "final.h"
#include <cmath>
#include <cstdio>

using namespace std;

int main()
{
    Space* space = new Space;
    //cout<<space.checkDis(39830929,116392052,39830441,116379280)<<endl;
    //cout<<space.checkDis(39830021,116364861,39830441,116379280)<<endl;


    space->addFile("output_all.dat.plain.txt");
    space->buildAll();
    space->checkAll();
    //cout<<space.check(14953,1717)<<endl;
    //cout<<space.check(13343,5392)<<endl;
    //cout<<space.check(17896,6986)<<endl;
    //cout<<space.check(17896,17243)<<endl;
    // cout<<space.checkDis(11,12)<<endl;
    // cout<<space.checkDis(12,13)<<endl;
    // cout<<space.checkDis(13,14)<<endl;
    // cout<<space.simDis(11,12)<<endl;
    // cout<<space.simDis(12,13)<<endl;
    // cout<<space.simDis(13,14)<<endl;
    //space.printDebug();
}