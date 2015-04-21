/**
 * @file CoreMacros.h
 * @brief basic macro collection
 *
 * @author S.Tanaka
 * @date 2006.07.24
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CORE_MACROS_H__
#define __KOME_CORE_MACROS_H__


/** NULL */
#ifndef NULL
	#define NULL 0
#endif


//
// Numerical
//

/** get max value */
#define MAX( x, y )		 ( ( ( x ) < ( y ) ) ? ( y ) : ( x ) )

/** get min value */
#define MIN( x, y )		 ( ( ( x ) > ( y ) ) ? ( y ) : ( x ) )

/** If x is greater than high, the value is high.
	If x is less than low, the value is low.
	Bisides the value is x. */
#define CLAMP( x, low, high )   MIN( high, MAX( x, low ) )

/** get absolute value */
#define ABS( v )			( ( v ) < 0 ? - ( v ) : ( v ) )
/** get range of 2 values */
#define RANGE( x, y )	   ( ( ( x ) < ( y ) ) ? ( y - x ) : ( x - y ) )


//
// bit
//

/** circular left shift */
#define CIRCULAR_LEFT_SHIFT( v, bits )	( ( ( v ) << ( bits ) ) | ( ( v ) >> ( ( sizeof( v ) * 8 ) - ( bits ) ) ) )

/** circular right shift */
#define CIRCULAR_RIGHT_SHIFT( v, bits )	( ( ( v ) >> ( bits ) ) | ( ( v ) << ( ( sizeof( v ) * 8 ) - ( bits ) ) ) )

//
// File System
//

#ifdef __unix

	/** the system-dependent default name separator charactor, represented as a string */
	#define DIR_SEPARATOR		  "/"

	/** the system-dependent default name separator charactor */
	#define DIR_SEPARATOR_CHAR	'/'

	/** the system-dependent default path separator charactor, represented as a string */
	#define PATH_SEPARATOR		  ":"

	/** the system-dependent default path separator charactor */
	#define PATH_SEPARATOR_CHAR   ':'

	/** new line string */
	#define NEW_LINE			  "\n"

#else

	/** the system-dependent default name separator charactor, represented as a string */
	#define DIR_SEPARATOR		  "\\"

	/** the system-dependent default name separator charactor */
	#define DIR_SEPARATOR_CHAR	'\\'

	/** the system-dependent default path separator charactor, represented as a string */
	#define PATH_SEPARATOR		  ";"

	/** the system-dependent default path separator charactor */
	#define PATH_SEPARATOR_CHAR   ';'

	/** new line string */
	#define NEW_LINE			  "\r\n"

#endif	//  __unix


//
// Others
//

/**  NULL Test */
#define NVL( checkVal, replaceVal )	( ( ( checkVal ) == NULL ) ? ( replaceVal ) : ( checkVal ) )



#endif		// __KOME_CORE_MACROS_H__
