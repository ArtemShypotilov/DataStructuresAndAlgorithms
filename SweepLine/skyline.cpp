


class skyline_point
{
    public:
    
        skyline_point(int x, int height, int* index):
                            x(x),
                            height(height),
                            index(index){};
    
        int x;
        int height;
        int *index;
    
    bool operator < (const skyline_point& that) const
    {
        return (x < that.x);
    }
    
};

static const int max_heap = 10000;

int indexes_array[max_heap];
skyline_point* heap[max_heap];
int heap_size;


void heapify_bottom_top(int index) {
	skyline_point* temp;
	int parent_node = (index - 1) / 2;

	if (heap[parent_node]->height < heap[index]->height) {
		//swap and recursive call
        *(heap[index]->index) = parent_node;
        *(heap[parent_node]->index) = index;
		temp = heap[parent_node];
		heap[parent_node] = heap[index];
		heap[index] = temp;
		heapify_bottom_top(parent_node);
	}
}

void heapify_top_bottom(int parent_node) {
	int left = parent_node * 2 + 1;
	int right = parent_node * 2 + 2;
	int min;
	skyline_point* temp;

	if (left >= heap_size || left < 0)
		left = -1;
	if (right >= heap_size || right < 0)
		right = -1;

	if (left != -1 && heap[left]->height > heap[parent_node]->height)
		min = left;
	else
		min = parent_node;
	if (right != -1 && heap[right]->height > heap[min]->height)
		min = right;

	if (min != parent_node) {
        *(heap[min]->index) = parent_node;
        *(heap[parent_node]->index) = min;
		temp = heap[min];
		heap[min] = heap[parent_node];
		heap[parent_node] = temp;
		// recursive  call
		heapify_top_bottom(min);
	}
}

void push_heap(skyline_point* key) {
    *(key->index)=heap_size; 
	heap[heap_size] = key;
	heapify_bottom_top(heap_size);
	heap_size++;

}

void erase_at_index(int index){
	heap[index] = heap[heap_size - 1];
    *(heap[index]->index) = index;
	heap_size--;
	heapify_top_bottom(index);
}


class Solution {
public:
    vector<vector<int>> getSkyline(vector<vector<int>>& buildings) {
        vector<skyline_point> all_points;
        vector<vector<int>> results;
        if (!buildings.size()) return results;
        heap_size=0;
        for(int i=0;i<buildings.size();i++)
        {
            all_points.push_back(skyline_point(buildings[i][0], buildings[i][2], &indexes_array[i]));
            all_points.push_back(skyline_point(buildings[i][1], 0, &indexes_array[i]));
            
        }
        
        sort(all_points.begin(), all_points.end(),less<skyline_point>());
        
        push_heap(&all_points[0]);
        results.push_back({all_points[0].x, all_points[0].height});
        
        for(int i=1; i< all_points.size(); i++)
        {
            int x=0;
            int y=0;
        
            if (!all_points[i].height) erase_at_index(*(all_points[i].index));
            else push_heap(&all_points[i]);
            
            x = all_points[i].x;
            y = heap_size!=0?heap[0]->height:0;
            
            if (results[results.size()-1][0]==x)
            {
                results[results.size()-1][1]=y;
                if ((int)results.size()-2>=0 && results[results.size()-1][1]==results[results.size()-2][1])
                    results.pop_back();
                continue;
            }
            
            if (results[results.size()-1][1]!=y)
                results.push_back({x,y});
        
        }
        
        return results;
    }
};
