#pragma once
#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include "../Database/DatabaseManager.h"
#include "../Database/Graph.h"
#include "Server.h"
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>

using namespace std;
// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

// Thread parameters struct
struct PARAMETERS
{
    Graph* graph;
    SOCKET lpParam;
};

// Admin panel options
enum Options { updateCity, searchPath, exitOption, addNewEdge, deleteCity, addCity, getCityInfo, printDB, printGraph };

// Checks for city id input correctness
bool checkInput(string id);
// Checks for city input correctness
bool checkInput(string id, string name, string weight);
// Starts admin Panel
void startAdminPanel(Graph& graph, DatabaseManager& database);
// Receive until the peer shuts down the connection
// our thread for receiving commands
DWORD WINAPI receive_cmds(void* params);
// Process clients requests
int processRequests(DatabaseManager& database, Graph& graph);
// Main function
int __cdecl main(void);