#include "LogonConfigParser.h"
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

enum LogonConfigError
{
	ERR_ALL_OK,
	ERR_CANNOT_OPEN_FILE,
	ERR_NOT_A_CONFIG_FILE,
	ERR_NOT_LOGON_CONFIG,
	ERR_WRONG_VERSION,
	ERR_DB_NO_HOST,
	ERR_DB_NO_USERNAME,
	ERR_DB_NO_PASSWORD,
	ERR_DB_NO_DB,
	ERR_HOST_NO_LOGON_ADDR,
	ERR_HOST_NO_LOGON_PORT,
	ERR_HOST_NO_IS_ADDR,
	ERR_HOST_NO_IS_PORT,
	ERR_SEC_NO_PASSWORD,
	ERR_SEC_NO_IP,
	ERR_SEC_NO_MOD_IP,
	ERR_NO_DB_SETTINGS,
	ERR_NO_HOST_SETTINGS,
	ERR_NO_SECURITY_SETTINGS,
	MAX_ERROR
};

const char* errorString[] = 
{
	"No error.",
	"Cannot open file",
	"Not a config file",
	"Not a logon config file",
	"Logon database, hostname couldn't be loaded",
	"Logon database, username couldn't be loaded",
	"Logon database, password couldn't be loaded",
	"Logon database, database name couldn't be loaded",
	"Host, logon address couldn't be loaded",
	"Host, logon port couldn't be loaded",
	"Host, interserver address couldn't be loaded",
	"Host, interserver port couldn't be loaded",
	"Security, remote password couldn't be loaded",
	"Security, allowed ip ranges couldn't be loaded",
	"Security, allowed mod ip ranges couldn't be loaded",
	"Database connection settings couldn't be loaded",
	"Host settings couldn't be loaded",
	"Security settings couldn't be loaded",
	"Wrong version"
};

LogonConfigParser::LogonConfigParser()
{
	lastError        = ERR_ALL_OK;
	dbPartDone       = false;
	hostPartDone     = false;
	securityPartDone = false;
}

LogonConfigParser::~LogonConfigParser()
{
}

const char* LogonConfigParser::getLastError() const
{
	return errorString[ lastError ];
}

bool LogonConfigParser::parseFile( const std::string &name )
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
		if( xmlStrcmp( node->name, BAD_CAST "logondatabase" ) == 0 )
			ok = parseDBPart( node );
		else
		if( xmlStrcmp( node->name, BAD_CAST "host" ) == 0 )
			ok = parseHostPart( node );
		else
		if( xmlStrcmp( node->name, BAD_CAST "log" ) == 0 )
			ok = parseLogPart( node );
		else
		if( xmlStrcmp( node->name, BAD_CAST "rates" ) == 0 )
			ok = parseRatesPart( node );
		else
		if( xmlStrcmp( node->name, BAD_CAST "security" ) == 0 )
			ok = parseSecurityPart( node );

		if( !ok )
			break;

		node = node->next;
	}

	xmlFreeDoc( document );

	if( !ok )
		return false;

	if( !dbPartDone )
	{
		lastError = ERR_NO_DB_SETTINGS;
		ok = false;
	}
	else
	if( !hostPartDone )
	{
		lastError = ERR_NO_HOST_SETTINGS;
		ok = false;
	}
	else
	if( !securityPartDone )
	{
		lastError = ERR_NO_SECURITY_SETTINGS;
		ok = false;
	}
	
	if( !ok )
		return false;
	else
		return true;
}

bool LogonConfigParser::isConfig( _xmlNode *node )
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
	if( xmlStrcmp( prop, BAD_CAST "logon" ) != 0 )
	{
		lastError = ERR_NOT_LOGON_CONFIG;
		return false;
	}

	prop = xmlGetProp( node, BAD_CAST "version" );
	if( prop == NULL )
	{
		lastError = ERR_NOT_A_CONFIG_FILE;
		return false;
	}
	if( xmlStrcmp( prop, BAD_CAST requiredVersion.c_str() ) )
	{
		lastError = ERR_WRONG_VERSION;
		return false;
	}

	return true;
}

bool LogonConfigParser::parseDBPart( _xmlNode *node )
{
	xmlChar *prop = NULL;

	prop = xmlGetProp( node, BAD_CAST "hostname" );
	if( prop == NULL )
	{
		lastError = ERR_DB_NO_HOST;
		return false;
	}
	data.logondb.host = reinterpret_cast< char* >( prop );

	prop = xmlGetProp( node, BAD_CAST "username" );
	if( prop == NULL )
	{
		lastError = ERR_DB_NO_USERNAME;
		return false;
	}
	data.logondb.username = reinterpret_cast< char* >( prop );

	prop = xmlGetProp( node, BAD_CAST "password" );
	if( prop == NULL )
	{
		lastError = ERR_DB_NO_PASSWORD;
		return false;
	}
	data.logondb.password = reinterpret_cast< char* >( prop );

	prop = xmlGetProp( node, BAD_CAST "database" );
	if( prop == NULL )
	{
		lastError = ERR_DB_NO_DB;
		return false;
	}
	data.logondb.database = reinterpret_cast< char* >( prop );

	prop = xmlGetProp( node, BAD_CAST "port" );
	if( prop == NULL )
		data.logondb.port = 3306u;
	else
		data.logondb.port = static_cast< unsigned int >( atoi( reinterpret_cast< char* >( prop ) ) );
		
	dbPartDone = true;
	return true;
}

bool LogonConfigParser::parseHostPart( _xmlNode *node )
{
	xmlChar *prop = NULL;

	prop = xmlGetProp( node, BAD_CAST "logon_address" );
	if( prop == NULL )
	{
		lastError = ERR_HOST_NO_LOGON_ADDR;
		return false;
	}
	data.host.logon_address = reinterpret_cast< char* >( prop );

	prop = xmlGetProp( node, BAD_CAST "logon_port" );
	if( prop == NULL )
	{
		lastError = ERR_HOST_NO_LOGON_PORT;
		return false;
	}
	data.host.logon_port = static_cast< unsigned int >( atoi( reinterpret_cast< char* >( prop ) ) );

	prop = xmlGetProp( node, BAD_CAST "is_address" );
	if( prop == NULL )
	{
		lastError = ERR_HOST_NO_IS_ADDR;
		return false;
	}
	data.host.is_address = reinterpret_cast< char* >( prop );

	prop = xmlGetProp( node, BAD_CAST "is_port" );
	if( prop == NULL )
	{
		lastError = ERR_HOST_NO_IS_PORT;
		return false;
	}
	data.host.is_port = static_cast< unsigned int >( atoi( reinterpret_cast< char* >( prop ) ) );

	hostPartDone = false;
	return true;
}

bool LogonConfigParser::parseLogPart( _xmlNode *node )
{
	xmlChar *prop = xmlGetProp( node, BAD_CAST "level" );
	if( prop == NULL )
		data.log.level = 0;
	else
		data.log.level = static_cast< unsigned int >( atoi( reinterpret_cast< char* >( prop ) ) );

	return true;
}

bool LogonConfigParser::parseRatesPart( _xmlNode *node )
{
	xmlChar *prop = xmlGetProp( node, BAD_CAST "account_refresh" );
	if( prop == NULL )
		data.rates.account_refresh = 600u;
	else
		data.rates.account_refresh = static_cast< unsigned int >( atoi( reinterpret_cast< char* >( prop ) ) );

	return true;
}

bool LogonConfigParser::parseSecurityPart( _xmlNode *node )
{
	xmlChar *prop = NULL;

	prop = xmlGetProp( node, BAD_CAST "remote_password" );
	if( prop == NULL )
	{
		lastError = ERR_SEC_NO_PASSWORD;
		return false;
	}
	data.security.remote_password = reinterpret_cast< char* >( prop );

	prop = xmlGetProp( node, BAD_CAST "allowed_ip_ranges" );
	if( prop == NULL )
	{
		lastError = ERR_SEC_NO_IP;
		return false;
	}
	data.security.allowed_ip_ranges = reinterpret_cast< char* >( prop );

	prop = xmlGetProp( node, BAD_CAST "allowed_mod_ip_ranges" );
	if( prop == NULL )
	{
		lastError = ERR_SEC_NO_MOD_IP;
		return false;
	}
	data.security.allowed_mod_ip_ranges = reinterpret_cast< char* >( prop );

	securityPartDone = true;
	return true;
}

