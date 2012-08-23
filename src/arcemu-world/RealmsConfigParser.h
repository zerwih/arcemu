#ifndef REALMS_CONFIG_PARSER_H
#define REALMS_CONFIG_PARSER_H

#include "RealmsConfig.h"

struct _xmlNode;

class RealmsConfigParser
{
public:
	RealmsConfigParser();
	~RealmsConfigParser();

	bool parseFile( const std::string &name );
	bool isConfig( _xmlNode *node );
	const RealmsConfigData& getData() const{ return data; }

private:
	bool parseLogon( _xmlNode *node );
	bool parseRealm( _xmlNode *node );

	RealmsConfigData data;
};

#endif
