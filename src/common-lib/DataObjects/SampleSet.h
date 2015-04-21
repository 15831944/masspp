/**
 * @file SampleSet.h
 * @brief interfaces of SampleSet class
 *
 * @author S.Tanaka
 * @date 2007.12.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_OBJECTS_SAMPLE_SET_H__
#define __KOME_OBJECTS_SAMPLE_SET_H__


#include <string>
#include <vector>


namespace kome {
	namespace objects {

		class Sample;

		/**
		 * @class SampleSet
		 * @brief sample set information management class
		 */
		class DATA_OBJECTS_CLASS SampleSet {
		public:
			/**
			 * @fn SampleSet()
			 * @brief constructor
			 */
			 SampleSet();

			/**
			 * @fn virtual ~SampleSet()
			 * @brief destructor
			 */
			virtual ~SampleSet();

		protected:
			/** file path */
			std::string m_filePath;

			/** file name */
			std::string m_fileName;

			/** samples */
			std::vector< Sample* > m_samples;

			/** opened flag */
			bool m_opened;

			/** loading timer */
			static kome::core::Timer m_loadingTimer;
			
			// >>>>>>	@Date:2013/06/13	<Add>	A.Ozaki
			/** error code of file access */
			int		m_errorCode;
			// <<<<<<	@Date:2013/06/13	<Add>	A.Ozaki

			/** sample set ID */
			int m_sampleSetId;

		protected:
			static int m_currentSampleSetId;

		public:
			/**
			 * @fn int getSampleSetId()
			 * @brief gets the sample set ID
			 * @return sample set ID
			 */
			int getSampleSetId();

		public:
			/**
			 * @fn void setFilePath( const char* path )
			 * @brief sets the file path 
			 * @param[in] path file path
			 */
			void setFilePath( const char* path );

			/**
			 * @fn const char* getFilePath()
			 * @brief gets file path
			 * @return file path
			 */
			const char* getFilePath();

			/**
			 * @fn const char* getFileName()
			 * @brief gets file name
			 * @return file name
			 */
			const char* getFileName();

			/**
			 * @fn bool isOpened()
			 * @brief judges whther this sample set file is opened or not
			 * @return If true, this sample set file is opened.
			 */
			bool isOpened();
			
			// >>>>>>	@Date:2013/06/13	<Add>	A.Ozaki
			/**
			 * @fn int getErrorCode( void )
			 * @brief get error code of file access.
			 * @return error code.
			 */
			int	getErrorCode( void );

			/**
			 * @fn int setErrorCode( const int errorCode )
			 * @brief set error code of file access.
			 * @param[in] errorCode error code
			 */
			void	setErrorCode( const int errorCode );
			// <<<<<<	@Date:2013/06/13	<Add>	A.Ozaki
		public:
			/**
			 * @fn void clearSamples()
			 * @brief clears samples
			 */
			void clearSamples();

			/**
			 * @fn void addSample( Sample* sample )
			 * @brief adds sample
			 * @param[in] sample sample object to be added
			 */
			void addSample( Sample* sample );

			/**
			 * @fn unsigned int getNumberOfSamples()
			 * @brief gets the nubmer of samples
			 * @return the number of samples
			 */
			unsigned int getNumberOfSamples();

			/**
			 * @fn Sample* getSample( const unsigned int index )
			 * @brief gets sample object
			 * @param[in] index sample index
			 * @return sample object (If NULL, the index is illegal.)
			 */
			Sample* getSample( const unsigned int index );

		public:
			/**
			 * @fn bool openFile( const char* path )
			 * @brief opens file
			 * @param[in] path file path
			 * @param[out] progress progress bar dialog
			 * @return If true, it succeeded to open the file
			 */
			bool openFile( const char* path, kome::core::Progress* progress = NULL );

			/**
			 * @fn bool closeFile()
			 * @brief closes file
			 * @return If true, it succeeded to close the file
			 */
			bool closeFile();

		public:
			/**
			 * @fn static void startLoadingTimer()
			 * @brief starts loading timer
			 */
			static void startLoadingTimer();

			/**
			 * @fn static void stopLoadingTimer()
			 * @brief stops loading timer
			 */
			static void stopLoadingTimer();

			/**
			 * @fn static double getTotalLoadingTime()
			 * @brief gets total loading time
			 * @return total loading time
			 */
			static double getTotalLoadingTime();

			/**
			 * @fn static int issueSampleSetId()
			 * @brief issues sample set ID
			 * @return sample set ID
			 */
			static int issueSampleSetId();

		protected:
			/**
			 * @fn virtual bool onOpenFile( const char* path ) = 0
			 * @brief This method is called by openFile method. (abstract method)
			 * @param[in] path file path
			 * @param[out] progress progress bar dialog
			 * @return If true, it succeeded to open the file
			 */
			virtual bool onOpenFile( const char* path, kome::core::Progress* progress = NULL ) = 0;

			/**
			 * @fn virtual bool onCloseFile() = 0
			 * @brief This method is called by closeFile method. (abstract method)
			 * @return If true, it succeeded to close the file
			 */
			virtual bool onCloseFile() = 0;

		public:
			/**
			 * @fn virtual bool onOpenSample( Sample* sample ) = 0
			 * @brief This method is called by openSample method. (abstract method)
			 * @param[in,out] sample sample object to be opened (If NULL, opens all samples.)
			 * @param[out] progress progress bar dialog
			 * @return If true, it succeeded to open the sample.
			 */
			virtual bool onOpenSample( Sample* sample, kome::core::Progress* progress = NULL ) = 0; 

			/**
			 * @fn virtual bool onCloseSample( Sample* sample ) = 0
			 * @brief This method is called by closeSample method. (abstract method)
			 * @param[in,out] sample sample object to be closed (If NULL, closes all samples.)
			 * @return If true, it succeeded to close the sample.
			 */
			virtual bool onCloseSample( Sample* sample ) = 0;
		};
	}
}

#endif		// __KOME_OBJECTS_SAMPLE_SET_H__
