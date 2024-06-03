#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <algorithm>
#include <functional>
#include <chrono>
#include <unordered_map>
#include <set>
#include <cstdint>

class cTimer
{
public:
    typedef uint64_t timer_id;
    
    typedef std::string str_argument;
    typedef std::function<void(str_argument)> handler_type_str;
    typedef std::function<void(int)> handler_type_int;

private:
    std::mutex sync;
    typedef std::unique_lock<std::mutex> ScopedLock;

    std::condition_variable wakeUp;

private:
    typedef std::chrono::steady_clock Clock;
    typedef std::chrono::time_point<Clock> Timestamp;
    typedef std::chrono::milliseconds Duration;

    struct Instance
    {
        Instance(timer_id id = 0)
            : id(id)
            , running(false)
            , repeat (true)
            , argument (0)
        {
        }

        template<typename Tfunction>
        Instance(timer_id id, Timestamp next, Duration period, Tfunction&& handler, bool repeat, int argument) noexcept
            : id(id)
            , next(next)
            , period(period)
            , handler(std::forward<Tfunction>(handler))
            , running(false)
            , repeat (repeat)
            , argument (argument)
        {
        }

        Instance(Instance const& r) = delete;

        Instance(Instance&& r) noexcept
            : id(r.id)
            , next(r.next)
            , period(r.period)
            , handler(std::move(r.handler))
            , running(r.running)
            , repeat (r.repeat)
            , argument (r.argument)
        {
        }

        Instance& operator=(Instance const& r) = delete;

        Instance& operator=(Instance&& r)
        {
            if (this != &r)
            {
                id = r.id;
                next = r.next;
                period = r.period;
                handler = std::move(r.handler);
                running = r.running;
                repeat = r.repeat;
                argument = r.argument;
            }
            return *this;
        }

        timer_id id;
        Timestamp next;
        Duration period;
        handler_type_int handler;
        bool running;
        bool repeat;
        int argument;
    };

    typedef std::unordered_map<timer_id, Instance> InstanceMap;
    timer_id nextId;
    InstanceMap active;

    
    struct NextActiveComparator
    {
        bool operator()(const Instance &a, const Instance &b) const
        {
            return a.next < b.next;
        }
    };
    NextActiveComparator comparator;

    
    typedef std::reference_wrapper<Instance> QueueValue;
    typedef std::multiset<QueueValue, NextActiveComparator> Queue;
    Queue queue;

    
    std::thread worker;
    bool done;
    void threadStart();

public:
    cTimer();
    ~cTimer();
    
    timer_id fStartPeriodicTimer(uint64_t period, const handler_type_int& handler, int argument);
    timer_id fStartTimer(uint64_t period, const handler_type_int& handler, int argument);
    timer_id fStartPeriodicTimer(uint64_t period, handler_type_int&& handler, int argument);
    timer_id fStartTimer(uint64_t period, handler_type_int&& handler, int argument);

    








    timer_id fCreateTimer(uint64_t period, const handler_type_int& handler, bool repeat, int argument);
    timer_id fCreateTimer(uint64_t period, handler_type_int&& handler, bool repeat, int argument);

private:
    timer_id createImpl(Instance&& item);

public:
    bool fStopTimer(timer_id id);

    bool fIsTimerRunning(timer_id id);
};

#endif 
