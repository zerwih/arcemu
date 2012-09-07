#include "RealmsConfigParser.h"
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>


RealmsConfigParser::RealmsConfigParser()
{
}

RealmsConfigParser::~RealmsConfigParser()
{
}

bool RealmsConfigParser::parseFile( const std::string &name )
{
	xmlDocPtr document = xmlParseFile( name.c_str() );
	if( document == NULL )
		return false;

	xmlNodePtr root = xmlDocGetRootElement( document );
	if( root == NULL )
	{
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
		return false;

	xmlChar *prop = NULL;
	prop = xmlGetProp( node, BAD_CAST "type" );
	if( prop == NULL )
		return false;

	if( xmlStrcmp( prop, BAD_CAST "realms" ) != 0 )
		return false;

	prop = xmlGetProp( node, BAD_CAST "version" );
	if( prop == NULL )
		return false;

	if( xmlStrcmp( prop, BAD_CAST requiredVersion.c_str() ) != 0 )
		return false;


	return true;
}

bool RealmsConfigParser::parseLogon( _xmlNode *node )
{
	xmlChar *prop = NULL;
	
	prop = xmlGetProp( node, BAD_CAST "address" );
	if( prop == NULL )
		return false;
	data.logon.address = reinterpret_cast< char* >( prop );

	prop = xmlGetProp( node, BAD_CAST "port" );
	if( prop == NULL )
		data.logon.port = 8093;
	else
		data.logon.port = atoi( reinterpret_cast< char* >( prop ) );

	prop = xmlGetProp( node, BAD_CAST "name" );
	if( prop == NULL )
		return false;
	data.logon.name = reinterpret_cast< char* >( prop );

	prop = xmlGetProp( node, BAD_CAST "realm_count" );
	if( prop == NULL )
		return false;
	data.logon.realmCount = atoi( reinterpret_cast< char* >( prop ) );

	return true;
}

bool RealmsConfigParser::parseRealm( _xmlNode *node )
{
	xmlChar *prop = NULL;

	prop = xmlGetProp( node, BAD_CAST "id" );
	if( prop == NULL )
		return false;

	RealmsConfigData::RealmSettings realm;
	realm.id = atoi( reinterpret_cast< char* >( prop ) );

	prop = xmlGetProp( node, BAD_CAST "name" );
	if( prop == NULL )
		return false;
	realm.name = reinterpret_cast< char* >( prop );

	prop = xmlGetProp( node, BAD_CAST "address_and_port" );
	if( prop == NULL )
		return false;
	realm.addressAndPort = reinterpret_cast< char* >( prop );

	prop = xmlGetProp( node, BAD_CAST "icon" );
	if( prop == NULL )
		return false;
	realm.icon = reinterpret_cast< char* >( prop );

	prop = xmlGetProp( node, BAD_CAST "population" );
	if( prop == NULL )
		return false;
	realm.population = atof( reinterpret_cast< char* >( prop ) );

	prop = xmlGetProp( node, BAD_CAST "timezone" );
	if( prop == NULL )
		return false;
	realm.timezone = atoi( reinterpret_cast< char* >( prop ) );

	data.realms.push_back( realm );

	return true;
}

