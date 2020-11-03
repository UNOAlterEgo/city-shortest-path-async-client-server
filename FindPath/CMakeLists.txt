# CMakeList.txt: progetto CMake per Consegna. Includere l'origine e definire
# qui la logica specifica del progetto.
#
cmake_minimum_required (VERSION 3.8)

# Aggiungere l'origine all'eseguibile di questo progetto.
add_executable (client "Client/Client.cpp" "Client/Client.h" )
add_executable (server "Server/Server.cpp" "Server/Server.h" "Database/DatabaseManager.cpp" "Database/DatabaseManager.h" "Database/UML/City.cpp" "Database/UML/City.h"   "Database/Graph.cpp" "Database/Graph.h")

# TODO: Aggiungere i test e, se necessario, installare le destinazioni.
