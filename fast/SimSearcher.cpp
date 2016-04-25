#include "SimSearcher.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <algorithm>
#include <unordered_set>
using namespace std;




#ifndef IN_CLASS
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


SimSearcher::SimSearcher()
{

#ifdef USE_LARGE_MEM
	char* kkk[6];
	unsigned bsz = (1<<30);
	for(int i = 0; i < 6; ++i)
	{
		try{
			kkk[i] = new char[bsz];
			memset(kkk[i], 0xBB,bsz);
		} catch (...) {
			kkk[i] = 0;
		}
	}
	for(int i = 0; i < 6; ++i)
	{
		delete [](kkk[i]);
	}
#endif
	//unsigned *p = new unsigned[WORD_MAX_LEN*WORD_MAX_LEN];
	//memset((void*)p, -1, WORD_MAX_LEN*WORD_MAX_LEN*4);
	rec.clear();

	qmap = new vector<int>[Q_MAP_LEN];
	qmap_sort = new std::vector<int>*[Q_MAP_LEN];
	for(size_t i = 0; i < Q_MAP_LEN; ++i)
		qmap[i].clear();
	times = 0;
	sech_que = new status[MAX_QUE_LEN];
	sech_que[0].x = sech_que[0].y = 0;

	smap.clear();
	tmap = new vector<int>[MAX_TOKENS];
	tmap_sort = new vector<int>*[MAX_TOKENS];
	jrec = new int[MAX_TOKENS];
	qtks = new int[MAX_TOKENS];
	str_map = new vector<pair<long long, int>>[S_HASH_SIZE];
	//edtr = new unsigned[WORD_MAX_LEN*WORD_MAX_LEN];
	//memset((void*)edtr, 0, WORD_MAX_LEN*WORD_MAX_LEN*sizeof(unsigned));
}

SimSearcher::~SimSearcher()
{
	for(size_t i = 0; i < rec.size(); ++i){
		delete [](rec[i]);
	}
	delete []qmap;
	delete []qmap_sort;
	delete []tmap;
	delete []tmap_sort;
	delete []scount;
	delete []rec_len;
	delete []jrec;
	delete []qtks;
}
/*
bool SimSearcher::validateED(int id, int leftsz, int mincan, int minnot)
{
	int y = 0, n = 0;
	for(int i = 0; i < leftsz; ++i)
	{
		vector<int>& qvec = *(qmap_sort[i]);
		int p = 0, q = qvec.size(), m;
		while(p < q)
		{
			m = (p+q)>>1;
			if(qvec[m] == id)
			{
				p = m;
				break;
			}
			else if(qvec[m] >= id)
			{
				q = m;
			}
			else
			{
				p = m+1;
			}
		}
		if(qvec[p] == id)
		{
			if(++y >= mincan)
				return true;
		}
		else
		{
			if(++n > minnot)
				return false;
		}
	}
	if(y >= mincan)
		return true;
	else
		return false;
}
*/
int SimSearcher::filtED(const char* a, int alen, int t)
{
	filter.clear();
	const char* ed = a + alen-4;
	qmap_size = 0;
	for(const char* it = a; it < ed; ++it)
	{
		unsigned idx = (*((unsigned*)it))%Q_MAP_LEN;
		qmap_sort[qmap_size++] = qmap + idx;
	}

//------------------------------------------

	int leftsz = min((int)((double)t/1.1), t-5);
	leftsz = t-2;
	if(leftsz <= 0)
		leftsz = 1;

	nth_element(qmap_sort, qmap_sort+leftsz, qmap_sort+qmap_size, [](vector<int>* a, vector<int>* b)
	{
		return a->size() > b->size();
	});

	for(int i = leftsz; i < qmap_size; ++i)
	{
		vector<int>* qvec = qmap_sort[i];
		int qsz = qvec->size();
		for(int j = 0; j < qsz; ++j)
		{
			unsigned id = (*qvec)[j];
			if(scount[id].first < times) {
				scount[id].first = times;
				//filter.push_back(id);
			} else {
				if(scount[id].second < times) {
					filter.push_back(id);
					scount[id].second = times;
				}
			}
		}
	}

	return 0;
}

int SimSearcher::searchED(const char *query, unsigned threshold, vector<pair<unsigned, unsigned> > &result)
{
	times ++;
	result.clear();
	int qlen = strlen(query);

	int t = qlen - (4-1+threshold*4);
	unsigned ed;
	if(t > 0)
	{
		filtED(query, qlen, t+2);
		//filt(query, qlen, t);
		for(size_t i = 0; i < filter.size(); ++i)
		{
			size_t id = filter[i];
			if(abs((int)(qlen-rec_len[id])) > threshold)
				continue;
			//calcED(query, qlen, rec[id], rec_len[id], threshold);
			if((ed = sech_ed(query, qlen, rec[id], rec_len[id], threshold)) <= threshold)
			{
				result.push_back(make_pair(id, ed));
			}
		}
	}
	else
	{
		for(size_t i = 0; i < rec.size(); ++i)
		{
			if(abs((int)(qlen-rec_len[i])) > threshold)
				continue;
			//calcED(query, qlen, rec[i], rec_len[i], threshold);
			if((ed = sech_ed(query, qlen, rec[i], rec_len[i], threshold)) <= threshold)
			{
				result.push_back(make_pair(i, ed));
			}
		}
	}
	sort(result.begin(), result.end());
	return SUCCESS;
}


unsigned SimSearcher::sech_ed(const char* a, int alen, const char* b, int blen, unsigned threshold)
{

	static unsigned ttt = 0;
	unsigned k = threshold*2+1;
	++ttt;
	unsigned d = 0, nd = 1;
	unsigned nxt = 1;
	int lab = alen - blen;
	for(unsigned p = 0, q = 1; p < q; ++p)
	{
		status& s = sech_que[p];
		int x = s.x;
		int y = s.y;
		if(p == nxt)
		{
			d = nd++;
			nxt = q;
		}

RETRY_THIS_ELEMENT:

		//if(x == alen || y == blen){
		unsigned maxd = d + max(alen - x, blen - y);
		//unsigned mind = d + abs(lab - (x - y));

		//if(mind > threshold)
		//	continue;
		if(maxd <= threshold)
			return 0;
		//}
		//else if(mind > threshold)
		//	continue;
		else if(x < alen && y < blen)
		{
			if(a[x] == b[y])
			{
				x++;
				y++;
				goto RETRY_THIS_ELEMENT;
			}
			else
			{
				if(nd <= threshold)
				{
					sech_que[q++] = {x+1, y+1};
					sech_que[q++] = {x+1, y};
					sech_que[q++] = {x, y+1};
				}
			}
		}
	}
	return (unsigned)-2;
}


unsigned SimSearcher::calcED(const char* a, int alen, const char* b, int blen, unsigned threshold)
{
	static unsigned ed_tmp_[0x10000];
	int bst = (int)threshold;
	int bed = (int)threshold;
	if(blen-alen > 0)
	{
		bst -= blen-alen;
	}
	else
	{
		bed += blen-alen;
	}
	int z = threshold+1;
	for(int i = 0; i <= z; ++i)
		ed_tmp_[z+i] = i;
	int zs = z-bst;
	int ze = z+bed+1;
	int zbl = z+blen;
	int st = 0, ed = 1;

	for(int i = 0; i < alen; ++i)
	{
		st = max(z - i, zs);
		ed = min(zbl - i, ze);
		int iz = i-z;
		bool f = false;
		ed_tmp_[st-1] = i+1;
		for(int j = st; j < ed; ++j)
		{
			ed_tmp_[j] = (unsigned)(ed_tmp_[j-1]+1) > (unsigned)(ed_tmp_[j+1]+1) ?
			((unsigned)(ed_tmp_[j+1]+1) > (unsigned)(ed_tmp_[j]+(a[i] != b[iz+j]))? (unsigned)(ed_tmp_[j]+(a[i] != b[iz+j])):
				(unsigned)(ed_tmp_[j+1]+1)):
			((unsigned)(ed_tmp_[j-1]+1) > (unsigned)(ed_tmp_[j]+(a[i] != b[iz+j]))? (unsigned)(ed_tmp_[j]+(a[i] != b[iz+j])):
				(unsigned)(ed_tmp_[j-1]+1));
			if(ed_tmp_[j] <= threshold)
			{
				f = true;
				if(ed_tmp_[j] + max(alen - i, blen - iz -j) <= threshold)
					return 0;
			}
		}
		if(!f)
		{
			if(i == alen-1)
				return ed_tmp_[ed-1];
			else
				return threshold + 1;
		}
		ed_tmp_[ed] = (unsigned)-2;
	}
	return ed_tmp_[ed-1];
}

double SimSearcher::calcJaccard(int id, int qtk, double threshold)
{
	vector<int>& v = trec[id];
	int in = 0;
	for(size_t i = 0; i < v.size(); ++i)
	{
		if(jrec[ v[i] ] == times)
			++in;
	}
	//printf("%d %d %d\n", in,qtk, v.size());
	return double(in)/double(qtk+v.size()-in);
}

int SimSearcher::filtJ(int qtlen, int t)
{
	for(int i = 0; i < qtlen; ++i)
	{
		tmap_sort[i] = tmap + qtks[i];
	}

	if(t > 1){
		int leftsz = t-2;


		nth_element(tmap_sort, tmap_sort+leftsz, tmap_sort+qtlen, [](vector<int>* a, vector<int>* b)
		{
			return a->size() > b->size();
		});
		//scan count, better than merge skip?

		for(int i = leftsz; i < qtlen; ++i)
		{
			vector<int>* tvec = tmap_sort[i];
			int tsz = tvec->size();
			for(int j = 0; j < tsz; ++j)
			{
				unsigned id = (*tvec)[j];
				if(scount[id].first < times)
				{
					//filter.push_back(id);
					scount[id].first = times;
				} else
				{
					if(scount[id].second < times)
					{
						filter.push_back(id);
						scount[id].second = times;
					}
				}
			}
		}
	} else {
		for(int i = 0; i < qtlen; ++i)
		{
			vector<int>* tvec = tmap_sort[i];
			int tsz = tvec->size();
			for(int j = 0; j < tsz; ++j)
			{
				unsigned id = (*tvec)[j];
				if(scount[id].first < times)
				{
					filter.push_back(id);
					scount[id].first = times;
				} 
				/*else {
					if(scount2[id] < times) {
						filter.push_back(id);
						scount2[id] = times;
					}
				}*/
			}
		}
	}

	return 0;
}

long long SimSearcher::str_hash(const char *s, int len)
{
	long long x = 0;
	int t = len / sizeof(const long long);
	for (int i = t; i--; )
	{
		x ^= (x << 3);
		x ^= ((const long long*)s)[i];
	}
	int i = t * sizeof(const long long);
	if (i + sizeof(int) <= len)
	{
		x ^= x << 5;
		x ^= *(const int*)(s + i);
		i += sizeof(int);
	}
	while(i < len)
	{
		x ^= x << 5;
		x ^= s[i++];
	}
	return x;
}

int SimSearcher::get_id(const char *w, int len) {
	long long h = str_hash(w, len);
	vector<pair<long long, int>>& v = str_map[(unsigned long long)h % S_HASH_SIZE];

	for (size_t idx = 0; idx < v.size(); idx++) {
		pair<long long, int>& p = v[idx];
		if (p.first == h)
			return p.second;
	}
	v.push_back(pair<long long, int>(h, str_cnt));
	return str_cnt++;
}


int SimSearcher::predeal(const char *query)
{
	int qlen = strlen(query);
	char buf[1024];
	for(int i = 0; i <= qlen; ++i)
	{
		buf[i] = query[i];
	}

	char* ed = buf + qlen;
	char* tst = buf;
	int qtk = 0;

	for(char* c = buf; c < ed; ++c)
	{
		if(*c != ' ')
			continue;
		//string s = tst;
		/*map_it mit = smap.find(s);
		if(mit == smap.end())
		{
			pair<map_it, bool> rst = smap.insert(make_pair(s,token_size++));
			//assert(rst.second);
			mit = rst.first;
		}*/
		int sid = get_id(tst, c-tst);//mit->second
		if(jrec[sid] < times)
		{
			jrec[sid] = times;
			qtks[qtk ++] = sid;
		}
		tst = c+1;
	}
	//string s = tst;
	/*map_it mit = smap.find(s);
	if(mit == smap.end())
	{
		pair<map_it, bool> rst = smap.insert(make_pair(s,token_size++));
		//assert(rst.second);
		mit = rst.first;
	}*/
	int sid = get_id(tst, ed-tst);//mit->second
	if(jrec[sid] < times)
	{
		jrec[sid] = times;
		qtks[qtk ++] = sid;
	}
	return qtk;
}

int zzz;

int SimSearcher::searchJaccard(const char *query, double threshold, vector<pair<unsigned, double> > &result)
{
	result.clear();
	times ++;

	// ---
	int qtk = predeal(query);

	double t = max(threshold* qtk, (qtk+smin)*threshold/(threshold+1));
	// -------
	filter.clear();
	filtJ(qtk, (int)t + (t > (int)t));
	//filtJ(qtk, (int)t + (t > (int)t));

	double jc = 0;
	for(int i = 0 ; i < filter.size(); ++i)
	{
		//calcJaccard(filter[i], qtk, threshold);
		if((jc = calcJaccard(filter[i], qtk, threshold)) >= threshold)
		{
			result.push_back(make_pair(filter[i], jc));
		}
	}

	sort(result.begin(), result.end());


	//for(int i = 0; i < 5; ++i)
	//{
	//	predeal(query);
	//}

	//filtJ(qtk, (int)t + (t > (int)t));
	return SUCCESS;
}

int SimSearcher::createIndex(const char *filename, unsigned q)
{

	char buf[1024];

	FILE* f = fopen(filename, "r");
	int rst = SUCCESS;
	int id = 0;
	smin = (unsigned)-2;
	qmap_size = 0;
	token_size = 0;
	while(fgets(buf, 1024, f))
	{
		int len = strlen(buf);

		if(buf[len-1] == '\n')
		{
			buf[--len] = 0;
		}
		if(len == 0)
			continue;
		char* a = new char[len+1];
		for(int i = 0; i < len; ++i)
			a[i] = buf[i];
		a[len] = 0;
		rec.push_back(a);

		// for ED
		char* ed = buf + (len - 4);
		for(char* it = buf; it < ed; ++it)
		{
			unsigned idx = (*((unsigned*)it))%Q_MAP_LEN;
			if(qmap[idx].size() == 0 || qmap[idx][ qmap[idx].size()-1 ] < id)//???TODO
				qmap[idx].push_back(id);
		}

		// for Jaccard
		char* tst = buf;
		ed = buf+len;
		trec.push_back(vector<int>());
		vector<int>& v = trec[trec.size()-1];
		for(char* it = buf; it < ed; ++it)
		{
			if(*it != ' ')
				continue;
			//string s = tst;
			/*map_it mit = smap.find(s);
			if(mit == smap.end())
			{
				pair<map_it, bool> rst = smap.insert(make_pair(s,token_size++));
				//assert(rst.second);
				mit = rst.first;
				//printf("%s %d\n", mit->first.c_str(), mit->second);
			}*/
			int sid = get_id(tst, it-tst);// mit->second
			if(tmap[sid].size() == 0 || tmap[sid][ tmap[sid].size()-1 ] < id)
			{
				tmap[sid].push_back(id);
				v.push_back(sid);
			}
			tst = it+1;
		}
		//string s = tst;
		/*map_it mit = smap.find(s);
		if(mit == smap.end())
		{
			pair<map_it, bool> rst = smap.insert(make_pair(s,token_size++));
			//assert(rst.second);
			mit = rst.first;
			//printf("%s %d\n", mit->first.c_str(), mit->second);
		}*/
		int sid = get_id(tst, ed-tst);// mit->second
		if(tmap[sid].size() == 0 || tmap[sid][ tmap[sid].size()-1 ] < id)
		{
			tmap[sid].push_back(id);
			v.push_back(sid);
		}

		if(v.size() < smin)
			smin = v.size();

		++id;
	}
	fclose(f);

	scount = new pair<int, int>[rec.size()];
	rec_len = new int[rec.size()];

	memset((void*)scount, 0, rec.size() * sizeof(pair<int, int>));
	memset((void*)jrec, 0, token_size * sizeof(int));

	for(size_t i = 0; i < rec.size(); ++i){
		rec_len[i] = strlen(rec[i]);
	}
	return rst;

}


