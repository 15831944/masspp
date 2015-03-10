/**
 * @file msppcmd.h
 * @brief interfaces of msppcmd functions
 *
 * @author S.Tanaka
 * @date 2006.11.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_MSPPCMD_COMMAND_H__
#define __KOME_MSPPCMD_COMMAND_H__

/**
 * @fn int main( int argc, char** argv )
 * @brief main function
 * @param argc the number of arguments
 * @param argv the array of arguments
 */
int main( int argc, char** argv );

/**
 * @fn bool init( int argc, char** argv )
 * @brief initializes batch
 * @param argc the number of arguments
 * @param argv the array of arguments
 * @return If it succeeded to initialize, this method returns true;
 */
bool init( int argc, char** argv );

/**
 * @fn void finalize()
 * @brief finalizes batch
 */
void finalize();

/**
 * @fn bool loadPluginFiles()
 * @brief loads plugi-in definition files
 * @return If it succeeded to get plug-in information, this function returns true.
 */ 
bool loadPluginFiles();

/**
 * @fn bool isPluginPath( const char* path )
 * @brief judges whether specified path is plug-in file or directory.
 * @param path file path
 * @return If specified path is plug-in file or directory, this function returns true.
 */
bool isPluginPath( const char* path );

/**
 * @fn void onSignal( int )
 * @brief This function is called when receiving a signal
 */
void onSignal( int );


#endif	// __KOME_MSPPCMD_COMMAND_H__
