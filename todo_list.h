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


class task{
    
    public:

    friend class TaskManager;

    int id;
    std::string title;
    std::string description;
    std::string d_creation;
    std::string d_cloture{"Tâche non terminée"};
    std::string status;
    int progress;
    int priority;
    std::string comms;
    std::vector<task*> subtasks;


    task(int n, std::string t, std::string desc, std::string crea, std::string clot, std::string stat, int prog, int prio, std::string comm, std::vector<task*> sub);
    task(const task& t2);

    ~task();

    void print();
    void printdatecr();
    void printdatecl();
    void getStandardInfo();
    void getSubtasksToDo();
    
    bool operator==(const task& t) const;
    bool operator!=(const task& t) const;
    bool operator<(const task& t) const;
};

class TaskManager{

    public:

    std::vector<task*> todolist{std::vector<task*>()};
    int idGen{0};

    const char task_delim{'%'};
    const char attribute_delim{'|'};
    const char sub_delim{'$'};
    const char sub_attr_delim{'{'};


    TaskManager();
    TaskManager(std::string filename);

    ~TaskManager();

    int MainMenu();
    void RedirectFromMainMenu(int choice);

    // bool TaskComparison(task* t1, task* t2);
    bool IdentifyTask(int id, task t);
    void view(int id);
    void viewall();

    task* create();

    task* Subtask();
    task* CreateSubtask();
    std::vector<task*> addsub();

    task* GetById(int id);
    void removetask(int id);
    void edit(int id);

    void extraction(std::string const& str, const char delim, std::vector<std::string>& out);

    void write_task(task* t, std::string filename);
    task* extract_task(std::string line);
    task* extract_subtask(std::string line);
    std::vector<task*> extract_subtasks(std::string str);
    void save();
};

#endif
