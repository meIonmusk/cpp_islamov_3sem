#include <iostream>

int main() {
    unsigned n;
    std::cin >> n;
    long long *arr = new long long[n+2];

    arr[1] = 2;
    arr[2] = 3;
    arr[3] = 4;

    for (unsigned i = 4; i <= n; i++)
        arr[i] = arr[i-1] + arr[i-3];

    
    std::cout << arr[n] << std::endl;

    delete[] arr;
    return 0;
}
