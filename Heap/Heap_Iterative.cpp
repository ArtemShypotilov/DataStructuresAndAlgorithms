
#define MAX_USER      1000
#define MAX_TAG       5000


int u_followed_by[MAX_USER][MAX_USER];

int u_followed_b_num[MAX_USER];

struct message
{
	int msgID;
	int userID;
	int time;

};

struct message_list {
	message* p_message;
	message_list* next;
};

struct tag_hashed
{
	unsigned long long hash_id;
	int heap_size;
	message* message_heap[5];
	tag_hashed* next;
};


message_list message_list_mem[1000001];
tag_hashed tag_hashed_mem[5001];
message messages_mem[1000001];

#define HASH_TABLE_SIZE 9833

tag_hashed* tag_hashed_table[HASH_TABLE_SIZE];

int tag_hashed_mem_ind;
int message_list_mem_ind;

message_list* user[MAX_USER];


void init()
{
	tag_hashed_mem_ind=0;
	message_list_mem_ind=0;
	for (int i = 0; i < MAX_USER; i++)
	{
		user[i] = nullptr;
		u_followed_b_num[i] = 0;
	}
	for (int i = 0; i < HASH_TABLE_SIZE; i++)
	{
		tag_hashed_table[i] = nullptr;
	}

}

message_list* create_new_message_list(message* p_message)
{
	message_list* new_message_list = &message_list_mem[message_list_mem_ind++];
	new_message_list->p_message = p_message;
	new_message_list->next = nullptr;
	return new_message_list;
}


tag_hashed* create_new_tag_hashed(unsigned long long hash_id)
{
	tag_hashed*  new_tag_hashed = &tag_hashed_mem[tag_hashed_mem_ind++];
	new_tag_hashed->hash_id = hash_id;
	new_tag_hashed->heap_size = 0;
	new_tag_hashed->next = nullptr;;
	return new_tag_hashed;
}

bool compare_for_heap(message *a, message *b)
{
	if (a->time == b->time)
		return a->userID < b->userID;
	return a->time > b->time;
}

void heapify(message *heap[5], int *heap_size)
{

	int item, index, swap, other;
	message* temp;
	item = (*heap_size >> 1) - 1;
	while (1)
	{
		temp = heap[item];
		for (index = item; 1; index = swap)
		{
			swap = (index << 1) + 1;
			if (swap >= *heap_size) break;
			other = swap + 1;
			if ((other < *heap_size) && !compare_for_heap(heap[other], heap[swap])) swap = other;
			if (!compare_for_heap(temp, heap[swap])) break;
			heap[index] = heap[swap];

		}
		if (index != item)
		{
			heap[index] = temp;

		}
		if (!item) return;
		--item;
	}
}

void heap_push(message *heap[5], int *heap_size, message* message_to_push)
{

	if (*heap_size == 5)
		if (compare_for_heap(heap[0], message_to_push))
			return;
		else
		{
			heap[0] = message_to_push;
			heapify(heap, heap_size);
			return;
		}
	int index, parent;
	for (index = (*heap_size)++; index; index = parent)
	{
		parent = (index - 1) >> 1;
		if (!compare_for_heap(heap[parent], message_to_push)) break;
		heap[index] = heap[parent];

	}
	heap[index] = message_to_push;
}

void heap_pop(message *heap[5], int *heap_size)
{
	if (*heap_size == 1)
	{
		*heap_size = 0;
		return;
	}
	int index, swap, other;
	message* temp = heap[--(*heap_size)];
	for (index = 0; 1; index = swap)
	{
		swap = (index << 1) + 1;
		if (swap >= *heap_size) break;
		other = swap + 1;
		if ((other < *heap_size) && !compare_for_heap(heap[other], heap[swap])) swap = other;
		if (!compare_for_heap(temp, heap[swap])) break;
		heap[index] = heap[swap];
	}
	heap[index] = temp;

}


tag_hashed* find_in_hash_table(unsigned int hash_int, unsigned long long hash_id)
{
	tag_hashed* curr_tag_hashed = tag_hashed_table[hash_int];
	while (curr_tag_hashed)
	{
		if (curr_tag_hashed->hash_id == hash_id)
			break;
		curr_tag_hashed = curr_tag_hashed->next;
	}
	return curr_tag_hashed;
}

unsigned int calc_hash_for_tag(unsigned long long hash_id)
{

	unsigned int hash_int = ((hash_id & 0xffffffff00000000)>>32) ^ (hash_id & 0x00000000ffffffff);
	return hash_int % HASH_TABLE_SIZE;
}

void insert_to_hash_table(unsigned long long hash_id, message *new_message)
{

	if (hash_id == 0x0078637069767174)
		int blblbl = 0;

	unsigned int hash_int = calc_hash_for_tag(hash_id);
	tag_hashed* curr_tag_hashed = find_in_hash_table(hash_int, hash_id);
	if (!curr_tag_hashed)
	{
		
		tag_hashed* new_tag_hashed = create_new_tag_hashed(hash_id);
		heap_push(new_tag_hashed->message_heap, &new_tag_hashed->heap_size, new_message);
		if (!tag_hashed_table[hash_int])
		{
			tag_hashed_table[hash_int] = new_tag_hashed;
		}
		else
		{
			new_tag_hashed->next = tag_hashed_table[hash_int];
			tag_hashed_table[hash_int] = new_tag_hashed;
		}
	}
	else
	{
		heap_push(curr_tag_hashed->message_heap, &curr_tag_hashed->heap_size, new_message);
	}
}


void createMessage(int msgID, int userID, char msgData[])
{
	bool after_number = true;
	char curr_char;
	int number=0;
	unsigned long long hash_id = 0;
	message* new_message = &messages_mem[msgID];
	new_message->msgID = msgID;
	new_message->userID = userID;
	while (1)
	{
		curr_char = *(msgData++);
		if (curr_char >= '0' && curr_char <= '9')
			number = number * 10 + (curr_char - '0');
		else if (curr_char >= 'a' && curr_char <= 'z')
		{

			hash_id =(unsigned long long) ((hash_id <<  8) + curr_char);
		}
		else if (curr_char == ' ' || curr_char == '\0')
		{
			if (after_number)
			{
				new_message->time = number;
				after_number = false;
				continue;
			}
			insert_to_hash_table(hash_id, new_message);
			if (curr_char == '\0')
			{
				new_message->time = number;
				break;

			}
			hash_id = 0;
		}
	}
	message_list* new_message_list = create_new_message_list(new_message);
	if (!user[userID])
	{
		user[userID] = new_message_list;
	}
	else
	{
		new_message_list->next = user[userID];
		user[userID] = new_message_list;
	}
}



void followUser(int userID1, int userID2)
{
	u_followed_by[userID1][u_followed_b_num[userID1]++] = userID2;
}


