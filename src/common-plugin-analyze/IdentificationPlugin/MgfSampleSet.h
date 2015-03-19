/**
 * @file MgfSampleSet.h
 * @brief interfaces of MgfSampleSet class
 *
 * @author M.Izumi
 * @date 2013.07.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_IDENT_MGF_SAMPLE_SET_H__
#define __KOME_IDENT_MGF_SAMPLE_SET_H__

namespace kome {
	namespace ident {
		/**
		 * @class MgfSampleSet
		 * @brief mgf sample set class
		 */
		class MgfSampleSet : public kome::objects::SampleSet {
		public:
			/**
			 * @fn MgfSampleSet()
			 * @brief constructor
			 */
			MgfSampleSet();

			/**
			 * @fn virtual ~MgfSampleSet()
			 * @brief destructor
			 */
			virtual ~MgfSampleSet();

		protected:
			FILE* m_fp;
		public:
			FILE* getFile() ;

			/**
			 * @fn bool openFile( const char* path )
			 * @brief open file
			 * @param path
			 * @return 
 			 */
			bool openFile( const char* path );

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
		};
	}
}

#endif // _KOME_IDENT_MGF_SAMPLE_SET_H__

