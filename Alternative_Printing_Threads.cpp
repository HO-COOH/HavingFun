/*
Problem: Two threads alternatively printing numbers and the corresponding letters.
    One thread print numbers from 1->26, while another print letters from A->Z.
    The output should be: 1A2B3C...25Y26Z
*/

#include "Range/Range.hpp"  //SugarPP
#include "IO/IO.hpp"        //SugarPP
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

std::condition_variable canPrint;
std::atomic_bool letterPrinted{false};
std::atomic_bool numberPrinted{false};
std::mutex m;

int main()
{
    std::thread t1{
        [] {
            for (auto i : Range(1, 26 + 1))
            {
                std::unique_lock lk{m};
                letterPrinted = false;
                std::cerr << i;
                numberPrinted = true;
                canPrint.notify_one();
                canPrint.wait(lk, [] { return letterPrinted.load(); });
            }
        }};

    std::thread t2{
        [] {
            for (char i : Range('A', 'Z' + 1))
            {
                std::unique_lock lk{m};
                canPrint.wait(lk, [] { return numberPrinted.load(); });
                std::cerr << static_cast<char>(i);
                numberPrinted = false;
                letterPrinted = true;
                canPrint.notify_one();
            }
        }};

    t1.join();
    t2.join();
    std::cout << "\nFinish!\n";
}