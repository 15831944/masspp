/**
 * @file CorrectionCanvas.h
 * @brief interfaces of CorrectionCanvas class
 *
 * @author S.Tanaka
 * @date 2007.08.22
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_ALIGNMENT_CORRECTION_CANVAS_H__
#define __KOME_ALIGNMENT_CORRECTION_CANVAS_H__


#include "CanvasBase.h"


namespace kome {
	namespace alignment {

		/**
		 * @class CorrectionCanvas
		 * @brief canvas class to draw correction
		 */
		class CorrectionCanvas : public CanvasBase {
		public:
			/**
			 * @fn CorrectionCanvas( wxWindow* parent )
			 * @brief constructor
			 * @param parent parent window
			 */
			CorrectionCanvas( wxWindow* parent );

			/**
			 * @fn virtual ~CorrectionCanvas()
			 * @brief destructor
			 */
			virtual ~CorrectionCanvas();

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

#endif		// __KOME_ALIGNMENT_CORRECTION_CANVAS_H__
