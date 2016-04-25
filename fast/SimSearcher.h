#pragma once
#include <vector>
#include <utility>
#include <cstdlib>
#include <unordered_map>
#include <unordered_set>
#include <string>

const int SUCCESS = 0;
const int FAILURE = 1;
const unsigned WORD_MAX_LEN = 256;
const unsigned Q_MAP_LEN = 990023;
const unsigned S_HASH_SIZE = 1500007;

const unsigned MAX_QUE_LEN = (1<<16);
const unsigned MAX_TOKENS = 1000000;

typedef std::unordered_map<std::string, int>::iterator map_it;

#define IN_CLASS

class SimSearcher
{

#ifdef IN_CLASS


//-- for both --
	std::vector<char*> rec;
	int *rec_len;
	int times;
	std::pair<int, int>* scount;
	std::vector<int> filter;


//--  for ED --
	std::vector<int>* qmap;
	std::vector<int>** qmap_sort;
	int qmap_size;
	//unsigned* edtr;

	struct status
	{
		short x,y;
	};

	status* sech_que;

//--  for Jaccard --
	std::unordered_map<std::string, int> smap;

	std::vector<std::pair<long long, int> >* str_map;
	int str_cnt;

	std::vector<int>* tmap;
	std::vector<int>** tmap_sort;
	std::vector< std::vector<int>> trec;
	int token_size;
	unsigned smin;
	int* jrec;
	int* qtks;

#endif

	unsigned calcED(const char* a, int alen, const char* b, int blen, unsigned threshold);
	unsigned sech_ed(const char* a, int alen, const char* b, int blen, unsigned threshold);
	int filtED(const char* a, int alen, int t);
	bool validateED(int id, int leftsz, int mincan, int minnot);

	long long str_hash(char const *s, int len);
	int get_id(const char* s, int len);

	int predeal(const char *query);
	double calcJaccard(int id, int qtk, double threshold);
	int filtJ(int qlen, int t);
	//bool validateJ(int id, int leftsz, int mincan, int minnot);

public:
	SimSearcher();
	~SimSearcher();

	int createIndex(const char *filename, unsigned q);
	int searchJaccard(const char *query, double threshold, std::vector<std::pair<unsigned, double> > &result);
	int searchED(const char *query, unsigned threshold, std::vector<std::pair<unsigned, unsigned> > &result);
};

