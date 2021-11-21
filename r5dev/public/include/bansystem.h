#pragma once
#include "public/include/json.hpp"

class CBanSystem
{
public:
	CBanSystem();
	void operator[](std::pair<std::string, std::int64_t> pair);
	void Load();
	void Save();
	void AddEntry(std::string ipAddress, std::int64_t originID);
	void DeleteEntry(std::string ipAddress, std::int64_t originID);
	void AddConnectionRefuse(std::string error, int userID);
	void DeleteConnectionRefuse(int userID);
	bool IsBanned(std::string ipAddress, std::int64_t originID);
	bool IsRefuseListValid();
	bool IsBanListValid();

	std::vector<std::pair<std::string, int>> refuseList = {};;
private:
	std::vector<std::pair<std::string, std::int64_t>> banList = {};
};

extern CBanSystem* g_pBanSystem;
