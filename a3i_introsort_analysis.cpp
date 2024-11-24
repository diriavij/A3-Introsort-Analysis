#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <random>

class ArrayGenerator {
    size_t size_;
    int min_value_;
    int max_value_;
public:
    explicit ArrayGenerator(size_t size, int min_value = 0, int max_value = 6000) : size_(size),
    min_value_(min_value), max_value_(max_value) {
    }

    [[nodiscard]] std::vector<int> getBasicArray() const {
        std::vector<int> return_array(size_);
        for (size_t i = 0; i < size_; i++) {
            return_array[i] = std::rand() % (max_value_ - min_value_ + 1) + min_value_;
        }
        return return_array;
    }

    [[nodiscard]] std::vector<int> getReversedArray() const {
        std::vector<int> return_array = getBasicArray();
        std::ranges::sort(return_array, std::greater<int>());
        return return_array;
    }

    std::vector<int> getAlmostSortedArray() {
        std::vector<int> return_array = getBasicArray();
        std::ranges::sort(return_array);
        size_t swaps = size_ / 100;
        for (size_t i = 0; i < swaps; i++) {
            std::swap(return_array[std::rand() % size_], return_array[std::rand() % size_]);
        }
        return return_array;
    }

};

void heapify(std::vector<int>& heap, int left, int heap_size, int index) {
    while (true) {
        int largest = index;
        int left_child = 2 * (index - left) + 1 + left;
        int right_child = 2 * (index - left) + 2 + left;
        if (left_child < heap_size && heap[left_child] > heap[largest]) {
            largest = left_child;
        }
        if (right_child < heap_size && heap[right_child] > heap[largest]) {
            largest = right_child;
        }
        if (largest != index) {
            std::swap(heap[index], heap[largest]);
            index = largest;
        } else {
            break;
        }
    }
}

void buildMaxHeap(std::vector<int>& input, int left, int right) {
    int heap_size = right + 1;
    for (int i = left + (heap_size - left) / 2 - 1; i >= left; --i) {
        heapify(input, left, heap_size, i);
    }
}

void heapSort(std::vector<int>& heap, int left, int right) {
    buildMaxHeap(heap, left, right);
    int heap_size = right + 1;
    for (int i = right; i > left; --i) {
        std::swap(heap[left], heap[i]);
        heap_size--;
        heapify(heap, left, heap_size, left);
    }
}

void insertionSort(std::vector<int>& container, int left, int right) {
    for (int i = left + 1; i <= right; ++i) {
        int key = container[i];
        int j = i - 1;
        while (j >= left && container[j] > key) {
            container[j + 1] = container[j];
            --j;
        }
        container[j + 1] = key;
    }
}

int partition(std::vector<int>& container, int left, int right) {
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(left, right);
    int pivot_index = dist(rng);
    std::swap(container[pivot_index], container[right]);
    int pivot = container[right];
    int i = left - 1;
    for (int j = left; j <= right - 1; j++) {
        if (container[j] <= pivot) {
            i++;
            std::swap(container[i], container[j]);
        }
    }
    std::swap(container[i + 1], container[right]);
    return i + 1;
}

void introSort(std::vector<int>& container, int left, int right, int recursion_depth = 0) {
    if (left < right) {
        if (right - left + 1 < 16) {
            insertionSort(container, left, right);
        } else if (recursion_depth >= 2 * std::log2(right - left + 1)) {
            heapSort(container, left, right);
        } else {
            int pi = partition(container, left, right);
            introSort(container, left, pi - 1, recursion_depth + 1);
            introSort(container, pi + 1, right, recursion_depth + 1);
        }
    }
}

void quickSort(std::vector<int>& container, int left, int right, int recursion_depth = 0) {
    if (left < right) {
        int pi = partition(container, left, right);
        quickSort(container, left, pi - 1, recursion_depth + 1);
        quickSort(container, pi + 1, right, recursion_depth + 1);
    }
}

class SortTester {
public:
    static long long runQuickSort(std::vector<int> tester_array) {
        size_t left = 0;
        size_t right = tester_array.size() - 1;
        auto start = std::chrono::high_resolution_clock::now();
        quickSort(tester_array, left, right);
        auto elapsed = std::chrono::high_resolution_clock::now()- start;
        return std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    }

    static long long runIntroSort(std::vector<int> tester_array) {
        size_t left = 0;
        size_t right = tester_array.size() - 1;
        auto start = std::chrono::high_resolution_clock::now();
        introSort(tester_array, left, right);
        auto elapsed = std::chrono::high_resolution_clock::now()- start;
        return std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    ArrayGenerator generator(10000);
    std::vector<int> basic_array = generator.getBasicArray();
    std::vector<int> reversed_array = generator.getReversedArray();
    std::vector<int> almost_sorted_array = generator.getAlmostSortedArray();
    std::cout << "BASIC ARRAY\n" << std::endl;
    for (int array_size = 500; array_size <= 10000; array_size += 100) {
        std::vector<int> tester_array(basic_array.begin(), basic_array.begin() + array_size);
        long long microsec_first = SortTester::runQuickSort(tester_array);
        long long microsec_second = SortTester::runIntroSort(tester_array);
        std::cout << array_size << " " << microsec_first << " " << microsec_second << std::endl;
    }
    std::cout << "\nREVERSED ARRAY\n" << std::endl;
    for (int array_size = 500; array_size <= 10000; array_size += 100) {
        std::vector<int> tester_array(reversed_array.begin(), reversed_array.begin() + array_size);
        long long microsec_first = SortTester::runQuickSort(tester_array);
        long long microsec_second = SortTester::runIntroSort(tester_array);
        std::cout << array_size << " " << microsec_first << " " << microsec_second << std::endl;
    }
    std::cout << "\nALMOST SORTED ARRAY\n" << std::endl;
    for (int array_size = 500; array_size <= 10000; array_size += 100) {
        std::vector<int> tester_array(almost_sorted_array.begin(), almost_sorted_array.begin() + array_size);
        long long microsec_first = SortTester::runQuickSort(tester_array);
        long long microsec_second = SortTester::runIntroSort(tester_array);
        std::cout << array_size << " " << microsec_first << " " << microsec_second << std::endl;
    }
    return 0;
}