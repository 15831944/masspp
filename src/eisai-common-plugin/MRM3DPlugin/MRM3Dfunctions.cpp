#include "stdafx.h"
#include "MRM3DPeakDetector.h"

// same functions..
const bool Judgement_2dPeakPicking (
	kome::objects::Parameters* params,
	kome::objects::DataSet*& dataSet,
	kome::objects::Peaks2D*& peaks2d,
	kome::objects::SettingParameterValues*& settings,
	kome::core::Progress*& progress
) {
	typedef kome::plugin::PluginCallTool PluginCallTool;
	// check dataSet
	dataSet = PluginCallTool::getDataSet(params);
	if ( dataSet == nullptr ) return false;
	// check peaks2D
	peaks2d = PluginCallTool::getPeaks2D(params);
	if ( peaks2d == nullptr ) return false;
	// check have setting params
	settings = PluginCallTool::getSettingValues(params);
	if (settings == nullptr) return false;
	// set Progress
	progress = PluginCallTool::getProgress(params);
	if( progress == nullptr ) {
		progress = &kome::core::Progress::getIgnoringProgress();
	}
	return true;
}

// detect MRM 
kome::objects::Variant detectMRM3DPeaks( kome::objects::Parameters* params ) {
	// return value
	using namespace kome::objects;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG

	// data objects
	Variant ret;
	DataSet* dataSet;
	Peaks2D* peaks;
	SettingParameterValues* settings;
	kome::core::Progress* progress;

	// judge
	if (! Judgement_2dPeakPicking(params, dataSet, peaks, settings, progress)) {
		return ret;
	}

	// labeling object
	kome::mrm3d::MRM3DCore labeling(settings);

	// detect peaks
	labeling.executeLabeling(*dataSet, *progress, *peaks);
	progress->fill();

	return ret;
}
