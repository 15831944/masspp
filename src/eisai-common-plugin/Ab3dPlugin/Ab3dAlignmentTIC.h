/**
 * @file Ab3dAlignmentTIC.h
 * @brief interfaces of Ab3dAlignmentTIC class
 *
 * @author S.Tanaka
 * @date 2007.08.23
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_AB3D_ALIGNMENT_TIC_H__
#define __KOME_AB3D_ALIGNMENT_TIC_H__


#include "Ab3dAlignment.h"


namespace kome {
	namespace ab3d {

		/**
		 * @class Ab3dAlignmentTIC
		 * @brief AB3D alignment class
		 */
		class Ab3dAlignmentTIC : public Ab3dAlignment {
		public:
			/**
			 * @fn Ab3dAlignmentTIC( kome::objects::SettingParameterValues* settings )
			 * @brief constructor
			 * @param settings setting parameter values
			 */
			Ab3dAlignmentTIC( kome::objects::SettingParameterValues* settings );

			/**
			 * @fn virtual ~Ab3dAlignmentTIC()
			 * @brief destructor
			 */
			virtual ~Ab3dAlignmentTIC();

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

			/** traceback position */
			kome::core::Point< int > m_tbPt;

			/** correction point */
			kome::core::DataPoints m_corrPts;

			/** inverce point */
			kome::core::DataPoints m_invPts;

		public:
			/**
			 * @fn double getScore( const int row, const int col )
			 * @brief gets score
			 * @param row row number
			 * @param col col number
			 * @return score
			 */
			double getScore( const int row, const int col );

		protected:
			/** row count */
			int m_row;
			/** col count */
			int m_col;
			/** start RT of control */
			double m_ctrlStart;
			/** start RT of treatment */
			double m_trmtStart;

			/** control intensities */
			double* m_ctrlIntensities;
			/** treatment intensities */
			double* m_trmtIntensities;

			/** score matrix */
			double* m_scoreMatrix;

			/** dynamic programing */
			kome::numeric::DynamicPrograming* m_dp;

		protected:
			/**
			 * @fn void createPoints()
			 * @brief creates points
			 */
			void createPoints();

			/**
			 * @fn void initScoreMatrix()
			 * @brief initializes score matrix
			 */
			void initScoreMatrix();

			/**
			 * @fn bool execDp( kome::core::Progress* progress )
			 * @brief executes dynamic programing
			 * @param progress progress object to show progress
			 * @return If true, it succeeded to execute dynamic programing.
			 */
			bool execDp( kome::core::Progress* progress );

			/**
			 * @fn void traceback( int row, int col, kome::core::XYData* xyData )
			 * @brief traceback
			 * @param row row number
			 * @param col column number
			 * @param xyData data points object to store correction points
			 */
			void traceback( int row, int col, kome::core::XYData* xyData );

			/**
			 * @fn std::string getRowTitle( const int row )
			 * @brief gets row title
			 * @param row row number
			 * @return row title
			 */
			std::string getRowTitle( const int row );

			/**
			 * @fn std::string getColTitle( const int col )
			 * @brief gets column title
			 * @param col column title
			 * @return column title
			 */
			std::string getColTitle( const int col );

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

#endif		// __KOME_AB3D_ALIGNMENT_TIC_H__
