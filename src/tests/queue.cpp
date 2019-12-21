#include "../queue.hpp"

#include <iostream>
#include <iomanip>
#include <thread>
#include <string>

SafeQueue<int> queue;

void addQueue_thread(int thread_id, int count) {
    for (int i=0; i < count; ++i) {
        queue.enqueue(i);
    }
}
void removeQueue_thread(int thread_id, int count) {
    int a;
    for (int i=0; i < count; ++i) {
        //std::cout << queue.dequeue() << "\n";
        if (queue.try_pop(a))
            std::cout << a << "\n";
    }
}

int main() {
    int operation_count = 10;
    int write_count = 3;
    int read_count  = 4;

    std::vector<std::thread> threads;
    threads.reserve(write_count + read_count);

    for (int i=0; i<write_count; i++) {
        threads.emplace_back( addQueue_thread, i, operation_count );
    }
    for (int i=0; i<read_count; i++) {
        threads.emplace_back( removeQueue_thread, i, operation_count );
    }

    for (int i=0; i<write_count + read_count; i++) {
        threads[i].join();
    }
}
