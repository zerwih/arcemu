#ifndef WORLD_CONFIG_H
#define WORLD_CONFIG_H

#include <string>

struct WorldConfigData
{
	struct WorldDB
	{
		std::string hostname;
		std::string username;
		std::string password;
		std::string database;
		unsigned short port;
	}worldDB;

	struct CharacterDB
	{
		std::string hostname;
		std::string username;
		std::string password;
		std::string database;
		unsigned short port;
	}characterDB;

	struct Host
	{
		std::string address;
		unsigned short port;
	}host;

	struct LogSettings
	{
		unsigned short level;
		bool packetLogging;
		bool noCrashReport;
		bool logCheaters;
		bool logGMCommands;
		bool useTimeStamp;
	}log;

	struct Server
	{
		unsigned int playerLimit;
		std::string motd;
		bool sendStatsOnJoin;
		bool enableBreathing;
		bool separateChatChannels;
		unsigned int compressionThreshold;
		unsigned int queueUpdateInterval;
		bool kickAFKPlayers;
		unsigned int connectionTimeout;
		unsigned short realmType;
		bool adjustPriority;
		bool requireAllSignatures;
		bool showGMInWhoList;
		unsigned int mapUnloadTime;
		bool limitedNames;
		bool useAccountData;
		bool allowPlayerCommands;
		bool enableLFGJoin;
		unsigned int timezone;
		bool collision;
		bool disableFearMovement;
		bool saveExtendedCharData;
		unsigned int playerAutoSaveInterval;
		std::string banTable;
	}server;

	struct Announce
	{
		std::string tag;
		bool GMAdminTag;
		bool nameInAnnounce;
		bool nameInWsAnnounce;
		bool showInConsole;
	}announce;

	struct Rates
	{
		float hp;
		float mana;
		float rage;
		float focus;
		float energy;
		float reputation;
		float killReputation;
		float honor;
		unsigned int pvpTimer;
		unsigned int arenaQueueDiff;
		bool compression;
		float XP;
		float questXP;
		float restXP;
		float exploreXP;
		float dropGrey;
		float dropWhite;
		float dropGreen;
		float dropBlue;
		float dropPurple;
		float dropOrange;
		float dropArtifact;
		float dropMoney;
		float skillChance;
		float skillRate;
		float arenaMultiplier2x;
		float arenaMultiplier3x;
		float arenaMultiplier5x;
	}rates;

	struct GMClient
	{
		std::string channel;
	}gmClient;

	struct Terrain
	{
		std::string mapPath;
		std::string vmapPath;
		bool unloadMaps;
	}terrain;

	struct Mail
	{
		unsigned int reloadDelay;
		bool disablePostageCost;
		bool disableGMPostageCost;
		bool disablePostageDelayItems;
		bool disableMessageExpiry;
		bool enableInterfactionMail;
		bool enableGMInterfactionMail;
	}mail;

	struct StatDumper
	{
		std::string fileName;
		unsigned long interval;
	}statDumper;

	struct Startup
	{
		bool preloading;
		bool backgroundLootLoading;
		bool enableMultithreadedLoading;
		bool enableSpellIdDump;
		std::string loadAdditionalTables;
	}startup;

	struct SpamProtection
	{
		unsigned int lines;
		unsigned int seconds;
		bool sendMessage;
	}spamProtection;

	struct Security
	{
		bool disablePings;
		std::string remotePassword;
	}security;

	struct HackDetection
	{
		bool teleport;
		bool speed;
		bool fallDamage;
		bool flight;
		unsigned int flightThreshold;
		bool disableOnGM;
	}hackDetection;

	struct Periods
	{
		std::string honorUpdate;
		std::string arenaUpdate;
		std::string dailyUpdate;
	}periods;

	struct Channels
	{
		std::string bannedChannels;
		std::string minReqLevel;
	}channels;

	struct RemoteConsole
	{
		bool enabled;
		std::string address;
		unsigned short port;
	}remoteConsole;

	struct Movement
	{
		unsigned int flushInterval;
		unsigned int compressionRate;
		float compressionThreshold;
		float compressionThresholdCreatures;
	}movement;

	struct Localization
	{
		std::string localeBindings;
	}localization;

	struct InstanceHandling
	{
		bool takeGroupLeaderId;
		bool slidingExpiration;
		unsigned int dailyHeroicResetHour;
		bool checkTriggerPrerequisites;
	}instanceHandling;

	struct Battlegrounds
	{
		unsigned int AV_MIN;
		unsigned int AV_MAX;
		unsigned int AB_MIN;
		unsigned int AB_MAX;
		unsigned int EOTS_MIN;
		unsigned int EOTS_MAX;
		unsigned int WSG_MIN;
		unsigned int WSG_MAX;
		unsigned int SOTA_MIN;
		unsigned int SOTA_MAX;
		unsigned int IOC_MIN;
		unsigned int IOC_MAX;
	}battlegrounds;

	struct Arena
	{
		unsigned int season;
		bool inProgress;
	}arena;

	struct GM
	{
		bool startOnGMIsland;
		bool disableAchievements;
	}gm;

	struct Limits
	{
		bool enabled;
		unsigned int autoAttackDmg;
		unsigned int spellDmg;
		unsigned int hp;
		unsigned int mana;
		bool disconnect;
		bool report;
	}limits;

};

#endif
