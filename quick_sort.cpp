#include <iostream>
#include <vector>

int a[10] = {1309,2982,2013,1994,2651,2610,3096,448,2018,350};

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
        {
            std::swap(a[i], a[j]);
        }
    }
    // Move the pivot to the correct position, which is j.
    std::swap(a[l], a[j]);
    return j;
}

// Sort the array a between indices l and h (inclusive) using quicksort.
void quick_sort(int l, int h)
{
    if (l < h)
    {
        int j = partition(l, h);  // Divide the array into two partitions.
        quick_sort(l, j);  // Sort the left partition.
        quick_sort(j + 1, h);  // Sort the right partition.
    }
}

int main()
{
    quick_sort(0, 10);  // Sort the entire array.
    for (int i : a)
    {
        std::cout << i << "\n";  // Print out the sorted array.
    }
}
