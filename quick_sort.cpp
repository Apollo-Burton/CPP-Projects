#include <iostream>
#include <vector>

// Function that swaps parameters
void swap(int &a, int &b)
{
	int temp = a;
	a = b;
	b = temp;
}

std::vector<int> quick_sort(std::vector<int> &vec)
{
	// Initializing variables
	int pivot = vec.size() / 2;
	int pointerl = 0;
	int pointerh = vec.size() - 1;
	bool pointingl = false;
	bool pointingh = false;
	
	// Running quick sort
	while(pointerl < pointerh || pointerh > pointerl) {
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
		if (pointingl == false && (pointerl) - 1 < pivot) {
			pointerl++;
		}
		if (pointingh == false && (pointerh) + 1 > pivot) {
			pointerh--;
		}
	}
	if (pointingl) {
		swap(vec[pointerl], vec[pivot]);
		pivot = pointerl;
	}
	if (pointingh) {
		swap(vec[pointerh], vec[pivot]);
		pivot = pointerh;
	}
	vec.push_back(pivot);
	return vec;
}

std::vector<int> recursion(std::vector<int> &vec)
{
	quick_sort(vec);
	int pivot = vec[vec.size() - 1];
	int sections = 1;
	std::vector<int> section;
	std::vector<int> sorted_vec;
	bool sorted = false;

	while (sorted == false) {
		sorted = true;
		for (int i = 0; i < vec.size(); i += 2) {
			if (vec[i] > vec[i + 1]) {
				// Change to "sorted = false" once recursion is figured out
				sorted = true;
			}
		}
		for (int i = 0; i < pivot; i++) {
		    section.push_back(vec[i]);
		}
		quick_sort(section);
		
		for (int i = 0; i < section.size() - 1; i++) {
		    sorted_vec.push_back(section[i]);
		}
		section.clear();
	}
	
	return sorted_vec;
}

int main()
{
	std::vector<int> vec = {3, 2, 5, 0, 1, 8, 7, 6, 9, 4};

	std::cout << "Unsorted: ";
	for (int i : vec) {
		std::cout << i << " ";
	}
	std::cout << "\n";

    std::vector<int> test = quick_sort(vec);

	std::cout << "  Sorted: ";
	for (int i : test) {
		std::cout << i << " ";
	}
}
// int sections *= 2 everytime quick_sort() is called
// for (section : sections) {
//     quick_sort(section);
// }
