# Projet_Info
Projet C++ todo-list

FORMAT DU FICHIER DE STOCKAGE / DELIMITEURS :

entre tâches : %

entre attributs de tâches : |

entre sous-tâches : $

entre attributs de sous-tâches : {

NE PAS UTILISER CES CARACTERES LORS DE L'UTILISATION ! (Sauf si vous souhaitez justement tester la casse du logiciel =/ )

AJOUT DE DERNIERE MINUTE : Après avoir relu ma "doc" et fait quelques essais, je me suis rendu compte que les "caractères étranges" qui apparaissaient dans le fichier de sauvegarde sont évidemment des accents (mais ils n'apparaissaient pas pareil dans VSCode, je voyais des losanges avec un point d'interrogation au milieu...). J'ai donc remplacé tous les caractères accentués dans mon programme par des caractères sans accents, et l'erreur "std::basic_string:_M_construct null not valid" semble ne plus avoir lieu... Lors de l'utilisation, je conseille donc également de ne pas utiliser d'accent.
