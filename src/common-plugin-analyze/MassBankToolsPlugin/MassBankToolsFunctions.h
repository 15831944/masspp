/**
 * @file MassBankToolsFunctions.h
 * @brief interfaces of MassBankTools plug-in functions
 *
 * @author S.Tanaka
 * @date 2009.06.24
 * 
 * @modify M.Fukuda / Eisai Co 
 * @date 2011.08.30
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_MASBANK_TOOLS_FUNCTION_H__
#define __KOME_MASBANK_TOOLS_FUNCTION_H__

#ifdef __unix
	extern "C" {
#endif  // __unix

/**
 * @fn kome::objects::Variant initGui( kome::objects::Parameters* )
 * @brief initialize Search Result Pane
 * @return return value (This value has no meanings.)
 */
// Not USED
//kome::objects::Variant initGui( kome::objects::Parameters* params );


/**
 * @fn kome::objects::Variant exitGui( kome::objects::Parameters* )
 * @brief releases Search Result Pane
 * @return This value has no meanings.
 */
// Not USED
//kome::objects::Variant exitGui( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant callCreateSpectrumRecordDialog( kome::objects::Parameters* params )
 * @brief opens Create Spectrum Record Dialog
 * @param params parameters object
 * @return If false, the operation is failed.
 */
kome::objects::Variant callCreateSpectrumRecordDialog( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant callSpectrumSearchDialog( kome::objects::Parameters* params )
 * @brief opens Spectrum Search Dialog
 * @param params parameters object
 * @return If false, the operation is failed.
 */
kome::objects::Variant callSpectrumSearchDialog( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant callBatchSearchDialog( kome::objects::Parameters* params )
 * @brief opens Batch Search Dialog
 * @param params parameters object
 * @return If false, the operation is failed.
 */
kome::objects::Variant callBatchSearchDialog( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant callPeakSearchDialog( kome::objects::Parameters* params )
 * @brief opens Peak Search Dialog
 * @param params parameters object
 * @return If false, the operation is failed.
 */
kome::objects::Variant callPeakSearchDialog( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant callPeakDifferenceSearchDialog( kome::objects::Parameters* params )
 * @brief opens Peak Difference Search Dialog
 * @param params parameters object
 * @return If false, the operation is failed.
 */
kome::objects::Variant callPeakDifferenceSearchDialog( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant drawMassBankRecordPeaks( kome::objects::Parameters* params )
 * @brief draws MassBank record peaks
 * @param params parameters object
 * @return This value has no meanings.
 */
kome::objects::Variant drawMassBankRecordPeaks( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant toggleSearchResultsPane( kome::objects::Parameters* )
 * @brief toggles the visibility of the Search Results Pane
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant toggleSearchResultsPane( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant ischeckSearchResultsPaneOpened( kome::objects::Parameters* )
 * @brief toggles the visibility of the Search Results Pane
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant ischeckSearchResultsPaneOpened( kome::objects::Parameters* );

kome::objects::Variant batchCommandCalled(kome::objects::Parameters*);

// Getter.... Inner Values. InterumentLists, LicenseArray.So.on.
kome::objects::Variant getVendorArrayData(kome::objects::Parameters* params);
kome::objects::Variant getInstrumentArrayData(kome::objects::Parameters* params);

kome::objects::Variant getLicenseArrayData(kome::objects::Parameters* params);

kome::objects::Variant getGettedInstrumentTypeArrayData(kome::objects::Parameters* params);

kome::objects::Variant getGettedInstrumentTypeArrayDataWithFree(kome::objects::Parameters* params);

kome::objects::Variant getAppendedValuesArrayData(kome::objects::Parameters* params);

std::vector<kome::plugin::SettingsValue::EnumItem>* onGetEnumItems(
	kome::objects::Parameters* params);


kome::objects::Variant initialiseSearchEngine(kome::objects::Parameters* params);

// check modifications
kome::objects::Variant checkPrefixStatus(kome::objects::Parameters* params);

// local function
kome::objects::Variant setBoolVal(const bool fg);
// Get Avctive Spectrum ( local Function )
kome::objects::Spectrum* inGetActiveSpectrum (kome::objects::Parameters* params);
// Get Avctive DataGroupNode ( local Function )
kome::objects::DataGroupNode* inGetActiveDataGroupNode (kome::objects::Parameters* params);

#ifdef __unix
	}
#endif  // __unix

#endif// __KOME_MASBANK_TOOLS_FUNCTION_H__
