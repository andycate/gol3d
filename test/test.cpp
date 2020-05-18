#include <iostream>

void example(int* arr, int pos, int val) {
    arr[pos] = val;
}

int main(int argc, char** argv) {
    std::cout << "Hello, World!" << std::endl;
    int arr[3]{0, 0, 0};
    std::cout << arr[0] << ":" << arr[1] << ":" << arr[2] << std::endl;
    example(arr, 1, 1);
    std::cout << arr[0] << ":" << arr[1] << ":" << arr[2] << std::endl;
    std::cout << (1/2) << std::endl;
    return 0;
}
