//Title: Coin Change Enumeration Problem
//Problem： Develop a program that uses recursion to enumerate the distinct ways in which the given amount of money in cents, can be charged into quarters, dimes, nickels and pennies.
//Example: Given 17
//Output: 
//1) 1 dime, 1 nickel, 2 penny
//2) 1 dime, 7 penny
//3) 3 nickel, 2 penny
//4) 2 nickel, 7 penny
//5) 1 nickel, 12 penny
//6) 17penny
//Restriction: Use constant amount of memory for local variables and method parameters, so lists, stacks or queues are not allowed!

#include <iostream>

enum Coins { penny = 1, nickel = 5, dime = 10, quarter = 25 };

struct picks
{
    int q_pick{};
    int d_pick{};
    int n_pick{};
    int p_pick{};
};

void printResult(const picks& pick)
{
    std::cout   << pick.q_pick << " quarters, "
                << pick.d_pick << " dimes, "
                << pick.n_pick << " nickels, "
                << pick.p_pick << " pennies\n";
}

void print_change(int change, picks pick, Coins last)
{
    if (change >= quarter && last == Coins::quarter)
    {
        for (auto i = change / Coins::quarter; i >= 0; --i)
        {
            picks temp = pick;
            temp.q_pick += i;
            print_change(change - i * Coins::quarter, temp, Coins::dime);
        }
    }
    else if (change >= dime && last == Coins::dime)
    {
        for (auto i = change / Coins::dime; i >= 0; --i)
        {
            picks temp = pick;
            temp.d_pick += i;
            print_change(change - i * Coins::dime, temp, Coins::nickel);
        }
    }
    else if (change >= nickel && last == Coins::nickel)
    {
        for (auto i = change / Coins::nickel; i >= 0; --i)
        {
            picks temp = pick;
            temp.n_pick += i;
            print_change(change - i * Coins::nickel, temp, Coins::penny);
        }
    }
    else 
    {
        pick.p_pick = change;
        printResult(pick);
    }
}

int main()
{
    std::cout << "Enter an amount: ";
    int amount{};
    std::cin >> amount;
    print_change(amount, {}, amount >= Coins::quarter ? Coins::quarter : (amount >= Coins::dime ? Coins::dime : (amount >= Coins::nickel ? Coins::nickel : Coins::penny)));
}