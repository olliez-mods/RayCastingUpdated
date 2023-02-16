#ifndef sockets_hpp
#define sockets_hpp

#include <SFML/Network.hpp>

bool connect(std::string IPaddress);

void tickServer();

bool checkNewData();

sf::Packet getData();

void setBlocking(bool block);

void disconnect();

bool getConnected();

void send(sf::Packet packet);
#endif
