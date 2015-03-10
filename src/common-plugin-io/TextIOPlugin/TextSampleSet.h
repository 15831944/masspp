/**
 * @file TextSampleSet.h
 * @brief interfaces of TextSampleSet class
 *
 * @author S.Tanaka
 * @date 2010.01.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_TEXT_SAMPLE_SET_H__
#define __KOME_IO_TEXT_SAMPLE_SET_H__


namespace kome {
	namespace io {
		namespace text {

			/**
			 * @class TextSampleSet
			 * @brief text sample file management class
			 */
			class TextSampleSet : public kome::objects::SampleSet {
			public:
				/**
				 * @fn TextSampleSet();
				 * @brief constructor
				 */
				TextSampleSet();

				/**
				 * @fn virtual ~TextSampleSet()
				 * @brief destructor
				 */
				virtual ~TextSampleSet();

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
				
				/**
				 * @fn bool opneFile( const char* path )
				 * @brief file open
				 * @param path file path
				 * @return If true, it succeeded to open the file
				 */
				bool openFile( const char* path );


				/**
				 * @fn bool closeFile()
				 * @brief close file
				 * @retrn If true, it succeeded to close the file
				 */
				bool closeFile();

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
				virtual bool onOpenSample( kome::objects::Sample* sample, kome::core::Progress* progress = NULL );

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

#endif		//  __KOME_IO_TEXT_SAMPLE_SET_H__
