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
	int searchJaccard(const char *query, double threshold, std::vector<std::pair<unsigned, double> > &result);
	int searchED(const char *query, unsigned threshold, std::vector<std::pair<unsigned, unsigned> > &result);

	double checkJaccard(const string &a, const string &b);
	int checkED(const string &a, const string &b, int threshold);
	int checkED_naive(const string &a, const string &b, int threshold);

	std::vector<string> words;
	std::map<string, std::vector<int> > revers;
};


