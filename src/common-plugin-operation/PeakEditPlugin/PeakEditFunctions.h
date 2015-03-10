/**
 * @file PeakEditFunctions.h
 * @brief interfaces of PeakEdit plug-in functions
 *
 * @author S.Tanaka
 * @date 2008.12.17
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_PEAKS_PEAK_EDIT_FUNCTIONS_H__
#define __KOME_PEAKS_PEAK_EDIT_FUNCTIONS_H__

#ifdef _DEBUG
//	#define _DEBUG_1
//	#define _DEBUG_JPG
#endif

#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant initLabelMode( kome::objects::Parameters* )
 * @brief initializes label mode
 * @return This value has no meanings.
 */
kome::objects::Variant initLabelMode( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant finalizeLabelMode( kome::objects::Parameters* )
 * @brief finalizes label mode
 * @return This value has no meanings.
 */
kome::objects::Variant initLabelMode( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant setLabelAddMode( kome::objects::Parameters* )
 * @brief set label add mode
 * @return This function always returns true. (boolean value)
 */
kome::objects::Variant setLabelAddMode( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant setLabelEditMode( kome::objects::Parameters* )
 * @brief set label edit mode
 * @param params parameters object
 * @return This function always returns true. (boolean value)
 */
kome::objects::Variant setLabelEditMode( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant deleteSelectedPeak( kome::objects::Parameters* params )
 * @brief deletes selected peak
 * @param params parameters object
 * @If true, deleting selected peak is succeeded. (boolean)
 */
kome::objects::Variant deleteSelectedPeak( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant canDeletePeak( kome::objects::Parameters* params )
 * @brief judges whether deleting peak is available
 * @param params parameters objedct
 * @return If true, deleting peak is available.
 */
kome::objects::Variant canDeletePeak( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant drawSpecPeakPos( kome::objects::Parameters* params )
 * @brief draws spectrum peak position
 * @param params parameters
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant drawSpecPeakPos( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant drawChromPeakPos( kome::objects::Parameters* params )
 * @brief draws chromatogram peak position
 * @param params parameters
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant drawChromPeakPos( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant drawSpecPeakShape( kome::objects::Parameters* params )
 * @brief draws spectrum peak shape
 * @param params parameters
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant drawSpecPeakShape( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant drawChromPeakShape( kome::objects::Parameters* params )
 * @brief draws chromatogram peak shape
 * @param params parameters
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant drawChromPeakShape( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant startToAddSpecLabel( kome::objects::Parameters* params )
 * @brief starts to add spectrum label
 * @param params parameters
 * @return refresh flag value (boolean)
 */
kome::objects::Variant startToAddSpecLabel( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant startToEditSpecLabel( kome::objects::Parameters* params )
 * @brief starts to edit spectrum label
 * @param params parameters
 * @return refresh flag value (boolean)
 */
kome::objects::Variant startToEditSpecLabel( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant moveCursorInAddingSpecLabel( kome::objects::Parameters* params )
 * @brief moves cursor in adding spectrum label
 * @param params parameters
 * @return refresh flag value (boolean)
 */
kome::objects::Variant moveCursorInAddingSpecLabel( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant moveCursorInEditingSpecLabel( kome::objects::Parameters* params )
 * @brief moves cursor in editing spectrum label
 * @param params parameters
 * @return refresh flag value (boolean)
 */
kome::objects::Variant moveCursorInEditingSpecLabel( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant finishToAddSpecLabel( kome::objects::Parameters* params )
 * @brief finishes to add spectrum label
 * @param params parametres
 * @return refresh flag value (boolean)
 */
kome::objects::Variant finishToAddSpecLabel( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant finishToEditSpecLabel( kome::objects::Parameters* params )
 * @brief finishes to edit spectrum label
 * @param params parametres
 * @return refresh flag value (boolean)
 */
kome::objects::Variant finishToEditSpecLabel( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant startToAddChromLabel( kome::objects::Parameters* params )
 * @brief starts to add chromatogram label
 * @param params parameters
 * @return refresh flag value (boolean)
 */
kome::objects::Variant startToAddChromLabel( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant startToEditChromLabel( kome::objects::Parameters* params )
 * @brief starts to edit chromatogram label
 * @param params parameters
 * @return refresh flag value (boolean)
 */
kome::objects::Variant startToEditChromLabel( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant moveCursorInAddingChromLabel( kome::objects::Parameters* params )
 * @brief moves cursor in adding chromatogram label
 * @param params parameters
 * @return refresh flag value (boolean)
 */
kome::objects::Variant moveCursorInAddingChromLabel( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant moveCursorInEditingChromLabel( kome::objects::Parameters* params )
 * @brief moves cursor in editing chromatogram label
 * @param params parameters
 * @return refresh flag value (boolean)
 */
kome::objects::Variant moveCursorInEditingChromLabel( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant finishToAddChromLabel( kome::objects::Parameters* params )
 * @brief finishes to add chromatogram label
 * @param params parameters
 * @return refresh flag value (boolean)
 */
kome::objects::Variant finishToAddChromLabel( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant finishToEditChromLabel( kome::objects::Parameters* params )
 * @brief finishes to edit chromatogram label
 * @param params parameters
 * @return refresh flag value (boolean)
 */
kome::objects::Variant finishToEditChromLabel( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant commitSpectrumPeaks( kome::objects::Parameters* params )
 * @brief commits spectrum peaks
 * @param params parameters object
 * @return This value has no meanings.
 */
kome::objects::Variant commitSpectrumPeaks( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant commitChromatogramPeaks( kome::objects::Parameters* params )
 * @brief commits chromatogram peaks
 * @param params parameters object
 * @return This value has no meanings.
 */
kome::objects::Variant commitChromatogramPeaks( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant rollbackSpectrumPeaks( kome::objects::Parameters* params )
 * @brief rollbacks spectrum peaks
 * @param params parameters object
 * @return This value has no meanings.
 */
kome::objects::Variant rollbackSpectrumPeaks( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant rollbackChromatogramPeaks( kome::objects::Parameters* params )
 * @brief rollbacks chromatogram peaks
 * @param params parameters object
 * @return This value has no meanings.
 */
kome::objects::Variant rollbackChromatogramPeaks( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant moveMouseCursorInEditingSpec( kome::objects::Parameters* params );
 * @brief moves cursor in editing spectrum label
 * @param params parameters
 * @return refresh flag value (boolean)
 */
kome::objects::Variant moveMouseCursorInEditingSpec( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onSpecButtonDown( kome::objects::Parameters* params )
 * @brief push button in editing spectrum label
 * @param params parameters
 * @return refresh flag value (boolean)
 */
kome::objects::Variant onSpecButtonDown( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onSpecButtonUp( kome::objects::Parameters* params )
 * @brief unpush button in editing spectrum label
 * @param params parameters
 * @return refresh flag value (boolean)
 */
kome::objects::Variant onSpecButtonUp( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant moveMouseCursorInEditingChrom( kome::objects::Parameters* params );
 * @brief moves cursor in editing chromatogram label
 * @param params parameters
 * @return refresh flag value (boolean)
 */
kome::objects::Variant moveMouseCursorInEditingChrom( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant canExitPeakEditMode( kome::objects::Parameters* params )
 * @brief check whether can exit the Quantitation mode
 * @param params parameters object (This object has no information.)
 * @return This function returns true or false. (boolean)
 */
kome::objects::Variant canExitPeakEditMode( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant setPeakDrawMode( kome::objects::Parameters* params )
 * @brief sets peak draw mode on batch
 * @return This function always returns true. (boolean)
*/
kome::objects::Variant setPeakDrawMode( kome::objects::Parameters* params );


/**
 * @fn bool chkOverlapping( kome::objects::Parameters* params )
 * @brief check overlapping window
 * @param params parameters object (This object has no information.)
 * @return This function returns true or false. (boolean)
 */
bool chkOverlapping( kome::objects::Parameters* params );

#ifdef __unix
	}
#endif  // __unix



#endif		// __KOME_PEAKS_PEAK_EDIT_FUNCTIONS_H__
