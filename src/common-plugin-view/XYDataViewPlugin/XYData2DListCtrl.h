/**
 * @file XYData2DListCtrl.h
 * @brief interfaces of XYData2DListCtrl class
 *
 * @author okada_h
 * @date 2011/05/25
 * 
 * Copyright(C) 2014 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_VIEW_XY_DATA_2D_LIST_CTRL_H__
#define __KOME_VIEW_XY_DATA_2D_LIST_CTRL_H__


#include <vector>
#include <set>


namespace kome {
	namespace view {

		/**
		 * @class XYData2DListCtrl
		 * @brief xy data list control class
		 */
		class XYData2DListCtrl : public kome::window::GridListCtrl {
		public:
			/**
			 * @fn XYData2DListCtrl( wxWindow* parent )
			 * @brief constructor
			 */
			XYData2DListCtrl( wxWindow* parent );

			/**
			 * @fn virtual ~XYData2DListCtrl()
			 * @brief destructor
			 */
			virtual ~XYData2DListCtrl();

		protected:
			/** spectrum */
			kome::objects::Spectrum* m_spectrum;
			/** chromatogram */
			kome::objects::Chromatogram* m_chromatogram;
			/** xy data */
			kome::core::XYData* m_xyData;
			/** peaks flag */
			int m_peaksFlg;	// enum peak_kind
			/** x data */
			std::vector< double > m_xArray;
			/** y data */
			std::vector< double > m_yArray;
			/** left x data */
			std::vector< double > m_lxArray;
			/** left y data */
			std::vector< double > m_lyArray;
			/** right x data */
			std::vector< double > m_rxArray;
			/** right y data */
			std::vector< double > m_ryArray;
			/** apex x ndata */
			std::vector< double > m_axArray;
			/** apex y ndata */
			std::vector< double > m_ayArray;
			/** charge */
			std::vector< int > m_chargeArray;
			/** area data */
			std::vector< double > m_areaArray;
			/** FWHM data */
			std::vector< double > m_fwhmArray;

		protected:
			/** spectrum group */
			kome::objects::DataGroupNode* m_group;

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
			 * @fn void createXYData()
			 * @brief creates list ctrl
			 */
			void createXYData();// @date 2011/05/25 <Add> OKADA

		protected:
			/**
			 * @fn virtual int getInt( const long long data, const int column ) const
			 * @brief gets integer value from data
			 * @param[in] data data
			 * @param[in] column column number
			 * @return integer value
			 */
			virtual int getInt( const long long data, const int column ) const;

			/**
			 * @fn virtual double getDouble( const long long data, const int column ) const
			 * @brief gets double value from data
			 * @param data data
			 * @param column column number
			 */
			virtual double getDouble( const long long data, const int column ) const;

			/**
			 * @fn double getDoubleXYData( const long long data, const int column ) const
			 * @brief gets double value from data
			 * @param data data
			 * @param column column number
			 */
			double getDoubleXYData( const long long data, const int column ) const;

		public:
			/**
			 * @fn void setGroup( kome::objects::DataGroupNode* group )
			 * @brief sets spectrum group
			 * @param group spectrum group
			 */
			void setGroup( kome::objects::DataGroupNode* group );

		protected:
			/**
			 * @fn void create2D()
			 * @brief creates list for heatmap
			 */
			void create2D();

		protected:
			/**
			 * @fn virtual double getDouble2D( const long long data, const int column ) const
			 * @brief gets double value from data for heatmap
			 * @param data data
			 * @param column column number
			 */
			virtual double getDouble2D( const long long data, const int column ) const;
						
		protected:
			/**
			 * @fn void onSelectItem( wxListEvent& evt )
			 * @brief This method is called when peaks are selected
			 * @param evt event information
			 */
			void onSelectItem( wxListEvent& evt );

		private:
			DECLARE_EVENT_TABLE()
		};
	}
}


#endif // __KOME_VIEW_XY_DATA_2D_LIST_CTRL_H__
