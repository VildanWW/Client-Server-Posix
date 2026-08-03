#include <iostream>
#include <string>
#include <vector>
#include "Client.h"

int main() {
	Client client;
	std::string ip = "127.0.0.1";
	int port = 9090;
	 
	std::cout << "Enter your name: ";
	std::string name;
	std::getline(std::cin, name);

	if (name.empty()) {
		name = "User";
	}

	std::cout << "Connect to the server\n";
	if (!client.connectToServer(ip, port)) {
		std::cerr << "Don't to connect!\n";
		return -1;
	} 

	std::vector<char> nameData(name.begin(), name.end());
	client.sendToData(nameData);

	std::string input;

	while (true) {
		std::cout << "> ";
		std::getline(std::cin, input);

		if (input == "exit") break;

		if (!input.empty()) {
			std::vector<char> dataSend(input.begin(), input.end());
			client.sendToData(dataSend);
		}
	}

	std::cout << "Client exits\n";

	return 0;
}