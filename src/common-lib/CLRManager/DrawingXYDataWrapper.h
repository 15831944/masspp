/**
 * @file DrawingXYDataWrapper.h
 * @brief interfaces of DrawingXYDataWrapper class
 *
 * @author S.Tanaka
 * @date 2007.02.07
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_CLR_DRAWING_XY_DATA_WRAPPER_H__
#define __KOME_CLR_DRAWING_XY_DATA_WRAPPER_H__

namespace kome {
	namespace clr {

		/**
		 * @class DrawingXYDataWrapper
		 * @brief drawing xy data wrapper class to use on CLR environment
		 */
		public ref class DrawingXYDataWrapper {
		public:
			/**
			 * @fn DrawingXYDataWrapper( kome::img::DrawingXYData& data )
			 * @brief constructor
			 * @param[in] data drawing xy data to be wrapped
			 */
			DrawingXYDataWrapper( kome::img::DrawingXYData& data );

			/**
			 * @fn virtual ~DrawingXYDataWrapper()
			 * @brief destructor
			 */
			virtual ~DrawingXYDataWrapper();

		protected:
			/** drawing xy data to be wrapped */
			kome::img::DrawingXYData& m_data;

		public:
			/**
			 * @fn kome::img::DrawingXYData& getDrawingXYData()
			 * @brief gets drawing xy data
			 * @return drawing xy data
			 */
			kome::img::DrawingXYData& getDrawingXYData();

		public:
			/**
			 * @fn void updatePoints()
			 * @brief update data
			 */
			void updatePoints();

			/**
			 * @fn unsigned int getLength( unsigned short level )
			 * @brief gets the number of points
			 * @param[in] level data level
			 * @return the number of points
			 */
			unsigned int getLength( unsigned short level );

			/**
			 * @fn double getX( 
					unsigned short level,
					unsigned int index 
				)
			 * @brief gets x coordinate
			 * @param[in] level data level
			 * @param[in] index point index
			 * @return x coordinate
			 */
			double getX( 
				unsigned short level,
				unsigned int index 
			);


			/**
			 * @fn double getLeftY(
					unsigned short level,
					unsigned int index
				)
			 * @brief gets y coordinate at left end
			 * @param[in] level data level
			 * @param[in] index point index
			 * @return y coordinate at left end
			 */
			double getLeftY(
				unsigned short level,
				unsigned int index
			);

			/**
			 * @fn double getTopY(
					unsigned short level,
					unsigned int index
				)
			 * @brief gets maximum y coordinate
			 * @param[in] level data level
			 * @param[in] index point index
			 * @return maximum y coordinate
			 */
			double getTopY(
				unsigned short level,
				unsigned int index
			);

			/**
			 * @fn double getRightY(
					unsigned short level,
					unsigned int index
				)
			 * @brief gets y coordinate at right end
			 * @param[in] level data level
			 * @param[in] index point index
			 * @return y coordinate at right end
			 */
			double getRightY(
				unsigned short level,
				unsigned int index
			);

			/**
			 * @fn int searchIndex( unsigned short level, double x )
			 * @brief searches index of specified x value.
			 * @param[in] level data level
			 * @param[in] x x coordinate value
			 * @return index (If there isn't specified x, this method returns negative value.
					   (See SearchTool::binarySearch. SearchTool class exists in AlgorithmTools library.)
			 */
			int searchIndex( unsigned short level, double x );
		};
	}
}

#endif		// __KOME_CLR_DRAWING_XY_DATA_WRAPPER_H__
