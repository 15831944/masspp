/**
 * @file FileFunctions.h
 * @brief interfaces of file system function.
 *
 * @author S.Tanaka
 * @date 2006.06.28
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CORE_FILE_TOOL_H__
#define __KOME_CORE_FILE_TOOL_H__


#include <string>
#include <vector>

/**
 * @fn bool fileexists( const char* path )
 * @brief judge whether file exists
 * @param[in] path file path
 * @return true: file exists. | false: file doesn't exist
 */
bool fileexists( const char* path );

/**
 * @fn bool isdirectory( const char* path )
 * @brief judge whether file is directory
 * @param[in] path file path
 * @return true: path is directory. | false: path is not directory.
 */
bool isdirectory( const char* path );

/**
 * @fn bool checkfile( const char* path )
 * @brief check file before open. (This function calls fileexists and isdirectory )
 * @param[in] path file path
 * @return true: The path specified exists and is not directory.
 */
bool checkfile( const char* path );
 
/**
 * @fn std::string getfilename( const char* path )
 * @brief get file name
 * @param[in] path file path
 * @return file name (excluding directory part)
 */
std::string getfilename( const char* path );

/** 
 * @fn std::string getdir( const char* path )
 * @brief get dir that the file exists
 * @param[in] path file path
 * @return directory part
 */
std::string getdir( const char* path );

/**
 * @fn std::string getpath( const char* dir, const char* file )
 * @brief get file path
 * @param dir directory name
 * @param[in] file file name
 * @return file path
 */
std::string getpath( const char* dir, const char* file );

/**
 * @fn std::string absolutepath( const char* path )
 * @brief get absolute path
 * @param[in] path file path
 * @return absolute path
 */
std::string absolutepath( const char* path );

/**
 * @fn std::string getabsolutepath( const char* dir, const char* file )
 * @brief get absolute path
	  - If dir has root, this function returns getpath( dir, file )
	  - If dir doesn't have root, this function returns absolutepath( getpath( dir, file ) );
 * @param[in] dir directory name
 * @param[in] file file name
 * @return file path
 */
std::string getabsolutepath( const char* dir, const char* file );

/**
 * @fn bool isabsolutepath( const char* file )
 * @brief judges whether the specified file path is absolute file path or not.
 * @param[in] file file path
 * @return If true, the specified file path is absolute file path.
 */
bool isabsolutepath( const char* file );

/**
 * @fn void setarguments( int argc, char** argv )
 * @brief set arguments
 * @param[in] argc the number of arguments
 * @param[in] argv the array of arguments
 */
void setarguments( int argc, char** argv );

/**
 * @fn int getargc()
 * @brief get number of arguments
 * @return number of arguments
 */
int getargc();

/**
 * @fn const char* getargv( int index )
 * @brief get argument
 * @param[in] index argument index
 * @return argument ( if index < 0 or index > 0 returns NULL )
 */
const char* getargv( int index );

/**
 * @fn std::string getext( const char* path )
 * @brief get the extension of the file
 * @param[in] path file path
 * @return the extension of the file
 */
std::string getext( const char* path );

/**
 * @fn std::string getenvariable( const char* name )
 * @brief get environment variable
 * @param[in] name environment variable name
 * @return environment vaiable value
 */
std::string getenvariable( const char* name );

/**
 * @fn std::string getmodulefile()
 * @brief gets module file path
 * @return module file path
 */
std::string getmodulefile();

/**
 * @fn std::string getmoduledir()
 * @brief gets module file directory
 * @return module file directory
 */
std::string getmoduledir();

/**
 * @fn std::string gethomedir()
 * @brief gets current user's home directory
 * @return current user's home directory
 */
std::string gethomedir();

/**
 * @fn std::string getplugindir()
 * @brief gets mspp plugin paths
 */
std::string getplugindir();

/**
 * @fn FILE* fileopen( const char* path, const char* mode )
 * @brief opens file
 * @param[in] path file path to be opened
 * @param[in] mode open mode
 * @return file descriptor
 */
FILE* fileopen( const char* path, const char* mode );

/**
 * @fn void fileseek( FILE* fp, long long offset, int origin )
 * @brief sets file position
 * @param[in] fp file descriptor
 * @param[in] offset position from origin
 * @param[in] origin SEEK_SET, SEEK_CUR or SEEK_END (see "fseek" document)
 */
void fileseek( FILE* fp, long long offset, int origin );

/**
 * @fn long long filetell( FILE* fp )
 * @brief gets file position
 * @param[in] fp file description
 * @return the file position
 */
long long filetell( FILE* fp );

/**
 * @fn bool copyfile( const char* src, const char* dst )
 * @brief copies file
 * @param[in] src file path to be copied
 * @param[in] dst copy file path
 * @return If true, it succeeded to copy a file.
 */
bool copyfile( const char* src, const char* dst );

/**
 * @fn void removefile( const char* path )
 * @brief removes file
 * @param[in] path file path to be removed
 * @return If true, it succeeded to remove a file.
 */
bool removefile( const char* path );

/**
 * @fn bool makedirectory( const char* path )
 * @brief creates directory
 * @param[in] path directory path to be maken
 * @return If true, it succeeded to make a directory.
 */
bool makedirectory( const char* path );

/**
 * @fn bool removedirs( const char* path )
 * @brief remove directories
 * @param[in] path directory path
 */
bool removedirs( const char* path );

/**
 * @fn bool makedirs( const char* path )
 * @brief make directories
 * @param[in] path directory path
 */
bool makedirs( const char* path );

/**
 * @fn unsigned long long filesize( const char* path )
 * @brief gets the file size
 * @param[in] path file path
 * @return file size
 */
unsigned long long filesize( const char* path );

/**
 * @fn std::string searchfile( const char* dir, const char* fileName, const bool recursive )
 * @brief searches file
 * @param[in] dir file directory
 * @param[in] fileName file name
 * @param[in] recursive If true, search file recursively.
 * @return If the file is found, this function returns file path. Otherwise, empty string.
 */
std::string searchfile( const char* dir, const char* fileName, const bool recursive );


#endif	// __KOME_CORE_FILE_TOOL_H__
