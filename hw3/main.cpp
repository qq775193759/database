#include "AEE.h"

using namespace std;

int main(int argc, char **argv) {
    AEE aee;

    vector<EDExtractResult> resultED;
    vector<JaccardExtractResult> resultJaccard;

    unsigned edThreshold = 2;
    double jaccardThreshold = 0.85;

    aee.createIndex(argv[1]);
    aee.aeeJaccard(argv[2], jaccardThreshold, resultJaccard);
    aee.aeeED(argv[2], edThreshold, resultED);

    return 0;
}
