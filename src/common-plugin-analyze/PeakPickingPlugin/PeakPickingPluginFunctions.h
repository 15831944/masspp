/**
 * @file PeakPickingPluginFunctions.h
 * @brief interfaces of PeakPicking plug-in functions
 *
 * @author S.Tanaka
 * @date 2006.10.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_LABELING_PEAK_PICKING_PLUGIN_FUNCTIONS_H__
#define __KOME_LABELING_PEAK_PICKING_PLUGIN_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant executeSpectrumLabeling( kome::objects::Parameters* params )
 * @brief executes spectrum labeling
 * @param params parameters object
 * @return If it finishes successfully, this function returns true. (boolean)
 */
kome::objects::Variant executeSpectrumLabeling( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant executeChromatogramLabeling( kome::objects::Parameters* params )
 * @brief executes spectrum labeling
 * @return If it finishes successfully, this function returns true. (boolean)
 */
kome::objects::Variant executeChromatogramLabeling( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant clearLabels( kome::objects::Parameters* params )
 * @brief clear labels
 * @return If it finishes successfully, this function returns true. (boolean)
 */
kome::objects::Variant clearLabels( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant detectSpectrumPeaksOnOpen( kome::objects::Parameters* params )
 * @brief detects spectrum peaks on open
 * @param params parameters object (This object has spectrum information.)
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant detectSpectrumPeaksOnOpen( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant detectChromatogramPeaksOnOpen( kome::objects::Parameters* params )
 * @brief detects chromatogram peaks on open
 * @param params parameters object (This object has chromatogram information.)
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant detectChromatogramPeaksOnOpen( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant pickPeaks( kome::objects::Parameters* params )
 * @brief picks peaks
 * @param params parameters object
 * @return return value (boolean)
 */
kome::objects::Variant pickPeaks( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onChromatogramProcessing( kome::objects::Parameters* params )
 * @brief detect chromatogram peaks
 * @param params parameters object
 * @return return value (boolean)
 */
kome::objects::Variant onChromatogramProcessing( kome::objects::Parameters* params );	// @date 2011.01.25 <Add> M.Izumi

// merge from Peaks2dPluginFunctions.h

/** 
 * @fn kome::objects::Variant initPeakListView( kome::objects::Parameters* )
 * @brief initializes peak list view
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant initPeakListView( kome::objects::Parameters* );

/**
 * @fnkome::objects::Variant changeActiveObject( kome::objects::Parameters* params )
 * @brief changes active object
 * @param parameters object
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant changeActiveObject( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant updatePeaksView( kome::objects::Parameters* params )
 * @brief updates peaks view
 * @param params parameters object
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant updatePeaksView( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant exitGui( kome::objects::Parameters* )
 * @brief exists GUI
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant exitGui( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant executeGroup2dLabeling( kome::objects::Parameters* params )
 * @brief executes 2D labeling
 * @param params paraemters object
 * @return return value (If true, it finished successfully.)
 */ 
kome::objects::Variant executeGroup2dLabeling( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant isVisiblePeaksView( kome::objects::Parameters* )
 * @brief checks 2D Peaks View visibility
 * @return If true, the 2D Peaks View is visible. (boolean)
 */
kome::objects::Variant isVisiblePeaksView( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant peakDetection( kome::objects::Parameters* params)
 * @brief peak detection
 * @param params parameters object
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant peakDetection( kome::objects::Parameters* params);

/**
 * @fn kome::objects::Variant save2dPeaksFile( kome::objects::Parameters* params )
 * @brief saves peaks file
 * @param params parameters object
 * @return If true, it succeeded to save file.
 */
kome::objects::Variant save2dPeaksFile( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant getSpecPeakDetectOperation( kome::objects::Parameters* )
 * @brief gets peak detection  operation
 * @return return value PeakDetectOperation object
 */
kome::objects::Variant getSpecPeakDetectOperation( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant getChromPeakDetectOperation( kome::objects::Parameters* )
 * @brief gets peak detection  operation
 * @return return value PeakDetectOperation object
 */
kome::objects::Variant getChromPeakDetectOperation( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant get2DPeakDetectOperation( kome::objects::Parameters* )
 * @brief gets peak detection  operation
 * @return return value PeakDetectOperation object
 */
kome::objects::Variant get2DPeakDetectOperation( kome::objects::Parameters* );

 /**
  * @fn kome::objects::Variant detectSpectrumPeaks( kome::objects::Parameters* params )
  * @brief detects spectrum peaks
  * @param[in] params parameters object
  * @return This value has no meanings.
  */
 kome::objects::Variant detectSpectrumPeaks( kome::objects::Parameters* params );
 
 /**
  * @fn kome::objects::Variant detectChromatogramPeaks( kome::objects::Parameters* params )
  * @brief detects chromatogram peaks
  * @param[in] params parameters object
  * @return This value has no meanings.
  */
 kome::objects::Variant detectChromatogramPeaks( kome::objects::Parameters* params );
 
 /**
  * @fn kome::objects::Variant detect2DPeaks( kome::objects::Parameters* params )
  * @brief detects 2D peaks
  * @param[in] params parameters object
  * @return This value has no meanings.
  */
 kome::objects::Variant detect2DPeaks( kome::objects::Parameters* params );
 
 /**
 * @fn kome::objects::Variant draw2dPeaks( kome::objects::Parameters* params )
 * @brief draws 2D peaks
 * @param params parameters object
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant draw2dPeaks( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant draw2dClusters( kome::objects::Parameters* params )
 * @brief draws 2D clusters
 * @param params parameters object
 * @return return value (boolean)
 */
kome::objects::Variant draw2dPeaks( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant SpecPeakFilterThreshold( kome::objects::Parameters* params )
 * @brief spec peak filter Threshold
 * @param params parameters object
 * @return return value (boolean)
 */
 kome::objects::Variant SpecPeakFilterThreshold( kome::objects::Parameters* params );
 
/**
 * @fn kome::objects::Variant SpecPeakFilterMzRange( kome::objects::Parameters* params )
 * @brief spec peak filter m/z range
 * @param params parameters object
 * @return return value (boolean)
 */
 kome::objects::Variant SpecPeakFilterMzRange( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant SpecPeakFilterElimination( kome::objects::Parameters* params )
 * @brief spec peka filter Elimination
 * @param params parameters object
 * @return return value (boolean)
 */
 kome::objects::Variant SpecPeakFilterElimination( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant SpecPeakFilterElimination( kome::objects::Parameters* params )
 * @brief spec peka filter Elimination
 * @param params parameters object
 * @return return value (boolean)
 */
 kome::objects::Variant SpecPeakFilterSn( kome::objects::Parameters* params );    // @date 2013.07.02 <Add> Y.Fujita
 
 /**
  * @fn kome::objects::Variant SpecPeakFilterDifference( kome::objects::Parameters* params )
  * @brief spec peak filter difference
  * @param params parameters object
  * @return return value (boolean)
  */
 kome::objects::Variant SpecPeakFilterDifference( kome::objects::Parameters* params );
 
 /**
 * @fn kome::objects::Variant normalizPeakDetectSpectra( kome::objects::Parameters* params )
 * @brief normalization spectra peak detection
 * @param params parameters object
 * @return return value (boolean)
 */
 kome::objects::Variant normalizPeakDetectSpectra( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant normalizPeakDetect2d( kome::objects::Parameters* params )
 * @brief normalization 2d peak detect
 * @param params parameters object
 * @return return value (boolean)
 */ 
 kome::objects::Variant normalizPeakDetect2d( kome::objects::Parameters* params );


#ifdef __unix
	}
#endif  // __unix


#endif// __KOME_LABELING_PEAK_PICKING_PLUGIN_FUNCTIONS_H__
