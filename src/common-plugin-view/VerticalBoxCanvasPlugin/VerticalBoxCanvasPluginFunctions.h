/**
 * @file VerticalBoxCanvasPluginFunctions.h
 * @brief VerticalBoxCanvasPluginFunctions.cppの ヘッダ
 * @author OKADA, H.
 * @date 2010.11
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 * 
 */

#ifndef _VERTICALBOXCANVASPLUGINFUNCTIONS_H_
#define _VERTICALBOXCANVASPLUGINFUNCTIONS_H_

#include "VerticalBoxChromatogramCanvas.h"
#include "VerticalBoxSpectrumCanvas.h"

#define VERTICAL_BOX_MARGIN_TOP	15	/* @date 2010/12/14 <add> OKADA */

/**
 * @fn kome::objects::Variant getSpectrumVerticalBox( kome::objects::Parameters* params )
 * @brief create Spectrum canvas
 * @param[in] params parameter information  
 * @return true
 */
kome::objects::Variant getSpectrumVerticalBox( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant getChromatogramVerticalBox( kome::objects::Parameters* params )
 * @brief create chromatgram canvas
 * @param[in] params parameter information  
 * @return true
 */
kome::objects::Variant getChromatogramVerticalBox( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant setSpecAndChromStatus( kome::objects::Parameters* )
 * @brief set Spectrum and Chromatogram status
 * @param[in] kome::objects::Parameters* 未使用
 * @return true
 */
kome::objects::Variant setSpecAndChromStatus( kome::objects::Parameters* );


// >>>>>> support delete function	@Date:2013/05/23	<Add>	A.Ozaki

/**
 * @fn kome::objects::Variant initGui( kome::objects::Parameters* )
 * @brief initialize view
 * @return return value (This value has no meaning.)
 */
kome::objects::Variant	initGui( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant changeActiveObject( kome::objects::Parameters* params )
 * @brief This function is called when active object is changed
 * @param params parameters object
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant changeActiveObject( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant exitGui( kome::objects::Parameters* )
 * @brief releases details view
 * @return This value has no meaning.
 */
kome::objects::Variant exitGui( kome::objects::Parameters* );

/**
 * @fnkome::objects::Variant toggleVerticalBoxDialog( kome::objects::Parameters* )
 * @brief toggles the visibility of the vertical box dialog
 * @return If true, It succeeded to toggle the the visibility of the vertical box dialog (boolean)
 */
kome::objects::Variant toggleVerticalBoxDialog( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant checkVerticalBoxDialog( kome::objects::Parameters* )
 * @brief judges whether the vertical box dialog is shown
 * @return If true, the vertical box dialog is shown
 */
kome::objects::Variant checkVerticalBoxDialog( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant SelectCanvasOfSpectrum( kome::objects::Parameters* pParams )
 * @brief notice select spectrum on canvas
 * @params pParams parameters object
 * @return This value has no meaning.
 */
kome::objects::Variant	SelectCanvasOfSpectrum( kome::objects::Parameters* pParams );

/**
 * @fn kome::objects::Variant SelectCanvasOfChromatogram( kome::objects::Parameters* pParams )
 * @brief notice select chromatogram on canvas
 * @params pParams parameters object
 * @return This value has no meaning.
 */
kome::objects::Variant	SelectCanvasOfChromatogram( kome::objects::Parameters* pParams );

//
// <<<<<< support delete function	@Date:2013/05/23	<Add>	A.Ozaki

kome::objects::Variant getSpecStackedCanvasOperation( kome::objects::Parameters* params );
kome::objects::Variant getChromStackedCanvasOperation( kome::objects::Parameters* params );



#endif
