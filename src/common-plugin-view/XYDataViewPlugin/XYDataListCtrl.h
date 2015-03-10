/**
 * @file XYDataListCtrl.h
 * @brief interfaces of XYDataListCtrl class
 *
 * @author S.Tanaka
 * @date 2007.11.12
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_VIEW_XY_DATA_LIST_CTRL_H__
#define __KOME_VIEW_XY_DATA_LIST_CTRL_H__


#include <vector>
#include <set>


namespace kome {
	namespace view {

		/**
		 * @class XYDataListCtrl
		 * @brief xy data list control class
		 */
		class XYDataListCtrl : public kome::window::GridListCtrl {
		public:
			/**
			 * @fn XYDataListCtrl( wxWindow* parent )
			 * @brief constructor
			 */
			XYDataListCtrl( wxWindow* parent );

			/**
			 * @fn virtual ~XYDataListCtrl()
			 * @brief destructor
			 */
			virtual ~XYDataListCtrl();

		protected:
			/** spectrum */
			kome::objects::Spectrum* m_spectrum;
			/** chromatogram */
			kome::objects::Chromatogram* m_chromatogram;
			/** xy data */
			kome::core::XYData* m_xyData;
			/** peaks flag */
			bool m_peaksFlg;
			/** x data */
			std::vector< double > m_xArray;
			/** y data */
			std::vector< double > m_yArray;
			/** left data */
			std::vector< double > m_leftArray;
			/** right data */
			std::vector< double > m_rightArray;
			/** apex data */
			std::vector< double > m_apexArray;
			/** area data */
			std::vector< double > m_areaArray;
			/** FWHM data */
			std::vector< double > m_fwhmArray;

		public:
			/**
			 * @fn void setData( kome::objects::Spectrum* spec, kome::core::XYData* xyData, const bool peaksFlg )
			 * @brief sets spectrum data information
			 * @param spec spectrum
			 * @param xyData xy data
			 * @param peaksFlg peaks flag
			 */
			void setData( kome::objects::Spectrum* spec, kome::core::XYData* xyData, const bool peaksFlg );

			/**
			 * @fn void setData( kome::objects::Chromatogram* chrom, kome::core::XYData* xyData, const bool peaksFlg )
			 * @brief sets chromatogram data information
			 * @param chrom chromatogram 
			 * @param xyData xy data
			 * @param peaksFlg peaks flag
			 */
			void setData( kome::objects::Chromatogram* chrom, kome::core::XYData* xyData, const bool peaksFlg );

		protected:
			/**
			 * @fn void create()
			 * @brief creates list ctrl
			 */
			void create();

		protected:
			/**
			 * @fn virtual double getDouble( const long long data, const int column ) const
			 * @brief gets double value from data
			 * @param data data
			 * @param column column number
			 */
			virtual double getDouble( const long long data, const int column ) const;

		private:
			DECLARE_EVENT_TABLE()
		};
	}
}


#endif // __KOME_VIEW_XY_DATA_LIST_CTRL_H__
