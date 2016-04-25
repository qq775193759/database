#ifndef __EXP2_SIMJOINER_H__
#define __EXP2_SIMJOINER_H__

#include <vector>
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


    vector<string> words1;
    vector<string> words2;

    //ED
    int ed_res;
    int ed_threshold;
    int ed_threshold_plus;
    unordered_map<string, vector<int> > part_map[260][MAX_T_PLUS_ONE];


    //Jaccard
    double j_threshold;
};

#endif
