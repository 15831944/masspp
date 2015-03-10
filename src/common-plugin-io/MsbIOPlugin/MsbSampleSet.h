/**
 * @file MsbSampleSet.h
 * @brief interfaces of MsbSampleSet class
 *
 * @author S.Tanaka
 * @date 2008.01.22
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_MSB_SAMPLE_SET_H__
#define __KOME_IO_MSB_SAMPLE_SET_H__


#include <set>


namespace kome {
	namespace io {
		namespace msb {

			class MsbManager;

			/**
			 * @class MsbSampleSet
			 * @brief msb sample file management class
			 */
			class MsbSampleSet : public kome::objects::SampleSet {
			public:
				/**
				 * @fn MsbSampleSet();
				 * @brief constructor
				 */
				MsbSampleSet();

				/**
				 * @fn virtual ~MsbSampleSet()
				 * @brief destructor
				 */
				virtual ~MsbSampleSet();

			protected:
				/** msb IO manager class */
				MsbManager* m_mgr;

				/** source file path */
				std::string m_sourceFile;

			public:
				/**
				 * @fn void setSourceFilePath( const char* path )
				 * @brief sets source file path
				 * @param path source file path
				 */
				void setSourceFilePath( const char* path );

				/**
				 * @fn const char* getSourceFilePath()
				 * @brief gets source file path
				 * @return source file path
				 */
				const char* getSourceFilePath();

				/**
				 * @fn MsbManager* getManager()
				 * @brief gets msb IO manager object
				 */
				MsbManager* getManager();

			protected:
				/**
				 * @fn virtual bool onOpenFile( const char* path )
				 * @brief This method is called by openFile method. (override method)
				 * @param path file path
				 * @param progress
				 * @return If true, it succeeded to open the file
				 */
				virtual bool onOpenFile( const char* path, kome::core::Progress* progress = NULL );


				/**
				 * @fn virtual bool onCloseFile()
				 * @brief This method is called by closeFile method. (override method)
				 * @param path file path
				 * @return If true, it succeeded to close the file
				 */
				virtual bool onCloseFile();

			public:
				/**
				 * @fn virtual bool onOpenSample( kome::objects::Sample* sample )
				 * @brief This method is called by openTreatment method. (override method)
				 * @param treatment treatment object to be opened (If NULL, opens all treatments.)
				 * @param progress
				 * @return If true, it succeeded to open the treatment.
				 */
				virtual bool onOpenSample( kome::objects::Sample* sample, kome::core::Progress* progress = NULL); 

				/**
				 * @fn virtual bool onCloseSample( kome::objects::Sample* sample )
				 * @brief This method is called by closeTreatment method. (override method)
				 * @param treatment treatment object to be closed (If NULL, closes all treatments.)
				 * @return If true, it succeeded to close the treatment.
				 */
				virtual bool onCloseSample( kome::objects::Sample* sample );
			};
		}
	}
}

#endif		//  __KOME_IO_MSB_SAMPLE_SET_H__
