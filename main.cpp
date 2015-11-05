#design a cache to compare the speed of different cache
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <list>

using namespace std;

const int K=1024;
struct cache_content
{
	bool v;
	unsigned int  tag;
};

double log2(int haha)
{
	return log((double)haha)/log(2.0);
}

/////////////////////////////////////////////////////////////

void simulate(int cache_size, int block_size, int way)
{
	unsigned int tag, index, x;

	int block_num = cache_size/block_size;
	int set_num   = block_num/way;

	int offset_bit = (int) log2(block_size);
	int index_bit = (int) log2(set_num);
	int tag_bit = 32 - index_bit - offset_bit;

	cout << "Cache size = " << cache_size << "B. Block size = " << block_size << "B." << endl;
	cout << "This is a " << way << "-way cache." << endl;
	cout << "This cache contains " << block_num << " blocks." << endl;
	cout << "There are " << set_num << " sets" << endl << endl;

	cache_content **cache = new cache_content*[set_num];
	list<int>* lru = new list<int>[set_num];
	for(int i=0; i<set_num; i++)
	{
		cache[i] = new cache_content[way];
		for(int j=0; j<way; j++)
		{
			cache[i][j].v=false;
			lru[i].push_back(j);
		}
	}

	FILE * fp=fopen("LU.txt","r"); // read file
	int total=0, miss=0;

	while(fscanf(fp,"%x",&x)!=EOF)
	{
		index = (x>>offset_bit)&(set_num-1);
		tag   = x>>(index_bit+offset_bit);
		cout << "addr=" << x << " index=" << index << " tag=" << tag << " ";

		bool miss_flag = true;
		for(int i=0; i<way; i++)
		{
			if(cache[index][i].v && (cache[index][i].tag==tag)) // hit
			{
				miss_flag = false;
				cache[index][i].v = true;
				lru[index].remove(i);
				lru[index].push_back(i);
				cout << "hit" << endl;
			}
		}

		if(miss_flag) // miss
		{
			int i = lru[index].front();
			cache[index][i].v = true;
			cache[index][i].tag = tag;
			lru[index].remove(i);
			lru[index].push_back(i);
			miss++;
			cout << "miss -> put in num " << i << endl;
		}
		total++;
	}

	cout << "MISS RATE = " << (double)miss/total << endl;
	fclose(fp);
}

int main(){

	simulate(1*K, 32, 2);
	system("PAUSE");
}
