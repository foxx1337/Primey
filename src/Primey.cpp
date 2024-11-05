#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <atomic>

std::atomic_bool stop;

std::vector<unsigned long long> primes;

unsigned long long loops = 0;

void primes_worker()
{
    primes.push_back(2);
    unsigned long long n = 3;
    bool ready = true;
    unsigned long long ops = 0;
    while (ready)
    {
        bool is_prime = true;
        for (unsigned long long i = 2; i * i <= n; i++)
        {
            ops += 1;
            if (n % i == 0)
            {
                is_prime = false;
                break;
            }

            if (ops >= 10'000'000)
            {
                loops += ops;
                ops = 0;
                if (stop)
                {
                    ready = false;
                    break;
                }
            }
        }

        if (is_prime)
        {
            primes.push_back(n);
        }
        n += 2;
    }
}

template <class _Rep, class _Period>
void progress_sleep(const std::chrono::duration<_Rep, _Period>& t)
{
    using std::chrono::operator""s;

    std::chrono::steady_clock::time_point stop = std::chrono::steady_clock::now() + t;
    while (std::chrono::steady_clock::now() < stop)
    {
        std::this_thread::sleep_for(1s);
        std::cout << "." << std::flush;
    }
    std::cout << std::endl;
}

int main() {
    using namespace std::chrono_literals;

    primes.reserve(10'000'00);
    std::jthread worker(primes_worker);
    std::cout << "Started worker." << std::endl;

    progress_sleep(5s);
    stop = true;

    std::cout << "Found " << primes.size() << " primes.\n";
    std::cout << "Loops: " << loops << '\n';
    std::cout << "Last prime: " << primes.back() << std::endl;
    return 0;
}
