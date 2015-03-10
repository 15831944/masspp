/**
 * @file SpectrumDataCanvas.h
 * @brief interfaces of SpectrumDataCanvas class
 *
 * @author S.Tanaka
 * @date 2008.09.29
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_SPECTRUM_DATA_CANVAS_H__
#define __KOME_SPECTRUM_DATA_CANVAS_H__


#include "ProfilePanel.h"


namespace kome {
	namespace matrix {
		namespace overlapping {

			/**
			 * @class SpectrumDataCanvas
			 * @brief matrix data profile canvas
			 */
			class SpectrumDataCanvas : public kome::window::OverlappingSpectraCanvas {
			public:
				/**
				 * @fn SpectrumDataCanvas(
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
				SpectrumDataCanvas(
					wxWindow* parent,
					const int width,
					const int height,
					const int peakId,
					std::vector< ProfilePanel::DataInfo >& data
				);

				/**
				 * @fn virtual ~SpectrumDataCanvas()
				 * @brief destructor
				 */
				virtual ~SpectrumDataCanvas();

			protected:
				/** peak ID */
				int m_peakId;

				/** spectra */
				std::vector< kome::ident::DataSpectrum* > m_dataSpectra;

				/** m/z */
				double m_mz;

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
				 * @fn void updateSpectra()
				 * @brief updates spectra
				 */
				void updateSpectra();

			protected:
				/**
				 * @fn void deleteSpectra()
				 * @brief delete spectra
				 */
				void deleteSpectra();

			protected:
				/**
				 * @fn virtual void onDrawGraphFg( kome::window::DCGraphics& g )
				 * @brief This method is called when drawing graph foregorund (override method)
				 * @param g graphics
				 */
				virtual void onDrawGraphFg( kome::window::DCGraphics& g );

				/** 
				 * @fn virtual void onDrawSpectrumProfile(
							kome::window::DCGraphics& g,
							kome::objects::Spectrum& spec
						)
				 * @brief draws spectrum profile (override method)
				 * @param g graphics
				 * @param spec spectrum
				 */
				virtual void onDrawSpectrumProfile(
					kome::window::DCGraphics& g,
					kome::objects::Spectrum& spec
				);

			private:
				DECLARE_EVENT_TABLE()
			};
		}
	}
}


#endif		// __KOME_SPECTRUM_DATA_CANVAS_H__
