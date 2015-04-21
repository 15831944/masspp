/**
 * @file GLDataMapInfo.h
 * @brief interfaces of GLDataMapInfo class
 *
 * @author S.Tanaka
 * @date 2008.01.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_VIEW_GRAPH_INFO__
#define __KOME_VIEW_GRAPH_INFO__


#include <vector>
#include <set>


namespace kome {
	namespace gl {

		/**
		 * @class GLDataMapInfo
		 * @brief 3D graph information class
		 */
		class GL_TOOLS_CLASS GLDataMapInfo {
		public:
			/**
			 * @fn GLDataMapInfo( kome::objects::DataMapInfo& dataMap )
			 * @brief constructor
			 * @param[in] dataMap data map information object
			 */
			GLDataMapInfo( kome::objects::DataMapInfo& dataMap );

			/**
			 * @fn virtual ~GLDataMapInfo()
			 * @brief destructor
			 */
			virtual ~GLDataMapInfo();

		protected:
			/** data map */
			kome::objects::DataMapInfo& m_dataMap;

			/**
			 * @typedef Vec4
			 * @brief 4 dimentions vector
			 */
			typedef float Vec4[ 4 ];

			/**
			 * @typedef Vec3
			 * @brief 3 dimentions vector
			 */
			typedef float Vec3[ 3 ];

			/** vertex coordinate data */
			Vec4* m_vertices;

			/** vertex normal data */
			Vec3* m_normals;

			/** max intensity */
			float m_maxInt;

		protected:
			/** view */
			kome::core::Matrix m_view;

			/** transform matrix */
			kome::core::Matrix m_transform;

		protected:
			/** graph information set */
			static std::set< GLDataMapInfo* > m_graphSet;

			/** common view */
			static kome::core::Matrix m_commonView;

			/** common transform */
			static kome::core::Matrix m_commonTransform;

			/** synchronized view flag */
			static bool m_syncView;

			/** draw axis flag */
			static bool m_axisDrawing;

		public:
			/**
			 * @fn kome::objects::DataMapInfo& getDataMap()
			 * @brief gets data map object
			 * @return data map object
			 */
			kome::objects::DataMapInfo& getDataMap();

			/**
			 * @fn double getGraphWidth()
			 * @brief gets graph width
			 * @return graph width
			 */
			double getGraphWidth();

			/**
			 * @fn unsigned int getRowCount()
			 * @brief get the number of rows
			 * @return the number of rows
			 */
			unsigned int getRowCount();

			/**
			 * @fn unsigned int getColCount()
			 * @brief gets the number of columns
			 * @return the nubmer of columns
			 */
			unsigned int getColCount();
			
		public:
			/**
			 * @fn void update()
			 * @brief updates graph information
			 */
			void update();

			/**
			 * @fn void getXYZ(
					const unsigned int row,
					const unsigned int col,
					float* x,
					float* y,
					float* z,
					float* t
				)
			 * @brief gets the coordinate
			 * @param[in] row row number
			 * @param[in] col column number
			 * @param[out] x the pointer to store x coordinate
			 * @param[out] y the pointer to store y coordinate
			 * @param[out] z the pointer to store z coordinate
			 * @param[out] t the pointer to store texture position
			 */
			void getXYZ(
				const unsigned int row,
				const unsigned int col,
				float* x,
				float* y,
				float* z,
				float* t
			);

			/**
			 * @fn void getNormal(
					const unsigned int row,
					const unsigned int col,
					float* x,
					float* y,
					float* z
				)
			 * @brief gets the vertex normal vector
			 * @param[in] row row number
			 * @param[in] col column number
			 * @param[out] x the pointer to store x element of vertex normal
			 * @param[out] y the pointer to store y element of vertex normal
			 * @param[out] z the pointer to store z element of vertex normal
			 */
			void getNormal(
				const unsigned int row,
				const unsigned int col,
				float* x,
				float* y,
				float* z
			);

			/**
			 * @fn double getMaxIntensity()
			 * @brief gets maximum intensity
			 * @return maximum intensity
			 */
			double getMaxIntensity();

		protected:
			/**
			 * @fn void createVertices()
			 * @brief creates vertices data
			 */
			void createVertices();

			/**
			 * @fn void createNormals()
			 * @brief creates normal data
			 */
			void createNormals();

		public:
			/**
			 * @fn kome::core::Matrix& getView()
			 * @brief gets view matrix
			 * @return view matrix
			 */
			kome::core::Matrix& getView();

			/**
			 * @fn kome::core::Matrix& getTransform()
			 * @brief gets transform matrix
			 * @return transform matrix
			 */
			kome::core::Matrix& getTransform();

		public:
			/**
			 * @fn void initView()
			 * @brief initializes view
			 */
			void initView();

		public:
			/**
			 * @fn static void setSyncView()
			 * @brief sets sync view flag from ini file
			 */
			static void setSyncView();

			/**
			 * @fn static void setSyncView( const bool sync, kome::objects::DataMapInfo* dataMap )
			 * @brief sets sync view flag
			 * @param[in] sync sync view flag value
			 * @param[in] dataMap data map object to use drawing area
			 */
			static void setSyncView( const bool sync, kome::objects::DataMapInfo* dataMap );

			/**
			 * @fn static bool isSyncView()
			 * @brief gets sync view flag
			 * @return sync view flag value
			 */
			static bool isSyncView();

		public:
			/**
			 * @fn static void setAxisDrawing()
			 * @brief sets axis drawing flag value from ini file
			 */
			static void setAxisDrawing();

			/**
			 * @fn static void setAxisDrawing( const bool axisDrawing )
			 * @brief sets axis drawing flag value
			 * @param[in] axisDrawing drawing axis drawing flag value
			 */
			static void setAxisDrawing( const bool axisDrawing );

			/**
			 * @fn static bool isAxisDrawing()
			 * @brief  gets axis drawing flag value
			 * @return axis drawing flag value
			 */
			static bool isAxisDrawing();

		protected:
			/**
			 * @fn static void initView( kome::core::Matrix& view )
			 * @brief initializes view matrix
			 * @param[out] view view matrix
			 */
			static void initView( kome::core::Matrix& view );
		};
	}
}

#endif		// __KOME_VIEW_GRAPH_INFO__

