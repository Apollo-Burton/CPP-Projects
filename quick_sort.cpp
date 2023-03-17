#include <iostream>

int a[10] = {1309,2982,2013,1994,2651,2610,3096,448,2018,350};
// The numbers the algorithm will sort ^.
int size = 10;

// Partition the array a between indices l and h (inclusive) by selecting the
// first element as the pivot and dividing the array such that elements less
// than or equal to the pivot are to the left, and elements greater than the
// pivot are to the right.
int partition(int l, int h)
{
	int pivot = a[l];  // Select the first element as the pivot.
	int i = l;
	int j = h;
	while (i < j)
	{
		// Move the i pointer to the right until we find an element that is
		// greater than the pivot.
		do
		{
			i++;
		}
		while (a[i] <= pivot);
		
		// Move the j pointer to the left until we find an element that is
		// less than or equal to the pivot.
		do
		{
			j--;
		}
		while (a[j] > pivot);
		
		// If i is less than j, swap the elements at i and j to keep the
		// elements less than or equal to the pivot to the left, and the
		// elements greater than the pivot to the right.
		if (i < j)
			std::swap(a[i], a[j]);
	}
	// Move the pivot to the correct position, which is j.
	std::swap(a[l], a[j]);
	return j;
}

// Sort the array a between indices l and h (inclusive) using quicksort.
void quick_sort(int l, int h)
{
	if (l < h)  // If the partitioned array has 1 element or less, stop the recursion. The array is sorted.
	{
		int j = partition(l, h);  // Divide the array into two partitions.
		quick_sort(l, j);  // Sort the left partition.
		quick_sort(j + 1, h);  // Sort the right partition.
	}
}

std::string verify_sort()
{
	for (int i = 0; i < size; i += 2)
		if (a[i] > a[i + 1])
			return "unsuccesfully";
	return "succesfully";
}

int main()
{
	char pref;
	char verify;
	std::string status;
	
	std::cout << "\nSorting array...\n";
	quick_sort(0, size);  // Sort the entire array.
	
	std::cout << "Verify sort? (Y/n) "; // Ask if the user wants to verify the sort.
	std::cin >> verify;

	if (verify == 'Y')
	{
		status = verify_sort(); // Verify the sort if the user wants to.
		std::cout << "Array " << status << " sorted\n\n";
	}

	std::cout << "See new array? (Y/n) ";
	std::cin >> pref;

	if (pref == 'Y')
		for (int i = 0; i < size; i++)
			std::cout << a[i] << ", ";  // Print out the sorted array.
	std::cout << "\n";
}
