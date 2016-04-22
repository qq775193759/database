#include "SimSearcher.h"
#include <ctime>
#include <cstdlib>
#include <iostream>

using namespace std;
# define TEST_SIZE 32
# define TEST_SIZE_PLUS (TEST_SIZE + 1)
# define TEST_ADD_SIZE 3

string rand_string()
{
	string s;
	s.assign(TEST_SIZE, 'A');
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

string rand_string(string s)
{
	string t;
	int r[TEST_SIZE_PLUS]={0};
	for(int i=0;i<TEST_ADD_SIZE;i++)
		r[rand()%TEST_SIZE_PLUS]++;
	for(int i=0;i<TEST_SIZE_PLUS;i++)
	{
		for(int j=0;j<r[i];j++)
			t.append(1, 'A' + rand()%26);
		if(i <TEST_SIZE)
		t.append(1, s[i]);
	}
			//cout<<s<<"   "<<t<<"   "<<s.size()<<t.size()<<endl;
	return t;
}


int main(int argc, char **argv)
{
	srand(time(0));
	SimSearcher searcher;

	vector<pair<unsigned, unsigned> > resultED;
	vector<pair<unsigned, double> > resultJaccard;

	unsigned q = 3, edThreshold = 2;
	double jaccardThreshold = 0.85;

	//searcher.createIndex(argv[1], q);
	//searcher.searchJaccard("query", jaccardThreshold, resultJaccard);
	//searcher.searchED("query", edThreshold, resultED);




	/*string a = "JDQMMODGNUZWLXAPKCEIHOQOWEHBUJHS";
	string b = "DQTMMODGNUZWLXAPUCEIHOQOWEHBUJHS";
	searcher.checkED(a,b,3);*/
	/*string a = "AZFSENJU";
	string b = "DAZFDSESNMJU";
	searcher.checkED(a,b,5);*/

	/*for(int i=0;i<1000000;i++)
	{
		string s = rand_string();
		//string t = rand_string(s, 4);
		string t = rand_string(rand_string(s), 3);
		int res1 = searcher.checkED(s,t,5);
		int res2 = searcher.checkED_naive(s,t,5); 
		if(res1 != res2)
		{
			cout<<s<<"   "<<t<<endl;
			cout<<res1<<" "<<res2<<" ";
		}
	}*/
	//searcher.readFile("input");
	searcher.createIndex("input", q);
	searcher.searchED("aaabc", edThreshold, resultED);

	map<int, int> m;
	m[1];
	cout<<m.count(1)<<endl;
	return 0;
}

