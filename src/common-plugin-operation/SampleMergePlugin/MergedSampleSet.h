/**
 * @file MergedSampleSet.h
 * @brief interfaces of MergedSampleSet class
 *
 * @author S.Tanaka
 * @date 2009.03.03
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_SAMPLE_MERGE_SAMPLE_SET_H__
#define __KOME_SAMPLE_MERGE_SAMPLE_SET_H__


namespace kome {
	namespace merged {

		class MergedSpectrum;

		/**
		 * @class MergedSampleSet
		 * @brief composition data object class
		 */
		class MergedSampleSet : public kome::objects::SampleSet {
		public:
			/**
			 * @fn MergedSampleSet( const double rtTol, const double mzTol )
			 * @brief constructor
			 * @param rtTol RT toleerance
			 * @parma mzTol m/z tolerance
			 */
			MergedSampleSet( const double rtTol, const double mzTol );

			/**
			 * @fn virtual ~MergedSampleSet()
			 * @brief destructor
			 */
			virtual ~MergedSampleSet();

		protected:
			/** groups */
			std::vector< kome::objects::Sample* > m_mergeSamplse;

			/** ID */
			unsigned int m_id;

			/** RT tolerance */
			const double m_rtTol;

			/** m/z tolerance */
			const double m_mzTol;

		public:
			/**
			 * @fn void addMergeSample( kome::objects::Sample* sample )
			 * @brief adds merge sample
			 * @param sample sample object to be added
			 */
			void addMergeSample( kome::objects::Sample* sample );

			/**
			 * @fn unsigned int getNumberOfMeargeSamples()
			 * @brief gets the number of merged samples
			 * @return the number of groups
			 */
			unsigned int getNumberOfMeargeSamples();

			/**
			 * @fn kome::objects::Sample* getMergeSample( const unsigned int idx )
			 * @brief gets the number of merged samples
			 * @param idx group index
			 * @return group (If NULL, the index is illegal.)
			 */
			kome::objects::Sample* getMergeSample( const unsigned int idx );
			
		public:
			/**
			 * @fn unsigned int getId()
			 * @brief gets merged sample set ID
			 * @return merged sample set ID
			 */
			unsigned int getId();

			/**
			 * @fn double getRtTol()
			 * @brief gets RT tolerance
			 * @return RT tolerance
			 */
			double getRtTol();

			/**
			 * @fn double getMzTol()
			 * @brief gets m/z tolerance
			 * @return m/z tolerance
			 */
			double getMzTol();

		public:
			/**
			 * @fn void openSampleSet()
			 * @brief opens sample set
			 */
			void openSampleSet();

		protected:
			/**
			 * @fn virtual bool onOpenFile( const char* path )
			 * @brief This method is called by openFile method. (override method)
			 * @param path file path
			 * @param progress
			 * @return If true, it succeeded to open the file
			 */
			virtual bool onOpenFile( const char* path, kome::core::Progress* progress );


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


#endif		// __KOME_SAMPLE_MERGE_SAMPLE_SET_H__
