/**
 * @file NormalizationFunction.cpp
 * @brief implements of Normalization Function plug-in functions
 *
 * @author M.Izumi
 * @date 2012.07.02
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "NormalizationFunction.h"
#include "NormalizationManager.h"
#include "NormalizationDialog.h"

#include <wx/wx.h>

using namespace kome::normalization;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// >>>>>>	@Date:2014/02/19	<Add>	A.Ozaki
// - Local Function -
//  指定されたパラメータからm/zの領域範囲を決定します
//
void	getPartition( std::vector<std::string> &param, std::set<double> &partition )
{
	std::set<double>	setMz;

	for ( unsigned int unIndex = 0 ; unIndex < param.size( ) ; unIndex++ )
	{
		std::vector< std::string >tmp;
		kome::plugin::SettingsValue::separateListValue( param[unIndex].c_str(), tmp );
			
		// 一番外側の[]が存在すれば削除
		std::string strmz = kome::plugin::SettingsValue::removeOuterBracket(( tmp.size() >= 1 ? tmp[0].c_str() : "" ));
		std::string strmzTol = kome::plugin::SettingsValue::removeOuterBracket(( tmp.size() >= 1 ? tmp[1].c_str() : "" ));

		double mz = todouble( strmz.c_str(), -1.0 );

		setMz.insert( mz );
	}
	//
	// 正規化する領域を決定します
	//
	partition.clear( );

	partition.insert( -DBL_MAX );
	if  ( 1 < setMz.size( ) )
	{
		std::set<double>::iterator	it = setMz.begin( );
		double	dPartition = *it;
		++it;
		while  ( it != setMz.end( ) )
		{
			partition.insert( ( *it - dPartition ) / 2 + dPartition );
			dPartition = *it;
			++it;
		}
	}

	partition.insert( DBL_MAX );

	return;
}
//
// <<<<<<	@Date:2014/02/19	<Add>	A.Ozaki

// open Normalization Dialog
kome::objects::Variant openNormalizationDialog( kome::objects::Parameters* ){
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;


	NormalizationDialog dlg( kome::window::WindowTool::getMainWindow() );
	if( dlg.ShowModal() == wxID_OK ){
		const char* method = dlg.getMethodName();
		const char* peak = dlg.getPeakDetect();

		kome::objects::SettingParameterValues* settings = dlg.getPeakDetectSettings();

		// get groups
		kome::objects::DataGroupNode* std = dlg.getStdGroup();
		kome::objects::DataGroupNode* trmt = dlg.getTrmtGroup();
				
		// manager
		NormalizationManager& mgr = NormalizationManager::getInstance();
		
		mgr.clearInfos();
				
		mgr.setCtrlGroup( std );
		mgr.setTrmtGroup( trmt );		
		mgr.setPeakDetectName( peak );
		
		// progress
		kome::core::Progress* pprogress = NULL;		
		kome::window::DialogProgress* progress = NULL;
		if( kome::window::WindowTool::getMainWindow() != NULL  ){
				// progress
				progress = new kome::window::DialogProgress( 
				kome::window::WindowTool::getMainWindow(),
				"Normalization", "preparing..."
			);
			pprogress = progress;
		}
		
		pprogress->createSubProgresses( 3 );
		kome::core::Progress* subProgress = pprogress->getSubProgress(0);
		
		// Peak detect
		if( mgr.executePeakDetect( peak, settings, *subProgress ) ){
			if( !subProgress->isStopped() ){
				subProgress->fill();

				if( strcmp( peak, "peak_2d" ) != 0 ){	// @date 2013/08/05 <Add> OKADA
					// ピーク変換 peak -> 2d
					if( std != NULL ){
						mgr.ConverPeaks( std );
					}
					if( trmt != NULL ){
						mgr.ConverPeaks( trmt );			
					}
				}	// @date 2013/08/05 <Add> OKADA
			
				// Normalization Filtering Function
				subProgress = pprogress->getSubProgress(1);
				mgr.executeNormalizFiltering( *subProgress );
				if( !subProgress->isStopped() ){
					subProgress->fill();
			
					// Method
					subProgress = pprogress->getSubProgress(2);
					mgr.executeNormalizMethod( method, *subProgress );
					if( !subProgress->isStopped() ){
						subProgress->fill();
					}
				}
			}
		}
		
		pprogress->fill();
		delete progress;
	
	}
	
	ret.prim.boolVal = true;
	return ret;
}

// Normalization Filter ( Threshold )
kome::objects::Variant NormalizFilterThreshold( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	
	// progress
	kome::core::Progress* progress = kome::plugin::PluginCallTool::getProgress( params );
	if( progress == NULL ){
		progress = &( kome::core::Progress::getIgnoringProgress() );
	}
	
	if( settings == NULL ){
		return ret;
	}

	if( settings->getNumberOfParameters() == 0 ){
		return ret;
	}
		
	kome::objects::DataGroupNode* group = kome::plugin::PluginCallTool::getGroup( params );
	if( group == NULL ){
		return ret;
	}

	kome::objects::Peaks2D* peak2d = kome::plugin::PluginCallTool::getPeaks2D( params );
	if( peak2d == NULL ){
		return ret;
	}

	kome::objects::Peaks2D* dst = kome::plugin::PluginCallTool::getUpdatedPeaks2D( params );

	std::string strType;
	std::string strRange;
	
	strType = settings->getParameterValue( SET_VAL_TYPE );
	strRange = settings->getParameterValue( "threshold" );
	
	std::vector< std::string >tokens;
	if( !strRange.empty() ){
		stringtoken( strRange.c_str(), ":", tokens );
	
		double start = todouble( tokens[0].c_str(), -1.0 );
		double end  = todouble( tokens[1].c_str(), -1.0 );

		NormalizationManager& mgr = NormalizationManager::getInstance();
		ret.prim.boolVal = mgr.filterThreshold( group, strType, start, end, *peak2d, *dst, progress );

	}
	return ret;
}

// Normalization Filter ( RT Range )
kome::objects::Variant NormalizFilterRtRange( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;

	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ){
		return ret;
	}
	if( settings->getNumberOfParameters() == 0 ){
		return ret;
	}
	std::string strRange;
	strRange = settings->getParameterValue( SET_VAL_RTRANGE );

	kome::objects::DataGroupNode* group = kome::plugin::PluginCallTool::getGroup( params );
	if( group == NULL ){
		return ret;
	}
	kome::objects::Peaks2D* peak2d = kome::plugin::PluginCallTool::getPeaks2D( params );
	if( peak2d == NULL ){
		return ret;
	}

	kome::objects::Peaks2D* dst = kome::plugin::PluginCallTool::getUpdatedPeaks2D( params );

	// prosess
	kome::core::Progress* progress = kome::plugin::PluginCallTool::getProgress( params );
	if( progress == NULL ){
		progress = &( kome::core::Progress::getIgnoringProgress() );
	}

	std::vector< std::string >tokens;
	if( !strRange.empty() ){
		stringtoken( strRange.c_str(), ":", tokens );
	
		double start = todouble( tokens[0].c_str(), -1.0 );
		double end  = todouble( tokens[1].c_str(), -1.0 );

		// manager
		NormalizationManager& mgr = NormalizationManager::getInstance();
		
		ret.prim.boolVal = mgr.filterRtRange( group, start, end, *peak2d, *dst, progress );
		
	}
	return ret;
}

// Normalization ( m/z Range )
kome::objects::Variant NormalizFilterMzRange( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;

	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ){
		return ret;
	}
	if( settings->getNumberOfParameters() == 0 ){
		return ret;
	}
	std::string strRange;
	strRange = settings->getParameterValue( SET_VAL_MZRANGE );
	
	kome::objects::DataGroupNode* group = kome::plugin::PluginCallTool::getGroup( params );
	if( group == NULL ){
		return ret;
	}
	
	kome::objects::Peaks2D* peak2d = kome::plugin::PluginCallTool::getPeaks2D( params );
	if( peak2d == NULL ){
		return ret;
	}

	kome::objects::Peaks2D* dst = kome::plugin::PluginCallTool::getUpdatedPeaks2D( params );
	
	// progress
	kome::core::Progress* progress = kome::plugin::PluginCallTool::getProgress( params );
	if( progress == NULL ){
		progress = &( kome::core::Progress::getIgnoringProgress() );
	}

	std::vector< std::string >tokens;
	if( !strRange.empty() ){
		stringtoken( strRange.c_str(), ":", tokens );
	
		double start = todouble( tokens[0].c_str(), -1.0 );
		double end  = todouble( tokens[1].c_str(), -1.0 );

		// manager
		NormalizationManager& mgr = NormalizationManager::getInstance();
		ret.prim.boolVal = mgr.filterMzRange( group, start, end, *peak2d, *dst, progress );

	}
	return ret;
}

// Normailzation Method Internal Standard ( Multi Sample )
kome::objects::Variant NormalizMethodInteMultiSample( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	
	// get data points
	kome::core::XYData* src = kome::plugin::PluginCallTool::getXYData( params );
	if( src == NULL ) {
		return ret;
	}

	kome::core::XYData* dst = kome::plugin::PluginCallTool::getUpdatedXYData( params );
	if( dst == NULL ) {
		return ret;
	}

	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ){
		return ret;
	}

	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ){
		return ret;
	}
		
	kome::objects::DataGroupNode* stdGroup = kome::plugin::PluginCallTool::getControlGroup( params );
	if( stdGroup == NULL ){
		return ret;
	}

	kome::objects::DataGroupNode* trmtGroup = kome::plugin::PluginCallTool::getGroup( params );
	
	// manager
	NormalizationManager& mgr = NormalizationManager::getInstance();
	mgr.setCtrlGroup( stdGroup );
	mgr.setTrmtGroup( trmtGroup );
	
	unsigned int inum = settings->getNumberOfParameters();
	for( unsigned int i=0; i < inum; i++ ){
		const char* val = settings->getParameterValue( i );
		std::vector<std::string> s;
		kome::plugin::SettingsValue::separateListValue( val, s );

		// >>>>>>	@Date:2014/02/19	<Add>	A.Ozaki
		//
		// 正規化する領域を取得します
		//
		std::set<double>	setPartition;

		getPartition( s, setPartition );

		//
		// partitionをmanagerに設定
		//
		mgr.setPartitions( setPartition );

		//
		// <<<<<<	@Date:2014/02/19	<Add>	A.Ozaki

		for( unsigned int j=0; j < s.size(); j++ ){
			std::vector< std::string >tmp;
			kome::plugin::SettingsValue::separateListValue( s[j].c_str(), tmp );			
			
			// 一番外側の[]が存在すれば削除
			std::string strRt = kome::plugin::SettingsValue::removeOuterBracket(( tmp.size() >= 1 ? tmp[0].c_str() : "" ));
			std::string strRtTol = kome::plugin::SettingsValue::removeOuterBracket(( tmp.size() >= 1 ? tmp[1].c_str() : "" ));
			std::string strmz = kome::plugin::SettingsValue::removeOuterBracket(( tmp.size() >= 1 ? tmp[2].c_str() : "" ));
			std::string strmzTol = kome::plugin::SettingsValue::removeOuterBracket(( tmp.size() >= 1 ? tmp[3].c_str() : "" ));

			double rt = todouble( strRt.c_str(), -1.0 );
			double rtTol = todouble( strRtTol.c_str(), -1.0 );
			double mz = todouble( strmz.c_str(), -1.0 );
			double mzTol = todouble( strmzTol.c_str(), -1.0 );

			// Execute a function
			mgr.executeIntStd( 
				stdGroup, 
				trmtGroup, 
				*src, 
				*dst, 
				spec, 
				false, 
				mz, 
				mzTol,
				rt, 
				rtTol );
	
		}
	}
	
	return ret;
}

// Normailzation Method Internal Standards in all Fractions ( Each Fraction )
kome::objects::Variant NormalizMethodInteEachFract( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;

	// get data points
	kome::core::XYData* src = kome::plugin::PluginCallTool::getXYData( params );
	if( src == NULL ) {
		return ret;
	}

	kome::core::XYData* dst = kome::plugin::PluginCallTool::getUpdatedXYData( params );
	if( dst == NULL ) {
		return ret;
	}

	// spectrum
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ){
		return ret;
	}

	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ){
		return ret;
	}
		
	kome::objects::DataGroupNode* stdGroup = kome::plugin::PluginCallTool::getControlGroup( params );
	if( stdGroup == NULL ){
		return ret;
	}

	kome::objects::DataGroupNode* trmtGroup = kome::plugin::PluginCallTool::getGroup( params );

	// manager
	NormalizationManager& mgr = NormalizationManager::getInstance();
	mgr.setCtrlGroup( stdGroup );
	mgr.setTrmtGroup( trmtGroup );

	unsigned int inum = settings->getNumberOfParameters();
	for( unsigned int i=0; i < inum; i++ ){
		const char* val = settings->getParameterValue( i );
		
		std::vector<std::string> s;
		kome::plugin::SettingsValue::separateListValue( val, s );

		// >>>>>>	@Date:2014/02/19	<Add>	A.Ozaki
		//
		//
		// 正規化する領域を取得します
		//
		std::set<double>	setPartition;

		getPartition( s, setPartition );

		//
		// partitionをmanagerに設定
		//
		mgr.setPartitions( setPartition );
		//
		// <<<<<<	@Date:2014/02/19	<Add>	A.Ozaki

		for( unsigned int j=0; j < s.size(); j++ ){
			std::vector< std::string >tmp;
			kome::plugin::SettingsValue::separateListValue( s[j].c_str(), tmp );
			
			// 一番外側の[]が存在すれば削除
			std::string strmz = kome::plugin::SettingsValue::removeOuterBracket(( tmp.size() >= 1 ? tmp[0].c_str() : "" ));
			std::string strmzTol = kome::plugin::SettingsValue::removeOuterBracket(( tmp.size() >= 1 ? tmp[1].c_str() : "" ));

			double mz = todouble( strmz.c_str(), -1.0 );
			double mzTol = todouble( strmzTol.c_str(), -1.0 );
			
			// Execute a function
 			mgr.executeIntStd( 
				stdGroup,
				trmtGroup,
				*src, 
				*dst, 
				spec, 
				true, 
				mz, 
				mzTol, 
				-1.0, 
				-1.0 );
		}
	}

	return ret;
}

// Normalization Method Integration ( Each Fraction )
kome::objects::Variant NormalizMethodIntegratEachFrect( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	
	// get data points
	kome::core::XYData* src = kome::plugin::PluginCallTool::getXYData( params );
	if( src == NULL ) {
		return ret;
	}

	kome::core::XYData* dst = kome::plugin::PluginCallTool::getUpdatedXYData( params );
	if( dst == NULL ) {
		return ret;
	}

	// spectrum
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ){
		return ret;
	}
	
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ){
		return ret;
	}
		
	kome::objects::DataGroupNode* stdGroup = kome::plugin::PluginCallTool::getControlGroup( params );
	if( stdGroup == NULL ){
		return ret;
	}

	kome::objects::DataGroupNode* trmtGroup = kome::plugin::PluginCallTool::getGroup( params );

	// manager
	NormalizationManager& mgr = NormalizationManager::getInstance();
	mgr.setCtrlGroup( stdGroup );
	mgr.setTrmtGroup( trmtGroup );
	
	unsigned int inum = settings->getNumberOfParameters();
	for( unsigned int i=0; i < inum; i++ ){
		const char* val = settings->getParameterValue( i );

		std::vector<std::string> s;
		kome::plugin::SettingsValue::separateListValue( val, s );

		for( unsigned int j=0; j < s.size(); j++ ){
			// Execute a function
			mgr.executeIntegrat( stdGroup, trmtGroup, *src, *dst, spec, true, s[j] );
		}
	}
			
	return ret;
}

// Normalization Metohd Integration ( Multi Sample )
kome::objects::Variant NormalizMethodIntegratMultiSample( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;

	// get data points
	kome::core::XYData* src = kome::plugin::PluginCallTool::getXYData( params );
	if( src == NULL ) {
		return ret;
	}

	kome::core::XYData* dst = kome::plugin::PluginCallTool::getUpdatedXYData( params );
	if( dst == NULL ) {
		return ret;
	}

	// spectrum
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ){
		return ret;
	}

	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ){
		return ret;
	}
		
	kome::objects::DataGroupNode* stdGroup = kome::plugin::PluginCallTool::getControlGroup( params );
	if( stdGroup == NULL ){
		return ret;
	}

	kome::objects::DataGroupNode* trmtGroup = kome::plugin::PluginCallTool::getGroup( params );

	// manager
	NormalizationManager& mgr = NormalizationManager::getInstance();
	mgr.setCtrlGroup( stdGroup );
	mgr.setTrmtGroup( trmtGroup );
		
	unsigned int inum = settings->getNumberOfParameters();
	for( unsigned int i=0; i < inum; i++ ){
		const char* val = settings->getParameterValue( i );

		std::vector<std::string> s;
		kome::plugin::SettingsValue::separateListValue( val, s );
		
		for( unsigned int j=0; j < s.size(); j++ ){
			// Execute a function
			mgr.executeIntegrat( stdGroup, trmtGroup, *src, *dst, spec, false, s[j] );
		}			
	}
		
	return ret;
}

// Normalization Method Distribution ( Max Intensity )( Each Fraction )
kome::objects::Variant NormalizMethodDistributMaxIntentEachFract( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	// get data points
	kome::core::XYData* src = kome::plugin::PluginCallTool::getXYData( params );
	if( src == NULL ) {
		return ret;
	}

	kome::core::XYData* dst = kome::plugin::PluginCallTool::getUpdatedXYData( params );
	if( dst == NULL ) {
		return ret;
	}

	// spectrum
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ){
		return ret;
	}

	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ){
		return ret;
	}

	kome::objects::DataGroupNode* stdGroup = kome::plugin::PluginCallTool::getControlGroup( params );
	if( stdGroup == NULL ){
		return ret;
	}

	kome::objects::DataGroupNode* trmtGroup = kome::plugin::PluginCallTool::getGroup( params );

	// manager
	NormalizationManager& mgr = NormalizationManager::getInstance();
	mgr.setCtrlGroup( stdGroup );
	mgr.setTrmtGroup( trmtGroup );

	unsigned int inum = settings->getNumberOfParameters();
	for( unsigned int i=0; i < inum; i++ ){
		const char* val = settings->getParameterValue( i );

		std::vector<std::string> s;
		kome::plugin::SettingsValue::separateListValue( val, s );

		for( unsigned int j=0; j < s.size(); j++ ){
			// Execute a function
			mgr.executeMaxIntensity( stdGroup, trmtGroup, *src, *dst, spec, true, s[j] );
		}

	}

	return ret;
}

// Normalization Method Distribution ( Max Intensity )( Multi Sample )
kome::objects::Variant NormalizMethodDistributMaxIntentMultSample( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	// get data points
	kome::core::XYData* src = kome::plugin::PluginCallTool::getXYData( params );
	if( src == NULL ) {
		return ret;
	}

	kome::core::XYData* dst = kome::plugin::PluginCallTool::getUpdatedXYData( params );
	if( dst == NULL ) {
		return ret;
	}

	// spectrum
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ){
		return ret;
	}

	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ){
		return ret;
	}

	kome::objects::DataGroupNode* stdGroup = kome::plugin::PluginCallTool::getControlGroup( params );
	if( stdGroup == NULL ){
		return ret;
	}

	kome::objects::DataGroupNode* trmtGroup = kome::plugin::PluginCallTool::getGroup( params );

	// manager
	NormalizationManager& mgr = NormalizationManager::getInstance();
	mgr.setCtrlGroup( stdGroup );
	mgr.setTrmtGroup( trmtGroup );

	unsigned int inum = settings->getNumberOfParameters();
	for( unsigned int i=0; i < inum; i++ ){
		const char* val = settings->getParameterValue( i );

		std::vector<std::string> s;
		kome::plugin::SettingsValue::separateListValue( val, s );

		for( unsigned int j=0; j < s.size(); j++ ){
			// Execute a function
			mgr.executeMaxIntensity( stdGroup, trmtGroup, *src, *dst, spec, false, s[j] );
		}
	}

	return ret;
}

// Normalization Method Distribution ( Median )( Each Fraction )
kome::objects::Variant NormalizMethodDistributMedianEachFract( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;

	// get data points
	kome::core::XYData* src = kome::plugin::PluginCallTool::getXYData( params );
	if( src == NULL ) {
		return ret;
	}

	kome::core::XYData* dst = kome::plugin::PluginCallTool::getUpdatedXYData( params );
	if( dst == NULL ) {
		return ret;
	}

	// spectrum
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ){
		return ret;
	}
	
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ){
		return ret;
	}
	
	kome::objects::DataGroupNode* stdGroup = kome::plugin::PluginCallTool::getControlGroup( params );
	if( stdGroup == NULL ){
		return ret;
	}

	kome::objects::DataGroupNode* trmtGroup = kome::plugin::PluginCallTool::getGroup( params );

	// manager
	NormalizationManager& mgr = NormalizationManager::getInstance();
	mgr.setCtrlGroup( stdGroup );
	mgr.setTrmtGroup( trmtGroup );

	unsigned int inum = settings->getNumberOfParameters();
	for( unsigned int i=0; i < inum; i++ ){
		const char* val = settings->getParameterValue( i );

		std::vector<std::string> s;
		kome::plugin::SettingsValue::separateListValue( val, s );

		for( unsigned int j=0; j < s.size(); j++ ){
			// Execute a function
			mgr.executeMedian( stdGroup, trmtGroup,  *src, *dst, spec, true, s[j] );
		}
	}

	return ret;
}

// Normailzation Method Distribution ( Median )( Multi Sample )
kome::objects::Variant NormalizMethodDistributMedianMultiSample( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;

	// get data points
	kome::core::XYData* src = kome::plugin::PluginCallTool::getXYData( params );
	if( src == NULL ) {
		return ret;
	}

	kome::core::XYData* dst = kome::plugin::PluginCallTool::getUpdatedXYData( params );
	if( dst == NULL ) {
		return ret;
	}

	// spectrum
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ){
		return ret;
	}

	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ){
		return ret;
	}

	kome::objects::DataGroupNode* stdGroup = kome::plugin::PluginCallTool::getControlGroup( params );
	if( stdGroup == NULL ){
		return ret;
	}

	kome::objects::DataGroupNode* trmtGroup = kome::plugin::PluginCallTool::getGroup( params );

	// manager
	NormalizationManager& mgr = NormalizationManager::getInstance();
	mgr.setCtrlGroup( stdGroup );
	mgr.setTrmtGroup( trmtGroup );

	unsigned int inum = settings->getNumberOfParameters();
	for( unsigned int i=0; i < inum; i++ ){
		const char* val = settings->getParameterValue( i );

		std::vector<std::string> s;
		kome::plugin::SettingsValue::separateListValue( val, s );

		for( unsigned int j=0; j < s.size(); j++ ){
			// Execute a function
			mgr.executeMedian( stdGroup, trmtGroup, *src, *dst, spec, false, s[j] );
		}
	}
		
	return ret;
}

// Normalization Method Distribution ( Precentile )( Each Fraction )
kome::objects::Variant NormalizMethodDistributPrecentileEachFract( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	
	// get data points
	kome::core::XYData* src = kome::plugin::PluginCallTool::getXYData( params );
	if( src == NULL ) {
		return ret;
	}

	kome::core::XYData* dst = kome::plugin::PluginCallTool::getUpdatedXYData( params );
	if( dst == NULL ) {
		return ret;
	}

	// spectrum
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ){
		return ret;
	}
	
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ){
		return ret;
	}

	kome::objects::DataGroupNode* stdGroup = kome::plugin::PluginCallTool::getControlGroup( params );
	if( stdGroup == NULL ){
		return ret;
	}

	kome::objects::DataGroupNode* trmtGroup = kome::plugin::PluginCallTool::getGroup( params );

	// manager
	NormalizationManager& mgr = NormalizationManager::getInstance();
	mgr.setCtrlGroup( stdGroup );
	mgr.setTrmtGroup( trmtGroup );

	unsigned int inum = settings->getNumberOfParameters();
	for( unsigned int i=0; i < inum; i++ ){
		const char* val = settings->getParameterValue( i );

		std::vector<std::string> s;
		kome::plugin::SettingsValue::separateListValue( val, s );

		for( unsigned int j=0; j < s.size(); j++ ){
			std::vector< std::string >tmp;
			kome::plugin::SettingsValue::separateListValue( s[j].c_str(), tmp );
			
			std::string strTop = kome::plugin::SettingsValue::removeOuterBracket(( tmp.size() >= 1 ? tmp[1].c_str() : "" ));
			int top = toint( strTop.c_str(), 10, -1 );
			
			// Execute a function
			mgr.executeDistributePercent( stdGroup, trmtGroup, *src, *dst, spec, true, tmp[0], top );
		}
	}
	
	return ret;
}

// Normalization Method Distribution ( Percentile )( Multi Sample )
kome::objects::Variant NormalizMethodDistributPrecentileMultiSample( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;

	// get data points
	kome::core::XYData* src = kome::plugin::PluginCallTool::getXYData( params );
	if( src == NULL ) {
		return ret;
	}

	kome::core::XYData* dst = kome::plugin::PluginCallTool::getUpdatedXYData( params );
	if( dst == NULL ) {
		return ret;
	}

	// spectrum
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ){
		return ret;
	}

	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ){
		return ret;
	}

	kome::objects::DataGroupNode* stdGroup = kome::plugin::PluginCallTool::getControlGroup( params );
	if( stdGroup == NULL ){
		return ret;
	}

	kome::objects::DataGroupNode* trmtGroup = kome::plugin::PluginCallTool::getGroup( params );

	// manager
	NormalizationManager& mgr = NormalizationManager::getInstance();
	mgr.setCtrlGroup( stdGroup );
	mgr.setTrmtGroup( trmtGroup );

	unsigned int inum = settings->getNumberOfParameters();
	for( unsigned int i=0; i < inum; i++ ){
		const char* val = settings->getParameterValue( i );

		std::vector<std::string> s;
		kome::plugin::SettingsValue::separateListValue( val, s );

		for( unsigned int j=0; j < s.size(); j++ ){
			std::vector< std::string >tmp;
			kome::plugin::SettingsValue::separateListValue( s[j].c_str(), tmp );
			
			std::string strTop = kome::plugin::SettingsValue::removeOuterBracket(( tmp.size() >= 1 ? tmp[1].c_str() : "" ));
			int top = toint( strTop.c_str(), 10, -1 );
			
			// Execute a function
			mgr.executeDistributePercent( stdGroup, trmtGroup, *src, *dst, spec, false, tmp[0], top );
		}
	}
	
	return ret;
}

// Set Normalization Standard Sample	// #280-2 (1)-norm_standard
kome::objects::Variant setNormStandard( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;	ret.prim.boolVal = false;

	// manager
	NormalizationManager& mgr = NormalizationManager::getInstance();
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();

	// matrix name
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		LOG_ERROR( FMT( "Failed to get arguments information." ) );
		return ret;
	}
	
	const char* n = settings->getParameterValue( "type" ); 
	std::string type = NVL( n, "" );	
	if( type.empty() ) {
		LOG_ERROR( FMT( "Failed to get the type." ) );
		return ret;
	}

	n = settings->getParameterValue( "key" ); 
	std::string key = NVL( n, "" );	
	if( key.empty() ) {
		LOG_ERROR( FMT( "Failed to get the key." ) );
		return ret;
	}

	std::string strCompareType = "file";

	// 小文字に変換してから比較
	std::transform(type.begin(), type.end(), type.begin(), tolower );
	std::transform(strCompareType.begin(), strCompareType.end(), strCompareType.begin(), tolower );
	if( type.compare( strCompareType ) != 0 ){
		// 一致しない
		LOG_ERROR( FMT( "Failed the type." ) );
		return ret;
	}else{
		// ファイルをオープンしてDataGroupNode*にする
		mgr.clearInfos();

		kome::plugin::PluginFunctionItem* pItem;
		kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
		//　szType には "file","ditfp"のどちらかが入る
		pItem = plgMgr.getFunctionItem( "GET_FILE_READER", type.c_str() );

		if( pItem != NULL ){
			const char* szKey = settings->getParameterValue( "key" );
			if( szKey == NULL || strcmp(szKey, "") == 0 ) { 
				LOG_ERROR_CODE( FMT( "Failed to get the key." ), ERR_NULL_POINTER );
				return ret;
			}

			kome::objects::SampleReader* pSampleReader = (kome::objects::SampleReader*)pItem->getCall()->invoke( NULL ).prim.pt;
		
			kome::objects::Sample* sample = NULL;

			if( pSampleReader != NULL ){
				sample = pSampleReader->openData( szKey );
			}
		
			if( sample != NULL ){

				// @date 2013/09/04 <Add> OKADA
				for( unsigned int j = 0; j < sample->getNumberOfGroups(); j++ ) {
					kome::objects::DataGroupNode* groupStandard = sample->getGroup( j );

					if( groupStandard == NULL ){
						LOG_ERROR_CODE( FMT( "Failed to get the standard group." ), ERR_NULL_POINTER );
						return ret;
					}
					// @date 2014.04.16 <Del> M.Izumi
					//if( groupStandard->getNumberOfChromatograms() > 0 ) {
						mgr.setCtrlGroup( groupStandard );
						break;
					//}
				}

				// active sample
				kome::objects::Sample* acSample = aoMgr.getActiveSample();
				if( acSample == NULL ){
					LOG_ERROR_CODE( FMT( "Failed to get the Active Sample." ), ERR_NULL_POINTER );
					return ret;
				}

				// @date 2013/09/04 <Add> OKADA
				for( unsigned int j = 0; j < acSample->getNumberOfGroups(); j++ ) {
					kome::objects::DataGroupNode* groupTreatment = acSample->getGroup( j );

					if( groupTreatment == NULL ){
						LOG_ERROR_CODE( FMT( "Failed to get the treatment group." ), ERR_NULL_POINTER );
						return ret;
					}
					// @date 2014.04.16 <Del> M.Izumi
					//if( groupTreatment->getNumberOfChromatograms() > 0 ) {
						mgr.setTrmtGroup( groupTreatment );
						break;
					//}
				}
			}
		}
	}

	ret.prim.boolVal = true;
	return ret;
}

// Set Normalization Intensity Filter	// #280-2 (2)--norm_filter_int
kome::objects::Variant setNormFilterInt( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// manager
	NormalizationManager& mgr = NormalizationManager::getInstance();

	// matrix name
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		LOG_ERROR( FMT( "Failed to get arguments information." ) );
		return ret;
	}
	
	const char* n = settings->getParameterValue( "type" ); 
	std::string type = NVL( n, "" );	
	if( type.empty() ) {
		LOG_ERROR( FMT( "Failed to get the type." ) );
		return ret;
	}

	n = settings->getParameterValue( "threshold" ); 
	std::string threshold = NVL( n, "" );	
	if( threshold.empty() ) {
		LOG_ERROR( FMT( "Failed to get the threshold." ) );
		return ret;
	}

	// setThresholdSetting()を使う
	mgr.setThresholdSetting( *settings );

	ret.prim.boolVal = true;
	return ret;
}


// Set Normalization RT Filter	// #280-2 (3)--norm_filter_rt
kome::objects::Variant setNormFilterRt( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// manager
	NormalizationManager& mgr = NormalizationManager::getInstance();

	// matrix name
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		LOG_ERROR( FMT( "Failed to get arguments information." ) );
		return ret;
	}
	
	const char* n = settings->getParameterValue( "rt_range" ); 
	std::string rt_range = NVL( n, "" );	
	if( rt_range.empty() ) {
		LOG_ERROR( FMT( "Failed to get the rt_range." ) );
		return ret;
	}

	// setRtRangeSetting()を使う
	mgr.setRtRangeSetting( *settings );

	ret.prim.boolVal = true;
	return ret;
}

// Set Normalization m/z Filter	// #280-2 (4)--norm_filter_mz
kome::objects::Variant setNormFilterMz( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// manager
	NormalizationManager& mgr = NormalizationManager::getInstance();

	// matrix name
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		LOG_ERROR( FMT( "Failed to get arguments information." ) );
		return ret;
	}
	
	const char* n = settings->getParameterValue( "mz_range" ); 
	std::string mz_range = NVL( n, "" );	
	if( mz_range.empty() ) {
		LOG_ERROR( FMT( "Failed to get the mz_range." ) );
		return ret;
	}

	// setMzRangeSetting()を使う
	mgr.setMzRangeSetting( *settings );

	ret.prim.boolVal = true;
	return ret;
}

// Normalize samples using spectrum peaks	// #280-2 (5)--norm_spec
kome::objects::Variant execNormSpec( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// manager
	NormalizationManager& mgr = NormalizationManager::getInstance();

	// matrix name
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		LOG_ERROR( FMT( "Failed to get arguments information." ) );
		return ret;
	}
	
	const char* n = settings->getParameterValue( "Labeling" ); 
	std::string peak_detector = NVL( n, "" );	
	if( peak_detector.empty() ) {
		LOG_ERROR( FMT( "Failed to get the peak_detector." ) );
		return ret;
	}

	n = settings->getParameterValue( "Clustering" ); 
	std::string methods = NVL( n, "" );	
	if( methods.empty() ) {
		LOG_ERROR( FMT( "Failed to get the methods." ) );
		return ret;
	}

	//////////
	//
	/*
	iniファイルから、「defaultcharge[true,false,true,false,false,false,false,false,false]」といった
	文字列を取得して、setParameterString()すれば、Defaultの値を設定

	[Normaliz Peak]
	PEAK_DETECTOR=gion[5,11]
	CHARGE_DETECTOR=defaultcharge[true,false,true,false,false,false,false,false,false]

	*/

	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
//	kome::core::IniFile* ini = msppMgr.getIniFile();	// kome.ini
	kome::core::IniFile* ini = msppMgr.getParameters();	// parameters.ini

	const char* DEFAULT_STRING = "DEFAULT_VALUE";
	const char* pszChargeStr = ini->getString( "Normaliz Peak", "CHARGE_DETECTOR", DEFAULT_STRING );

	if( strcmp( pszChargeStr, DEFAULT_STRING ) == 0 ){
		// 一致した==iniファイルから読み込み失敗
	}else{

		// pszChargeStrに、「defaultcharge[true,false,true,false,false,false,false,false,false]」といった感じの文字列が入るので、
		// "defaultcharge" と "true,false,true,false,false,false,false,false,false" に分割する

		std::string strCharge = pszChargeStr;

		std::string::size_type uKakkoFirst = strCharge.find( "[" );
		std::string::size_type uKakkoLast = strCharge.find_last_of ( "]" );

		std::string strKey;
		std::string strParam;

		bool flgError = false;
		if(	( std::string::npos == uKakkoFirst ) || ( std::string::npos == uKakkoLast ) ){
			//error
			flgError = true;
		}else if( (uKakkoLast - uKakkoFirst) < 0 ){
			flgError = true;
		}

		if( flgError ){
			strKey = strCharge;
			strParam = "";
		}else{
			strKey = strCharge.substr( 0, uKakkoFirst );
			strParam = strCharge.substr( uKakkoFirst+1, uKakkoLast - uKakkoFirst - 1 );
		}
		// 文字列処理終わり

		kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

		kome::plugin::PluginFunctionItem* itemChargeDetector = plgMgr.getFunctionItem("CHARGE_DETECT", strKey.c_str() );


		if( itemChargeDetector != NULL )
		{
			kome::plugin::SettingsPage* pageChargeDetector = itemChargeDetector->getSettingsPage();

			if( pageChargeDetector != NULL ){

				settings->setValue( "chargeDetector", strKey.c_str() );
				
				kome::objects::SettingParameterValues* subParameter = settings->getSubParameterValues( "chargeDetector", strKey.c_str() );

				pageChargeDetector->setParameterString( *subParameter, strParam.c_str() );

			}
		}
	}

	//
	//////////

	mgr.setPeakDetectName( "peak_spec" );
	execNormalizationCommand( "peak_spec", settings, methods.c_str() );
	
	ret.prim.boolVal = true;
	return ret;
}

// Normalize samples using 2D peaks	// #280-2 (6)-matrix_norm_2d
kome::objects::Variant execNorm2d( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// manager
	NormalizationManager& mgr = NormalizationManager::getInstance();

	// matrix name
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		LOG_ERROR( FMT( "Failed to get arguments information." ) );
		return ret;
	}
	
	const char* n = settings->getParameterValue( "Labeling" ); 
	std::string peak_detector = NVL( n, "" );	
	if( peak_detector.empty() ) {
		LOG_ERROR( FMT( "Failed to get the peak_detector." ) );
		return ret;
	}

	n = settings->getParameterValue( "Clustering" ); 
	std::string methods = NVL( n, "" );	
	if( methods.empty() ) {
		LOG_ERROR( FMT( "Failed to get the methods." ) );
		return ret;
	}

	//////////
	//
	/*
	iniファイルから、「defaultcharge[true,false,true,false,false,false,false,false,false]」といった
	文字列を取得して、setParameterString()すれば、Defaultの値を設定

	[Normaliz Peak]
	PEAK_DETECTOR=gion[5,11]
	CHARGE_DETECTOR=defaultcharge[true,false,true,false,false,false,false,false,false]

	*/

	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
//	kome::core::IniFile* ini = msppMgr.getIniFile();	// kome.ini
	kome::core::IniFile* ini = msppMgr.getParameters();	// parameters.ini

	const char* DEFAULT_STRING = "DEFAULT_VALUE";
	const char* pszChargeStr = ini->getString( "Normaliz 2D Peak", "CLUSTER_DETECTOR", DEFAULT_STRING );

	if( strcmp( pszChargeStr, DEFAULT_STRING ) == 0 ){
		// 一致した==iniファイルから読み込み失敗
	}else{

		// pszChargeStrに、「defaultcharge[true,false,true,false,false,false,false,false,false]」といった感じの文字列が入るので、
		// "defaultcharge" と "true,false,true,false,false,false,false,false,false" に分割する

		std::string strCharge = pszChargeStr;

		std::string::size_type uKakkoFirst = strCharge.find( "[" );
		std::string::size_type uKakkoLast = strCharge.find_last_of ( "]" );

		std::string strKey;
		std::string strParam;

		bool flgError = false;
		if(	( std::string::npos == uKakkoFirst ) || ( std::string::npos == uKakkoLast ) ){
			//error
			flgError = true;
		}else if( (uKakkoLast - uKakkoFirst) < 0 ){
			flgError = true;
		}

		if( flgError ){
			strKey = strCharge;
			strParam = "";
		}else{
			strKey = strCharge.substr( 0, uKakkoFirst );
			strParam = strCharge.substr( uKakkoFirst+1, uKakkoLast - uKakkoFirst - 1 );
		}
		// 文字列処理終わり

		kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

		kome::plugin::PluginFunctionItem* itemChargeDetector = plgMgr.getFunctionItem("2D_PEAK_CLUSTERING", strKey.c_str() );


		if( itemChargeDetector != NULL )
		{
			kome::plugin::SettingsPage* pageChargeDetector = itemChargeDetector->getSettingsPage();

			if( pageChargeDetector != NULL ){

				settings->setValue( "chargeDetector", strKey.c_str() );
				
				kome::objects::SettingParameterValues* subParameter = settings->getSubParameterValues( "Clustering", strKey.c_str() );

				pageChargeDetector->setParameterString( *subParameter, strParam.c_str() );

			}

			int fff= 0;
		}
	}

	//
	//////////

	mgr.setPeakDetectName( "peak_2d" );
	execNormalizationCommand( "peak_2d", settings, methods.c_str() );

	ret.prim.boolVal = true;
	return ret;
}

// exec normalization command
kome::objects::Variant execNormalizationCommand( const char* peak, kome::objects::SettingParameterValues* settings, const char* method ){
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// manager
	NormalizationManager& mgr = NormalizationManager::getInstance();

	// get groups
	kome::objects::DataGroupNode* std = mgr.getCtrlGroup();
	kome::objects::DataGroupNode* trmt = mgr.getTrmtGroup();

	// progress
	kome::core::Progress* pprogress = NULL;		

	// @date 2013/08/03 <Add> OKADA
	kome::core::DisplayProgress* progress = new kome::core::DisplayProgress( "normalization" );
	pprogress = progress;

	pprogress->createSubProgresses( 3 );
	kome::core::Progress* subProgress = pprogress->getSubProgress(0);
		
	// Peak detect
	if( mgr.executePeakDetect( peak, settings, *subProgress ) ){
		if( !subProgress->isStopped() ){
			subProgress->fill();
			if( strcmp( peak, "peak_2d" ) != 0 ){	// @date 2013/08/05 <Add> OKADA
				// ピーク変換 peak -> 2d
				if( std != NULL ){
					mgr.ConverPeaks( std );
				}
				if( trmt != NULL ){
					mgr.ConverPeaks( trmt );			
				}
			}	// @date 2013/08/05 <Add> OKADA
			
			// Normalization Filtering Function
			subProgress = pprogress->getSubProgress(1);

			if( subProgress == NULL ){	// @date 2013/08/03 <Add> OKADA
				subProgress = &kome::core::Progress::getIgnoringProgress();
			}

			mgr.executeNormalizFiltering( *subProgress );
			if( !subProgress->isStopped() ){
				subProgress->fill();
			
				// Method
				subProgress = pprogress->getSubProgress(2);

				if( subProgress == NULL ){	// @date 2013/08/03 <Add> OKADA
					subProgress = &kome::core::Progress::getIgnoringProgress();
				}

				mgr.executeNormalizMethod( method, *subProgress );
				if( !subProgress->isStopped() ){
					subProgress->fill();
				}
			}
		}

	}
		
	pprogress->fill();	
	delete progress;

	ret.prim.boolVal = true;
	return ret;
	
}
