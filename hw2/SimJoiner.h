#ifndef __EXP2_SIMJOINER_H__
#define __EXP2_SIMJOINER_H__

#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>

#include <map>
#include <unordered_map>
#include <string>
#include <queue>
#include <functional>
#include <utility>
#include <set>
#include <sstream>
#include <unordered_set>

using namespace std;

template <typename IDType, typename SimType>
struct JoinResult {
    IDType id1;
    IDType id2;
    SimType s;
};

typedef JoinResult<unsigned, double> JaccardJoinResult;
typedef JoinResult<unsigned, unsigned> EDJoinResult;

const int SUCCESS = 0;
const int FAILURE = 1;


const int MAX_T_PLUS_ONE = 4;


struct Count_set{
	vector<int> *co;
	vector<int> dirty;
	void set_size(int n)
	{
		co = new vector<int>(n);
	}
	void clear()
	{
		for(int i=0;i<dirty.size();i++)
		{
			(*co)[dirty[i]] = 0;
		}
		dirty.clear();
	}
	void add(int n)
	{
		if((*co)[n]) return;
		(*co)[n] = 1;
		dirty.push_back(n);
	}
};

struct j_Count_set{
    vector<int> *co;
    vector<int> dirty;
    vector<int> candidate;
    int min_size;
    void set_size(int n)
    {
        co = new vector<int>(n);
    }
    void clear(int n)
    {
        min_size = n;
        for(int i=0;i<dirty.size();i++)
        {
            (*co)[dirty[i]] = 0;
        }
        dirty.clear();
        candidate.clear();
    }
    void add(int n)
    {
        if((*co)[n])
        {
            (*co)[n]++;
        }
        else
        {
            (*co)[n] = 1;
            dirty.push_back(n);
        }
        if((*co)[n] == min_size) candidate.push_back(n);
    }
};

struct StrIndex
{
    unordered_map<string, int> str_index_map;
    vector<int> freq;
    int co;
    StrIndex():co(-1){}
    int to_int(string& s)
    {
        unordered_map<string, int>::iterator it = str_index_map.find(s);
        if(it != str_index_map.end())
        {
            return it->second;
        }
        else
        {
            co++;
            str_index_map[s] = co;
            freq.push_back(0);
            return co;
        }
    }
    int add(string& s)
    {
        int res = to_int(s);
        freq[res]++;
        return res;
    }
    int find(string& s)
    {
        unordered_map<string, int>::iterator it = str_index_map.find(s);
        if(it != str_index_map.end())
        {
            return it->second;
        }
        else
        {
            return -1;
        }
    }
};




class SimJoiner {
public:
    SimJoiner();
    ~SimJoiner();

    void readFile(const char *filename1, const char *filename2);

    int joinJaccard(const char *filename1, const char *filename2, double threshold, std::vector<JaccardJoinResult> &result);
    int joinED(const char *filename1, const char *filename2, unsigned threshold, std::vector<EDJoinResult> &result);

    //ED method
    int checkED(const string &a, const string &b, int threshold);
    void build_part_map();
    void print_part_map();
    void add_ed_res(int n, std::vector<EDJoinResult> &result);

    //Jaccard method
    void addGram(int n);
    void add_j_res(int n, std::vector<JaccardJoinResult> &result);


    vector<string> words1;
    vector<string> words2;

    //ED
    int ed_res;
    int ed_threshold;
    int ed_threshold_plus;
    unordered_map<string, vector<int> > part_map[262][MAX_T_PLUS_ONE];
    Count_set ed_candidate_set;


    //Jaccard
    double j_threshold;
    StrIndex j_str_index;
    unordered_map<int, vector<int> > gram_index[257];
    j_Count_set j_candidate_set;
};

#endif
