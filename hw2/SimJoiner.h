#ifndef __EXP2_SIMJOINER_H__
#define __EXP2_SIMJOINER_H__

#include <vector>

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

    int checkED(const string &a, const string &b, int threshold);


    vector<string> words1;
    vector<string> words2;

    //ED
    unordered_map<string, vector<int> > part_map[MAX_T_PLUS_ONE];


    //J
};

#endif
