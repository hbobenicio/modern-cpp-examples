#include <iostream>
#include <chrono>

static void doSomeWork()
{
  std::cout << "Doing some work for measuring." << '\n';
}

static void measuringTime()
{
  using namespace std::chrono;
  using std::cout;

  // deduced type = steady_clock::time_point
  auto start = steady_clock::now();
  doSomeWork();
  auto end = steady_clock::now();

  auto us = duration_cast<microseconds>(end - start).count();
  cout << "doSomeWork() run for " << us << " microseconds\n";
}

int main()
{
  measuringTime();
}

