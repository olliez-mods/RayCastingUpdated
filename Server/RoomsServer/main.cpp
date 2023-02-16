#include <SFML/Network.hpp>
#include <iostream>
#include <cmath>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif



#define timeoutTime 1

#define maxPlayers 4

int main() {
    
    sf::TcpListener listener;
    sf::TcpSocket players[maxPlayers];
    
    float timeoutTimer[maxPlayers] = {0};
    
    bool connectedPlayers[maxPlayers] = {false};
    
    
    int rootUser = 0;
    
    listener.listen(3388);
    
    listener.setBlocking(false);
    
    std::cout << "Server running on port " << 3388 << "\n";
    
    // player requesting all walls from root user (first user to join server)
    int sendAllWallsIndex = -1;
    
    
    sf::Clock deltaClock;
    while (true){
        sf::Time dt = deltaClock.restart();
        
        

        if(dt.asSeconds() == 0.0){
            // If the program is running too fast sf::Clock cant register, so we pause for 1/100 of a second (1000000 being 1 second)
            // this usaly happens when there's no packages to receive or there's no one connected
                usleep(10000);
        }
        // every tick, increment all connected players ping timer by x milliseconds, if its over the limit then send a ping, if that ping isnt returned
        // then disconnect the player, and free that spot
        

        
        for(int i = 0; i < maxPlayers; i++){
            if(connectedPlayers[i]){
                
                timeoutTimer[i] += dt.asSeconds();
               // std::cout << timeoutTimer[i] << "\n";
                if(timeoutTimer[i] > timeoutTime){
                    players[i].disconnect();
                    connectedPlayers[i] = false;
                    timeoutTimer[i] = 0;
                    std::cout << "[" << i << "] disconnected a player\n";
                    if(i == rootUser){
                        // default root is 0
                        rootUser = 0;
                        
                        // choose a new root user
                        for(int a = 0; a < maxPlayers; a++){
                            if(connectedPlayers[a]){
                                rootUser = a;
                                break;
                            }
                        }
                        std::cout << "New root user: " << rootUser << "\n";
                    }
                }
                
                
            }
        }
        
        
        
        
        int firstOpenIndex = -1;
        for(int i = 0; i < maxPlayers; i++){
            if(!connectedPlayers[i]){
                firstOpenIndex = i;
                break;
            }
        }
        //std::cout << firstOpenIndex << "\n";
        if(firstOpenIndex != -1){
            if (listener.accept(players[firstOpenIndex]) != sf::Socket::Status::NotReady) {
                std::cout << "SERVER : new connection with index of: " << firstOpenIndex << "\n";
                players[firstOpenIndex].setBlocking(false);
                connectedPlayers[firstOpenIndex] = true;
                sf::Packet p;
                p << "connectionMade";
                players[firstOpenIndex].send(p);
            }
        }
        
        
        sf::Packet data;
        for(int i = 0; i < maxPlayers; i++){
            if(connectedPlayers[i]){
                if (players[i].receive(data) == sf::Socket::Status::Done){
                    std::string command;
                    data >> command;
                    
                    
                    
                    if(command == "server"){
                        data >> command;
                        
                        
                        if(command == "getAllWalls"){
                            if(i != rootUser){
                                sf::Packet p;
                                p << "sendWalls";
                                sendAllWallsIndex = i;
                                players[rootUser].send(p);
                            }else{
                                std::cout << "[" << i << "] Root user attempted to get all walls from himselfXD\n";
                            }
                        }
                        
                        if(command == "ping"){
                            timeoutTimer[i] = 0;
                        }else{
                            std::cout  << "[" << i << "] Received server packet " << command << "\n";
                            
                        }
                        
                        if(command == "disconnect"){
                            // just timeout the connetion emediatly, nice
                            timeoutTimer[i] = timeoutTime+1;
                        }
                        
                        // the client requests this when he joins the server, to sync up with the root user (first user to join)
                        if(command == "groupWalls"){
                            std::string tag;
                            float pos1x;
                            float pos1y;
                            float pos2x;
                            float pos2y;
                            std::string materialName;
                            
                            data >> tag >> pos1x >> pos1y >> pos2x >> pos2y >> materialName;
                            //std::cout << "send this wall to user: " << tag << "  " << pos1x << "  " << pos1y << "  " << pos2x << "  " << pos2y << "  " << materialName << "\n";
                            sf::Packet p;
                            p << "newWall" << tag << pos1x << pos1y << pos2x << pos2y << materialName;
                            players[sendAllWallsIndex].send(p);
                        }
                        
                        
                    }else{
                        std::cout << "[" << i  << "] Received packet " << command << "\n";
                        for(int a = 0; a < maxPlayers; a++){
                            if(a != i && connectedPlayers[a]){
                                players[a].send(data);
                            }
                        }
                    }
                }
            }
        }
        
    }
    
}
