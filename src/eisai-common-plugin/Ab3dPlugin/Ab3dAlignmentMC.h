/**
 * @file Ab3dAlignmentMC.h
 * @brief interfaces of Ab3dAlignmentMC class
 *
 * @author S.Tanaka
 * @date 2009.02.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_AB3D_ALIGNMENT_MC_H__
#define __KOME_AB3D_ALIGNMENT_MC_H__


#include "Ab3dAlignment.h"


namespace kome {
	namespace ab3d {

		/**
		 * @class Ab3dAlignmentMC
		 * @brief AB3D alignment class
		 */
		class Ab3dAlignmentMC : public Ab3dAlignment {
		public:
			/**
			 * @fn Ab3dAlignmentMC( kome::objects::SettingParameterValues* settings )
			 * @brief constructor
			 * @param settings setting parameter values
			 */
			Ab3dAlignmentMC( kome::objects::SettingParameterValues* settings );

			/**
			 * @fn virtual ~Ab3dAlignmentMC()
			 * @brief destructor
			 */
			virtual ~Ab3dAlignmentMC();

		protected:
			/** baseline item */
			kome::plugin::PluginFunctionItem* m_baselineItem;

			/** smoothing item */
			kome::plugin::PluginFunctionItem* m_smoothingItem;

			/** window width */
			double m_windowWidth;

			/** point density */
			unsigned int m_pointDensity;

			/** gap penalty */
			double m_gapPenalty;

			/** min valid score */
			double m_invalidScore;

			/** m/z tolerance */
			double m_mzTol;

			/** m/z interval */
			double m_mzInterval;

			/** tmp directory path */
			std::string m_tmpDir;

			/** timer */
			kome::core::Timer m_timer;

		protected:
			/**
			 * @struct MCInfo
			 * @brief MC information
			 */
			struct MCInfo {
				int mzIdx;
				unsigned int num;
			};

			/** mc info */
			std::map< int, std::pair< kome::core::DataPoints, kome::core::DataPoints > > m_mcMap;

			/** mc list */
			std::list< int > m_mcList;

		protected:
			/**
			 * @fn void initMc()
			 * @briefinitializes MC information
			 */
			void initMc();

			/**
			 * @fn std::pair< kome::core::XYData*, kome::core::XYData* > getCorrectionPoint( const double mz );
			 * @brief gets the correction points
			 * @param[in] mz m/z
			 * @return correction points
			 */
			std::pair< kome::core::XYData*, kome::core::XYData* > getCorrectionPoint( const double mz );

		protected:
			/**
			 * @fn void setTmpDir()
			 * @brief sets temporary directory
			 */
			void setTmpDir();

		public:
			/**
			 * @fn double getScore(
						const int row,
						const int col,
						std::vector< double >& ctrlInts,
						std::vector< double >& trmtInts,
						std::vector< double >& scoreMatrix,
						int rowCnt,
						int colCnt
					)
			 * @brief gets score
			 * @param row row number
			 * @param col col number
			 * @param ctrlInts control intensities
			 * @param trmtInts treatment intensities
			 * @param scoreMatrix score matrix
			 * @param rowCnt row count
			 * @param colCnt col count
			 * @return score
			 */
			double getScore(
				const int row,
				const int col,
				std::vector< double >& ctrlInts,
				std::vector< double >& trmtInts,
				std::vector< double >& scoreMatrix,
				int rowCnt,
				int colCnt
			);

		protected:
			/**
			 * @fn void createPoints(
						const double mz,
						std::vector< double >& ctrlInts,
						std::vector< double >& trmtInts,
						int* rowCnt,
						int* colCnt,
						double* ctrlStart,
						double* trmtStart
					)
			 * @brief creates points
			 * @param mz m/z
			 * @param ctrlInts the array to store control intensities
			 * @param trmtInts the array to store treatment intensities
			 * @param rowCnt the pointer to store row count
			 * @para colCnt the pointer to store column count
			 * @param ctrlStart the pointer to store control start time
			 * @param trmtStart the pointer to store treatment start time
			 */
			void createPoints(
				const double mz,
				std::vector< double >& ctrlInts,
				std::vector< double >& trmtInts,
				int* rowCnt,
				int* colCnt,
				double* ctrlStart,
				double* trmtStart
			);

			/**
			 * @fn void initScoreMatrix(
						std::vector< double >& scoreMatrix,
						const unsigned int row,
						const unsigned int col
					)
			 * @brief initializes score matrix
			 * @param score matrix
			 * @param row row size
			 * @param col col size
			 */
			void initScoreMatrix(
				std::vector< double >& scoreMatrix,
				const unsigned int row,
				const unsigned int col
			);

			/**
			 * @fn kome::numeric::DynamicPrograming* execDp(
						 std::vector< double >& ctrlInts,
						 std::vector< double >& trmtInts,
						 std::vector< double >& scoreMatrix,
						 int rowCnt,
						 int colCnt,
						 const double ctrlStart,
						 const double trmtStart
					 )
			 * @brief executes dynamic programing
			 * @param ctrlInts control intensities
			 * @param trmtInts treatment intensities
			 * @param scoreMatrix score matrix
			 * @param rowCnt row count
			 * @param colCnt column count
			 * @param ctrlStart contorl start time
			 * @param trmtStart treatment start time
			 * @return Dynamic Programing object. (If NULL, it failed to execute dynamic programing.)
			 */
			 kome::numeric::DynamicPrograming* execDp(
				 std::vector< double >& ctrlInts,
				 std::vector< double >& trmtInts,
				 std::vector< double >& scoreMatrix,
				 int rowCnt,
				 int colCnt,
				 const double ctrlStart,
				 const double trmtStart
			 );

			/**
			 * @fn void traceback(
						int row,
						int col,
						kome::core::XYData* xyData,
						kome::core::Point< int >* tbPt,
						const double ctrlStart,
						const double trmtStart
					)
			 * @brief traceback
			 * @param row row number
			 * @param col column number
			 * @param xyData data points object to store correction points
			 * @param tbPt traceback point
			 * @param ctrlStart control start time
			 * @param trmtStart treatment start time
			 */
			void traceback(
				int row,
				int col,
				kome::core::XYData* xyData,
				kome::core::Point< int >* tbPt,
				const double ctrlStart,
				const double trmtStart
			);

			/**
			 * @fn std::string getRowTitle( const int row, const double ctrlStart )
			 * @brief gets row title
			 * @param row row number
			 * @param ctrlStart control start time
			 * @return row title
			 */
			std::string getRowTitle( const int row, const double ctrlStart );

			/**
			 * @fn std::string getColTitle( const int col, const double trmtStart )
			 * @brief gets column title
			 * @param col column title
			 * @param trmtStart treatment start time
			 * @return column title
			 */
			std::string getColTitle( const int col, const double trmtStart );

		protected:
			/**
			 * @fn virtual void onPrepare( kome::objects::Sample* standard, kome::objects::Sample* treatment, kome::core::Progress* progress )
			 * @brief This method is called by prepare method. (override method)
			 * @param[in] standard standard sample
			 * @param[in] treatment treatment sample
			 * @param[in,out] progress the object to show the progress
			 */
			virtual void onPrepare( kome::objects::Sample* standard, kome::objects::Sample* treatment, kome::core::Progress* progress );

			/**
			 * @fn virtual double onConvertRt( const double rt, const double mz )
			 * @brief This method is called by convertRt method. (override method)
			 * @param[in] rt RT before alignment
			 * @param[in] mz m/z before alignment
			 * @return aligned RT
			 */
			virtual double onConvertRt( const double rt, const double mz );

			/**			
			 * @fn virtual double onConvertMz( const double rt, const double mz )
			 * @brief This method is called by convertMz method. (override method)
			 * @param[in] rt RT before alignment
			 * @param[in] mz m/z before alignment
			 * @return aligned m/z
			 */
			virtual double onConvertMz( const double rt, const double mz );

			/**
			 * @fn virtual double onInvertRt( const double rt, const double mz )
			 * @brief This method is called by invertRt method. (override method)
			 * @param[in] rt aligned RT
			 * @param[in] mz aligned m/z
			 * @return RT before alignment
			 */
			virtual double onInvertRt( const double rt, const double mz );

			/**
			 * @fn virtual double onInvertMz( const double rt, const double mz )
			 * @brief This method is called by invertMz method. (override method)
			 * @parma[in] rt aligned RT
			 * @param[in] mz aligned m/z
			 * @return m/z before alignemtn
			 */
			virtual double onInvertMz( const double rt, const double mz );
		};
	}
}

#endif		// __KOME_AB3D_ALIGNMENT_MC_H__
