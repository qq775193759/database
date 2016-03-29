#include "SimSearcher.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>

using namespace std;


int compare_gram_freq(vector<int> * a, vector<int> * b)
{
	if(a == 0) return 1;
	if(b == 0) return 0;
	return a->size() < b->size();
}

SimSearcher::SimSearcher()
{
}

SimSearcher::~SimSearcher()
{
}

int SimSearcher::createIndex(const char *filename, unsigned q)
{
	this->q = q;
	readFile(filename);
	for(int i=0;i<words.size();i++)
	{
		addWord(i);
	}
	return SUCCESS;
}

void SimSearcher::readFile(const char *filename)
{
	ifstream fin(filename);
	string s;
	while(getline(fin,s))
	{
		words.push_back(s);
	}
	fin.close();
}

void SimSearcher::addWord(int n)
{
	string& a=words[n];
	int a_size = a.size();
	for(int i=0;i<=a.size()-q;i++)
	{
		string temp = a.substr(i,q);
		//for(int j=MAX_THRESHOLD;j<=MAX_THRESHOLD;j++)
		int j = MAX_THRESHOLD;
			for(int k=-j;k<=j;k++)
			{
				vector<int> &temp_v = index[a_size+k][temp];
				if(temp_v.empty() || temp_v.back() != n)
					temp_v.push_back(n);
			}
	}
}

int SimSearcher::searchJaccard(const char *query, double threshold, vector<pair<unsigned, double> > &result)
{
	result.clear();
	return SUCCESS;
}

int SimSearcher::searchED(const char *query, unsigned threshold, vector<pair<unsigned, unsigned> > &result)
{
	result.clear();
	len_list.clear();
	candidate.clear();

	string a(query);
	int a_size = a.size();
	int min_cross = a_size-q+1-q*threshold;
	if(min_cross <= 0)
	{
		violence(a, threshold, result);
		return SUCCESS;
	}
	//heap search
	//make list
	string temp;
	pair<unsigned, unsigned> temp_pair;
	for(int i=0;i<=a.size()-q;i++)
	{
		temp = a.substr(i,q);
		if((index[a_size]).count(temp))
			len_list.push_back(&index[a_size][temp]);
		else
			len_list.push_back(0);
	}
	sort(len_list.begin(), len_list.end(), compare_gram_freq);
	for(int i=0;i<=q*threshold;i++)
	{
		if(len_list[i] == 0) continue;
		for(vector<int>::iterator it = len_list[i]->begin();it != len_list[i]->end();it++)
		{
			candidate.insert(*it);
		}
	}
	for(set<int>::iterator it = candidate.begin();it != candidate.end();it++)
	{
		if(checkED(a,words[*it],threshold))
		{
			temp_pair.first = *it;
			temp_pair.second = ed_res;
			result.push_back(temp_pair);
		}
	}



	//violence
	//violence(a, threshold, result);

	return SUCCESS;
}

int SimSearcher::checkED(const string &a, const string &b, int threshold)
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

	ed_res = ed[a.size()][b.size()];
	if(ed[a.size()][b.size()] > threshold) return 0;
	return 1;
}


void SimSearcher::violence(const string &a, unsigned threshold, vector<pair<unsigned, unsigned> > &result)
{
	//violence
	pair<unsigned, unsigned> temp;

	for(int i=0;i<words.size();i++)
	{
		int delta = abs((int)a.size() - (int)words[i].size());
		if(delta > (int)threshold)continue;
		if(checkED(a,words[i],threshold))
		{
			temp.first = i;
			temp.second = ed_res;
			result.push_back(temp);
		}
	}
}