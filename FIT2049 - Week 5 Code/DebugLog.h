#pragma once
#include <sstream>

/*	FIT2049 - Assignment 1
*	DebugLog.h
*	Created by Jascha Scholz - 2015 - Monash Univeristy
*	Debug Makro.
*/

#define DBOUT( s )            \
{                             \
   std::ostringstream os_;    \
   os_ << s;                   \
   OutputDebugString( os_.str().c_str() );  \
}