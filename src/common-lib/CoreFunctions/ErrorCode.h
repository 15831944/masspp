/**
 * @file ErrorCode.h
 * @brief common error code definition
 *
 * @author S.Tanaka
 * @date 2006.07.11
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CORE_ERROR_CODE_H__
#define __KOME_CORE_ERROR_CODE_H__


/**
 * @fn int errorcode( const char* errorName )
 * @brief get new error code
 * @param errorName error name
 * @return error code
 */
unsigned int errorcode( const char* errorName );

/**
 * @fn const char* errorstring( unsigned int errorCode )
 * @brief get error string for an existing error
 * @param errorCode error code
 * @return error string
 */
const char* errorstring( unsigned int errorCode );

/**
 * @fn void errorcleanup( void )
 * @brief deallocate error strings object
 */
void errorcleanup( void );

/** OK */
const unsigned int ERR_OK				= errorcode( "ok" );

/** File is not found. */
const unsigned int ERR_FILE_NOT_FOUND	= errorcode( "file not found" );

/** It failed to open a file */
const unsigned int ERR_FILE_OPEN_FAILED  = errorcode( "file open failed" );

/** It failed to close a file */
const unsigned int ERR_FILE_CLOSE_FAILED = errorcode( "file close failed" );

/** It failed to read a file */
const unsigned int ERR_FILE_READ_FAILED  = errorcode( "file read failed" );

/** It failed to write a file */
const unsigned int ERR_FILE_WRITE_FAILED = errorcode( "file write failed" );

/** The path is directory */
const unsigned int ERR_PATH_IS_DIRECTORY = errorcode( "path is a directory" );

/** File is illegal */
const unsigned int ERR_ILLEGAL_FILE	  = errorcode( "illegal file" );

/** Name is illegal */
const unsigned int ERR_ILLEGAL_NAME	  = errorcode( "illegal name" );

/** Value is illegal */
const unsigned int ERR_ILLEGAL_VALUE	 = errorcode( "illegal value" );

/** The pointer is null */
const unsigned int ERR_NULL_POINTER	  = errorcode( "null pointer" );

/** DB error */
const unsigned int ERR_DB				= errorcode( "database" );

/** etc */
const unsigned int ERR_OTHER			 = errorcode( "other" );

// >>>>>>	@Date:2013/06/17	<Add>	A.Ozaki
/** File permissions is illegal **/
const unsigned int ERR_FILE_PARMISSIONS_ILLEGAL	= errorcode( "file permisions is illegal" );
// <<<<<<	@Date:2013/06/17	<Add>	A.Ozaki

#endif		// __KOME_CORE_ERROR_CODE_H__
