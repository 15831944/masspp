/**
 * @file PickedProfileDialog.h
 * @brief interfaces of PickedProfileDialog class
 *
 * @author S.Tanaka
 * @date 2009.03.17
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_MATRIX_PICKED_PROFILE_DIALOG_H__
#define __KOME_MATRIX_PICKED_PROFILE_DIALOG_H__


namespace kome {
	namespace matrix {
		namespace overlapping {

			/**
			 * @class PickedProfileDialog
			 * @brief picked data profile viewing dialog
			 */
			class PickedProfileDialog : public kome::window::StandardDialog {
			public:
				/**
				 * @fn PickedProfileDialog(
							wxWindow* parent,
							kome::ident::DataSpectrum* spec,
							const double minX,
							const double maxX
						)
				 * @brief constructor
				 * @param parent parent window
				 * @param spec data spectrum
				 * @param minX minimum X to display profile
				 * @param maxX maximum X to display profile
				 */
				PickedProfileDialog(
					wxWindow* parent,
					kome::ident::DataSpectrum* spec,
					const double minX,
					const double maxX
				);

				/**
				 * @fn PickedProfileDialog(
							wxWindow* parent,
							kome::ident::DataChromatogram* chrom,
							const double minX,
							const double maxX
						)
				 * @brief constructor
				 * @param parent parent window
				 * @param chrom data chromatogram
				 */
				PickedProfileDialog(
					wxWindow* parent,
					kome::ident::DataChromatogram* chrom,
					const double minX,
					const double maxX
				);

				/**
				 * @fn virtual ~PickedProfileDialog()
				 * @brief destructor
				 */
				virtual ~PickedProfileDialog();

			protected:
				/** spectrum */
				kome::ident::DataSpectrum* m_spec;

				/** chromatogram */
				kome::ident::DataChromatogram* m_chrom;

				/** peaks */
				kome::objects::Peaks* m_peaks;

				/** canvas */
				kome::window::Canvas* m_canvas;

				/** minX */
				const double m_minX;

				/** maxX */
				const double m_maxX;

			protected:
				/**
				 * @fn void init()
				 * @brief initializes data
				 */
				void init();

			protected:
				/**
				 * @fn void updatePeak()
				 * @brief update peak
				 */
				void updatePeak();

			protected:
				/**
				 * @fn virtual wxSizer* createMainSizer()
				 * @brief creates main sizer
				 * @return main sizer
				 */
				virtual wxSizer* createMainSizer();

			protected:
				/**
				 * @fn virtual bool onOK()
				 * @brief This method is called when the "OK" button is clicked and values in the window is valid.
				 * @return If true, this window is closed.
				 */
				virtual bool onOK();

				/**
				 * @fn virtual bool onCancel()
				 * @brief This method is called when the "Cancel" button is clicked. (override method)
				 * @return If true, this dialog is closed.
				 */
				virtual bool onCancel();

			private:
				DECLARE_EVENT_TABLE();
			};
		}
	}
}

#endif	// __KOME_MATRIX_PICKED_PROFILE_DIALOG_H__
