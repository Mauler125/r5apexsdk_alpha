#include "core/stdafx.h"
#include "networksystem/serverlisting.h"
#include "gameui/IServerBrowser.h"

void ServerListing::Select()
{
	std::stringstream cmd;
	cmd << "connect " << this->ip << ":" << this->port;
	g_ServerBrowser->ProcessCommand(cmd.str().c_str());
}
