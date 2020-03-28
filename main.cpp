#include "todo_list.h"

int main(){
    TaskManager TM("To_Do_List.txt");
    
    while(true){
        int choice = TM.MainMenu();
        TM.RedirectFromMainMenu(choice);
        if(choice == 5){
            break;
        }
    }
    return 0;
};