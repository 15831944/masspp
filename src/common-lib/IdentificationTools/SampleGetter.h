/**
 * @file SampleGetter.h
 * @brief Sample Getter class
 *
 * @author M.Izumi
 * @date 2012.11.20
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */

#ifndef __KOME_IDENT_TOOL_SAMPLE_GETTER_H__
#define __KOME_IDENT_TOOL_SAMPLE_GETTER_H__

#include "IdentificationManager.h"

namespace kome {
	
	namespace operation{
		class ManipulatedSample;
	}

	namespace ident {
		/**
		 * @class SampleGetter
		 * @brief sample getter class
		 */
		class IDENT_TOOLS_CLASS SampleGetter{
		public:
			/**
			 * @fn SampleGetter()
			 * @brief constructor
			 */
			SampleGetter();

			/**
			 * @fn ~SampleGetter()
			 * @brief destructor
			 */
			virtual ~SampleGetter();

		protected:
			/** matrix id */
			int m_matrixId;

			/** samples */
			std::vector< int > m_samples;

			/** index */
			int m_index;

			/** standard sample */
			kome::objects::Sample* m_stdSample;

			/** current sample */
			kome::objects::Sample* m_sample;

			/** manipulated sample */
			kome::operation::ManipulatedSample* m_manipulatedSample;

			/** read info */
			SampleReadInfo* m_readInfo;

			/** alignment */
			kome::operation::Alignment* m_align;

		public:
			/**
			 * @fn void setMatrix( int matrix )
			 * @brief set matrix
			 * @param matrix matrix id
			 */
			void setMatrix( int matrix );

			/**
			 * @fn kome::objects::Sample* next()
			 * @brief gets next sample
			 * @return next sample
			 */
			kome::objects::Sample* next();
			
		protected:
			/**
			 * @fn void releaseSample()
			 * @brief releases sample
			 */
			void releaseSample();

		public:
			/**
			 * @fn void setStdSample( kome::objects::Sample* sample )
			 * @brief sets the standard sample
			 * @param[in] sample standard sample
			 */
			void setStdSample( kome::objects::Sample* sample );

			/**
			 * @fn kome::objects::Sample* getStdSample()
			 * @brief gets the standard sample
			 * @return standard sample
			 */
			kome::objects::Sample* getStdSample();

			/**
			 * @fn void setAlignment( kome::operation::Alignment* alignment )
			 * @brief sets the alignment
			 * @param[in] alignment alignment
			 */
			void setAlignment( kome::operation::Alignment* alignment );

			/**
			 * @fn kome::operation::Alignment* getAlignment()
			 * @brief gets the alignment
			 * @return alignment
			 */
			kome::operation::Alignment* getAlignment();

			/**
			 * @fn void align( const double rt, const double mz, double* alignedRt, double* alignedMz )
			 * @brief aligns the RT and m/z position
			 * @param[in] rt RT position before alignment
			 * @param[in] mz m/z position before alignment
			 * @param[out] alignedRt the pointer to store the aligned RT
			 * @param[out] alignedMz the pointet to store the aligned m/z
			 */
			void align( const double rt, const double mz, double* alignedRt, double* alignedMz );

			/**
			 * @fn unsigned int getNumberOfSamples()
			 * @brief gets the number of samples
			 * @return the number of samples
			 */
			unsigned int getNumberOfSamples();

			/**
			 * @fn int getMatrixSampleId()
			 * @brief gets the matrix sample ID
			 * @return matrix sample ID
			 */
			int getMatrixSampleId();

			/**
			 * @fn oid setFilter(
				kome::objects::DataGroupNode* group, 
				double minInt,
				double maxInt,
				bool relFlg,
				double minRt, 
				double maxRt, 
				double minMz, 
				double maxMz,
				kome::core::Progress& progress )
			 * @brief set filter
			 * @param[in] group
			 * @param[in] minInt
			 * @param[in] maxInt
			 * @param[in] relFlg
			 * @param[in] minRt
			 * @param[in] maxRt
			 * @param[in] minMz
			 * @param[in] maxMz
			 * @param[in] progress
			 */
			void setFilter(
				kome::objects::DataGroupNode* group, 
				double minInt,
				double maxInt,
				bool relFlg,
				double minRt, 
				double maxRt, 
				double minMz, 
				double maxMz,
				kome::core::Progress& progress );

			/**
			 * @fn void setFilterCallFunctions( 
				kome::plugin::PluginFunctionItem* fun,
				kome::core::Progress& progress, 
				kome::objects::SettingParameterValues* settings,
				kome::objects::DataGroupNode* group,
				kome::objects::Peaks2D* peaks2d,
				kome::objects::Peaks2D& dst	)
			 * @brief set filter call functions
			 * @param[in] fun
			 * @param[in] progress
			 * @param[in] settings
			 * @param[in] group
			 * @param[in] peaks2d
			 * @param[in] dst
			 */
			void setFilterCallFunctions( 
				kome::plugin::PluginFunctionItem* fun,
				kome::core::Progress& progress, 
				kome::objects::SettingParameterValues* settings,
				kome::objects::DataGroupNode* group,
				kome::objects::Peaks2D* peaks2d,
				kome::objects::Peaks2D& dst
			);
		};
	}
}

#endif // __KOME_IDENT_TOOL_SAMPLE_GETTER_H__
