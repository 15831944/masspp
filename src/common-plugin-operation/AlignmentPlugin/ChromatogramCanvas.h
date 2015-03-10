/**
 * @file ChromatogramCanvas.h
 * @brief interfaces of ChromatogramCanvas class
 *
 * @author S.Tanaka
 * @date 2007.08.20
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_ALIGNMENT_CHROMATOGRAM_CANVAS_H__
#define __KOME_ALIGNMENT_CHROMATOGRAM_CANVAS_H__


#include "CanvasBase.h"


namespace kome {
	namespace alignment {

		/**
		 * @class ChromatogramCanvas
		 * @brief canvas class to draw chromatogram
		 */
		class ChromatogramCanvas : public CanvasBase {
		public:
			/**
			 * @fn ChromatogramCanvas( wxWindow* parent, const int width, const int height, const bool ctrl )
			 * @brief constructor
			 * @param parent parent window
			 * @param width window width
			 * @param height window height
			 * @param ctrl control flag
			 */
			ChromatogramCanvas( wxWindow* parent, const int width, const int height, const bool ctrl );

			/**
			 * @fn virtual ~ChromatogramCanvas()
			 * @brief destructor
			 */
			virtual ~ChromatogramCanvas();

		protected:
			/** control flag */
			const bool m_ctrlFlg;

		protected:
			/**
			 * @fn virtual void getChromatograms(
					std::vector< kome::objects::Chromatogram* >& chroms,
					std::vector< COLOR >& colors
				)
			 * @brief gets chromatograms (override method)
			 * @param chroms the array object to store chromatograms
			 * @param colors the array object to store colors to draw profile
			 */
			virtual void getChromatograms(
				std::vector< kome::objects::Chromatogram* >& chroms,
				std::vector< COLOR >& colors
			);
		
		private:
			DECLARE_EVENT_TABLE()
		};
	}
}

#endif		// __KOME_ALIGNMENT_CHROMATOGRAM_CANVAS_H__
