#include <iostream>
#include <ctime>
using namespace std;

string NowToString(){
  time_t str = time(nullptr);
  return ctime(&str);
}