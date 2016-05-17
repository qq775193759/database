#include "AEE.h"

using namespace std;

AEE::AEE() {
}

AEE::~AEE() {
}

int AEE::createIndex(const char *entity_file_name) {
    return SUCCESS;
}

int AEE::aeeJaccard(const char *document, double threshold, vector<JaccardExtractResult> &result) {
    result.clear();
    return SUCCESS;
}

int AEE::aeeED(const char *document, unsigned threshold, vector<EDExtractResult> &result) {
    result.clear();
    return SUCCESS;
}
