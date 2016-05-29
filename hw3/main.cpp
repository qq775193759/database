#include "AEE.h"

using namespace std;

int main(int argc, char **argv) {
    AEE aee;

    vector<EDExtractResult> resultED;
    vector<JaccardExtractResult> resultJaccard;

    unsigned edThreshold = 2;
    double jaccardThreshold = 0.85;

    aee.createIndex("entity");
    aee.aeeED("123456789abcdefgh", edThreshold, resultED);

    return 0;
}
