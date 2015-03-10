/**
 * @file FileIOPluginFunctions.h
 * @brief interfaces of FileIO plug-in function
 *
 * @author S.Tanaka
 * @date 2006.08.14
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_FILE_IO_PLUGIN_FUNCTIONS_H__
#define __KOME_IO_FILE_IO_PLUGIN_FUNCTIONS_H__


#include <string>


#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant openFileOnBatch( kome::objects::Parameters* params )
 * @brief opens sample file on batch command (kome::object::DataManager object has opened file object.)
 * @param params parameters object (This object has path information.)
 * @return If it succeeded to open the file, this method returns true. (boolean value)
 */
kome::objects::Variant openFileOnBatch( kome::objects::Parameters* params );

/**
 * @fn bool openFileWithDialog( kome::objects::Parameters* )
 * @brief select file from dialog and open
 * @return If it finishes successfully, this function returns true.
 */
kome::objects::Variant openFileWithDialog( kome::objects::Parameters* );


/**
 * @fn bool openDialog( kome::objects::Parameters* params )
 * @brief open dialog ( SampleCloseCheckDialog )
 * @return If it finishes successfully, this function returns true.
 */
kome::objects::Variant openDialog( kome::objects::Parameters* params );

/**
 * @fn bool openExportSampleDialog( kome::objects::Parameters* params )
 * @brief open Export Sample Dialog
 * @return If it finishes successfully, this function returns true.
 */
kome::objects::Variant openExportSampleDialog( kome::objects::Parameters* params );	// @date 2011.07.25 <Add> M.Izumi


/**
 * @fn kome::objects::Variant closeOpenedSample( kome::objects::Parameters* params )
 * @brief close opened sample
 * @param params parameters object
 * @return If true, it succeeded to close the sample
 */
kome::objects::Variant closeOpenedSample( kome::objects::Parameters* params );

/**
 * @fn closeAllFileskome::objects::Variant closeAllFiles( kome::objects::Parameters* )
 * @brief closes all sample files
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant closeAllFiles( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant initGUI( kome::objects::Parameters* )
 * @brief initializes GUI
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant initGUI( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant selectSamples( kome::objects::Parameters* params )
 * @brief selects samples
 * @param params parameters object
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant selectSamples( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant setFileNameSuffix( kome::objects::Parameters* params )
 * @brief sets file name suffix
 * @param params parameters object
 * @return return value (boolean)
 */
kome::objects::Variant setFileNameSuffix( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant setOutputFilePath( kome::objects::Parameters* params )
 * @brief sets output file path
 * @param params parameters object
 * @return return value (boolean)
 */
kome::objects::Variant setOutputFilePath( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant6 setSampleNumbers( kome::objects::Parameters* params )
 * @brief sets sample numbers
 * @param params parameters object
 * @return return value (boolean)
 */
kome::objects::Variant setSampleNumbers( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant6 setSampleNames( kome::objects::Parameters* params )
 * @brief sets sample names
 * @param params parameters object
 * @return return value (boolean)
 */
kome::objects::Variant setSampleNames( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant printNumberOfSamples( kome::objects::Parameters* )
 * @brief prints the number of samples
 * @return return value (boolean)
 */
kome::objects::Variant printNumberOfSamples( kome::objects::Parameters* );


/**
 * @fn kome::objects::Variant openRawdataFile( kome::objects::Parameters* params )
 * @brief opens rawdata file
 * @param[in] params parameters
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant openRawdataFile( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant getFileOpenOperation( kome::objects::Parameters* )
 * @brief get file open operation
 * @return return value file io operation object
 */
kome::objects::Variant getFileOpenOperation( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant getExportSampleOperation( kome::objects::Parameters* )
 * @brief get export sample operation
 * @return return value file io operation object
 */
kome::objects::Variant getExportSampleOperation( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant closeSampleFromContextMenu( kome::objects::Parameters* )
 * @brief close selected sample right click menu
 * @return return value true
 */
kome::objects::Variant closeSampleFromContextMenu( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant openSpectrumFromContextMenu( kome::objects::Parameters* params )
 * @brief open selected spectrum right click menu
 * @return return value true
 */
kome::objects::Variant openSpectrumFromContextMenu( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant openChromatogramFromContextMenu( kome::objects::Parameters* params )
 * @brief open selected chromatgram right click menu
 * @return return value true
 */
kome::objects::Variant openChromatogramFromContextMenu( kome::objects::Parameters* params );

#ifdef __unix
	}
#endif  // __unix



#endif// __KOME_IO_FILE_IO_PLUGIN_FUNCTIONS_H__
