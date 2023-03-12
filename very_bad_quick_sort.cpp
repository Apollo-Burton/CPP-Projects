// There is a stack overflow in this code I need to find when the input vector in recursion() has more than 40 elements on average

#include <iostream>
#include <vector>

std::vector<int> selection_sort(std::vector<int> vec)
{
    for (int i = 0; i < vec.size(); i++)
    {
        for (int j = i + 1; j < vec.size(); j++)
	{
            if (vec[i] > vec[j])
	    {
                std::swap(vec[i], vec[j]);
            }
        }
    }
    return vec;
}

std::vector<int> partition(std::vector<int> vec)
{
	if (vec.size() <= 1)
	{
	    vec.push_back(0);
	    return vec;
	}
    if (vec.size() == 2)
    {
        vec = selection_sort(vec);
        vec.push_back(0);
        return vec;
    }
    
	// Choosing pivot
	int start = 0;
	int middle = vec.size() / 2;
	int back = vec.size() - 1;
	int pivot = selection_sort({vec[start], vec[middle], vec[back]})[1];
	
	if (pivot == vec[start])
	{
	    pivot = start;
	}
	if (pivot == vec[middle])
	{
	    pivot = middle;
	}
	if (pivot == vec[back])
	{
	    pivot = back;
	}
	
	// Starting partition
	int pointerl = 0;
	int pointerh = vec.size() - 2;
	bool pointingl = false;
	bool pointingh = false;
	std::swap(vec[back], vec[pivot]);
	std::swap(back, pivot);

	while (pointerl <= pointerh)
	{
		if (vec[pointerl] > vec[pivot])
		{
			pointingl = true;
		}
		if (vec[pointerh] < vec[pivot])
		{
			pointingh = true;
		}
		if (pointingl && pointingh)
		{
			std::swap(vec[pointerl], vec[pointerh]);
			pointingl = false;
			pointingh = false;
		}
		if (pointingl == false)
		{
			pointerl++;
		}
		if (pointingh == false && pointingl)
		{
			pointerh--;
		}
	}
	
	if (pointingl)
	{
		std::swap(vec[pointerl], vec[pivot]);
		pivot = pointerl;
	}
	if (pointingh)
	{
		std::swap(vec[pointerh], vec[pivot]);
		pivot = pointerh;
	}
	
	// Adding the pivot index to the back of the vector to be used in recursion
	vec.push_back(pivot);
	return vec;
}

std::vector<int> recursion(std::vector<int> input)
{
	// Partitioning input vector and initializing variables
	std::vector<int> vec = partition(input);
	std::vector<int> vec1;
	std::vector<int> vec2;
	int pivot = vec.back();
	int split = pivot;
	
	// Splitting vectors on partition
	std::vector<int> sorted1(split + 1);
	std::vector<int> sorted2(input.size() - (split + 1));
	sorted1[pivot] = vec[pivot];
    	vec.pop_back();
    
	for (int i = 0; i < sorted1.size(); i++)
	{
	    vec1.push_back(vec[i]);
	}
	for (int i = split + 1; i < vec.size(); i++)
	{
	    vec2.push_back(vec[i]);
	}
	
	// Running recursion
	if (vec1.size() > 1)
	{
		vec1 = recursion(vec1);
	}
	if (vec2.size() > 1)
	{
		vec2 = recursion(vec2);
	}
    
    // Merging and returning the vectors
	for (int i : vec2)
	{
	    vec1.push_back(i);
	}
	
	return vec1;
}

int main()
{
	std::vector<int> vec = {7, 8, 10, 2, 26, 4, 13, 5, 3, 16, 11, 6, 12, 19};
	
	std::cout << " Unsorted: ";
	for (int i : vec)
	{
		std::cout << i << " ";
	}

	std::vector<int> partitioned = partition(vec);
	std::vector<int> sorted = recursion(vec);
 
	std::cout << "\n\nPartition: ";
	for (int i = 0; i < partitioned.size() - 1; i++)
	{
		std::cout << partitioned[i] << " ";
	}
	std::cout << "\n\nRecursion: ";
	for (int i : sorted)
	{
	    std::cout << i << " ";
	}
}
