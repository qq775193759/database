#include "AEE.h"

using namespace std;

int ed_res_cmp(const EDExtractResult& a, const EDExtractResult b)
{
    if(a.id != b.id) return a.id < b.id;
    if(a.pos != b.pos) return a.pos < b.pos;
    return a.len < b.len;
}

AEE::AEE() {
}

AEE::~AEE() {
}

int AEE::createIndex(const char *entity_file_name) {
    words2.clear();
    ed_threshold = 2;
    ed_threshold_plus = 3;
    min_len = 10000;
    max_len = 0;
    string s;
    ifstream fin(entity_file_name);
    while(getline(fin,s))
    {
        words2.push_back(s);
        int s_size = s.size();
        min_len = min(min_len, s_size);
        max_len = max(max_len, s_size);
    }
    fin.close();

    build_part_map();
    ed_candidate_set.set_size(words2.size());
    //print_part_map();
    return SUCCESS;
}

int AEE::aeeJaccard(const char *document, double threshold, vector<JaccardExtractResult> &result) {
    result.clear();
    return SUCCESS;
}

void AEE::print_all(vector<EDExtractResult> &result)
{
    for(int i=0;i<result.size();i++)
    {
        cout<<result[i].id<<" ";
        cout<<result[i].pos<<" ";
        cout<<result[i].len<<" ";
        cout<<result[i].sim<<" ";
        cout<<endl;
    }
}

int AEE::aeeED(const char *document, unsigned threshold, vector<EDExtractResult> &result) {
    result.clear();
    string doc(document);
    int doc_size = doc.size();
    for(int i=max(0, min_len - ed_threshold);i<=max_len + ed_threshold;i++)
    {
        for(int j=0;j <= (doc_size-i);j++)
        {
            string temp = doc.substr(j,i);
            add_ed_res(temp, j, i, result);
        }
    }
    sort(result.begin(), result.end(), ed_res_cmp);
    //print_all(result);
    return SUCCESS;
}

int AEE::checkED(const string &a, const string &b, int threshold)
{
    int a_size = a.size();
    int b_size = b.size();
    int delta_ab = a_size - b_size;
    if(abs(delta_ab) > threshold) return 0;

    int min_size = a_size;
    int wide_size = 2*threshold + 1; 
    int ed[min_size][wide_size];
    int a_equel_b;

    for(int j=0; j<threshold; j++)
        ed[0][j] = threshold - j;
    for(int j=threshold; j<wide_size; j++)
        ed[0][j] = min(j - threshold + 1 - (a[0] == b[j - threshold]), ed[0][j-1]+1);

    for(int i=1; i<min_size; i++)
    {
        int bigger_than_threshold=0;
        a_equel_b = (i>=threshold) && (a[i] == b[i - threshold]);
        ed[i][0] = min(ed[i-1][0] + 1 - a_equel_b,  ed[i-1][1]+1);
        if(ed[i][0] + abs(delta_ab - threshold) > threshold) bigger_than_threshold++;
        for(int j=1; j<wide_size-1; j++)
        {
            a_equel_b = ((i + j - threshold)>=0 && (i + j - threshold)<b_size) && (a[i] == b[i + j - threshold]);
            ed[i][j] = min(ed[i-1][j] + 1 - a_equel_b, min(ed[i][j-1]+1, ed[i-1][j+1]+1));
            if(ed[i][j] + abs(delta_ab - threshold + j) > threshold) bigger_than_threshold++;
        }
        a_equel_b = ((i + threshold) < b_size) && (a[i] == b[i + threshold]);
        ed[i][wide_size-1] = min(ed[i-1][wide_size-1] + 1 - a_equel_b, ed[i][wide_size-2]+1);
        if(ed[i][wide_size-1] + abs(delta_ab + threshold) > threshold) bigger_than_threshold++;
        if(bigger_than_threshold == wide_size) return 0;
    }


    if(ed[a_size-1][b_size+threshold-a_size] > threshold) return 0;
    ed_res = ed[a_size-1][b_size+threshold-a_size];
    return 1;
}

void AEE::build_part_map()
{
    int part_len;
    for(int i=0;i<words2.size();i++)
    {
        //cout<<words2[i]<<endl;
        int word_len = words2[i].size();
        int part_len_base = word_len/ed_threshold_plus;
        int part_len_rest = word_len - part_len_base*ed_threshold_plus;
        //cal st_pos and lenth
        int pos = 0;
        for(int j=0;j<ed_threshold_plus;j++)
        {
            part_len = part_len_base + (j < part_len_rest);
            part_map[word_len][j][words2[i].substr(pos, part_len)].push_back(i);
            pos += part_len;
        }
    }
}

void AEE::print_part_map()
{
    for(int k=0;k<ed_threshold_plus;k++)
    {
        for(unordered_map<string, vector<int> >::iterator it=part_map[7][k].begin();it!=part_map[7][k].end();it++)
        {
            cout<<it->first<<endl;
            for(int i=0;i<(it->second).size();i++)
            {
                cout<<(it->second)[i]<<" ";
            }
            cout<<endl;
        }
        cout<<endl;
    }
    
}

void AEE::add_ed_res(string &str, int pos, int len, vector<EDExtractResult> &result)
{
    ed_candidate_set.clear();
    EDExtractResult temp_res;
    temp_res.pos = pos;
    temp_res.len = len;

    //from part_map get candidate
    int query_len = str.size();
    int part_len;
    for(int delta = -ed_threshold;delta <= ed_threshold;delta++)
    {
        int word_len = query_len + delta;
        int part_len_base = word_len/ed_threshold_plus;
        int part_len_rest = word_len - part_len_base*ed_threshold_plus;
        int pos = 0;
        for(int part = 0;part < ed_threshold_plus;part++)
        {
            part_len = part_len_base + (part < part_len_rest);
            unordered_map<string, vector<int> >& temp_map = part_map[word_len][part];
            //from map to find candidate
            //for(int i=max(0,pos - (ed_threshold + delta)/2);i<=min(query_len - part_len,pos + (ed_threshold - delta)/2);i++)
            for(int i=max(0,max(pos - part, pos - delta - ed_threshold + part));
                i<=min(query_len - part_len,min(pos + part, pos - delta + ed_threshold - part));i++)
            {
                string temp_s = str.substr(i, part_len);
                if(temp_map.find(temp_s) != temp_map.end())
                {
                    //add candidate
                    vector<int>& temp_vector = temp_map[temp_s];
                    for(int j=0;j<temp_vector.size();j++)
                    {
                        ed_candidate_set.add(temp_vector[j]);
                    }
                }
            }
            pos += part_len;
        }
    }


    for(int i=0;i<ed_candidate_set.dirty.size();i++)
    {
        int index_rank = ed_candidate_set.dirty[i];
        if(checkED(str, words2[index_rank], ed_threshold))
        {
            temp_res.id = index_rank;
            temp_res.sim = ed_res;
            result.push_back(temp_res);
        }
    }
    
}