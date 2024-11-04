#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <set>

std::mutex sync;
std::condition_variable stopper;
bool stop;
std::set<int> terminated;

void worker(int id)
{
    using namespace std::chrono_literals;

    for (int i = 0; i < 20; i++)
    {
        std::unique_lock lock{ sync };
        if (stopper.wait_for(lock, 250ms, [id] { std::cout << id << ": check" << std::endl; return terminated.contains(id); }))
        {
            terminated.erase(id);
            std::cout << "Worker " << id << " interrupted" << std::endl;
            return;
        }
        std::cout << id << ": " << i << std::endl;
    }
    std::cout << "Worker " << id << " finished" << std::endl;
}


int main() {
    using namespace std::chrono_literals;

    std::jthread t1(worker, 1);
    std::jthread t2(worker, 2);

    std::cout << "Hello, World!" << std::endl;

    std::this_thread::sleep_for(1s);
    {
        std::unique_lock lock{ sync };
        stop = true;
        terminated.insert(1);
        stopper.notify_one();
    }
    std::this_thread::sleep_for(8s);
    return 0;
}
