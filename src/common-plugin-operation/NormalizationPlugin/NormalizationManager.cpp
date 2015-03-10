/**
 * @file NormalizationManager.cpp
 * @brief implements of CompositionManager class
 *
 * @author M.Izumi
 * @date 2012.07.02
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "NormalizationManager.h"

#include <wx/utils.h>

#define NORMALIZ_FILTER_FUNK	"normalization_filter"
#define NORMALIZ_METHOD_FUNK	"normalization_method"
#define NORMALIZ_PEAK_DETECT	"normaliz_peak_detect"
#define DETECT_NAME				"peak_spec"

using namespace kome::normalization;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


struct peakInfo{
	double rt;
	double srt;
	double ert;
	double mz;
	double smz;
	double emz;
	double intensity;
	int id;
};

// constructor
NormalizationManager::NormalizationManager(){
	m_groupMap.clear();
	m_ctrlGroup = NULL;
	m_trmtGroup = NULL;

	clearInfos();
}

// destructor
NormalizationManager::~NormalizationManager(){
}

// on close sample
void NormalizationManager::onCloseSample( kome::objects::Sample* sample, const bool deleting ){
	
	if( sample == NULL  ){
		return;
	}
	kome::objects::DataGroupNode* group = sample->getRootDataGroupNode();
	if( group == NULL ){
		return;
	}
		
	removeOperation( group );
}

// clear infos
void NormalizationManager::clearInfos(){
	m_groupMap.clear();
	
	removeOperation( m_ctrlGroup );
	removeOperation( m_trmtGroup );

	m_startMz = -1.0;

	m_endMz = -1.0;

	// @Date:2014/02/19	<Add>	A.Ozaki
	m_setPartition.clear( );
}

// execute normalization filtering
void NormalizationManager::executeNormalizFiltering( kome::core::Progress& progress ){
	// set group Map
	setGroupMap( m_ctrlGroup );
	setGroupMap( m_trmtGroup );

#if 1	// @date 2013/09/03 <Add> OKADA ------->
	progress.createSubProgresses( 3 );

	kome::core::Progress* subProgress = progress.getSubProgress(0);
	if( subProgress == NULL ){
		subProgress = &kome::core::Progress::getIgnoringProgress();
	}
	executeThreshold( *subProgress );

	subProgress = progress.getSubProgress(1);
	if( subProgress == NULL ){
		subProgress = &kome::core::Progress::getIgnoringProgress();
	}
	executeRtRange( *subProgress );

	subProgress = progress.getSubProgress(2);
	if( subProgress == NULL ){
		subProgress = &kome::core::Progress::getIgnoringProgress();
	}
	executeMzRange( *subProgress );
	// @date 2013/09/03 <Add> OKADA <-------
#else
	executeThreshold( progress );
	executeRtRange( progress );
	executeMzRange( progress );
#endif
}

// conver peaks
void NormalizationManager::ConverPeaks( kome::objects::DataGroupNode* group ){
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	if( !pkMgr.hasPeaks( group ) ){
		kome::objects::Peaks2D* peaks2d = pkMgr.createPeaks( group );
		
		for( unsigned int i=0; i < group->getNumberOfSpectra(); i++ ){
			kome::objects::Spectrum* spec = group->getSpectrum( i );
			if( spec != NULL ){
				kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
				kome::objects::Peaks* peak = pkMgr.getPeaks( spec );
				if( peak != NULL ){	// @date 2013/08/05 <Add> OKADA
					for( unsigned int j=0; j < peak->getLength(); j++ ){
						kome::objects::PeakElement* elt = peak->getPeak( j );
					
						kome::objects::Peak2DElement* elt2d = peaks2d->createPeak( spec->getRt(), elt->getX(), elt->getY() );
						elt2d->setId( spec->getId() );// set spec id
						kome::objects::PeaksCluster2D* cluster = peaks2d->createCluster();
						cluster->setRt( spec->getRt() );
						cluster->setIntensity( elt->getY() );
						cluster->setMz( elt->getX() );
						if( elt2d != NULL ){
							elt2d->setCluster( cluster );
						}
					}
				}
			}
		}

		kome::plugin::PluginCallTool::onUpdate2DPeaks( *group );
		kome::plugin::PluginCallTool::onUpdateGroup( *group );
	}
}

// set filter call functions
void NormalizationManager::setFilterCallFunctions( 
	kome::plugin::PluginFunctionItem* fun,
	kome::core::Progress& progress, 
	kome::objects::SettingParameterValues* settings,
	kome::objects::DataGroupNode* group,
	kome::objects::Peaks2D* peaks2d,
	kome::objects::Peaks2D& dst
){

	ConverPeaks( group );
	
	// get call
	kome::plugin::PluginCall* call = fun->getCall();
	
	// create parameters
	kome::objects::Parameters params;
	kome::plugin::PluginCallTool::setProgress( params, progress );
	kome::plugin::PluginCallTool::setSettingValues( params, *settings );
	kome::plugin::PluginCallTool::setGroup( params, *group );
	kome::plugin::PluginCallTool::setPeaks2D( params, *peaks2d );
	kome::plugin::PluginCallTool::setUpdatedPeaks2d( params, dst );

	if( call != NULL ){
		call->invoke( &params );
	}
}

// execute Filter Threshold
void NormalizationManager::executeThreshold( kome::core::Progress& progress ){
	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	
	if( m_thresholdSetting.getNumberOfParameters() == 0 ){
		return;
	}
	std::string strRange;
	strRange = m_thresholdSetting.getParameterValue( "threshold" );
	if( strRange.empty() ){
		return;
	}

	kome::plugin::PluginFunctionItem* fun = plgMgr.getFunctionItem( NORMALIZ_FILTER_FUNK, "call_threshold" );
	if( fun == NULL ){
		return ;
	}

	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	kome::objects::Peaks2D* peaks = NULL;
	kome::objects::Peaks2D* dst = NULL;

	// @date 2013/09/03 <Add> OKADA ------->
	progress.createSubProgresses( 2 );
	kome::core::Progress* subProgress = progress.getSubProgress(0);
	if( subProgress == NULL ){
		subProgress = &kome::core::Progress::getIgnoringProgress();
	}
	// @date 2013/09/03 <Add> OKADA <-------

	if( m_ctrlGroup != NULL ){
	
		peaks = pkMgr.getPeaks( m_ctrlGroup );
		dst = ( !pkMgr.hasPeaks( m_ctrlGroup ) ? pkMgr.createPeaks( m_ctrlGroup ) : peaks );
	
		// standard 
		setFilterCallFunctions( fun, *subProgress, &m_thresholdSetting, m_ctrlGroup, peaks, *dst );	// @date 2013/09/03 <Add> OKADA
	}
	if( !subProgress->isStopped() ){
		subProgress->fill();	// @date 2013/09/03 <Add> OKADA

		subProgress = progress.getSubProgress(1);	// @date 2013/09/03 <Add> OKADA
		if( subProgress == NULL ){					// @date 2013/09/03 <Add> OKADA
			subProgress = &kome::core::Progress::getIgnoringProgress();	// @date 2013/09/03 <Add> OKADA
		}											// @date 2013/09/03 <Add> OKADA

		if( !subProgress->isStopped() ){
			if( m_trmtGroup != NULL ){
				peaks = pkMgr.getPeaks( m_trmtGroup );
				dst = ( !pkMgr.hasPeaks( m_trmtGroup ) ? pkMgr.createPeaks( m_trmtGroup ) : peaks );
				// treatment
				setFilterCallFunctions( fun, *subProgress, &m_thresholdSetting, m_trmtGroup, peaks, *dst );// @date 2013/09/03 <Add> OKADA
			}
			subProgress->fill();	// @date 2013/09/03 <Add> OKADA

		}
	}
	
}

// execute Filter RT Range
void NormalizationManager::executeRtRange( kome::core::Progress& progress ){
	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	if( m_rtRangeSetting.getNumberOfParameters() == 0 ){
		return;
	}

	kome::plugin::PluginFunctionItem* fun = plgMgr.getFunctionItem( NORMALIZ_FILTER_FUNK, "call_rt_range" );
	if( fun == NULL ){
		return ;
	}

	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	kome::objects::Peaks2D* peaks = NULL; 
	kome::objects::Peaks2D* dst = NULL;

	// @date 2013/09/03 <Add> OKADA ------->
	progress.createSubProgresses( 2 );
	kome::core::Progress* subProgress = progress.getSubProgress(0);
	if( subProgress == NULL ){
		subProgress = &kome::core::Progress::getIgnoringProgress();
	}
	// @date 2013/09/03 <Add> OKADA <-------

	if( !subProgress->isStopped() ){
		if( m_ctrlGroup != NULL ){
			peaks = pkMgr.getPeaks( m_ctrlGroup );
		
			dst = ( !pkMgr.hasPeaks( m_ctrlGroup ) ? pkMgr.createPeaks( m_ctrlGroup ) : peaks );

			// standard
			setFilterCallFunctions( fun, *subProgress, &m_rtRangeSetting, m_ctrlGroup, peaks, *dst );
		}

		subProgress->fill();	// @date 2013/09/03 <Add> OKADA

		subProgress = progress.getSubProgress(1);	// @date 2013/09/03 <Add> OKADA
		if( subProgress == NULL ){					// @date 2013/09/03 <Add> OKADA
			subProgress = &kome::core::Progress::getIgnoringProgress();	// @date 2013/09/03 <Add> OKADA
		}		

		if( subProgress->isStopped() ){
			if( m_trmtGroup != NULL ){
				peaks = pkMgr.getPeaks( m_trmtGroup );
				dst = ( !pkMgr.hasPeaks( m_trmtGroup ) ? pkMgr.createPeaks( m_trmtGroup ) : peaks );

				// treatment
				setFilterCallFunctions( fun, *subProgress, &m_rtRangeSetting, m_trmtGroup, peaks, *dst );	// @date 2013/09/03 <Add> OKADA
			}

			subProgress->fill();	// @date 2013/09/03 <Add> OKADA
		}
	}
}

// execute Filter m/z Range 
void NormalizationManager::executeMzRange( kome::core::Progress& progress ){
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	if( m_mzRangeSetting.getNumberOfParameters() == 0 ){
		return;
	}

	kome::plugin::PluginFunctionItem* fun = plgMgr.getFunctionItem( NORMALIZ_FILTER_FUNK, "call_mz_range" );
	if( fun == NULL ){
		return ;
	}

	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	kome::objects::Peaks2D* peaks = NULL;
	kome::objects::Peaks2D* dst = NULL;

	// @date 2013/09/03 <Add> OKADA ------->
	progress.createSubProgresses( 2 );
	kome::core::Progress* subProgress = progress.getSubProgress(0);
	if( subProgress == NULL ){
		subProgress = &kome::core::Progress::getIgnoringProgress();
	}
	// @date 2013/09/03 <Add> OKADA <-------

	if( !subProgress->isStopped() ){
		if( m_ctrlGroup != NULL ){
			peaks = pkMgr.getPeaks( m_ctrlGroup );
			dst = ( !pkMgr.hasPeaks( m_ctrlGroup ) ? pkMgr.createPeaks( m_ctrlGroup ) : peaks );

			// standard
			setFilterCallFunctions( fun, *subProgress, &m_mzRangeSetting, m_ctrlGroup, peaks, *dst );	// @date 2013/09/03 <Add> OKADA
		}

		subProgress->fill();	// @date 2013/09/03 <Add> OKADA

		subProgress = progress.getSubProgress(1);	// @date 2013/09/03 <Add> OKADA
		if( subProgress == NULL ){					// @date 2013/09/03 <Add> OKADA
			subProgress = &kome::core::Progress::getIgnoringProgress();	// @date 2013/09/03 <Add> OKADA
		}		

		if( !subProgress->isStopped() ){
			if( m_trmtGroup != NULL ){
				peaks = pkMgr.getPeaks( m_trmtGroup );
				dst = ( !pkMgr.hasPeaks( m_trmtGroup ) ? pkMgr.createPeaks( m_trmtGroup ) : peaks );

				// treatment
				setFilterCallFunctions( fun, *subProgress, &m_mzRangeSetting, m_trmtGroup, peaks, *dst );	// @date 2013/09/03 <Add> OKADA
			}

			subProgress->fill();	// @date 2013/09/03 <Add> OKADA
		}
	}
}

// execute normalization method
void NormalizationManager::executeNormalizMethod( const char* methodName, kome::core::Progress& progress )
{
	// clear 
	m_groupMap.clear();

	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	std::vector< std::string >tokens;
	kome::plugin::SettingsValue::separateListValue( methodName, tokens );
	
	// set group Map
	setGroupMap( m_ctrlGroup );
	setGroupMap( m_trmtGroup );

	progress.setRange( 0,  tokens.size() );	// @date 2013/09/03 <Add> OKADA

	for( unsigned int i=0; i < tokens.size() && !progress.isStopped(); i++ ){
		std::string s = tokens[i];

		int ifind = s.find_first_of("[");
		std::string strItem =  s.substr( 0, ifind );
		std::string strParam = s.substr( ifind );

		kome::plugin::PluginFunctionItem* fun = plgMgr.getFunctionItem( NORMALIZ_METHOD_FUNK , strItem.c_str() );
		if( fun == NULL ){
			return;
		}
		kome::objects::SettingParameterValues settings;
		kome::plugin::SettingsPage* page = ( fun == NULL ? NULL : fun->getSettingsPage() );
		if( page != NULL ){
			page->setParameterString( settings, strParam.c_str() );
		}
		// get call
		kome::plugin::PluginCall* call = fun->getCall();
		if( call == NULL ){
			return;
		}
		
		// Multi Sampleかどうかチェック
		bool bSample = false;
		if( strItem.compare( INT_STD_SAMPLE ) == 0 || strItem.compare( INTEGRAT_SAMPLE ) == 0 ||
			strItem.compare( MAX_INTENT_SAMPLE ) == 0 || strItem.compare( MEDIAN_SAMPLE ) == 0 ||
			strItem.compare( PERCENT_SAMPLE ) == 0 
		){
			bSample = true;
		}

		// add XYDataOperation
		addOperation( call, &settings, bSample );
			
		progress.setPosition( i );	// @date 2013/09/03 <Add> OKADA
	}
	progress.fill();	// @date 2013/09/03 <Add> OKADA
	
	kome::window::WindowTool::refresh();
}

// execute Peak Detection
bool NormalizationManager::executePeakDetect( const char* peakDetect, kome::objects::SettingParameterValues* settings, kome::core::Progress& progress ){
	bool bflg = false;
	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	
	kome::plugin::PluginFunctionItem* fun = plgMgr.getFunctionItem( NORMALIZ_PEAK_DETECT , peakDetect );
	if( fun == NULL ){
		return false;
	}
	kome::plugin::SettingsPage* page = ( fun == NULL ? NULL : fun->getSettingsPage() );
	
	if( page != NULL ){
		page->getParameterString( *settings );		
	}
					
	// get call
	kome::plugin::PluginCall* call = fun->getCall();

	if( !progress.isStopped() ){
		
		// create parameters
		kome::objects::Parameters params;	
		kome::objects::DataSet* ds = m_trmtGroup;
				
		kome::plugin::PluginCallTool::setDataSet( params, *ds );
		kome::plugin::PluginCallTool::setControlGroup( params, *m_ctrlGroup );
		kome::plugin::PluginCallTool::setSettingValues( params, *settings );
		kome::plugin::PluginCallTool::setProgress( params, progress );
	
		if( call != NULL ){
			call->invoke( &params );
		}		
	}

	bflg = true;
	return bflg;
}

// get spectra
void NormalizationManager::getSpectra( kome::objects::DataSet* ds, std::vector< kome::objects::Spectrum* >& spectra ){
	if( ds == NULL ){
		return;
	}
		
	for( unsigned int i=0; i < ds->getNumberOfSpectra(); i++ ) {
		kome::objects::Spectrum* spec = ds->getSpectrum( i );
		if( spec != NULL ){
			if(  spec->getMsStage() == 1 ){
				spectra.push_back( spec );
			}
		}
	}
}

// add xy data operation
void NormalizationManager::addOperation( kome::plugin::PluginCall* call, kome::objects::SettingParameterValues* settings, bool bSample ){
	
	if( settings == NULL ){
		return;
	}
	
	// point manager
	kome::objects::PointsManager& ptMgr = kome::objects::PointsManager::getInstance();
	wxBeginBusyCursor();
	
	for( std::map<kome::objects::DataGroupNode*, std::vector< kome::objects::Spectrum*> >::iterator it = m_groupMap.begin();
		it != m_groupMap.end(); it++ ){
		std::vector< kome::objects::Spectrum* > spectra;
		spectra.clear();
		
		// サンプル間
		if( bSample ){
			if( (*it).first->getGroup() != m_ctrlGroup ){
				spectra	= m_groupMap[(*it).first->getGroup()];
			}
		}else{
			spectra	= m_groupMap[(*it).first->getGroup()];
		}

		for( unsigned int i=0; i < spectra.size(); i++ ){
			kome::objects::Spectrum* spec = spectra[i];
			if( spec != NULL ){
				// create filter subtract object
				kome::operation::Normalization* normaliz = new kome::operation::Normalization();
				normaliz->setNormalizInfo( call, settings, m_ctrlGroup );
				// update
				ptMgr.addOperation( spec, normaliz );

				kome::plugin::PluginCallTool::onUpdateSpectrum( *spec );
			}
		}
	}

	wxEndBusyCursor();
}

// remove XYDataOperation
void NormalizationManager::removeOperation( kome::objects::DataGroupNode* group ){
	if( group == NULL || group->getSample() == NULL ){
		return;
	}

	if( group != m_ctrlGroup && group != m_trmtGroup ){
		return;
	}

	kome::objects::PointsManager& mgr = kome::objects::PointsManager::getInstance();
	for( unsigned int i=0; i < group->getNumberOfSpectra(); i++ ){
		kome::objects::Spectrum* spec = group->getSpectrum(i);
		unsigned int index = mgr.getNumberOfOperations( spec );

		for( unsigned int j=0; j < index; j++ ){
			kome::objects::XYDataOperation* opt = mgr.getOperation( spec, j );
			if( opt != NULL ){
				std::string str( opt->getName() );
				if( str.find( "Normalization" ) != -1 ){
					mgr.removeOperation( spec, opt );
				}
			}
		}			
	}	

	// @date 2014.03.06 <Add> M.Izumi ->
	if( group == m_ctrlGroup ){
		m_ctrlGroup = NULL;
	}else if( group == m_trmtGroup ){
		m_trmtGroup = NULL;
	}
	// @date 2014.03.06 <Add> M.Izumi <-

}

// set group Map
void NormalizationManager::setGroupMap( kome::objects::DataGroupNode* group ){
	if( group == NULL ){
		return;
	}

	std::vector< kome::objects::Spectrum* > spectra;
	getSpectra( (kome::objects::DataSet*)group, spectra );
		
	m_groupMap[group] = spectra;
}

// get Max Intensity
void NormalizationManager::getIntStdVal(
	double mz,
	double mzTol,
	double rt,
	double rtTol,
	bool bEachFract,
	kome::objects::Spectrum* spec,
	double& maxSpecVal,
	std::vector< double >& maxY
){	
	maxY.clear();

	// m/zの範囲
	double minMz = mz-mzTol;
	double maxMz = mz+mzTol;
	
	// RTの範囲
	double minRt = 0.0 ;
	double maxRt = 0.0;
	if( rt != -1.0 ){
		minRt = rt-rtTol;
		maxRt = rt+rtTol;
	}
	// manager
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	
	double tmpmaxSpecVal = 0.0;

	for( std::map<kome::objects::DataGroupNode*, std::vector< kome::objects::Spectrum*> >::iterator it = m_groupMap.begin();
			it != m_groupMap.end(); it++ )
	{
		kome::objects::DataGroupNode* group = (*it).first->getGroup();
		maxSpecVal = 0.0;
		if( group != NULL ){
			double max = -1.0;
			kome::objects::Peaks2D* p2d = pkMgr.getPeaks( group );
			if( p2d != NULL ){
				getMaxY( spec, p2d, minMz, maxMz, minRt, maxRt, rt, max, maxSpecVal );
			}
			if( max != -1.0 ){
				maxY.push_back( max );
			}
			// @date 2014.04.16 <Add> M.Izumi
			if( spec->getGroup() == m_ctrlGroup ){
				tmpmaxSpecVal = maxSpecVal;
			}
		}
	}
	// @date 2014.04.16 <Add> M.Izumi
	if( spec->getGroup() == m_ctrlGroup ){
		maxSpecVal = tmpmaxSpecVal;
	}
}

// get Max Y ( 2d peak detect )
void NormalizationManager::getMaxY(
	kome::objects::Spectrum* spec,
	kome::objects::Peaks2D* peak, 
	double minMz,
	double maxMz,
	double minRt,
	double maxRt,
	double rt,
	double& max,
	double& specMax
){
	max = 0.0;
	for( unsigned int i=0; i < peak->getNumberOfPeaks(); i++ ){
		kome::objects::Peak2DElement* elmt = peak->getPeak(i);
			
		if( elmt != NULL ){
			double px = elmt->getMz();
			double py = elmt->getIntensity();

			// 2dPeakの時

			// m/zの指定範囲
			if( minMz < px && px < maxMz ){
				// RTの指定なし
				if( rt == -1.0 ){
					max = std::max( max, py );
				// RTの指定あり
				}else{
					double elmtRt = elmt->getRt();
					if( minRt < elmtRt && elmtRt < maxRt ){
						max = std::max( max, py );
					}
				}

				if( spec->getOrgSpectrum()->getId() == elmt->getId() ){
					specMax = std::max( specMax, py );
				}
			}
		}
	}
}

// get Value used to Normalize
void NormalizationManager::getIntegratMaxVal(
	double startMz, 
	double endMz, 
	bool bEachFract, 
	kome::objects::Spectrum* spec,
	double& maxSpecVal, 
	std::vector< double >& NormalizeVal
){
	NormalizeVal.clear();

	// manager
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	
	double tmpmaxSpecVal = 0.0;

	for( std::map<kome::objects::DataGroupNode*, std::vector< kome::objects::Spectrum*> >::iterator it = m_groupMap.begin();
			it != m_groupMap.end(); it++ ){
		
		maxSpecVal = 0.0;
		
		kome::objects::DataGroupNode* group = (*it).first;
		if( group != NULL ){
			double sum = 0.0;
			
			kome::objects::Peaks2D* p2d = pkMgr.getPeaks( group );
			if( p2d != NULL ){
				calcIntagratVal( spec, p2d, startMz, endMz, sum, maxSpecVal );	
			}
			// @date 2014.04.16 <Add> M.Izumi
			if( spec->getGroup() == m_ctrlGroup ){
				tmpmaxSpecVal = maxSpecVal;
			}	

			NormalizeVal.push_back( sum );
		}
	}
	// @date 2014.04.16 <Add> M.Izumi
	if( spec->getGroup() == m_ctrlGroup ){
		maxSpecVal = tmpmaxSpecVal;
	}
}

// calculated integration value
void NormalizationManager::calcIntagratVal(
	kome::objects::Spectrum* spec,
	kome::objects::Peaks2D* peak,
	double startMz, 
	double endMz, 
	double& sum,
	double& specMaxVal
){
	for( unsigned int i=0; i < peak->getNumberOfPeaks(); i++ ){
		kome::objects::Peak2DElement* elmt = peak->getPeak(i);
		if( elmt != NULL ){
			double px = elmt->getMz();
			// m/zの指定範囲
			if( startMz < px && px < endMz ){
				sum += elmt->getIntensity();
								
				if( spec->getOrgSpectrum()->getId() == elmt->getId() ){ // @date 2014.03.07 <Mod> M.Izumi 
					specMaxVal += elmt->getIntensity();
				}
			}
		}
	}
}

// get the distribution Max Intensity used value
void NormalizationManager::getMaxIntentUseVal(
	double startMz, 
	double endMz, 
	bool bEachFract,
	kome::objects::Spectrum* spec, 
	double& maxSpecVal, 
	std::vector< double>& NormalizeVal
){	
	NormalizeVal.clear();
	// manager
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	
	double tmpmaxSpecVal = 0.0;
	
	for( std::map<kome::objects::DataGroupNode*, std::vector< kome::objects::Spectrum*> >::iterator it = m_groupMap.begin();
		it != m_groupMap.end(); it++ ){
		
		maxSpecVal = 0.0;
		
		kome::objects::DataGroupNode* group = (*it).first;
		double max = 0.0;
		if( group != NULL ){
			kome::objects::Peaks2D* p2d = pkMgr.getPeaks( group );
			if( p2d != NULL ){
				calcMaxIntentVal( spec, p2d, startMz, endMz, max, maxSpecVal );
			}		
			NormalizeVal.push_back( max );
			// @date 2014.04.16 <Add> M.Izumi
			if( spec->getGroup() == m_ctrlGroup ){
				maxSpecVal = tmpmaxSpecVal;
			}
		}	
	}

	// @date 2014.04.16 <Add> M.Izumi
	if( spec->getGroup() == m_ctrlGroup ){
		maxSpecVal = tmpmaxSpecVal;
	}
}

// calculated Distribution Max Intensity Value
void NormalizationManager::calcMaxIntentVal(
	kome::objects::Spectrum* spec,
	kome::objects::Peaks2D* peak, 
	double startMz, 
	double endMz, 
	double& max,
	double& specMax
){
	if( peak == NULL ){	// @date 2013/08/07 <Add> OKADA
		return;			// @date 2013/08/07 <Add> OKADA
	}					// @date 2013/08/07 <Add> OKADA
	
	for( unsigned int i=0; i < peak->getNumberOfPeaks(); i++ ){
		kome::objects::Peak2DElement* elmt = peak->getPeak(i);
		double px = elmt->getMz();
		double py = elmt->getIntensity();
		// m/zの指定範囲
		if( startMz < px && px < endMz ){
			max = std::max( max, py );	
		}

		if( spec->getOrgSpectrum()->getId() == elmt->getId() ){ // @date 2014.03.07 <Mod> M.Izumi 
			specMax = std::max( specMax, py );
		}
	}
}

// get the Distribution Median used value
void NormalizationManager::getMedianUseVal( 
	double startMz,
	double endMz, 
	bool bEachFract,
	kome::objects::Spectrum* spec, 
	double& maxSpecVal,
	std::vector< double >& NormalizeVal
){
	NormalizeVal.clear();

	// manager
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	
	double tmpmaxSpecVal = 0.0;

	for( std::map<kome::objects::DataGroupNode*, std::vector< kome::objects::Spectrum*> >::iterator it = m_groupMap.begin();
		it != m_groupMap.end(); it++ ){
		
		maxSpecVal = 0.0;
		
		kome::objects::DataGroupNode* group = (*it).first;
		if( group != NULL ){
			double med = 0.0;

			kome::objects::Peaks2D* p2d = pkMgr.getPeaks( group );
			if( p2d != NULL ){ // @date 2014.03.07 <Mod> M.Izumi 
				calcMedianVal( spec, p2d, startMz, endMz, med, maxSpecVal );
			}
		
			NormalizeVal.push_back( med );
			// @date 2014.04.16 <Add> M.Izumi
			if( spec->getGroup() == m_ctrlGroup ){
				tmpmaxSpecVal = maxSpecVal;
			}
		}			
	}
	// @date 2014.04.16 <Add> M.Izumi
	if( spec->getGroup() == m_ctrlGroup ){
		maxSpecVal = tmpmaxSpecVal;
	}
}

// calculated Distribution Median Value (2d)
void NormalizationManager::calcMedianVal( 
	kome::objects::Spectrum* spec,
	kome::objects::Peaks2D* peak, 
	double startMz, 
	double endMz,
	double& max,
	double& maxSpecVal
){
	std::vector< double > specVal;
	std::vector< double >val;

	for( unsigned int i=0; i < peak->getNumberOfPeaks(); i++ ){
		kome::objects::Peak2DElement* elmt = peak->getPeak(i);
		double px = elmt->getMz();
		// m/zの指定範囲
		if( startMz < px && px < endMz ){
			val.push_back( elmt->getIntensity() );

			if( spec->getOrgSpectrum()->getId() == elmt->getId() ){ // @date 2014.03.07 <Mod> M.Izumi 
				specVal.push_back( elmt->getIntensity() );
			}
		}
	}

	// median
	if( val.size() > 0 ){
		max = kome::numeric::Statistics::median( &( val[ 0 ] ), val.size() );
	//	LOG_DEBUG( FMT( "med = %f", max ) );
	}
	if( specVal.size() > 0 ){
		maxSpecVal = kome::numeric::Statistics::median( &( specVal[ 0 ] ), specVal.size() );
	}

}

// get the Distribution Percentile used value
void NormalizationManager::getPercentileUseVal(
	double startMz, 
	double endMz, 
	int top, 
	bool bEachFract,
	kome::objects::Spectrum* spec,
	double& maxSpecVal,
	std::vector< double >& NormalizeVal
){
	// manager
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	maxSpecVal = 0.0;
	
	for( std::map<kome::objects::DataGroupNode*, std::vector<kome::objects::Spectrum*> >::iterator it = m_groupMap.begin();
		it != m_groupMap.end(); it++ ){

		kome::objects::DataGroupNode* group = (*it).first;
		if( group != NULL ){
			double percent = 0.0;
			kome::objects::Peaks2D* p2d = pkMgr.getPeaks( group );
			if( p2d != NULL ){ // @date 2014.03.07 <Mod> M.Izumi 
				calcPercentileUseVal( spec, p2d, startMz, endMz, top, percent, maxSpecVal );
			}
			
			NormalizeVal.push_back( percent );
		}
	}
}

// calculated Distribution Percentile used vale
void NormalizationManager::calcPercentileUseVal(
	kome::objects::Spectrum* spec,
	kome::objects::Peaks2D* peak,
	double startMz, 
	double endMz, 
	int top, 
	double& max,
	double& specMaxVal
){
	//get max intensity
	double maxIntensity;
	getMaxIntensity( peak,maxIntensity );
	
	max = 0.0;

	for( unsigned int i=0; i < peak->getNumberOfPeaks(); i++ ){
		kome::objects::Peak2DElement* elmt = peak->getPeak(i);
		if( elmt != NULL ){
			//double sum = 0.0;
			double py = elmt->getIntensity();
			double px = elmt->getMz();

			double val = ( maxIntensity * top ) / 100;
			if( startMz < px && px < endMz && val <= py ){
				max += py;
			}
		}
	}
}

// Filter Threshold
bool NormalizationManager::filterThreshold(
	kome::objects::DataGroupNode* group,
	std::string type, 
	double low,
	double height, 
	kome::objects::Peaks2D& peak,
	kome::objects::Peaks2D& dst,
	kome::core::Progress* progress
){
	double intentMax;
	getMaxIntensity( &peak, intentMax );
		
	double lowVal = 0.0;
	double heightVal = 0.0; 
	
	// Threshold処理
	if( type.compare( "Relative" ) == 0 ){
		if( ( 0 <= low && low <= 100 ) &&
			( low <= height && height <= 100 ) ){
					
			lowVal = ( intentMax * low ) / 100;
			heightVal = ( intentMax * height ) / 100;
		}
					
	}else{
		lowVal = abs( low );
		heightVal = abs( height );
	}

	// @date 2013/09/03 <Add> OKADA ------->
	// progress
	progress->createSubProgresses( 2 );
	kome::core::Progress* subProgress = progress->getSubProgress(0);
	if( subProgress == NULL ){
		subProgress = &kome::core::Progress::getIgnoringProgress();
	}
	subProgress->setRange( 0, peak.getNumberOfPeaks() );
	subProgress->setPosition( 0 );
	// @date 2013/09/03 <Add> OKADA <-------

	std::vector< std::pair< peakInfo, peakInfo > > info;
	for( unsigned int i=0; i < peak.getNumberOfPeaks() && !progress->isStopped(); i++ ){
		kome::objects::Peak2DElement* peakElt = peak.getPeak(i);

		// status
		std::string msg = FMT( "Peak Filtering (Threshold) [%d/%d]", ( i + 1 ), peak.getNumberOfPeaks() );
		progress->setStatus( msg.c_str() );

		if( peakElt != NULL ){
			double rt = peakElt->getRt();

			if( lowVal <= rt && rt <= heightVal ){
				peakInfo tmpPeakInfo;
				tmpPeakInfo.rt = rt;
				tmpPeakInfo.srt = peakElt->getStartRt();
				tmpPeakInfo.ert = peakElt->getEndRt();
				tmpPeakInfo.mz = peakElt->getMz();
				tmpPeakInfo.smz = peakElt->getStartMz();
				tmpPeakInfo.emz = peakElt->getEndMz();
				tmpPeakInfo.intensity = peakElt->getIntensity();
				tmpPeakInfo.id = peakElt->getId();

				peakInfo tmpClusterInfo;
				kome::objects::PeaksCluster2D* cluster = peakElt->getCluster();
				if( cluster != NULL ){
					tmpClusterInfo.rt = cluster->getRt();
					tmpClusterInfo.srt = cluster->getStartRt();
					tmpClusterInfo.ert = cluster->getEndRt();
					tmpClusterInfo.mz = cluster->getMz();
					tmpClusterInfo.smz = cluster->getStartMz();
					tmpClusterInfo.emz = cluster->getEndMz();
					tmpClusterInfo.intensity = cluster->getIntensity();
				}

				info.push_back( std::make_pair( tmpPeakInfo, tmpClusterInfo ) );
			}
			
		}
		subProgress->setPosition( i );	// @date 2013/09/03 <Add> OKADA
	}

	subProgress->fill();	// @date 2013/09/03 <Add> OKADA

	dst.clearClusters();
	dst.clearPeaks();	

	// @date 2013/09/03 <Add> OKADA ------->
	subProgress = progress->getSubProgress(1);
	if( subProgress == NULL ){
		subProgress = &kome::core::Progress::getIgnoringProgress();
	}
	// progress
	subProgress->setRange( 0, info.size() );
	subProgress->setPosition( 0 );
	// @date 2013/09/03 <Add> OKADA <-------

	for( unsigned int i=0; i < info.size() && !progress->isStopped(); i++ ){
		
		// status
		std::string msg = FMT( "create peak (Threshold) [%d/%d]", ( i + 1 ), info.size() );
		progress->setStatus( msg.c_str() );

		peakInfo pInfo = info[i].first;
		kome::objects::Peak2DElement* tmp = dst.createPeak( 
			pInfo.rt, pInfo.srt, pInfo.ert, pInfo.mz, pInfo.smz, pInfo.emz, pInfo.intensity);	
		
		tmp->setId( pInfo.id );

		// create cluster
		kome::objects::PeaksCluster2D* newCluster = dst.createCluster();
		peakInfo cInfo = info[i].second;
		if( &cInfo != NULL ){
			newCluster->setRt( cInfo.rt );
			newCluster->setMz( cInfo.mz );
			newCluster->setRtRange( cInfo.srt, cInfo.ert );
			newCluster->setMzRange( cInfo.smz, cInfo.ert );
			newCluster->setIntensity( cInfo.intensity );
		}
		// set cluster 
		if( tmp != NULL ){
			tmp->setCluster(newCluster);
		}	

		subProgress->setPosition( i );	// @date 2013/09/03 <Add> OKADA
	}

	subProgress->fill();	// @date 2013/09/03 <Add> OKADA

	kome::window::WindowTool::refresh();

	if( !progress->isStopped() ) {
		progress->fill();
	}

	return true;
}

// Filter RT Range
bool NormalizationManager::filterRtRange( 
	kome::objects::DataGroupNode* group,
	double startRt,
	double endRt,
	kome::objects::Peaks2D& peak,
	kome::objects::Peaks2D& dst,
	kome::core::Progress* progress
){
		
	std::vector< std::pair< peakInfo, peakInfo > > info;
	
	// @date 2013/09/04 <Add> OKADA ------->
	// progress
	progress->createSubProgresses( 2 );
	kome::core::Progress* subProgress = progress->getSubProgress(0);
	if( subProgress == NULL ){
		subProgress = &kome::core::Progress::getIgnoringProgress();
	}
	subProgress->setRange( 0, peak.getNumberOfPeaks() );
	subProgress->setPosition( 0 );
	// @date 2013/09/04 <Add> OKADA <-------

	for( unsigned int i=0; i < peak.getNumberOfPeaks() && !progress->isStopped(); i++ ){
		
		// status
		std::string msg = FMT( "Peak Filtering (RT Range) [%d/%d]", ( i + 1 ), peak.getNumberOfPeaks() );
		progress->setStatus( msg.c_str() );

		kome::objects::Peak2DElement* elt = peak.getPeak(i);
		if( elt != NULL ){
			double rt = elt->getRt();
			
			if(	startRt < rt && rt <= endRt ){
				peakInfo tmpPeakInfo;
				tmpPeakInfo.rt = rt;
				tmpPeakInfo.srt = elt->getStartRt();
				tmpPeakInfo.ert = elt->getEndRt();
				tmpPeakInfo.mz = elt->getMz();
				tmpPeakInfo.smz = elt->getStartMz();
				tmpPeakInfo.emz = elt->getEndMz();
				tmpPeakInfo.intensity = elt->getIntensity();
				tmpPeakInfo.id = elt->getId();

				peakInfo tmpClusterInfo;
				kome::objects::PeaksCluster2D* cluster = elt->getCluster();
				if( cluster != NULL ){
					tmpClusterInfo.rt = cluster->getRt();
					tmpClusterInfo.srt = cluster->getStartRt();
					tmpClusterInfo.ert = cluster->getEndRt();
					tmpClusterInfo.mz = cluster->getMz();
					tmpClusterInfo.smz = cluster->getStartMz();
					tmpClusterInfo.emz = cluster->getEndMz();
					tmpClusterInfo.intensity = cluster->getIntensity();
				}
				info.push_back( std::make_pair( tmpPeakInfo, tmpClusterInfo ) );
			}
		}
		subProgress->setPosition( i );	// @date 2013/09/04 <Add> OKADA
	}

	subProgress->fill();	// @date 2013/09/04 <Add> OKADA

	dst.clearClusters();
	dst.clearPeaks();

	// @date 2013/09/04 <Add> OKADA ------->
	subProgress = progress->getSubProgress(1);
	if( subProgress == NULL ){
		subProgress = &kome::core::Progress::getIgnoringProgress();
	}
	// progress
	subProgress->setRange( 0, info.size() );
	subProgress->setPosition( 0 );
	// @date 2013/09/04 <Add> OKADA <-------

	for( unsigned int i=0; i < info.size() && !progress->isStopped(); i++ ){

		// status
		std::string msg = FMT( "create peak (RT Range) [%d/%d]", ( i + 1 ), info.size() );
		progress->setStatus( msg.c_str() );

		peakInfo pInfo = info[i].first;
		kome::objects::Peak2DElement* tmp = dst.createPeak( 
			pInfo.rt, pInfo.srt, pInfo.ert, pInfo.mz, pInfo.smz, pInfo.emz, pInfo.intensity);	
		tmp->setId( pInfo.id );

		// create cluster
		kome::objects::PeaksCluster2D* newCluster = dst.createCluster();
		peakInfo cInfo = info[i].second;
		if( &cInfo != NULL ){
			newCluster->setRt( cInfo.rt );
			newCluster->setMz( cInfo.mz );
			newCluster->setRtRange( cInfo.srt, cInfo.ert );
			newCluster->setMzRange( cInfo.smz, cInfo.ert );
			newCluster->setIntensity( cInfo.intensity );
		}
		// set cluster 
		if( tmp != NULL ){
			tmp->setCluster(newCluster);
		}

		subProgress->setPosition( i + 1 );	// @date 2013/09/04 <Add> OKADA
	}

	subProgress->fill();	// @date 2013/09/04 <Add> OKADA

	if( !progress->isStopped() ) {	
		progress->fill();		
	}
	
	return true;
}

// Filter m/z range
bool NormalizationManager::filterMzRange(
	kome::objects::DataGroupNode* group,
	double startMz,
	double endMz,
	kome::objects::Peaks2D& peak,
	kome::objects::Peaks2D& dst,
	kome::core::Progress* progress
){
	std::vector< std::pair< peakInfo, peakInfo > > info;
	
	m_startMz = startMz;
	m_endMz = endMz;

	// @date 2013/09/03 <Add> OKADA ------->
	// progress
	progress->createSubProgresses( 2 );
	kome::core::Progress* subProgress = progress->getSubProgress(0);
	if( subProgress == NULL ){
		subProgress = &kome::core::Progress::getIgnoringProgress();
	}
	subProgress->setRange( 0, peak.getNumberOfPeaks() );
	subProgress->setPosition( 0 );
	// @date 2013/09/03 <Add> OKADA <-------

	for( unsigned int i=0; i < peak.getNumberOfPeaks() && !progress->isStopped(); i++ ){
		kome::objects::Peak2DElement* elt = peak.getPeak(i);
		
		// status
		std::string msg = FMT( "Peak Filtering (m/z Range) [%d/%d]", ( i + 1 ), peak.getNumberOfPeaks() );
		progress->setStatus( msg.c_str() );

		if( elt != NULL ){
			double mz = elt->getMz();
			
			if( startMz < mz && mz <= endMz ){
				peakInfo tmpPeakInfo;
				tmpPeakInfo.rt = elt->getRt();
				tmpPeakInfo.srt = elt->getStartRt();
				tmpPeakInfo.ert = elt->getEndRt();
				tmpPeakInfo.mz = elt->getMz();
				tmpPeakInfo.smz = elt->getStartMz();
				tmpPeakInfo.emz = elt->getEndMz();
				tmpPeakInfo.intensity = elt->getIntensity();
				tmpPeakInfo.id = elt->getId();

				peakInfo tmpClusterInfo;
				kome::objects::PeaksCluster2D* cluster = elt->getCluster();
				if( cluster != NULL ){
					tmpClusterInfo.rt = cluster->getRt();
					tmpClusterInfo.srt = cluster->getStartRt();
					tmpClusterInfo.ert = cluster->getEndRt();
					tmpClusterInfo.mz = cluster->getMz();
					tmpClusterInfo.smz = cluster->getStartMz();
					tmpClusterInfo.emz = cluster->getEndMz();
					tmpClusterInfo.intensity = cluster->getIntensity();
				}
				info.push_back( std::make_pair( tmpPeakInfo, tmpClusterInfo ) );
			}
		}
		subProgress->setPosition( i + 1 );	// @date 2013/09/03 <Add> OKADA
	}
	
	subProgress->fill();	// @date 2013/09/03 <Add> OKADA

	dst.clearClusters();
	dst.clearPeaks();

	// @date 2013/09/03 <Add> OKADA ------->
	subProgress = progress->getSubProgress(1);
	if( subProgress == NULL ){
		subProgress = &kome::core::Progress::getIgnoringProgress();
	}
	subProgress->setRange( 0, info.size() );
	subProgress->setPosition( 0 );
	// @date 2013/09/03 <Add> OKADA <-------

	for( unsigned int i=0; i < info.size() && !progress->isStopped(); i++ ){
		
		// status
		std::string msg = FMT( "create peak (m/z Range) [%d/%d]", ( i + 1 ), info.size() );
		progress->setStatus( msg.c_str() );

		peakInfo pInfo = info[i].first;
		kome::objects::Peak2DElement* tmp = dst.createPeak( 
			pInfo.rt, pInfo.srt, pInfo.ert, pInfo.mz, pInfo.smz, pInfo.emz, pInfo.intensity);	
		tmp->setId( pInfo.id );

		// create cluster
		kome::objects::PeaksCluster2D* newCluster = dst.createCluster();
		peakInfo cInfo = info[i].second;
		if( &cInfo != NULL ){
			newCluster->setRt( cInfo.rt );
			newCluster->setMz( cInfo.mz );
			newCluster->setRtRange( cInfo.srt, cInfo.ert );
			newCluster->setMzRange( cInfo.smz, cInfo.ert );
			newCluster->setIntensity( cInfo.intensity );
		}
		// set cluster 
		if( tmp != NULL ){
			tmp->setCluster(newCluster);
		}

		subProgress->setPosition( i + 1 );	// @date 2013/09/03 <Add> OKADA
	}

	subProgress->fill();	// @date 2013/09/03 <Add> OKADA
		
	if( !progress->isStopped() ) {	
		progress->fill();
	}
	
	return true;
}

// get calculated normalization value
double NormalizationManager::getCalcNormalizValue( kome::objects::Spectrum* spec, bool bEachFract, double specMax, std::vector<double> maxVal ){
	m_calc = 1.0;
	
	if( maxVal.empty() ){
		return m_calc;
	}

	bool flgReverse = false;
	kome::objects::DataGroupNode* groupTemp = m_groupMap.begin()->first->getGroup();
	if( groupTemp == m_ctrlGroup ){
		;	// ctrl - treatmentの順
	}else if( groupTemp == m_trmtGroup ){
		flgReverse = true;
	}
	
	double dMaxValueCtrl;
	double dMaxValueTreatment;
	if( flgReverse == false ){
		dMaxValueCtrl = maxVal[0];
		if( 2 <= maxVal.size() ){	dMaxValueTreatment = maxVal[1];		}else{	dMaxValueTreatment = 0.0;	}
	}else{
		if( 2 <= maxVal.size() ){	dMaxValueCtrl = maxVal[1];		}else{	dMaxValueCtrl = 0.0;	}
		dMaxValueTreatment = maxVal[0];
	}

	if( bEachFract ){
		kome::objects::DataGroupNode* group = spec->getGroup();

		// >>>>>>	@Date:2014/02/19	<Modify>	A.Ozaki
		//
		// EachFract==trueの場合、Standard/Treatmentの区別無く処理する必要があります
		// 1.maxVal内の最大値を求めます
		// 2.等倍比を算出します
		//
		m_calc = 1.0;
		if  ( 0 < maxVal.size( ) && 0.0 != specMax )
		{
			double	dMax = maxVal[0];
			for ( unsigned int unIndex = 0 ; unIndex < maxVal.size( ) ; unIndex++ )
			{
				if  ( dMax < maxVal[unIndex] )
				{
					dMax = maxVal[unIndex];
				}
			}
			m_calc = dMax / specMax;
		}

		//
		// <<<<<<	@Date:2014/02/19	<Modify>	A.Ozaki
	}else{
		if( maxVal.size() >= 2 || dMaxValueCtrl != 0.0 ){
			m_calc = dMaxValueCtrl / dMaxValueTreatment;
		}
	}	
	
	return m_calc;
}

// add XYData point
void NormalizationManager::addXYDataPoint( kome::core::XYData& src, kome::core::XYData& dst, double calc ){
	
	// >>>>>>	@Date:2014/02/19	<SPEC97116>	A.Ozaki
	//
	// 同じスペクトラムに対して処理を行った場合、2回目以降の補正値がdstに反映されません
	// 更新が必要な場合には、addPointではなく、updatePointを使用するように変更しました
	// また、m_startMz/m_endMzを使用して、複数の領域を処理できるようにしています
	// このとき、updatePointを使用すると前回の値に新しい値が加算されるような動作になっているので、
	// 範囲を超えているものについては、dstに出力しないようにしています。
	//
	bool	bUpdate = false;

	for( unsigned int i=0; i < src.getLength(); i++ ){
		double px = src.getX(i);
		
		bUpdate = false;
		// Spec No.88636 正規化におけるm/z Filterの不具合 @date 2012.10.31 <Mod> M.Izumi
		double py = src.getY(i);
		if( m_startMz != -1.0 && m_endMz != -1.0 ){
			if( m_startMz < px && px <= m_endMz ){
				py = calc * src.getY(i);
				bUpdate = true;
			}
		}else{
			py = calc * src.getY(i);
			bUpdate = true;
		}

		int	nIndex = dst.searchIndex( px );

		if  ( 0 > nIndex )
		{
			dst.addPoint( px, py );
		}
		else
		{
			if  ( bUpdate )
			{
				dst.updatePoint( nIndex, px, py );
			}
		}
	}
	//
	// <<<<<<	@Date:2014/02/19	<SPEC97116>	A.Ozaki
}

// execute Internal Standard
void NormalizationManager::executeIntStd( 
	kome::objects::DataGroupNode* stdGroup,
	kome::objects::DataGroupNode* trmGroup,
	kome::core::XYData& src, 
	kome::core::XYData& dst, 
	kome::objects::Spectrum* spec,
	bool bEachFract, 
	double mz,	
	double mzTol, 
	double rt, 
	double rtTol
){	
	// clear 
	m_groupMap.clear();

	setGroupMap( stdGroup );
	setGroupMap( trmGroup );
	
	std::vector< double > max;
	double specMax;
	
	getIntStdVal( mz, mzTol, rt, rtTol, bEachFract, spec, specMax, max );

	double calc = getCalcNormalizValue( spec, bEachFract, specMax, max );

	// >>>>>>	@Date:2014/02/19	<Add>	A.Ozaki
	// 
	// m_startMz/m_endMzを利用して、正規化領域を限定します
	//

	// 待避…
	double	dStartMz = m_startMz;
	double	dEndMz = m_endMz;

	//
	// partitionが設定されているか判断します
	//
	if  ( 2 < m_setPartition.size( ) )
	{
		std::set<double>::iterator	it = m_setPartition.begin( );
		double	dTempStartMz = -1.0;
		double	dTempEndMz = -1.0;
		while ( it != m_setPartition.end( ) )
		{
			if  ( mz > *it )
			{
				dTempStartMz = *it;
			}
			if  ( mz < *it )
			{
				dTempEndMz = *it;
				break;
			}
			it++;
		}
		m_startMz = dTempStartMz;
		m_endMz = dTempEndMz;
	}

	addXYDataPoint( src, dst, calc );

	// 復帰…
	m_startMz = dStartMz;
	m_endMz = dEndMz;

	//
	// <<<<<<	@Date:2014/02/19	<Add>	A.Ozaki
}

// execute Integration 
void NormalizationManager::executeIntegrat(
	kome::objects::DataGroupNode* stdGroup,
	kome::objects::DataGroupNode* trmGroup,
	kome::core::XYData& src, 
	kome::core::XYData& dst, 
	kome::objects::Spectrum* spec,
	bool bEachFract, 
	std::string strRange
){
	// clear 
	m_groupMap.clear();

	setGroupMap( stdGroup );
	setGroupMap( trmGroup );
	
	std::vector< double > max;
	double specMax;
	
	std::vector< std::string > tokens;
	stringtoken( strRange.c_str(), ":", tokens );

	double startMz = todouble( tokens[0].c_str(), -1.0 );
	double endMz = todouble( tokens[1].c_str(), -1.0 );
	
	getIntegratMaxVal( startMz, endMz, bEachFract, spec, specMax, max );

	double calc = getCalcNormalizValue( spec, bEachFract, specMax, max );
	
	addXYDataPoint( src, dst, calc );
}

// execute Distribution( Max Intensity )
void NormalizationManager::executeMaxIntensity(
	kome::objects::DataGroupNode* stdGroup,
	kome::objects::DataGroupNode* trmGroup,
	kome::core::XYData& src,
	kome::core::XYData& dst,
	kome::objects::Spectrum* spec,
	bool bEachFract,
	std::string strRange
){
	// clear 
	m_groupMap.clear();

	setGroupMap( stdGroup );
	setGroupMap( trmGroup );

	std::vector< double > max;
	double specMax;
	
	std::vector< std::string > tokens;
	stringtoken( strRange.c_str(), ":", tokens );

	double startMz = todouble( tokens[0].c_str(), -1.0 );
	double endMz = todouble( tokens[1].c_str(), -1.0 );

	getMaxIntentUseVal( startMz, endMz, bEachFract, spec, specMax, max );
	
	double calc = getCalcNormalizValue( spec, bEachFract, specMax, max );
	
	addXYDataPoint( src, dst, calc );
}

// execute Distribution( Median )
void NormalizationManager::executeMedian(
	kome::objects::DataGroupNode* stdGroup,
	kome::objects::DataGroupNode* trmGroup,
	kome::core::XYData& src,
	kome::core::XYData& dst,
	kome::objects::Spectrum* spec, 
	bool bEachFract,
	std::string strRange
){
	// clear 
	m_groupMap.clear();

	setGroupMap( stdGroup );
	setGroupMap( trmGroup );

	std::vector< double > max;
	double specMax;
	
	std::vector< std::string > tokens;
	stringtoken( strRange.c_str(), ":", tokens );

	double startMz = todouble( tokens[0].c_str(), -1.0 );
	double endMz = todouble( tokens[1].c_str(), -1.0 );
	
	getMedianUseVal( startMz, endMz, bEachFract, spec, specMax, max );
	
	double calc = getCalcNormalizValue( spec, bEachFract, specMax, max );
	
	addXYDataPoint( src, dst, calc );
}

// execute Distribution( Percentile )
void NormalizationManager::executeDistributePercent(
	kome::objects::DataGroupNode* stdGroup,
	kome::objects::DataGroupNode* trmGroup,
	kome::core::XYData& src, 
	kome::core::XYData& dst, 
	kome::objects::Spectrum* spec, 
	bool bEachFract, 
	std::string strRange,
	int top
){
	// clear 
	m_groupMap.clear();
	
	setGroupMap( stdGroup );
	setGroupMap( trmGroup );

	std::vector< double > max;
	double specMax;
	
	std::vector< std::string > tokens;
	stringtoken( strRange.c_str(), ":", tokens );

	double startMz = todouble( tokens[0].c_str(), -1.0 );
	double endMz = todouble( tokens[1].c_str(), -1.0 );
	
	getPercentileUseVal( startMz, endMz, top, bEachFract, spec, specMax, max );
	
	double calc = getCalcNormalizValue( spec, bEachFract, specMax, max );
	addXYDataPoint( src, dst, calc );
}

// get max intensity
void NormalizationManager::getMaxIntensity( kome::objects::Peaks2D* peak2d, double& maxIntensiy ){
	maxIntensiy = 0.0;
	for( unsigned int i=0; i < peak2d->getNumberOfPeaks(); i++ ){
		kome::objects::Peak2DElement* elt = peak2d->getPeak(i);
		if( elt != NULL ){
			maxIntensiy = std::max( maxIntensiy, elt->getIntensity() );
		}
	}
}

// set ctrl group
void NormalizationManager::setCtrlGroup( kome::objects::DataGroupNode* group ){
	m_ctrlGroup = group;
}

// get ctrl group
kome::objects::DataGroupNode* NormalizationManager::getCtrlGroup(){
	return m_ctrlGroup;
}

// set trmt group
void NormalizationManager::setTrmtGroup( kome::objects::DataGroupNode* group ){
	m_trmtGroup = group;
}

// get trmt group
kome::objects::DataGroupNode* NormalizationManager::getTrmtGroup(){
	return m_trmtGroup;
}

// set peak detection name
void NormalizationManager::setPeakDetectName( const char* name ){
	m_peakDetect = name;
}

// get peak detection name
std::string NormalizationManager::getPeakDetectName(){
	return m_peakDetect;
}

// set threshold settings
void NormalizationManager::setThresholdSetting( kome::objects::SettingParameterValues settings ){
	m_thresholdSetting.clear();
	m_thresholdSetting = settings;
}

// set rt range settings
void NormalizationManager::setRtRangeSetting( kome::objects::SettingParameterValues settings ){
	m_rtRangeSetting.clear();
	m_rtRangeSetting = settings;
}

// set m/z range settings
void NormalizationManager::setMzRangeSetting( kome::objects::SettingParameterValues settings ){
	m_mzRangeSetting.clear();
	m_mzRangeSetting = settings;
}

// get instance
NormalizationManager& NormalizationManager::getInstance() {
	// create object (This is the only object.)
	static NormalizationManager mgr;

	return mgr;
}

// >>>>>>	@Date:2014/02/19	<Add>	A.Ozaki
//
// set partitions
void	NormalizationManager::setPartitions( std::set<double>	&partition )
{
	m_setPartition.clear( );

	if  ( 2 < partition.size( ) )
	{
		// 2つ以上の場合、処理する分割領域が設定されています
		//
		std::set<double>::iterator	it = partition.begin( );

		double	dPartition = *it;
		++it;
		if  ( m_startMz != -1.0 )
		{
			dPartition = m_startMz;
		}
		m_setPartition.insert( dPartition );

		while  ( it != partition.end( ) )
		{
			if  ( dPartition < *it )
			{
				if  ( m_endMz != -1.0 )
				{
					if  ( m_endMz > *it )
					{
						m_setPartition.insert( *it );
					}
				}
				else
				{
					m_setPartition.insert( *it );
				}
			}
			++it;
		}
		if  ( m_endMz != -1.0 )
		{
			m_setPartition.insert( m_endMz );
		}
	}

	return;
}
//
// <<<<<<	@Date:2014/02/19	<Add>	A.Ozaki
