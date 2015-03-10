/**
 * @file MergedSpectrumManager.h
 * @brief interfaces of MergedSpectrumManager class
 *
 * @author S.Tanaka
 * @date 2007.03.06
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_OPERATION_MERGED_SPECTRUM_MANAGER_H__
#define __KOME_OPERATION_MERGED_SPECTRUM_MANAGER_H__


#include <map>
#include <set>

//2012/02/28 add by t.okuno -> This plugin displays a range choice screen.
#include "AveragedSpectrumDialog.h"

using namespace kome::operation::sel_range;
//2012/02/28 add by t.okuno <- This plugin displays a range choice screen.

namespace kome {
	namespace operation {
		namespace merged {

			class MergedSpectrum;

			/**
			 * @class MergedSpectrumManager
			 * @brief merged spectrum plug-in management class
			 */
			class MergedSpectrumManager : public kome::objects::DefaultDataManager {
			protected:
				/**
				 * @fn MergedSpectrumManager()
				 * @brief constructor
				 */
				MergedSpectrumManager();

				/**
				 * @fn virtual ~MergedSpectrumManager()
				 * @brief destructor
				 */
				virtual ~MergedSpectrumManager();

			protected:
				/** merged spectra */
				std::set< MergedSpectrum* > m_spectraSet;

				/** merged spectra */
				std::vector< MergedSpectrum* > m_mspectra;

				kome::objects::Spectrum* m_spec;

			public:
				void setSpec( kome::objects::Spectrum* spec ){ m_spec = spec; }
				kome::objects::Spectrum* getSpec(){ return m_spec; }
						
			public:
				/**
				 * @fn void addMergedSpectrum( MergedSpectrum* spec )
				 * @brief adds merged spectrum
				 * @param spec merged spectrum
				 */
				void addMergedSpectrum( MergedSpectrum* spec );

				/**
				 * @fn void removeMergedSpectrum( MergedSpectrum* spec )
				 * @brief removes merged spectrum
				 * @param spec merged spectrum
				 */
				void removeMergedSpectrum( MergedSpectrum* spec );

			public:
				/**
				 * @fn bool openMergedSpectrum(
						kome::objects::Chromatogram* chrom,
						const double startRt,
						const double endRt,
					)
				 * @brief opens merged spectrum
				 * @param chrom chromatogram object
				 * @param startRt start RT
				 * @param endRt end Rt
				 * @return 
				 */
				bool openMergedSpectrum(
					kome::objects::Chromatogram* chrom,
					const double startRt,
					const double endRt
				);

				/**
				 * @fn bool mergeAllSpectra()
				 * @brief merges all spectra
				 * @return If it succeeded to merge spectra.
				 */
				bool mergeAllSpectra();

				/**
				 * @fn bool mergeMs2Spectra()
				 * @brief merges MS/MS spectra
				 */
				bool mergeMs2Spectra();

				//2012/02/28 add by t.okuno -> This plugin displays a range choice screen.
				/**
				 * @fn void createAveragedSpectrum( kome::objects::Chromatogram* chrom,
													const double startRT,
													const double endRT )
				 * @brief creates Averaged Spectrum dialog
				 * @param chrom source chromatogram
				 * @param startRT start RT
				 * @param endRT end RT
				 */
				void createAveragedSpectrum( kome::objects::Chromatogram* chrom,
											 const double startRT,
											 const double endRT );
								
				//2012/02/28 add by t.okuno <- This plugin displays a range choice screen.

			protected:
				/**
				 * @fn virtual void onCloseSample( kome::objects::Sample* sample, const bool deleting )
				 * @brief This method is called when a sample is closed. (override method)
				 * @param sample sample object to be closed
				 * @param[in] deleting If true, the specified object is being deleted now.
				 */
				virtual void onCloseSample( kome::objects::Sample* sample, const bool deleting );

				/**
				 * @fn virtual void onCloseSpectrum( kome::objects::Spectrum* spec, const bool deleting )
				 * @brief This method is called when a spectrum is closed. (override method)
				 * @param spec spectrum object to be opened
				 * @param[in] deleting If true, the specified object is being deleted now.
				 */
				virtual void onCloseSpectrum( kome::objects::Spectrum* spec, const bool deleting );
								
			public:
				/**
				 * @fn static MergedSpectrumManager& getInstance()
				 * @brief get merged spectrum plug-in management object
				 * @return merged spectrum plug-in management object (This is the only object)
				 */
				static MergedSpectrumManager& getInstance();
			};
		}
	}
}


#endif		// __KOME_OPERATION_MERGED_SPECTRUM_MANAGER_H__
