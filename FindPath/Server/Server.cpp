#include "Server.h"
#include <thread>

bool checkInput(string id)
{
    try
    {
        std::stoi(id);
    }
    catch (...)
    {
        cout << "Invalid Id" << endl;
        return false;
    }

    return true;
}

bool checkInput(string id, string name, string weight)
{
    try
    {
        int inputWeight = stoi(weight);
        if (inputWeight < 0)
        {
            cout << "Invalid weight: must be >= 0" << endl;
            return false;
        }
    }
    catch (...)
    {
        cout << "Invalid Weight" << endl;
        return false;
    }

    if (name.size() <= 0)
    {
        cout << "Invalid name" << endl;
        return false;
    }

    return  checkInput(id);
}

void startAdminPanel(Graph& graph, DatabaseManager& database)
{
    string sInput;
    int input;
    cout << "\n************************************************" << endl;
    cout << "\n0.UPDATE CITY\n";
    cout << "1.SEARCH PATH\n";
    cout << "2.EXIT\n";
    cout << "3.ADD EDGE\n";
    cout << "4.DELETE CITY\n";
    cout << "5.ADD CITY\n";
    cout << "6.GET CITY INFO\n";
    cout << "7.PRINT DB\n";
    cout << "8.PRINT GRAPH\n";
    cout << "\nEnter your choice : ";
    cin >> sInput;
    
    if(!checkInput(sInput))
    {
        input = -1;
    }
    input = stoi(sInput);

    switch (input)
    {
    case exitOption:
    {
        cout << "Exit..." << endl;
        break;
    }
    // Adds edge in graph
    case addNewEdge:
    {
        string input = "";
        int idA, idB;
  
        // validate input
        cout << "City A id: ";
        cin >> input;
        if (!checkInput(input))
            break;
        idA = stoi(input);

        cout << "City B id: ";
        cin >> input;
        if (!checkInput(input))
            break;
        idB = stoi(input);

        // add edge
        graph.addEdge(idA, idB);
        break;
    }
    // Updates city in graph and DB
    case updateCity:
    {
        string id, name, weight;
        City city;

        cout << "City id: ";
        cin >> id;
        cin.ignore();
        cout << "City name: ";
        getline(cin, name);
        cout << "City weight: ";
        cin >> weight;

        // Validate input
        if (!checkInput(id, name, weight))
            break;

        city.id = stoi(id);
        city.name = name;
        city.weight = stoi(weight);
        
        
        bool isUpdated = database.updateCity(city);
        if (isUpdated)
        {
            cout << "City " << city.id << " updated" << endl;
        }
        else
        {
            cout << "City " << city.id << " not updated" << endl;
        }
        break;
    }
    // Delete city in graph and DB
    case deleteCity:
    {
        string input;
        int id;

        // Get and validate input
        cout << "City id: ";
        cin >> input;
        if (!checkInput(input))
            break;
        id = stoi(input);

        bool isDeleted = database.deleteCity(id);
        if (isDeleted)
        {
            cout << "City " << id << " deleted" << endl;
        }
        else
        {
            cout << "City " << id << " not deleted" << endl;
        }
        break;
    }
    // Add city in DB
    case addCity:
    {
        string id, name, weight;
        City city;
       
        cout << "City id: ";
        cin >> id;
        cin.ignore();
        cout << "City name: ";
        getline(cin, name);
        cout << "City weight: ";
        cin >> weight;

        // Get and validate input
        if (!checkInput(id, name, weight))
            break;

        city.id = stoi(id);
        city.name = name;
        city.weight = stoi(weight);

        bool isAdded = database.addCity(city);
        if (isAdded)
        {
            cout << "City " << city.id << " added" << endl;
        }
        else
        {
            cout << "City " << city.id << " not added" << endl;
        }
        break;
    }
    // Print city info from DB
    case getCityInfo:
    {
        string input;
        int id;

        // Get and validate input
        cout << "City id: ";
        cin >> input;
        if (!checkInput(input))
            break;
        id = stoi(input);
        
        cout << database.getCity(id) << endl;
        break;
    }
    // Search for shortest path
    case searchPath:
    {
        string input;
        int source, dest;
        cout << "Type the source and destination you want to travel to." << endl;
     
        // Get and validate input
        cout << "Source city id: ";
        cin >> input;
        if (!checkInput(input))
            break;
        source = stoi(input);

        cout << "Destination city id: ";
        cin >> input;
        if (!checkInput(input))
            break;
        dest = stoi(input);

        cout << graph.getShortestPath(source, dest) << endl;
        break;
    }
    // Prints DB entries
    case printDB:
    {
        cout << "ID | Name | Weight" << endl;
        cout << database << endl;
        break;
    }
    // Prints graph
    case printGraph:
    {
        cout << graph << endl;
        break;
    }
    default:
        cout << "Choose a valid option." << endl;
        break;
    }

    if (input != exitOption)
    {
        startAdminPanel(graph, database);
    }
    else
    {
        exit(0);
    }
}

DWORD WINAPI receive_cmds(void* params)
{
    PARAMETERS* parameters = (PARAMETERS*)params;
    string shortestPath;
    // set our socket to the socket passed in as a parameter
    SOCKET ClientSocket = (SOCKET)parameters->lpParam;
    int iResult;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    char sendbuf[DEFAULT_BUFLEN];


    int recvbuflen = DEFAULT_BUFLEN;

    string received;
    string origin, destination;
    int iOrigin, iDestination;

    while (1)
    {
        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            printf("Bytes received: %d\n", iResult);
        }
        else if (iResult == 0) {
            printf("No data received...\n");
        }
        else {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
        }

        istringstream iss(recvbuf);
        if (iss >> origin >> destination)
        {
            // check input
            if (checkInput(origin) && checkInput(destination))
            {
                iOrigin = stoi(origin);
                iDestination = stoi(destination);
                shortestPath = parameters->graph->getShortestPath(iOrigin, iDestination);
                snprintf(sendbuf, DEFAULT_BUFLEN, shortestPath.c_str());
            }
            else
            {
                shortestPath = "Not valid input\n";
                snprintf(sendbuf, DEFAULT_BUFLEN, shortestPath.c_str());
            }

            // Send the path back to the sender
            iSendResult = send(ClientSocket, sendbuf, (int)(strlen(sendbuf) + 1), 0);
            if (iSendResult == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                return 1;
            }

        }


    }

    // shutdown the connection since we're done
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }
}

int processRequests(DatabaseManager& database, Graph& graph)
{
    PARAMETERS params;
    WSADATA wsaData;
    DWORD   dwThread;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo* result = NULL;
    struct addrinfo hints;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    while (1)
    {
        // Accept a client socket
        ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET) {
            printf("accept failed with error: %d\n", WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
            return 1;
        }

        params.graph = &graph;
        params.lpParam = ClientSocket;
        CreateThread(
            NULL,                   // default security attributes
            0,                      // use default stack size  
            receive_cmds,           // thread function name
            &params,                // argument to thread function 
            0,                      // use default creation flags 
            &dwThread);             // returns the thread identifier 

    }

    // cleanup
    closesocket(ClientSocket);
    WSACleanup();
}

int __cdecl main(void)
{
    cout << "Initializing DB..." << endl << endl;
    DatabaseManager database;
    cout << "Creating initial graph from graph.txt" << endl << endl;
    Graph graph(database);
    cout << "Listening for connections..." << endl << endl;
    thread thread_obj(processRequests, ref(database), ref(graph));

    // start admin panel
    startAdminPanel(graph, database);

    cout << "Exiting from Server.." << endl;
    return 0;
}