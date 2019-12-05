#include <stdio.h>


static const int max_level = 16;
static const int max_level_index = max_level - 1;
static const int max_skiplist_elements = 50002;



unsigned short lfsr = 0xACE1u;
unsigned bit;

unsigned my_rand()
{
	bit = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5)) & 1;
	return lfsr = (lfsr >> 1) | (bit << 15);
}

struct Node {
	int num;
	Node* levels[max_level];
};

Node skiplist_mem[max_skiplist_elements*max_level];
int skiplist_mem_index;

inline Node* create_new_node(int num = -1)
{
	Node* new_node = &skiplist_mem[skiplist_mem_index++];
	for (register int i = 0; i < max_level; i++)
		new_node->levels[i] = nullptr;
	new_node->num = num;
	return new_node;
}

static inline int random_level()
{
	register int lvl = 0;
	while (my_rand() % 2 == 0 && lvl <= max_level_index)
		lvl++;
	return lvl;
}


class Skiplist {
public:
	Skiplist() {
		skiplist_mem_index = 0;
		level = 0;
		head = create_new_node();
	}

	bool search(int target) {
		//printf("obj->search(%d);\n", target);
		const register Node* node = head;
		for (register int i = level; i >= 0; --i)
		{
			while (node->levels[i] && node->levels[i]->num < target)
				node = node->levels[i];
		}
		node = node->levels[0];
		return node && node->num == target;
	}

	void add(int num)
	{
		//printf("obj->add(%d);\n", num);
		register Node* node = head;
		Node* update[max_level] = { 0 };
		for (int i = level; i >= 0; --i)
		{
			while (node->levels[i] && node->levels[i]->num <= num)
				node = node->levels[i];
			update[i] = node;
		}

		node = node->levels[0];

		if (!node || node->num != num)
		{
			int lvl = random_level();
			if (lvl > level)
			{
				lvl = ++level;
				update[lvl] = head;
			}
			node = create_new_node(num);
			for (int i = 0; i <= lvl; ++i)
			{
				node->levels[i] = update[i]->levels[i];
				update[i]->levels[i] = node;
			}
		}

	}

	bool erase(int num)
	{
		//printf("obj->erase(%d);\n", num);
		Node* node = head;
		Node* update[max_level] = { 0 };
		for (int i = level; i >= 0; --i)
		{
			while (node->levels[i] && node->levels[i]->num < num)
				node = node->levels[i];
			update[i] = node;
		}
		node = node->levels[0];

		if (node && node->num == num)
		{
			for (int i = 0; i <= level; ++i) {
				if (update[i]->levels[i] != node)
					break;
				update[i]->levels[i] = node->levels[i];
			}
			while (level >= 0 && !head->levels[level])
			{

				--level;
			}
			return true;
		}
		return false;
	}

private:

	Node* head;
	int level;
};


/**
 * Your Skiplist object will be instantiated and called as such:
 * Skiplist* obj = new Skiplist();
 * bool param_1 = obj->search(target);
 * obj->add(num);
 * bool param_3 = obj->erase(num);
 */


int main()
{
	Skiplist* obj = new Skiplist();
	obj->add(16);
	obj->add(5);
	obj->add(14);
	obj->add(13);
	obj->add(0);
	obj->add(3);
	obj->add(12);
	obj->add(9);
	obj->add(12);
	obj->erase(3);
	obj->search(6);
	obj->add(7);
	obj->erase(0);
	obj->erase(1);
	obj->erase(10);
	obj->add(5);
	obj->search(12);
	obj->search(7);
	obj->search(16);
	obj->erase(7);
	obj->search(0);
	obj->add(9);
	obj->add(16);
	obj->add(3);
	obj->erase(2);
	obj->search(17);
	obj->add(2);
	obj->search(17);
	obj->erase(0);
	obj->search(9);
	obj->search(14);
	obj->erase(1);
	obj->erase(6);
	obj->add(1);
	obj->erase(16);
	obj->search(9);
	obj->erase(10);
	obj->erase(9);
	obj->search(2);
	obj->add(3);
	obj->add(16);
	obj->erase(15);
	obj->erase(12);
	obj->erase(7);
	obj->add(4);
	obj->erase(3);
	obj->add(2);
	obj->erase(1);
	obj->erase(14);
	obj->add(13);
	obj->add(12);
	obj->add(3);
	obj->search(6);
	obj->search(17);
	obj->add(2);
	obj->erase(3);
	obj->search(14);
	obj->add(11);
	obj->add(0);
	obj->search(13);
	obj->add(2);
	obj->search(1);
	obj->erase(10);
	obj->erase(17);
	obj->search(0);
	obj->search(5);
	obj->erase(8);
	obj->search(9);
	obj->add(8);
	obj->erase(11);
	obj->search(10);
	obj->erase(11);
	obj->search(10);
	obj->erase(9);
	obj->erase(8);
	obj->search(15);
	obj->search(14);
	obj->add(1);
	obj->add(6);
	obj->add(17);
	obj->add(16);
	obj->search(13);
	obj->search(4);
	obj->search(5);
	obj->search(4);
	obj->search(17);
	obj->search(16);
	obj->search(7);
	obj->search(14);
	obj->search(1);
	return 0;

}