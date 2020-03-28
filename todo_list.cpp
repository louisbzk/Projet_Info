#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include "extract_int.h"
#include "stringdate.h"

using namespace std;

class task{
    friend class TaskManager;

    public:

    int id;
    string title;
    string description;
    string d_creation;
    string d_cloture{"Tâche non terminée"};
    string status;
    int progress;
    int priority;
    string comms;
    vector<task> subtasks;

    task(int n, string t, string desc, string crea, string clot, string stat, int prog, int prio, string comm, vector<task> sub = vector<task>()) : id(n), title(t), description(desc), d_creation(crea), d_cloture(clot), status(stat), progress(prog), priority(prio), comms(comm), subtasks(sub){}

    ~task(){}

    void print(){
        cout << this->id << this->title << this->status << "\n";
    }

    void printdatecr(){
        cout << "Créée le :" << this->d_creation << "\n";
    }

    void printdatecl(){
        if(this->d_cloture == "Tâche non terminée"){
            cout << d_cloture;
        }
        else{
        cout << "Clôturée le :" << this->d_cloture << endl;
        }
    }

    void getStandardInfo(){
        cout << this->id << "\n";
        cout << this->title << "\n";
        cout << this->description << "\n";
        cout << this->comms << endl;
    }

    void getSubtasksToDo(){
        cout << "Tâches nécessaires :" << "\n";

        for(task t: this->subtasks){
            string noclot = "Tâche non terminée";
            if(t.d_cloture == noclot){
                t.getStandardInfo();
            }
            cout << '\n';
        };
        cout << flush;
    }

    bool operator==(const task& t) const {
        return this->id == t.id;
    }

    bool operator!=(const task& t) const {
        return this->id != t.id;
    }
};

class TaskManager{

    vector<task> todolist{vector<task>()};
    int idGen{0};

    const char task_delim{'%'};
    const char attribute_delim{'|'};

    const char sub_delim{'$'};
    const char sub_attr_delim{'{'};

    public:

    TaskManager(){}
    TaskManager(string filename){
        fstream f(filename);
        string line;
        while(getline(f, line, task_delim)){
            task t = extract_task(line);
            todolist.push_back(t);
            idGen += t.id;
        }
    }

    ~TaskManager(){}



    int MainMenu(){

        /* recueille l'input de l'utilisateur sous forme d'int puis adapte son comportement en fonction de l'input */

        cout << "Menu principal : entrez un nombre pour gérer la To Do List" << "\n";

        cout << "1 : Visualiser une ou plusieurs tâches" << "\n";
        cout << "2 : Ajouter des tâches" << "\n";
        cout << "3 : Supprimer des tâches" << "\n";
        cout << "4 : Modifier une tâche" << "\n";
        cout << "5 : Sauvegarder et quitter" << "\n";

        int choice = getnumber();
        cin.ignore(INT8_MAX, '\n');

        return choice;            
    }

    void RedirectFromMainMenu(int choice){

        /* appelle les fonctions qui correspondent au comportement voulu */

        int id;
        const string a = "ALL";

        switch(choice){
            case 1:
                {
                string in;
                cout << "Veuillez entrer les identifiants des tâches que vous souhaitez visualiser, séparés par un espace. Pour tout visualiser, tapez ALL" << endl;
                getline(cin, in);

                if(in != a){
                    while(id != -1){
                        id = extractInt(in);
                        removeInt(in, id);
                        view(id);
                    }
                }

                else{
                    viewall();
                }

                }

                break;
            
            case 2:
                {
                task t = create();
                this->todolist.push_back(t);
                break;
                }

            case 3:
                {

                /* Ici, on vérifie que l'utilisateur ne s'est pas trompé dans sa saisie avant de supprimer les tâches */

                bool made_a_mistake = true;
                char sure;
                string in;

                while(made_a_mistake){

                    cout << "Veuillez entrer les identifiants des tâches que vous souhaitez supprimer, séparés par un espace. \n";
                    getline(cin, in);

                    cout << "êtes vous sûr(e) de votre saisie ? [Y/N] ";
                    cin >> sure;
                    cin.ignore(INT8_MAX, '\n');

                    switch(sure){
                        case 'y':
                            made_a_mistake = false;
                            break;

                        case 'n':
                            cout << "OK, on recommence ! \n";
                            break;

                        case 'Y':
                            made_a_mistake = false;
                            break;

                        case 'N':
                            cout << "OK, on recommence ! \n";
                            break;

                        default:
                            cout << "Entrée invalide, veuillez entrer n, N, y ou Y ! Désolé, je considère que c'est non... \n";
                            break;
                    }
                }

                cout << endl;

                while(id != -1){
                        id = extractInt(in);
                        removeInt(in, id);
                        removetask(id);                    
                }
               
                break;
                }

            case 4:
                {
                string in;
                cout << "Veuillez entrer l'identifiant de la tâche à modifier" << endl;
                cin >> in;
                cin.ignore(INT8_MAX, '\n');

                id = extractInt(in);
                edit(id);
                
                break;
                }

            case 5:
                save();
                break;

            default:
                cout << "Entrée invalide !" << endl;
                break;
        }
    }

    bool IdentifyTask(int id, task t){
            return t.id == id;
    }

    void view(int id){
        for(task t: this->todolist){
            if(IdentifyTask(id, t)){
                cout << t.id << " : " << t.title << endl;
                cout << "Description : " << t.description << "\n" << "\n" << "Commentaires : " << t.comms << endl;
                t.printdatecr();
                cout << "Statut : " << t.status << ", Priorité : " << t.priority << endl;
                for(task sub: t.subtasks){
                    cout << sub.id << " : " << sub.title << endl;
                    cout << sub.status << endl;
                }
                t.printdatecl();
            }
        }
    }

    void viewall(){
        for(task t: this->todolist){
            cout << t.id << " : " << t.title << "\n";
            cout << "Description : " << t.description << "\n" << "\n" << "Commentaires : " << t.comms << "\n";
            t.printdatecr();
            cout << "Statut : " << t.status << ", avancement : " << t.progress << ", priorité : " << t.priority << "\n";

            cout << "Tâches préalables nécessaires : " << "\n";
            t.getSubtasksToDo();
            t.printdatecl();
        }
    }

    task create(){

        /* Crée une tâche à partir d'inputs utilisateur. */

        int id = idGen;
        idGen++;

        string titl;
        string descr;
        int prog;
        int prio;
        string comm;
        char addsubtasks;
        vector<task> sub;

        cout << "Saisissez l'intitulé de la tâche :";
        getline(cin, titl);

        cout << "\n Description de la tâche : ";
        getline(cin, descr);

        cout << "\n Avancement actuel (pourcentage de 0 à 100, sans le caractère %) : ";
        cin >> prog;
        cin.ignore(INT8_MAX, '\n');

        cout << "\n Priorité (int) : ";
        cin >> prio;
        cin.ignore(INT8_MAX, '\n');

        cout << "\n Commentaires : ";
        getline(cin, comm);
        cout << "\n Ajouter des sous-tâches ? [Y/N]";
        cin >> addsubtasks;
        cin.ignore(INT8_MAX, '\n');

        switch(addsubtasks){
            case 'y':
                sub = addsub();
                break;
            
            case 'Y':
                sub = addsub();
                break;
            
            case 'n':
                break;
            
            case 'N':
                break;
            
            default:
                cout << "I'll take this as a no..." << endl;
        }

        
        string d_crea = NowToString();
        string d_clot = "Tâche non terminée";
        string sta = "En cours";

        task newtask = task(id, titl, descr, d_crea, d_clot, sta, prog, prio, comm, sub);
        (this->todolist).push_back(newtask);
        
    }

    vector<task> addsub(){

        /* Fonction renvoyant le vecteur des sous-tâches désirées pour une tâche donnée. */

        vector<task> subtasks;
        char keeprunning = 'y';

        while(keeprunning == 'y' || keeprunning == 'Y'){        
            subtasks.push_back(Subtask());
            this->todolist.push_back(Subtask());

            cout << "Ajouter une autre tâche ? [Y/N]";
            cin >> keeprunning;
            cin.ignore(INT8_MAX, '\n');
        }

        cout << "Sous-tâches enregistrées" << endl;
        return subtasks;
    }

    task Subtask(){

        /* Fonction renvoyant une sous-tâche à ajouter au vecteur des sous-tâches d'une tâche donnée. Cette
        fonction ne crée rien en soi, puisqu'elle choisit si elle appelle la fonction qui crée une sous-tâche
        CreateSubtask() ou si on ajoute au vecteur des sous-tâche une tâche qui existe déjà. */

        char addorcreate;
        cout << "[C]réer une nouvelle tâche ou [A]jouter une tâche existante comme sous-tâche ? [C/A]" << "\n";
        cin >> addorcreate;
        cin.ignore(INT8_MAX, '\n');

        if(addorcreate == 'C' || addorcreate == 'c'){
            task sub = CreateSubtask();
            return sub;
        }

        else if(addorcreate == 'A' || addorcreate == 'a'){
            string _id;
            viewall();
            cout << "Entrez l'identifiant de la tâche correspondante. Tâches rappelées ci-dessus." << "\n";
            cin >> _id;
            cin.ignore(INT8_MAX, '\n');

            int id = stoi(_id);
            for(task t: this->todolist){
                if(IdentifyTask(id, t)){
                    return t;
                }
                cout << "Identifiant non reconnu" << "\n";
            }
        }
        cout << flush;
    }

    task CreateSubtask(){

        /* Fonction créant une sous-tâche. */

        int id = idGen;
        idGen++;

        string titl;
        string descr;
        int prog;
        int prio;
        string comm;

        cout << "Saisissez l'intitulé de la tâche :";
        cin.ignore();
        getline(cin, titl);

        cout << "\n Description de la tâche : ";
        cin.ignore();
        getline(cin, descr);

        cout << "\n Avancement actuel (pourcentage de 0 à 100, sans le caractère %) : ";
        cin >> prog;
        cin.ignore(INT8_MAX, '\n');

        cout << "\n Priorité (entier) : ";
        cin >> prio;
        cin.ignore(INT8_MAX, '\n');

        cout << "\n Commentaires : ";
        getline(cin, comm);
       
        string d_crea = NowToString();
        string d_clot = "Tâche non terminée";
        string sta = "En cours";

        task newtask = task(id, titl, descr, d_crea, d_clot, sta, prog, prio, comm);
        return newtask;
    }

    void removetask(int id){
        /* Supprime une tâche à partir de l'id saisi. Si l'id est invalide (aucune tâche n'a cet id), la fonction
        ne fait rien. Si c'est le cas, elle n'affiche rien dans le terminal.

        Pour cette fonction, je n'ai pas trouvé d'autre solution que d'utiliser cette syntaxe "old school" 
        plutôt que for(task t : this->todolist){...} puisque pour appeler vector::erase() j'ai besoin d'un 
        objet de type vector::iterator */

        for(vector<task>::iterator it = this->todolist.begin(); it != this->todolist.end(); ++it){
            if((*it).id == id){
                (this->todolist).erase(it);
                cout << "Tâche" << id << "effacée avec succès";
            }
        }
    }

    void edit(int id){
        /* permet de modifier les données d'une tâche : titre, description, et d'ajouter un nouveau commentaire.

        Je n'ai pas réussi à trouver une solution pratique pour modifier en place le commentaire (donc faire
        quelque chose qui ressemble à nano To_Do_List.txt (avec le pointeur au bon endroit) pour que l'utilisateur
        change le commentaire). De plus, j'ai estimé qu'il était assez risqué d'ouvrir le fichier de sauvegarde
        durant l'utilisation, parce que l'utilisateur peut potentiellement effacer un délimiteur par exemple */

        for(task t: this->todolist){
            if(IdentifyTask(id, t)){
                char chgt;
                cout << "Changer le titre ? [Y/N]" << "\n";
                cin >> chgt;
                cin.ignore(INT8_MAX, '\n');

                if(chgt == 'y' || chgt == 'Y'){
                    string ntitle;
                    cout << "Entrez le nouveau titre" << "\n";
                    getline(cin, ntitle);
                    t.title = ntitle;
                }

                else if(chgt != 'n' || chgt != 'N'){
                    cout << "I'll take this as a no..." << "\n";
                }

                cout << "Changer la description ? [Y/N]" << "\n";
                cin >> chgt;
                cin.ignore(INT8_MAX, '\n');

                if(chgt == 'y' || chgt == 'Y'){
                    string ndesc;
                    cout << "Entrez la nouvelle description" << "\n";
                    getline(cin, ndesc);
                    t.description = ndesc;
                }

                cout << "Ajouter un commentaire ? [Y/N]" << "\n";
                cin >> chgt;
                cin.ignore(INT8_MAX, '\n');

                if(chgt == 'y' || chgt == 'Y'){
                    string ncomm;
                    cout << "Entrez le commentaire" << "\n";
                    getline(cin, ncomm);
                    t.comms += "\n";
                    t.comms += ncomm;                    
                }
                
                else if(chgt != 'n' || chgt != 'N'){
                    cout << "I'll take this as a no..." << "\n";
                }
            }
        }
        cout << flush;
    }

    void extraction(string const& str, const char delim, vector<string>& out){
        size_t start;
        size_t end = 0;

        while ((start = str.find_first_not_of(delim, end)) != string::npos){
            end = str.find(delim, start);
            out.push_back(str.substr(start, end - start));
        }
    }

    vector<task> extract_subtasks(string str){

        /* Extrait des sous-tâches associées à une tâche à partir du fichier (d'où l'argument string)
        S'il n'y a pas de sous-tâches associées, la string décrivant les sous-tâches sera "NONE" */

        vector<task> _subtasks = vector<task>();
        string nosubs = "NONE";
        if(str == nosubs){
            return _subtasks;
        }

        else{
            vector<string> _tasks;
            extraction(str, sub_delim, _tasks); // chaque élément de _tasks est une task sous forme de string

            for(string _task : _tasks){
                task t = extract_task(_task);
                _subtasks.push_back(t);
            }

            return _subtasks;
        }
    }


    task extract_task(string line){

        /* Extrait une tâche à partir du fichier. Le délimiteur entre attributs de tâche est le caractère | */

        vector<string> out;
        extraction(line, attribute_delim, out); // chaque élément de out est un attribut de task sous forme de string

        int _id = stoi(out[0]);
        string _title = out[1];
        string _description = out[2];
        string _d_creation = out[3];
        string _d_cloture = out[4];
        string _status = out[5];
        int _progress = stoi(out[6]);
        int _priority = stoi(out[7]);
        string _comms = out[8];
        vector<task> _subtasks = extract_subtasks(out[9]);

        task t = task(_id, _title, _description, _d_creation, _d_cloture, _status, _progress, _priority, _comms, _subtasks);

        return t;
    }


    void write_task(task t, string filename = "To_Do_List.txt"){

        /* écrit une tâche sous forme d'une string dans le fichier (ie sauvegarde d'une tâche)

        Cette fonction est la plus "délicate" puisqu'elle fait appel aux délimiteurs, donc si ceux-ci ont été saisis
        par l'utilisateur dans un titre par exemple, le programme plante. A l'heure où j'écris ceci je n'ai
        pas encore écrit de fonction (ou autre code) permettant d'empêcher l'utilisateur de saisir ces caractères. */

        fstream f;
        f.open(filename, ios::app);

        int id = t.id;
        string title = t.title;
        string description = t.description;
        string d_creation = t.d_creation;
        string d_cloture = t.d_cloture;
        string status = t.status;
        int progress = t.progress;
        int priority = t.priority;
        string comms = t.comms;
        vector<task> subtasks = t.subtasks;
        
        f << id << attribute_delim << title << attribute_delim << d_creation << attribute_delim 
        << d_cloture << attribute_delim << status << attribute_delim << progress 
        << attribute_delim << priority << attribute_delim;
        
        if(subtasks == vector<task>()){
            f << "NONE";
        }
        else{

            for(task s : subtasks){
                
                int sid = s.id;
                string stitle = s.title;
                string sdescription = s.description;
                string sd_creation = s.d_creation;
                string sd_cloture = s.d_cloture;
                string sstatus = s.status;
                int sprogress = s.progress;
                int spriority = s.priority;
                string scomms = s.comms;

                f << sid << sub_attr_delim << stitle << sub_attr_delim << sdescription << sub_attr_delim << sd_creation 
                << sub_attr_delim << sd_cloture << sub_attr_delim << sstatus << sub_attr_delim 
                << sprogress << sub_attr_delim << spriority << sub_attr_delim << scomms;

                if (s != subtasks.back()){
                    f << sub_delim;
                }
            }
        }
        f << task_delim;
        f.close();
    }

    void save(){
        for(task t: this->todolist){
            write_task(t);
        }
        cout << "Tâches enregistrées avec succès" << endl;
    }

};
