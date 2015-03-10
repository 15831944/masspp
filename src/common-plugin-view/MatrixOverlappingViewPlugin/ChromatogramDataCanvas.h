/**
 * @file ChromatogramDataCanvas.h
 * @brief interfaces of ChromatogramDataCanvas class
 *
 * @author S.Tanaka
 * @date 2008.04.28
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CHROMATOGRAM_DATA_CANVAS_H__
#define __KOME_CHROMATOGRAM_DATA_CANVAS_H__


#include "ProfilePanel.h"


namespace kome {
	namespace matrix {
		namespace overlapping {

			/**
			* @class ChromatogramDataCanvas
			* @brief matrix data profile canvas
			*/
			class ChromatogramDataCanvas : public kome::window::OverlappingChromatogramsCanvas {
			public:
				/**
				 * @fn ChromatogramDataCanvas(
							wxWindow* parent,
							const int width,
							const int height,
							const int peakId,
							std::vector< ProfilePanel::DataInfo >& data
						)
				 * @brief constructor
				 * @param parent parent window
				 * @param width window width
				 * @param height window height
				 * @param peakId peak ID
				 * @param data data information
				 */
				ChromatogramDataCanvas(
					wxWindow* parent,
					const int width,
					const int height,
					const int peakId,
					std::vector< ProfilePanel::DataInfo >& data
				);

				/**
				 * @fn virtual ~ChromatogramDataCanvas()
				 * @brief destructor
				 */
				virtual ~ChromatogramDataCanvas();

			protected:
				/** peak ID */
				int m_peakId;

				/** chromatograms */
				std::vector< kome::ident::DataChromatogram* > m_dataChroms;

				/** RT */
				double m_rt;

				/** draw peak flag */
				bool m_drawPeakFlg;

			public:
				/**
				 * @fn void setPeakId( const int peakId, std::vector< ProfilePanel::DataInfo >& data )
				 * @brief sets the peak ID
				 * @param[in] peakId peak ID
				 * @parma[in] data data information
				 */
				void setPeakId( const int peakId, std::vector< ProfilePanel::DataInfo >& data );

			public:
				/**
				 * @fn void setNormalized( const bool normalized )
				 * @brief sets normalized
				 * @param normalized normalized flag value
				 */
				void setNormalized( const bool normalized );

				/**
				 * @fn void setAligned( const bool aligned )
				 * @brief sets aligned
				 * @param aligned aligned flag value
				 */
				void setAligned( const bool aligned );

				/**
				 * @fn void setDrawingPeak( const bool drawing )
				 * @brief sets drawing peak flag
				 * @param drawing drawing peak flag value
				 */
				void setDrawingPeak( const bool drawing );

				/**
				 * @fn void updateChroms()
				 * @brief updates chromatograms
				 */
				void updateChroms();

			protected:
				/**
				 * @fn void deleteChroms()
				 * @brief delete chromatograms
				 */
				void deleteChroms();

			protected:
				/**
				 * @fn virtual void onDrawGraphFg( kome::window::DCGraphics& g )
				 * @brief This method is called when drawing graph foregorund (override method)
				 * @param g graphics
				 */
				virtual void onDrawGraphFg( kome::window::DCGraphics& g );

				/** 
				 * @fn virtual void onDrawChromatogramProfile(
							kome::window::DCGraphics& g,
							kome::objects::Chromatogram& chrom
						)
				 * @brief draws spectrum profile (override method)
				 * @param g graphics
				 * @param chrom chromatogram
				 */
				virtual void onDrawChromatogramProfile(
					kome::window::DCGraphics& g,
					kome::objects::Chromatogram& chrom
				);

			private:
				DECLARE_EVENT_TABLE()
			};
		}
	}
}


#endif		// __KOME_CHROMATOGRAM_DATA_CANVAS_H__
