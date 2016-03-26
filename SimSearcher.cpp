#include "SimSearcher.h"
#include <iostream>
#include <cmath>

using namespace std;

SimSearcher::SimSearcher()
{
}

SimSearcher::~SimSearcher()
{
}

int SimSearcher::createIndex(const char *filename, unsigned q)
{
	return SUCCESS;
}

int SimSearcher::searchJaccard(const char *query, double threshold, vector<pair<unsigned, double> > &result)
{
	result.clear();
	return SUCCESS;
}

int SimSearcher::searchED(const char *query, unsigned threshold, vector<pair<unsigned, unsigned> > &result)
{
	result.clear();
	return SUCCESS;
}

int SimSearcher::checkED(const string &a, const string &b, int threshold)
{
	if(abs(a.size() - b.size()) > threshold) return 0;
	
	int min_size = a.size();
	int wide_size = 2*threshold + 1; 
	int ed[min_size][wide_size];
	int a_equel_b;

	for(int j=0; j<threshold; j++)
		ed[0][j] = threshold + 1;
	for(int j=threshold; j<wide_size; j++)
		ed[0][j] = min(j - threshold + 1 - (a[0] == b[j - threshold]), ed[0][j-1]+1);

	for(int i=1; i<min_size; i++)
	{
		int bigger_than_threshold=0;
		a_equel_b = (i>=threshold) ? (a[i] == b[i - threshold]):0;
		ed[i][0] = min(ed[i-1][0] + 1 - a_equel_b,  ed[i-1][1]+1);
		if(ed[i][0] > threshold) bigger_than_threshold++;
		for(int j=1; j<wide_size-1; j++)
		{
			a_equel_b = ((i + j - threshold)>=0 && (i + j - threshold)<b.size()) ? (a[i] == b[i + j - threshold]):0;
			ed[i][j] = min(ed[i-1][j] + 1 - a_equel_b, min(ed[i][j-1]+1, ed[i-1][j+1]+1));
			if(ed[i][j] > threshold) bigger_than_threshold++;
		}
		a_equel_b = ((i + threshold) < b.size()) ? (a[i] == b[i + threshold]):0;
		ed[i][wide_size-1] = min(ed[i-1][wide_size-1] + 1 - a_equel_b, ed[i][wide_size-2]+1);
		if(ed[i][wide_size-1] > threshold) bigger_than_threshold++;
		if(bigger_than_threshold == wide_size) return 0;
	}

    /*for(int i=0;i<min_size;i++)
    {
    	for(int j=0;j<wide_size;j++)
    		cout<<ed[i][j]<<" ";
    	cout << endl;
    }*/
    if(ed[a.size()-1][b.size()+threshold-a.size()] > threshold) return 0;
    return 1;
}

int SimSearcher::checkED_naive(const string &a, const string &b, int threshold)
{
	int ed[a.size() + 1][b.size() + 1];
	for(int i=0;i<=b.size();i++)
		ed[0][i]=i;
	for(int i=0;i<=a.size();i++)
		ed[i][0]=i;
	for(int i=0;i<a.size();i++)
		for(int j=0;j<b.size();j++)
			ed[i+1][j+1] = min(ed[i][j] + 1 - (a[i] == b[j]), min(ed[i][j+1] + 1, ed[i+1][j] + 1));

	if(ed[a.size()][b.size()] > threshold) return 0;
	return 1;
}