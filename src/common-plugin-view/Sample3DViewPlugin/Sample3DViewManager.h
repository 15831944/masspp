/**
 * @file Sample3DViewManager.h
 * @brief interfaces of Sample3DViewManager class
 *
 * @author S.Tanaka
 * @date 2008.01.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_VIEW_RAW_DATA_3D_VIEW_MANAGER_H__
#define __KOME_VIEW_RAW_DATA_3D_VIEW_MANAGER_H__


namespace kome {
	namespace view {

		class Sample3DCanvas;

		/**
		 * @class Sample3DViewManager
		 * @brief 3D View management class
		 */
		class Sample3DViewManager {
		protected:
			/**
			 * @fn Sample3DViewManager()
			 * @brief constructor
			 */
			Sample3DViewManager();

			/**
			 * @fn virtual ~Sample3DViewManager()
			 * @brief destructor
			 */
			virtual ~Sample3DViewManager();

		public:
			/** texture image width */
			enum{
				GRAPH_TEXTURE_IMAGE_SIZE  = 512
			};

			/** graph texture image */
			unsigned char m_graphTexture[ GRAPH_TEXTURE_IMAGE_SIZE ][ 3 ];

		public:
			/**
			 * @fn void createGraphTextureImage()
			 * @brief creates graph texture image
			 */
			void createGraphTextureImage();

			/**
			 * @fn void* getGraphTextureImage()
			 * @brief gets graph texture image
			 * @return graph texture image data
			 */
			void* getGraphTextureImage();

			/**
			 * @fn unsigned int getGraphTextureImageSize()
			 * @brief gets graph texture image size
			 * @return graph texture image size
			 */
			unsigned int getGraphTextureImageSize();
		public:
			/**
			 * @fn static Sample3DViewManager& getInstance()
			 * @brief gets 3D View management object.
			 * @return 3D View management object. (This is the only object.)
			 */
			static Sample3DViewManager& getInstance();

			/**
			  * @fn void open( kome::objects::DataGroupNode* group, kome::objects::DataSet* dataSet )
			  * @brief opens data map
			  * @param group spectrum group
			  * @param spectra spectra
			  */
			void open3D( kome::objects::DataGroupNode* group, kome::objects::DataSet* dataSet );

		};
	}
}

#endif		// __KOME_VIEW_RAW_DATA_3D_VIEW_MANAGER_H__
