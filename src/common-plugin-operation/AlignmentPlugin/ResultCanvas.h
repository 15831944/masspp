/**
 * @file ResultCanvas.h
 * @brief interfaces of ResultCanvas class
 *
 * @author S.Tanaka
 * @date 2007.08.22
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_ALIGNMENT_RESULT_CANVAS_H__
#define __KOME_ALIGNMENT_RESULT_CANVAS_H__


#include "CanvasBase.h"


namespace kome {
	namespace alignment {

		/**
		 * @class ResultCanvas
		 * @brief canvas class to draw correction
		 */
		class ResultCanvas : public CanvasBase {
		public:
			/**
			 * @fn ResultCanvas( wxWindow* parent )
			 * @brief constructor
			 * @param parent parent window
			 */
			ResultCanvas( wxWindow* parent );

			/**
			 * @fn virtual ~ResultCanvas()
			 * @brief destructor
			 */
			virtual ~ResultCanvas();

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

#endif		// __KOME_ALIGNMENT_RESULT_CANVAS_H__
