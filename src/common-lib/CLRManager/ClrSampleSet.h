/**
 * @file ClrSampleSet.h
 * @brief interfaces of ClrSampleSet class
 *
 * @author S.Tanaka
 * @date 2008.01.07
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_SAMPLE_SET_H__
#define __KOME_CLR_SAMPLE_SET_H__


#include "ClrSampleSetBase.h"


namespace kome {
	namespace clr {

		ref class SampleWrapper;

		/**
		 * @class ClrSampleSet
		 * @brief SampleSet class that has managed class object
		 */
		class ClrSampleSet : public kome::objects::SampleSet {
		public:
			/**
			 * @fn ClrSampleSet()
			 * @brief constructor
			 */
			ClrSampleSet();

			/**
			 * @fn virtual ~ClrSampleSet()
			 * @brief destructor
			 */
			virtual ~ClrSampleSet();

		protected:
			/** base spectrum object */
			gcroot< ClrSampleSetBase^ > m_baseSampleSet;

		public:
			/**
			 * @fn void setBaseSampleSet( ClrSampleSetBase^ baseSampleSet )
			 * @brief sets base spectrum object
			 * @param[in] baseSampleSet base spectrum object
			 */
			void setBaseSampleSet( ClrSampleSetBase^ baseSampleSet );

			/**
			 * @fn ClrSampleSetBase^ getBaseSampleSet()
			 * @brief gets base spectrum object
			 * @return base spectrum object
			 */
			ClrSampleSetBase^ getBaseSampleSet();

		protected:
			/**
			 * @fn virtual bool onOpenFile( const char* path ) 
			 * @brief This method is called by openFile method. (override method)
			 * @param[in] path file path
			 * @param[in] progress
			 * @return If true, it succeeded to open the file
			 */
			virtual bool onOpenFile( const char* path, kome::core::Progress* progress = NULL );


			/**
			 * @fn virtual bool onCloseFile() 
			 * @brief This method is called by closeFile method. (override method)
			 * @return If true, it succeeded to close the file
			 */
			virtual bool onCloseFile() ;

		public:
			/**
			 * @fn virtual bool onOpenSample( kome::objects::Sample* sample )
			 * @brief This method is called by openSample method. (override method)
			 * @param[in,out] sample sample object to be opened (If NULL, opens all samples.)
			 * @param[in] progress progress bar
			 * @return If true, it succeeded to open the sample.
			 */
			virtual bool onOpenSample( kome::objects::Sample* sample, kome::core::Progress* progress = NULL ); //Åö

			/**
			 * @fn virtual bool onCloseSample( kome::objects::Sample* sample )
			 * @brief This method is called by closeSample method. (override method)
			 * @param[in,out] sample sample object to be closed (If NULL, closes all samples.)
			 * @return If true, it succeeded to close the sample.
			 */
			virtual bool onCloseSample( kome::objects::Sample* sample );
		};
	}
}

#endif	// __KOME_CLR_SAMPLE_SET_H__
