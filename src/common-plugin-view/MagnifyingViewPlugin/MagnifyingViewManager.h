/**
 * @file MagnifyingViewManager.h
 * @brief interfaces of MagnifyingViewManager class
 *
 * @author N.Tashiro
 * @date 2012.03.06
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_VIEW_MAGNIFYING_VIEW_MANAGER_H__
#define __KOME_VIEW_MAGNIFYING_VIEW_MANAGER_H__


#include "MagnifyDialog.h"

namespace kome {
	namespace view {

		/**
		 * @class MagnifyingViewManager
		 * @brief change range plug-in management class
		 */
		class MagnifyingViewManager {
		protected:
			/**
			 * @fn MagnifyingViewManager()
			 * @brief constructor
			 */
			MagnifyingViewManager();

			/**
			 * @fn virtual ~MagnifyingViewManager()
			 * @brief destructor
			 */
			virtual ~MagnifyingViewManager();

		public:
			/**
			 * @struct ScaleInfo
			 * @brief scale information
			 */
			struct ScaleInfo {
				double					 minX;
				double					 maxX;
				int						 scale;
				kome::objects::Spectrum* spec;
				
			};

		protected:
			/** Spectram ScaleInfo */
			std::map< kome::objects::Spectrum*, std::vector<ScaleInfo> > m_scaleMap;

			/** draw flag */
			bool m_bDraw;

		public:
			/**
			 * @fn ScaleInfo* addScaleInfo( kome::objects::Spectrum* spec, const double start, const double end )
			 * @brief adds scale information
			 * @param[in] spec spectrum
			 * @param[in] start the start of the range
			 * @param[in] end the end of the range
			 * @return scale information object
			 */
			ScaleInfo* addScaleInfo( kome::objects::Spectrum* spec, const double start, const double end );

			/**
			 * @fn unsigned int getNumberOfScaleInfo( kome::objects::Spectrum* spec )
			 * @brief gets the number of scale information
			 * @param[in] spec spectrum
			 * @return the number of scale information
			 */
			unsigned int getNumberOfScaleInfo( kome::objects::Spectrum* spec );

			/**
			 * @fn ScaleInfo* getScaleInfo( kome::objects::Spectrum* spec, const unsigned int idx )
			 * @brief gets scale information
			 * @param[in] spec spectrum
			 * @param[in] idx scale information index
			 * @return scale information
			 */
			ScaleInfo* getScaleInfo( kome::objects::Spectrum* spec, const unsigned int idx );

			/**
			 * @fn void removeScaleInfo( kome::objects::Spectrum* spec, const unsigned int idx )
			 * @brief removes the scale information
			 * @param[in] spec spectrum
			 * @param[in] idx scale information idex
			 */
			void removeScaleInfo( kome::objects::Spectrum* spec, const unsigned int idx );

			/**
			 * @fn void clearScaleInfo( kome::objects::Spectrum* spec )
			 * @brief clears scale information
			 * @param[in] spec spectrum
			 */
			void clearScaleInfo( kome::objects::Spectrum* spec );

		public:
			/**
			 * @fn bool downButton( kome::objects::Spectrum& spec,
									kome::evt::MouseEvent& evt,
									kome::img::Graphics& graphics,
									wxWindow* canvas )
			 * @brief button down event
			 * @param spec spectrum object
			 * @param evt mouse event information object
			 * @param graphics graphics object
			 * @param canvas canva ID
			 * @return refresh flag 
			 */
			bool downButton( kome::objects::Spectrum& spec,
							 kome::evt::MouseEvent& evt,
							 kome::img::Graphics& graphics,
							 wxWindow* canvas );

			/**
			 * @fn bool dblClick( kome::objects::Spectrum& spec,
								  kome::evt::MouseEvent& evt,
								  kome::img::Graphics& graphics,
								  wxWindow* canvas )
			 * @brief double click event
			 * @param spec spectrum object
			 * @param evt mouse event information object
			 * @param graphics graphics object
			 * @param canvas canvas
			 * @return refresh flag
			 */
			bool dblClick( kome::objects::Spectrum& spec,
						   kome::evt::MouseEvent& evt,
						   kome::img::Graphics& graphics,
						   wxWindow* canvas );

			/**
			 * @fn bool DrawSpecWindowFg( kome::objects::Parameters* params )
			 * @brief This function is called when graphics changed on the spectrum window.
			 * @param params parameters object. (This has mouse event and graphics object.)
			 * @return refresh flag (boolean value)
			 */
			bool DrawSpecWindowFg( kome::objects::Parameters* params );

			/**
			 * @fn bool DrawMagnify( unsigned int nID,
									 kome::objects::Spectrum* spectrum,
									 kome::img::Graphics* graphics,
									 wxWindow* canvas )
			 * @brief This function is called when graphics changed on the spectrum window.
			 * @param nID ScaleInfo position
			 * @param spec spectrum object
			 * @param graphics graphics object
			 * @param canvas canvas
			 * @return refresh flag (boolean value)
			 */
			bool DrawMagnify( unsigned int nID,
							  kome::objects::Spectrum* spectrum,
							  kome::img::Graphics* graphics,
							  wxWindow* canvas );

			//2012/03/22 upd by t.okuno -> update drawing label.
			/**
			 * @fn void drawLabel( kome::img::DrawingXYData* drawing,
								   const int accuracy,
								   kome::objects::Peaks* peaks,
								   kome::core::XYData* xyData,
								   kome::img::Graphics* g )
			 * @brief draws label
			 * @param drawing drawing data
			 * @param accuracy accuracy
			 * @param peaks peaks
			 * @param xyData data points
			 * @param g graphics object
			 */
			void drawLabel( kome::img::DrawingXYData* drawing,
							const int accuracy,
							kome::objects::Peaks* peaks,
							kome::core::XYData* xyData,
							kome::img::Graphics* g );
			//2012/03/22 upd by t.okuno <- update drawing label.

		public:
			/**
			 * @fn static MagnifyingViewManager& getInstance()
			 * @brief gets Magnifying View plug-in management object
			 * @return Magnifying View plug-in management object (This is the only object.)
			 */
			static MagnifyingViewManager& getInstance();
		};
	}
}

#endif		// __KOME_VIEW_MAGNIFYING_VIEW_MANAGER_H__
