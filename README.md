# Quizz-Game

This project is a client-server application that manages multiple game rooms for a general knowledge game. 

# Technologies:

 * TCP-multithreading for managing an unlimited number of clients and game rooms
 * SQLite Database for storing questions and clients accounts
 * Gtk library (and Glade Designer) for creating an intuitive and friendly GUI

# Description

Every client logs in with their own account in order to join/create a room (also there is a system that counts wins and defeats).

<img width="350" src="https://github.com/andreihaivas6/Quizz-Game/blob/main/ss/a1.png">

In main menu a client choose to create or to join a room based on a secret code.

<img width="350" src="https://github.com/andreihaivas6/Quizz-Game/blob/main/ss/a2.png">

Room administrator may choose the categories, time for every question, and number of questions. Also, he decides to start the game when all participants are ready.

<img width="700" src="https://github.com/andreihaivas6/Quizz-Game/blob/main/ss/a3.png">

Every player has a limited time to give an answear. The server stores their score in order to create a leaderboard at the end of the game.

<img width="700" src="https://github.com/andreihaivas6/Quizz-Game/blob/main/ss/a4.png">


