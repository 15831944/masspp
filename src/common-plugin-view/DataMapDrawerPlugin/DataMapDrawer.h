/**
 * @file DataMapDrawer.h
 * @brief interfaces of DataMapDrawer class
 *
 * @author S.Tanaka
 * @date 2007.09.24
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_VIEW_DATA_MAP_DRAWER_H__
#define __KOME_VIEW_DATA_MAP_DRAWER_H__

#include <list>
#include <string>

namespace kome {
	namespace view {

		class ColorTheme;

		/**
		 * @class DataMapDrawer
		 * @brief class to draw profile
		 */
		class DataMapDrawer : public kome::objects::DefaultDataManager {
		protected:
			/**
			 * @fn DataMapDrawer()
			 * @brief constructor
			 */
			DataMapDrawer();

			/**
			 * @fn virtual ~DataMapDrawer()
			 * @brief destructor
			 */
			virtual ~DataMapDrawer();

		protected:
			/**
			 * @struct BitmapInfo
			 * @brief data map information
			 */
			struct BitmapInfo {
				kome::img::BitmapData bitmap;
				kome::img::BitmapData navi;
				double minRt;
				double maxRt;
				double minMz;
				double maxMz;
				double minInt;
				double maxInt;
				int width;
				int height;
				int naviLeft;
				int naviRight;
			};

			/**
			 * @typedef Scaling
			 * @brief Scaling
			 */
			typedef enum {
				SCALE_LINEAR,
				SCALE_SQUARE,
				SCALE_LOG
			} Scaling;

			/** data map */
			std::map< kome::objects::DataMapInfo*, BitmapInfo > m_map;

			/** scaling */
			Scaling m_scaling;

			/** theme */
			ColorTheme* m_theme;

			/** themes */
			std::vector< ColorTheme* > m_themes;

		public:
			/**
			 * @fn void draw( kome::img::Graphics& g, kome::objects::DataMapInfo& dataMap )
			 * @brief draws data map
			 * @param g graphics object
			 * @param dataMap data map information
			 */
			void draw( kome::img::Graphics& g, kome::objects::DataMapInfo& dataMap );

			/**
			 * @fn void drawNaviBar( kome::img::Graphics& g, kome::objects::DataMapInfo& dataMap )
			 * @brief draws navi bar
			 * @param g graphics object
			 * @param dataMap data map information
			 */
			void drawNaviBar( kome::img::Graphics& g, kome::objects::DataMapInfo& dataMap );

			/**
			 * @fn void prepare( kome::img::Graphics& g, kome::objects::DataMapInfo& dataMap )
			 * @brief prepare to draw
			 * @param g graphics object
			 * @param dataMap data map information
			 */
			void prepare( kome::img::Graphics& g, kome::objects::DataMapInfo& dataMap );

			/**
			 * @fn void deleteData( kome::objects::DataMapInfo& dataMap )
			 * @brief delete data
			 * @param dataMap data map information
			 */
			void deleteData( kome::objects::DataMapInfo& dataMap );

			/**
			 * @fn void deleteData( kome::objects::Sample& sample )
			 * @brief delete data
			 * @param sample sample information
			 */
			void deleteData( kome::objects::Sample& sample );

		public:
			/**
			 * @fn void updateStatus()
			 * @brief update status
			 */
			void updateStatus();

		protected:
			/**
			 * @fn void createBitmap( kome::img::Graphics& g, kome::objects::DataMapInfo& dataMap );
			 * @brief creates bitmap data
			 * @param g graphics object
			 * @param dataMap data map information
			 */
			void createBitmap( kome::img::Graphics& g, kome::objects::DataMapInfo& dataMap );

			/**
			 * @fn double getScaledIntensity( const double intensity )
			 * @brief gets scaled intensity
			 * @param intensity intensity
			 * @return scaled intensity
			 */
			double getScaledIntensity( const double intensity );

			/**
			 * @fn void createThemes()
			 * @brief creates color themes
			 */
			void createThemes();

		protected:
			/**
			 * @fn virtual void onCloseSample( kome::objects::Sample* sample, const bool deleting ) 
			 * @brief This method is called when a sample is closed. (override method)
			 * @param sample sample object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void onCloseSample( kome::objects::Sample* sample, const bool deleting );

			/**
			 * @fn virtual void onCloseDataMap( DataMapInfo* dataMap, const bool deleting ) 
			 * @brief This method is called when a data map is closed. (override method)
			 * @param[in] dataMap data map object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void onCloseDataMap(  kome::objects::DataMapInfo* dataMap, const bool deleting ) ;

		public:
			/**
			 * @fn static DataMapDrawer& getInstance()
			 * @brief gets instance. (This is the only object.)
			 * @return DataMapDrawer object
			 */
			static DataMapDrawer& getInstance();
		};
	}
}

#endif		//  __KOME_VIEW_DATA_MAP_DRAWER_H__
