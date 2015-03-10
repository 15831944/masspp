/**
 * @file MzmlSampleSet.h
 * @brief interfaces of MzmlSampleSet class
 *
 * @author S.Tanaka
 * @date 2009.03.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_MZML_SAMPLE_SET_H__
#define __KOME_IO_MZML_SAMPLE_SET_H__


namespace kome {
	namespace io {
		namespace mzml {

			/**
			 * @class MzmlSampleSet
			 * @brief mzML sample set class
			 */
			class MzmlSampleSet : public kome::objects::SampleSet {
			public:
				/**
				 * @fn MzmlSampleSet()
				 * @brief constructor
				 */
				MzmlSampleSet();

				/**
				 * @fn virtual ~MzmlSampleSet()
				 * @brief destructor
				 */
				virtual ~MzmlSampleSet();

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
			};
		}
	}
}


#endif		// __KOME_IO_MZML_SAMPLE_SET_H__
