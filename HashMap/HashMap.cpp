#define MAX_TABLE 500007
#define kMaxWords 500007

//#define DEBUG

#ifdef DEBUG
#include <stdio.h>
#endif

struct hash_table
{
	unsigned long long word_int;
	bool deleted = false;
};

struct Node
{
	int value;
	struct Node* next[0];
};

Node* some_node;

hash_table hash_array[MAX_TABLE];

void init() {
	for (register int i = 0; i < MAX_TABLE; i++)
	{
		hash_array[i].word_int = 0;
		hash_array[i].deleted = false;
	}
}

void add(char* word) {
#ifdef DEBUG
	printf("add: %s\n", word);
#endif
	unsigned long long hash_full = *(unsigned long long *)(word) & 0xFFFFFFFFFFFFull;
	register int  hash_cut= hash_full % MAX_TABLE;
	while (hash_array[hash_cut].word_int && hash_array[hash_cut].word_int != hash_full)
	{
		hash_cut = (hash_cut + 1) % MAX_TABLE;
	}

	hash_array[hash_cut].word_int = hash_full;
	hash_array[hash_cut].deleted = false;
}


void del(char* word) {
#ifdef DEBUG
	printf("del: %s\n", word);
#endif

	register unsigned long long hash_full = *(unsigned long long *)(word) & 0xFFFFFFFFFFFFull;
	register int  hash_cut = hash_full % MAX_TABLE;
	while (hash_array[hash_cut].word_int)
	{
		if (hash_array[hash_cut].word_int == hash_full && !hash_array[hash_cut].deleted)
		{
			hash_array[hash_cut].deleted = true;
			return;
		}
		hash_cut = (hash_cut + 1) % MAX_TABLE;
	}

	
}

bool check(char* word) {
#ifdef DEBUG
	printf("check: %s - ", word);
#endif

	register unsigned long long hash_full = *(unsigned long long *)(word) & 0xFFFFFFFFFFFFull;
	register int  hash_cut = hash_full % MAX_TABLE;
	while (hash_array[hash_cut].word_int)
	{
		if (hash_array[hash_cut].word_int == hash_full && !hash_array[hash_cut].deleted)
		{
#ifdef DEBUG
			printf("true\n");
#endif
			return true;
		}
		hash_cut = (hash_cut + 1) % MAX_TABLE;
	}
#ifdef DEBUG
	printf("false\n");
#endif
	return false;
}