/* 
  Model de client:
   https://profs.info.uaic.ro/~computernetworks/files/NetEx/S12/ServerConcThread/cliTcpNr.c 
*/
#include <iostream>
#include <arpa/inet.h>
#include <string>
#include <vector>
#include <string.h>
#include <chrono>
#include <time.h>

#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/socket.h>
#include <string.h>

#include "declarations.h"

using namespace std;

extern int errno;

void scriere(string);
string citire();
void quit(); // inchidere fereastra
void event3();
void event4();
void event5();
void event6();
void swapWindow(GtkWidget *, GtkWidget *);

int sd, port;
bool w3 = false, w4 = false, w5 = false, w6 = false; // cv eroare

int main (int argc, char **argv)
{
  struct sockaddr_in server;	// structura folosita pentru conectare 

  if (argc != 3){
    printf ("Sintaxa gresita.\n");
    return -1;
  }

  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1){ // deschidem socket
    perror ("Eroare socket.\n");
    return errno;
  }

  port = atoi(argv[2]); // port
  server.sin_family = AF_INET; // tip socket
  server.sin_addr.s_addr = inet_addr(argv[1]); // adresa IP a serverului
  server.sin_port = htons (port); // portul de conectare la server
  
  if (connect (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1){ // conectare la server
    perror ("[client]Eroare connect.\n");
    return errno;
  }
  
  cout << "Conectat cu succes la server.\n";

  gtk_init(&argc, &argv);
  builder = gtk_builder_new_from_file("client.glade");
  gtk_builder_connect_signals(builder, NULL);

  declarations();

  g_signal_connect(window3, "event", G_CALLBACK (event3), NULL);
  g_signal_connect(window4, "event", G_CALLBACK (event4), NULL);
  g_signal_connect(window5, "event", G_CALLBACK (event5), NULL);
  g_signal_connect(window6, "event", G_CALLBACK (event6), NULL);
  g_signal_connect(window1, "destroy", G_CALLBACK(quit), NULL);
  g_signal_connect(window2, "destroy", G_CALLBACK(quit), NULL);
  g_signal_connect(window3, "destroy", G_CALLBACK(quit), NULL);
  g_signal_connect(window4, "destroy", G_CALLBACK(quit), NULL);
  g_signal_connect(window5, "destroy", G_CALLBACK(quit), NULL);
  g_signal_connect(window6, "destroy", G_CALLBACK(quit), NULL);

  gtk_widget_show(window1);

  gtk_main();

  return EXIT_SUCCESS;
}

void scriere(string s)
{
  char mesaj[1024];
  bzero(mesaj, 1023);
  strcpy(mesaj, s.c_str());
  int lung = strlen(mesaj) + 1;
  write(sd, &lung, sizeof(int));
  write(sd, &mesaj, lung);
  cout << "\n[client] Am trimis catre server... " << mesaj << "\n";
}

string citire()
{
  int lung;
  char mesaj[1024];
  bzero(mesaj, 1023);
  read(sd, &lung, sizeof(int));
  read(sd, &mesaj, lung);
  cout << "\n[client] Mesaj primit de la server... " << mesaj << "\n"; fflush(stdout);
  return mesaj;
}

void quit() // orice tentativa de inchidere a jocului
{
  scriere("quit");
  close(sd);
  exit(0);
}

void event3(){
  if(w3 == true)
    gtk_button_clicked(GTK_BUTTON(button10)); // refresh
}

void event4()
{
  if(w4 == true)
  {
    gtk_button_clicked(GTK_BUTTON(button12)); // refresh
    scriere("request");
    if(citire() == "OK")
    {
      w4 = false; w6 = true;
      swapWindow(window4, window6);
    }
  }
}

void event5() // fereastra celui care raspunde
{
  if(w5 == true)
  {
    scriere("time");
    if(citire() == "OK")
      gtk_label_set_text(GTK_LABEL(label29), (citire() + (string)"s").c_str()); // timp
    else // a trecut timpul
    {
      swapWindow(window5, window6);
      w5 = false; w6 = true;
      gtk_label_set_text(GTK_LABEL(label38), citire().c_str()); // score
      gtk_label_set_text(GTK_LABEL(label39), citire().c_str()); // tip raspuns 
    }
  }
}

void event6() // fereastra celor care asteapta
{
  if(w6 == true)
  {
    scriere("wait");
    string raspuns = citire();
    if(raspuns == "OK") // verifica daca a venit randul lui sa raspunda
    {
      swapWindow(window6, window5);
      w6 = false; w5 = true;
      gtk_label_set_text(GTK_LABEL(label27), citire().c_str()); // intrebare
      gtk_label_set_text(GTK_LABEL(label31), citire().c_str()); // A
      gtk_label_set_text(GTK_LABEL(label32), citire().c_str()); // B
      gtk_label_set_text(GTK_LABEL(label33), citire().c_str()); // C
      gtk_label_set_text(GTK_LABEL(label28), citire().c_str()); // player care raspunde
      gtk_label_set_text(GTK_LABEL(label30), citire().c_str()); // nr intrebari
      gtk_label_set_text(GTK_LABEL(label29), citire().c_str()); // timp
      gtk_label_set_text(GTK_LABEL(label34), citire().c_str()); // scor
    }
    else if(raspuns == "end") // verifica  daca s-a ajuns la finalul jocului
    {
      swapWindow(window6, window7);
      w6 = false;
      gtk_label_set_text(GTK_LABEL(label41), citire().c_str()); // winner
      gtk_label_set_text(GTK_LABEL(label42), citire().c_str()); // playeri
      gtk_label_set_text(GTK_LABEL(label43), citire().c_str()); // scor
    }
  }
}

void swapWindow(GtkWidget *w1, GtkWidget *w2)
{
  gint x, y;
  gtk_window_get_position(GTK_WINDOW(w1), &x, &y);
  gtk_widget_hide(w1);
  gtk_widget_show(w2);
  gtk_window_move(GTK_WINDOW(w2), x, y);
}

void on_button1_clicked(GtkButton *b)  // login
{
  scriere("login");
  scriere(gtk_entry_get_text(GTK_ENTRY(entry1))); // user
  scriere(gtk_entry_get_text(GTK_ENTRY(entry2))); // pass
  string raspuns = citire();
  if(raspuns == "OK")
  {
    string hello = (string)"Hello, " + gtk_entry_get_text(GTK_ENTRY(entry1)) + (string)"!";
    gtk_label_set_text(GTK_LABEL(label11), hello.c_str());
    swapWindow(window1, window2);
    raspuns = "Wins: " + citire();
    gtk_label_set_text(GTK_LABEL(label10), raspuns.c_str());
    gtk_entry_set_text(GTK_ENTRY(entry3), "");
    gtk_label_set_text(GTK_LABEL(label6), "");
  }
  else
    gtk_label_set_text(GTK_LABEL(label5), raspuns.c_str());
}

void on_button2_clicked(GtkButton *b)  // sign up
{
  scriere("sign up");
  scriere(gtk_entry_get_text(GTK_ENTRY(entry1))); // user
  scriere(gtk_entry_get_text(GTK_ENTRY(entry2))); // pass
  gtk_label_set_text(GTK_LABEL(label5), citire().c_str());
}

void on_button3_clicked(GtkButton *b){ // exit
  quit();
}

void on_button6_clicked(GtkButton *b){ // exit
  quit();
}

void on_button7_clicked(GtkButton *b) // logout
{
  scriere("logout");
  swapWindow(window2, window1);
  gtk_label_set_text(GTK_LABEL(label5), "You have been disconnected.");
  gtk_entry_set_text(GTK_ENTRY(entry1), "");
  gtk_entry_set_text(GTK_ENTRY(entry2), "");
}

void on_button4_clicked(GtkButton *b) // join room
{
  scriere("join");
  scriere(gtk_entry_get_text(GTK_ENTRY(entry3)));
  string raspuns = citire();
  if(raspuns == "OK")
  {
    swapWindow(window2, window4);
    w4 = true;
    gtk_label_set_text(GTK_LABEL(label21), citire().c_str()); // luam codu camerei
    on_button12_clicked(b); // refresh player pt informatii
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle1), FALSE);
  }
  else
    gtk_label_set_text(GTK_LABEL(label6), raspuns.c_str());
}

void on_button5_clicked(GtkButton *b) // create room
{
  scriere("create");
  scriere(gtk_entry_get_text(GTK_ENTRY(entry3)));
  string raspuns = citire();
  if(raspuns == "OK")
  {
    swapWindow(window2, window3);
    w3 = true;
    gtk_label_set_text(GTK_LABEL(label12), citire().c_str()); // luam codu camerei
    gtk_label_set_text(GTK_LABEL(label18), gtk_entry_get_text(GTK_ENTRY(entry1)));
    gtk_label_set_text(GTK_LABEL(label19), "Owner");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check1), TRUE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check2), TRUE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check3), TRUE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check4), TRUE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check5), TRUE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check6), TRUE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check7), TRUE);
  }
  else
    gtk_label_set_text(GTK_LABEL(label6), raspuns.c_str());
}

void on_button8_clicked(GtkButton *b) // leave room owner
{
  scriere("leave room");
  swapWindow(window3, window2);
  gtk_label_set_text(GTK_LABEL(label6), "Your room has been closed.");
  w3 = false;
}

void on_button11_clicked(GtkButton *b) // leave room player normal
{
  scriere("leave room");
  swapWindow(window4, window2);
  gtk_label_set_text(GTK_LABEL(label6), "You are no longer part of that room.");
  w4 = false; 
}

void on_button9_clicked(GtkButton *b) // start game
{
  on_button10_clicked(b); // refresh
  scriere("start");
  string raspuns = citire();
  if(raspuns != "OK"){
    gtk_label_set_text(GTK_LABEL(label17), raspuns.c_str());
    return;
  }
  raspuns = citire();
  if(raspuns != "OK"){
    gtk_label_set_text(GTK_LABEL(label17), raspuns.c_str());
    return;
  }

  string categ;
  if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check1)) == TRUE) categ += "Diverse\n";
  if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check2)) == TRUE) categ += "Geografie\n";
  if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check3)) == TRUE) categ += "Istorie\n";
  if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check4)) == TRUE) categ += "Arta\n";
  if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check5)) == TRUE) categ += "Stiinte\n";
  if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check6)) == TRUE) categ += "Sport\n";
  if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check7)) == TRUE) categ += "Filme\n";
  scriere(categ);
  raspuns = citire();
  if(raspuns != "OK"){
    gtk_label_set_text(GTK_LABEL(label17), raspuns.c_str());
    return;
  }

  scriere (to_string (gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (spin1) ) ) ); // nr secunde
  scriere (to_string (gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (spin2) ) ) ); // nr intrebari

  scriere("wait");
  if(citire() == "OK")
  {
    swapWindow(window3, window5);
    w3 = false; w5 = true;
    gtk_label_set_text(GTK_LABEL(label27), citire().c_str()); // intrebare
    gtk_label_set_text(GTK_LABEL(label31), citire().c_str()); // A
    gtk_label_set_text(GTK_LABEL(label32), citire().c_str()); // B
    gtk_label_set_text(GTK_LABEL(label33), citire().c_str()); // C
    gtk_label_set_text(GTK_LABEL(label28), citire().c_str()); // player care raspunde
    gtk_label_set_text(GTK_LABEL(label30), citire().c_str()); // nr intrebari
    gtk_label_set_text(GTK_LABEL(label29), citire().c_str()); // timp
    gtk_label_set_text(GTK_LABEL(label34), citire().c_str()); // scor
  }
}

void on_button10_clicked(GtkButton *b) // refresh owner
{
  scriere("refresh");
  gtk_label_set_text(GTK_LABEL(label18), citire().c_str());
  gtk_label_set_text(GTK_LABEL(label19), citire().c_str());
}

void on_button12_clicked(GtkButton *b) // refresh player normal
{
  scriere("refresh");
  gtk_label_set_text(GTK_LABEL(label23), citire().c_str()); // toti playerii din camera
  gtk_label_set_text(GTK_LABEL(label24), citire().c_str());

  if((string)gtk_label_get_text(GTK_LABEL(label23)) == "") { // nu avem playeri cand camera e inchisa
    swapWindow(window4, window2);
    gtk_label_set_text(GTK_LABEL(label6), "The room has been closed.");
    w4 = false;
  }
}

void on_toggle1_toggled(GtkButton *b) // ready
{
  scriere("ready");
  gtk_label_set_text(GTK_LABEL(label22), citire().c_str());
  on_button12_clicked(b);  //refresh
}

void on_button13_clicked(GtkButton *b) // leave game
{
  scriere("leave game");
  swapWindow(window5, window2);
  gtk_label_set_text(GTK_LABEL(label6), "You are no longer part of that room.");
  w5 = false; 
}

void on_button14_clicked(GtkButton *b) // submit
{
  scriere("submit");
  if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio1)) == TRUE) scriere("A");
  if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio2)) == TRUE) scriere("B");
  if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio3)) == TRUE) scriere("C");
  swapWindow(window5, window6);
  w5 = false; w6 = true;
  gtk_label_set_text(GTK_LABEL(label38), citire().c_str()); // score
  gtk_label_set_text(GTK_LABEL(label39), citire().c_str()); // tip raspuns
}

void on_button15_clicked(GtkButton *b) // leave game
{
  scriere("leave game");
  swapWindow(window6, window2);
  gtk_label_set_text(GTK_LABEL(label6), "You are no longer part of that room.");
  w6 = false; 
}

void on_button16_clicked(GtkButton *b) // back to menu din ranking
{
  gtk_label_set_text(GTK_LABEL(label38), "Score: 0");
  gtk_label_set_text(GTK_LABEL(label39), "");
  swapWindow(window7, window2);
}
