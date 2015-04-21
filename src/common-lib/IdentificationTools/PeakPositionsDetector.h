/**
 * @file PeakPositionsDetector.h
 * @brief PeakPositions Detector class
 *
 * @author M.Izumi
 * @date 2013.12.10
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_IDENT_TOOL_PEAK_POSITONS_DETECTOR_H__
#define __KOME_IDENT_TOOL_PEAK_POSITONS_DETECTOR_H__

namespace kome {
	namespace ident {
		
		class PeakPositions;		

		/**
		 * @class PeakPositionsDetector
		 * @brief peak positons detector
		 */
		class IDENT_TOOLS_CLASS PeakPositionsDetector {
		public:
			/**
			 * @fn PeakPositionsDetector( bool bCalcFlg, bool bAlignmentFlg )
			 * @brief constructor
			 * @param[in] bCalcFlg
			 * @param[in] bAlignmentFlg
			 */
			PeakPositionsDetector( bool bCalcFlg, bool bAlignmentFlg );

			/**
			 * @fn virtual ~PeakPositionsDetector()
			 * @brief destructor
			 */
			virtual ~PeakPositionsDetector();

		protected:
			/** calc flag */
			bool m_bCalcFlg;
			/** alignment flg */
			bool m_bAlignmentFlg;
			/** error message */
			std::string m_errorMsg;

		public:
			/**
			 * @typedef PeakInfoFlag
			 * @brief peak info flag
			 */
			typedef enum {
				HAS_RT = 1,
				HAS_MZ = 2,
				HAS_CHARGE = 4

			} PeakInfoFlag;
		protected:

			/**
			 * @fn virtual int onDetectPeakPositions( SampleGetter& sample, PeakPositions& peakPos, kome::core::Progress& progress ) = 0
			 * @brief detect peak positons
			 * @param[in] sample
			 * @param[in] peakPos
			 * @param[in] progress
			 * @return zero
			 */
			virtual int onDetectPeakPositions( SampleGetter& sample, PeakPositions& peakPos, kome::core::Progress& progress ) = 0;

		public:
			/**
			 * @fn bool canCalculatePeakValues()
			 * @brief can calculate peak values
			 * @return can calculate peak values
			 */
			bool canCalculatePeakValues();

			/**
			 * @fn bool isAlignmentSuppoprted()
			 * @brief is alignment suppoprted
			 * @return is alignment suppoprted
			 */
			bool isAlignmentSuppoprted();

			/**
			 * @fn int detectPeakPositions( SampleGetter& sample, PeakPositions& peakPos, kome::core::Progress& progress )
			 * @brief detect peak positions
			 * @param[in] sample
			 * @param[in] peakPos
			 * @param[in] progress
			 * @return detect peak position value
			 */
			int detectPeakPositions( SampleGetter& sample, PeakPositions& peakPos, kome::core::Progress& progress );

			/**
			 * @fn void setLastError( const char* msg )
			 * @brief
			 * @param[in] msg error message
			 */
			void setLastError( const char* msg );

			/**
			 * @fn std::string getLastError()
			 * @brief get the last error
			 * @return error message
			 */
			std::string getLastError();

		};

	}
}

#endif // __KOME_IDENT_TOOL_PEAK_POSITONS_DETECTOR_H__
