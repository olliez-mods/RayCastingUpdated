#ifndef sockets_hpp
#define sockets_hpp

#include <SFML/Network.hpp>
namespace Client {

bool connect(std::string IPaddress);

void pingServer();

bool checkNewData();

sf::Packet getData();

void setBlocking(bool block);

void disconnect();

bool isConnected();

void send(sf::Packet packet);
}
#endif
