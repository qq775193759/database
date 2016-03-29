#pragma once
#include <vector>
#include <map>
#include <string>
#include <queue>
#include <functional>
#include <utility>
#include <set>

using namespace std;

const int MAX_THRESHOLD = 2;

const int SUCCESS = 0;
const int FAILURE = 1;



class SimSearcher
{
public:
	SimSearcher();
	~SimSearcher();

	int createIndex(const char *filename, unsigned q);
	void readFile(const char *filename);
	void addWord(int n);

	int searchJaccard(const char *query, double threshold, std::vector<std::pair<unsigned, double> > &result);
	int searchED(const char *query, unsigned threshold, std::vector<std::pair<unsigned, unsigned> > &result);

	double checkJaccard(const string &a, const string &b);
	int checkED(const string &a, const string &b, int threshold);
	int checkED_naive(const string &a, const string &b, int threshold);
	void violence(const string &a, unsigned threshold, vector<pair<unsigned, unsigned> > &result);

	std::vector<string> words;
	//map<string, map<int, vector<int> > > index;
	map<string, vector<int> > index[262];

	unsigned q;
	int ed_res;
	set<int> candidate;
	//vector<vector<int>::iterator> filter;
	//vector<vector<int>::iterator> filter_end;
	//priority_queue<int> heap;
	vector<vector<int>*> len_list;
};


