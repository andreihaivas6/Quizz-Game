/*
  Model server:
  https://profs.info.uaic.ro/~computernetworks/files/NetEx/S12/ServerConcThread/servTcpConcTh2.c
*/
#include "Structs.h"

#include <iostream>
#include <ctime>
#include <chrono>
#include <vector>
#include <string>
#include <string.h>
#include <algorithm>
#include <sqlite3.h> 
#include <chrono>
#include <time.h>

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>

using namespace std;

#define PORT 2020
extern int errno;


const char* db_name = "DB.db";
vector <newClient> clients;  
vector <Room> rooms;
int totalQ = 240;
int idRoom = 0;

static void *func(void *);  // fiecare thread executa aceasta functie
string citire     (struct thData&);
void scriere      (struct thData&, string);
void request      (struct newClient&); 
void logout       (struct newClient&);
bool login        (struct newClient&);
void signUp       (struct newClient&);
void joinRoom     (struct newClient&);
void createRoom   (struct newClient&);
void refresh      (struct newClient&);
void leaveRoom    (struct newClient&);
void startGame    (struct newClient&);
void pressReady   (struct newClient&);
void leaveGame    (struct newClient&);
void submit       (struct newClient&);
void outOfTime    (struct newClient&);
void game_        (struct newClient&);
void setEnd       (struct newClient&);
void endGame      (struct newClient&);
void sendGameInfo (struct newClient&);
void getQuestion  (struct newClient&);
void updateDB     (struct newClient&, string);
vector <string> splitToWords(string);
static int callback_login       (void*, int, char**, char**);
static int callback_getQuestion (void*, int, char**, char**);


int main (int argc, char **argv)
{
  struct sockaddr_in server;	// structura folosita de server
  struct sockaddr_in from;	
  int sd;		// socket

  pthread_t clientsTh[100];    // clienti

  if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1){  // deschidere socket
    perror ("[server] Eroare socket.\n");
    return errno;
  }

  int var = 1;
  setsockopt(sd, SOL_SOCKET,SO_REUSEADDR, &var, sizeof(var));

  bzero (&server, sizeof (server));
  bzero (&from, sizeof (from));
  
  server.sin_family = AF_INET;	// familie socket
  server.sin_addr.s_addr = htonl (INADDR_ANY); // primim orice adresa
  server.sin_port = htons (PORT); // port utilizator
  
  if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1){ // atasam socketul 
    perror ("[server] Eroare bind.\n");
    return errno;
  }

  if (listen (sd, 2) == -1){ // asculta pentru clientii ce vor sa se conecteze
    perror ("[server] Eroare listen.\n");
    return errno;
  }
  printf ("[server] Asteptam la portul %d...\n",PORT);

  int idCl = 0;
  do{ // acceptam clientii si le cream cate un thread fiecaruia
    int client;
    unsigned int length = sizeof(from);

    if ((client = accept (sd, (struct sockaddr *) &from, &length)) < 0){ // acceptam un client (stare blocanta)
      perror ("[server]Eroare accept.\n");
      continue;
    }

    newClient t;
    t.td = (struct thData*)malloc(sizeof(struct thData));	
    t.td->idThread = ++idCl;
    t.td->client = client;
    
    clients.push_back(t); 
    pthread_create(&clientsTh[idCl], NULL, &func, &clients[clients.size()- 1]);	      	
	}while (true);

  return 0;
}				

static void *func(void * arg)
{		
	struct newClient th = *((struct newClient*)arg);	
	printf ("[Thread %d] S-a realizat conexiunea cu thread-ul.\n", th.td->idThread);
	pthread_detach(pthread_self());	

  do{
    for(int i = 0; i < clients.size(); ++i)   // in cazul in care ownerul incepe jocul
      if(th.td->client == clients[i].td->client)
        th.inGame = clients[i].inGame;

    string id = citire(*th.td);
    if(th.end == true && id == "wait")
      endGame(th);
    else if(id == "request") // playerii afla daca ownerul a inceput jocul
      request(th);
    else if(id == "logout")
      logout(th);
    else if(th.login == false) // e in fereastra de login
    {
      if(id == "login")
        th.login = login(th);
      else if(id == "sign up")
        signUp(th);
      else if(id == "quit")
        break;
    }
    else if(th.idRoom == 0) // e in menu
    {
      if(id == "join")
        joinRoom(th);
      else if(id == "create")
        createRoom(th);
      else if(id == "quit"){
        logout(th); break; 
      }
    }
    else if(th.inGame == false) // e in camera
    {
      if(id == "refresh")
        refresh(th);
      else if(id == "leave room")
        leaveRoom(th);
      else if(id == "start")
        startGame(th);
      else if(id == "ready")
        pressReady(th);
      else if(id == "quit"){
        leaveRoom(th); logout(th); break;
      }
      else { // MARE EROARE DACA NU
        th.idRoom = 0; // !!!
        if(id == "join") joinRoom(th);
        else if(id == "create") createRoom(th);
        else if(id == "logout") logout(th);
      }
    }
    else if(th.inGame == true)  // e in joc
    {
      for(int i = 0; i < clients.size(); ++i) // verificam daca s-a terminat jocul
        if(th.td->client == clients[i].td->client)
          if(clients[i].end == true)  
            endGame(th);

      if(id == "time") // verificam daca s-a scurs timpul de raspuns la intrebare
        outOfTime(th);
      else if(id == "leave game")
        leaveGame(th);
      else if(id == "quit"){
        leaveGame(th); logout(th); break;
      }
      else if(id == "wait")
        game_(th);
      else if(id == "submit")
        submit(th); 
    }
    else if(id == "quit")
      break; // n-ar trebui sa ajunga aiki
  }while(true);

	close((intptr_t)arg); // inchidem conexiunea cu clientul
  for(int i = 0; i < clients.size(); i++)
    if(clients[i].td->idThread == th.td->idThread)
      clients.erase(clients.begin() + i);   // stergem clientul care pleaca
	return 0;	
};

void request(struct newClient& th) // ceilalti clienti afla daca ownerul a inceput jocul
{
  bool ok = false;
  for(int i = 0; i < rooms.size(); ++i)
    if(th.idRoom == rooms[i].idRoom && rooms[i].start == true)
      ok = true;
  scriere(*th.td, ((ok == true) ? "OK" : "NOT OK"));
}

void logout(struct newClient& th)
{
  th.login = false;
  th.username.clear();
  for(int i = 0; i < clients.size(); ++i)
    if(clients[i].td->client == th.td->client)
      clients[i].username.clear();
}

bool login(struct newClient& th)
{
  string user, pass;
  user = citire(*th.td);
  pass = citire(*th.td);

  sqlite3 *db;
  sqlite3_open(db_name, &db);
  char mesaj[1024], rasp[1024];// in rasp o sa punem "nr_wins/nr_games" 
  sprintf(mesaj, "SELECT * FROM accounts WHERE username = '%s' AND password = '%s';", 
    user.c_str(), pass.c_str());
  bzero(rasp, 1023);
  sqlite3_exec(db, mesaj, callback_login, rasp, NULL);
  sqlite3_close(db);
  
  if(strcmp(rasp, "") != 0)
  {
    for(int i = 0; i < clients.size(); ++i) // verificam daca e deja conectat
      if(clients[i].username == user){
        scriere(*th.td, "This account is already connected.");
        return false;
      }

    th.username = user;
    for(int i = 0; i < clients.size(); ++i)
      if(clients[i].td->client == th.td->client)
        clients[i].username = user;
    scriere(*th.td, "OK");
    scriere(*th.td, rasp);
    return true;
  }
  else
  {
    scriere(*th.td, "This account doesn't exist.");
    return false;
  }
}

void signUp(struct newClient& th)
{
  string user, pass;
  user = citire(*th.td);
  pass = citire(*th.td);
  
  if(pass.length() < 4)
    scriere(*th.td, "This password is too weak.");
  else
  {
    sqlite3 *db;
    sqlite3_open(db_name, &db);
    char mesaj[1024];
    sprintf(mesaj, "INSERT INTO accounts(username, password) VALUES('%s', '%s');",
    user.c_str(), pass.c_str());

    if(sqlite3_exec(db, mesaj, NULL, NULL, NULL) != SQLITE_OK) // username -> PRIMARY KEY
      scriere(*th.td, "This username already exists.");
    else
      scriere(*th.td, "Account succesfully created.");
    sqlite3_close(db);
  }
}

static int callback_login(void *data, int nrCol, char **col, char **colName)
{
  char* rasp = (char*)data;
  sprintf(rasp, "%s/%s", col[2], col[3]);
  return 1;
}

void joinRoom(struct newClient& th)
{
  string cod = citire(*th.td);
  if(cod == ""){
    scriere(*th.td, "You must introduce a code.");
    return;
  }
  if(rooms.size() == 0){
    scriere(*th.td, "There is no room created.");
    return;
  }

  for(int i = 0; i < rooms.size(); ++i) 
    if(rooms[i].cod == cod)
    {
      if(rooms[i].start == true){
        scriere(*th.td, "The game has already started.");
        return;
      }
      rooms[i].addPlayer(th.td->client, th.username);
      th.typeUser = 1;
      th.idRoom = rooms[i].idRoom;
      for(int i = 0; i < clients.size(); ++i)
        if(clients[i].td->client == th.td->client){
          clients[i].idRoom = th.idRoom;
          clients[i].ready = false;
        }
      
      scriere(*th.td, "OK");
      scriere(*th.td, (string)"Room #" + to_string(th.idRoom) + (string)" (" + cod + (string)")");
      return;
    }
  scriere(*th.td, "There is no room with this code.");
}

void createRoom(struct newClient& th)
{
  string cod = citire(*th.td);
  if(cod.length() < 4){
    scriere(*th.td, "Code must have at least 4 characters.");
    return;
  }
  for(char c : cod)
    if( !( (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ) ){
      scriere(*th.td, "Try using only letters or digits.");
      return;
    }
  for(int i = 0; i < rooms.size(); ++i)
    if(rooms[i].cod == cod){
      scriere(*th.td, "This code is already used.");
      return;
    }
  
  rooms.push_back(Room(th.td->client, ++idRoom, cod, th.username));
  th.typeUser = 2;
  th.idRoom = idRoom;
  for(int i = 0; i < clients.size(); ++i)
    if(clients[i].td->client == th.td->client)
      clients[i].idRoom = idRoom;
      
  scriere(*th.td, "OK");
  scriere(*th.td, (string)"Room #" + to_string(idRoom) + (string)" (" + cod + (string)")");
}

void refresh(struct newClient& th) // daca vin playeri noi in camera
{
  for(int i = 0; i < rooms.size(); ++i) // cautam camera noastra
    if(rooms[i].idRoom == th.idRoom)
    {
      scriere(*th.td, rooms[i].getPlayers());
      scriere(*th.td, rooms[i].getStatus());
      return;
    }
  scriere(*th.td, ""); // daca s-a inchis camera de catre owner
  scriere(*th.td, "");
}

void leaveRoom(struct newClient& th)
{
  if(th.typeUser == 2)  // daca ownerul camerei pleaca, se cauta toti clientii din acea camera
  {
    for(int i = 0; i < clients.size(); ++i) //si le spunem ca nu mai fac parte dintr-o camera
      if(clients[i].idRoom == th.idRoom) // !!!!!!!!!!!!!!!!!!!!!!!!!!!
      {
        clients[i].typeUser = 0;
        clients[i].idRoom = 0;
      }

    for(int i = 0; i < rooms.size(); ++i) // eliminam camera
      if(rooms[i].idRoom == th.idRoom)
        rooms.erase(rooms.begin() + i);
    th.typeUser = 0;
    th.idRoom = 0;
  }
  else // player normal
  {
    for(int i = 0; i < rooms.size(); ++i) // cautam camera userului ca sa il scoatem din camera
      if(rooms[i].idRoom == th.idRoom)
        rooms[i].deletePlayer(th.td->client);
    th.typeUser = 0;
    th.idRoom = 0;
  }
}

void startGame(struct newClient& th)
{
  int ind;
  for(int i = 0; i < rooms.size(); ++i) // cautam indicele camerei
    if(rooms[i].idRoom == th.idRoom)
      ind = i;

  if(rooms[ind].players.size() == 1) 
    scriere(*th.td, "You need one more player to start the game.");
  else
  {
    scriere(*th.td, "OK");
    if(rooms[ind].playersReady() == false)
      scriere(*th.td, "All players must be 'ready' to play.");
    else
    {
      scriere(*th.td, "OK");
      vector <string> categories = splitToWords(citire(*th.td));
      if(categories.size() < 3)
        scriere(*th.td, "You must choose at least 3 categories.");
      else
      {
        scriere(*th.td, "OK");
        rooms[ind].categories = categories; // luam informatiile stabilite de owner
        rooms[ind].nrSec = stoi(citire(*th.td));
        rooms[ind].nrQ = stoi(citire(*th.td));
        rooms[ind].start = true;

        // setam pentru toti cei din camera idGame = true
        for(int i = 0; i < rooms[ind].players.size(); ++i)
          for(int j = 0; j < clients.size(); ++j)
            if(rooms[ind].players[i].sd == clients[j].td->client)
              clients[j].inGame = true;
        
        for(int i = 0; i < rooms[ind].players.size(); ++i) // reinitializam informatiile despre jucatori
        {
          rooms[ind].players[i].questions.clear();
          rooms[ind].players[i].score = 0;
        }
        getQuestion(th);
        rooms[ind].startTimer();
        th.inGame = true;
      }
    }
  }
}

void pressReady(struct newClient& th)
{
  th.ready = !th.ready;
  if(th.ready == true)
    scriere(*th.td, "You are READY to play.");
  else
    scriere(*th.td, "You are NOT READY to play.");
  for(int i = 0; i < rooms.size(); ++i)  // cautam playerul in camere
    if(th.idRoom == rooms[i].idRoom)
      for(int j = 0; j < rooms[i].players.size(); ++j)
        if(th.td->client == rooms[i].players[j].sd)
          rooms[i].players[j].ready = th.ready;
}

void leaveGame(struct newClient& th)
{ 
  updateDB(th, "");
  for(int i = 0; i < rooms.size(); ++i)  // stergem din camera
    if(th.idRoom == rooms[i].idRoom)
    {
      rooms[i].deletePlayer(th.td->client);
      if(rooms[i].players.size() == 0) // daca iese si ultimul jucator, stergem camera
        rooms.erase(rooms.begin() + i);
      else // daca pleaca playerul care trebuie sa raspunda la intrebare
        if(rooms[i].players[rooms[i].indPlayerCurrent].sd == th.td->client){
          rooms[i].nextPlayer();
          getQuestion(th);
          rooms[i].startTimer();
        }
    }

  for(int i = 0; i < clients.size(); ++i)
    if(clients[i].td->client == th.td->client)  // nu mai face parte din joc sau camera
    {
      clients[i].inGame = th.inGame = false;
      clients[i].ready  = th.ready  = false;
      clients[i].idRoom = th.idRoom = 0;
      clients[i].typeUser = th.typeUser = 0;
    }
  
}

void game_(struct newClient& th)
{
  for(int i = 0; i < rooms.size(); ++i)
    if(th.idRoom == rooms[i].idRoom)
    {
      if(th.td->client == rooms[i].players[rooms[i].indPlayerCurrent].sd)
      {
        scriere(*th.td, "OK");
        sendGameInfo(th);
      }
      else
        scriere(*th.td, "NOT OK");
      break;
    }
}

void getQuestion(struct newClient& th)
{
  srand(time(NULL));
  char rez[1024];
  vector <string> categories; // categoriile disponibile
  for(int i = 0; i < rooms.size(); ++i)
    if(th.idRoom == rooms[i].idRoom)
      categories = rooms[i].categories;

  sqlite3 *db;
  if(sqlite3_open(db_name, &db) != 0)
    cout << "EROARE CU BD";

  do{ 
    // cautam intrebari pana gasim una din categoriile selectate si nu a mai fost pusa acestui player
    int rowid = rand() % totalQ + 1;
    char mesaj[1024];
    bzero(mesaj, 1023);
    sprintf(mesaj, "SELECT * FROM questions WHERE rowid = %d;", rowid);
    bzero(rez, 1023);
    if(sqlite3_exec(db, mesaj, callback_getQuestion, rez, NULL) != SQLITE_OK)
      cout << sqlite3_errmsg(db);
    vector <string> question = splitToWords(rez);

    for(int i = 0; i < categories.size(); ++i)
      if(question[0] == categories[i]) // sa fie intrebarea in categoriile selectate de owner
      {
        // verificam daca a mai fost pusa aceasta intrebare acestui player
        bool ok = true;
        for(int i = 0; i < rooms.size(); ++i)
          if(th.idRoom == rooms[i].idRoom) //            ^
            for(int j = 0; j < rooms[i].players[rooms[i].indPlayerCurrent].questions.size(); ++j)
              if(rooms[i].players[rooms[i].indPlayerCurrent].questions[j] == rowid)
                ok = false;
        
        if(ok == true)
        {
          for(int i = 0; i < rooms.size(); ++i) // adaugam intrebarea care e pusa acum
            if(th.idRoom == rooms[i].idRoom)
              rooms[i].players[rooms[i].indPlayerCurrent].questions.push_back(rowid); 
          
          for(int i = 0; i < rooms.size(); ++i) // setam intrebarea
            if(th.idRoom == rooms[i].idRoom)
              rooms[i].question = question;

          sqlite3_close(db);
          return;
        }
      }
  }while(true);
}

static int callback_getQuestion(void *data, int nrCol, char **col, char **colName)
{
  char* rasp = (char*)data;
  sprintf(rasp, "%s\n%s\n%s\n%s\n%s\n%s\n", col[5], col[0], col[1], col[2], col[3], col[4]);
  return 1;
}

void sendGameInfo(struct newClient& th)
{
  char mesaj[1024];
  for(int i = 0; i < rooms.size(); ++i)
    if(th.idRoom == rooms[i].idRoom)
    {
      auto question = rooms[i].question; // question -> categorie | intrebare | A | B | C | varianta_corecta
      string s = question[0] + (string)" ---> " + question[1]; // categ + intrebare
      for(int i = 50; i < s.length(); i = i + 50) // sa incapa in label cand e afisata
      {
        while(s[i] != ' ')
          i--;
        s.insert(s.begin() + i + 1, '\n'); 
      }

      scriere(*th.td, s);  // s
      scriere(*th.td, question[2]); // A
      scriere(*th.td, question[3]); // B
      scriere(*th.td, question[4]); // C

      bzero(mesaj, 1023);
      if(th.td->client == rooms[i].players[rooms[i].indPlayerCurrent].sd)
        sprintf(mesaj, "It's your turn.");
      else
        sprintf(mesaj, "It is %s's turn.", rooms[i].players[rooms[i].indPlayerCurrent].username.c_str());
      scriere(*th.td, mesaj); // numele celui care raspunde

      bzero(mesaj, 1023);
      sprintf(mesaj, "Question: %d/%d", rooms[i].indQuestion + 1, rooms[i].nrQ);
      scriere(*th.td, mesaj); // "nr intrebari parcurse / nr total de intrebari"

      scriere(*th.td, to_string(rooms[i].nrSec)); // timp raspuns 

      bzero(mesaj, 1023);
      sprintf(mesaj, "Score: %d", rooms[i].players[rooms[i].indPlayerCurrent].score);
      scriere(*th.td, mesaj); // scor
      return;
    }
}

void submit(struct newClient& th)
{
  string correct;
  for(int i = 0; i < rooms.size(); ++i)
    if(th.idRoom == rooms[i].idRoom)
      correct = rooms[i].question[5];
  int pct;
  if(citire(*th.td) == correct)
    pct = 5;
  else
    pct = -2;
  
  for(int i = 0; i < rooms.size(); ++i)
    if(th.idRoom == rooms[i].idRoom)
    {
      rooms[i].players[rooms[i].indPlayerCurrent].score += pct;
      scriere(*th.td, "Score: " + to_string(rooms[i].players[rooms[i].indPlayerCurrent].score));
      if(pct == 5)
        scriere(*th.td, "Correct answear. You won 5 points.");
      else
        scriere(*th.td, "Wrong answear. You lost 2 points.");
      
      rooms[i].nextPlayer();
      getQuestion(th);
      rooms[i].startTimer();
      if(rooms[i].indQuestion == rooms[i].nrQ)
        setEnd(th); // stabilim finalul jocului
      break;
    }
}

void outOfTime(struct newClient& th)
{
  for(int i = 0; i < rooms.size(); ++i)
    if(th.idRoom == rooms[i].idRoom)
    {
      time_t t1 = rooms[i].getStartTimer();
      time_t t2 = time(NULL);
      double timeElapsed = t2 - t1; // timpul de cand a primit intrebarea pana in prezent
      if(timeElapsed >= rooms[i].nrSec) // daca a depasit timpul limita
      {
        scriere(*th.td, "time elapsed");
        scriere(*th.td, "Score: " + to_string(rooms[i].players[rooms[i].indPlayerCurrent].score));
        scriere(*th.td, "You didn't lose any points for not answearing");

        rooms[i].nextPlayer();
        getQuestion(th);
        rooms[i].startTimer();
        if(rooms[i].indQuestion == rooms[i].nrQ)
          setEnd(th);
       }
      else
      {
        scriere(*th.td, "OK");
        scriere(*th.td, to_string(rooms[i].nrSec - (int)timeElapsed));
      }
    }
}

void setEnd(struct newClient& th) // golim structurile de date si salvam scorurile
{
  int id = th.idRoom;
  vector <Player> ranking;
  for(int i = 0; i < rooms.size(); ++i)
    if(th.idRoom == rooms[i].idRoom)
    {
      ranking = rooms[i].players;      // luam detaliile despre scor pt ranking
      rooms.erase(rooms.begin() + i);  // stergem camera
    }
  sort(ranking.begin(), ranking.end(),
  [](Player a, Player b) -> bool { return a.score > b.score; }); 

  
  for(int i = 0; i < clients.size(); ++i)
    if(clients[i].idRoom == id)  // reinitializam playerii pentru menu
    {
      th.end = clients[i].end = true;
      th.ranking = clients[i].ranking = ranking;
      th.idRoom = clients[i].idRoom = 0;
      th.inGame = th.ready = clients[i].inGame = clients[i].ready = false;
      th.typeUser = clients[i].typeUser = 0;
    }
}

void endGame(struct newClient& th)
{  
  scriere(*th.td, "end");
  vector <Player> ranking;
  for(int i = 0; i < clients.size(); ++i) // luam rankingul
    if(th.td->client == clients[i].td->client)
      ranking = clients[i].ranking; 
   
  // curatam datele despre client
  for(int i = 0; i < clients.size(); ++i) 
    if(th.td->client == clients[i].td->client)
    {
      clients[i].end = false;
      clients[i].ranking.clear();
    }
  th.ranking.clear();
  th.idRoom = 0;
  th.typeUser = 0;
  th.inGame = th.end = th.ready = false;
 
  string s1 = "", s2 = "";
  for(int i = 0; i < ranking.size(); ++i)
  {
    s1 += ranking[i].username + (string)"\n";
    s2 += to_string(ranking[i].score) + (string)"\n";
  }
  
  updateDB(th, ranking[0].username);
  scriere(*th.td, ranking[0].username); // winner
  scriere(*th.td, s1);  // rank playeri
  scriere(*th.td, s2);  // rank scor
}

void updateDB(struct newClient& th, string winner)
{
  sqlite3 *db;
  sqlite3_open(db_name, &db);

  char mesaj[1024];
  bzero(mesaj, 1023);
  sprintf(mesaj, "UPDATE accounts SET games = 1 + (SELECT games FROM accounts WHERE username = '%s') WHERE username = '%s';", 
  th.username.c_str(), th.username.c_str());
  sqlite3_exec(db, mesaj, NULL, NULL, NULL);

  int win = (th.username == winner);
  bzero(mesaj, 1023);
  sprintf(mesaj, "UPDATE accounts SET wins = %d + (SELECT wins FROM accounts WHERE username = '%s') WHERE username = '%s';", 
  win, th.username.c_str(), th.username.c_str());
  sqlite3_exec(db, mesaj, NULL, NULL, NULL);

  sqlite3_close(db);
}

vector <string> splitToWords(string s)
{
  vector <string> rez;
  string cuv;
  for(char c : s)
    if(c == '\n')
    {
      if(cuv != "")
        rez.push_back(cuv);
      cuv.clear();
    }
    else
      cuv += c;
  return rez;
}

string citire(struct thData& td)
{
  char mesaj[1024];
  int lung;
  bzero(mesaj, 1023);
  read(td.client, &lung, sizeof(int));
  read(td.client, mesaj, lung);
  cout << "[Thread " << td.idThread << "] Am primit mesaj... " << mesaj << "\n";
  return mesaj;
}

void scriere(struct thData& td, string s)
{
  char mesaj[1024];
  bzero(mesaj, 1023);
  strcpy(mesaj, s.c_str());
  int lung = strlen(mesaj) + 1;
  cout << "[Thread " << td.idThread << "] Trimitem mesaj... " << s << "\n";
  write(td.client, &lung, sizeof(int));
  write(td.client, mesaj, lung);
}
