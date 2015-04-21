/**
 * @file GraphTool.h
 * @brief interfaces of GraphTool class
 *
 * @author S.Tanaka
 * @date 2007.08.22
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IMG_GRAPH_TOOL_H__
#define __KOME_IMG_GRAPH_TOOL_H__

#include "ColorTool.h"

namespace kome {
	namespace img {

		class Graphics;
		

		/**
		 * @class GraphTool
		 * @brief This class has some functions for graph drawing
		 */
		class IMAGE_MANAGER_CLASS GraphTool {
		public:
			/**
			 * @fn static void drawGraphRect( Graphics& graphics )
			 * @brief draws graph rect
			 * @param[in] graphics graphics object to draw graph rect
			 */
			static void drawGraphRect( Graphics& graphics );

			/**
			 * @fn static void drawXScale(
						Graphics& graphics,
						const unsigned int density,
						const int length,
						const int space,
						const bool grid,
						const double minX = -1.0,
						const double maxX = -1.0
					)
			 * @brief draws x scale
			 * @param[in] graphics graphics object to draw x scale
			 * @param[in] density scale density
			 * @param[in] length scale length
			 * @param[in] space label space (If space is negative value, label is not drawed.)
			 * @param[in] grid draw grid flag
			 * @param[in] minX minimum X
			 * @param[in] maxX maximum X
			 */
			static void drawXScale(
					Graphics& graphics,
					const unsigned int density,
					const int length,
					const int space,
					const bool grid,
					const double minX = -1.0,
					const double maxX = -1.0
			);

			/**
			 * @fn static void drawYScale(
						Graphics& graphics,
						const unsigned int density,
						const int length,
						const int space,
						const bool grid,
						const double minY = -1.0,
						const double maxY = -1.0
					)
			 * @brief draws y scale
			 * @param[in] graphics graphics object to draw y scale
			 * @param[in] density scale density
			 * @param[in] length scale length
			 * @param[in] space label space (If space is negative value, label is not drawed.)
			 * @param[in] grid draw grid flag
			 * @param[in] minY minimum Y
			 * @param[in] maxY maximum Y
			 */
			static void drawYScale(
					Graphics& graphics,
					const unsigned int density,
					const int length,
					const int space,
					const bool grid,
					const double minY = -1.0,
					const double maxY = -1.0
			);

			/**
			 * @fn static int getMaxWidthOfYLabels(
						Graphics& graphics,
						const unsigned int density,
						const double minY = -1.0,
						const double maxY = -1.0
				    )
			 * @brief gets max width of y labels
			 * @param[in] graphics graphics object to draw y scale
			 * @param[in] density scale density
			 * @param[in] minY minimum Y
			 * @param[in] maxY maximum Y
			 * @return max width of y labels
			 */
			static int getMaxWidthOfYLabels(
					Graphics& graphics,
					const unsigned int density,
					const double minY = -1.0,
					const double maxY = -1.0
			);

			/**
			 * @fn static COLOR getColorFromIni( const char* section, const char* key, const char* defaultColor )
			 * @brief gets color from ini file
			 * @param[in] section ini file section
			 * @param[in] key ini file key
			 * @param[in] defaultColor defalut color
			 * @return color
			 */
			static COLOR getColorFromIni( const char* section, const char* key, const char* defaultColor );
		
			/**
			 * @fn static int getWidthFromIni( const char* section, const char* key )
			 * @brief gets width from ini file
			 * @param[in] section ini file section
			 * @param[in] key ini file key
			 * @return width
			 */
			static int getWidthFromIni( const char* section, const char* key );
			
		};
	}
}

#endif		// __KOME_IMG_GRAPH_TOOL_H__
