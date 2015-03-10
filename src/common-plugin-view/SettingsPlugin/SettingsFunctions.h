/**
 * @file SettingsFunctions.h
 * @brief interfaces of settings plug-in functions
 *
 * @author S.Tanaka
 * @date 2007.02.02
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_VIEW_SETTINGS_FUNCTIONS_H__
#define __KOME_VIEW_SETTINGS_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant showParametersDialog( kome::objects::Parameters* )
 * @brief shows settings dialog
 * @return If true, it succeeded to open settings dialog. (boolean)
 */
kome::objects::Variant showParametersDialog( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant showSettingsDialog( kome::objects::Parameters* )
 * @brief shows settings dialog
 * @return If true, it succeeded to open settings dialog. (boolean)
 */
kome::objects::Variant showSettingsDialog( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant setParamsFile( kome::objects::Parameters* params )
 * @brief sets parameters file
 * @param params parameters
 * @return If true, It succeeded to set parameters file.
 */
kome::objects::Variant setParamsFile( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant updateParamsFile( kome::objects::Parameters* )
 * @brief updates parameters file
 * @return This value has no meanings.
 */
kome::objects::Variant updateParamsFile( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant saveParamsFile( kome::objects::Parameters* )
 * @brief save parameters file
 * @return This value has no meanings.
 */
kome::objects::Variant saveParamsFile( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant showSettingsDialogFromContextMenu( PropertiesPageType pt );
 * @brief shows settings dialog from popup menu
 * @return If true, it succeeded to open settings dialog. (boolean)
 */
kome::objects::Variant showSettingsDialogFromContextMenu( PropertiesPageType pt );

/**
 * @fn kome::objects::Variant showSettingsDialogFromContextMenu( PropertiesPageType pt );
 * @brief shows settings dialog from popup menu
 * @return If true, it succeeded to open settings dialog. (boolean)
 */
kome::objects::Variant showSettingsDialogFromContextMenu( PropertiesPageType pt );

/**
 * @fn kome::objects::Variant showSettingsDialogFromContextMenuChromView( kome::objects::Parameters* );
 * @brief shows settings dialog from popup menu
 * @return If true, it succeeded to open settings dialog. (boolean)
 */
kome::objects::Variant showSettingsDialogFromContextMenuChromView( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant showSettingsDialogFromContextMenuSpecView( kome::objects::Parameters* );
 * @brief shows settings dialog from popup menu
 * @return If true, it succeeded to open settings dialog. (boolean)
 */
kome::objects::Variant showSettingsDialogFromContextMenuSpecView( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant showSettingsDialogFromContextMenuHeatmapView( kome::objects::Parameters* );
 * @brief shows settings dialog from popup menu
 * @return If true, it succeeded to open settings dialog. (boolean)
 */
kome::objects::Variant showSettingsDialogFromContextMenuHeatmapView( kome::objects::Parameters* );


#ifdef __unix
	}
#endif  // __unix



#endif		// __KOME_VIEW_SETTINGS_FUNCTIONS_H__
