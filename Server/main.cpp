#include "server.hpp"

int main() {

	Server::start();

	while (true) {
		Server::tick();
	}

}