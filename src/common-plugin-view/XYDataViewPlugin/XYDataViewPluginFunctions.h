/**
 * @file XYDataViewPluginFunctions.h
 * @brief interfaces of plug-in functions
 *
 * @author fujita
 * @date 2012.04.19
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */



#ifndef __KOME_X_Y_DATA_VIEW_PLUGIN_FUNCTIONS_H__
#define __KOME_X_Y_DATA_VIEW_PLUGIN_FUNCTIONS_H__

#ifdef __unix
	extern "C"
#endif  


	/**
	 * @fn kome::objects::Variant initPointsView( kome::objects::Parameters* )
	 * @brief initializes data grid view
	 * @return return value (This value has no meanings.)
	 */
	kome::objects::Variant initPointsView( kome::objects::Parameters* );

	/**
	* @fn kome::objects::Variant initPeaksView( kome::objects::Parameters* )
	* @brief initializes peaks grid view
	* @return return value (This value has no meanings.)
	*/
	kome::objects::Variant initPeaksView( kome::objects::Parameters* );

	/**
	* @fn kome::objects::Variant changeActiveObject( kome::objects::Parameters* params )
	* @brief changes active object
	* @param params parameters object	
	 * @return return value (This value has no meanings.)
	*/
	kome::objects::Variant changeActiveObject( kome::objects::Parameters* params );

	/**
	* @fn kome::objects::Variant updateSpecXYData( kome::objects::Parameters* )
	* @brief updates grid view
	* @return return value (This value has no meanings.)
	*/
	kome::objects::Variant updateSpecXYData( kome::objects::Parameters* );

	/**
	* @fn kome::objects::Variant updateChromXYData( kome::objects::Parameters* )
	* @brief updates grid view
	* @return return value (This value has no meanings.)
	*/
	kome::objects::Variant updateChromXYData( kome::objects::Parameters* );

	/**
	* @fn kome::objects::Variant togglePointsXYData( kome::objects::Parameters* )
	* @brief toggles the visibility of the data grid view
	* @return This function always returns true. (boolean)
	*/
	kome::objects::Variant togglePointsXYData( kome::objects::Parameters* );

	/**
	* @fn kome::objects::Variant togglePeaksXYData( kome::objects::Parameters* )
	* @brief toggles the visibility of the peaks grid view
	* @return This function always returns true. (boolean)
	*/
//	kome::objects::Variant togglePeaksXYData( kome::objects::Parameters* );

	/**
	* @fn kome::objects::Variant isVisiblePointsXYData( kome::objects::Parameters* )
	* @brief checks the visibility of the data grid view
	* @return If the data grid view is visible, this function returns true.
	*/
	kome::objects::Variant isVisiblePointsXYData( kome::objects::Parameters* );

	/**
	* @fn kome::objects::Variant isVisiblePeaksXYData( kome::objects::Parameters* )
	* @brief checks the visibility of the peaks grid view
	* @return If the peaks grid view is bisible, this function returns true.
	*/
//	kome::objects::Variant isVisiblePeaksXYData( kome::objects::Parameters* );

	/**
	* @fn kome::objects::Variant exitGui( kome::objects::Parameters* )
	* @brief exits GUI
	* @return return value (This value has no meanings.)
	*/
	kome::objects::Variant exitGui( kome::objects::Parameters* );

	/**
	* @fn kome::objects::Variant updatePeaksView( kome::objects::Parameters* params )
	* @brief updates peaks view
	* @param params parameters object
	* @return return value (This value has no meanings.)
	*/
	kome::objects::Variant updatePeaksView( kome::objects::Parameters* params );

	/**
	* @fn kome::objects::Variant togglePeakXY2DData( kome::objects::Parameters* )
	* @brief toggles the visibility of the peak grid view
	* @return This function always returns true. (boolean)
	*/
	kome::objects::Variant togglePeakXY2DData( kome::objects::Parameters* param );

	/**
	* @fn kome::objects::Variant isVisibleXY2DPeaksView( kome::objects::Parameters* )
	* @brief checks the visibility of the data grid view
	* @return If the data grid view is visible, this function returns true.
	*/
	kome::objects::Variant isVisibleXY2DPeakView( kome::objects::Parameters* );



#ifdef __unix
	}
#endif 

#endif    // __KOME_X_Y_DATA_VIEW_PLUGIN_FUNCTIONS_H__
