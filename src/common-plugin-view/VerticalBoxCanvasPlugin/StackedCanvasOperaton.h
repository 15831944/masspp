/**
 * @file StackedCanvasOperation.h
 * @brief stacked Canvas Operation class
 *
 * @author M.Izumi
 * @date 2013.07.05
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_VIEW_STACKED_CANVAS_OPERATION_H__
#define __KOME_VIEW_STACKED_CANVAS_OPERATION_H__

#include "VerticalBoxChromatogramCanvas.h"
#include "VerticalBoxSpectrumCanvas.h"

namespace kome {
	namespace view {
		
		/**
		 * @class stackCanvasOperation
		 * @brief stacked canvas operation class
		 */
		class stackCanvasOperation : public kome::operation::Operation{
		public:
			/**
			 * @fn stackCanvasOperation()
			 * @brief constructor
			 */
			stackCanvasOperation();

			/**
			 * @fn virtual ~stdCanvasOperation()
			 * @brief destructor
			 */
			virtual ~stackCanvasOperation();
		protected:
			kome::objects::Spectrum* m_spec;
			kome::objects::Chromatogram* m_chrom;
			wxWindow* m_parent;

			kome::window::VerticalBoxSpectrumCanvas* m_specCanvas;
			kome::window::VerticalBoxChromatogramCanvas* m_chromCanvas;

		public:
			void setSpec( kome::objects::Spectrum* spec ){ m_spec = spec; }
			kome::objects::Spectrum* getSpec(){ return m_spec; }

			void setChrom( kome::objects::Chromatogram* chrom ){ m_chrom = chrom; }
			kome::objects::Chromatogram* getChrom(){ return m_chrom; }

			void setParentWindow( wxWindow* parent ){ m_parent = parent; }
			wxWindow* getParnetWindow(){ return m_parent; }
			
			kome::window::VerticalBoxSpectrumCanvas* getSpecCanvas();
			kome::window::VerticalBoxChromatogramCanvas* getChromCanvas();
	
		private:
			bool createSpecCanvas();
			bool createChromCanvas();

			void specLoadCondition();
			void chromLoadConditon();

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

#endif	// __KOME_VIEW_STACKED_CANVAS_OPERATION_H__
