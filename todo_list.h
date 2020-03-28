#ifndef TODO_LIST_H
#define TODO_LIST_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <fstream>
#include "extract_int.h"
#include "stringdate.h"

using namespace std;

class task{
    int id;
    string title;
    string description;
    char* d_creation;
    char* d_cloture{"Tâche non terminée"};
    string status;
    int progress;
    int priority;
    string comms;
    vector<task> subtasks;

    task(int n, string t, string desc, char* crea, char* clot, string stat, int prog, int prio, string comm, vector<task> sub) : id(n), title(t), description(desc), d_creation(crea), d_cloture(clot), status(stat), progress(prog), priority(prio), comms(comm), subtasks(sub){}

    ~task();

    void print();
    void printdatecr();
    void printdatecl();
    void getStandardInfo();
    void getSubtasksToDo();
    
    bool operator==(const task& t) const;
    bool operator!=(const task& t) const;
};

class TaskManager{
    vector<task> todolist;
    int idGen;

    public:

    TaskManager(){}
    TaskManager(string filename){}

    ~TaskManager();

    int MainMenu();
    void RedirectFromMainMenu(int choice);

    bool IdentifyTask(int id, task t);
    void view(int id);
    void viewall();

    task create();

    task Subtask();
    task CreateSubtask();
    vector<task> addsub();

    void removetask(int id);
    void edit(int id);

    void extraction(string const& str, const char delim, vector<string>& out);

    void write_task(task t, string filename);
    task extract_task(string line);
    vector<task> extract_subtasks(string str);
    void save();
};

#endif
