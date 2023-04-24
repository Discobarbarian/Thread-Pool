#include "threadpool.h"

threadpool::threadpool(size_t numThreads)
{
    if(!numThreads) throw invalid_argument("Bad amount of threads!");

           cout << "creating " << numThreads << " workers..." << endl;
           for(auto i = 0; i < numThreads; ++i)
           {
               mWorkers.push_back(thread([this]()
               {
                   while (true)
                   {
                       jobPointer job(nullptr);
                       {
                           unique_lock guard(mQueueLock);
                           mCondition.wait(guard, [&]() { return !mQueue.empty(); });
                           job = move(mQueue.front());
                           mQueue.pop();
                       }
                       if(!job)
                       {
                           break;
                       }
                       (*job)();
                       this_thread::sleep_for(2000ms);
                   }
               }));
           }
}

threadpool::~threadpool()
{
    {
        unique_lock guard(mQueueLock);
        for(auto& worker: mWorkers)
        mQueue.push(jobPointer(nullptr));
    }
    for(auto& worker : mWorkers)
    {
       worker.join();
    }
}

void threadpool::doJob(jobType job)
{
    auto jobItem = make_unique<jobType>(move(job));
    {
        unique_lock guard(mQueueLock);
        mQueue.push(move(jobItem));
    }
    mCondition.notify_one();
}

void threadpool::clearQueue()
{
    {
        unique_lock guard(mQueueLock);
        while (!mQueue.empty()) mQueue.pop();
    }
    mCondition.notify_all();
}
