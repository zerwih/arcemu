#ifndef REALMSCONF_H
#define REALMSCONF_H

#include <string>
#include <vector>

struct RealmsConfigData
{
	struct LogonServerStuff
	{
		std::string address;
		unsigned int port;
		std::string name;
		unsigned int realmCount;

	}logon;

	struct RealmSettings
	{
		unsigned int id;
		std::string name;
		std::string addressAndPort;
		std::string icon;
		float population;
		unsigned short timezone;

	};

	std::vector< RealmSettings > realms;
};

#endif

