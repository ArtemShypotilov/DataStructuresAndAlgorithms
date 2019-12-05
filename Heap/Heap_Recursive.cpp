#define MAX_SCENARIO_LEN 64
#define MAX_MATERIALS_LEN 96
#define SCENARIO_ALPHABET 8
#define MATERIALS_ALPHABET 16
#define FIRST_SYMBOL 97
#define TOPS 5

#define MAX_MATERIAL 33620
#define MAX_SCENARIO 420

#define MATERIAL_MAX_HASH 70000
#define SCENARIO_MAX_HASH 1000

//#define MAX_MATERIAL_TRIE MAX_MATERIALS_LEN*MAX_MATERIAL
//#define MAX_SCENARIO_TRIE MAX_SCENARIO_LEN*MAX_SCENARIO*100

#define MAX_MATERIAL_TRIE 836662
#define MAX_SCENARIO_TRIE 797469

//#define DEBUG

#ifdef DEBUG
#include <stdio.h>
#endif

struct material_trie_node {
	short int mass;
	material_trie_node *childs[MATERIALS_ALPHABET];
};

struct scenario_trie_node {
	short int mass;
	scenario_trie_node *childs[SCENARIO_ALPHABET];
};

struct material_hash_node
{
	char name[MAX_MATERIALS_LEN];
	scenario_trie_node *scenario_trie_root;
	material_hash_node* next;
};

struct scenario_hash_node
{
	char name[MAX_SCENARIO_LEN];
	material_trie_node *material_trie_root;
	scenario_hash_node* next;
};

material_hash_node *material_hash_table[MATERIAL_MAX_HASH];
scenario_hash_node *scenario_hash_table[SCENARIO_MAX_HASH];

material_hash_node material_hash_node_mem[MAX_MATERIAL];
scenario_hash_node scenario_hash_node_mem[MAX_SCENARIO];

int material_hash_node_mem_ind;
int scenario_hash_node_mem_ind;

material_trie_node material_trie_node_mem[MAX_MATERIAL_TRIE];
scenario_trie_node scenario_trie_node_mem[MAX_SCENARIO_TRIE];

int material_trie_node_mem_ind;
int scenario_trie_node_mem_ind;


unsigned int hash_func(const char *str, int hash_table_size)
{
	unsigned int hash = 5381;
	int c;

	while (c = *str++)
	{
		hash = (((hash << 5) + hash) + c);
	}

	return hash % hash_table_size;
}

int mstrncmp(const char *a, const char *b, int len = 4)
{
	for (int i = 0; i < len; i++)
	{
		if (a[i] != b[i])
			return a[i] - b[i];
	}
	return 0;
}

void mstrncpy(char *dest, const char *src, int len = 4)
{
	for (int i = 0; i < len; i++)
	{
		dest[i] = src[i];
	}
	dest[len] = '\0';
}

int mstrcmp(const char *a, const char *b)
{
	int i;
	for (i = 0; a[i] != '\0'; i++)
	{
		if (a[i] != b[i])
			return a[i] - b[i];
	}
	return a[i] - b[i];
}


void mstrcpy(char *dest, const char *src)
{
	int i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = src[i];
}

material_trie_node* create_new_material_trie_node()
{
	material_trie_node* new_material_trie_node = &material_trie_node_mem[material_trie_node_mem_ind++];
	for (int i = 0; i < MATERIALS_ALPHABET; i++)
		new_material_trie_node->childs[i] = nullptr;
	new_material_trie_node->mass = -1;
	return new_material_trie_node;

}

scenario_trie_node* create_new_scenario_trie_node()
{
	scenario_trie_node* new_scenario_trie_node = &scenario_trie_node_mem[scenario_trie_node_mem_ind++];
	for (int i = 0; i < SCENARIO_ALPHABET; i++)
		new_scenario_trie_node->childs[i] = nullptr;
	new_scenario_trie_node->mass = -1;
	return new_scenario_trie_node;
}

material_hash_node * create_new_material_hash_node(char *material_name)
{
	material_hash_node* new_material_hash_node = &material_hash_node_mem[material_hash_node_mem_ind++];
	mstrncpy(new_material_hash_node->name, material_name);
	new_material_hash_node->scenario_trie_root = create_new_scenario_trie_node();
	new_material_hash_node->next = nullptr;
	return new_material_hash_node;
}

scenario_hash_node * create_new_scenario_hash_node(char *scenario_name)
{
	scenario_hash_node* new_scenario_hash_node = &scenario_hash_node_mem[scenario_hash_node_mem_ind++];
	mstrcpy(new_scenario_hash_node->name, scenario_name);
	new_scenario_hash_node->material_trie_root = create_new_material_trie_node();
	new_scenario_hash_node->next = nullptr;
	return new_scenario_hash_node;
}




void init()
{
	material_trie_node_mem_ind = 0;
	scenario_trie_node_mem_ind = 0;
	material_hash_node_mem_ind = 0;
	scenario_hash_node_mem_ind = 0;
	for (int i = 0; i < MATERIAL_MAX_HASH; i++) material_hash_table[i] = nullptr;
	for (int i = 0; i < SCENARIO_MAX_HASH; i++) scenario_hash_table[i] = nullptr;
}

void search_scenario_trie_add_if_needed(char *scenario, scenario_trie_node* root, short int mass)
{
	scenario_trie_node* curr_scenario_trie_node = root;

	while (*scenario)
	{
		int index = *scenario - 'a';
		scenario++;
		if (!curr_scenario_trie_node->childs[index])
			curr_scenario_trie_node->childs[index] = create_new_scenario_trie_node();
		curr_scenario_trie_node = curr_scenario_trie_node->childs[index];

	}
	curr_scenario_trie_node->mass = mass;
}

void search_material_trie_add_if_needed(char *materials, material_trie_node* root, short int mass)
{
	material_trie_node* curr_material_trie_node = root;

	while (*materials)
	{
		int index = *materials - 'a';
		materials++;
		if (!curr_material_trie_node->childs[index])
			curr_material_trie_node->childs[index] = create_new_material_trie_node();
		curr_material_trie_node = curr_material_trie_node->childs[index];

	}
	curr_material_trie_node->mass = mass;
}

scenario_hash_node* search_scenario_hash_table(int scenario_hash, char *scenario)
{
	scenario_hash_node* curr_scenario_hash_node = scenario_hash_table[scenario_hash];
	while (curr_scenario_hash_node && mstrcmp(curr_scenario_hash_node->name, scenario))
		curr_scenario_hash_node = curr_scenario_hash_node->next;
	return curr_scenario_hash_node;
}

material_hash_node* search_material_hash_table(int material_hash, char *materials)
{
	material_hash_node* curr_material_hash_node = material_hash_table[material_hash];
	while (curr_material_hash_node && mstrncmp(curr_material_hash_node->name, materials))
		curr_material_hash_node = curr_material_hash_node->next;
	return curr_material_hash_node;
}

void integrate(char scenario[MAX_SCENARIO_LEN], char materials[MAX_MATERIALS_LEN], int mass)
{
#ifdef DEBUG
	printf("integrate - scenario=%s, materials=%s, mass=%d\n", scenario, materials, mass);
#endif
	if (mass == 951)
		int blablabla = 0;
	int scenario_hash = hash_func(scenario, SCENARIO_MAX_HASH);
	int materials_hash = hash_func(materials, MATERIAL_MAX_HASH);

	scenario_hash_node* curr_scenario_hash_node = search_scenario_hash_table(scenario_hash, scenario);

	if (!curr_scenario_hash_node)
	{
		curr_scenario_hash_node = create_new_scenario_hash_node(scenario);
		curr_scenario_hash_node->next = scenario_hash_table[scenario_hash];
		scenario_hash_table[scenario_hash] = curr_scenario_hash_node;
	}

	search_material_trie_add_if_needed(materials, curr_scenario_hash_node->material_trie_root, mass);

	material_hash_node* curr_material_hash_node = search_material_hash_table(materials_hash, materials);

	if (!curr_material_hash_node)
	{
		curr_material_hash_node = create_new_material_hash_node(materials);
		curr_material_hash_node->next = material_hash_table[materials_hash];
		material_hash_table[materials_hash] = curr_material_hash_node;
	}

	search_scenario_trie_add_if_needed(scenario, curr_material_hash_node->scenario_trie_root, mass);


}

int heap[5];
int heap_size;

void heapify_bottom_top(int index) {
	int temp;
	int parent_node = (index - 1) / 2;

	if (heap[parent_node] > heap[index]) {
		//swap and recursive call
		temp = heap[parent_node];
		heap[parent_node] = heap[index];
		heap[index] = temp;
		heapify_bottom_top(parent_node);
	}
}

void push_heap(int key) {

	heap[heap_size] = key;
	heapify_bottom_top(heap_size);
	heap_size++;

}



void heapify_top_bottom(int parent_node) {
	int left = parent_node * 2 + 1;
	int right = parent_node * 2 + 2;
	int min;
	int temp;

	if (left >= heap_size || left < 0)
		left = -1;
	if (right >= heap_size || right < 0)
		right = -1;

	if (left != -1 && heap[left] < heap[parent_node])
		min = left;
	else
		min = parent_node;
	if (right != -1 && heap[right] < heap[min])
		min = right;

	if (min != parent_node) {
		temp = heap[min];
		heap[min] = heap[parent_node];
		heap[parent_node] = temp;

		// recursive  call
		heapify_top_bottom(min);
	}
}

int pop_heap() {
	int pop;
	pop = heap[0];
	heap[0] = heap[heap_size - 1];
	heap_size--;
	heapify_top_bottom(0);
	return pop;
}

void dfs_material(material_trie_node* curr_material_trie_node)
{
	if (curr_material_trie_node->mass != -1)
	{
		if (heap_size == 5)
		{
			if (heap[0] < curr_material_trie_node->mass)
			{
				heap[0] = curr_material_trie_node->mass;
				heapify_top_bottom(0);
			}
		}
		else push_heap(curr_material_trie_node->mass);
	}
	for (int i = 0; i < MATERIALS_ALPHABET; i++)
		if (curr_material_trie_node->childs[i])
			dfs_material(curr_material_trie_node->childs[i]);
}

void dfs_scenario(scenario_trie_node* curr_scenario_trie_node)
{
	if (curr_scenario_trie_node->mass != -1)
	{
		if (heap_size == 5)
		{
			if (heap[0] < curr_scenario_trie_node->mass)
			{
				heap[0] = curr_scenario_trie_node->mass;
				heapify_top_bottom(0);
			}
		}
		else push_heap(curr_scenario_trie_node->mass);
	}
	for (int i = 0; i < SCENARIO_ALPHABET; i++)
		if (curr_scenario_trie_node->childs[i])
			dfs_scenario(curr_scenario_trie_node->childs[i]);
}

int produceM(char scenario[MAX_SCENARIO_LEN], char materials_prefix[MAX_MATERIALS_LEN], int output[TOPS])
{

#ifdef DEBUG
	printf("produceM - scenario=%s, materials_prefix=%s\n", scenario, materials_prefix);
#endif


	heap_size = 0;
	int ret_val = 0;
	int scenario_hash = hash_func(scenario, SCENARIO_MAX_HASH);
	scenario_hash_node* curr_scenario_hash_node = search_scenario_hash_table(scenario_hash, scenario);

	material_trie_node* curr_material_trie_node = curr_scenario_hash_node->material_trie_root;

	if (!curr_material_trie_node) return ret_val;

	while (*materials_prefix)
	{
		int index = *materials_prefix - 'a';
		materials_prefix++;
		if (!curr_material_trie_node->childs[index]) return ret_val;
		curr_material_trie_node = curr_material_trie_node->childs[index];

	}
	dfs_material(curr_material_trie_node);
	ret_val = heap_size;
#ifdef DEBUG
	printf("size=%d, elements=", heap_size);
#endif
	for (int i = 0; i < ret_val; i++)
	{
		output[i] = pop_heap();
#ifdef DEBUG
		printf("%d ", output[i]);
#endif
	}
#ifdef DEBUG
	printf("\n");
#endif
	return ret_val;
}

int produceS(char scenario_prefix[MAX_SCENARIO_LEN], char materials[MAX_MATERIALS_LEN], int output[TOPS])
{
#ifdef DEBUG
	printf("produceS - scenario_prefix=%s, materials_prefix=%s\n", scenario_prefix, materials);
#endif
	heap_size = 0;
	int ret_val = 0;
	int materials_hash = hash_func(materials, MATERIAL_MAX_HASH);
	material_hash_node* curr_material_hash_node = search_material_hash_table(materials_hash, materials);

	if (!curr_material_hash_node) return ret_val;

	scenario_trie_node* curr_scenario_trie_node = curr_material_hash_node->scenario_trie_root;

	while (*scenario_prefix)
	{
		int index = *scenario_prefix - 'a';
		scenario_prefix++;
		if (!curr_scenario_trie_node->childs[index]) return ret_val;

		curr_scenario_trie_node = curr_scenario_trie_node->childs[index];

	}
	dfs_scenario(curr_scenario_trie_node);
	ret_val = heap_size;
#ifdef DEBUG
	printf("size=%d, elements=", heap_size);
#endif
	for (int i = 0; i < ret_val; i++)
	{
		output[i] = pop_heap();
#ifdef DEBUG
		printf("%d ", output[i]);
#endif
	}
#ifdef DEBUG
	printf("\n");
#endif
	return ret_val;
}
