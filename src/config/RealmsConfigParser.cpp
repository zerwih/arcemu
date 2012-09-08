#include "RealmsConfigParser.h"
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

namespace
{
	enum RealmsConfigErrors
	{
		ERR_ALL_OK,
		ERR_CANNOT_OPEN_FILE,
		ERR_NOT_A_CONFIG_FILE,
		ERR_NOT_A_REALMS_CFG,
		ERR_WRONG_VERSION,
		ERR_NO_LOGON_ADDR,
		ERR_NO_LOGON_NAME,
		ERR_NO_REALMS_COUNT,
		ERR_NO_REALM_ID,
		ERR_NO_REALM_NAME,
		ERR_NO_REALM_ADDR_PORT,
		ERR_WRONG_REALM_ADDR_PORT,
		ERR_NO_REALM_ICON,
		ERR_NO_REALM_POPULATION,
		ERR_NO_REALM_TZ,
		ERR_MAX_ERROR
	};

	const char* errorStrings[] =
	{
		"No errors",
		"Cannot open file",
		"Not a config file",
		"Not a realms config file",
		"Wrong version",
		"No logon address specified",
		"No logon name specified",
		"No realms count",
		"No realm Id specified",
		"No realm name specified",
		"No realm address and port specified",
		"Malformed realm address and port",
		"No realms icon specified",
		"No realm population specified",
		"No realm timezone specified"
	};
}


RealmsConfigParser::RealmsConfigParser()
{
	lastError = ERR_ALL_OK;
}

RealmsConfigParser::~RealmsConfigParser()
{
}

const char* RealmsConfigParser::getLastError() const
{
	return errorStrings[ lastError ];
}

bool RealmsConfigParser::parseFile( const std::string &name )
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
		lastError = ERR_NOT_A_CONFIG_FILE;
		xmlFreeDoc( document );
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
		if( xmlStrcmp( node->name, BAD_CAST "logon" ) == 0 )
			ok = parseLogon( node );
		else
		if( xmlStrcmp( node->name, BAD_CAST "realm" ) == 0 )
			ok = parseRealm( node );

		if( !ok )
			break;

		node = node->next;
	}

	xmlFreeDoc( document );

	if( ok )
		return true;
	else
		return false;
}

bool RealmsConfigParser::isConfig( _xmlNode *node )
{
	if( xmlStrcmp( node->name, BAD_CAST "config" ) != 0 )
	{
		lastError = ERR_NOT_A_CONFIG_FILE;
		return false;
	}

	xmlChar *prop = NULL;
	prop = xmlGetProp( node, BAD_CAST "type" );
	if( prop == NULL )
	{
		lastError = ERR_NOT_A_CONFIG_FILE;
		return false;
	}

	if( xmlStrcmp( prop, BAD_CAST "realms" ) != 0 )
	{
		lastError = ERR_NOT_A_REALMS_CFG;
		return false;
	}

	prop = xmlGetProp( node, BAD_CAST "version" );
	if( prop == NULL )
	{
		lastError = ERR_NOT_A_REALMS_CFG;
		return false;
	}

	if( xmlStrcmp( prop, BAD_CAST requiredVersion.c_str() ) != 0 )
	{
		lastError = ERR_WRONG_VERSION;
		return false;
	}


	return true;
}

bool RealmsConfigParser::parseLogon( _xmlNode *node )
{
	xmlChar *prop = NULL;
	
	prop = xmlGetProp( node, BAD_CAST "address" );
	if( prop == NULL )
	{
		lastError = ERR_NO_LOGON_ADDR;
		return false;
	}
	data.logon.address = reinterpret_cast< char* >( prop );

	prop = xmlGetProp( node, BAD_CAST "port" );
	if( prop == NULL )
		data.logon.port = 8093;
	else
		data.logon.port = atoi( reinterpret_cast< char* >( prop ) );

	prop = xmlGetProp( node, BAD_CAST "name" );
	if( prop == NULL )
	{
		lastError = ERR_NO_LOGON_NAME;
		return false;
	}
	data.logon.name = reinterpret_cast< char* >( prop );

	prop = xmlGetProp( node, BAD_CAST "realm_count" );
	if( prop == NULL )
	{
		lastError = ERR_NO_REALMS_COUNT;
		return false;
	}
	data.logon.realmCount = atoi( reinterpret_cast< char* >( prop ) );

	return true;
}

bool RealmsConfigParser::parseRealm( _xmlNode *node )
{
	xmlChar *prop = NULL;

	prop = xmlGetProp( node, BAD_CAST "id" );
	if( prop == NULL )
	{
		lastError = ERR_NO_REALM_ID;
		return false;
	}

	RealmsConfigData::RealmSettings realm;
	realm.id = atoi( reinterpret_cast< char* >( prop ) );

	prop = xmlGetProp( node, BAD_CAST "name" );
	if( prop == NULL )
	{
		lastError = ERR_NO_REALM_NAME;
		return false;
	}
	realm.name = reinterpret_cast< char* >( prop );

	prop = xmlGetProp( node, BAD_CAST "address_and_port" );
	if( prop == NULL )
	{
		lastError = ERR_NO_REALM_ADDR_PORT;
		return false;
	}
	realm.addressAndPort = reinterpret_cast< char* >( prop );

	if( realm.addressAndPort.find( ":" ) == std::string::npos )
	{
		lastError = ERR_WRONG_REALM_ADDR_PORT;
		return false;
	}

	prop = xmlGetProp( node, BAD_CAST "icon" );
	if( prop == NULL )
	{
		lastError = ERR_NO_REALM_ICON;
		return false;
	}
	realm.icon = reinterpret_cast< char* >( prop );

	prop = xmlGetProp( node, BAD_CAST "population" );
	if( prop == NULL )
	{
		lastError = ERR_NO_REALM_POPULATION;
		return false;
	}
	realm.population = atof( reinterpret_cast< char* >( prop ) );

	prop = xmlGetProp( node, BAD_CAST "timezone" );
	if( prop == NULL )
	{
		lastError = ERR_NO_REALM_TZ;
		return false;
	}
	realm.timezone = atoi( reinterpret_cast< char* >( prop ) );

	data.realms.push_back( realm );

	return true;
}

