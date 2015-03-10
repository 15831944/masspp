/**
 * @file PeakSearchFunctions.cpp
 * @brief implement of Peak Search plug-in functions
 *
 * @author S.Tanaka
 * @date 2008.09.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "PeakSearchFunctions.h"       
#include "PeakSearch.h"
#include "PeakSearchDialog.h"
#include "PeakListCtrl.h"
#include "PeakDifferenceSearch.h"
#include "PeakDifferenceSearchDialog.h"
#include "PeakDifferenceListCtrl.h"


using namespace kome::search;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// @date 2011/05/30 <ADD> OKADA ------->
enum{
	PEAK_DISP_NONE,
	PEAK_DISP_NORMAL,		// without difference
	PEAK_DISP_DIFFERENCE,	// with difference
};
static int flgLastDisplay = PEAK_DISP_NONE;
// @date 2011/05/30 <ADD> OKADA <-------

// search peak difference
kome::objects::Variant openPeaksDifferenceSearchDialog( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// active group
	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
	kome::objects::MsDataVariant obj( var );
	kome::objects::DataGroupNode* group = obj.getGroup();
	if( group == NULL ) {
		LOG_ERROR( FMT( "A group is not active." ) );
		return ret;
	}

	// dialog
	PeakDifferenceSearchDialog dlg( kome::window::WindowTool::getMainWindow() );
	if( dlg.ShowModal() == wxID_OK ) {

		// @date 2012/04/26 <Add> OKADA ------->
		// Stage, RT, m/z, Intensity が入力されているかチェック
		if( ( dlg.getMinStage()    == 0        )&&( dlg.getMaxStage() == INT_MAX ) &&
			( dlg.getMinRt()       <= 0.000001 )&&( (DBL_MAX-1) <= dlg.getMaxRt() ) &&
			( dlg.getMinMz()       <= 0.000001 )&&( (DBL_MAX-1) <= dlg.getMaxMz() ) &&
			( dlg.getMinIntensity()<= 0.000001 )&&( (DBL_MAX-1) <= dlg.getMaxIntensity() )
		
			&& ( ( dlg.getMzDiff() < 0.0 ) && ( dlg.getMzTol() < 0.0 ) )	// Diff.とTol.の両方が未入力	//　SPEC 85552		//　@date 2012/05/10 <Add> OKADA

			) {
			// 何も入力されていない
			bool bRet = false;	// [Yes]=true [No]=false
			bRet = kome::window::WindowTool::showYesNo( 
						"No filter conditions have been specified. Do you want to filter peaks anyway?", "Filter anyway", "Don't filter" );
			if( bRet ){
				;	// Yes
			}else{
				ret.prim.boolVal = true;
				return ret;		// No
			}
		}
		// @date 2012/04/26 <Add> OKADA <-------

		// If any spectra have not been peak-detected, display the Peak Detection dialog to prompt the
		// user to detect peaks.
		kome::window::DialogProgress progress( kome::window::WindowTool::getMainWindow(), "Filter Peaks" );
		kome::core::Progress* prog = &progress;

		kome::objects::DataSet dataSet;
		group->getDataSet(&dataSet);

		// manager
		kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();

		// get spectra
		kome::objects::DataSet spectra;
		for( unsigned int i = 0; i < dataSet.getNumberOfSpectra(); i++ ) {
			// spectrum
			kome::objects::Spectrum* spec = dataSet.getSpectrum( i );

			// peaks
			kome::objects::Peaks* peaks = pkMgr.getPeaks( spec );
		
			// check whether peaks exist.
			if( peaks == NULL || peaks->getLength() == 0 ) {
				spectra.addSpectrum( spec );
			}
		}

		if( spectra.getNumberOfSpectra() > 0 ) {

			progress.createSubProgresses(2);
			prog = progress.getSubProgress(0);

			// peak detection
			kome::plugin::PluginCallTool::detectSpecPeaks( spectra, prog );
		
			if( progress.isStopped() ){
				ret.prim.boolVal = true;
				return ret;
			}

			prog = progress.getSubProgress(1);
		}

		// @date 2011/05/27 <Add> OKADA ------->
		if(	( dlg.getMzDiff() < 0.0 ) && ( dlg.getMzTol() < 0.0 ) ){

			// PeakSearchを実行すべき！

			// Difference searchのウインドウがあれば閉じる
			{
				PeakDifferenceSearch& s = PeakDifferenceSearch::getInstance();
				if ( s.isShownDialog() ){
					s.closeDialog();
				}
			}

			// search
			PeakSearch& s = PeakSearch::getInstance();

			s.reset();
			s.setStageRange( dlg.getMinStage(), dlg.getMaxStage() );
			s.setRtRange( dlg.getMinRt(), dlg.getMaxRt() );
			s.setMzRange( dlg.getMinMz(), dlg.getMaxMz() );
			s.setIntRange( dlg.getMinIntensity(), dlg.getMaxIntensity() );

			bool result = s.search( *group, prog );

			if( result ) {
				if( s.getNumberOfNotLabeledSpectra() > 0 ) {
					std::string msg = FMT( "%d spectra have not been peak-detected.", s.getNumberOfNotLabeledSpectra() );
					kome::window::WindowTool::showInfo( msg.c_str() );
				}
				s.updateDialog();
				flgLastDisplay = PEAK_DISP_NORMAL;
			}
			else {
				s.reset();
			}

			ret.prim.boolVal = result;

		}else{
		
		// @date 2011/05/27 <Add> OKADA <-------

			// Peak searchのウインドウがあれば閉じる
			{
				PeakSearch& s = PeakSearch::getInstance();
				if ( s.isShownDialog() ){
					s.closeDialog();
				}
			}

			// Difference search
			PeakDifferenceSearch& s = PeakDifferenceSearch::getInstance();

			s.reset();
			s.setMzDifference( dlg.getMzDiff() );
			s.setMzTolerance( dlg.getMzTol() );
			s.setStageRange( dlg.getMinStage(), dlg.getMaxStage() );
			s.setRtRange( dlg.getMinRt(), dlg.getMaxRt() );
			s.setMzRange( dlg.getMinMz(), dlg.getMaxMz() );
			s.setIntRange( dlg.getMinIntensity(), dlg.getMaxIntensity() );

			bool result = s.search( *group, prog );

			if( result ) {
				if( s.getNumberOfNotLabeledSpectra() > 0 ) {
					std::string msg = FMT( "%d spectra have not been peak-detected.", s.getNumberOfNotLabeledSpectra() );
					kome::window::WindowTool::showInfo( msg.c_str() );
				}
				s.updateDialog();
				flgLastDisplay = PEAK_DISP_DIFFERENCE;
			}
			else {
				s.reset();
			}

			ret.prim.boolVal = result;

		}// @date 2011/05/27 <Add> OKADA
	}
	else {
		ret.prim.boolVal = true;
	}

	return ret;
}

// toggle visibility of the Peak Search Result dialog
kome::objects::Variant togglePeakSearchDialog( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// @date 2011/05/30 <Add> OKADA ------->
	if( flgLastDisplay != PEAK_DISP_NORMAL ){
		// Difference searchのウインドウがあれば閉じる
		PeakDifferenceSearch& s = PeakDifferenceSearch::getInstance();
		if ( s.isShownDialog() ){
			s.closeDialog();
		}
		flgLastDisplay = PEAK_DISP_DIFFERENCE;
		return togglePeakDifferenceSearchDialog( NULL );
	}
	// @date 2011/05/30 <Add> OKADA <-------

	// toggle
	PeakSearch& s = PeakSearch::getInstance();
	s.toggleDialog();

	return ret;
}

// toggle visibility of the Peak Difference Result dialog
kome::objects::Variant togglePeakDifferenceSearchDialog( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// @date 2011/05/30 <Add> OKADA ------->
	if( flgLastDisplay != PEAK_DISP_DIFFERENCE ){
		// Difference searchのウインドウがあれば閉じる
		PeakDifferenceSearch& s = PeakDifferenceSearch::getInstance();
		if ( s.isShownDialog() ){
			s.closeDialog();
		}
		flgLastDisplay = PEAK_DISP_NORMAL;
		return togglePeakSearchDialog( NULL );
	}
	// @date 2011/05/30 <Add> OKADA <-------

	// toggle
	PeakDifferenceSearch& s = PeakDifferenceSearch::getInstance();
	s.toggleDialog();

	return ret;
}

// get the visibility of the Peak Search Result dialog
kome::objects::Variant isVisiblePeakSearchDialog( kome::objects::Parameters* ) {
	// search object
	PeakSearch& s = PeakSearch::getInstance();

	// @date 2011/05/30 <Add> OKADA ------->
	if( flgLastDisplay != PEAK_DISP_NORMAL ){
		flgLastDisplay = PEAK_DISP_DIFFERENCE;
		return isVisiblePeakDifferenceSearchDialog( NULL );
	}
	// @date 2011/05/30 <Add> OKADA <-------

	// get the visibility
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = s.isShownDialog();

	return ret; 
}

// get the visibility of the Peak Difference Search Result dialog
kome::objects::Variant isVisiblePeakDifferenceSearchDialog( kome::objects::Parameters* ) {
	// search object
	PeakDifferenceSearch& s = PeakDifferenceSearch::getInstance();

	// @date 2011/05/30 <Add> OKADA ------->
	if( flgLastDisplay != PEAK_DISP_DIFFERENCE ){
		flgLastDisplay = PEAK_DISP_NORMAL;
		return isVisiblePeakSearchDialog( NULL );
	}
	// @date 2011/05/30 <Add> OKADA <-------

	// get the visibility
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = s.isShownDialog();

	return ret; 
}

// update settings
kome::objects::Variant updateSettings( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;

	// search object
	PeakSearch& s = PeakSearch::getInstance();
	s.setSpecViewRange();

	return ret;
}

// exit GUI
kome::objects::Variant exitGui( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;

	// search object
	PeakSearch& ps = PeakSearch::getInstance();
	PeakDifferenceSearch& ds = PeakDifferenceSearch::getInstance();

	// release dialog
	ps.releaseDialog();
	ds.releaseDialog();

	return ret;
}
