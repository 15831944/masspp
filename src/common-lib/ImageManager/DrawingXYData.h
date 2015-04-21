/**
 * @file DrawingXYData.h
 * @brief interfaces of DrawingXYData class
 *
 * @author S.Tanaka
 * @date 2006.09.06
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IMG_DRAWING_XY_DATA_H__
#define __KOME_IMG_DRAWING_XY_DATA_H__


#include <vector>


namespace kome {
	namespace img {

		/**
		 * @class DrawingXYData
		 * @brief class that treats data to draw profile fast
		 */		
		class IMAGE_MANAGER_CLASS DrawingXYData {
		public:
			/**
			 * @fn DrawingXYData( kome::core::XYData& xyData )
			 * @brief constructor
			 * @param[in] xyData profile data
			 */
			DrawingXYData( kome::core::XYData& xyData );

			/**
			 * @fn virtual ~DrawingXYData()
			 * @brief destructor
			 */
			virtual ~DrawingXYData();

		protected:
			/**
			 * @struct SumPoint
			 * @brief summarized data in specific section
			 */
			struct SumPoint {
				double x;
				double leftX;
				double leftY;
				double topX;
				double topY;
				double bottomX;
				double bottomY;
				double rightX;
				double rightY;
			};

			/** profile data */
			kome::core::XYData* m_xyData;

			/** data to draw profile fast */
			std::vector< SumPoint >* m_data;

		public:
			/**
			 * @fn void updatePoints()
			 * @brief update data
			 */
			void updatePoints();

			/**
			 * @fn unsigned int getLength( const unsigned short level )
			 * @brief gets the number of points
			 * @param[in] level data level
			 * @return the number of points
			 */
			unsigned int getLength( const unsigned short level );

			/**
			 * @fn double getX( 
					const unsigned short level,
					const unsigned int index 
				)
			 * @brief gets x coordinate
			 * @param[in] level data level
			 * @param[in] index point index
			 * @return x coordinate
			 */
			double getX( 
				const unsigned short level,
				const unsigned int index 
			);

			/**
			 * @fn double getLeftX(
					const unsigned short level,
					const unsigned int index
				)
			 * @brief gets x coordinate at left end
			 * @param[in] level data level
			 * @param[in] index point index
			 * @return x coordinate at left end
			 */
			double getLeftX(
				const unsigned short level,
				const unsigned int index
			);

			/**
			 * @fn double getLeftY(
					const unsigned short level,
					const unsigned int index
				)
			 * @brief gets y coordinate at left end
			 * @param[in] level data level
			 * @param[in] index point index
			 * @return y coordinate at left end
			 */
			double getLeftY(
				const unsigned short level,
				const unsigned int index
			);

			/**
			 * @fn double getTopX(
					const unsigned short level,
					const unsigned int index
				)
			 * @brief gets x coordinate at top
			 * @param[in] level data level
			 * @param[in] index point index
			 * @return x coordinate at top
			 */
			double getTopX(
				const unsigned short level,
				const unsigned int index
			);

			/**
			 * @fn double getTopY(
					const unsigned short level,
					const unsigned int index
				)
			 * @brief gets maximum y coordinate
			 * @param[in] level data level
			 * @param[in] index point index
			 * @return maximum y coordinate
			 */
			double getTopY(
				const unsigned short level,
				const unsigned int index
			);

			/**
			 * @fn double getBottomX(
					const unsigned short level,
					const unsigned int index
				)
			 * @brief gets x coordinate at bottom
			 * @param[in] level data level
			 * @param[in] index point index
			 * @return x coordinate at bottom
			 */
			double getBottomX(
				const unsigned short level,
				const unsigned int index
			);

			/**
			 * @fn double getBottomY(
					const unsigned short level,
					const unsigned int index
				)
			 * @brief gets minimum y coordinate
			 * @param[in] level data level
			 * @param[in] index point index
			 * @return minimum y coordinate
			 */
			double getBottomY(
				const unsigned short level,
				const unsigned int index
			);

			/**
			 * @fn double getRightX(
					const unsigned short level,
					const unsigned int index
				)
			 * @brief gets x coordinate at right end
			 * @param[in] level data level
			 * @param[in] index point index
			 * @return x coordinate at right end
			 */
			double getRightX(
				const unsigned short level,
				const unsigned int index
			);

			/**
			 * @fn double getRightY(
					const unsigned short level,
					const unsigned int index
				)
			 * @brief gets y coordinate at right end
			 * @param[in] level data level
			 * @param[in] index point index
			 * @return y coordinate at right end
			 */
			double getRightY(
				const unsigned short level,
				const unsigned int index
			);

			/**
			 * @fn int searchIndex( const unsigned short level, const double x )
			 * @brief searches index of specified x value.
			 * @param[in] level data level
			 * @param[in] x x coordinate value
			 * @return index (If there isn't specified x, this method returns negative value.
					   (See SearchTool::binarySearch. SearchTool class exists in AlgorithmTools library.)
			 */
			int searchIndex( const unsigned short level, const double x );

		protected:
			/**
			 * @fn void checkIndex( const unsigned short level, const unsigned int index )
			 * @brief check index
			 * @param[in] level data level
			 * @param[in] index index to be checked
			 */
			void checkIndex( const unsigned short level, const unsigned int index );

			/**
			 * @fn void createArray()
			 * @brief creates data to draw profile fast
			 */
			void createArray();

			/**
			 * @fn double getY( kome::core::XYData& xyData, const int idx )
			 * @brief gets y-coordinate
			 * @param[in] xyData xy data
			 * @param[in] idx index
			 * @return y-coordinate
			 */
			double getY( kome::core::XYData& xyData, const int idx );

		public:
			/**
			 * @fn static unsigned short getLevel(
					const unsigned int width,
					const double minX,
					const double maxX
				)
			 * @brief calculate data level to use
			 * @param[in] width window width
			 * @param[in] minX minimum x coordinate of displayed data
			 * @param[in] maxX maximum x coordinate of displayed data
			 * @return data level to use
			 */
			static unsigned short getLevel(
				const unsigned int width,
				const double minX,
				const double maxX
			);

		};
	}
}

#endif	//  __KOME_IMG_DRAWING_XY_DATA_H__
