#include <iostream>
#include <vector>

void swap(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
}

std::vector<int> quick_sort(std::vector<int> vec)
{
    int pivot = vec.size() / 2;
    int pointerl = 0;
    int pointerh = vec.size() - 1;
    bool pointingl = false;
    bool pointingh = false;
    
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
    }
    if (pointingh) {
        swap(vec[pointerh], vec[pivot]);
    }
    
    return vec;
}

int main()
{
    std::vector<int> vec = quick_sort({7, 5, 4, 6, 3, 2});
    for (int i : vec) {
        std::cout << i << " ";
    }
}
