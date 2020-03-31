#include <iostream>
#include <sstream>

int extractInt(std::string str){

    /* extrait le premier entier trouvé dans une string donnée */

    std::stringstream ss;
    ss << str;

    std::string word;
    int number;
    while(!ss.eof()){
        ss >> word;

        if(std::stringstream(word) >> number){
            return number;
        }
    }

    return -1;
}

void removeInt(std::string str, int number){

    /* supprime l'entier donné dans une string donnée */

    std::string num = std::to_string(number);
    int pos1 = str.find(number);
    int pos2 = str.find(" ", pos1);
    str.erase(pos1, pos2-pos1);
}

/* les deux fonctions ci-dessus permettent donc d'extraire tous les entiers (différents) dans une string selon le
principe suivant : string input -> extraction du 1er int trouvé -> int supprimé -> retour à l'extraction */


/* ensuite, deux fonctions qui ici assurent d'éviter un undefined behavior lorsqu'un int est demandé en input, 
mais que l'utilisateur saisit une donnée qui ne contient pas QUE des digits. Tant qu'un int n'est pas saisi,
getnumber() continue de demander un input valide. */

bool check_number(std::string str){
    int l = str.length() - 1;
    int i = 0;
    while(isdigit(str[i])){
        if(i == l){
            return true;
        }
        else{
            i++;
        }
    }
    return false;
}

int getnumber(){
    std::string choice;
    std::cin >> choice;
    while(!check_number(choice)){
        std::cout << "Warning : your input cannot be interpreted as an integer ! \n" 
        << "Please enter an integer according to what you want to do : \n";
        std::cin >> choice;
    }
    std::cout << std::flush;
    int _choice = stoi(choice);
    return _choice;
}