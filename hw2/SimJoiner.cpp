#include "SimJoiner.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>

using namespace std;

SimJoiner::SimJoiner() {
}

SimJoiner::~SimJoiner() {
}

void SimJoiner::readFile(const char *filename1, const char *filename2)
{
	words1.clear();
	words2.clear();
	string s;
	ifstream fin1(filename1);
	while(getline(fin1,s))
	{
		words1.push_back(s);
	}
	fin1.close();
	ifstream fin2(filename2);
	while(getline(fin2,s))
	{
		words2.push_back(s);
	}
	fin2.close();
}

int SimJoiner::joinJaccard(const char *filename1, const char *filename2, double threshold, vector<JaccardJoinResult> &result) {
    result.clear();
    return SUCCESS;
}

int SimJoiner::joinED(const char *filename1, const char *filename2, unsigned threshold, vector<EDJoinResult> &result) {
    result.clear();
    readFile(filename1, filename2);
    return SUCCESS;
}

int SimJoiner::checkED(const string &a, const string &b, int threshold)
{
	int a_size = a.size();
	int b_size = b.size();
	int delta_ab = a_size - b_size;
	if(abs(delta_ab) > threshold) return 0;

	int min_size = a_size;
	int wide_size = 2*threshold + 1; 
	int ed[min_size][wide_size];
	int a_equel_b;

	for(int j=0; j<threshold; j++)
		ed[0][j] = threshold - j;
	for(int j=threshold; j<wide_size; j++)
		ed[0][j] = min(j - threshold + 1 - (a[0] == b[j - threshold]), ed[0][j-1]+1);

	for(int i=1; i<min_size; i++)
	{
		int bigger_than_threshold=0;
		a_equel_b = (i>=threshold) && (a[i] == b[i - threshold]);
		ed[i][0] = min(ed[i-1][0] + 1 - a_equel_b,  ed[i-1][1]+1);
		if(ed[i][0] + abs(delta_ab - threshold) > threshold) bigger_than_threshold++;
		for(int j=1; j<wide_size-1; j++)
		{
			a_equel_b = ((i + j - threshold)>=0 && (i + j - threshold)<b_size) && (a[i] == b[i + j - threshold]);
			ed[i][j] = min(ed[i-1][j] + 1 - a_equel_b, min(ed[i][j-1]+1, ed[i-1][j+1]+1));
			if(ed[i][j] + abs(delta_ab - threshold + j) > threshold) bigger_than_threshold++;
		}
		a_equel_b = ((i + threshold) < b_size) && (a[i] == b[i + threshold]);
		ed[i][wide_size-1] = min(ed[i-1][wide_size-1] + 1 - a_equel_b, ed[i][wide_size-2]+1);
		if(ed[i][wide_size-1] + abs(delta_ab + threshold) > threshold) bigger_than_threshold++;
		if(bigger_than_threshold == wide_size) return 0;
	}


    if(ed[a_size-1][b_size+threshold-a_size] > threshold) return 0;
    ed_res = ed[a_size-1][b_size+threshold-a_size];
    return 1;
}