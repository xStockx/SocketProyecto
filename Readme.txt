La mise en oeuvre du programme de gestion de carte Arduino par Websocket nécessite:
- l'installation du fichier "stantor_websocket_uno_v06.ino" dans une carte Arduino Uno/Duemilanove par l'IDE 1.0 ou 1.0.1 ,
- l'installation de la librairie WebsocketServer dans le répertoire librairies de l'IDE,
- un navigateur supportant les "WebSockets" (Firefox 14 ou ultérieur, Chrome 2O ou ultérieur, etc...),
- la configuration du fichier "stantor_websocket_uno_v06.ino" conformément au manuel de configuration ci-joint,
- la configuration du fichier "websocket_arduino_uno_v06.html" conformément au manuel de configuration ci-joint,
- la configuration du fichier "graphe_tendance_v06.html" conformément au manuel de configuration ci-joint,

Le lancement du programme s'effectue en ouvrant le fichier "websocket_arduino_uno_v06.html" par le navigateur.
L'appui sur le bouton "initialisation connexion" permet d'établir la liaison permanente avec la carte Arduino au travers d'Internet/Intranet.
L'appui sur le bouton "déconnexion" interrompt la cette liaison.

Ces programmes sont sous licences GPL (cf fichier licence ci-joint).
