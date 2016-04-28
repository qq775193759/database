#include "SimJoiner.h"

using namespace std;

int ed_res_cmp(const EDJoinResult& a, const EDJoinResult b)
{
	if(a.id1 == b.id1) return a.id2 < b.id2;
	return a.id1 < b.id1;
}

int j_res_cmp(const JaccardJoinResult& a, const JaccardJoinResult b)
{
	if(a.id1 == b.id1) return a.id2 < b.id2;
	return a.id1 < b.id1;
}

int freq_cmp(const int&a, const int &b)
{
	return a<b;
}

SimJoiner::SimJoiner() {
}

SimJoiner::~SimJoiner() {
}

void SimJoiner::readFile(const char *filename1, const char *filename2)
{
	words1.clear();
	words2.clear();
	string s;
	ifstream fin1(filename1);
	while(getline(fin1,s))
	{
		words1.push_back(s);
	}
	fin1.close();
	ifstream fin2(filename2);
	while(getline(fin2,s))
	{
		words2.push_back(s);
	}
	fin2.close();
}

int SimJoiner::joinJaccard(const char *filename1, const char *filename2, double threshold, vector<JaccardJoinResult> &result) {
    j_threshold = threshold;
    result.clear();
    readFile(filename1, filename2);
    j_candidate_set.set_size(words2.size());
    for(int i=0;i<words2.size();i++)
    {
    	addGram(i);
    }
    for(int i=0;i<words1.size();i++)
    {
    	add_j_res(i, result);
    }
    return SUCCESS;
}

int SimJoiner::joinED(const char *filename1, const char *filename2, unsigned threshold, vector<EDJoinResult> &result) {
	ed_threshold = threshold;
	ed_threshold_plus = threshold+1;
    result.clear();
    readFile(filename1, filename2);
    ed_candidate_set.set_size(words2.size());
    build_part_map();
    //print_part_map();
    for(int i=0;i<words1.size();i++)
    {
    	add_ed_res(i, result);
    }


    return SUCCESS;
}

int SimJoiner::checkED(const string &a, const string &b, int threshold)
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

void SimJoiner::build_part_map()
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

void SimJoiner::print_part_map()
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

void SimJoiner::add_ed_res(int n, vector<EDJoinResult> &result)
{
	ed_candidate_set.clear();
	vector<EDJoinResult> temp_result;
	EDJoinResult temp_res;
	temp_res.id1 = n;

	//from part_map get candidate
	int query_len = words1[n].size();
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
				string temp_s = words1[n].substr(i, part_len);
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
		if(checkED(words1[n], words2[index_rank], ed_threshold))
		{
			temp_res.id2 = index_rank;
			temp_res.s = ed_res;
			temp_result.push_back(temp_res);
		}
	}

	sort(temp_result.begin(), temp_result.end(), ed_res_cmp);



	for(int i=0;i<temp_result.size();i++)
	{
		result.push_back(temp_result[i]);
	}
	
}

void SimJoiner::addGram(int n)
{
	stringstream ss(words2[n]);
	string temp_gram;
	unordered_set<int> words_set;
	while(ss>>temp_gram)
	{
		words_set.insert(j_str_index.add(temp_gram));
	}
	int w_size = words_set.size();
	for(unordered_set<int>::iterator it=words_set.begin(); it!=words_set.end();it++)
	{
		gram_index[w_size][*it].push_back(n);
	}
}

void SimJoiner::add_j_res(int n, vector<JaccardJoinResult> &result)
{
	vector<JaccardJoinResult> temp_result;
	JaccardJoinResult temp_res;
	temp_res.id1 = n;
	//begin to find
	stringstream ss(words1[n]);
	string temp_gram;
	unordered_set<int> words_set;
	vector<int> words_vector;
	while(ss>>temp_gram)
	{
		int find_res = j_str_index.find(temp_gram);
		if(find_res != -1) words_set.insert(find_res);
	}
	for(unordered_set<int>::iterator it=words_set.begin(); it!=words_set.end();it++)
	{
		words_vector.push_back(*it);
	}
	sort(words_vector.begin(), words_vector.end(), freq_cmp);
	int q_size = words_vector.size();
	int min_size = ceil((double)q_size*j_threshold);
	int max_size = floor((double)q_size/j_threshold);
	max_size = min(256, max_size);

	for(int i=min_size;i<=max_size;i++)
	{
		int co=0;
		int temp_min_size =  ceil((q_size + i)*j_threshold/(1+j_threshold));
		//int j_parameter_size = ceil(temp_min_size/J_PARAMETER);
		temp_min_size = max(temp_min_size, min_size);
		for(vector<int>::iterator it=words_vector.begin(); it!=words_vector.end();it++)
		{
			if(gram_index[i].find(*it) != gram_index[i].end()) co++;
		}
		if(co < temp_min_size) continue;
		//do something
		j_candidate_set.clear(temp_min_size);
		co = 0;
		for(vector<int>::iterator it=words_vector.begin(); it!=words_vector.end();it++)
		{
			co++;
			if(gram_index[i].find(*it) == gram_index[i].end()) continue;
			vector<int> &temp_v = gram_index[i][*it];
			for(int j=0;j<temp_v.size();j++)
			{
				j_candidate_set.add(temp_v[j]);
			}
			/*if(co <= (q_size - temp_min_size + 1))
			{
				for(int j=0;j<temp_v.size();j++)
				{
					j_candidate_set.add(temp_v[j]);
				}
			}
			else
			{
				for(vector<int>::iterator iter = j_candidate_set.dirty.begin(); iter != j_candidate_set.dirty.end();iter++)
				{
					if(binary_search(temp_v.begin(), temp_v.end(), *iter))
					{
						j_candidate_set.add(*iter);
					}
				}
			}*/
			
		}
		for(int j=0;j<j_candidate_set.candidate.size();j++)
		{
			int temp_rank = j_candidate_set.candidate[j];
			double distance = (*j_candidate_set.co)[temp_rank];
			distance = distance/(q_size + i - distance);
			if(distance > j_threshold)
			{
				temp_res.id2 = temp_rank;
				temp_res.s = distance;
				temp_result.push_back(temp_res);
			}
		}
	}
	sort(temp_result.begin(), temp_result.end(), j_res_cmp);
	for(int i=0;i<temp_result.size();i++)
	{
		result.push_back(temp_result[i]);
	}
}