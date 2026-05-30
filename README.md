# Multiplayer Chess Game
This project is a console-based multiplayer Chess game developed in C using a client-server architecture. Two players can connect, choose their sides, and play a complete game of chess through inter-process communication using Windows Named Pipes.
The project was built to understand networking concepts, game logic implementation, file handling, and Windows system programming.

## Features
* Multiplayer chess gameplay
* Client-server communication using Named Pipes
* Real-time move exchange between players
* Complete chess board setup
* Validation of chess piece movements
* Check and Checkmate detection
* Player name registration
* White and Black piece selection
* Save and resume game functionality
* Turn-based gameplay system

## How the Project Works
1. The server application is started first.
2. Two clients connect to the server.
3. Players enter their names and choose their preferred side.
4. The server manages communication between both players.
5. Players take turns entering chess moves.
6. The game validates each move according to chess rules.
7. The board is updated and synchronized between both clients.
8. The game continues until a player wins, loses, or saves the game.
