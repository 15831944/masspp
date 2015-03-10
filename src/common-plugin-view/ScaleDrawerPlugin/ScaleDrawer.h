/**
 * @file ScaleDrawer.h
 * @brief interfaces of ScaleDrawer class
 *
 * @author S.Tanaka
 * @date 2007.03.01
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_VIEW_SCALE_DRAWER_H__
#define __KOME_VIEW_SCALE_DRAWER_H__

#include <list>
#include <string>

namespace kome {
	namespace view {
		/**
		 * @class ScaleDrawer
		 * @brief class to draw profile
		 */
		class ScaleDrawer {
		protected:
			/**
			 * @fn ScaleDrawer()
			 * @brief constructor
			 */
			ScaleDrawer();

			/**
			 * @fn virtual ~ScaleDrawer()
			 * @brief destructor
			 */
			virtual ~ScaleDrawer();

		protected:
			/** font */
			kome::img::Font m_font;

			/**
			 * @struct ScaleData
			 * @brief scale data
			 */
			struct ScaleData {
				std::string label;
				int pos;
			};

		public:
			/**
			 * @fn void prepareToDraw( kome::img::Graphics* g )
			 * @brief prepare to draw profile
			 * @param g graphics object
			 */
			void prepareToDraw( kome::img::Graphics* g );

			/**
			 * @fn void drawScale( kome::objects::Spectrum* spec, kome::img::Graphics* g )
			 * @brief draws scale
			 * @param spec spectrum object
			 * @param g graphics object
			 */
			void drawScale( kome::objects::Spectrum* spec, kome::img::Graphics* g );

			/**
			 * @fn void drawScale( kome::objects::Chromatogram* chrom, kome::img::Graphics* g )
			 * @brief draws scale
			 * @param chrom chromatogram object
			 * @param g graphics object
			 */
			void drawScale( kome::objects::Chromatogram* chrom, kome::img::Graphics* g );


			/**
			 * @fn void drawScale( kome::objects::DataMapInfo* dataMap, kome::img::Graphics* g )
			 * @brief draw scale
			 * @param dataMap data map object
			 * @param g graphics object
			 */
			void drawScale( kome::objects::DataMapInfo* dataMap, kome::img::Graphics* g );

		protected:
			/**
			 * @fn void drawScale( kome::img::Graphics* g, const char* xTitle, const char* yTitle )
			 * @brief draws scale
			 * @param g graphics object
			 * @param xTitle the title of x axis
			 * @param yTitle the title of y axis
			 */
			void drawScale( kome::img::Graphics* g, const char* xTitle, const char* yTitle );
		
		protected:
			/**
			 * @fn static void getScaleInfo(
					const unsigned int length,
					double minVal,
					double maxVal,
					int* accuracy,
					bool* insertHalf,
					double* unit,
					std::string& fmt
				)
			 * @brief gets scale information
			 * @param length length of graph area
			 * @param minVal minimum value
			 * @param maxVal maximum value
			 * @param accuracy the pointer to store accuracy value
			 * @param insertHalf the pointer to store whether it must insert half scale
			 * @param unit the pointer to store scale unit
			 * @param fmt string object to store scale label format
			 */
			static void getScaleInfo(
				const unsigned int length,
				double minVal,
				double maxVal,
				int* accuracy,
				bool* insertHalf,
				double* unit,
				std::string& fmt
			);

		public:
			/**
			 * @fn static ScaleDrawer& getInstance()
			 * @brief gets instance. (This is the only object.)
			 * @return ScaleDrawer object
			 */
			static ScaleDrawer& getInstance();
		};
	}
}

#endif		//  __KOME_VIEW_SCALE_DRAWER_H__
