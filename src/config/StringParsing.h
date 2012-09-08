#ifndef STRINGPARSING_H
#define STRINGPARSING_H

#include <string>
#include <cstdlib>

#define PCHAR_CAST (char*)

inline unsigned short toUShort( const std::string &s )
{
	unsigned short v = static_cast< unsigned short >( atoi( s.c_str() ) );
	return v;
}

inline unsigned int toUInt( const std::string &s )
{
	unsigned int v = atoi( s.c_str() );
	return v;
}

inline unsigned long toULong( const std::string &s )
{
	unsigned long v = strtol( s.c_str(), NULL, 10 );
	return v;
}

inline float toFloat( const std::string &s )
{
	float v = static_cast< float >( atof( s.c_str() ) );
	return v;
}

inline bool toBool( const std::string &s )
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


#endif

