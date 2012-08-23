#include "OptionalConfigParser.h"
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>


bool fromString( std::string s )
{
	if( s == "true" )
		return true;

	if( s[0] == '1' )
		return true;

	return false;
}

#define PCHAR_CAST (char*)

OptionalConfigParser::OptionalConfigParser()
{
}

OptionalConfigParser::~OptionalConfigParser()
{
}

bool OptionalConfigParser::parseFile( const std::string &name )
{
	xmlDocPtr document = xmlParseFile( name.c_str() );
	if( document == NULL )
		return false;

	xmlNodePtr root = xmlDocGetRootElement( document );
	if( root == NULL )
		return false;

	if( !isConfig( root ) )
	{
		xmlFreeDoc( document );
		return false;
	}

	bool ok = true;
	xmlNodePtr node = root->children;
	while( node != NULL )
	{
		if( xmlStrcmp( node->name, BAD_CAST "optional" ) == 0 )
			ok = parseOptional( node );
		else
		if( xmlStrcmp( node->name, BAD_CAST "interfaction" ) == 0 )
			ok = parseInterfaction( node );
		else
		if( xmlStrcmp( node->name, BAD_CAST "announce_color" ) == 0 )
			ok = parseAnnColor( node );
		else
		if( xmlStrcmp( node->name, BAD_CAST "extra" ) == 0 )
			ok = parseExtra( node );
		else
		if( xmlStrcmp( node->name, BAD_CAST "game_master" ) == 0 )
			ok = parseGM( node );
		else
		if( xmlStrcmp( node->name, BAD_CAST "common_scheduler" ) == 0 )
			ok = parseCommonScheduler( node );
		else
		if( xmlStrcmp( node->name, BAD_CAST "class_options" ) == 0 )
			ok = parseClassOptions( node );
		else
		if( xmlStrcmp( node->name, BAD_CAST "gold_settings" ) == 0 )
			ok = parseGoldSettings( node );

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

bool OptionalConfigParser::isConfig( _xmlNode *node )
{
	xmlChar *prop = NULL;
	prop = xmlGetProp( node, BAD_CAST "type" );
	if( prop == NULL )
		return false;
	if( xmlStrcmp( prop, BAD_CAST "optional" ) != 0 )
		return false;

	prop = xmlGetProp( node, BAD_CAST "version" );
	if( prop == NULL )
		return false;
	if( xmlStrcmp( prop, BAD_CAST "1" ) != 0 )
		return false;

	return true;
}

bool OptionalConfigParser::parseOptional( _xmlNode *node )
{
	xmlChar *prop = NULL;
	prop = xmlGetProp( node, BAD_CAST "starting_level" );
	if( prop == NULL )
		data.optional.startingLevel = 1;
	else
		data.optional.startingLevel = atoi( PCHAR_CAST prop );

	prop = xmlGetProp( node, BAD_CAST "level_cap" );
	if( prop == NULL )
		data.optional.levelCap = 80;
	else
		data.optional.levelCap = atoi( PCHAR_CAST prop );

	prop = xmlGetProp( node, BAD_CAST "gen_level_cap" );
	if( prop == NULL )
		data.optional.genLevelCap = 80;
	else
		data.optional.genLevelCap = atoi( PCHAR_CAST prop );

	prop = xmlGetProp( node, BAD_CAST "allow_tbc" );
	if( prop == NULL )
		data.optional.allowTBC = true;
	else
		data.optional.allowTBC = fromString( PCHAR_CAST prop );

	prop = xmlGetProp( node, BAD_CAST "anti_master_loot_ninja" );
	if( prop == NULL )
		data.optional.antiMasterLootNinja = false;
	else
		data.optional.antiMasterLootNinja = fromString( PCHAR_CAST prop );

	prop = xmlGetProp( node, BAD_CAST "additional_fun" );
	if( prop == NULL )
		data.optional.additionalFun = false;
	else
		data.optional.additionalFun = fromString( PCHAR_CAST prop );

	prop = xmlGetProp( node, BAD_CAST "dk_starting_talents" );
	if( prop == NULL )
		data.optional.DKStartingTalent = 0;
	else
		data.optional.DKStartingTalent = atoi( PCHAR_CAST prop );

	prop = xmlGetProp( node, BAD_CAST "max_professions" );
	if( prop == NULL )
		data.optional.maxProfessions = 2;
	else
		data.optional.maxProfessions = atoi( PCHAR_CAST prop );


	return true;
}

bool OptionalConfigParser::parseInterfaction( _xmlNode *node )
{
	xmlChar *prop = NULL;

	prop = xmlGetProp( node, BAD_CAST "chat" );
	if( prop == NULL )
		data.interfaction.chat = false;
	else
		data.interfaction.chat = fromString( PCHAR_CAST prop );

	prop = xmlGetProp( node, BAD_CAST "group" );
	if( prop == NULL )
		data.interfaction.group = false;
	else
		data.interfaction.group = fromString( PCHAR_CAST prop );

	prop = xmlGetProp( node, BAD_CAST "guild" );
	if( prop == NULL )
		data.interfaction.guild = false;
	else
		data.interfaction.guild = fromString( PCHAR_CAST prop );

	prop = xmlGetProp( node, BAD_CAST "trade" );
	if( prop == NULL )
		data.interfaction.trade = false;
	else
		data.interfaction.trade = fromString( PCHAR_CAST prop );

	prop = xmlGetProp( node, BAD_CAST "friends" );
	if( prop == NULL )
		data.interfaction.friends = false;
	else
		data.interfaction.friends = fromString( PCHAR_CAST prop );

	prop = xmlGetProp( node, BAD_CAST "misc" );
	if( prop == NULL )
		data.interfaction.misc = false;
	else
		data.interfaction.misc = fromString( PCHAR_CAST prop );

	prop = xmlGetProp( node, BAD_CAST "cross_over_characters" );
	if( prop == NULL )
		data.interfaction.crossoverChars = true;
	else
		data.interfaction.crossoverChars = fromString( PCHAR_CAST prop );

	return true;
}

bool OptionalConfigParser::parseAnnColor( _xmlNode *node )
{
	xmlChar *prop = NULL;
	
	prop = xmlGetProp( node, BAD_CAST "tag" );
	if( prop == NULL )
		data.announceColor.tag = 2;
	else
		data.announceColor.tag = atoi( PCHAR_CAST prop );

	prop = xmlGetProp( node, BAD_CAST "gm_admin_tag" );
	if( prop == NULL )
		data.announceColor.GMAdminTag = 1;
	else
		data.announceColor.GMAdminTag = atoi( PCHAR_CAST prop );

	prop = xmlGetProp( node, BAD_CAST "name" );
	if( prop == NULL )
		data.announceColor.name = 4;
	else
		data.announceColor.name = atoi( PCHAR_CAST prop );

	prop = xmlGetProp( node, BAD_CAST "msg" );
	if( prop == NULL )
		data.announceColor.msg = 6;
	else
		data.announceColor.msg = atoi( PCHAR_CAST prop );

	return true;
}

bool OptionalConfigParser::parseExtra( _xmlNode *node )
{
	xmlChar *prop = NULL;

	prop = xmlGetProp( node, BAD_CAST "instant_80" );
	if( prop == NULL )
		data.extra.instant80 = false;
	else
		data.extra.instant80 = fromString( PCHAR_CAST prop );


	prop = xmlGetProp( node, BAD_CAST "portals" );
	if( prop == NULL )
		data.extra.portals = false;
	else
		data.extra.portals = fromString( PCHAR_CAST prop );

	prop = xmlGetProp( node, BAD_CAST "pvp_token_id" );
	if( prop == NULL )
		data.extra.pvpTokenId = 0;
	else
		data.extra.pvpTokenId = atoi( PCHAR_CAST prop );

	return true;
}

bool OptionalConfigParser::parseGM( _xmlNode *node )
{
	xmlChar *prop = NULL;

	prop = xmlGetProp( node, BAD_CAST "list_only_active_gms" );
	if( prop == NULL )
		data.gm.listOnlyActive = false;
	else
		data.gm.listOnlyActive = fromString( PCHAR_CAST prop );


	prop = xmlGetProp( node, BAD_CAST "hide_permissions" );
	if( prop == NULL )
		data.gm.hidePermissions = false;
	else
		data.gm.hidePermissions = fromString( PCHAR_CAST prop );


	return true;
}

bool OptionalConfigParser::parseCommonScheduler( _xmlNode *node )
{
	xmlChar *prop = NULL;

	prop = xmlGetProp( node, BAD_CAST "auto_broadcast" );
	if( prop == NULL )
		data.commonScheduler.autoBroadcast = false;
	else
		data.commonScheduler.autoBroadcast = fromString( PCHAR_CAST prop );


	prop = xmlGetProp( node, BAD_CAST "broadcast_interval" );
	if( prop == NULL )
		data.commonScheduler.broadcastInterval = 10;
	else
		data.commonScheduler.broadcastInterval = atoi( PCHAR_CAST prop );

	prop = xmlGetProp( node, BAD_CAST "broadcast_trigger_percent_cap" );
	if( prop == NULL )
		data.commonScheduler.broadcastTriggerPercentCap = 2;
	else
		data.commonScheduler.broadcastTriggerPercentCap = atoi( PCHAR_CAST prop );

	prop = xmlGetProp( node, BAD_CAST "broadcast_order_mode" );
	if( prop == NULL )
		data.commonScheduler.broadcastOrderMode = 0;
	else
		data.commonScheduler.broadcastOrderMode = atoi( PCHAR_CAST prop );

	return true;
}

bool OptionalConfigParser::parseClassOptions( _xmlNode *node )
{
	xmlChar *prop = NULL;

	prop = xmlGetProp( node, BAD_CAST "death_knight_pre_req" );
	if( prop == NULL )
		data.classOptions.DKPrerequisites = true;
	else
		data.classOptions.DKPrerequisites = fromString( PCHAR_CAST prop );


	prop = xmlGetProp( node, BAD_CAST "death_knight_limit" );
	if( prop == NULL )
		data.classOptions.DKLimit = true;
	else
		data.classOptions.DKLimit = fromString( PCHAR_CAST prop );


	return true;
}

bool OptionalConfigParser::parseGoldSettings( _xmlNode *node )
{
	xmlChar *prop = NULL;

	prop = xmlGetProp( node, BAD_CAST "starting_gold" );
	if( prop == NULL )
		data.goldSettings.startingGold = 0;
	else
		data.goldSettings.startingGold = atoi( PCHAR_CAST prop );

	prop = xmlGetProp( node, BAD_CAST "enable_cap" );
	if( prop == NULL )
		data.goldSettings.enableCap = true;
	else
		data.goldSettings.enableCap = fromString( PCHAR_CAST prop );

	prop = xmlGetProp( node, BAD_CAST "cap" );
	if( prop == NULL )
		data.goldSettings.cap = 214748;
	else
		data.goldSettings.cap = atoi( PCHAR_CAST prop );


	return true;
}

