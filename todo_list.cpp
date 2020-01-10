#include <iostream>
#include <string>
using namespace std;

class date{};
    int day;
    int month;
    int year;

class task{
    int id;
    string title;
    string description;
    date creation;
    date cloture;
    string status;
    int progress;
    string priority;
    string comms;
    taskset todo;
};

class taskset{
    friend class task;
    int amount;
};