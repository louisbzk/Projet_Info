#include <iostream>
#include <ctime>

std::string NowToString(){
  std::time_t str = time(nullptr);
  std::string current_time = ctime(&str);
  current_time.pop_back();
  return current_time;
}