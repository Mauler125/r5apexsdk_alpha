#pragma once

// For builds which have/do not have a server or client
#define SERVER
#define CLIENT

// For builds where function signatures don't match
#define S2
#define S0

// For enabling or disabling OriginSDK initialization
//#define NOORIGIN

	//const char* path = "platform/";
	//char buffer[512] = { 0 };

	//strcpy_s(buffer, sizeof(buffer), path);
	//strncat_s(buffer, rson_name, sizeof(buffer));