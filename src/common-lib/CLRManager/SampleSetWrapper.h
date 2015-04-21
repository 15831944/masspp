/**
 * @file SampleSetWrapper.h
 * @brief interfaces of SampleSetWrapper class
 *
 * @author S.Tanaka
 * @date 2008.01.07
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_SAMPLE_SET_WRAPPER_H__
#define __KOME_CLR_SAMPLE_SET_WRAPPER_H__


namespace kome {
	namespace clr {

		ref class SampleWrapper;

		/**
		 * @class SampleSetWrapper
		 * @brief SampleSet object wrapper class to use on CLR environment
		 */
		public ref class SampleSetWrapper {
		public:
			/**
			 * @fn SampleSetWrapper( kome::objects::SampleSet& sampleSet )
			 * @brief constructor
			 * @param[in] sampleSet SampleSet object
			 */
			SampleSetWrapper( kome::objects::SampleSet& sampleSet );

			/**
			 * @fn virtual ~SampleSetWrapper()
			 * @brief destructor
			 */
			virtual ~SampleSetWrapper();

		protected:
			/** SampleSet object */
			kome::objects::SampleSet& m_sampleSet;

		public:
			/**
			 * @fn kome::objects::SampleSet& getSampleSet()
			 * @brief gets the SampleSet object
			 * @return the pointer to the SampleSet object
			 */
			kome::objects::SampleSet& getSampleSet();

		public:
			/**
			 * @fn int getSampleSetId()
			 * @brief gets the sample set ID
			 * @return sample set ID
			 */
			int getSampleSetId();

		public:
			/**
			 * @fn void setFilePath( System::String^ path )
			 * @brief sets the file path 
			 * @param[in] path file path
			 */
			void setFilePath( System::String^ path );

			/**
			 * @fn System::String^ getFilePath()
			 * @brief gets file path
			 * @return file path
			 */
			System::String^ getFilePath();

			/**
			 * @fn System::String^ getFileName()
			 * @brief gets file name
			 * @return file name
			 */
			System::String^ getFileName();

			/**
			 * @fn bool isOpened()
			 * @brief judges whther this sample set file is opened or not
			 * @return If true, this sample set file is opened.
			 */
			bool isOpened();

		public:
			/**
			 * @fn void clearSamples()
			 * @brief clears samples
			 */
			void clearSamples();

			/**
			 * @fn void addSample( SampleWrapper^ sample )
			 * @brief adds sample
			 * @param[in] sample sample object to be added
			 */
			void addSample( SampleWrapper^ sample );

			/**
			 * @fn unsigned int getNumberOfSamples()
			 * @brief gets the nubmer of samples
			 * @return the number of samples
			 */
			unsigned int getNumberOfSamples();

			/**
			 * @fn SampleWrapper^ getSample( unsigned int index )
			 * @brief gets sample object
			 * @param[in] index sample index
			 * @return sample object (If NULL, the index is illegal.)
			 */
			SampleWrapper^ getSample( unsigned int index );

		public:
			/**
			 * @fn bool openFile( System::String^ path )
			 * @brief opens file
			 * @param[in] path file path
			 * @param[in] progress
			 * @return If true, it succeeded to open the file
			 */
			bool openFile( System::String^ path );

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
		};
	}
}

#endif	// __KOME_CLR_SAMPLE_SET_WRAPPER_H__
