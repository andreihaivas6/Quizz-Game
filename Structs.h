#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <time.h>

using namespace std;

typedef struct thData{
	int idThread; // id thread
	int client; // descriptorul clientului
}thData;

struct Player {
  int sd; // socket descriptor
  bool ready;
  string username;
  int score;
  vector <int> questions; // rowid pt intrebarilor puse unui player

  Player(int sd_, bool ready_, string username_) 
  : sd(sd_), ready(ready_), username(username_), score(0) { questions.clear(); }
};

struct newClient{ // un thread pentru fiecare jucator
  string username;
  thData* td; // descriptor si IDthread
  bool login = false;
  int typeUser = 0; // 1 -> obisnuit, 2-> owner;
  int idRoom = 0;
  bool ready = false;
  bool inGame = false;
  bool end = false; // finalul unui joc
  vector <Player> ranking;
};

class Room{
public:
    vector <Player> players; // [0] -> owner
    string cod;
    int idRoom;
    int indQuestion; // la a cata serie de intrebari am ajuns
    int indPlayerCurrent;  // indicele playerului care trebuie sa raspunda la intrebare
    int nrQ, nrSec;
    bool start = false;
    time_t t1; // timer intrebare
    vector <string> categories;
    vector <string> question; // categorie | intrebare | A | B | C | varianta_corecta
    

    Room() {}
    Room(int d, int idRoom_, string s, string username) : cod(s), idRoom(idRoom_) {
      addPlayer(d, username);
      players[0].ready = true;// ownerul e mereu pregatit sa dea start
      indPlayerCurrent = 0; // ownerul e primul intrat, deci raspunde primul la intrebari
      indQuestion = 0;
    }

    void addPlayer(int k, string username){
      players.push_back(Player(k, false, username));
    }

    void deletePlayer(int sd){
      for(int i = 0; i < players.size(); ++i)
        if(sd == players[i].sd)
        {
          players.erase(players.begin() + i);
          if(indPlayerCurrent > i && start == true) // reaactualizam pozitia celui care tre sa raspunda
            --indPlayerCurrent;
        }
    }

    string getPlayers(){
      string rez;
      for(int i = 0; i < players.size(); ++i)
        rez += players[i].username + (string)"\n";
      return rez;
    }

    string getStatus(){
      string rez = "Owner\n";
      for(int i = 1; i < players.size(); ++i)
        rez += (players[i].ready == true ? "Ready\n" : "Not Ready\n");
      return rez;
    }

    bool playersReady(){
      for(int i = 1; i < players.size(); ++i)
        if(players[i].ready == false)
          return false;
      return true;
    }

    void nextPlayer()
    {
      if(indPlayerCurrent < players.size() - 1)
        indPlayerCurrent++;
      else // daca au rasp toti la o serie de intrebari, 
      {    // se trece la urmatoarea
        indPlayerCurrent = 0;
        indQuestion++;
      }
    }

    void startTimer(){
      t1 = time(NULL);
    }

    time_t getStartTimer(){
      return t1;
    }
};
