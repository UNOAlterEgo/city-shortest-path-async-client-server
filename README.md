### Quick summary ###
The system consists of a client, a Windows server and a database.

For reasons of simplicity the server acts as an admin panel, although for correctness the admin should be managed like any other client and users should be distinguished by the role (for example standard, admin).

The Database contains entries for cities. A city must be present in the database to be added to the path graph.
We have two UML classes:


   - User with username(key), password and role;
   - City with id(key), name, weight



### Build

The project requeres [CMake](https://cmake.org/download/) v3.18+ to build.
[Running CMake](https://cmake.org/runningcmake/)
Note that the server is a Windows server, so this OS is required to run the solution.


### Usage ###

Run server.exe followed by client.exe and follow instruction.

Cities are saved in the default file cities.txt in the format: id,name,weight.  
This file can be given as input to the program or created through the administration panel.

```diff
Note that due to the fact that there can be two cities with the same name, you have chosen to use the id of the cities as keys.  
```

The graph paths are saved in the file graph.txt in the format: source, destination.  
This file can be given as input to the program or created via the administration panel.

```diff
Being a non-directed graph, if there is an edge from city 0 to city 1 it must be inserted in the graph.txt file as:  
1 0  
0 1  
```

### To do ###
For lack of time the authentication management has not been implemented, so the user class is not used.  
The socket closing of the client is not well handled.