/**
 * @file ChromatogramOperationFunctions.h
 * @brief interfaces of ChromatogramOperation plug-in functions
 *
 * @author S.Tanaka
 * @date 2007.04.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_OPERATION_CHROMATOGRAM_FUNCTIONS_H__
#define __KOME_OPERATION_CHROMATOGRAM_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant setChromatogramFilter( kome::objects::Parameters* params )
 * @brief sets chromatogram filter on open
 * @param params parameters object (This object has chromatogram information.)
 * @return return value (This function always returns true.)
 */
kome::objects::Variant setChromatogramFilter( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant setFilterAll( kome::objects::Parameters* )
 * @brief sets chromatogram filter "all"
 * @return This value is always true. (boolean)
 */
kome::objects::Variant setFilterAll( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant setFilterMs( kome::objects::Parameters* )
 * @brief sets chromatogram filter "ms"
 * @return This value is always true. (boolean)
 */
kome::objects::Variant setFilterMs( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant setFilterMsms( kome::objects::Parameters* )
 * @brief sets chromatogram filter "msms"
 * @return This value is always true. (boolean)
 */
kome::objects::Variant setFilterMsms( kome::objects::Parameters* );

//2012/02/24 add by t.okuno -> This plugin distributes 'MC' and 'BPC'.
/**
 * @fn kome::objects::Variant showGenerateChromatogram( kome::objects::Parameters* params )
 * @brief shows Generate Chromatogram Dialog
 * @param params parameters information (This object has spectrum and m/z range informations.)
 * @return This value has no meanings.
 */
kome::objects::Variant showGenerateChromatogram( kome::objects::Parameters* params );
//2012/02/24 add by t.okuno <- This plugin distributes 'MC' and 'BPC'.

/**
 * @fn kome::objects::Variant openGenerateChromatogramDlg( kome::objects::Parameters* params )
 * @brief open generate chromatogram dialog ( メニューから表示 ）
 */
kome::objects::Variant openGenerateChromatogramDlg( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant getGenelateChrom( kome::objects::Parameters* params )
 * @brief get Genelate chrom 
 * @return operation
 */
kome::objects::Variant getGenelateChrom( kome::objects::Parameters* params );

#ifdef __unix
	}
#endif  // __unix



#endif		// __KOME_OPERATION_CHROMATOGRAM_FUNCTIONS_H__
