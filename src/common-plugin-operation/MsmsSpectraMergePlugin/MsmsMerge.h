/**
 * @file MsmsMerge.h
 * @brief interfaces of MsmsMerge class
 *
 * @author S.Tanaka
 * @date 2013.01.16
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_OPERATION_MSMS_MERGE_H__
#define __KOME_OPERATION_MSMS_MERGE_H__


namespace kome {
	namespace operation {
		namespace msmsmerge {

			/**
			 * @class MsmsMerge
			 * @brief merged spectrum plug-in management class
			 */
			class MsmsMerge {
			protected:
				/**
				 * @fn MsmsMerge()
				 * @brief constructor
				 */
				MsmsMerge();

				/**
				 * @fn virtual ~MsmsMerge()
				 * @brief destructor
				 */
				virtual ~MsmsMerge();

			public:
				/**
				 * @fn bool openParametersDialog( double* rtTol, double* mzTol )
				 * @brief opens parameters dialog.
				 * @parma[in] rtTol the pointer to store the rt tolerance.
				 * @parma[in] mzTol the pointer to store the m/z tolerance.
				 * @return If true, the [OK] button is clicked otherwise the [Cancel] button is clicked.
				 */
				bool openParametersDialog( double* rtTol, double* mzTol );

				/**
				 * @fn void mergeSpectra( kome::objects::Sample* sample, const double rtTol, const double mzTol, kome::core::Progress& progress )
				 * @brief merges spectra
				 * @param[in] sample sample object
				 * @param[in] rtTol RT tolerance
				 * @param[in] mzTol m/z tolerance
				 * @paramn[in,out] progress the object to show the progress
				 */
				void mergeSpectra( kome::objects::Sample* sample, const double rtTol, const double mzTol, kome::core::Progress& progress );

			protected:
				/**
				 * @fn static bool lessSpectrum( kome::objects::Spectrum* spec0, kome::objects::Spectrum* spec1 )
				 * @brief compare spectra to sort
				 * @param[in] spec0 spectrum to compare
				 * @param[in] spec1 spectrum to be compared
				 */
				static bool lessSpectrum( kome::objects::Spectrum* spec0, kome::objects::Spectrum* spec1 );

			public:
				/**
				 * @fn static MsmsMerge& getInstance()
				 * @brief get MsmsMerge object. (This is the only object.)
				 * @return MsmsMerge object.
				 */
				static MsmsMerge& getInstance();
			};
		}
	}
}


#endif		// __KOME_OPERATION_MSMS_MERGE_H__
