/**
 * @file SpecChromWindowManager.cpp
 * @brief implements of SpecChromWindowManager class
 *
 * @author S.Tanaka
 * @date 2013.05.21
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "SpecChromWindowManager.h"

#include "SpectrumCanvasEx.h"
#include "ChromatogramCanvasEx.h"


using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
SpecChromWindowManager::SpecChromWindowManager() {
	// initialize
	m_sample = NULL;
	m_chromFrame = NULL;
	m_msSpecFrame = NULL;
	m_clientSize.px = -1;
	m_clientSize.py = -1;
	m_startRt = -1.0;
	m_endRt = -1.0;
	m_linkMode = false;
}

// destructor
SpecChromWindowManager::~SpecChromWindowManager() {
}

// set RT
void SpecChromWindowManager::setRt( const double start, const double end ) {
	m_startRt = start;
	m_endRt = end;
}

// get start RT
double SpecChromWindowManager::getStartRt() {
	return m_startRt;
}

// get end RT
double SpecChromWindowManager::getEndRt() {
	return m_endRt;
}

// set link mode
void SpecChromWindowManager::setLinkMode( const bool flg ) {
	m_linkMode = flg;
}

// get link mode
bool SpecChromWindowManager::isLinkMode() {
	return m_linkMode;
}

// sets the client size
void SpecChromWindowManager::setClientSize( const int width, const int height ) {
	m_clientSize.px = width;
	m_clientSize.py = height;
}

// layout children
void SpecChromWindowManager::layoutChildren() {
	// check the size
	const int width = m_clientSize.px;
	const int height = m_clientSize.py;
	if( width <= 0 || height <= 0 ) {
		return;
	}

	// rows
	int rows = 0;
	if( m_chromFrame != NULL ) {
		rows = rows + 1;
	}
	if( m_msSpecFrame != NULL ) {
		rows = rows + 1;
	}
	if( m_msmsSpecFrames.size() > 0 ) {
		rows = rows + 1;
	}
	if( m_dataMapFrames.size() > 0 ) {
		rows = rows + 1;
	}
	if( rows == 0 ) {
		return;
	}

	// layout
	int yPos = 0;
	int rowNum = 0;

	if( m_chromFrame != NULL ) {
		int w = width;
		int h = ( height - yPos ) / ( rows - rowNum );
		m_chromFrame->setFixedSize( w, h );
		m_chromFrame->Move( 0, yPos );
		m_chromFrame->Refresh();

		yPos = yPos + h;
		rowNum = rowNum + 1;
	}

	if( m_msSpecFrame != NULL ) {
		int w = width;
		int h = ( height - yPos ) / ( rows - rowNum );
		m_msSpecFrame->setFixedSize( w, h );
		m_msSpecFrame->Move( 0, yPos );
		m_msSpecFrame->Refresh();

		yPos = yPos + h;
		rowNum = rowNum + 1;
	}

	if( m_msmsSpecFrames.size() > 0 ) {
		int h = ( height - yPos ) / ( rows - rowNum );
		int xPos = 0;
		int colNum = 0;
		int cols = m_msmsSpecFrames.size();

		for( unsigned int i = 0; i < m_msmsSpecFrames.size(); i++ ) {
			kome::window::ChildFrame* frame = m_msmsSpecFrames[ i ];

			int w = ( width - xPos ) / ( cols - colNum );
			frame->setFixedSize( w, h );
			frame->Move( xPos, yPos );
			frame->Refresh();

			xPos = xPos + w;
			colNum = colNum + 1;
		}

		yPos = yPos + h;
		rowNum = rowNum + 1;
	}

	if( m_dataMapFrames.size() > 0 ) {
		int h = ( height - yPos ) / ( rows - rowNum );
		int xPos = 0;
		int colNum = 0;
		int cols = m_dataMapFrames.size();

		for( unsigned int i = 0; i < m_dataMapFrames.size(); i++ ) {
			kome::window::ChildFrame* frame = m_dataMapFrames[ i ];

			int w = ( width - xPos ) / ( cols - colNum );
			frame->setFixedSize( w, h );
			frame->Move( xPos, yPos );
			frame->Refresh();

			xPos = xPos + w;
			colNum = colNum + 1;
		}

		yPos = yPos + h;
		rowNum = rowNum + 1;
	}
}

// close all frames
void SpecChromWindowManager::closeAllChildren() {
	// check
	checkMemberFrames();

	// close
	if( m_chromFrame != NULL ) {
		m_chromFrame->Close();
		m_chromFrame->Destroy();
		delete m_chromFrame;
		m_chromFrame = NULL;
	}

	if( m_msSpecFrame != NULL ) {
		m_msSpecFrame->Close();
		m_msSpecFrame->Destroy();
		delete m_msSpecFrame;
		m_msSpecFrame = NULL;
	}

	for( unsigned int i = 0; i < m_msmsSpecFrames.size(); i++ ) {
		m_msmsSpecFrames[ i ]->Close();
		m_msmsSpecFrames[ i ]->Destroy();
		delete m_msmsSpecFrames[ i ];
	}
	m_msmsSpecFrames.clear();

	for( unsigned int i = 0 ; i < m_dataMapFrames.size(); i++ ) {
		m_dataMapFrames[ i ]->Close();
		m_dataMapFrames[ i ]->Destroy();
		delete m_dataMapFrames[ i ];
	}
	m_dataMapFrames.clear();
}

// initialize members
void SpecChromWindowManager::init() {
	m_chromFrame = NULL;
	m_msSpecFrame = NULL;
	m_msmsSpecFrames.clear();
	m_dataMapFrames.clear();
}

// check member frames
void SpecChromWindowManager::checkMemberFrames() {
	// frame set
	std::set< kome::window::ChildFrame* > frameSet;
	unsigned int num = kome::window::ChildFrame::getNumberOfFrames();
	for( unsigned int i = 0; i < num; i++ ) {
		frameSet.insert( kome::window::ChildFrame::getFrame( i ) );
	}

	// check frames
	if( frameSet.find( m_chromFrame ) == frameSet.end() ) {
		m_chromFrame = NULL;
	}

	if( frameSet.find( m_msSpecFrame ) == frameSet.end() ) {
		m_msSpecFrame = NULL;
	}

	for( int i = (int)m_msmsSpecFrames.size() - 1; i >= 0; i-- ) {
		kome::window::ChildFrame* frame = m_msmsSpecFrames[ i ];
		if( frameSet.find( frame ) == frameSet.end() ) {
			m_msmsSpecFrames.erase( m_msmsSpecFrames.begin() + i );
		}
	}

	for( int i = (int)m_dataMapFrames.size() - 1; i >= 0; i-- ) {
		kome::window::ChildFrame* frame = m_dataMapFrames[ i ];
		if( frameSet.find( frame ) == frameSet.end() ) {
			m_dataMapFrames.erase( m_dataMapFrames.begin() + i );
		}
	}
}

// open chromatogram
void SpecChromWindowManager::openFrame( kome::objects::Chromatogram* chrom ) {
	// check the parameter
	if( chrom == NULL ) {
		return;
	}

	// sample
	kome::objects::Sample* sample = chrom->getSample();
	if( sample == m_sample && m_chromFrame != NULL ) {  // same sample
		kome::window::ChromatogramCanvas* canvas = dynamic_cast< kome::window::ChromatogramCanvas* >( m_chromFrame->getCanvas() );
		if( canvas != NULL ) {
			canvas->setChromatogram( chrom );
			canvas->Refresh();
		}

		return;
	}

	// close
	closeAllChildren();

	// chromatogram window
	m_sample = sample;
	m_chromFrame = makeFrame();
	ChromatogramCanvasEx* canvas = new ChromatogramCanvasEx( m_chromFrame, chrom, -1, -1 );

	// open spectrum
	if( m_msSpecFrame == NULL && m_msmsSpecFrames.size() == 0 ) {
		// search spectrum
		kome::objects::DataSet spectra;
		if( sample != NULL ) {
			sample->getRootDataGroupNode()->getDataSet( &spectra );
		}

		double diff = FLT_MAX;
		kome::objects::Spectrum* spec = NULL;
		for( unsigned int i = 0; i < spectra.getNumberOfSpectra(); i++ ) {
			kome::objects::Spectrum* s = spectra.getSpectrum( i );
			if( s->hasChromatogram() ) {
				double tmp = fabs( m_startRt - s->getRt() );
				if( tmp < diff ) {
					diff = tmp;
					spec = s;
				}
			}
		}

		// open
		if( spec != NULL ) {
			openFrame( spec );
		}
	}

	// layout
	layoutChildren();
}

// open spectrum
void SpecChromWindowManager::openFrame( kome::objects::Spectrum* spec ) {
	// check the parameter
	if( spec == NULL ) {
		return;
	}

	// check members
	checkMemberFrames();

	// get spectra
	std::vector< kome::objects::Spectrum* > msmsSpectra;
	kome::objects::Spectrum* msSpec = getSpectra( spec, msmsSpectra );

	// sample
	kome::objects::Sample* sample = spec->getSample();
	if( sample != m_sample ) {
		closeAllChildren();
	}
	m_sample = sample;

	// open MS spectrum
	double startRt = -1.0;
	double endRt = -1.0;
	if( msSpec == NULL ) {
		if( m_msSpecFrame != NULL ) {
			m_msSpecFrame->Close();
			m_msSpecFrame->Destroy();
			delete m_msSpecFrame;
			m_msSpecFrame = NULL;
		}
	}
	else {
		startRt = msSpec->getStartRt();
		endRt = msSpec->getEndRt();

		if( m_msSpecFrame == NULL ) {
			m_msSpecFrame = makeFrame();
			wxSize size = m_msSpecFrame->GetClientSize();
			SpectrumCanvasEx* canvas = new SpectrumCanvasEx( m_msSpecFrame, msSpec, size.GetWidth(), size.GetHeight() );
		}
		else {
			kome::window::SpectrumCanvas* canvas = dynamic_cast< kome::window::SpectrumCanvas* >( m_msSpecFrame->getCanvas() );
			if( canvas != NULL ) {
				canvas->setSpectrum( msSpec );
				canvas->Refresh();
			}
		}
	}

	// open MS/MS spectra
	for( unsigned int i = 0; i < msmsSpectra.size(); i++ ) {
		// spectrum
		kome::objects::Spectrum* msmsSpec = msmsSpectra[ i ];
		if( startRt < 0.0 || endRt < 0.0 ) {
			startRt = msmsSpec->getStartRt();
			endRt = msmsSpec->getEndRt();
		}

		// frame
		kome::window::ChildFrame* frame = NULL;
		if( i < m_msmsSpecFrames.size() ) {
			frame = m_msmsSpecFrames[ i ];
			kome::window::SpectrumCanvas* canvas = dynamic_cast< kome::window::SpectrumCanvas* >( frame->getCanvas() );
			if( canvas != NULL ) {
				canvas->setSpectrum( msmsSpec );
				canvas->Refresh();
			}
		}
		else {
			frame = makeFrame();
			m_msmsSpecFrames.push_back( frame );
			wxSize size = frame->GetClientSize();
			SpectrumCanvasEx* canvas = new SpectrumCanvasEx( frame, msmsSpec, size.GetWidth(), size.GetHeight() );
		}
	}

	while( m_msmsSpecFrames.size() > msmsSpectra.size() ) {
		m_msmsSpecFrames.back()->Close();
		m_msmsSpecFrames.back()->Destroy();
		delete m_msmsSpecFrames.back();
		m_msmsSpecFrames.pop_back();
	}

	// RT
	m_startRt = startRt;
	m_endRt = endRt;

	// open chromatogram
	if( m_chromFrame == NULL ) {
		// search chromatogram
		kome::objects::Chromatogram* chrom = NULL;
		std::list< kome::objects::DataGroupNode* > groups;
		if( sample != NULL ) {
			groups.push_back( sample->getRootDataGroupNode() );

			while( chrom == NULL && groups.size() > 0 ) {
				kome::objects::DataGroupNode* group = groups.front();
				groups.pop_front();

				if( group->getNumberOfChromatograms() > 0 ) {
					chrom = group->getChromatogram( 0 );
				}
				else {
					for( unsigned int i = 0; i < group->getNumberOfChildren(); i++ ) {
						groups.push_back( group->getChild( i ) );
					}
				}
			}
		}

		// open
		if( chrom != NULL ) {
			openFrame( chrom );
		}
	}

	// layout
	layoutChildren();
}

// open frame
void SpecChromWindowManager::openFrame( kome::objects::DataMapInfo* dataMap ) {
	// parametere
	if( dataMap == NULL ) {
		return;
	}

	// check frames
	checkMemberFrames();

	// frame
	kome::window::ChildFrame* frame = makeFrame();

	// canvas
	wxWindow* wnd = kome::plugin::PluginCallTool::createDataMapWindow( *dataMap, frame );
	if( wnd == NULL ) {
		return;
	}
	kome::window::ChildFrame* parent = dynamic_cast< kome::window::ChildFrame* >( wnd->GetParent() );

	if( parent == frame ) {    // new frame
		m_dataMapFrames.push_back( frame );
	}
	else {    // old frame
		frame->Close();
		frame->Destroy();
		delete frame;
	}

	// layout
	layoutChildren();
}

// next spectrum
void SpecChromWindowManager::nextSpectrum() {
	// check the member
	if( m_msSpecFrame == NULL ) {
		return;
	}

	kome::objects::Spectrum* spec = m_msSpecFrame->getSpectrum( 0 );
	if( spec == NULL ) {
		return;
	}

	// group
	kome::objects::DataGroupNode* group = spec->getGroup();
	if( group == NULL ) {
		return;
	}

	// next spectrum
	kome::objects::Spectrum* nextSpec = NULL;
	bool loop = true;
	for( int i = (int)group->getNumberOfSpectra() - 1; i >= 0 && loop; i-- ) {
		kome::objects::Spectrum* tmp = group->getSpectrum( i );
		if( tmp == spec ) {
			loop = false;
		}
		else if( tmp->hasChromatogram() && tmp->getMsStage() == 1 ) {
			nextSpec = tmp;
		}
	}

	// open
	if( nextSpec != NULL ) {
		openFrame( nextSpec );
	}
}

// previous spectrum
void SpecChromWindowManager::prevSpectrum() {
	// check the member
	if( m_msSpecFrame == NULL ) {
		return;
	}

	kome::objects::Spectrum* spec = m_msSpecFrame->getSpectrum( 0 );
	if( spec == NULL ) {
		return;
	}

	// group
	kome::objects::DataGroupNode* group = spec->getGroup();
	if( group == NULL ) {
		return;
	}

	// previous spectrum
	kome::objects::Spectrum* prevSpec = NULL;
	bool loop = true;
	for( int i = 0; i < (int)group->getNumberOfSpectra() && loop; i++ ) {
		kome::objects::Spectrum* tmp = group->getSpectrum( i );
		if( tmp == spec ) {
			loop = false;
		}
		else if( tmp->hasChromatogram() && tmp->getMsStage() == 1 ) {
			prevSpec = tmp;
		}
	}

	// open
	if( prevSpec != NULL ) {
		openFrame( prevSpec );
	}
}

// make frame
kome::window::ChildFrame* SpecChromWindowManager::makeFrame() {
	// frame
	kome::window::ChildFrame* frame = new kome::window::ChildFrame( -1, -1, -1, -1, 0 );
	return frame;
}

// get spectra
kome::objects::Spectrum* SpecChromWindowManager::getSpectra( kome::objects::Spectrum* baseSpec, std::vector< kome::objects::Spectrum* >& msmsSpectra ) {
	// check the parameter
	if( baseSpec == NULL ) {
		return NULL;
	}

	// MS spectrum
	kome::objects::Spectrum* msSpec = baseSpec;
	while( msSpec->getParentSpectrum() != NULL ) {
		msSpec = msSpec->getParentSpectrum();
	}

	// MS/MS
	std::list< kome::objects::Spectrum* > specList;
	specList.push_back( msSpec );
	while( specList.size() > 0 ) {
		kome::objects::Spectrum* spec = specList.front();
		specList.pop_front();

		std::vector< kome::objects::Spectrum* > children;
		spec->getChildSpectra( children );

		for( unsigned int i = 0; i < children.size(); i++ ) {
			msmsSpectra.push_back( children[ i ] );
			specList.push_back( children[ i ] );
		}
	}

	return msSpec;
}

// get instance
SpecChromWindowManager& SpecChromWindowManager::getInstance() {
	// create object
	static SpecChromWindowManager mgr;

	return mgr;
}
