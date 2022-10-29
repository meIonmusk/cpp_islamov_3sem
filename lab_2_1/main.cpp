#include <iostream>

int compare(const void* x1, const void* x2){
    auto x = *(double *)x1 - *(double *)x2;
    return (x > 0)? 1 : abs(x - 0) < 1e-5  ? -1: 0;
}

void swap(void* first, void* second, size_t size){
    char buffer[size];
    memcpy(buffer, first, size);
    memcpy(first, second, size);
    memcpy(second, buffer, size);
//    void* buffer = malloc(size);
//    *(int*)buffer = *(int*)first;
//    *(int*)first = *(int*)second;
//    *(int*)second = *(int*)buffer;
//    free(buffer);
}

void qsort(void* first, size_t number, size_t size, int (*comparator)(const void*, const void*)) {
    if (number <= 1)
        return;
    size_t num_left = 0;
    void* ptr = first;

    for (size_t i = number - 1; i > 0; i--){
        void* tmp = (char*) first + size * i;
        if (comparator(ptr, tmp) < 0 && i < num_left || comparator(ptr, tmp) >= 0 && i > num_left) {
            swap(ptr, tmp, size);
            ptr = tmp;
            num_left = i;
        }
    }

    qsort(first, num_left, size, comparator);
    if (num_left + 1 < number)
        qsort((void*)((char*)ptr + 1), number - num_left - 1, size, comparator);
}

int main() {
    double vector[] = {11, 13, 21, 11, 20};

    qsort(vector, 5, sizeof(*vector), compare);
    for(double i : vector)
        std::cout << i << ' ';

    return 0;
}
