#pragma once
#include "stdafx.h"

class ServerListing
{
public:

	ServerListing() = default;

	ServerListing(std::string name, std::string map, std::string ip, std::string port) : name(name), map(map), ip(ip), port(port)
	{
		// for future constructor use.
	}

	void Select();

	std::string name;
	std::string map;
	std::string ip;
	std::string port;
	std::string password;
};
