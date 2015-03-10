/**
 * @file PeaksIOFunctions.h
 * @brief interfaces of PeaksIO plug-in functions
 *
 * @author S.Tanaka
 * @date 2011.02.14
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_PEAKS_PEAKS_IO_FUNCTIONS_H__
#define __KOME_IO_PEAKS_PEAKS_IO_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant exportPeaks( kome::objects::Parameters* params )
 * @brief export peaks
 * @param params parameters object
 * @return If true, it succeeded to export peaks
 */
kome::objects::Variant exportPeaks( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant importPeaks( kome::objects::Parameters* params )
 * @brief imports peaks
 * @param params parameters object
 * @return If true, it succeeded to import peaks
 */
kome::objects::Variant importPeaks( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant importOverlappingPeaks( kome::objects::Parameters* params )
 * @brief imports overlapping peaks
 * @param params parameters object
 * @return If true, it succeeded to import overlapping peaks.
 */
kome::objects::Variant importOverlappingPeaks( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant deleteOverlap( kome::objects::Parameters* params )
 * @brief deletes overlapping peaks
 * @param params parameters object
 * @return If true, it succeeded to delete overlapping peaks.
 */
kome::objects::Variant deleteOverlap( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant drawOverlappingPeaks( kome::objects::Parameters* params )
 * @brief draws overlapping peaks
 * @param params parameters
 * @return This value has no meanings
 */
kome::objects::Variant drawOverlappingPeaks( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant menuImportPeaks( kome::objects::Parameters* params )
 * @brief display select menu for import
 * @param params parameters
 * @return This value has no meanings
 */
kome::objects::Variant menuImportPeaks( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant getExportPeakOperation( kome::objects::Parameters* )
 * @brief gets export peak  operation
 * @return return value ExportPeakOperation object
 */
kome::objects::Variant getExportPeakOperation( kome::objects::Parameters* );

#ifdef __unix
	}
#endif  // __unix


#endif // __KOME_IO_PEAKS_PEAKS_IO_FUNCTIONS_H__
