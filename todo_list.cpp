#include "todo_list.h"

// FONCTIONS MEMBRES TASK 

    task::task(int n, std::string t, std::string desc, std::string crea, std::string clot, std::string stat, int prog, int prio, std::string comm, std::vector<task*> sub = std::vector<task*>()) : id(n), title(t), description(desc), d_creation(crea), d_cloture(clot), status(stat), progress(prog), priority(prio), comms(comm), subtasks(sub){}
    task::task(const task& t2){
        id = t2.id;
        title = t2.title;
        description = t2.description;
        d_creation = t2.d_creation;
        d_cloture = t2.d_cloture;
        status = t2.status;
        progress = t2.progress;
        priority = t2.priority;
        comms = t2.comms;
        subtasks = t2.subtasks;
    }

    task::~task(){}

    void task::print(){
        std::cout << this->id << this->title << this->status << '\n';
    }

    void task::printdatecr(){
        std::cout << "  Creee le : " << this->d_creation << '\n';
    }

    void task::printdatecl(){
        if(this->d_cloture == "Tache non terminee"){
            std::cout << "  " << d_cloture;
        }
        else{
        std::cout << "  Cloturee le : " << this->d_cloture << '\n';
        }
    }

    void task::getStandardInfo(){
        std::cout << this->id << '\n';
        std::cout << this->title << '\n';
        std::cout << this->description << '\n';
        std::cout << this->comms << std::endl;
    }

    void task::getSubtasksToDo(){;
        std::vector<task*> sub = this->subtasks;
        if(sub == std::vector<task*>()){
            std::cout << "Aucune \n";
        }
        else{
            for(task* t : sub){
                std::cout << (*t).id << " ";
            }
            std::cout << '\n';
        }

        std::cout << std::flush;
    }

    bool task::operator==(const task& t) const {
        return this->id == t.id;
    }

    bool task::operator!=(const task& t) const {
        return this->id != t.id;
    }



// FONCTIONS MEMBRES TASKMANAGER

    TaskManager::TaskManager(){}
    TaskManager::TaskManager(std::string filename){
        std::ifstream f(filename);
        std::string line;
        while(std::getline(f, line, task_delim)){
            task* t = extract_task(line);
            this->todolist.push_back(t);
            this->idGen += 1;
        }

        f.close();
        
        /* clearing the file, so that the same tasks are not written multiple times in it when save()
        (and therefore write_task() ) is called */

        std::ofstream ofs;
        ofs.open("To_Do_List.txt", std::ofstream::out | std::ofstream::trunc);
        ofs.close();
    }

    TaskManager::~TaskManager(){}


    int TaskManager::MainMenu(){

        /* recueille l'input de l'utilisateur sous forme d'int puis adapte son comportement en fonction de l'input */

        std::cout << "\nMenu principal : entrez un nombre pour gerer la To Do List" << "\n \n";

        std::cout << "1 : Visualiser une ou plusieurs taches" << '\n';
        std::cout << "2 : Ajouter une tache" << '\n';
        std::cout << "3 : Supprimer une tache" << '\n';
        std::cout << "4 : Modifier/cloturer une tache" << '\n';
        std::cout << "5 : Sauvegarder et quitter" << '\n';

        int choice = getnumber();
        std::cin.ignore();

        return choice;            
    }

    void TaskManager::RedirectFromMainMenu(int choice){

        /* appelle les fonctions qui correspondent au comportement voulu */

        const std::string a = "ALL";

        switch(choice){
            case 1:
                {
                int id;
                std::string in;
                std::cout << "Veuillez entrer les identifiants des taches que vous souhaitez visualiser, separes par un espace. Pour tout visualiser, tapez ALL" << std::endl;
                std::getline(std::cin, in);

                if(in != a){
                    while(id != -1){
                        id = extractInt(in);
                        removeInt(in, id);
                        TaskManager::view(id);
                    }
                }

                else{
                    TaskManager::viewall();
                }

                }

                break;
            
            case 2:
                {
                task* t = TaskManager::create();
                this->todolist.push_back(t);
                break;
                }

            case 3:
                {

                /* Ici, on verifie que l'utilisateur ne s'est pas trompe dans sa saisie avant de supprimer les taches */
                int id;
                bool made_a_mistake = true;
                char sure;
                std::string in;

                while(made_a_mistake){

                    std::cout << "Veuillez entrer l'identifiant de la tache que vous souhaitez supprimer. \n";
                    std::cin>>id;
                    std::cin.clear();
                    std::cin.ignore(INT8_MAX, '\n');

                    std::cout << "êtes vous sûr(e) de votre saisie ? [Y/N] ";
                    std::cin >> sure;
                    std::cin.clear();
                    std::cin.ignore(INT8_MAX, '\n');

                    switch(sure){
                        case 'y':
                            made_a_mistake = false;
                            break;

                        case 'n':
                            std::cout << "OK, on recommence ! \n";
                            break;

                        case 'Y':
                            made_a_mistake = false;
                            break;

                        case 'N':
                            std::cout << "OK, on recommence ! \n";
                            break;

                        default:
                            std::cout << "Entree invalide, veuillez entrer n, N, y ou Y ! Desole, je considere que c'est non... \n";
                            break;
                    }
                }

                removetask(id);
               
                break;
                }

            case 4:
                {
                int id;
                std::string in;
                std::cout << "Veuillez entrer l'identifiant de la tache a modifier" << std::endl;
                std::cin >> in;
                std::cin.clear();
                std::cin.ignore(INT8_MAX, '\n');

                id = extractInt(in);
                TaskManager::edit(id);
                
                break;
                }

            case 5:
                TaskManager::save();
                break;

            default:
                std::cout << "Entree invalide !" << std::endl;
                break;
        }
    }

    bool TaskManager::IdentifyTask(int id, task t){
            return t.id == id;
    }


    void TaskManager::view(int id){
        for(task* t: this->todolist){
            if(TaskManager::IdentifyTask(id, *t)){
                std::cout << t->id << " : " << t->title << std::endl;
                std::cout << "Description : " << t->description << "\n \n" << "Commentaires : " << t->comms << '\n';
                (*t).printdatecr();
                std::cout << "Statut : " << t->status << ", Priorite : " << t->priority << '\n';
                for(task* sub: t->subtasks){
                    std::cout << sub->id << " : " << sub->title << '\n';
                    std::cout << sub->status << '\n';
                }
                (*t).printdatecl();
            }
        }
    }

    void TaskManager::viewall(){

        /* Voir toutes les taches, par ordre de priorite decroissant
        Attention : les taches completees apparaîtront aussi */

        // sort by priority (highest to lowest). le tri ne se fait PAS en place (sinon risque de delete une subtask avant une tache lors de l'appel de save())

        int todolist_size = this->todolist.size();
        std::vector<task*> sorted_todo(todolist_size);
        std::partial_sort_copy(begin(todolist), end(todolist), begin(sorted_todo), end(sorted_todo), [](task* t1, task* t2)
        {return  (*t1).priority >= (*t2).priority; });
        
        /* 
        t1 has more priority than t2 <=> t1 < t2 where < compares "non-priority" (need this if we want to sort by descending priority)
        
        J'aurais voulu definir un operateur sous la forme task::operator<(const task& t){...} mais je pense que cela aurait
        donne un undefined behavior lorsque j'appelle std::sort avec un vecteur de task*, et non de task (ou simplement
        une erreur de type d'argument)
         */

        for(task* t: sorted_todo){
            std::cout << t->id << " : " << t->title << '\n';
            std::cout << "  Description : \n    " << t->description << '\n' << '\n' << "  Commentaires : \n    " << t->comms << "\n \n";
            (*t).printdatecr();
            std::cout << "  Statut : " << t->status << ", avancement : " << t->progress << ", priorite : " << t->priority << '\n';

            std::cout << "  Taches prealables necessaires : ";
            (*t).getSubtasksToDo();
            (*t).printdatecl();
            std::cout << "\n \n";
        }
    }

    task* TaskManager::create(){

        /* Cree une tache a partir d'inputs utilisateur. */

        int id = idGen;
        idGen++;

        std::string titl;
        std::string descr;
        int prog;
        int prio;
        std::string comm;
        char addsubtasks;
        std::vector<task*> sub;

        std::cout << "Saisissez l'intitule de la tache : ";
        std::getline(std::cin, titl);


        std::cout << "\n Description de la tache : ";
        std::getline(std::cin, descr);

        std::cout << "\n Avancement actuel (pourcentage de 0 a 100, sans le caractere %) : ";
        std::cin >> prog;
        std::cin.clear();
        std::cin.ignore(INT8_MAX, '\n');

        std::cout << "\n Priorite (int) : ";
        std::cin >> prio;
        std::cin.clear();
        std::cin.ignore(INT8_MAX, '\n');

        std::cout << "\n Commentaires : ";
        std::getline(std::cin, comm);
        std::cout << "\n Ajouter des sous-taches ? [Y/N] ";


        std::cin >> addsubtasks;
        std::cin.clear();
        std::cin.ignore(INT8_MAX, '\n');


        switch(addsubtasks){
            case 'y':
                sub = TaskManager::addsub();
                break;
            
            case 'Y':
                sub = TaskManager::addsub();
                break;
            
            case 'n':
                sub = std::vector<task*>();
                break;
            
            case 'N':
                sub = std::vector<task*>();
                break;
            
            default:
                sub = std::vector<task*>();
                std::cout << "I'll take this as a no..." << '\n';

        std::cout << std::flush;
        }

        
        std::string d_crea = NowToString();
        std::string d_clot = "Tache non terminee";
        std::string sta = "En cours";

        task* newtask = new task(id, titl, descr, d_crea, d_clot, sta, prog, prio, comm, sub);
        for(task* sub : newtask->subtasks){
            (this->todolist).push_back(sub);
        }
        return newtask;
        
    }

    std::vector<task*> TaskManager::addsub(){

        /* Fonction renvoyant le vecteur des sous-taches desirees pour une tache donnee. */

        std::vector<task*> subtasks;
        char keeprunning = 'y';

        while(keeprunning == 'y' || keeprunning == 'Y'){
            task* sub = TaskManager::Subtask();        
            subtasks.push_back(sub);

            std::cout << "Ajouter une autre tache ? [Y/N] ";
            std::cin >> keeprunning;
            std::cin.clear();
            std::cin.ignore(INT8_MAX, '\n');
        }

        std::cout << "Sous-taches enregistrees" << std::endl;
        return subtasks;
    }

    task* TaskManager::Subtask(){

        /* Fonction renvoyant une sous-tache a ajouter au vecteur des sous-taches d'une tache donnee. Cette
        fonction ne cree rien en soi, puisqu'elle choisit si elle appelle la fonction qui cree une sous-tache
        CreateSubtask() ou si on ajoute au vecteur des sous-tache une tache qui existe deja. */

        char addorcreate;
        std::cout << "[C]reer une nouvelle tache ou [A]jouter une tache existante comme sous-tache ? [C/A]" << '\n';
        std::cin >> addorcreate;
        std::cin.clear();
        std::cin.ignore(INT8_MAX, '\n');

        if(addorcreate == 'C' || addorcreate == 'c'){
            task* sub = TaskManager::CreateSubtask();
            return sub;
        }

        else if(addorcreate == 'A' || addorcreate == 'a'){
            std::string _id;
            TaskManager::viewall();
            std::cout << "Entrez l'identifiant de la tache correspondante. Taches rappelees ci-dessus. ";
            std::cin >> _id;
            std::cin.clear();
            std::cin.ignore(INT8_MAX, '\n');

            int id = std::stoi(_id);
            for(task* t: this->todolist){
                if(TaskManager::IdentifyTask(id, *t)){
                    std::cout << std::flush;
                    return t;
                }
            }
            std::cout << "Identifiant non reconnu" << '\n';
        }
        std::cout << std::flush;
    }

    task* TaskManager::CreateSubtask(){

        /* Fonction creant une sous-tache. */

        int id = idGen;
        idGen++;

        std::string titl;
        std::string descr;
        int prog;
        int prio;
        std::string comm;

        std::cout << "Saisissez l'intitule de la tache : ";
        std::getline(std::cin, titl);

        std::cout << "\n Description de la tache : ";
        std::getline(std::cin, descr);

        std::cout << "\n Avancement actuel (pourcentage de 0 a 100, sans le caractere %) : ";
        std::cin >> prog;
        std::cin.clear();
        std::cin.ignore(INT8_MAX, '\n');

        std::cout << "\n Priorite (entier) : ";
        std::cin >> prio;
        std::cin.clear();
        std::cin.ignore(INT8_MAX, '\n');

        std::cout << "\n Commentaires : ";
        std::getline(std::cin, comm);
       
        std::string d_crea = NowToString();
        std::string d_clot = "Tache non terminee";
        std::string sta = "En cours";

        std::vector<task*> subsub = std::vector<task*>();

        task* newtask = new task(id, titl, descr, d_crea, d_clot, sta, prog, prio, comm, subsub);

        return newtask;
    }

    task* TaskManager::GetById(int id){
        for(task* t : this->todolist){
            if(IdentifyTask(id, (*t) )){
                return t;
            }
        }
    }

    void TaskManager::removetask(int id){
        /* Supprime une tache a partir de l'id saisi. Si l'id est invalide (aucune tache n'a cet id), la fonction
        ne fait rien. Si c'est le cas, elle n'affiche rien dans le terminal.

        Pour cette fonction, je n'ai pas trouve d'autre solution que d'utiliser cette syntaxe "old school" 
        plutot que for(task* t : this->todolist){...} puisque pour appeler vector::erase() j'ai besoin d'un 
        objet de type vector::iterator.

        J'itere sur chaque task* de la todolist puis sur chaque subtasks de chaque task*, pour être sur que
        to_remove n'est plus present en tant que subtask lorsque je la delete.
        
        Je pense que cette fonction est sans doute la plus moche... mais la presence de
        sous-taches m'a vraiment complique la "tache" ! comment l'ameliorer ? */

        task* to_remove = GetById(id);
        std::vector<task*>::iterator to_remove_pos;

        for(std::vector<task*>::iterator position = (this->todolist).begin(); position != (this->todolist).end(); ++position){
            std::vector<task*> subtasks = (*position)->subtasks;

            if ((*position) == to_remove)
            {
                to_remove_pos = position;
            }
            
            // enlever to_remove des sous-taches des autres taches

            for(std::vector<task*>::iterator it = subtasks.begin(); it != subtasks.end(); ++it){

                if(to_remove == (*it)){
                    ( (*position)->subtasks ).erase(it);
                    break;
                }
            }
        }
        (this->todolist).erase(to_remove_pos);
        delete to_remove;
    }

    void TaskManager::edit(int id){
        /* permet de modifier les donnees d'une tache : titre, description, et d'ajouter un nouveau commentaire.

        Je n'ai pas reussi a trouver une solution pratique pour modifier en place le commentaire (donc faire
        quelque chose qui ressemble a nano To_Do_List.txt (avec le pointeur au bon endroit) pour que l'utilisateur
        change le commentaire directement dans le terminal). De plus, j'ai estime qu'il etait assez risque d'ouvrir le fichier de sauvegarde
        durant l'utilisation, parce que l'utilisateur peut potentiellement effacer un delimiteur par exemple */

        for(task* t: this->todolist){
            if(TaskManager::IdentifyTask(id, *t)){
                char chgt;
                std::cout << "Voulez-vous cloturer (i.e. ajouter la date actuelle comme date de cloture de la tache) ? Attention, cette tache ne sera pas supprimee. [Y/N] ";
                std::cin >> chgt;
                std::cin.clear();
                std::cin.ignore(INT8_MAX, '\n');
                if(chgt == 'y' || chgt == 'Y'){
                    std::string d_clo = NowToString();
                    t->d_cloture = d_clo;
                    t->status = "Terminee";
                    t->progress = 100;
                    t->title = "CLOTUREE : " + t->title; 
                    break;                   
                }
                else if(chgt != 'n' || chgt != 'N'){
                    std::cout << "I'll take this as a no..." << '\n';
                }

                std::cout << "Changer le titre ? [Y/N] ";
                std::cin >> chgt;
                std::cin.clear();
                std::cin.ignore(INT8_MAX, '\n');

                if(chgt == 'y' || chgt == 'Y'){
                    std::string ntitle;
                    std::cout << "Entrez le nouveau titre" << '\n';
                    std::getline(std::cin, ntitle);
                    t->title = ntitle;
                }

                else if(chgt != 'n' || chgt != 'N'){
                    std::cout << "I'll take this as a no..." << '\n';
                }

                std::cout << "Changer la description ? [Y/N] ";
                std::cin >> chgt;
                std::cin.clear();
                std::cin.ignore(INT8_MAX, '\n');

                if(chgt == 'y' || chgt == 'Y'){
                    std::string ndesc;
                    std::cout << "Entrez la nouvelle description" << '\n';
                    std::getline(std::cin, ndesc);
                    t->description = ndesc;
                }

                std::cout << "Ajouter un commentaire ? [Y/N] ";
                std::cin >> chgt;
                std::cin.clear();
                std::cin.ignore(INT8_MAX, '\n');

                if(chgt == 'y' || chgt == 'Y'){
                    std::string ncomm;
                    std::cout << "Entrez le commentaire : il sera rajoute au commentaire existant " << '\n';
                    std::getline(std::cin, ncomm);
                    t->comms += "\n    ";
                    t->comms += ncomm;                    
                }
                
                else if(chgt != 'n' || chgt != 'N'){
                    std::cout << "I'll take this as a no..." << '\n';
                }

                std::cout << "Ajouter une/des sous-tache(s) ? [Y/N] ";
                std::cin >> chgt;
                std::cin.clear();
                std::cin.ignore(INT8_MAX, '\n');
                if(chgt == 'y' || chgt == 'Y'){
                    std::vector<task*> sub_to_add = addsub();
                    for(task* sub : sub_to_add){
                        t->subtasks.push_back(sub);
                    }
                }
                else if(chgt != 'n' || chgt != 'N'){
                    std::cout << "I'll take this as a no..." << '\n';
                }

            }
        }
        std::cout << std::flush;
    }

    void TaskManager::extraction(std::string const& str, const char delim, std::vector<std::string>& out){
        size_t start;
        size_t end = 0;

        while ((start = str.find_first_not_of(delim, end)) != std::string::npos){
            end = str.find(delim, start);
            out.push_back(str.substr(start, end - start));
        }
    }

    std::vector<task*> TaskManager::extract_subtasks(std::string str){

        /* Extrait des sous-taches associees a une tache a partir du fichier (d'où l'argument string)
        S'il n'y a pas de sous-taches associees, la string decrivant les sous-taches sera "NONE" */

        std::vector<task*> _subtasks = std::vector<task*>();
        std::string nosubs = "NONE";
        if(str == nosubs){
            return _subtasks;
        }

        else{
            std::vector<std::string> _tasks;
            TaskManager::extraction(str, sub_delim, _tasks); // chaque element de _tasks est une task* sous forme de string

            for(std::string _task : _tasks){
                task* t = extract_subtask(_task);
                _subtasks.push_back(t);
            }

            return _subtasks;
        }
    }


    task* TaskManager::extract_task(std::string line){

        /* Extrait une tache a partir du fichier. Le delimiteur entre attributs de tache est le caractere '|' */

        std::vector<std::string> out;
        extraction(line, attribute_delim, out); // chaque element de out est un attribut de task sous forme de string

        int _id = std::stoi(out[0]);
        std::string _title = out[1];
        std::string _description = out[2];
        std::string _d_creation = out[3];
        std::string _d_cloture = out[4];
        std::string _status = out[5];
        int _progress = std::stoi(out[6]);
        int _priority = std::stoi(out[7]);
        std::string _comms = out[8];
        std::vector<task*> _subtasks = extract_subtasks(out[9]);

        task* t = new task(_id, _title, _description, _d_creation, _d_cloture, _status, _progress, _priority, _comms, _subtasks);

        return t;
    }

    task* TaskManager::extract_subtask(std::string line){

        /* Extrait une tache a partir du fichier. Le delimiteur entre attributs d'une sous-tache est le caractere '{' */

        std::vector<std::string> out;
        extraction(line, sub_attr_delim, out); // chaque element de out est un attribut de task sous forme de string

        int _id = std::stoi(out[0]);
        std::string _title = out[1];
        std::string _description = out[2];
        std::string _d_creation = out[3];
        std::string _d_cloture = out[4];
        std::string _status = out[5];
        int _progress = std::stoi(out[6]);
        int _priority = std::stoi(out[7]);
        std::string _comms = out[8];
        std::vector<task*> _subtasks = std::vector<task*>();

        task* t = new task(_id, _title, _description, _d_creation, _d_cloture, _status, _progress, _priority, _comms, _subtasks);

        return t;
    }



    void TaskManager::write_task(task* t, std::string filename = "To_Do_List.txt"){

        /* ecrit une tache sous forme d'une string dans le fichier (ie sauvegarde d'une tache)

        Cette fonction est la plus "delicate" puisqu'elle fait appel aux delimiteurs, donc si ceux-ci ont ete saisis
        par l'utilisateur dans un titre par exemple, le programme plante. A l'heure où j'ecris ceci je n'ai
        pas encore ecrit de fonction (ou autre code) permettant d'empêcher l'utilisateur de saisir ces caracteres. */

        std::fstream f;
        f.open(filename, std::ios::app);

        int id = t->id;
        // std::cout << "id check " << id << '\n';
        std::string title = t->title;
        // std::cout << "title check" << title << '\n';
        std::string description = t->description;
        // std::cout << "desc check" << description << '\n';
        std::string d_creation = t->d_creation;
        // std::cout << "d_crea check" << d_creation << '\n';
        std::string d_cloture = t->d_cloture;
        // std::cout << "d_clot check" << d_cloture << '\n';
        std::string status = t->status;
        // std::cout << "status check" << status << '\n';
        int progress = t->progress;
        // std::cout << "progress check" << progress << '\n';
        int priority = t->priority;
        // std::cout << "prio check" << priority << '\n';
        std::string comms = t->comms;
        // std::cout << "comm check" << comms << '\n';
        
        f << id << attribute_delim << title << attribute_delim << description << attribute_delim << d_creation << attribute_delim 
        << d_cloture << attribute_delim << status << attribute_delim << progress 
        << attribute_delim << priority << attribute_delim << comms << attribute_delim;
        
        if(t->subtasks == std::vector<task*>()){
            f << "NONE";
        }
        else{

            for(task* s : t->subtasks){
                
                int sid = s->id;
                std::string stitle = s->title;
                std::string sdescription = s->description;
                std::string sd_creation = s->d_creation;
                std::string sd_cloture = s->d_cloture;
                std::string sstatus = s->status;
                int sprogress = s->progress;
                int spriority = s->priority;
                std::string scomms = s->comms;

                f << sid << sub_attr_delim << stitle << sub_attr_delim << sdescription << sub_attr_delim << sd_creation
                << sub_attr_delim << sd_cloture << sub_attr_delim << sstatus << sub_attr_delim 
                << sprogress << sub_attr_delim << spriority << sub_attr_delim << scomms << sub_attr_delim << "NONE";

                if (s != t->subtasks.back()){
                    f << sub_delim;
                }
            }
        }
        f << task_delim;
        f.close();
    }

    void TaskManager::save(){
        for(std::vector<task*>::iterator it = this->todolist.begin(); it != this->todolist.end(); ++it){
            write_task(*it);
            delete (*it);
        }
        std::cout << "Taches enregistrees avec succes" << std::endl;
    }

