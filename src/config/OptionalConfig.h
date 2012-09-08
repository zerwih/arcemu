#ifndef OPTIONAL_CONFIG_H
#define OPTIONAL_CONFIG_H

#ifndef SERVER_DECL
#define SERVER_DECL
#endif

struct SERVER_DECL OptionalConfigData
{
	struct OptionalStuff
	{
		unsigned short startingLevel;
		unsigned short levelCap;
		unsigned short genLevelCap;
		bool allowTBC;
		bool antiMasterLootNinja;
		bool additionalFun;
		unsigned short DKStartingTalent;
		unsigned short maxProfessions;
	}optional;

	struct InterfactionStuff
	{
		bool chat;
		bool group;
		bool guild;
		bool trade;
		bool friends;
		bool misc;
		bool crossoverChars;
	}interfaction;

	struct AnnounceColor
	{
		unsigned short tag;
		unsigned short GMAdminTag;
		unsigned short name;
		unsigned short msg;
	}announceColor;

	struct ExtraStuff
	{
		bool instant80;
		bool portals;
		int pvpTokenId;
	}extra;

	struct GameMaster
	{
		bool listOnlyActive;
		bool hidePermissions;
	}gm;

	struct CommonScheduler
	{
		bool autoBroadcast;
		unsigned long broadcastInterval;
		unsigned short broadcastTriggerPercentCap;
		unsigned short broadcastOrderMode;
	}commonScheduler;

	struct ClassOptions
	{
		bool DKPrerequisites;
		bool DKLimit;
	}classOptions;

	struct GoldSettings
	{
		unsigned int startingGold;
		bool enableCap;
		unsigned int cap;
	}goldSettings;

};

#endif


