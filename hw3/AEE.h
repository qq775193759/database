#ifndef __EXP3__AEE_H__
#define __EXP3__AEE_H__

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

template <typename EntityIDType, typename PosType, typename LenType, typename SimType>
struct ExtractResult {
    EntityIDType id;
    PosType pos;
    LenType len;
    SimType sim;
};

typedef ExtractResult<unsigned, unsigned, unsigned, unsigned> EDExtractResult;
typedef ExtractResult<unsigned, unsigned, unsigned, double> JaccardExtractResult;

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

class AEE {
public:
    AEE();
    ~AEE();

    int createIndex(const char *entity_file_name);
    int aeeJaccard(const char *document, double threshold, std::vector<JaccardExtractResult> &result);
    int aeeED(const char *document, unsigned threshold, std::vector<EDExtractResult> &result);


    int checkED(const string &a, const string &b, int threshold);
    void build_part_map();
    void print_part_map();
    void add_ed_res(string &str, int pos, int len, std::vector<EDExtractResult> &result);
    void print_all(vector<EDExtractResult> &result);

    vector<string> words2;
    int max_len;
    int min_len;

    //ED
    int ed_res;
    int ed_threshold;
    int ed_threshold_plus;
    unordered_map<string, vector<int> > part_map[262][MAX_T_PLUS_ONE];
    Count_set ed_candidate_set;
};

#endif
