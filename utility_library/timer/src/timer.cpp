#include "timer.h"

void cTimer::threadStart()
{
    ScopedLock lock(sync);

    while (!done)
    {
        if (queue.empty())
        {
            // Wait (forever) for work
            wakeUp.wait(lock);
        }
        else
        {
            auto firstInstance = queue.begin();
            Instance& instance = *firstInstance;
            auto now = Clock::now();
            if (now >= instance.next)
            {
                queue.erase(firstInstance);

                // Mark it as running to handle racing destroy
                instance.running = true;

                // Call the handler
                lock.unlock();
//                if(instance.is_arg){
//                	instance.str_handler(instance.argument);
//                } else {
//
//                }
                instance.handler(instance.argument);
                lock.lock();
                if(!instance.repeat){
                	instance.running = false;
                }
                if (done)
                {
                    break;
                }
                else if (!instance.running)
                {
                    // Running was set to false, destroy was called
                    // for this Instance while the callback was in progress
                    // (this thread was not holding the lock during the callback)
                    active.erase(instance.id);
                }
                else
                {
                    instance.running = false;

                    // If it is periodic, schedule a new one
                    if (instance.period.count() > 0)
                    {
                        instance.next = instance.next + instance.period;
                        queue.insert(instance);
                    } else {
                        active.erase(instance.id);
                    }
                }
            } else {
                // Wait until the timer is ready or a timer creation notifies
                wakeUp.wait_until(lock, instance.next);
            }
        }
    }
}

cTimer::cTimer()
    : nextId(1)
    , queue(comparator)
    , done(false)
{
    ScopedLock lock(sync);
    worker = std::thread(std::bind(&cTimer::threadStart, this));
}

cTimer::~cTimer()
{
    ScopedLock lock(sync);
    done = true;
    wakeUp.notify_all();
    lock.unlock();
    worker.join();
}

//with argument as a int
cTimer::timer_id cTimer::fStartPeriodicTimer(uint64_t Period,
        const std::function<void(int)> &handler, int argument)
{
    return fCreateTimer (Period*1000, handler, 1, argument);
}

cTimer::timer_id cTimer::fStartPeriodicTimer(uint64_t Period,
        std::function<void(int)>&& handler, int argument)
{
    return fCreateTimer (Period*1000, handler, 1, argument);
}

cTimer::timer_id cTimer::fStartTimer(uint64_t Period,
        const std::function<void(int)> &handler, int argument)
{
    return fCreateTimer (Period*1000, handler, 0, argument);
}

cTimer::timer_id cTimer::fStartTimer(uint64_t Period,
        std::function<void(int)>&& handler, int argument)
{
    return fCreateTimer (Period*1000, handler, 0, argument);
}

cTimer::timer_id cTimer::fCreateTimer(uint64_t msPeriod,
        const std::function<void(int)> &handler, bool repeat, 
	int argument)
{
    return createImpl(Instance(0,
            Clock::now() + Duration(msPeriod), Duration(msPeriod),
            handler, repeat, argument));
}

cTimer::timer_id cTimer::fCreateTimer(uint64_t msPeriod,
        std::function<void(int)>&& handler, bool repeat,
	int argument)
{
    return createImpl(Instance(0,
            Clock::now() + Duration(msPeriod), Duration(msPeriod),
            std::move(handler), repeat, argument));
}

cTimer::timer_id cTimer::createImpl(Instance&& item)
{
    ScopedLock lock(sync);
    item.id = nextId++;
    auto iter = active.emplace(item.id, std::move(item));
    queue.insert(iter.first->second);
    wakeUp.notify_all();
    return item.id;
}

bool cTimer::fStopTimer(timer_id id)
{
    ScopedLock lock(sync);
    auto i = active.find(id);
    if (i == active.end())
        return false;
    else if (i->second.running)
    {
        // A callback is in progress for this Instance,
        // so flag it for deletion in the worker
        i->second.running = false;
    }
    else
    {
        queue.erase(std::ref(i->second));
        active.erase(i);
    }

    wakeUp.notify_all();
    return true;
}

bool cTimer::fIsTimerRunning(timer_id id)
{
    ScopedLock lock(sync);
    return active.find(id) != active.end();
}
