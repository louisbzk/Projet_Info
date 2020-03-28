#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <fstream>
using namespace std;

class Foo{
    friend class foo_fighter;
    public:

    int value;
    string data;

    Foo(int v, string d) : value(v), data(d){}
    ~Foo(){}

    void print(){
        cout << this->data;
        cout << this->value;
    }

};

class foo_fighter{

    public:
    int value;
    foo_fighter(){}

    Foo create(){
        Foo f = Foo(5, "hello world");
        return f;
    }
};

int main(){
    foo_fighter f;
    Foo fo = f.create();
    fo.print();

    return 0;
};