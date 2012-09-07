#ifndef WORLD_CONFIG_PARSER
#define WORLD_CONFIG_PARSER

#include "WorldConfig.h"

struct _xmlNode;

class WorldConfigParser
{
public:
	WorldConfigParser();
	~WorldConfigParser();

	bool parseFile( const std::string &name );
	bool isConfig( _xmlNode *node );

	const WorldConfigData& getWorldConfigData() const{ return data; }
	void setRequiredVersion( const std::string &s ){ requiredVersion = s; }

private:
	bool parseWorldDBStuff( _xmlNode *node );
	bool parseCharDBStuff( _xmlNode *node );
	bool parseHostStuff( _xmlNode *node );
	bool parseLogStuff( _xmlNode *node );
	bool parseServerStuff( _xmlNode *node );
	bool parseAnnounceStuff( _xmlNode *node );
	bool parseRatesStuff( _xmlNode *node );
	bool parseGMClientStuff( _xmlNode *node );
	bool parseTerrainStuff( _xmlNode *node );
	bool parseMailStuff( _xmlNode *node );
	bool parseStatDumperStuff( _xmlNode *node );
	bool parseStartupStuff( _xmlNode *node );
	bool parseSpamProtectionStuff( _xmlNode *node );
	bool parseSecurityStuff( _xmlNode *node );
	bool parseHackDetectionStuff( _xmlNode *node );
	bool parsePeriodsStuff( _xmlNode *node );
	bool parseChannelsStuff( _xmlNode *node );
	bool parseRemoteConsoleStuff( _xmlNode *node );
	bool parseMovementStuff( _xmlNode *node );

	bool parseLocalizationStuff( _xmlNode *node );
	bool parseInstanceHandlingStuff( _xmlNode *node );
	bool parseBGStuff( _xmlNode *node );
	bool parseArenaStuff( _xmlNode *node );
	bool parseGMStuff( _xmlNode *node );
	bool parseLimitStuff( _xmlNode *node );

	WorldConfigData data;
	std::string requiredVersion;
};

#endif

