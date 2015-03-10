/**
 * @file GcmsSampleSet.h
 * @brief interface of AIASampleSet class
 *
 * @author Y.Fujita
 * @date 2012.07.19
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_IO_AIA_SAMPLE_SET_H__
#define __KOME_IO_AIA_SAMPLE_SET_H__


namespace kome {
	namespace io {
		namespace aia {

			/**
			 * @class AIASampleSet
			 * @brief AIA sample file class
			 */
			class AIASampleSet : public kome::objects::SampleSet {
			public:
				/**
				 * @fn AIASampleSet()
				 * @brief constructor
				 */
				AIASampleSet();

				/**
				 * @fn virtual ~AIASampleSet()
				 * @brief destructor
				 */
				virtual ~AIASampleSet();

			protected:
				/** file descriptor */
				FILE* m_fp;

			public:
				/**
				 * @fn FILE* getFile()
				 * @brief gets file descriptor
				 * @return file descriptor
				 */
				FILE* getFile();

			protected:
				/** CDF file path */
				std::string m_cdfFile;

			public:
				/**
				 * @fn void setCdfFilePath( const char* path )
				 * @brief sets Net CDF File path
				 * @param path Net CDF File path
				 */
				void setCdfFilePath( const char* path );

				/**
				 * @fn const char* getCdfFilePath()
				 * @brief gets NET CDF file path
				 * @return NET CDF file path
				 */
				const char* getCdfFilePath();

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
				 * @param sample sample object to be opened (If NULL, opens all samples.)
				 * @param progress
				 * @return If true, it succeeded to open the sample.
				 */
				virtual bool onOpenSample( kome::objects::Sample* sample, kome::core::Progress* progress = NULL ); 

				/**
				 * @fn virtual bool onCloseSample( kome::objects::Sample* sample )
				 * @brief This method is called by closeTreatment method. (override method)
				 * @param sample sample object to be closed (If NULL, closes all samples.)
				 * @return If true, it succeeded to close the sample.
				 */
				virtual bool onCloseSample( kome::objects::Sample* sample );

			public:
				/**
				 * @fn static void error( int status )
				 * @brief error message 
				 * @param status error status
				 */
				static void error( int status );

			};
		}
	}
}


#endif		// __KOME_IO_AIA_SAMPLE_SET_H__
