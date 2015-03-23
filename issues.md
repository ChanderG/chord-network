###Issues

* Use getifaddr to note down external address, not internal one.
* Move to config file in dir of exec rather than taking input manually
* Config file parser is positive only, basic .
* Only IP is being mapped in index. Therefore file-transfer cannot be shown in simulation.

Organization:

Each node has 2 sockets. One as server looking ahead. binding self and waiting.
Another trying to connect to the prev peer,as a client.
