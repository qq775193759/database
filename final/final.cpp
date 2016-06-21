#include "final.h"
#include <ctime>
#include <cstdio>
#include <cmath>

using namespace std;

Space::Space()
{

}

void Space::addFile(const char* filename)
{
    time_t start_t,end_t;
    start_t = time(&start_t);
    cout<<"begin: readfile"<<endl;
    FILE* stream = fopen(filename,"r");
    int no;
    int x,y;
    while(fscanf(stream,"%d",&no)!=-1)
    {
        fscanf(stream,"%d %d",&x,&y);
        Point p(no,x,y);
        global_points.push_back(p);
    }
    cout<<"end:readfile  all points: "<<global_points.size()<<endl;
    end_t = time(&end_t);
    cout <<"readfile use time:"<< end_t - start_t <<endl;
    size = no+1;
    vector<Track> temp_track(size);
    global_tracks = temp_track;
}

void Space::buildAll()
{
    time_t start_t,end_t;
    start_t = time(&start_t);
    //body
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
    int sum = 0;
    for(int i=0;i<size;i++)
    {
        global_tracks[i].st = sum;
        sum += global_tracks[i].points.size();
        global_tracks[i].en = sum;
    }
    //body
    end_t = time(&end_t);
    cout <<"buildall use time:"<< end_t - start_t <<endl;
}

int Space::check(int a,int b)
{
    int st = global_tracks[a].st;
    int en = global_tracks[a].en;
    int b_st = global_tracks[b].st;
    int b_en = global_tracks[b].en;
    for(int i=st;i<en;i++)
    {
        int ok = 0;
        for(int j=b_st;j<b_en;j++)
        {
            if(simDis(i,j))
            {
                ok = 1;
                break;
            }
        }
        if(ok == 0) return 0;
    }
    return 1;
}

void Space::checkAll()
{
    time_t start_t,end_t;
    start_t = time(&start_t);

    double t1=0,t2=0,t3=0;
    //body
    for(int i=0;i<size;i++)
    {
        hashset.reset(i);
        for(int j=global_tracks[i].st;j<global_tracks[i].en;j++)
        {
            if(hashset.addStd()==0) break;
            int x = global_points[j].x;
            int y = global_points[j].y;
            int x_rank = (x-BASE_X)/GRID_X;
            int y_rank = (y-BASE_Y)/GRID_Y;
            Range &temp_range = ranges[x_rank][y_rank];
            clock_t start_time2 = clock();
            for(int k=0;k<temp_range.points.size();k++)
            {
                int rank_target = temp_range.points[k];
                clock_t start_time3 = clock();
                int temp_no = global_points[rank_target].no;
                clock_t end_time3 = clock();
                t3 +=(end_time3-start_time3); 
                clock_t start_time1 = clock();
                if(hashset.check(temp_no))
                {
                    if(simDis(rank_target,j))
                    {
                        hashset.set(temp_no);
                    }
                }
                clock_t end_time1 = clock();
                t1 +=(end_time1-start_time1); 
            }
            clock_t end_time2 = clock();
            t2 +=(end_time2-start_time2); 
        }
        hashset.addStd();
        hashset.print(i);
    }
    //body
    end_t = time(&end_t);
    cout <<"checkall use time:"<< end_t - start_t <<endl;
    cout<<"t1: "<<t1<<" t2: "<<t2<<" t3: "<<t3<<endl;
}

int Space::checkDis(int x1,int y1,int x2,int y2)
{

    double radLat1 = x1/BIG180 * PI;
    double radLat2 = x2/BIG180 * PI;
    double radLng1 = y1/BIG180 * PI;
    double radLng2 = y2/BIG180 * PI;
    double a = radLat1 - radLat2;
    double b = radLng1 - radLng2;
     
    double dst = 2 * asin((sqrt(pow(sin(a / 2), 2) + cos(radLat1) * cos(radLat2) * pow(sin(b / 2), 2) )));
    dst = dst * EARTH_RADIUS;
    int dis= round(dst * 1000);
    return dis;
}

int Space::checkDis(int a,int b)
{
    int x1 = global_points[a].x;
    int y1 = global_points[a].y;
    int x2 = global_points[b].x;
    int y2 = global_points[b].y;
    if(abs(x1-x2)>GRID_X || abs(y1-y2)>GRID_Y) return 0;
    int dis = checkDis(x1,y1,x2,y2);
    if(dis>100) return 0;
    return 1;
}

int Space::simDis(int a,int b)
{
    int x1 = global_points[a].x;
    int y1 = global_points[a].y;
    int x2 = global_points[b].x;
    int y2 = global_points[b].y;
    int dx = abs(x1-x2);
    int dy = abs(y1-y2);
    if(dx>GRID_X || dy>GRID_Y) return 0;
    double x100 = (double(dx)*0.111194996458);
    double y100 = (double(dy)*0.085499886753);
    int dis = x100*x100 + y100*y100;
    if(dis>10000) return 0;
    return 1;
}

void Space::printDebug()
{
    /*for(int i=0;i<RANGE_X;i++)
        for(int j=0;j<RANGE_Y;j++)
            cout<<ranges[i][j].points.size()<<endl;*/
    for(int i=0;i<size;i++)
    {
        cout<<global_tracks[i].st<<" "<<global_tracks[i].en<<" ";
        cout<<global_tracks[i].points.size()<<endl;
    }
}