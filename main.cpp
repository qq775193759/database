#include "SimSearcher.h"
#include <ctime>
#include <cstdlib>
#include <iostream>

using namespace std;


string rand_string()
{
	string s="AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
	for(int i=0;i<s.size();i++)
	{
		s[i] += rand()%26;
	}
	return s;
}

string rand_string(string s, int n)
{
	for(int i=0;i<n;i++)
	{
		s[rand()%s.size()] = 'A' + rand()%26;
	}
	return s;
}


int main(int argc, char **argv)
{
	srand(time(0));
	SimSearcher searcher;

	vector<pair<unsigned, unsigned> > resultED;
	vector<pair<unsigned, double> > resultJaccard;

	unsigned q = 3, edThreshold = 2;
	double jaccardThreshold = 0.85;

	searcher.createIndex(argv[1], q);
	searcher.searchJaccard("query", jaccardThreshold, resultJaccard);
	searcher.searchED("query", edThreshold, resultED);




	//string a = "abcdefgaaaaaaaaaa";
	//string b = "abcdefgaaaaaaa";
	for(int i=0;i<10000;i++)
	{
		string s = rand_string();
		string t = rand_string(s, 2);
		if(searcher.checkED(s,t,2) != searcher.checkED_naive(s,t,2))
		cout<<s<<"   "<<t<<endl;
	}
	//searcher.checkED(a,b,3);

	return 0;
}

