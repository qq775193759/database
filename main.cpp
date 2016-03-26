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




	/*string a = "JDQMMODGNUZWLXAPKCEIHOQOWEHBUJHS";
	string b = "DQTMMODGNUZWLXAPUCEIHOQOWEHBUJHS";
	searcher.checkED(a,b,3);*/

	for(int i=0;i<100000000;i++)
	{
		string s = rand_string();
		string t = rand_string(s, 4);
		int res1 = searcher.checkED(s,t,3);
		int res2 = searcher.checkED_naive(s,t,3); 
		if(res1 != res2)
		{
			cout<<s<<"   "<<t<<endl;
			cout<<res1<<" "<<res2<<" ";
		}
	}
	//searcher.readfile("input");

	return 0;
}

