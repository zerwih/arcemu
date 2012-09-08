#include "WorldConfigParser.h"
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

namespace
{
#define PCHAR_CAST (char*)

	unsigned short toUShort( const std::string &s )
	{
		unsigned short v = static_cast< unsigned short >( atoi( s.c_str() ) );
		return v;
	}

	unsigned int toUInt( const std::string &s )
	{
		unsigned int v = atoi( s.c_str() );
		return v;
	}

	unsigned long toULong( const std::string &s )
	{
		unsigned long v = strtol( s.c_str(), NULL, 10 );
		return v;
	}

	float toFloat( const std::string &s )
	{
		float v = atof( s.c_str() );
		return v;
	}

	bool toBool( const std::string &s )
	{
		if( s.size() == 1 )
		{
			if( s[ 0 ] == '1' )
				return true;
			else
				return false;
		}
		else
		{
			if( s == "true" )
				return true;
			else
				return false;
		}
	}

	enum WorldErrors
	{
		ERR_ALL_OK,
		ERR_CANNOT_OPEN_FILE,
		ERR_NOT_A_CONFIG_FILE,
		ERR_NOT_WORLD_CONFIG,
		ERR_WRONG_VERSION,
		ERR_WDB_NO_HOST,
		ERR_WDB_NO_USER,
		ERR_WDB_NO_PASS,
		ERR_WDB_NO_DB,
		ERR_CDB_NO_HOST,
		ERR_CDB_NO_USER,
		ERR_CDB_NO_PASS,
		ERR_CDB_NO_DB,
		ERR_HOST_NO_ADDRESS,
		ERR_HOST_NO_PORT,
		ERR_SEC_NO_PASS,
		ERR_NO_WDB_SETTINGS,
		ERR_NO_CDB_SETTINGS,
		ERR_NO_HOST_SETTINGS,
		ERR_MAX_ERROR
	};

	const char* errorStrings[] = 
	{
		"No errors",
		"Cannot open file",
		"Not a config file",
		"Not a world config file",
		"Wrong version of config file",
		"World database, couldn't load hostname",
		"World database, couldn't load username",
		"World database, couldn't load password",
		"World database, couldn't load database name",	
		"Character database, couldn't load hostname",
		"Character database, couldn't load username",
		"Character database, couldn't load password",
		"Character database, couldn't load database name",
		"Host, couldn't load address",
		"Host, couldn't load port",
		"Security, couldn't load remote password",
		"Couldn't load world database connection settings",
		"Couldn't load character database connection settings",
		"Couldn't load host settings"
	};

}

WorldConfigParser::WorldConfigParser()
{
	lastError        = ERR_ALL_OK;
	worldDBPartDone  = false;
	charDBPartDone   = false;
	hostPartDone     = false;
}

WorldConfigParser::~WorldConfigParser()
{
}

const char* WorldConfigParser::getLastError() const
{
	return errorStrings[ lastError ];
}

bool WorldConfigParser::parseFile( const std::string &name )
{
	xmlDocPtr document = xmlParseFile( name.c_str() );
	if( document == NULL )
	{
		lastError = ERR_CANNOT_OPEN_FILE;
		return false;
	}

	xmlNodePtr root = xmlDocGetRootElement( document );
	if( root == NULL )
	{
		xmlFreeDoc( document );
		lastError = ERR_NOT_A_CONFIG_FILE;
		return false;
	}

	if( !isConfig( root ) )
	{
		xmlFreeDoc( document );
		return false;
	}

	bool ok = true;
	xmlNodePtr node = root->children;
	while( node != NULL )
	{
		if( xmlStrcmp( node->name, BAD_CAST "world_database" ) == 0 )
			ok = parseWorldDBStuff( node );
		else
		if( xmlStrcmp( node->name, BAD_CAST "character_database" ) == 0 )
			ok = parseCharDBStuff( node );
		else
		if( xmlStrcmp( node->name, BAD_CAST "host" ) == 0 )
			ok = parseHostStuff( node );
		else
		if( xmlStrcmp( node->name, BAD_CAST "log" ) == 0 )
			ok = parseLogStuff( node );
		else
		if( xmlStrcmp( node->name, BAD_CAST "server" ) == 0 )
			ok = parseServerStuff( node );
		else
		if( xmlStrcmp( node->name, BAD_CAST "announce" ) == 0 )
			ok = parseAnnounceStuff( node );
		else
		if( xmlStrcmp( node->name, BAD_CAST "rates" ) == 0 )
			ok = parseRatesStuff( node );
		else
		if( xmlStrcmp( node->name, BAD_CAST "gm_client" ) == 0 )
			ok = parseGMClientStuff( node );
		else
		if( xmlStrcmp( node->name, BAD_CAST "terrain" ) == 0 )
			ok = parseTerrainStuff( node );
		else
		if( xmlStrcmp( node->name, BAD_CAST "mail" ) == 0 )
			ok = parseMailStuff( node );
		else
		if( xmlStrcmp( node->name, BAD_CAST "stat_dumper" ) == 0 )
			ok = parseStatDumperStuff( node );
		else
		if( xmlStrcmp( node->name, BAD_CAST "startup" ) == 0 )
			ok = parseStartupStuff( node );
		else
		if( xmlStrcmp( node->name, BAD_CAST "spam_protection" ) == 0 )
			ok = parseSpamProtectionStuff( node );
		else
		if( xmlStrcmp( node->name, BAD_CAST "security" ) == 0 )
			ok = parseSecurityStuff( node );
		else
		if( xmlStrcmp( node->name, BAD_CAST "hack_detection" ) == 0 )
			ok = parseHackDetectionStuff( node );
		else
		if( xmlStrcmp( node->name, BAD_CAST "periods" ) == 0 )
			ok = parsePeriodsStuff( node );
		else
		if( xmlStrcmp( node->name, BAD_CAST "channels" ) == 0 )
			ok = parseChannelsStuff( node );
		else
		if( xmlStrcmp( node->name, BAD_CAST "remote_console" ) == 0 )
			ok = parseRemoteConsoleStuff( node );
		else
		if( xmlStrcmp( node->name, BAD_CAST "movement" ) == 0 )
			ok = parseMovementStuff( node );
		else
		if( xmlStrcmp( node->name, BAD_CAST "localization" ) == 0 )
			ok = parseLocalizationStuff( node );
		else
		if( xmlStrcmp( node->name, BAD_CAST "instance_handling" ) == 0 )
			ok = parseInstanceHandlingStuff( node );
		else
		if( xmlStrcmp( node->name, BAD_CAST "battlegrounds" ) == 0 )
			ok = parseBGStuff( node );
		else
		if( xmlStrcmp( node->name, BAD_CAST "arena" ) == 0 )
			ok = parseArenaStuff( node );
		else
		if( xmlStrcmp( node->name, BAD_CAST "game_master" ) == 0 )
			ok = parseGMStuff( node );
		else
		if( xmlStrcmp( node->name, BAD_CAST "limits" ) == 0 )
			ok = parseLimitStuff( node );

		if( !ok )
			break;

		node = node->next;
	}

	xmlFreeDoc( document );

	if( !ok )
		return false;

	if( !worldDBPartDone )
	{
		ok = false;
		lastError = ERR_NO_WDB_SETTINGS;
	}
	else
	if( !charDBPartDone )
	{
		ok = false;
		lastError = ERR_NO_CDB_SETTINGS;
	}
	else
	if( !hostPartDone )
	{
		ok = false;
		lastError = ERR_NO_HOST_SETTINGS;
	}

	if( ok )
		return true;
	else
		return false;
}

bool WorldConfigParser::isConfig( _xmlNode *node )
{
	xmlChar *prop = NULL;

	prop = xmlGetProp( node, BAD_CAST "type" );
	if( prop == NULL )
	{
		lastError = ERR_NOT_A_CONFIG_FILE;
		return false;
	}
	if( xmlStrcmp( prop, BAD_CAST "world" ) != 0 )
	{
		lastError = ERR_NOT_WORLD_CONFIG;
		return false;
	}

	prop = xmlGetProp( node, BAD_CAST "version" );
	if( prop == NULL )
	{
		lastError = ERR_NOT_A_CONFIG_FILE;
		return false;
	}
	if( xmlStrcmp( prop, BAD_CAST requiredVersion.c_str() ) != 0 )
	{
		lastError = ERR_WRONG_VERSION;
		return false;
	}

	return true;
}

bool WorldConfigParser::parseWorldDBStuff( _xmlNode *node )
{
	xmlChar *prop = NULL;
	prop = xmlGetProp( node, BAD_CAST "hostname" );
	if( prop == NULL )
	{
		lastError = ERR_WDB_NO_HOST;
		return false;
	}
	data.worldDB.hostname = PCHAR_CAST prop;

	prop = xmlGetProp( node, BAD_CAST "username" );
	if( prop == NULL )
	{
		lastError = ERR_WDB_NO_USER;
		return false;
	}
	data.worldDB.username = PCHAR_CAST prop;

	prop = xmlGetProp( node, BAD_CAST "password" );
	if( prop == NULL )
	{
		lastError = ERR_WDB_NO_PASS;
		return false;
	}
	data.worldDB.password = PCHAR_CAST prop;

	prop = xmlGetProp( node, BAD_CAST "database" );
	if( prop == NULL )
	{
		lastError = ERR_WDB_NO_DB;
		return false;
	}
	data.worldDB.database = PCHAR_CAST prop;

	prop = xmlGetProp( node, BAD_CAST "port" );
	if( prop == NULL )
		data.worldDB.port = 3306;
	else
		data.worldDB.port = toUShort( PCHAR_CAST prop );
	
	worldDBPartDone = true;
	return true;
}

bool WorldConfigParser::parseCharDBStuff( _xmlNode *node )
{
	xmlChar *prop = NULL;
	prop = xmlGetProp( node, BAD_CAST "hostname" );
	if( prop == NULL )
	{
		lastError = ERR_CDB_NO_HOST;
		return false;
	}
	data.characterDB.hostname = PCHAR_CAST prop;

	prop = xmlGetProp( node, BAD_CAST "username" );
	if( prop == NULL )
	{
		lastError = ERR_CDB_NO_USER;
		return false;
	}
	data.characterDB.username = PCHAR_CAST prop;

	prop = xmlGetProp( node, BAD_CAST "password" );
	if( prop == NULL )
	{
		lastError = ERR_CDB_NO_PASS;
		return false;
	}
	data.characterDB.password = PCHAR_CAST prop;

	prop = xmlGetProp( node, BAD_CAST "database" );
	if( prop == NULL )
	{
		lastError = ERR_CDB_NO_DB;
		return false;
	}
	data.characterDB.database = PCHAR_CAST prop;

	prop = xmlGetProp( node, BAD_CAST "port" );
	if( prop == NULL )
		data.characterDB.port = 3306;
	else
		data.characterDB.port = toUShort( PCHAR_CAST prop );
	
	charDBPartDone = true;
	return true;
}

bool WorldConfigParser::parseHostStuff( _xmlNode *node )
{
	xmlChar *prop = NULL;

	prop = xmlGetProp( node, BAD_CAST "world_address" );
	if( prop == NULL )
	{
		lastError = ERR_HOST_NO_ADDRESS;
		return false;
	}
	data.host.address = PCHAR_CAST prop;

	prop = xmlGetProp( node, BAD_CAST "world_port" );
	if( prop == NULL )
	{
		lastError = ERR_HOST_NO_PORT;
		return false;
	}
	data.host.port = toUShort( PCHAR_CAST prop );

	hostPartDone = true;
	return true;
}

bool WorldConfigParser::parseLogStuff( _xmlNode *node )
{
	xmlChar *prop = NULL;
	prop = xmlGetProp( node, BAD_CAST "" );
	if( prop != NULL )
		data.log.level = toUShort( PCHAR_CAST prop );
	else
		data.log.level = 0;

	prop = xmlGetProp( node, BAD_CAST "packet_logging" );
	if( prop != NULL )
		data.log.packetLogging = toBool( PCHAR_CAST prop );
	else
		data.log.packetLogging = false;

	prop = xmlGetProp( node, BAD_CAST "disable_crashdump_report" );
	if( prop != NULL )
		data.log.noCrashReport = toBool( PCHAR_CAST prop );
	else
		data.log.noCrashReport = false;

	prop = xmlGetProp( node, BAD_CAST "cheaters" );
	if( prop != NULL )
		data.log.logCheaters = toBool( PCHAR_CAST prop );
	else
		data.log.logCheaters = false;

	prop = xmlGetProp( node, BAD_CAST "gm_commands" );
	if( prop != NULL )
		data.log.logGMCommands = toBool( PCHAR_CAST prop );
	else
		data.log.logGMCommands = false;

	prop = xmlGetProp( node, BAD_CAST "timestamp" );
	if( prop != NULL )
		data.log.useTimeStamp = toBool( PCHAR_CAST prop );
	else
		data.log.useTimeStamp = true;

	return true;
}

bool WorldConfigParser::parseServerStuff( _xmlNode *node )
{
	xmlChar *prop = NULL;
	prop = xmlGetProp( node, BAD_CAST "player_limit" );
	if( prop != NULL )
		data.server.playerLimit = toUInt( PCHAR_CAST prop );
	else
		data.server.playerLimit = 0;

	prop = xmlGetProp( node, BAD_CAST "motd" );
	if( prop != NULL )
		data.server.motd = PCHAR_CAST prop;
	else
		data.server.motd = "";

	prop = xmlGetProp( node, BAD_CAST "send_stats_on_join" );
	if( prop != NULL )
		data.server.sendStatsOnJoin = toBool( PCHAR_CAST prop );
	else
		data.server.sendStatsOnJoin = true;

	prop = xmlGetProp( node, BAD_CAST "enable_breathing" );
	if( prop != NULL )
		data.server.enableBreathing = toBool( PCHAR_CAST prop );
	else
		data.server.enableBreathing = true;

	prop = xmlGetProp( node, BAD_CAST "seperate_chat_channels" );
	if( prop != NULL )
		data.server.separateChatChannels = toBool( PCHAR_CAST prop );
	else
		data.server.separateChatChannels = false;

	prop = xmlGetProp( node, BAD_CAST "compression_threshold" );
	if( prop != NULL )
		data.server.compressionThreshold = toUInt( PCHAR_CAST prop );
	else
		data.server.compressionThreshold = 1000;

	prop = xmlGetProp( node, BAD_CAST "queue_update_interval" );
	if( prop != NULL )
		data.server.queueUpdateInterval = toUInt( PCHAR_CAST prop );
	else
		data.server.queueUpdateInterval = 5000;

	prop = xmlGetProp( node, BAD_CAST "kick_afk_players" );
	if( prop != NULL )
		data.server.kickAFKPlayers = toBool( PCHAR_CAST prop );
	else
		data.server.kickAFKPlayers = false;

	prop = xmlGetProp( node, BAD_CAST "connection_timeout" );
	if( prop != NULL )
		data.server.connectionTimeout = toUInt( PCHAR_CAST prop );
	else
		data.server.connectionTimeout = 180;

	prop = xmlGetProp( node, BAD_CAST "realm_type" );
	if( prop != NULL )
		data.server.realmType = toUShort( PCHAR_CAST prop );
	else
		data.server.realmType = 1;

	prop = xmlGetProp( node, BAD_CAST "adjust_priority" );
	if( prop != NULL )
		data.server.adjustPriority = toBool( PCHAR_CAST prop );
	else
		data.server.adjustPriority = false;

	prop = xmlGetProp( node, BAD_CAST "require_all_signatures" );
	if( prop != NULL )
		data.server.requireAllSignatures = toBool( PCHAR_CAST prop );
	else
		data.server.requireAllSignatures = true;

	prop = xmlGetProp( node, BAD_CAST "show_gm_in_who_list" );
	if( prop != NULL )
		data.server.showGMInWhoList = toBool( PCHAR_CAST prop );
	else
		data.server.showGMInWhoList = true;

	prop = xmlGetProp( node, BAD_CAST "map_unload_time" );
	if( prop != NULL )
		data.server.mapUnloadTime = toUInt( PCHAR_CAST prop );
	else
		data.server.mapUnloadTime = 300;

	prop = xmlGetProp( node, BAD_CAST "limited_names" );
	if( prop != NULL )
		data.server.limitedNames = toBool( PCHAR_CAST prop );
	else
		data.server.limitedNames = true;

	prop = xmlGetProp( node, BAD_CAST "use_account_data" );
	if( prop != NULL )
		data.server.useAccountData = toBool( PCHAR_CAST prop );
	else
		data.server.useAccountData = false;

	prop = xmlGetProp( node, BAD_CAST "allow_player_commands" );
	if( prop != NULL )
		data.server.allowPlayerCommands = toBool( PCHAR_CAST prop );
	else
		data.server.allowPlayerCommands = false;

	prop = xmlGetProp( node, BAD_CAST "enable_lfg_join" );
	if( prop != NULL )
		data.server.enableLFGJoin = toBool( PCHAR_CAST prop );
	else
		data.server.enableLFGJoin = false;

	prop = xmlGetProp( node, BAD_CAST "time_zone" );
	if( prop != NULL )
		data.server.timezone = toUInt( PCHAR_CAST prop );
	else
		data.server.timezone = 0;

	prop = xmlGetProp( node, BAD_CAST "collision" );
	if( prop != NULL )
		data.server.collision = toBool( PCHAR_CAST prop );
	else
		data.server.collision = false;

	prop = xmlGetProp( node, BAD_CAST "disable_fear_movement" );
	if( prop != NULL )
		data.server.disableFearMovement = toBool( PCHAR_CAST prop );
	else
		data.server.disableFearMovement = false;

	prop = xmlGetProp( node, BAD_CAST "save_extended_char_data" );
	if( prop != NULL )
		data.server.saveExtendedCharData = toBool( PCHAR_CAST prop );
	else
		data.server.saveExtendedCharData = false;

	prop = xmlGetProp( node, BAD_CAST "player_autosave_interval" );
	if( prop != NULL )
		data.server.playerAutoSaveInterval = toUInt( PCHAR_CAST prop );
	else
		data.server.playerAutoSaveInterval = 300000;

	return true;
}

bool WorldConfigParser::parseAnnounceStuff( _xmlNode *node )
{
	xmlChar *prop = NULL;
	prop = xmlGetProp( node, BAD_CAST "tag" );
	if( prop != NULL )
		data.announce.tag = PCHAR_CAST prop;
	else
		data.announce.tag = "Staff";

	prop = xmlGetProp( node, BAD_CAST "gm_admin_tag" );
	if( prop != NULL )
		data.announce.GMAdminTag = toBool( PCHAR_CAST prop );
	else
		data.announce.GMAdminTag = false;

	prop = xmlGetProp( node, BAD_CAST "name_in_announce" );
	if( prop != NULL )
		data.announce.nameInAnnounce = toBool( PCHAR_CAST prop );
	else
		data.announce.nameInAnnounce = true;

	prop = xmlGetProp( node, BAD_CAST "name_in_widescreen_announce" );
	if( prop != NULL )
		data.announce.nameInWsAnnounce = toBool( PCHAR_CAST prop );
	else
		data.announce.nameInWsAnnounce = true;

	prop = xmlGetProp( node, BAD_CAST "show_in_console" );
	if( prop != NULL )
		data.announce.showInConsole = toBool( PCHAR_CAST prop );
	else
		data.announce.showInConsole = true;

	return true;
}

bool WorldConfigParser::parseRatesStuff( _xmlNode *node )
{
	xmlChar *prop = NULL;
	prop = xmlGetProp( node, BAD_CAST "health" );
	if( prop != NULL )
		data.rates.hp = toFloat( PCHAR_CAST prop );
	else
		data.rates.hp = 1.0f;

	prop = xmlGetProp( node, BAD_CAST "power_mana" );
	if( prop != NULL )
		data.rates.mana = toFloat( PCHAR_CAST prop );
	else
		data.rates.mana = 1.0f;

	prop = xmlGetProp( node, BAD_CAST "power_rage" );
	if( prop != NULL )
		data.rates.rage = toFloat( PCHAR_CAST prop );
	else
		data.rates.rage = 1.0f;

	prop = xmlGetProp( node, BAD_CAST "power_pet_focus" );
	if( prop != NULL )
		data.rates.focus = toFloat( PCHAR_CAST prop );
	else
		data.rates.focus = 1.0f;

	prop = xmlGetProp( node, BAD_CAST "power_energy" );
	if( prop != NULL )
		data.rates.energy = toFloat( PCHAR_CAST prop );
	else
		data.rates.energy = 1.0f;

	prop = xmlGetProp( node, BAD_CAST "quest_reputation" );
	if( prop != NULL )
		data.rates.reputation = toFloat( PCHAR_CAST prop );
	else
		data.rates.reputation = 1.0f;

	prop = xmlGetProp( node, BAD_CAST "kill_reputation" );
	if( prop != NULL )
		data.rates.reputation = toFloat( PCHAR_CAST prop );
	else
		data.rates.reputation = 1.0f;

	prop = xmlGetProp( node, BAD_CAST "honor" );
	if( prop != NULL )
		data.rates.honor = toFloat( PCHAR_CAST prop );
	else
		data.rates.honor = 1.0f;

	prop = xmlGetProp( node, BAD_CAST "pvp_timer" );
	if( prop != NULL )
		data.rates.pvpTimer = toUInt( PCHAR_CAST prop );
	else
		data.rates.pvpTimer = 300000;

	prop = xmlGetProp( node, BAD_CAST "arena_queue_diff" );
	if( prop != NULL )
		data.rates.arenaQueueDiff = toUInt( PCHAR_CAST prop );
	else
		data.rates.arenaQueueDiff = 150;

	prop = xmlGetProp( node, BAD_CAST "compression" );
	if( prop != NULL )
		data.rates.compression = toBool( PCHAR_CAST prop );
	else
		data.rates.compression = false;

	prop = xmlGetProp( node, BAD_CAST "xp" );
	if( prop != NULL )
		data.rates.XP = toFloat( PCHAR_CAST prop );
	else
		data.rates.XP = 1.0f;

	prop = xmlGetProp( node, BAD_CAST "quest_xp" );
	if( prop != NULL )
		data.rates.questXP = toFloat( PCHAR_CAST prop );
	else
		data.rates.questXP = 1.0f;

	prop = xmlGetProp( node, BAD_CAST "rest_xp" );
	if( prop != NULL )
		data.rates.restXP = toFloat( PCHAR_CAST prop );
	else
		data.rates.restXP = 1.0f;

	prop = xmlGetProp( node, BAD_CAST "explore_xp" );
	if( prop != NULL )
		data.rates.exploreXP = toFloat( PCHAR_CAST prop );
	else
		data.rates.exploreXP = 1.0f;

	prop = xmlGetProp( node, BAD_CAST "drop_gray" );
	if( prop != NULL )
		data.rates.dropGrey = toFloat( PCHAR_CAST prop );
	else
		data.rates.dropGrey = 1.0f;

	prop = xmlGetProp( node, BAD_CAST "drop_white" );
	if( prop != NULL )
		data.rates.dropWhite = toFloat( PCHAR_CAST prop );
	else
		data.rates.dropWhite = 1.0f;

	prop = xmlGetProp( node, BAD_CAST "drop_green" );
	if( prop != NULL )
		data.rates.dropGreen = toFloat( PCHAR_CAST prop );
	else
		data.rates.dropGreen = 1.0f;

	prop = xmlGetProp( node, BAD_CAST "drop_blue" );
	if( prop != NULL )
		data.rates.dropBlue = toFloat( PCHAR_CAST prop );
	else
		data.rates.dropBlue = 1.0f;

	prop = xmlGetProp( node, BAD_CAST "drop_purple" );
	if( prop != NULL )
		data.rates.dropPurple = toFloat( PCHAR_CAST prop );
	else
		data.rates.dropPurple = 1.0f;

	prop = xmlGetProp( node, BAD_CAST "drop_orange" );
	if( prop != NULL )
		data.rates.dropOrange = toFloat( PCHAR_CAST prop );
	else
		data.rates.dropOrange = 1.0f;

	prop = xmlGetProp( node, BAD_CAST "drop_artifact" );
	if( prop != NULL )
		data.rates.dropArtifact = toFloat( PCHAR_CAST prop );
	else
		data.rates.dropArtifact = 1.0f;

	prop = xmlGetProp( node, BAD_CAST "drop_money" );
	if( prop != NULL )
		data.rates.dropMoney = toFloat( PCHAR_CAST prop );
	else
		data.rates.dropMoney = 1.0f;

	prop = xmlGetProp( node, BAD_CAST "skill_chance" );
	if( prop != NULL )
		data.rates.skillChance = toFloat( PCHAR_CAST prop );
	else
		data.rates.skillChance = 1.0f;

	prop = xmlGetProp( node, BAD_CAST "skill_rate" );
	if( prop != NULL )
		data.rates.skillRate = toFloat( PCHAR_CAST prop );
	else
		data.rates.skillRate = 1.0f;

	prop = xmlGetProp( node, BAD_CAST "arena_muliplier_2x" );
	if( prop != NULL )
		data.rates.arenaMultiplier2x = toFloat( PCHAR_CAST prop );
	else
		data.rates.arenaMultiplier2x = 1.0f;


	prop = xmlGetProp( node, BAD_CAST "arena_muliplier_3x" );
	if( prop != NULL )
		data.rates.arenaMultiplier3x = toFloat( PCHAR_CAST prop );
	else
		data.rates.arenaMultiplier3x = 1.0f;


	prop = xmlGetProp( node, BAD_CAST "arena_muliplier_5x" );
	if( prop != NULL )
		data.rates.arenaMultiplier5x = toFloat( PCHAR_CAST prop );
	else
		data.rates.arenaMultiplier5x = 1.0f;

	return true;
}

bool WorldConfigParser::parseGMClientStuff( _xmlNode *node )
{
	xmlChar *prop = NULL;
	prop = xmlGetProp( node, BAD_CAST "gm_client_channel" );
	if( prop != NULL )
		data.gmClient.channel = PCHAR_CAST prop;
	else
		data.gmClient.channel = "gm_sync_channel";

	return true;
}

bool WorldConfigParser::parseTerrainStuff( _xmlNode *node )
{
	xmlChar *prop = NULL;
	prop = xmlGetProp( node, BAD_CAST "map_path" );
	if( prop != NULL )
		data.terrain.mapPath = PCHAR_CAST prop;
	else
		data.terrain.mapPath = "maps";

	prop = xmlGetProp( node, BAD_CAST "vmap_path" );
	if( prop != NULL )
		data.terrain.vmapPath = PCHAR_CAST prop;
	else
		data.terrain.vmapPath = "vmaps";

	prop = xmlGetProp( node, BAD_CAST "unload_maps" );
	if( prop != NULL )
		data.terrain.unloadMaps = toBool( PCHAR_CAST prop );
	else
		data.terrain.unloadMaps = true;


	return true;
}

bool WorldConfigParser::parseMailStuff( _xmlNode *node )
{
	xmlChar *prop = NULL;
	prop = xmlGetProp( node, BAD_CAST "reload_delay" );
	if( prop != NULL )
		data.mail.reloadDelay = toBool( PCHAR_CAST prop );
	else
		data.mail.reloadDelay = false;

	prop = xmlGetProp( node, BAD_CAST "disable_postage_costs" );
	if( prop != NULL )
		data.mail.disablePostageCost = toBool( PCHAR_CAST prop );
	else
		data.mail.disablePostageCost = false;

	prop = xmlGetProp( node, BAD_CAST "disable_postage_cost_for_gm" );
	if( prop != NULL )
		data.mail.disableGMPostageCost = toBool( PCHAR_CAST prop );
	else
		data.mail.disableGMPostageCost = true;

	prop = xmlGetProp( node, BAD_CAST "disable_postage_delay_items" );
	if( prop != NULL )
		data.mail.disablePostageDelayItems = toBool( PCHAR_CAST prop );
	else
		data.mail.disablePostageDelayItems = true;

	prop = xmlGetProp( node, BAD_CAST "disable_message_expiry" );
	if( prop != NULL )
		data.mail.disableMessageExpiry = toBool( PCHAR_CAST prop );
	else
		data.mail.disableMessageExpiry = false;

	prop = xmlGetProp( node, BAD_CAST "enable_interfaction_mail" );
	if( prop != NULL )
		data.mail.enableInterfactionMail = toBool( PCHAR_CAST prop );
	else
		data.mail.enableInterfactionMail = false;

	prop = xmlGetProp( node, BAD_CAST "enable_interfaction_mail_for_gm" );
	if( prop != NULL )
		data.mail.enableGMInterfactionMail = toBool( PCHAR_CAST prop );
	else
		data.mail.enableGMInterfactionMail = true;

	return true;
}

bool WorldConfigParser::parseStatDumperStuff( _xmlNode *node )
{
	xmlChar *prop = NULL;
	prop = xmlGetProp( node, BAD_CAST "file_name" );
	if( prop != NULL )
		data.statDumper.fileName = PCHAR_CAST prop;
	else
		data.statDumper.fileName = "stats.xml";

	prop = xmlGetProp( node, BAD_CAST "interval" );
	if( prop != NULL )
		data.statDumper.interval = toULong( PCHAR_CAST prop );
	else
		data.statDumper.interval = 120000;

	return true;
}

bool WorldConfigParser::parseStartupStuff( _xmlNode *node )
{
	xmlChar *prop = NULL;
	prop = xmlGetProp( node, BAD_CAST "preloading" );
	if( prop != NULL )
		data.startup.preloading = toBool( PCHAR_CAST prop );
	else
		data.startup.preloading = false;

	prop = xmlGetProp( node, BAD_CAST "background_loot_loading" );
	if( prop != NULL )
		data.startup.backgroundLootLoading = toBool( PCHAR_CAST prop );
	else
		data.startup.backgroundLootLoading = true;

	prop = xmlGetProp( node, BAD_CAST "enable_multithreaded_loading" );
	if( prop != NULL )
		data.startup.enableMultithreadedLoading = toBool( PCHAR_CAST prop );
	else
		data.startup.enableMultithreadedLoading = true;

	prop = xmlGetProp( node, BAD_CAST "enable_spell_id_dump" );
	if( prop != NULL )
		data.startup.enableSpellIdDump = toBool( PCHAR_CAST prop );
	else
		data.startup.enableSpellIdDump = false;

	prop = xmlGetProp( node, BAD_CAST "load_additional_tables" );
	if( prop != NULL )
		data.startup.loadAdditionalTables = PCHAR_CAST prop;
	else
		data.startup.loadAdditionalTables = "";

	return true;
}

bool WorldConfigParser::parseSpamProtectionStuff( _xmlNode *node )
{
	xmlChar *prop = NULL;
	prop = xmlGetProp( node, BAD_CAST "lines" );
	if( prop != NULL )
		data.spamProtection.lines = toUInt( PCHAR_CAST prop );
	else
		data.spamProtection.lines = 0;

	prop = xmlGetProp( node, BAD_CAST "seconds" );
	if( prop != NULL )
		data.spamProtection.seconds = toUInt( PCHAR_CAST prop );
	else
		data.spamProtection.seconds = 0;

	prop = xmlGetProp( node, BAD_CAST "send_message" );
	if( prop != NULL )
		data.spamProtection.sendMessage = toBool( PCHAR_CAST prop );
	else
		data.spamProtection.sendMessage = false;

	return true;
}

bool WorldConfigParser::parseSecurityStuff( _xmlNode *node )
{
	xmlChar *prop = NULL;
	prop = xmlGetProp( node, BAD_CAST "disable_pings" );
	if( prop != NULL )
		data.security.disablePings = toBool( PCHAR_CAST prop );
	else
		data.security.disablePings = false;

	prop = xmlGetProp( node, BAD_CAST "remote_password" );
	if( prop != NULL )
		data.security.remotePassword = PCHAR_CAST prop;
	else
		data.security.remotePassword = "change_me_world";

	return true;
}

bool WorldConfigParser::parseHackDetectionStuff( _xmlNode *node )
{
	xmlChar *prop = NULL;
	prop = xmlGetProp( node, BAD_CAST "teleport" );
	if( prop != NULL )
		data.hackDetection.teleport = toBool( PCHAR_CAST prop );
	else
		data.hackDetection.teleport = true;

	prop = xmlGetProp( node, BAD_CAST "speed" );
	if( prop != NULL )
		data.hackDetection.speed = toBool( PCHAR_CAST prop );
	else
		data.hackDetection.speed = true;

	prop = xmlGetProp( node, BAD_CAST "fall_damage" );
	if( prop != NULL )
		data.hackDetection.fallDamage = toBool( PCHAR_CAST prop );
	else
		data.hackDetection.fallDamage = true;

	prop = xmlGetProp( node, BAD_CAST "flight" );
	if( prop != NULL )
		data.hackDetection.flight = toBool( PCHAR_CAST prop );
	else
		data.hackDetection.flight = true;

	prop = xmlGetProp( node, BAD_CAST "flight_threshold" );
	if( prop != NULL )
		data.hackDetection.flightThreshold = toUInt( PCHAR_CAST prop );
	else
		data.hackDetection.flightThreshold = 10;

	prop = xmlGetProp( node, BAD_CAST "disable_on_gm" );
	if( prop != NULL )
		data.hackDetection.disableOnGM = toBool( PCHAR_CAST prop );
	else
		data.hackDetection.disableOnGM = true;

	return true;
}

bool WorldConfigParser::parsePeriodsStuff( _xmlNode *node )
{
	xmlChar *prop = NULL;
	prop = xmlGetProp( node, BAD_CAST "honor_update" );
	if( prop != NULL )
		data.periods.honorUpdate = PCHAR_CAST prop;
	else
		data.periods.honorUpdate = "daily";

	prop = xmlGetProp( node, BAD_CAST "arena_update" );
	if( prop != NULL )
		data.periods.arenaUpdate = PCHAR_CAST prop;
	else
		data.periods.arenaUpdate = "weekly";

	prop = xmlGetProp( node, BAD_CAST "daily_update" );
	if( prop != NULL )
		data.periods.dailyUpdate = PCHAR_CAST prop;
	else
		data.periods.dailyUpdate = "daily";

	return true;
}

bool WorldConfigParser::parseChannelsStuff( _xmlNode *node )
{
	xmlChar *prop = NULL;
	prop = xmlGetProp( node, BAD_CAST "banned_channels" );
	if( prop != NULL )
		data.channels.bannedChannels = PCHAR_CAST prop;
	else
		data.channels.bannedChannels = "";

	prop = xmlGetProp( node, BAD_CAST "minimum_level_required" );
	if( prop != NULL )
		data.channels.minReqLevel = PCHAR_CAST prop;
	else
		data.channels.minReqLevel = "";

	return true;
}

bool WorldConfigParser::parseRemoteConsoleStuff( _xmlNode *node )
{
	xmlChar *prop = NULL;
	prop = xmlGetProp( node, BAD_CAST "enabled" );
	if( prop != NULL )
		data.remoteConsole.enabled = toBool( PCHAR_CAST prop );
	else
		data.remoteConsole.enabled = false;

	prop = xmlGetProp( node, BAD_CAST "listen_on_address" );
	if( prop != NULL )
		data.remoteConsole.address = PCHAR_CAST prop;
	else
		data.remoteConsole.address = "0.0.0.0";

	prop = xmlGetProp( node, BAD_CAST "listen_on_port" );
	if( prop != NULL )
		data.remoteConsole.port = toUShort( PCHAR_CAST prop );
	else
		data.remoteConsole.port = 8092;

	return true;
}

bool WorldConfigParser::parseMovementStuff( _xmlNode *node )
{
	xmlChar *prop = NULL;
	prop = xmlGetProp( node, BAD_CAST "flush_interval" );
	if( prop != NULL )
		data.movement.flushInterval = toUInt( PCHAR_CAST prop );
	else
		data.movement.flushInterval = 1000;

	prop = xmlGetProp( node, BAD_CAST "compression_rate" );
	if( prop != NULL )
		data.movement.compressionRate = toUInt( PCHAR_CAST prop );
	else
		data.movement.compressionRate = 1;

	prop = xmlGetProp( node, BAD_CAST "compression_threshold" );
	if( prop != NULL )
		data.movement.compressionThreshold = toFloat( PCHAR_CAST prop );
	else
		data.movement.compressionThreshold = 30.0f;

	prop = xmlGetProp( node, BAD_CAST "compression_threshold_creatures" );
	if( prop != NULL )
		data.movement.compressionThresholdCreatures = toFloat( PCHAR_CAST prop );
	else
		data.movement.compressionThresholdCreatures = 10.0f;

	return true;
}

bool WorldConfigParser::parseLocalizationStuff( _xmlNode *node )
{
	xmlChar *prop = NULL;
	prop = xmlGetProp( node, BAD_CAST "locale_bindings" );
	if( prop != NULL )
		data.localization.localeBindings = PCHAR_CAST prop;
	else
		data.localization.localeBindings = "";

	return true;
}

bool WorldConfigParser::parseInstanceHandlingStuff( _xmlNode *node )
{
	xmlChar *prop = NULL;
	prop = xmlGetProp( node, BAD_CAST "take_group_leader_id" );
	if( prop != NULL )
		data.instanceHandling.takeGroupLeaderId = toBool( PCHAR_CAST prop );
	else
		data.instanceHandling.takeGroupLeaderId = true;

	prop = xmlGetProp( node, BAD_CAST "sliding_expiration" );
	if( prop != NULL )
		data.instanceHandling.slidingExpiration = toBool( PCHAR_CAST prop );
	else
		data.instanceHandling.slidingExpiration = false;

	prop = xmlGetProp( node, BAD_CAST "daily_heroic_instance_reset_hour" );
	if( prop != NULL )
		data.instanceHandling.dailyHeroicResetHour = toUInt( PCHAR_CAST prop );
	else
		data.instanceHandling.dailyHeroicResetHour = 5;

	prop = xmlGetProp( node, BAD_CAST "check_trigger_prerequisites" );
	if( prop != NULL )
		data.instanceHandling.checkTriggerPrerequisites = toBool( PCHAR_CAST prop );
	else
		data.instanceHandling.checkTriggerPrerequisites = true;

	return true;
}

bool WorldConfigParser::parseBGStuff( _xmlNode *node )
{
	xmlChar *prop = NULL;
	prop = xmlGetProp( node, BAD_CAST "alterac_valley_minimum_players" );
	if( prop != NULL )
		data.battlegrounds.AV_MIN = toUInt( PCHAR_CAST prop );
	else
		data.battlegrounds.AV_MIN = 10;

	prop = xmlGetProp( node, BAD_CAST "alterac_valley_maximum_players" );
	if( prop != NULL )
		data.battlegrounds.AV_MAX = toUInt( PCHAR_CAST prop );
	else
		data.battlegrounds.AV_MAX = 40;

	prop = xmlGetProp( node, BAD_CAST "arathi_basin_minimum_players" );
	if( prop != NULL )
		data.battlegrounds.AB_MIN = toUInt( PCHAR_CAST prop );
	else
		data.battlegrounds.AB_MIN = 5;

	prop = xmlGetProp( node, BAD_CAST "arathi_basin_maximum_players" );
	if( prop != NULL )
		data.battlegrounds.AB_MAX = toUInt( PCHAR_CAST prop );
	else
		data.battlegrounds.AB_MAX = 15;

	prop = xmlGetProp( node, BAD_CAST "eye_of_the_storm_minimum_players" );
	if( prop != NULL )
		data.battlegrounds.EOTS_MIN = toUInt( PCHAR_CAST prop );
	else
		data.battlegrounds.EOTS_MIN = 5;

	prop = xmlGetProp( node, BAD_CAST "eye_of_the_storm_maximum_players" );
	if( prop != NULL )
		data.battlegrounds.EOTS_MAX = toUInt( PCHAR_CAST prop );
	else
		data.battlegrounds.EOTS_MAX = 15;

	prop = xmlGetProp( node, BAD_CAST "warsong_gulch_minimum_players" );
	if( prop != NULL )
		data.battlegrounds.WSG_MIN = toUInt( PCHAR_CAST prop );
	else
		data.battlegrounds.WSG_MIN = 5;

	prop = xmlGetProp( node, BAD_CAST "warsong_gulch_maximum_players" );
	if( prop != NULL )
		data.battlegrounds.WSG_MAX = toUInt( PCHAR_CAST prop );
	else
		data.battlegrounds.WSG_MAX = 10;

	prop = xmlGetProp( node, BAD_CAST "strand_of_the_ancients_minimum_players" );
	if( prop != NULL )
		data.battlegrounds.SOTA_MIN = toUInt( PCHAR_CAST prop );
	else
		data.battlegrounds.SOTA_MIN = 5;

	prop = xmlGetProp( node, BAD_CAST "strand_of_the_ancients_maximum_players" );
	if( prop != NULL )
		data.battlegrounds.SOTA_MAX = toUInt( PCHAR_CAST prop );
	else
		data.battlegrounds.SOTA_MAX = 15;

	return true;
}

bool WorldConfigParser::parseArenaStuff( _xmlNode *node )
{
	xmlChar *prop = NULL;
	prop = xmlGetProp( node, BAD_CAST "season" );
	if( prop != NULL )
		data.arena.season = toUInt( PCHAR_CAST prop );
	else
		data.arena.season = 8;

	prop = xmlGetProp( node, BAD_CAST "is_in_progress" );
	if( prop != NULL )
		data.arena.inProgress = toBool( PCHAR_CAST prop );
	else
		data.arena.inProgress = false;

	return true;
}

bool WorldConfigParser::parseGMStuff( _xmlNode *node )
{
	xmlChar *prop = NULL;
	prop = xmlGetProp( node, BAD_CAST "start_on_gm_island" );
	if( prop != NULL )
		data.gm.startOnGMIsland = toBool( PCHAR_CAST prop );
	else
		data.gm.startOnGMIsland = true;

	prop = xmlGetProp( node, BAD_CAST "disable_achievements" );
	if( prop != NULL )
		data.gm.disableAchievements = toBool( PCHAR_CAST prop );
	else
		data.gm.disableAchievements = true;

	return true;
}

bool WorldConfigParser::parseLimitStuff( _xmlNode *node )
{
	xmlChar *prop = NULL;
	prop = xmlGetProp( node, BAD_CAST "enable" );
	if( prop != NULL )
		data.limits.enabled = toBool( PCHAR_CAST prop );
	else
		data.limits.enabled = true;

	prop = xmlGetProp( node, BAD_CAST "auto_attack_damage" );
	if( prop != NULL )
		data.limits.autoAttackDmg = toUInt( PCHAR_CAST prop );
	else
		data.limits.autoAttackDmg = 10000;

	prop = xmlGetProp( node, BAD_CAST "spell_damage" );
	if( prop != NULL )
		data.limits.spellDmg = toUInt( PCHAR_CAST prop );
	else
		data.limits.spellDmg = 30000;

	prop = xmlGetProp( node, BAD_CAST "health" );
	if( prop != NULL )
		data.limits.hp = toUInt( PCHAR_CAST prop );
	else
		data.limits.hp = 500000;

	prop = xmlGetProp( node, BAD_CAST "mana" );
	if( prop != NULL )
		data.limits.mana = toUInt( PCHAR_CAST prop );
	else
		data.limits.mana = 500000;

	prop = xmlGetProp( node, BAD_CAST "disconnect_if_values_exceeded" );
	if( prop != NULL )
		data.limits.disconnect = toBool( PCHAR_CAST prop );
	else
		data.limits.disconnect = false;

	prop = xmlGetProp( node, BAD_CAST "broadcast_message_to_gms" );
	if( prop != NULL )
		data.limits.report = toBool( PCHAR_CAST prop );
	else
		data.limits.report = true;

	return true;
}

