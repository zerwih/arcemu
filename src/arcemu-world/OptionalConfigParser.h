#ifndef OPTIONAL_CONFIG_PARSER_H
#define OPTIONAL_CONFIG_PARSER_H

#include "OptionalConfig.h"
#include <string>

struct _xmlNode;

class OptionalConfigParser
{
public:
	OptionalConfigParser();
	~OptionalConfigParser();

	bool parseFile( const std::string &name );
	const OptionalConfigData& getData() const{ return data; }
	bool isConfig( _xmlNode *node );
	void setRequiredVersion( const std::string &s ){ requiredVersion = s; }

private:
	bool parseOptional( _xmlNode *node );
	bool parseInterfaction( _xmlNode *node );
	bool parseAnnColor( _xmlNode *node );
	bool parseExtra( _xmlNode *node );
	bool parseGM( _xmlNode *node );
	bool parseCommonScheduler( _xmlNode *node );
	bool parseClassOptions( _xmlNode *node );
	bool parseGoldSettings( _xmlNode *node );

	OptionalConfigData data;
	std::string requiredVersion;
};

#endif

