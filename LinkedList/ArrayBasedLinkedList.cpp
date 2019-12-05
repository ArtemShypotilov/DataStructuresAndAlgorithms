#define _CRT_SECURE_NO_WARNINGS

#define MAX 200003

struct list_array {
	int size;
	int it;
	int array[MAX];
};

list_array la;

void init() {
	la.size = 0;
	la.it = 0;
}


void decIterator() {
	la.it--;
}
void incIterator() {
	la.it++;
}
void erase() {
	for (int i = la.it; i < la.size-1; i++)
	{
		la.array[i] = la.array[i + 1];
	}
	la.size--;

}
void addBefore(int value) {
	for (int i = la.size; i > la.it; i--)
	{
		la.array[i] = la.array[i-1];
	}
	la.array[la.it] = value;
	la.size++;
	la.it++;
}

int getValue() {
	return la.array[la.it];
}