#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <queue>
#include <functional>
#include <condition_variable>
#include <memory>
#include <stdexcept>
#include <utility>
#include <chrono>

using namespace std;

class threadpool
{
public:
    explicit threadpool(size_t numWorkers = thread::hardware_concurrency());

    ~threadpool();

    using jobType = function<void(void)>;
    void doJob(jobType job);

    threadpool(const threadpool&) = delete;
    threadpool(threadpool&&) = delete;
    threadpool& operator =(const threadpool&) = delete;
    threadpool& operator =(threadpool&&) = delete;

    void clearQueue();

private:
    using jobPointer = unique_ptr<jobType>;

    using jobsQueue = queue<jobPointer>;
    jobsQueue mQueue;

    using workers = vector<std::thread>;
    workers mWorkers;

    mutex mQueueLock;
    condition_variable mCondition;


};

#endif // THREADPOOL_H
