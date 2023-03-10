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
	int pivot = vec.size() - 1;
	int pointerl = 0;
	int pointerh = -1;
	
	while (pointerl < vec.size()) {
		if (vec[pointerl] <= vec[pivot]) {
			pointerh++;
			if (vec[pointerl] < vec[pointerh]) {
				if (pointerl == pivot) {
					pivot = pointerh;
				}
				swap(vec[pointerl], vec[pointerh]);
			}
		}
		pointerl++;
	}
	vec.push_back(pivot);
	return vec;
}

std::vector<int> recursion(std::vector<int> &vec)
{
	quick_sort(vec);
	// The last element of vec will be the index of the pivot
	int pivot = vec.size() - 1;
	// Partition at pivot (AHHHHHHHHHFLAHBSFLKHBAFLKHBFLKHEBLFKBLKABFELHB)
	return {};
}

int main()
{
	std::vector<int> vec = {8, 7, 6, 9, 5};

	std::cout << "Unsorted: ";
	for (int i : vec) {
		std::cout << i << " ";
	}
	std::cout << "\n\n";

    quick_sort(vec);

	std::cout << "  Sorted: ";
	for (int i : vec) {
		std::cout << i << " ";
	}
}
