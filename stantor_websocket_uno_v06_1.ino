/*
##############################################################################################
 
	Programme  Serveur Web STANTOR & JLECTRONIQUE
	pour Cartes électroniques ARDUINO UNO et DUEMILANOVE avec carte d'extension Ethernet (modèle Wiznet W5100).
	Version compatible avec l'IDE 1.0.1

	Version v0.6.3 du 2012-09-03
	
----------------------------------------------------------------------------------------------
FRENCH
 Programme de lecture et écriture d'informations digitales et analogiques pour cartes Uno/Duemilanove par liaison sur Ethernet  & TCP/IP (par Websocket).
 
Les fonctions de ce programme sont:
- Serveur internet pour fournir les informations:
    -entrées et sorties digitales,
    -entrées analogiques,
    -sorties PWM,
    -compteurs d'impulsions,
    à navigateur (Firefox, Chrome, IE,...) ou à un micro-ordinateur distant.
- Serveur internet pour exécuter les commandes de:
    -sorties digitales
    -commandes PWM
    -initialisation de compteurs d'impulsion
   transmise par un navigateur ou un micro-ordinateur distant.
  
 Pour chaque entrée digitale il est possible de filtrer les parasites par un traitement anti-rebond.
 (paramètre nb_vi_pinDXX correspondant au nombre minimum d'états successifs identiques de cette pin).  
 Pour chaque entrée analogique il est possible de filtrer les valeurs aberrantes.
 (paramètre correspondant à la prise compte de la médiane des nb_vi_pinDXX dernières valeurs de cette pin).  
 Les entrées digitales 2, 3, 18, 19, 20 et 21 peuvent fonctionner comme compteurs d'impulsions sous interruption. 

 Les échanges sont basés sur le transfert d'une trame de données de type CSV.
 Chaque trame posséde à minima 1 caractère de début de trame STX et un caractère de fin de trame ETX.
 
 La trame de commande des sorties (outputs) digitales, des sorties PWM et des initialisations des compteurs d'impulsion a le format suivant: 
  STX
  identifiant carte
  séparateur  
  type de sortie
  séparateur
  rang de sortie
  séparateur
  valeur de sortie
  ETX
    
 Exemples:(UN02;OD;12;1)  => sortie digitale n°12 à la valeur 1 (HIGH) de la carte Uno 02.
	  (DU02;OD;12;1)  => sortie digitale n°12 à la valeur 1 (HIGH) de la carte Duemilanove 02.
	  (UN00;OP;09;128) => sortie PWM n°9 à la valeur 128 de la carte Uno 00. 
	  (UN05;CP;03;4567)=>compteur sur l'entrée digitale 3 forcé à la valeur 4567 (interruption 1 et valeur au format unsigned long).
	 

 La trame d'événement envoyée au client à l'initiative du serveur (propriété des webSockets) comprend:
 STX
  identifiant carte 
  séparateur
  type carte
  séparateur
  type d'information
  séparateur
  rang
  séparateur
  valeur
  ----------
  ETX
  
  
 Exemples de trame émise:
    (UN05;ID;02;1)  
    (DU04;OD;03;0)  
    (UN05;CP;03;5689)=>compteur sur l'entrée digitale 3 forcé à la valeur 5689 (interruption 1 et valeur au format unsigned long).
    Trame "ping" suivi de "pong".

 Lors de l'initialisation de la carte, un programme de repli des sorties (outputs) est exécuté.
 Ce programme positionne les sortie digitales et les sorties PWM à des valeurs prédéfinies en position de repli.

 Lors de l'initialisation il est possible de mettre en service les résistances de Pull-up internes pour les entrées digitales et analogiques concernées. 
 Pour chaque entrée un paramètre perment de choisir cette mise en oeuvre: valeur N (= non) ou Y (=oui).

 Lors de l'initialisation il est possible de positionner la référence de tension des entrées analogiques (pin AREF)
à une valeur prédéfinie. Les types acceptés sont: DEFAULT, INTERNAL, EXTERNAL, INTERNAL1V1 ou INTERNAL2V56.

--------------------------------------------------------------------------------------------
ENGLISH

Program for reading and writing digital and analog informations of Arduino Uno board by Ethernet network & TCP/IP (with WebSockets).

The functions of this program are:
- Internet Server to provide informations:
    -Digital inputs and outputs,
    -Analog inputs,
    PWM-output,
    -Pulse counters,
  to browser (Firefox, Chrome, IE, ...) or a remote computer.
- Internet Server to execute commands:
    -Digital outputs
    -PWM-control
    -Initialization pulse counters.
  transmitted by a browser or a remote computer.

 For each digital input it is possible to filter the noise by an debounce.
 (Nb_vi_pinDXX parameter corresponding to the minimum number of successive states of the same pin).
 For each analog input it is possible to filter outliers.
 (Parameter for taking into account the median values ​​of this last nb_vi_pinDXX pine).
 Digital inputs 2, 3, 18, 19, 20 and 21 can function as an interrupt pulse counters.

 The exchanges are based on the transfer of a data frame type CSV.
 Each frame possesses a minimum at start character STX frame and frame end character ETX.

The command frame outputs (outputs) digital, and PWM outputs initializations counters pulse has the following format:
  STX
  ID card
  separator
  output type
  separator
  rank output
  separator
  output value
  ETX
  
 Examples: (UN02;OD;12;1)  => sortie digitale n°12 à la valeur 1 (HIGH) de la carte Uno 02.
	  (DU02;OD;12;1)  => sortie digitale n°12 à la valeur 1 (HIGH) de la carte Duemilanove 02.
	  (UN00;OP;09;128) => sortie PWM n°9 à la valeur 128 de la carte Uno 00. 
	  

 The frame event sent to the client-initiated server (owned WebSockets) includes:
 STX
  ID card
  separator
  card type
  separator
  type of information
  separator
  rank
  separator
  value
  ----------
  ETX
 Examples of frame sent:
    (UN05;ID;02;1)  
    (DU04;OD;03;0)  
    (UN05;CP;03;5689)=>count on the digital input 3 forced to the value 5689 (interruption and a value to an unsigned long).
    Frame "ping" followed by "pong".

 When initializing the board a program of withdrawal of outputs (digital and analogic) is executed.
 This program sets the digital and PWM outputs to predefined values.
 
 During initialization it is possible to activate the pull-up resistor for analog and digital inputs concerned.
 For each input parameter perment choosing this implementation value N (= no) or Y (= yes).

 During initialization it is possible to position the reference voltage of analog inputs (pin AREF)
a predefined value. The accepted types are: DEFAULT, INTERNAL, EXTERNAL, INTERNAL1V1 ou INTERNAL2V56.	
	
##############################################################################################
 Projet/prototype STANTOR
 Copyright (C) 2000 Hubert HERSENT 
 Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les termes
 de la Licence Publique Générale GNU telle que publiée par la Free Software Foundation ;
 soit la version 2 de cette licence, soit toute autre version ultérieure.
 Ce programme est distribué dans l'espoir qu'il sera utile, mais SANS AUCUNE GARANTIE,
 ni explicite ni implicite, y compris les garanties de commercialisation ou d'adaptation
dans un but spécifique. Prenez connaissance de la Licence Publique Générale GNU pour plus de
 détails. Ecrivez à la Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 Pour des compléments d'informations E-mail : stantor@free.fr ou info@jlectronique.org
##############################################################################################
 */
// 
//ATTENTION: Ce programme doit être compilé avec l'IDE Arduino n°1.0 ou ultérieur.
#include <SPI.h>
#include <Ethernet.h>
//
// Here we define a maximum framelength to 64 bytes. Default is 256.
#define MAX_FRAME_LENGTH 64
//
#include <WebSocket.h>
//
#include <limits.h>
//*****************************************************************************************************************
//
//*********** SECTION DE PARAMETRAGE SPECIFIQUE A CHAQUE CARTE ( Zone à modifier -  Zone change) *****************
//
// Mode debug sur le Serial port.
//#define DEBUG
//
// Type de carte Uno ou Duemilanove 20 broches ou 22 broches utiles (22 pins pour Duemilanove Nano).  
#define TypeBoard "19" 	//ou 21 ------- Valeur 19 pour pin 0 à pin 19 ou Valeur 21 pour pin 0 à pin 21.  
//#define TypeBoard "21"		// Avec 6 entrées analogiques pour une carte 20 pins ou 8 entrées analogiques pour une carte 22 pins .
//
byte mac[] = { 0x52, 0x4F, 0x43, 0x4B, 0x45, 0x54 };
byte ip[] = { 192, 168, 99 , 75 };
//#define PortArduino  4832 //port  du serveur de la carte Arduino.
//
// Identifiant de la carte.
#define NumBoard  "UN01" // Valeurs UN00, UN01,...UN99 or DU00, DU01,...DU99.
//
// Valeur de la période de la boucle de base.
#define periode 1	 //en millisecondes 1 milliseconde en standard..
//
// Type de la tension de référence utilisée avec les entrées analogiques.
#define V_Reference DEFAULT //DEFAULT, INTERNAL, EXTERNAL
//
//Zone de définition du sens et du type de fonctionnement des broches avec:
// RX = receive serial (UART)
// TX = transmit serial (UART)
// ID = input digital
// OD = output digital
// IA = input analog
// OP = output PWM
// IC= serial I2C (ou TWI)
// SP = serial SPI 
// PC = Change -pulse counter
// PR = Rising -pulse counter
// PF = Falling-pulse counter
//Affectation des broches (pins) suivant le besoin.
#define SenspinD00 "ID"	 // ID ou OD (RX sur liaison USB) 	
#define SenspinD01 "OD"	 // ID ou OD (TX sur liaison USB)
#define SenspinD02 "ID"  // ID ou OD ou PC ou PR ou PF (interrupt 0: PC=Change, PR=Rising, PF=Falling. Cf explication sur le site http://www.arduino.cc) . 
#define SenspinD03 "PR"  // ID ou OD ou OP ou PC ou PR ou PF (interrupt 0: PC=Change, PR=Rising, PF=Falling. Cf explication sur le site http://www.arduino.cc) . 
#define SenspinD04 "OD"  // ID ou OD
#define SenspinD05 "OP"  // ID ou OD ou OP
#define SenspinD06 "ID"  // ID ou OD ou OP
#define SenspinD07 "OD"  // ID ou OD
#define SenspinD08 "ID"  // ID ou OD
#define SenspinD09 "OD"  // ID ou OD ou OP
#define SenspinD10 "SP"  // SP (ne pas modifier pour liaison Ethernet)  (ID ou OD ou OP sur liaison USB)
#define SenspinD11 "SP"  // SP (ne pas modifier pour liaison Ethernet)  (ID ou OD ou OP sur liaison USB)
#define SenspinD12 "SP"  // SP (ne pas modifier pour liaison Ethernet)  (ID ou OD sur liaison USB)
#define SenspinD13 "SP"  // SP (ne pas modifier pour liaison Ethernet)  (ID ou OD sur liaison USB)
#define SenspinD14 "ID"  // ID ou OD ou IA = pin 0 analogique
#define SenspinD15 "OD"  // ID ou OD ou IA = pin 1 analogique    
#define SenspinD16 "IA"  // ID ou OD ou IA = pin 2 analogique
#define SenspinD17 "IA"  // ID ou OD ou IA = pin 3 analogique
#define SenspinD18 "IA"  // ID ou OD ou IA = pin 4 analogique
#define SenspinD19 "OD"  // ID ou OD ou IA = pin 5 analogique
#define SenspinD20 "IA"  // ID ou OD ou IA = pin 6 analogique
#define SenspinD21 "IA"  // ID ou OD ou IA = pin 7 analogique
//
#define pull_up_pinD00 'N'	//valeur N (par défaut) ou Y pour le positionnement de la résistance interne de pull-up.
#define pull_up_pinD01 'N'
#define pull_up_pinD02 'N'	
#define pull_up_pinD03 'N'
#define pull_up_pinD04 'N'
#define pull_up_pinD05 'N'
#define pull_up_pinD06 'N'
#define pull_up_pinD07 'N'
#define pull_up_pinD08 'N'
#define pull_up_pinD09 'N'
#define pull_up_pinD10 'N'
#define pull_up_pinD11 'N'
#define pull_up_pinD12 'N'
#define pull_up_pinD13 'N'
#define pull_up_pinD14 'N'
#define pull_up_pinD15 'N'
#define pull_up_pinD16 'N'
#define pull_up_pinD17 'N'
#define pull_up_pinD18 'N'
#define pull_up_pinD19 'N'
#define pull_up_pinD20 'N'
#define pull_up_pinD21 'N'
//
#define width_max_0 100UL	//valeur du temps minimum entre l'apparition de 2 interruptions successives pour l'interruption 0 pour effectuer un comptage - exemple: 100ms (filtrage des parasites).
#define width_max_1 100UL	//valeur du temps minimum entre l'apparition de 2 interruptions successives pour l'interruption 1 pour effectuer un comptage - exemple: 100ms (filtrage des parasites).
//
// Valeurs à l'initialisation (ou de repli) des sorties digitales et des sorties PWM (sans effet sur les autres type de broches).
#define ReplipinD00 0	// valeur = 0 ou 1
#define ReplipinD01 0	// valeur = 0 ou 1
#define ReplipinD02 0	// valeur = 0 ou 1
#define ReplipinD03 0	// valeur = 0 ou 1 ou 0-255
#define ReplipinD04 0	// valeur = 0 ou 1
#define ReplipinD05 0	// valeur = 0 ou 1 ou 0-255
#define ReplipinD06 0	// valeur = 0 ou 1 ou 0-255
#define ReplipinD07 0	// valeur = 0 ou 1
#define ReplipinD08 0	// valeur = 0 ou 1
#define ReplipinD09 0	// valeur = 0 ou 1 ou 0-255
#define ReplipinD10 0	// valeur = 0 ou 1 ou 0-255
#define ReplipinD11 0	// valeur = 0 ou 1 ou 0-255
#define ReplipinD12 0	// valeur = 0 ou 1
#define ReplipinD13 0	// valeur = 0 ou 1
#define ReplipinD14 0	// valeur = 0 ou 1
#define ReplipinD15 0	// valeur = 0 ou 1
#define ReplipinD16 0	// valeur = 0 ou 1
#define ReplipinD17 0	// valeur = 0 ou 1
#define ReplipinD18 0	// valeur = 0 ou 1
#define ReplipinD19 0	// valeur = 0 ou 1
#define ReplipinD20 0	// valeur = 0 ou 1
#define ReplipinD21 0	// valeur = 0 ou 1
//
//Nombre de valeurs utilisées pour le filtrage anti-rebond et points aberrants pour
//chaque entrée digitale ou analogique. Nombre inférieur à nb_vi_max-1 et 0 équivalent à aucun filtrage.
// (sans effet sur les sorties digitales , PWM, RX, TX et SP ).
#define nb_vi_pinD00 0  //valeur 0,1,2....6 avec 0 par défaut.
#define nb_vi_pinD01 0
#define nb_vi_pinD02 0	
#define nb_vi_pinD03 0
#define nb_vi_pinD04 0
#define nb_vi_pinD05 0
#define nb_vi_pinD06 0
#define nb_vi_pinD07 0
#define nb_vi_pinD08 0
#define nb_vi_pinD09 0
#define nb_vi_pinD10 0
#define nb_vi_pinD11 0
#define nb_vi_pinD12 0
#define nb_vi_pinD13 0
#define nb_vi_pinD14 0
#define nb_vi_pinD15 0
#define nb_vi_pinD16 0
#define nb_vi_pinD17 0
#define nb_vi_pinD18 0
#define nb_vi_pinD19 0
#define nb_vi_pinD20 0
#define nb_vi_pinD21 0
//
//pin attribuée à la sortie digitale du watchdog visuel .
#define pin_watchdog 07
//
//format des caractères de gestion de la trame (CSV).
#define STX '('  // Caractère imprimable par défaut.
#define ETX ')'  // Caractère imprimable par défaut.
#define separator ';' //Caractère Point-virgule imprimable par défaut.
//
//********************** FIN SECTION SPECIFIQUE A LA CARTE ****************************
//
//
//********* DEBUT SECTION PARAMETRES INTERNES  AU PROGRAMME(Do not change - ne pas modifier) ***************
//
// Vitesse de transmission de la liaison tty virtuelle.
#define speedtransmission 115200 // en bit/s (uniquement pour test).
//
//Nombre de valeurs intermédiaires maximum pour le filtrage des entrées digitales et analogiques.
#define nb_vi_max 7
//
//définition des noms des broches entrées sorties digitales et sorties PWM.
#define pinD00 0
#define pinD01 1
#define pinD02 2
#define pinD03 3
#define pinD04 4
#define pinD05 5
#define pinD06 6
#define pinD07 7
#define pinD08 8
#define pinD09 9
#define pinD10 10
#define pinD11 11
#define pinD12 12
#define pinD13 13
#define pinD14 14
#define pinD15 15
#define pinD16 16
#define pinD17 17
#define pinD18 18
#define pinD19 19
#define pinD20 20
#define pinD21 21
//
//définition des noms des broches d'entrées analogiques.
#define pinA00 0
#define pinA01 1
#define pinA02 2
#define pinA03 3
#define pinA04 4
#define pinA05 5
#define pinA06 6
#define pinA07 7
//
#define nb_pins 22	//nombre de broches (pins) de la carte suivant le type de carte (par défaut le maximum).
#define nb_pins_ana 6	//nombre de broches (pins) réservées aux entrées analogiques (par défaut le maximum).
#define decalia 14	//rang des broches (pins) des entrées analogiques. 
//
//******** SECTION VARIABLES GLOBALES (Do not change - ne pas modifier) *****************
//
// Valeurs courantes des entrées digitales et analogiques pour transmision.
int vc_in_array[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//Valeurs courantes des sorties digitales et PWM pour lecture.
int PWM_digi_array[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//Valeurs du cycle précédent des sorties digitales et PWM pour comparaison.
int old_PWM_digi_array[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//
char evtserver[32];//chaine d'émission de message du serveur vers le client 64 caractères maximum.
int numsortie=0;
int valeurPWM=0;
//
//Zone de définition du sens de fonctionnement de chaque broche.
char* Senspin[nb_pins]={SenspinD00,SenspinD01,SenspinD02,SenspinD03,SenspinD04,SenspinD05,SenspinD06,SenspinD07,SenspinD08,SenspinD09,SenspinD10,SenspinD11,SenspinD12,SenspinD13,SenspinD14,SenspinD15,SenspinD16,SenspinD17,SenspinD18,SenspinD19,SenspinD20,SenspinD21};
//Zone Senspin transmis lors d'une commande digitale ou analogique.
char Senspin_commande[3] = "ND" ; //
//Zone Sens pin sous forme de string pour faire la comparaison.
char Senspin_string[3] = "ND" ; //
//
char statuscard[] = "E0";// Code status E0 = en phase d'initialisation de la carte. 
char numcard[] = NumBoard; //chaine représentant l'identifiant de la carte.
char rang_char[2];//Rang de la broche en caractère char.
//Compteur pour gérer le clignotement de la led du Watchdog.
int Watchdog=0;
//
//compteurs de ticks à la milliseconde pour synchronisation horloge.
unsigned long tick_old=0;
unsigned long tick_new=0;
//
//Compteurs d'impulsion sous interruption.
volatile unsigned long compteur_pulse_0=0;
volatile unsigned long compteur_pulse_1=0;
//
volatile unsigned long old_time_pulse_0=0;
volatile unsigned long old_time_pulse_1=0;
//
//Indicateur de fonctionnement du ping pong
byte indic_ping_pong=0;
//Indicateur de fonctionnement de la procédure d'envoi forcée des informations digitales et sorties PWM.
byte indic_init_digi=1;
//
//Indicateur de fonctionnement de la procédure d'envoi forcée des informations analogiques et compteurs d'impulsion.
byte indic_init_ana=0;
//
//**********************************************************************************************
//
//********* SECTION FONCTIONS DIVERSES  (Do not change - ne pas modifier) **********************
//
// Create a Websocket server
WebSocket wsServer;
//
void onConnect(WebSocket &socket) {
    #ifdef DEBUG 
		Serial.println("onConnect called");//pour test.
    #endif
}
//
// Programme de traitement des données reçues du client.
void onData(WebSocket &socket, char* dataString, byte frameLength) {
    #ifdef DEBUG
		Serial.print("Got data: ");
		Serial.write((unsigned char*)dataString, frameLength);
		Serial.println();
    #endif
     //Test de la chaine reçue.
	if (strncmp(dataString,"ping",4)==0){
		indic_ping_pong=1;
	}
	else{
		if ((dataString[0] == STX) && (dataString[1] == numcard[0]) && (dataString[2] == numcard[1]) && (dataString[3] == numcard[2]) && (dataString[4] == numcard[3])) {
			//test si commande de sortie digitale normalement formée.
			if ((dataString[6] == 'O') && (dataString[7] == 'D') && (dataString[8] == separator) && (dataString[11] == separator) && (dataString[13] == ETX)){ 
				// Calcul du numero de sortie.
				numsortie=((dataString[9] -'0')*10) + (dataString[10] -'0');
				//test si numéro de sortie digitale compris entre 0 et 21 pour la carte et le sens de la pin est correct.
				Senspin_commande[0]=dataString[6];
				Senspin_commande[1]=dataString[7];
				Senspin_string[0]=Senspin[numsortie][0];
				Senspin_string[1]=Senspin[numsortie][1];
				if ((numsortie <= 21) && (strcmp(Senspin_string,Senspin_commande)==0)){
					//commande de positionnement de l'information digitale.
					if ((dataString[12]-'0') == 1 ){
						digitalWrite(numsortie, HIGH);
						PWM_digi_array[numsortie]=1;//stockage de la valeur de la sortie.
						statuscard[0] = 'O';
						statuscard[1] = 'K';
					}
					else if ((dataString[12]-'0') == 0 ){
						digitalWrite(numsortie, LOW);
						PWM_digi_array[numsortie]=0;//stockage de la valeur de la sortie.
						statuscard[0] = 'O';
						statuscard[1] = 'K';
					}
					else {
						statuscard[0] = 'E';//Erreur dans les champs d'informations de la commande digitale.
						statuscard[1] = '2';
					} 
				}
				else {
					statuscard[0] = 'E';//Erreur dans les champs d'informations de la commande digitale.
					statuscard[1] = '2';
				}
			}
			// Test si commande de sortie PWM.
			else if ((dataString[6] == 'O') && (dataString[7] == 'P') && (dataString[8] == separator) && (dataString[11] == separator)){
				//Tests pour créer les zéros manquants avec Décalage à droite.
				if (dataString[13] == ETX){	
					dataString[15]=ETX;
					dataString[14]=dataString[12];
					dataString[13]='0';
					dataString[12]='0';
				}
				else if (dataString[14] == ETX){
					dataString[15]=ETX;
					dataString[14]=dataString[13];
					dataString[13]=dataString[12];
					dataString[12]='0';
				}
				// Calcul du numéro de sortie.
				numsortie=((dataString[9] -'0')*10) + (dataString[10] -'0');
				valeurPWM=(((dataString[12]-'0')*100)+((dataString[13]-'0')*10)+ (dataString[14]-'0')); 
				//Test de conformité des champs transmis - numéro de la sortie, valeur et sens de la pin sont corrects. 					
				Senspin_commande[0]=dataString[6];
				Senspin_commande[1]=dataString[7];
				Senspin_string[0]=Senspin[numsortie][0];
				Senspin_string[1]=Senspin[numsortie][1];		
				if (((numsortie == 3) || (numsortie == 5)  || (numsortie == 6) || (numsortie == 9) || (numsortie == 10) || (numsortie == 11)) && (valeurPWM <= 255) && (strcmp(Senspin_string,Senspin_commande)==0)) {  
					//commande de positionnement de l'information PWM.
					analogWrite(numsortie, valeurPWM);
					//sauvegarde de la valeur dans la zone mémoire spécifique.
					PWM_digi_array[numsortie]=valeurPWM;
					// Positionnement status
					statuscard[0] = 'O';
					statuscard[1] = 'K';
				}
				else {
					statuscard[0] = 'E';//Erreur dans les champs d'informations de la commande PWM.
					statuscard[1] = '3'; 
				}  
			}
			//test si demande de mise à XX d'un compteur d'impulsion.
			else if ((dataString[6] == 'C') && (dataString[7] == 'P') && (dataString[8] == separator) && (dataString[11] == separator)){
				numsortie=((dataString[9] -'0')*10) + (dataString[10] -'0');
				if ((numsortie == 2) || (numsortie == 3)){
					if (numsortie == 2){
						compteur_pulse_0=strtoul(&dataString[12],NULL,10); 
					}
					else if (numsortie== 3){
						compteur_pulse_1=strtoul(&dataString[12],NULL,10); 
					}
					// Positionnement status
					statuscard[0] = 'O';
					statuscard[1] = 'K';					
				}
				else {
					// Positionnement status
					statuscard[0] = 'E';//Erreur dans les champs d'informations de la commande de compteur.
					statuscard[1] = '4';         
				}
			}
			else {
				//Commande non comprise ou de format incorrecte.
				statuscard[0] = 'E';
				statuscard[1] = '1';
			}
		}
	}
}
//
void onDisconnect(WebSocket &socket) {
  #ifdef DEBUG    
  Serial.println("onDisconnect called");//pour test.
  #endif 
}
// Fonction d'écriture des valeurs de repli des sorties digitales et des sorties PWM lors du lancement du programme.
void repliOutputs(void){   
    if (SenspinD00 == "OD") digitalWrite(pinD00, ReplipinD00);
    PWM_digi_array[0]=ReplipinD00;//stockage de la valeur de la sortie.
    //
    if (SenspinD01 == "OD") digitalWrite(pinD01, ReplipinD01);
    PWM_digi_array[1]=ReplipinD01;//stockage de la valeur de la sortie.
    //
    if (SenspinD02 == "OD") digitalWrite(pinD02, ReplipinD02);
    PWM_digi_array[2]=ReplipinD02;//stockage de la valeur de la sortie.
    //
    if (SenspinD03 == "OD") digitalWrite(pinD03, ReplipinD03);
    else if (SenspinD03 == "OP") analogWrite(pinD03, ReplipinD03);
    PWM_digi_array[3]=ReplipinD03;//stockage de la valeur de la sortie.
    //
    if (SenspinD04 == "OD") digitalWrite(pinD04, ReplipinD04);
    PWM_digi_array[4]=ReplipinD04;//stockage de la valeur de la sortie.	
    //
    if (SenspinD05 == "OD") digitalWrite(pinD05, ReplipinD05);
    else if (SenspinD05 == "OP") analogWrite(pinD05, ReplipinD05);
    PWM_digi_array[5]=ReplipinD05;//stockage de la valeur de la sortie.		
    //
    if (SenspinD06 == "OD") digitalWrite(pinD06, ReplipinD06);
    else if (SenspinD06 == "OP") analogWrite(pinD06, ReplipinD06);
    PWM_digi_array[6]=ReplipinD06;//stockage de la valeur de la sortie.		
    //
    if (SenspinD07 == "OD") digitalWrite(pinD07, ReplipinD07);
    PWM_digi_array[7]=ReplipinD07;//stockage de la valeur de la sortie.			
    //
    if (SenspinD08 == "OD") digitalWrite(pinD08, ReplipinD08);
    PWM_digi_array[8]=ReplipinD08;//stockage de la valeur de la sortie.		
    //
    if (SenspinD09 == "OD") digitalWrite(pinD09, ReplipinD09);
    else if (SenspinD09 == "OP") analogWrite(pinD09, ReplipinD09);
    PWM_digi_array[9]=ReplipinD09;//stockage de la valeur de la sortie.	
    //
    if (SenspinD10 == "OD") digitalWrite(pinD10, ReplipinD10);
    else if (SenspinD10 == "OP") analogWrite(pinD10, ReplipinD10);
    PWM_digi_array[10]=ReplipinD10;//stockage de la valeur de la sortie.		
    //
    if (SenspinD11 == "OD") digitalWrite(pinD11, ReplipinD11);
    else if (SenspinD11 == "OP") analogWrite(pinD11, ReplipinD11);
    PWM_digi_array[11]=ReplipinD11;//stockage de la valeur de la sortie.	
    //
    if (SenspinD12 == "OD") digitalWrite(pinD12, ReplipinD12);
    PWM_digi_array[12]=ReplipinD12;//stockage de la valeur de la sortie.		
    //
    if (SenspinD13 == "OD") digitalWrite(pinD13, ReplipinD13);
    PWM_digi_array[13]=ReplipinD13;//stockage de la valeur de la sortie.
    //
    if (SenspinD14 == "OD") digitalWrite(pinD14, ReplipinD14);
    PWM_digi_array[14]=ReplipinD14;//stockage de la valeur de la sortie.
    //
    if (SenspinD15 == "OD") digitalWrite(pinD15, ReplipinD15);
    PWM_digi_array[15]=ReplipinD15;//stockage de la valeur de la sortie.
    //
    if (SenspinD16 == "OD") digitalWrite(pinD16, ReplipinD16);
    PWM_digi_array[16]=ReplipinD16;//stockage de la valeur de la sortie.
    //
    if (SenspinD17 == "OD") digitalWrite(pinD17, ReplipinD17);
    PWM_digi_array[17]=ReplipinD17;//stockage de la valeur de la sortie.
    //
    if (SenspinD18 == "OD") digitalWrite(pinD18, ReplipinD18);
    PWM_digi_array[18]=ReplipinD18;//stockage de la valeur de la sortie.
    //
    if (SenspinD19 == "OD") digitalWrite(pinD19, ReplipinD19);
    PWM_digi_array[19]=ReplipinD19;//stockage de la valeur de la sortie.
    //
    // Test si carte Uno ou Duemilanove 22 broches
    if (TypeBoard == "21") {    
	if (SenspinD20 == "OD") digitalWrite(pinD20, ReplipinD20);
	PWM_digi_array[20]=ReplipinD20;//stockage de la valeur de la sortie.
	//
	if (SenspinD21 == "OD") digitalWrite(pinD21, ReplipinD21);
	PWM_digi_array[21]=ReplipinD21;//stockage de la valeur de la sortie.
	//
    }
}
//
// Modules sous interruption pour le comptage d'impulsions.
void comptage_pulse_0() {
    //Verification de la largeur d'impulsion mini
    if((millis() >= old_time_pulse_0) && ((millis()-old_time_pulse_0)>width_max_0)){ 
		compteur_pulse_0++;
		old_time_pulse_0=millis();
    }
    else if ((millis() < old_time_pulse_0) && ((ULONG_MAX - old_time_pulse_0 + millis())>width_max_0)){
		compteur_pulse_0++;	
		old_time_pulse_0=millis();		
    } 
}
void comptage_pulse_1() {
    //Verification de la largeur d'impulsion mini
    if((millis() >= old_time_pulse_1) && ((millis()-old_time_pulse_1)>width_max_1)){ 
		compteur_pulse_1++;
		old_time_pulse_1=millis();
    }
    else if ((millis() < old_time_pulse_1) && ((ULONG_MAX - old_time_pulse_1 + millis())>width_max_1)){
		compteur_pulse_1++;
		old_time_pulse_1=millis();
    }
}

//
//********************************************************************************************
//
//******** SECTION INITIALISATION PROGRAMME  (Do not change - ne pas modifier) *************** 
void setup() {
    #ifdef DEBUG     
	Serial.begin(speedtransmission); //pour test.
	Serial.println("Lancement du Programme WebSocket v06.");
    #endif    
    // Initialisation de la tension de référence des entrées analogiques.
    analogReference(V_Reference);
    //
    // Initialisation des entrées sorties en INPUT ou OUTPUT  avec mise en service éventuelle de la résistance de pull-up .
    if (SenspinD00 == "ID"){
	pinMode(pinD00, INPUT);
	if (pull_up_pinD00!='N') digitalWrite(pinD00,HIGH);
    }
    else if (SenspinD00 == "OD") pinMode(pinD00, OUTPUT);
	//
    if (SenspinD01 == "ID"){
	pinMode(pinD01, INPUT);
	if (pull_up_pinD01!='N') digitalWrite(pinD01,HIGH);
    }
    else if (SenspinD01 == "OD") pinMode(pinD01, OUTPUT);
	//
    if (SenspinD02 == "ID"){
	pinMode(pinD02, INPUT);
	if (pull_up_pinD02!='N') digitalWrite(pinD02,HIGH);
    }
    else pinMode(pinD02, OUTPUT);
	//
    if (SenspinD03 == "ID"){
	pinMode(pinD03, INPUT);
	if (pull_up_pinD03!='N') digitalWrite(pinD03,HIGH);
    }
    else pinMode(pinD03, OUTPUT);
	//
    if (SenspinD04 == "ID"){
	pinMode(pinD04, INPUT);
	if (pull_up_pinD04!='N') digitalWrite(pinD04,HIGH);
    }
    else pinMode(pinD04, OUTPUT);
	//
    if (SenspinD05 == "ID") {
	pinMode(pinD05, INPUT);
	if (pull_up_pinD05!='N') digitalWrite(pinD05,HIGH);
    }		
    else pinMode(pinD05, OUTPUT);
	//
    if (SenspinD06 == "ID"){
	pinMode(pinD06, INPUT);
	if (pull_up_pinD06!='N') digitalWrite(pinD06,HIGH);
    }		
    else pinMode(pinD06, OUTPUT);
	//
    if (SenspinD07 == "ID") {
	pinMode(pinD07, INPUT);
	if (pull_up_pinD07!='N') digitalWrite(pinD07,HIGH);
    }		
    else pinMode(pinD07, OUTPUT);
	//
    if (SenspinD08 == "ID"){
	pinMode(pinD08, INPUT);
	if (pull_up_pinD08!='N') digitalWrite(pinD08,HIGH);
    }		
    else pinMode(pinD08, OUTPUT);
	//
    if (SenspinD09 == "ID") {
	pinMode(pinD09, INPUT);
	if (pull_up_pinD09!='N') digitalWrite(pinD09,HIGH);
    }		
    else pinMode(pinD09, OUTPUT);
	//
    if (SenspinD10 == "ID") {
	pinMode(pinD10, INPUT);
	if (pull_up_pinD10 !='N') digitalWrite(pinD10,HIGH);
    }		
    else pinMode(pinD10, OUTPUT);
	//
    if (SenspinD11 == "ID"){
	pinMode(pinD11, INPUT);
	if (pull_up_pinD11 !='N') digitalWrite(pinD11,HIGH);
    }		
    else pinMode(pinD11, OUTPUT);
	//
    if (SenspinD12 == "ID"){
	pinMode(pinD12, INPUT);
	if (pull_up_pinD12 !='N') digitalWrite(pinD12,HIGH);
    }		
    else pinMode(pinD12, OUTPUT);
    //
    if (SenspinD13 == "ID"){
	pinMode(pinD13, INPUT);
	if (pull_up_pinD13 !='N') digitalWrite(pinD13,HIGH);
    }		
    else pinMode(pinD13, OUTPUT);
    //
    //Traitement des entrées analogiques comme supplément d'entrées ou de sorties digitales.       
    if (SenspinD14 == "ID") pinMode(pinD14, INPUT);
    else if (SenspinD14 == "OD") pinMode(pinD14, OUTPUT);
    if (((SenspinD14 == "ID") || (SenspinD14 == "IA")) && (pull_up_pinD14 !='N')) digitalWrite(pinD14,HIGH);
    //
    if (SenspinD15 == "ID") pinMode(pinD15, INPUT);
    else if (SenspinD15 == "OD") pinMode(pinD15, OUTPUT);
    if (((SenspinD15 == "ID") || (SenspinD15 == "IA")) && (pull_up_pinD15 !='N')) digitalWrite(pinD15,HIGH);
    //
    if (SenspinD16 == "ID") pinMode(pinD16, INPUT);
    else if (SenspinD16 == "OD") pinMode(pinD16, OUTPUT);
    if (((SenspinD16 == "ID") || (SenspinD16 == "IA")) && (pull_up_pinD16 !='N')) digitalWrite(pinD16,HIGH);
    //
    if (SenspinD17 == "ID") pinMode(pinD17, INPUT);
    else if (SenspinD17 == "OD") pinMode(pinD17, OUTPUT);
    if (((SenspinD17 == "ID") || (SenspinD17 == "IA")) && (pull_up_pinD17 !='N')) digitalWrite(pinD17,HIGH);
    //
    if (SenspinD18 == "ID") pinMode(pinD18, INPUT);    
    else if (SenspinD18 == "OD") pinMode(pinD18, OUTPUT);
    if (((SenspinD18 == "ID") || (SenspinD18 == "IA")) && (pull_up_pinD18 !='N')) digitalWrite(pinD18,HIGH);
    //
    if (SenspinD19 == "ID") pinMode(pinD19, INPUT);   
    else if (SenspinD19 == "OD") pinMode(pinD19, OUTPUT);
    if (((SenspinD19 == "ID") || (SenspinD19 == "IA")) && (pull_up_pinD19 !='N')) digitalWrite(pinD19,HIGH);
    //
    if ((SenspinD20 == "ID") && (TypeBoard == "21")) pinMode(pinD20, INPUT);    
    else if ((SenspinD20 == "OD")  && (TypeBoard == "21")) pinMode(pinD20, OUTPUT);
    if (((SenspinD20 == "ID") || (SenspinD20 == "IA")) && (pull_up_pinD20 !='N') && (TypeBoard == "21")) digitalWrite(pinD20,HIGH);
    //	
    if ((SenspinD21 == "ID") && (TypeBoard == "21")) pinMode(pinD21, INPUT);    
    else if ((SenspinD21 == "OD") && (TypeBoard == "21")) pinMode(pinD21, OUTPUT);
    if (((SenspinD21 == "ID") || (SenspinD21 == "IA")) && (pull_up_pinD21 !='N') && (TypeBoard == "21")) digitalWrite(pinD21,HIGH);
    //  
    //initialisation des interruptions si le sens est PC ou PR ou PF.
    if ((SenspinD02 == "PC") || (SenspinD02 == "PR") || (SenspinD02 == "PF")){
		pinMode(pinD02, INPUT);
		compteur_pulse_0=0;
		old_time_pulse_0=millis();//init compteur de temps.
		if (SenspinD02 == "PR"){
			attachInterrupt(0, comptage_pulse_0, RISING);//vecteur interrupt 0 pour pin 2.
		}
		else if (SenspinD02 == "PF"){
			attachInterrupt(0, comptage_pulse_0, FALLING);//vecteur interrupt 0 pour pin 2.
		}
		else{
			attachInterrupt(0, comptage_pulse_0, CHANGE);//vecteur interrupt 0 pour pin 2.
		}
    }
    if ((SenspinD03 == "PC") || (SenspinD03 == "PR") || (SenspinD03 == "PF")){
		pinMode(pinD03, INPUT);
		compteur_pulse_1=0;
		old_time_pulse_1=millis();//init compteur de temps.
		if (SenspinD03 == "PR"){
			attachInterrupt(1, comptage_pulse_1, RISING);//vecteur interrupt 1 pour pin 3.
		}
		else if (SenspinD03 == "PF"){
			attachInterrupt(1, comptage_pulse_1, FALLING);//vecteur interrupt 1 pour pin 3.
		}
		else{
			attachInterrupt(1, comptage_pulse_1, CHANGE);//vecteur interrupt 1 pour pin 3.
		}
    } 
    //
    //Initialisation des sorties digitales et sorties analogiques (PWM).
    repliOutputs();
    //
    delay(300);//Attente après la ré-initialisation de l'ensemble des sorties digitales et sorties PWM.
    //
    //************Mise en service du serveur de WebSockets*********************************
    #ifdef DEBUG 
	Serial.println("setup avant ethernet");//pour TEST HH
    #endif
    
    Ethernet.begin(mac, ip);
    wsServer.registerConnectCallback(&onConnect);
    wsServer.registerDataCallback(&onData);
    wsServer.registerDisconnectCallback(&onDisconnect);  
    wsServer.begin();
    //
    delay(1000); // Attente pour mise en service du réseau Ethernet.
    #ifdef DEBUG 
	Serial.println("fin du setup");//pour TEST HH
    #endif
}
//
//********************************************************************************************
//
//******** SECTION PROGRAMME PRINCIPAL (Do not change - ne pas modifier) ***************
//
//Boucle infinie du Programme principal.
void loop() {
    int k;
    //
    // Initialisation écoute.
    wsServer.listen();
    //
    // Test des changements d'états sur les entrées digitales pour envoi vers le client WebSocket.
    if (wsServer.isConnected()){
		for (k=0; k<nb_pins; k++){
			if (Senspin[k]=="ID"){
				if ((digitalRead(k) !=vc_in_array[k]) || (indic_init_digi==1)){ 
					vc_in_array[k]=digitalRead(k); 
					//Elaboration du message.
					evtserver[0] = STX;
					strncpy(&evtserver[1], numcard, 4);
					evtserver[5] = separator;
					strncpy(&evtserver[6], "ID",2);
					evtserver[8] = separator;
					if (k < 10){
						evtserver[9]='0';		
						itoa(k,rang_char,10);
						evtserver[10]=rang_char[0];	
					}
					else{
						itoa(k,rang_char,10);
						evtserver[9]=rang_char[0];
						evtserver[10]=rang_char[1];	
					}
					evtserver[11] = separator;
					if (vc_in_array[k]==1) evtserver[12]='1';
					else evtserver[12]='0';
					evtserver[13] = ETX;
					evtserver[14] = '\0';
					//Vérification de la connexion du client.
					if (wsServer.isConnected()) {
						wsServer.send(evtserver,14);
						#ifdef DEBUG 
							//Serial.print("chaine emise ID = ");
							//Serial.println(evtserver);
						#endif
					}		    
					else{
						break;//sortie de boucle immédiat si déconnexion du client.
					}
				}
			}
		}
		// Test des changements d'états sur les sorties digitales pour envoi vers le client WebSocket.
		for (k=0; k<nb_pins; k++){
			if (Senspin[k]=="OD"){		
				if ((old_PWM_digi_array[k] !=PWM_digi_array[k]) || (indic_init_digi==1)){
					old_PWM_digi_array[k] = PWM_digi_array[k];
					//Elaboration du message.
					evtserver[0] = STX;
					strncpy(&evtserver[1], numcard, 4);
					evtserver[5] = separator;
					strncpy(&evtserver[6], "OD",2);
					evtserver[8] = separator;
					if (k < 10){
						evtserver[9]='0';		
						itoa(k,rang_char,10);
						evtserver[10]=rang_char[0];	
					}
					else{
						itoa(k,rang_char,10);
						evtserver[9]=rang_char[0];
						evtserver[10]=rang_char[1];	
					}
					evtserver[11] = separator;
					if (PWM_digi_array[k]==1) evtserver[12]='1';
					else evtserver[12]='0';
					evtserver[13] = ETX;
					evtserver[14] = '\0';
					//Vérification de la connexion du client.		    
					if (wsServer.isConnected()) {
						wsServer.send(evtserver,14);
						#ifdef DEBUG			
							//Serial.print("chaine emise OD = ");
							//Serial.println(evtserver);
						#endif
					}
					else{
						break;//sortie de boucle immédiat si déconnexion du client.
					}
				}
			}
		}
		// Envoi des sorties PWM vers le client WebSocket.
		for (k=0; k<decalia; k++){
			if (Senspin[k]=="OP"){
				if ((old_PWM_digi_array[k] !=PWM_digi_array[k]) || (indic_init_digi==1)){
					old_PWM_digi_array[k] = PWM_digi_array[k];
					//Elaboration du message.
					evtserver[0] = STX;
					strncpy(&evtserver[1], numcard, 4);
					evtserver[5] = separator;
					strncpy(&evtserver[6], "OP",2);
					evtserver[8] = separator;
					if (k < 10){
						evtserver[9]='0';		
						itoa(k,rang_char,10);
						evtserver[10]=rang_char[0];	
					}
					else{
						itoa(k,rang_char,10);
						evtserver[9]=rang_char[0];
						evtserver[10]=rang_char[1];	
					}
					evtserver[11] = separator;
					snprintf(&evtserver[12],4,"%03d",PWM_digi_array[k]);	
					evtserver[15] = ETX;
					evtserver[16] = '\0';
					//Vérification de la connexion du client.		    
					if (wsServer.isConnected()) {
					wsServer.send(evtserver,16);
					#ifdef DEBUG 
						Serial.print("chaine emise OP = ");
						Serial.println(evtserver);
					#endif			
					}
					else{
						break;//sortie de boucle immédiate si déconnexion du client.
					}
				}
			}
		}
		//
		if (indic_init_digi==1){
			indic_init_digi=0;// Fin de la procédure d'envoi des informations digitales et sorties PWM.
		}
		// Envoi des valeurs des entrées analogiques vers le client WebSocket.
		if (indic_init_ana==1){	    
			for (k=0; k<nb_pins_ana; k++){
				if (Senspin[k+decalia]=="IA"){
					//Elaboration du message.
					evtserver[0] = STX;
					strncpy(&evtserver[1], numcard, 4);
					evtserver[5] = separator;
					strncpy(&evtserver[6], "IA",2);
					evtserver[8] = separator;
					if (k < 10){
						evtserver[9]='0';		
						itoa(k,rang_char,10);
						evtserver[10]=rang_char[0];	
					}
					else{
						itoa(k,rang_char,10);
						evtserver[9]=rang_char[0];
						evtserver[10]=rang_char[1];	
					}
					evtserver[11] = separator;
					snprintf(&evtserver[12],5,"%04d",analogRead(k));		    
					evtserver[16] = ETX;
					evtserver[17] = '\0';
					//Vérification de la connexion du client.
					if (wsServer.isConnected()) {
						wsServer.send(evtserver,17);			
						#ifdef DEBUG
							//Serial.print("chaine emise IA = ");
							//Serial.println(evtserver);
						#endif			
					}
					else{
						break;//sortie de boucle immédiat si déconnexion du client.
					}
				}
			}
			// Envoi des compteurs d'impulsion
			for (k=0; k<decalia; k++){
				if ((Senspin[k] == "PC") ||(Senspin[k] == "PR")|| (Senspin[k] == "PF")) {
					//Elaboration du message.
					evtserver[0] = STX;
					strncpy(&evtserver[1], numcard, 4);
					evtserver[5] = separator;
					strncpy(&evtserver[6], "CP",2);
					evtserver[8] = separator;
					if (k < 10){
						evtserver[9]='0';		
						itoa(k,rang_char,10);
						evtserver[10]=rang_char[0];	
					}
					else{
						itoa(k,rang_char,10);
						evtserver[9]=rang_char[0];
						evtserver[10]=rang_char[1];	
					}
					evtserver[11] = separator;		    
					if (k==2) snprintf(&evtserver[12],11,"%010lu",compteur_pulse_0);
					else if (k==3) snprintf(&evtserver[12],11,"%010lu",compteur_pulse_1);
					evtserver[22] = ETX;
					evtserver[23] = '\0';
					//Vérification de la connexion du client.
					if (wsServer.isConnected()) {
						wsServer.send(evtserver,23);
						#ifdef DEBUG   
							//Serial.print("chaine emise CP = ");
							//Serial.println(evtserver);
						#endif	
					}
					else{
						break;//sortie de boucle immédiat si déconnexion du client.
					}
				}
			}	    
			indic_init_ana=0;
		}
		if (indic_ping_pong==1){
			wsServer.send("pong",4);
			indic_ping_pong=0;
		}
    }   
    else{
		//ré-armement de l'indicateur d'envoi des informations digitales et analogiques pour la prochaine connexion.
		#ifdef DEBUG 
			//Serial.println("connexion perdue");
		#endif
		indic_init_digi=1;
		indic_init_ana=1;
    }
    //
    // Sauvegarde du tick courant.
    tick_new=millis();
    //
    if (((tick_new - tick_old) <= 500) && (Watchdog==0)){
		digitalWrite(pin_watchdog, HIGH);//Watchdog
		//if (wsServer.isConnected()) wsServer.send("ping",4);
		Watchdog=1;
    }    
    else if (((tick_new - tick_old)>500) && (Watchdog==1)){
		digitalWrite(pin_watchdog, LOW);//Watchdog
		Watchdog=0;
    }    
    else if ((tick_new - tick_old)>=1000){
	    tick_old=tick_new;
	    // Armement de la demande d'envoi des entrées analogiques et des compteurs d'impulsions.
	    indic_init_ana=1;
    }    
    delay(periode);//Délai minimum de la boucle de base (effective pour opérations asybchrones: interruptions,réseaux, etc...). 
    //
    // Fin programme principal.	
}
//fin module du serveur WebSocket.

