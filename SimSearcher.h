#pragma once
#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <queue>
#include <functional>
#include <utility>
#include <set>
#include <sstream>

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
	void addGram(int n);

	int searchJaccard(const char *query, double threshold, std::vector<std::pair<unsigned, double> > &result);
	int searchED(const char *query, unsigned threshold, std::vector<std::pair<unsigned, unsigned> > &result);

	int checkED(const string &a, const string &b, int threshold);
	int checkED_naive(const string &a, const string &b, int threshold);
	void violence(const string &a, unsigned threshold, vector<pair<unsigned, unsigned> > &result);
	double checkJaccard(int n);

	vector<string> words;
	unordered_map<string, vector<int> > index[262];
	//Jaccard
	set<string> words_set;
	vector<set<string> > words_set_vector;
	unordered_map<string, vector<int> > gram_index[160];
	vector<int> *result_map;
	vector<int> j_candidate;
	vector<int> dirty;

	//ED
	unsigned q;
	int ed_res;
	set<int> candidate;
	vector<vector<int>*> len_list;
};


