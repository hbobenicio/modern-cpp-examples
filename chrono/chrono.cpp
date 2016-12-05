#include <iostream>
#include <chrono>
#include <ctime>

static void doSomeWork()
{
  std::cout << "Doing some work for measuring." << '\n';
  for (int i = 0; i < 10000; i++) {
    for (int j = 0; j < 100000; j++) {
      i--;
      i++;
    }
  }
}

static void measuringTime()
{
  using namespace std::chrono;
  using std::cout;

  // deduced type = steady_clock::time_point
  auto start = steady_clock::now();
  doSomeWork();
  auto end = steady_clock::now();

  auto us = duration_cast<seconds>(end - start).count();
  cout << "doSomeWork() run for " << us << " seconds\n";
}

int main()
{
  using std::cout;

  measuringTime();

  // You can use chrono literals to create duration objects
  using namespace std::literals::chrono_literals;
  auto day = 24h; // deduced type = std::chrono::hours
  auto halfHour = 0.5h;
  std::cout << "one day is " << day.count() << " hours\n"
            << "half an hour is " << halfHour.count() << " hours\n"; 
}

