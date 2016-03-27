#pragma once
#include <vector>
#include <map>
#include <string>
#include <utility>

using namespace std;

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
	int checkED_test(const string &a, const string &b, int threshold);

	std::vector<string> words;
	std::map<string, std::vector<int> > index;
	std::vector<int> short_index;
	unsigned q;
	int ed_res;
	std::vector<std::vector<int> > filter;
};


