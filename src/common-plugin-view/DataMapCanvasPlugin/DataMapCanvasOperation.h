/**
 * @file DataMapCanvasOperation.h
 * @brief DataMap Canvas Operation class
 *
 * @author M.Izumi
 * @date 2013.07.08
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_VIEW_DATA_MAP_CANVAS_OPERATION_H__
#define __KOME_VIEW_DATA_MAP_CANVAS_OPERATION_H__

#include "DataMapCanvas.h"

namespace kome {
	namespace view {
	
		class DataMapCanvasOperation : public kome::operation::Operation {
		public:
			/**
			 * @fn DataMapCanvasOperation()
			 * @brief constructor
			 */
			DataMapCanvasOperation();

			/**
			 * @fn ~DataMapCanvasOperation()
			 * @brief destructor
			 */
			virtual ~DataMapCanvasOperation();

		protected:
			DataMapCanvas* m_canvas;
			kome::objects::DataMapInfo* m_dataMap;
			wxWindow* m_parent;

			
		public:
			DataMapCanvas* getCanvas();

			void setDataMapInfo( kome::objects::DataMapInfo* dataMap ){ m_dataMap = dataMap; }
			kome::objects::DataMapInfo* getDataMapInfo(){ return m_dataMap; }

			void setParentWindow( wxWindow* parent ){ m_parent = parent; }
			wxWindow* getParentWindow(){ return m_parent; }

		protected:
			/**
			 * @fn virtual bool onExecute()
			 * @brief execute operation
			 * @return If true, it succeeded to perform search.
			 */
			virtual bool onExecute();

			/**
			 * @fn virtual void onLoadCondition( boost::function< int ( void*, int ) > readFun )
			 * @brief load the saved state with respect to to search engine operations
			 * @param[in] readFun function to read saved state
			 */
			virtual void onLoadCondition( boost::function< int ( void*, int ) > readFun );

			/**
			 * @fn virtual void onSaveCondition( boost::function< int ( void*, int ) > writeFun )
			 * @brief save the current state with respect to search engine operations
			 * @param[in] writeFun function to save state
			 */
			virtual void onSaveCondition( boost::function< int ( void*, int ) > writeFun  );

			/**
			 * @fn virtual const char* onGetDescription()
			 * @brief get discription (override method)
			 * @return discription
			 */
			virtual std::string onGetDescription();
			
			/**
			 * @fn virtual const char* onGetParametersString()
			 * @brief get parameters string (override method)
			 * @return parameters string 
			 */
			virtual std::string onGetParametersString();
			
			/**
			 * @fn virtual void onSetParametersString( const char* strParam )
			 * @brief set parameters string  (override method)
			 * @param[in] strParam parameters string
			 */
			virtual void onSetParametersString( const char* strParam );
		};

	}
}

#endif // __KOME_VIEW_DATA_MAP_CANVAS_OPERATION_H__

