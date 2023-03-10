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
	while(pointerl <= pointerh || pointerh >= pointerl) {
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
	if (pointingl) {
		swap(vec[pointerl], vec[pivot]);
		pivot = pointerl;
	}
	if (pointingh) {
		swap(vec[pointerh], vec[pivot]);
		pivot = pointerh;
	}
	//vec.push_back(pivot);
	return vec;
}

int main()
{
	std::vector<int> vec = {7, 8, 1, 3, 5, 2, 3, 6};

	std::cout << "Unsorted: ";
	for (int i : vec) {
		std::cout << i << " ";
	}
	std::cout << "\n";

	quick_sort(vec);
	std::cout << "  Sorted: ";
	for (int i : vec) {
		std::cout << i << " ";
	}
}
