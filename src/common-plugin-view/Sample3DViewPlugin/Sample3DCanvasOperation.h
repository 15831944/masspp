/**
 * @file Sample3DCanvasOperation.h
 * @brief Sample 3D Canvas Operation class
 *
 * @author M.Izumi
 * @date 2013.07.08
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_VIEW_SAMPLE_3D_CANVAS_OPERATION_H__
#define __KOME_VIEW_SAMPLE_3D_CANVAS_OPERATION_H__

#include "Sample3DCanvas.h"

namespace kome {
	namespace view {
		class Sample3DCanvasOperation : public kome::operation::Operation {
		public:
			/**
			 * @fn Sample3DCanvasOperation()
			 * @brief constructor
			 */
			Sample3DCanvasOperation();

			/**
			 * @fn ~Sample3DCanvasOperation()
			 * @brief destructor
			 */
			virtual ~Sample3DCanvasOperation();

		protected:
			kome::objects::DataMapInfo* m_dataMap;
			wxWindow* m_parent;
			Sample3DCanvas* m_canvas;
			
		public:
			Sample3DCanvas* getCanvas();
			
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


#endif	// __KOME_VIEW_SAMPLE_3D_CANVAS_OPERATION_H__
