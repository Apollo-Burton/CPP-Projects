// Broken as fuck

#include <iostream>
#include <vector>

// Function that swaps parameters
void swap(int &a, int &b)
{
	int temp = a;
	a = b;
	b = temp;
}

std::vector<std::vector<int>> partition(std::vector<int> vec)
{
	if (vec.size() == 1) {
	    return {{vec}, {vec}, {0, vec[0]}};
	}
	if (vec.size() == 0) {
	    return {{0}, {0}, {0, 0}};
	}
	// Initializing variables
	std::vector<int> vec1;
	std::vector<int> vec2;
	int pivot = vec.size() / 2;
	int	back = vec.size() - 1;
	int pointerl = 0;
	int pointerh = vec.size() - 2;
	bool pointingl = false;
	bool pointingh = false;
	swap(vec[back], vec[pivot]);
	swap(back, pivot);

	while (pointerl <= pointerh) {
		if (vec[pointerl] > vec[pivot]) {
			pointingl = true;
		}
		if (vec[pointerh] < vec[pivot]) {
			pointingh = true;
		}
		if (pointingl && pointingh) {
			swap(vec[pointerl], vec[pointerh]);
			pointingl = false;
			pointingh = false;
		}
		if (pointingl == false) {
			pointerl++;
		}
		if (pointingh == false) {
			pointerh--;
		}
	}
	// swap(back, pivot);
	// swap(vec[back], vec[pivot]);
	
	if (pointingl) {
		swap(vec[pointerl], vec[pivot]);
		pivot = pointerl;
	}
	if (pointingh) {
		swap(vec[pointerh], vec[pivot]);
		pivot = pointerh;
	}
	for (int i = 0; i < pivot; i++) {
		vec1.push_back(vec[i]);
	}
	for (int i = pivot + 1; i < vec.size(); i++) {
		vec2.push_back(vec[i]);
	}
	return {{vec1}, {vec2}, {pivot, vec[pivot]}};
}

std::vector<int> recursion(std::vector<int> input)
{
	std::vector<std::vector<int>> vec = partition(input);
	std::vector<int> vec1 = vec[0];
	std::vector<int> vec2 = vec[1];
	std::vector<int> sorted(input.size());
	int sorted_size = 1;
	sorted[vec[2][0]] = vec[2][1];
	
	while (sorted_size <= input.size() && vec[0].size() > 1) {
		if (vec[0].size() > 1) {
		    vec = partition(vec1);
		    vec1 = vec[0];
		}
		sorted[vec[2][0]] = vec[2][1];
		sorted_size++;
		
		if (vec[1].size() > 1) {
            vec = partition(vec2);
            vec2 = vec[1];
		}
		sorted[vec[2][0]] = vec[2][1];
        sorted_size++;
	}
	return sorted;
}

int main()
{
	std::vector<int> vec = {4, 8, 6, 2, 5, 8, 45, 3, 7, 3, 7, 9, 1, 34, 2, 15, 7, 5};
	
	std::cout << "Unsorted: ";
	for (int i : vec) {
		std::cout << i << " ";
	}
	std::cout << "\n";

	std::vector<int> sorted = recursion(vec);

	std::cout << "  Sorted: ";
	for (int i : sorted) {
		std::cout << i << " ";
	}
}
