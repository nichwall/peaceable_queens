#ifndef SAFE_QUEUE
#define SAFE_QUEUE

#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>

// A threadsafe-queue.
template <class T>
class SafeQueue
{
    public:
        SafeQueue(void)
            : q()
              , m()
              , c()
              , timeout(10)
        {}

        SafeQueue(size_t ms_delay)
            : q()
              , m()
              , c()
              , timeout(ms_delay)
        {}

        ~SafeQueue(void)
        {}

        // Add an element to the queue.
        void enqueue(T t)
        {
            std::lock_guard<std::mutex> lock(m);
            q.push(t);
            c.notify_one();
        }

        // Get the "front"-element.
        // If the queue is empty, wait till a element is avaiable.
        T dequeue(void)
        {
            std::unique_lock<std::mutex> lock(m);
            while(q.empty())
            {
                // release lock as long as the wait and reaquire it afterwards.
                c.wait(lock);
            }
            T val = q.front();
            q.pop();
            return val;
        }

        // Untested as of yet
        void push(T val)
        {
            {
                std::lock_guard<std::mutex> lock(m);

                q.push(std::move(val));
            }

            c.notify_one();
        }

        //bool try_pop(T& val, std::chrono::milliseconds timeout)
        bool try_pop(T& val)
        {
            std::unique_lock<std::mutex> lock(m);

            if(!c.wait_for(lock, timeout, [this] { return !q.empty(); }))
                return false;

            val = std::move(q.front());
            q.pop();

            return true;
        }

    private:
        std::queue<T> q;
        mutable std::mutex m;
        std::condition_variable c;

        // timeout?
        std::chrono::milliseconds timeout;
};
#endif
