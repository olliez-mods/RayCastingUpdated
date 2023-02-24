#include "sockets.hpp"
#include <iostream>

int port = 3388;
sf::TcpSocket socket;
std::optional<sf::IpAddress> address = sf::IpAddress::resolve("");
sf::Packet data;

sf::Clock deltaClock;
int pingTimer = 0;
#define pingTime 400


bool connected = false;

bool Client::connect(std::string IPaddress){
    
    address = sf::IpAddress::resolve(IPaddress);
    if(address.has_value()){
        
        // no ides what to do in this if statement XD
        float timeout = 3;
        socket.setBlocking(true);
        std::cout << "[Client]: Attempting to connect to \"" << IPaddress << "\"\n";
        if(socket.connect(address.value(), port, sf::seconds(timeout)) == sf::Socket::Status::Done){
            //socket.setBlocking(false);
            sf::Packet p;
            std::cout << "[Client]: Waiting for server to send connection confirmation\n";
            if(socket.receive(p) == sf::Socket::Status::Done){
                std::string stuff;
                p >> stuff;
                if(stuff == "connectionMade"){
                    socket.setBlocking(false);
                    connected = true;
                    std::cout << "[Client]: Connection made\n";
                    return true;
                }
            }
        }

    }else{
        std::cout << "[Client]: " << IPaddress << " is not a valid address\n";
    }
    std::cout << "[Client]: Couldnt connect to \"" << IPaddress << "\"\n";
    return false;
    
}

void Client::disconnect(){
    sf::Packet p;
    p << "server" << "disconnect";
    socket.send(p);
    socket.disconnect();
    connected = false;
    std::cout << "[Client]: Disconnected from server\n";
}

void Client::pingServer(){
    if(connected){
        sf::Time dt = deltaClock.restart();
        
        pingTimer += dt.asMilliseconds();
        if(pingTimer > pingTime){
            pingTimer = 0;
            sf::Packet p;
            p << "server" << "ping";
            socket.send(p);
        }
    }
}

void Client::setBlocking(bool block){
    socket.setBlocking(block);
}

bool Client::isConnected(){
    return connected;
}

void Client::send(sf::Packet packet)
{
    if(connected){
        socket.send(packet);
    }
}

bool Client::checkNewData()
{
    if(connected){
        sf::Packet newData;
        if (socket.receive(newData) == sf::Socket::Status::Done)
        {
            std::cout << "[Client]: Received data\n";
            data = newData;
            return true;
        }
        return false;
    }
    return false;
}

sf::Packet Client::getData()
{
    return data;
}
