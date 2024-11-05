#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <atomic>

std::atomic_bool stop;


std::vector<unsigned long long> primes;

void primes_worker()
{
    primes.push_back(2);
    unsigned long long n = 3;
    while (!stop)
    {
        bool is_prime = true;
        for (unsigned long long i = 2; i * i <= n; i++)
        {
            if (n % i == 0)
            {
                is_prime = false;
                break;
            }
        }
        if (is_prime)
        {
            primes.push_back(n);
        }
        n += 2;
    }
}

int main() {
    using namespace std::chrono_literals;

    std::jthread worker(primes_worker);
    std::cout << "Started worker." << std::endl;

    std::this_thread::sleep_for(5s);
    stop = true;

    std::cout << "Found " << primes.size() << " primes." << std::endl;
    return 0;
}
