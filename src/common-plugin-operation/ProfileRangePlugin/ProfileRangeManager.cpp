/**
 * @file ProfileRangeManager.cpp
 * @brief implements of ProfileRangeManager class
 *
 * @author S.Tanaka
 * @date 2009.01.07
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 *
 *  <補足説明>

 * ・Version 2.0.0 での変更点  2011.05.19 <Mod> M.Izumi
   Synchronized処理を1つにまとめる。（spec, chrom, datamap)
   
   【マウス操作】
   ・X軸制御領域
   　なし       … カーソル位置のX座標をステータスバー（coordinate）に表示
	 左クリック … 
	 左ドラッグ … カーソル移動方向へXパン表示
	 右クリック …
	 右ドラッグ … X連続ズーム（初期位置より右→拡大、左・ｨ縮小）ズーム中心は初期位置、ズーム?率は一差に比例
	 左D・Nリック… XパンとXズームをリセットしてX方向全体表示

   ・Y軸制御領域
   　なし       … カーソル位置のY座標をステータスバー（coordinate）に表示
	 左クリック … 
	 左ドラッグ … カーソル移動方向へYパン表示
	 右クリック …
	 右ドラッグ … Y連続ズーム（初期位置より右→拡大、左→縮小）ズーム中心は初期位置、ズーム率は一差に比例
	 左Dクリック… YパンとYズームをリセットしてY方向全体表示

   ステータスバーに座標を表示する。

   ※ 変更箇所が多いため変更箇所は削除しています。
   　 必要そうな箇所には、コメントを残しています。
	 
 */


#include "stdafx.h"
#include "ProfileRangeManager.h"
#include "ProfileRangeOperation.h"
#include <boost/format.hpp>
#include <wx/wx.h>
#include <math.h>


using namespace kome::operation::range;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define X_MINIMUM_RANGE			0.001
#define Y_MINIMUM_RANGE			0.01
#define MOVE_WEIGHT_RATIO		0.1

#define RANGE_STATUS_KEY		"SYNCHRONIZED_RANGE" 

#define PROFILE_RANGE			"Profile Range"
#define ICON_NAME_ZOOM_X		"zoom_x"
#define ICON_NAME_ZOOM_Y		"zoom_y"
#define ICON_NAME_TRANSE_X		"transe_x"
#define ICON_NAME_TRANSE_Y		"transe_y"

// @date 2011.09.14<Add> M.Izumi ->
#define STATUSBAR_SYNC			"SyncAxes"
#define MODE_SYNC				"Synchronize Axes: On"
// <-

// constructor
ProfileRangeManager::ProfileRangeManager() {
	m_changing.chromFlag = false;
	m_changing.canvas = NULL;
	m_changing.yAxisFlag = false;
	m_changing.startPos  = -1;
	m_changing.endPos	= -1;
	m_canvas = NULL;
	m_pdlgChangeDisplayedRangeMz = NULL;
	m_pdlgChangeDisplayedRangeRT = NULL;

	// flag
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();

	// synchronized処理のflag
	m_sync = tobool( statusMgr.getValue( RANGE_STATUS_KEY ), false );

	kome::window::StatusBar& statusBar = kome::window::StatusBar::getInstance(); 
	
	if( !m_sync ){
		statusBar.setStatusMsg("SyncAxes", "" );
	}else{
		statusBar.setFlgDisp( "SyncAxes", 1 );
		statusBar.setStatusMsg("SyncAxes", "Synchronize Axes: On" );  // ステータスバーに状態表示
	}
}

// destructor
ProfileRangeManager::~ProfileRangeManager() {
}

// prepare spectrum
void ProfileRangeManager::prepare(
		kome::objects::Spectrum& spec,
		kome::img::Graphics& graphics,
		wxWindow* canvas
) {
	// spectra
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();
	std::vector< kome::objects::Spectrum* > spectra;

	// check synchronized
	bool syncFlg = m_sync;

	if( syncFlg ) {	// @date 2011.02.14 <Mod> M.Izumi
		bool found = false;
		const unsigned int num = kome::window::ChildFrame::getNumberOfFrames();
		for( unsigned int i = 0; i < num; i++ ) {
			// window
			kome::window::ChildFrame* frame = kome::window::ChildFrame::getFrame( i );
			kome::window::Canvas* c = dynamic_cast< kome::window::Canvas* >( frame->getCanvas() );

			if( c != NULL ) {
				// spectrum
				for( unsigned int j = 0; j < c->getNumberOfSpectra(); j++ ) {
					kome::objects::Spectrum* tmp = c->getSpectrum( j );
					spectra.push_back( tmp );
				}

				// check synchronized
				if( c == canvas ) {
					found = true;
				}
			}
		}

		if( !found ) {
			syncFlg = false;
		}
	}

	if( !syncFlg ) {
		spectra.clear();
		spectra.push_back( &spec );
	}

	// m_data_replase;
	bool changeflag_for_m = false;
	datarange minmaxstore;

	// x range stack
	std::stack< std::pair< double, double > >& xRangeStack
		= syncFlg ? m_syncSpecRange.xRange : m_rangeMap[ canvas ].xRange;

	// get x range
	double minX = 0.0;
	double maxX = 0.0;
	if( xRangeStack.size() > 0 ) {
		minX = xRangeStack.top().first;
		maxX = xRangeStack.top().second;
	}
	else {
		for( unsigned int i = 0; i < spectra.size(); i++ ) {
			// spectrum
			kome::objects::Spectrum* spec = spectra[ i ];
			
			double tmpMinX = spec->getMinX();
			double tmpMaxX = spec->getMaxX();

			if( i == 0 || tmpMinX < minX ) { 
				minX = tmpMinX;
			}
			if( i == 0 || tmpMaxX > maxX ) {
				maxX = tmpMaxX;
			}
		}		
		minmaxstore.minX = minX;
		minmaxstore.maxX = maxX;
		changeflag_for_m = true;
	}

	double midX = ( minX + maxX ) / 2.0;
	double xMinRange = X_MINIMUM_RANGE;
	minX = std::min( minX, midX - xMinRange / 2.0 );
	maxX = std::max( maxX, midX + xMinRange / 2.0 );

	// y range stack
	std::stack< std::pair< double, double > >& yRangeStack
		= syncFlg ? m_syncSpecRange.yRange : m_rangeMap[ canvas ].yRange;

	// get y range
	double minY = 0.0;
	double maxY = Y_MINIMUM_RANGE;
	if( yRangeStack.size() > 0 ) {
		minY = yRangeStack.top().first;
		maxY = yRangeStack.top().second;
	}
	else {
		for( unsigned int i = 0; i < spectra.size(); i++ ) {
			// spectrum
			kome::objects::Spectrum* spec = spectra[ i ];

			// xy data
			kome::core::XYData* xyData = spec->getXYData();
			if( xyData != NULL && xyData->getLength() > 0 ) {
				double tmpMinY = 0.0;
				double tmpMaxY = 0.0;

				getIntensityRange( *xyData, minX, maxX, &tmpMinY, &tmpMaxY );

				minY = std::min( minY, tmpMinY );
				maxY = std::max( maxY, tmpMaxY + Y_MINIMUM_RANGE );
			}
		}
		if(changeflag_for_m) {
			minmaxstore.minY = minY;
			minmaxstore.maxY = maxY;
			minmaxstore.canvas = canvas;

			if(m_canvas != canvas) {
				m_canvas = canvas;

				if(m_specdata.empty()) {
					m_specdata.push_back(minmaxstore);
				} else {
					bool ccflag = true;
					for(unsigned int x = 0; x < m_specdata.size(); x++) {
						if(m_specdata[x].canvas == canvas) {
							ccflag = false;
							break;
						}
					}
					if(ccflag) m_specdata.push_back(minmaxstore);
				}
			}

		}
	}

	double midY = ( minY + maxY ) / 2.0;
	minY = std::min( minY, midY - Y_MINIMUM_RANGE / 2.0 );
	maxY = std::max( maxY, midY + Y_MINIMUM_RANGE / 2.0 );

	// set data range
	graphics.setDisplayedRange( minX, maxX, minY, maxY );
}

// prepare chromatogram
void ProfileRangeManager::prepare(
		kome::objects::Chromatogram& chrom,
		kome::img::Graphics& graphics,
		wxWindow* canvas
) {
	// chromatograms
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();
	std::vector< kome::objects::Chromatogram* > chroms;
	
	// check synchronized
	bool syncFlg = m_sync;

	if( syncFlg ) {
		bool found = false;
		const unsigned int num = kome::window::ChildFrame::getNumberOfFrames();
		for( unsigned int i = 0; i < num; i++ ) {
			// window
			kome::window::ChildFrame* frame = kome::window::ChildFrame::getFrame( i );
			kome::window::Canvas* c = dynamic_cast< kome::window::Canvas* >( frame->getCanvas() );

			if( c != NULL ) {
				// chromatogram
				for( unsigned int j = 0; j < c->getNumberOfChromatograms(); j++ ) {
					kome::objects::Chromatogram* tmp = c->getChromatogram( j );
					chroms.push_back( tmp );
				}

				// check synchronized
				if( c == canvas ) {
					found = true;
				}
			}
		}

		if( !found ) {
			syncFlg = false;
		}
	}

	if( !syncFlg ) {
		chroms.clear();
		chroms.push_back( &chrom );
	}

	// data_replase;
	bool changeflag_for_m = false;
	datarange minmaxstore;

	// x range stack
	std::stack< std::pair< double, double > >& xRangeStack
		= syncFlg ? m_syncChromRange.xRange : m_rangeMap[ canvas ].xRange;

	// get x range
	double minX = 0.0;
	double maxX = 0.0;
	if( xRangeStack.size() > 0 ) {
		minX = xRangeStack.top().first;
		maxX = xRangeStack.top().second;
	}
	else {
		for( unsigned int i = 0; i < chroms.size(); i++ ) {
			// chromatogram
			kome::objects::Chromatogram* chrom = chroms[ i ];

			// xy data
			kome::core::XYData* xyData = chrom->getXYData();
			if( xyData != NULL && xyData->getLength() > 0 ) {
				double tmpMinX = xyData->getX( 0 );
				double tmpMaxX = xyData->getX( xyData->getLength() - 1 );

				if( i == 0 || tmpMinX < minX ) {
					minX = tmpMinX;
				}
				if( i == 0 || tmpMaxX > maxX ) {
					maxX = tmpMaxX;
				}
			}
		}
		minmaxstore.minX = minX;
		minmaxstore.maxX = maxX;
		changeflag_for_m = true;
	}

	double midX = ( minX + maxX ) / 2.0;
	minX = std::min( minX, midX - X_MINIMUM_RANGE / 2.0 );
	maxX = std::max( maxX, midX + X_MINIMUM_RANGE / 2.0 );

	// y range stack
	std::stack< std::pair< double, double > >& yRangeStack
		= syncFlg ? m_syncChromRange.yRange : m_rangeMap[ canvas ].yRange;

	// get y range
	double minY = 0.0;
	double maxY = Y_MINIMUM_RANGE;
	if( yRangeStack.size() > 0 ) {
		minY = yRangeStack.top().first;
		maxY = yRangeStack.top().second;
	}
	else {
		for( unsigned int i = 0; i < chroms.size(); i++ ) {
			// chromatogram
			kome::objects::Chromatogram* chrom = chroms[ i ];

			// xy data
			kome::core::XYData* xyData = chrom->getXYData();
			if( xyData != NULL && xyData->getLength() > 0 ) {
				double tmpMinY = 0.0;
				double tmpMaxY = 0.0;

				getIntensityRange( *xyData, minX, maxX, &tmpMinY, &tmpMaxY );

				minY = std::min( minY, tmpMinY );
				maxY = std::max( maxY, tmpMaxY + Y_MINIMUM_RANGE );
			}
		}
		if(changeflag_for_m) {
			minmaxstore.minY = minY;
			minmaxstore.maxY = maxY;
			minmaxstore.canvas = canvas;

			if(m_canvas != canvas) {
				m_canvas = canvas;
			
				if(m_chromdata.empty()) {
					m_chromdata.push_back(minmaxstore);
				} else {
					bool ccflag = true;
					for(unsigned int x = 0; x < m_chromdata.size(); x++) {
						if(m_chromdata[x].canvas == canvas) {
							ccflag = false;
							break;
						}
					}
					if(ccflag) 	m_chromdata.push_back(minmaxstore);
				}
			}
		}
	}

	double midY = ( minY + maxY ) / 2.0;
	minY = std::min( minY, midY - Y_MINIMUM_RANGE / 2.0 );
	maxY = std::max( maxY, midY + Y_MINIMUM_RANGE / 2.0 );

	// set data range
	graphics.setDisplayedRange( minX, maxX, minY, maxY );
}

// get max intensity in range
void ProfileRangeManager::getIntensityRange(
		kome::core::XYData& xyData,
		const double minX,
		const double maxX,
		double* minY,
		double* maxY
) {
	// get index
	int startIndex = xyData.searchIndex( minX );
	if( startIndex < 0 ) {
		startIndex = - startIndex - 2;
	}
	if( startIndex < 0 ) {
		startIndex = 0;
	}

	int endIndex = xyData.searchIndex( maxX );
	if( endIndex < 0 ) {
		endIndex = - endIndex - 1;
	}
	if( endIndex >= (int)xyData.getLength() ) {
		endIndex = (int)xyData.getLength() - 1;
	}

	// get max intensity
	double minIntensity = 0.0;
	double maxIntensity = 0.0;

	for( int i = startIndex; i <= endIndex; i++ ) {
		double y = xyData.getY( i );
		if( i == startIndex || y < minIntensity ) {
			minIntensity = y;
		}
		if( i == startIndex || y > maxIntensity ) {
			maxIntensity = y;
		}
	}

	// store
	if( minY != NULL ) {
		*minY = minIntensity;
	}
	if( maxY != NULL ) {
		*maxY = maxIntensity;
	}	
}

// close 
void ProfileRangeManager::close( wxWindow* canvas ) {
	if( m_rangeMap.find( canvas ) != m_rangeMap.end() ) {
		m_rangeMap.erase( canvas );
	}
	// @date 2011.12.23 <Add> M.Izumi
	if( m_canvas == canvas ){
		m_canvas = NULL;
	}
}

// down button (spectrum)
bool ProfileRangeManager::downButton(
		kome::objects::Spectrum& spec,		
		kome::evt::MouseEvent& evt,
		kome::img::Graphics& graphics,
		wxWindow* canvas
) {
	//2012/02/28 add by t.okuno -> This plugin displays a range choice screen.
	// close spec dialog
	if( m_pdlgChangeDisplayedRangeMz != NULL ){
		m_pdlgChangeDisplayedRangeMz->Destroy();
		delete m_pdlgChangeDisplayedRangeMz;
		m_pdlgChangeDisplayedRangeMz = NULL;
	}
	//2012/02/28 add by t.okuno <- This plugin displays a range choice screen.

	m_changing.chromFlag = false;
	m_changing.canvas = canvas;
	for(unsigned int i = 0; i < m_specdata.size(); i++) {
		if(m_specdata[i].canvas == canvas) {
			m_range = &m_specdata[i];

			return setDownButtonInfo( evt, graphics );
		}
	}
	return false;
}

// down button (chromatogram)
bool ProfileRangeManager::downButton(
		kome::objects::Chromatogram& chrom,
		kome::evt::MouseEvent& evt,
		kome::img::Graphics& graphics,
		wxWindow* canvas
) {
	//2012/02/28 add by t.okuno -> This plugin displays a range choice screen.
	// close spec dialog
	if( m_pdlgChangeDisplayedRangeRT != NULL ){
		m_pdlgChangeDisplayedRangeRT->Destroy();
		delete m_pdlgChangeDisplayedRangeRT;
		m_pdlgChangeDisplayedRangeRT = NULL;
	}

	//2012/02/28 add by t.okuno <- This plugin displays a range choice screen.
	m_changing.chromFlag = true;
	m_changing.canvas = canvas;
	for(unsigned int i = 0; i < m_chromdata.size(); i++) {
		if(m_chromdata[i].canvas == canvas) {
			m_range = &m_chromdata[i];
			
			return setDownButtonInfo( evt, graphics );
		}
	}
	return false;
}

// get range information
ProfileRangeManager::RangeInfo* ProfileRangeManager::getRange() {
	// get range
	RangeInfo* range = NULL;

	// 拡大、縮小の処理をChildFrameだけでなくCanvasにも反映させる為の変更 @date 2013.04.03 <Add> M.Izumi
	kome::window::ChildFrame* frame = ( m_changing.canvas == NULL ? NULL : dynamic_cast< kome::window::ChildFrame* >( m_changing.canvas->GetParent() ) );
	if( frame == NULL ){ // ChildFrame 以外のキャンバスの表示範囲取得
		range = &( m_rangeMap[ m_changing.canvas ] );
		return range;
	}

	if( m_changing.chromFlag ) {  // chromatogram
		range = ( m_sync ? &m_syncChromRange : &( m_rangeMap[ m_changing.canvas ] ) );
	}else{   // spectrum
		range = ( m_sync ? &m_syncSpecRange : &( m_rangeMap[ m_changing.canvas ] ) );
	}

	return range;
}

// get range information
ProfileRangeManager::RangeInfo* ProfileRangeManager::getRange(wxWindow* canvas) {
	// get range
	RangeInfo* range = NULL;
	
	// 拡大、縮小の処理をChildFrameだけでなくCanvasにも反映させる為の変更 @date 2013.04.03 <Add> M.Izumi
	kome::window::ChildFrame* frame = ( canvas == NULL ? NULL : dynamic_cast< kome::window::ChildFrame* >( canvas->GetParent() ));
	if( frame == NULL ){ // ChildFrame 以外のキャンバスの表示範囲取得
		range = &( m_rangeMap[ canvas ] );
		return range;
	}
	
	if( m_changing.chromFlag ) {  // chromatogram
		range = ( m_sync ? &m_syncChromRange : &( m_rangeMap[ canvas ] ) );
		
	}else{   // spectrum
		range = ( m_sync ? &m_syncSpecRange : &( m_rangeMap[ canvas ] ) );
	}

	return range;
}

// set down button information
bool ProfileRangeManager::setDownButtonInfo(
		kome::evt::MouseEvent& evt,
		kome::img::Graphics& graphics
) {
	bool setNull = true;
	bool refresh = false;

	double x = double();
	double y = double();
	graphics.transformPositionToData( evt.x(), evt.y(), &x, &y );

	if( evt.x() >= graphics.getGraphPositionLeft()
			&& evt.x() <= graphics.getGraphPositionRight()
			&& evt.y() > graphics.getGraphPositionBottom()
			&& evt.y() <= (int)graphics.getHeight()
	) {	// x range
		// start selecting range
		m_changing.startPos  = evt.x();
		m_changing.endPos	= evt.x();
		m_changing.yAxisFlag = false;

		setNull = false;
		refresh = true;		

		// マウスカーソル設定
		if( evt.lbutton() ){
			setMouseCursor( false );
		}else if( evt.rbutton() ){
			setMouseCursor( true );
		}
	}
	else if ( evt.x() >= 0
			&& evt.x() < graphics.getGraphPositionLeft()
			&& evt.y() >= graphics.getGraphPositionTop()
			&& evt.y() <= graphics.getGraphPositionBottom()
	) {	// y range
		// start selecting range
		m_changing.startPos  = evt.y();
		m_changing.endPos	= evt.y();
		m_changing.yAxisFlag = true;

		setNull = false;
		refresh = true;

		// マウスカーソル設定
		if( evt.lbutton() ){
			setMouseCursor( false );
		}else if( evt.rbutton() ){
			setMouseCursor( true );
		}
	}

	// unset active object
	if( setNull ) {
		m_changing.canvas = NULL;
	}

	return refresh;
}

// move mouse (spectrum)
bool ProfileRangeManager::moveMouse(
		kome::objects::Spectrum& spec,		
		kome::evt::MouseEvent& evt,
		kome::img::Graphics& graphics,
		wxWindow* canvas
) {
	return setMoveMouseInfo( evt, graphics );
}

// move mouse (chromatogram)
bool ProfileRangeManager::moveMouse(
		kome::objects::Chromatogram& chrom,		
		kome::evt::MouseEvent& evt,
		kome::img::Graphics& graphics,
		wxWindow* canvas
) {

	return setMoveMouseInfo( evt, graphics );
}

// set move mouse information
bool ProfileRangeManager::setMoveMouseInfo(
		kome::evt::MouseEvent& evt,
		kome::img::Graphics& graphics
) {
	// get position
	int endPos = m_changing.endPos;
	int iePos = m_changing.endPos;
	
	// y position
	if( m_changing.yAxisFlag ) {  
		endPos = CLAMP( evt.y(), graphics.getGraphPositionTop(), graphics.getGraphPositionBottom() );
	}
	 // x position
	else { 
		endPos = CLAMP( evt.x(), graphics.getGraphPositionLeft(), graphics.getGraphPositionRight() );
	}
	
	m_changing.endPos = endPos;
		
	// get range information
	RangeInfo* rangeInfo = getRange(m_changing.canvas);
	if( rangeInfo == NULL ) {
		return false;
	}
		
	bool refresh = false;
		
	// X軸制御領域
	if( evt.x() >= graphics.getGraphPositionLeft()
			&& evt.x() <= graphics.getGraphPositionRight()
			&& evt.y() > graphics.getGraphPositionBottom()
			&& evt.y() <= (int)graphics.getHeight()
	) {	// x range

		// マウスカーソル設定
		// 左ドラッグ  Xパン処理
		if( evt.lbutton() ){
			refresh = TranseProc( rangeInfo,  graphics );	// kome::img::Graphics追加 @date 2011.12.23 <Mod> M.Izumi
			setMouseCursor( false );
		// 右ドラッグ　X連続ズーム処理
		}else if( evt.rbutton() ){
			refresh = ZoomProc( graphics, rangeInfo, iePos );	
			setMouseCursor( true );
		}
	// Y軸制御領域 
	}else
	if ( evt.x() >= 0
			&& evt.x() < graphics.getGraphPositionLeft()
			&& evt.y() >= graphics.getGraphPositionTop()
			&& evt.y() <= graphics.getGraphPositionBottom()
	) {	

		// マウスカーソル設定
		// 左ドラッグ Yパン処理
		if( evt.lbutton() ){
			refresh = TranseProc( rangeInfo, graphics );		// kome::img::Graphics追加 @date 2011.12.23 <Mod> M.Izumi
			setMouseCursor( false );
		// 右ドラッグ Y連続ズーム処理
		}else if( evt.rbutton() ){
			setMouseCursor( true );
			refresh = ZoomProc( graphics,rangeInfo,  iePos );	
		}
	}
	
	return refresh;
}

// up button (spectrum)
bool ProfileRangeManager::upButton(
		kome::objects::Spectrum& spec,		
		kome::evt::MouseEvent& evt,
		kome::img::Graphics& graphics,
		wxWindow* canvas
) {
// >>>>>>	@Date:2013/06/14	<Modify>	A.Ozaki
// v.2.4.4時代のロジックに戻して、拡大処理しているときだけ
// OperationLogに登録するように変更します
//
	if  ( m_changing.canvas != NULL && (m_changing.startPos != m_changing.endPos) )
	{
		ProfileRangeOperation* opt = new ProfileRangeOperation();
		opt->setShortName("spec_range");
		opt->setOperationName( "Change Displaying Range" );
		opt->setOperationType( kome::operation::Operation::TYPE_STANDARD );
		opt->setTargetSample( spec.getSample() );
		opt->setSpec( &spec );
		opt->setCanvas( canvas );
		
		RangeInfo* rangeInfo = getRange( canvas );
		opt->setRangeInfo( rangeInfo );
		
		int index = -1;
		// y pos
		if( m_changing.yAxisFlag ){
			index = 1;
		}else{
			index = 0;
		}
		opt->setIndex( index );

		if( !rangeInfo->xRange.empty() ){
			opt->setMinX( rangeInfo->xRange.top().first );
			opt->setMaxX( rangeInfo->xRange.top().second );
		}
		if( !rangeInfo->yRange.empty() ){
			opt->setMinY( rangeInfo->yRange.top().first );
			opt->setMaxY( rangeInfo->yRange.top().second );
		}

		if( opt->execute() ){
			kome::window::WindowTool::refresh();
		}else{
			delete opt;
		}
	}

	return	setUpButtonInfo( evt, graphics, canvas );	
// <<<<<<	@Date:2013/06/14	<Modify>	A.Ozaki
}

// up button (chromatogram)
bool ProfileRangeManager::upButton(
		kome::objects::Chromatogram& chrom,
		kome::evt::MouseEvent& evt,
		kome::img::Graphics& graphics,
		wxWindow* canvas
) {
// >>>>>>	@Date:2013/06/14	<Modify>	A.Ozaki
// v.2.4.4時代のロジックに戻して、拡大処理しているときだけ
// OperationLogに登録するように変更します
//
	if  ( m_changing.canvas != NULL && (m_changing.startPos != m_changing.endPos) )
	{
		ProfileRangeOperation* opt = new ProfileRangeOperation();
		opt->setShortName("chrom_range");
		opt->setOperationName( "Change Displaying Range" );
		opt->setOperationType( kome::operation::Operation::TYPE_STANDARD );
		opt->setTargetSample( chrom.getSample() );
		opt->setChrom( &chrom );
		opt->setCanvas( canvas );
		
		RangeInfo* rangeInfo = getRange( canvas );
		opt->setRangeInfo( rangeInfo );

		int index = -1;
		// y pos
		if( m_changing.yAxisFlag ){
			index = 1;			
		}else{
			index = 0;
		}
		opt->setIndex( index );

		if( !rangeInfo->xRange.empty() ){
			opt->setMinX( rangeInfo->xRange.top().first );
			opt->setMaxX( rangeInfo->xRange.top().second );
		}
		if( !rangeInfo->yRange.empty() ){
			opt->setMinY( rangeInfo->yRange.top().first );
			opt->setMaxY( rangeInfo->yRange.top().second );
		}
		if( opt->execute() ){
			kome::window::WindowTool::refresh();
		}else{
			delete opt;
		}
	}

	return	setUpButtonInfo( evt, graphics, canvas );	
// <<<<<<	@Date:2013/06/14	<Modify>	A.Ozaki
}

// set up button information
bool ProfileRangeManager::setUpButtonInfo(
		kome::evt::MouseEvent& evt,
		kome::img::Graphics& graphics,
		wxWindow* canvas
) {
	bool refresh = false;
	
	// X軸制御領域
	if( evt.x() >= graphics.getGraphPositionLeft()
			&& evt.x() <= graphics.getGraphPositionRight()
			&& evt.y() > graphics.getGraphPositionBottom()
			&& evt.y() <= (int)graphics.getHeight()
	) {	
		refresh = true;
	// Y軸制御領域 
	}else if ( evt.x() >= 0
			&& evt.x() < graphics.getGraphPositionLeft()
			&& evt.y() >= graphics.getGraphPositionTop()
			&& evt.y() <= graphics.getGraphPositionBottom()
	) {	
		refresh = true;
	}
			
	if( canvas != NULL ){
		canvas->SetCursor(wxCursor(wxCURSOR_ARROW)); // 矢印
	}else{
		if( m_range != NULL ){
			if( m_range->canvas != NULL ){
				m_range->canvas->SetCursor(wxCursor(wxCURSOR_ARROW)); // 矢印
			}else{
				refresh = false;
			}
		}
	}

// >>>>>>	@Date:2013/06/14	<Modify>	A.Ozaki
// v.2.4.4時代のロジックに戻します
	m_changing.canvas = NULL;
	m_changing.startPos = -1;
	m_changing.endPos = -1;	
// <<<<<<	@Date:2013/06/14	<Modify>	A.Ozaki
	return refresh;
}

// double click (spectrum)
bool ProfileRangeManager::dblClick(
		kome::objects::Spectrum& spec,
		kome::evt::MouseEvent& evt,
		kome::img::Graphics& graphics,
		wxWindow* canvas
) {	
	// >>>>>>	@Date:2013/06/14	<Add>	A.Ozaki
	// 右ボタンのダブルクリックではリセットされないので、
	// 処理を除外します
	if  ( evt.rbutton( ) )
	{
		return	false;
	}
	// <<<<<<	@Date:2013/06/14	<Add>	A.Ozaki

	// get range information
	RangeInfo* rangeInfo = getRange( canvas ); // @date 2013.04.03 <Mod> M.Izumi
	if( rangeInfo == NULL ){
		return false;
	}
	int index = getIndex( evt, graphics );

	// add operation
	ProfileRangeOperation* opt = new ProfileRangeOperation();
	opt->setShortName("spec_range");
	opt->setOperationName( "Change Displaying Range" );
	opt->setOperationType( kome::operation::Operation::TYPE_STANDARD );
	opt->setTargetSample( spec.getSample() );
	opt->setSpec( &spec );
	opt->setCanvas( canvas );
	opt->setRangeInfo( rangeInfo );
	opt->setbReset( true );
	opt->setIndex( index );

	if( opt->execute() ){
		kome::window::WindowTool::refresh();
	}else{
		delete opt;
		
		return false;
	}

	return true;
}

// double click (chromatogram)
bool ProfileRangeManager::dblClick(
		kome::objects::Chromatogram& chrom,
		kome::evt::MouseEvent& evt,
		kome::img::Graphics& graphics,
		wxWindow* canvas
) {
	// >>>>>>	@Date:2013/06/14	<Add>	A.Ozaki
	// 右ボタンのダブルクリックではリセットされないので、
	// 処理を除外します
	if  ( evt.rbutton( ) )
	{
		return	false;
	}
	// <<<<<<	@Date:2013/06/14	<Add>	A.Ozaki

	// get range information
	RangeInfo* rangeInfo = getRange( canvas ); // @date 2013.04.03 <Mod> M.Izumi
	if( rangeInfo == NULL ){
		return false;
	}

	int index = getIndex( evt, graphics );

	// add operation
	ProfileRangeOperation* opt = new ProfileRangeOperation();
	opt->setShortName("chrom_range");
	opt->setOperationName( "Change Displaying Range" );
	opt->setOperationType( kome::operation::Operation::TYPE_STANDARD );
	opt->setTargetSample( chrom.getSample() );
	opt->setChrom( &chrom );
	opt->setCanvas( canvas );
	opt->setRangeInfo( rangeInfo );
	opt->setbReset( true );
	opt->setIndex( index );

	if( opt->execute() ){
		kome::window::WindowTool::refresh();
	}else{
		delete opt;
		
		return false;
	}

	return true;
}

int ProfileRangeManager::getIndex( kome::evt::MouseEvent& evt, kome::img::Graphics& graphics ){
	int index = -1;
	// check button
	if( !evt.lbutton() ) {
		return index;
	}

	if( evt.x() >= graphics.getGraphPositionLeft()
			&& evt.x() <= graphics.getGraphPositionRight()
			&& evt.y() > graphics.getGraphPositionBottom()
			&& evt.y() <= (int)graphics.getHeight()
	) {	// x range
		index = 0;
	}
	else if ( evt.x() >= 0
			&& evt.x() < graphics.getGraphPositionLeft()
			&& evt.y() >= graphics.getGraphPositionTop()
			&& evt.y() <= graphics.getGraphPositionBottom()
	) {		// y range
		index = 1;
	// グラフ制御領域
	}else{
		index = 2;
	}

	return index;
}

// Any Keybord Down(spectrum)
bool ProfileRangeManager::keyDown(
	kome::evt::KeyEvent& evt,
	kome::img::Graphics& graphics,
	wxWindow* canvas
) {
	// range
	double startX = graphics.getMinX();
	double endX = graphics.getMaxX();
	double xRange = std::max( X_MINIMUM_RANGE, endX - startX );
	double startY = graphics.getMinY();
	double endY = graphics.getMaxY();
	double yRange = std::max( Y_MINIMUM_RANGE, endY - startY );

	RangeInfo* range = getRange( canvas );
	if( range == NULL ) {
		return false;
	}

	// min & max
	kome::objects::PointsManager& ptMgr = kome::objects::PointsManager::getInstance();
	double minX = 0.0;
	double maxX = 1000000.0;
	double minY = - FLT_MAX;
	double maxY = FLT_MAX;

	kome::window::SpectrumCanvas* specCanvas = dynamic_cast< kome::window::SpectrumCanvas* >( canvas );
	if( specCanvas != NULL ) {
		kome::objects::Spectrum* spec = specCanvas->getBaseSpectrum();
		if( spec != NULL ){
			kome::core::XYData* xyData = ptMgr.getXYData( spec );
			minX = spec->getMinX();
			maxX = std::max( spec->getMaxX(), minX + X_MINIMUM_RANGE );
			minY = std::min( 0.0, xyData->getMinY() );
			maxY = std::max( xyData->getMaxY(), minY + Y_MINIMUM_RANGE );
		}
	}

	kome::window::ChromatogramCanvas* chromCanvas = dynamic_cast< kome::window::ChromatogramCanvas* >( canvas );
	if( chromCanvas != NULL ) {
		kome::objects::Chromatogram* chrom = chromCanvas->getBaseChromatogram();
		if( chrom != NULL ){
			kome::core::XYData* xyData = ptMgr.getXYData( chrom );
			minX = xyData->getMinX();
			maxX = std::max( xyData->getMaxX(), minX + X_MINIMUM_RANGE );
			minY = std::min( 0.0, xyData->getMinY() );
			maxY = std::max( xyData->getMaxY(), minY + Y_MINIMUM_RANGE );
		}
	}

	// alpha
	double alpha = 0.1;
	if( evt.ctrl() ) {
		alpha = 0.01;
	}

	// key board
	bool xFlg = false;
	bool yFlg = false;
	if( evt.getCode() == kome::core::key::KEY_RIGHT ) {    // right
		if( evt.shift() ) {    // zoom
			double midX = ( startX + endX ) / 2.0;
			xRange = std::max( X_MINIMUM_RANGE, ( 1.0 - alpha ) * xRange );
			startX = std::max( minX, midX - xRange / 2.0 );
		}
		else {    // pan
			startX = startX + xRange * alpha;
		}
		endX = startX + xRange, startX + xRange;
		xFlg = true;
	}
	else if( evt.getCode() == kome::core::key::KEY_LEFT ) {    // left
		if( evt.shift() ) {    // zoom
			double midX = ( startX + endX ) / 2.0;
			xRange = ( 1.0 + alpha ) * xRange;
			startX = std::max( minX, midX - xRange / 2.0 );
		}
		else {    // pan
			startX = std::max( minX, startX - xRange * alpha );
		}
		endX = startX + xRange;
		xFlg = true;
	}
	else if( evt.getCode() == kome::core::key::KEY_UP ) {    // up
		if( evt.shift() ) {    // zoom
			yRange = std::max( Y_MINIMUM_RANGE, ( 1.0 - alpha ) * yRange );
		}
		else {    // pan
			startY = startY + yRange * alpha;
		}
		endY = startY + yRange;
		yFlg = true;			
	}
	else if( evt.getCode() == kome::core::key::KEY_DOWN ) {    // down
		if( evt.shift() ) {    // zoom
			yRange = ( 1.0 + alpha ) * yRange;
		}
		else {    // pan
			startY = std::max( minY, startY - yRange * alpha );
		}
		endY = startY + yRange;
		yFlg = true;
	}

	// update range
	if( xFlg ) {
		if( endX > maxX ) {
			endX = maxX;
			startX = endX - xRange;
			if( startX < minX ) {
				startX = minX;
			}
		}

		if( range->xRange.empty() ) {
			range->xRange.push( std::make_pair( 0.0, 0.0 ) );
		}
		range->xRange.top() = std::make_pair( startX, endX );
		return true;
	}
	if( yFlg ) {
		if( endY > maxY ) {
			endY = maxY;
			startY = endY - yRange;
			if( startY < minY ) {
				startY = minY;
			}
		}

		if( range->yRange.empty() ) {
			range->yRange.push( std::make_pair( 0.0, 0.0 ) );
		}
		range->yRange.top() = std::make_pair( startY, endY );
		return true;
	}

	return false;
}

// Key Event Controler
bool ProfileRangeManager::KeyEvtControler(
	kome::evt::KeyEvent& evt,
	kome::img::Graphics& graphics,
	wxWindow* canvas
) {
	return false;
}

// ReSize Window
bool ProfileRangeManager::KeyEvtReSizeWindow(
	kome::evt::KeyEvent& evt,
	kome::img::Graphics& graphics,
	wxWindow* canvas
) {
	return false;
}

// move Window
bool ProfileRangeManager::KeyEvtMoveWindow(
	kome::evt::KeyEvent& evt,
	kome::img::Graphics& graphics,
	wxWindow* canvas
) {
	return false;	
}

// draw foreground (spectrum)
void ProfileRangeManager::drawForeground(
		kome::objects::Spectrum& spec,
		kome::img::Graphics& graphics,
		wxWindow* canvas
) {
	// check the object
	if( m_changing.chromFlag
			|| m_changing.canvas != canvas ) {
		return;
	}
	// draw
	drawForeground( graphics );
}

// draw foreground (chromatogram)
void ProfileRangeManager::drawForeground(
		kome::objects::Chromatogram& chrom,
		kome::img::Graphics& graphics,
		wxWindow* canvas
) {
	// check the object
	if( !m_changing.chromFlag
			|| m_changing.canvas != canvas ) {
		return;
	}

	// draw
	drawForeground( graphics );
}

// draw foreground
void ProfileRangeManager::drawForeground( kome::img::Graphics& graphics ) {
	return;
}

// draw background (spectrum)
void ProfileRangeManager::drawBackground(
		kome::objects::Spectrum& spec,
		kome::img::Graphics& graphics,
		wxWindow* canvas
) {
	// check the object
	if( m_changing.chromFlag
			|| m_changing.canvas != canvas ) {
		return;
	}

	// draw
	drawBackground( graphics );
}

// draw background (chromatogram)
void ProfileRangeManager::drawBackground(
		kome::objects::Chromatogram& chrom,
		kome::img::Graphics& graphics,
		wxWindow* canvas
) {
	// check the object
	if( !m_changing.chromFlag
			|| m_changing.canvas != canvas ) {
		return;
	}

	// draw
	drawBackground( graphics );
}

// draw background
void ProfileRangeManager::drawBackground( kome::img::Graphics& graphics ) {
	return;
}

// toggle chromatogram range status
void ProfileRangeManager::toggleRangeStatus() {
	// toggle flag
	m_sync = !m_sync;

	// set status
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
	statusMgr.setValue( RANGE_STATUS_KEY,  m_sync ? "on" : "off" );

	// range stack
	if( m_sync ) {		// synchronized
		
		clearStack();

		// copy statck
		kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();
		kome::window::Canvas* canvas = NULL;
		if( frame != NULL ) {
			canvas = dynamic_cast< kome::window::Canvas* >( frame->getCanvas() );
		}
		if( canvas != NULL ) {
			kome::objects::MsDataVariant obj( frame->getActiveObject() );
			kome::objects::Chromatogram* chrom = obj.getChromatogram();
			kome::objects::Spectrum* spec = obj.getSpectrum();
			if( chrom != NULL ) {
				m_syncChromRange = m_rangeMap[ canvas ];
			}else if( spec != NULL ){
				m_syncSpecRange = m_rangeMap[ canvas ];
			}
		}		
	}
	else {
		// copy stack
		unsigned int num = kome::window::ChildFrame::getNumberOfFrames();
		for( unsigned int i = 0; i < num; i++ ) {	// each frames
			kome::window::ChildFrame* frame = kome::window::ChildFrame::getFrame( i );

			// canvas
			kome::window::Canvas* canvas = dynamic_cast< kome::window::Canvas* >( frame->getCanvas() );
			if( canvas != NULL ) {
				// active object
				kome::objects::MsDataVariant obj( canvas->getActiveObject() );
				kome::objects::Chromatogram* chrom = obj.getChromatogram();
				kome::objects::Spectrum* spec = obj.getSpectrum();
				// set range
				if( chrom != NULL ) {
					m_rangeMap[ canvas ] = m_syncChromRange;
				}else if( spec != NULL ){
					m_rangeMap[ canvas ] = m_syncSpecRange;	// @date 2011.02.14 <Mod> M.Izumi
				}
			}
		}		
	}
	
	kome::window::StatusBar& statusBar = kome::window::StatusBar::getInstance(); 

	if( !m_sync ){
		statusBar.setFlgDisp( STATUSBAR_SYNC, 0 );
	}else{
		statusBar.setFlgDisp( STATUSBAR_SYNC, 1 );
		statusBar.setStatusMsg(STATUSBAR_SYNC, MODE_SYNC ); // ステータスバーに状態表示	
	}
	// refresh
	kome::window::WindowTool::refresh();
}

// push spectrum x range
void ProfileRangeManager::pushSpectrumXRange( RangeInfo* rangeInfo, wxWindow* canvas, const double start, const double end ) {
	if( rangeInfo != NULL ){
		rangeInfo->xRange.push( std::make_pair( start, end ) );
	}
}

// push spectrum y range
void ProfileRangeManager::pushSpectrumYRange( RangeInfo* rangeInfo, wxWindow* canvas, const double start, const double end ) {
	if( rangeInfo != NULL ){
		rangeInfo->yRange.push( std::make_pair( start, end ) );
	}
}

// push chromatogram x range
void ProfileRangeManager::pushChromatogramXRange( RangeInfo* rangeInfo, wxWindow* canvas, const double start, const double end ) {
	if( rangeInfo != NULL ){
		rangeInfo->xRange.push( std::make_pair( start, end ) );
	}
}

// push chromatogram y range
void ProfileRangeManager::pushChromatogramYRange( RangeInfo* rangeInfo, wxWindow* canvas, const double start, const double end ) {
	if( rangeInfo != NULL ){
		rangeInfo->yRange.push( std::make_pair( start, end ) );
	}
}

// get object
ProfileRangeManager& ProfileRangeManager::getInstance() {
	
	static ProfileRangeManager mgr;

	return mgr;
}

// x/y軸制御領域 ステータスバーの設定
bool ProfileRangeManager::setValStatusBar(
	kome::evt::MouseEvent& evt,
	kome::img::Graphics& graphics,
	wxWindow* canvas
){
	kome::window::StatusBar& statusBar = kome::window::StatusBar::getInstance(); 
	
	statusBar.setFlgDisp( "coordinate", 1 );	// ステータスバーを有効化
	
	std::string strDisp ="";

	// position
	int lp = graphics.getGraphPositionLeft();
	int rp = graphics.getGraphPositionRight();
	int tp = graphics.getGraphPositionTop();
	int bp = graphics.getGraphPositionBottom();
	
	int cx = evt.x();
	int cy = evt.y();

	cx = CLAMP( cx, lp, rp );
	cy = CLAMP( cy, tp, bp );

	double x1 = double();
	double y1 = double();

	// Mouse Move　マウスボタンが押されていない
	if( !evt.lbutton() && !evt.rbutton() ){		
		// x range
		if( (evt.x() >= lp
			&& evt.x() <= rp
			&& evt.y() > bp
			&& evt.y() <= (int)graphics.getHeight())

		) {	
			graphics.transformPositionToData( (double)cx, (double)cy, &x1, &y1 );
			strDisp = (boost::format("%.2f") % x1 ).str();

		// y range
		}else if ( evt.x() >= 0
			&& evt.x() < lp
			&& evt.y() >= tp
			&& evt.y() <= bp
		) {	
			graphics.transformPositionToData( (double)cx, (double)cy, &x1, &y1 );
			strDisp = (boost::format("%.2f") % y1 ).str();

		}
	}
	
	// ステータスバーに表示するメッセージをセット
	statusBar.setStatusMsg( "coordinate", strDisp.c_str() );
	
	return false;
}

// clear stack
void ProfileRangeManager::clearStack()
{
	// Chromatogram	
	while( !m_syncChromRange.xRange.empty() ) {
		m_syncChromRange.xRange.pop();
	}
	while( !m_syncChromRange.yRange.empty() ) {
		m_syncChromRange.yRange.pop();
	}

	// Spectrum

	while( !m_syncSpecRange.xRange.empty() ) {
		m_syncSpecRange.xRange.pop();
	}
	while( !m_syncSpecRange.yRange.empty() ) {
		m_syncSpecRange.yRange.pop();
	}
}

// x/y軸のパン・ズーム処理のステータスバー表示
void ProfileRangeManager::setRangeStatusMsg( double min, double max ){
}

// パン処理
bool ProfileRangeManager::TranseProc( RangeInfo* rangeInfo, kome::img::Graphics& graphics ){
	
	bool refresh = false;
	double min;
	double max;

	if( !m_changing.yAxisFlag ){// xrange
		if( !calcXRange( graphics, rangeInfo, min, max ) ){
			return false;
		}

		if( !rangeInfo->xRange.empty() ){
			rangeInfo->xRange.pop();
		}
		
		rangeInfo->xRange.push(std::make_pair( min, max ));
				
		refresh = true;
		
	}else{// yrange
		
		if( !calcYRange( graphics, rangeInfo, min, max ) ){
			return false;
		}

		if( !rangeInfo->yRange.empty() ){
			rangeInfo->yRange.pop();
		}
				
		rangeInfo->yRange.push(std::make_pair( min, max ));
		refresh = true;
	}
	
	// ステータスバーに範囲表示
	setRangeStatusMsg( min, max );
	return refresh;
}

// calc x range ( pan )
bool ProfileRangeManager::calcXRange( kome::img::Graphics& graphics, RangeInfo* rangeInfo, double&min, double& max ){
	bool bflg;

	if(rangeInfo->xRange.size() == 0){
		return false; // No Need Replase
	}

	std::pair< double, double > orgPt = rangeInfo->xRange.top();		
	min = orgPt.first;
	max = orgPt.second;
	// false: 左に移動   true:　右に移動
	bflg = false;
	if( m_changing.startPos < m_changing.endPos ){
		bflg = true;
	}
	// 新しい計算式		@date 2011.12.23 <Mod> M.Izumi
	double W = (double)graphics.getGraphWidth();// Spec No.87544 @date 2012.10.10 <Mod> M.Izumi
	double d =  (double)m_changing.startPos - (double)m_changing.endPos;
	double Wm = max - min;
		
	double resulte = ( d * Wm ) / W;
			
	m_changing.startPos = m_changing.endPos;

	if( m_range == NULL ){
		return false;
	}
			
	// Replase
	min += resulte;
	max += resulte;
	
	if( bflg ){//右に移動
		if( (min - resulte) <= m_range->minX ){
			return false; // No Need Replase
		}
	}else{
		if( (max + resulte) >= m_range->maxX ){
			return false; // No Need Replase
		}
	}
	return true;
}

// calc y range ( pan )
bool ProfileRangeManager::calcYRange( kome::img::Graphics& graphics, RangeInfo* rangeInfo, double&min, double& max ){
	bool bflg;

	if(rangeInfo->yRange.size() == 0){
		return false; // No Need Replase
	}

	std::pair< double, double > orgPt = rangeInfo->yRange.top();		
	min = orgPt.first;
	max = orgPt.second;

	// true:下に移動   false:上に移動
	bflg = false;
	if( m_changing.startPos > m_changing.endPos ){
		bflg = true;
	}
	
	// 新しい計算式		@date 2011.12.23 <Mod> M.Izumi
	double H = (double)graphics.getGraphHeight(); // Spec No.87544 @date 2012.10.10 <Mod> M.Izumi

	double d =  (double)m_changing.startPos - (double)m_changing.endPos;
	double Hm =  min - max;
		
	double resulte = ( d * Hm ) / H;
			
	m_changing.startPos = m_changing.endPos;
		
	if( m_range == NULL ){
		return false;
	}
				
	// Replase
	min += resulte;
	max += resulte;

	if( bflg ){
		if( (min - resulte) <= m_range->minY ){
			return false; // No Need Replase
		}
	}else{
		if( (max + resulte) >= m_range->maxY ){
			return false; // No Need Replase
		}
	}
	return true;
}

// ズーム処理
bool ProfileRangeManager::ZoomProc( kome::img::Graphics& graphics, RangeInfo* rangeInfo,  int iePos ){
	bool refresh = false;
	
	double minPt;
	double maxPt;
		
	if( !m_changing.yAxisFlag ){ // xrange
	
		if( !calcXRange( graphics, minPt, maxPt, iePos ) ){
			return false;
		}
		
		if( !rangeInfo->xRange.empty() ){
			rangeInfo->xRange.pop();
		}
		
		rangeInfo->xRange.push(std::make_pair( minPt, maxPt ));
		
		refresh = true;
	}else{ // yrange
		
		if( !calcYRange( graphics, minPt, maxPt, iePos ) ){
			return false;
		}
				
		if( !rangeInfo->yRange.empty() ){
			rangeInfo->yRange.pop();
		}
		
		rangeInfo->yRange.push(std::make_pair( minPt, maxPt ));
			
		refresh =  true;	
	}
		
	// ステータスバーに範囲表示
	setRangeStatusMsg( minPt, maxPt );
			
	return refresh;
}

// calc x range ( zoom )
bool ProfileRangeManager::calcXRange( kome::img::Graphics& graphics, double& minPt, double& maxPt, int iePos ){
	minPt = graphics.getMinX();
	maxPt = graphics.getMaxX();
				
	// 新しい計算式		@date 2011.12.23 <Mod> M.Izumi
	// get range
	double sPos = (double)m_changing.startPos;
	double ePos = (double)m_changing.endPos;
		
	double py = double();
	double startPt = double();
	double endPt = double();

	graphics.transformPositionToData( sPos, 0.0, &startPt, &py );
	graphics.transformPositionToData( ePos, 0.0, &endPt, &py );
		
	double a = 0.0;
	double lstart = startPt - minPt;
	double lend = maxPt - startPt;
		
	double dx( m_changing.endPos - iePos );
		
	a = pow( 1 + 0.08, -(double)dx );
			
	minPt = startPt - ( a * lstart );
	maxPt = startPt + ( a * lend );		
			
	// Spec No.87546 @date 2012.10.10 <Add> M.Izumi
	if( m_changing.startPos < m_changing.endPos ){
		if( (maxPt-minPt) < 0.001 ){
			return false;
		}
	}
	if( m_range == NULL ){
		return false;
	}

	if( minPt < m_range->minX ){// over Minimum point
		double lossRange = m_range->minX  - minPt;
		minPt = m_range->minX;
		maxPt += lossRange;
	}else if( maxPt > m_range->maxX ){// over Maximum point
		double lossRange = maxPt - m_range->maxX;
		maxPt = m_range->maxX;
		minPt -= lossRange;			
	}
				
	if( minPt < m_range->minX ){
		minPt = m_range->minX;
	}else if( maxPt > m_range->maxX ){
		maxPt = m_range->maxX;
	}

	return true;
}

// calc y range (zoom)
bool ProfileRangeManager::calcYRange( kome::img::Graphics& graphics, double& minPt, double& maxPt, int iePos ){
	minPt = graphics.getMinY();
	maxPt = graphics.getMaxY();
			
	// 新しい計算式		@date 2011.12.23 <Mod> M.Izumi
	// get range
	double sPos = (double)m_changing.startPos;
	double ePos = (double)m_changing.endPos;

	double px = double();
	double startPt = double();
	double endPt = double();

	graphics.transformPositionToData( 0.0, sPos, &px, &startPt );
	graphics.transformPositionToData( 0.0, ePos, &px, &endPt );
		
	double a = 0.0;
	double lstart = startPt - minPt;
	double lend = maxPt - startPt;
		
	double dx( m_changing.endPos - iePos );
		
	a = pow( 1 + 0.08, (double)dx );

	minPt = startPt - ( a * lstart );
	maxPt = startPt + ( a * lend );		

	// Spec No.87546 @date 2012.10.10 <Add> M.Izumi
	if( m_changing.startPos > m_changing.endPos ){
		if( (maxPt-minPt) < 0.01 ){
			return false;
		}
	}

	if( m_range == NULL ){
		return false;
	}

	if( minPt < m_range->minY ){// over Minimum point
		double lossRange = m_range->minY  - minPt;
		minPt = m_range->minY;
		maxPt += lossRange;
	}else if( maxPt > m_range->maxY ){// over Maximum point
		double lossRange = maxPt - m_range->maxY;
		maxPt = m_range->maxY;
		minPt -= lossRange;
	}

	if( minPt < m_range->minY ){
		minPt = m_range->minY;
	}else if( maxPt > m_range->maxY ){
		maxPt = m_range->maxY;
	}

	return true;
}

// マウスカーソルのアイコン設定
void ProfileRangeManager::setMouseCursor( bool bbtn ){
	kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();
	kome::window::Canvas* canvas = ( frame == NULL ? NULL : dynamic_cast< kome::window::Canvas* >( frame->getCanvas() ) );
		
	if( canvas == NULL ){
		return;
	}
	
	kome::plugin::PluginManager& pluginMgr = kome::plugin::PluginManager::getInstance();

	kome::plugin::PluginInfo* info = NULL;
	kome::plugin::PluginIcon* icon = NULL;
	
	info = pluginMgr.getPlugin( PROFILE_RANGE );
	if( info == NULL ){
		canvas->SetCursor(wxCursor(wxCURSOR_ARROW)); // 矢印
	}

	if( !m_changing.yAxisFlag ){	// x range
		if( bbtn ){
			icon = info->getIcon( ICON_NAME_ZOOM_X );		
		}else{
			icon = info->getIcon( ICON_NAME_TRANSE_X );
		}
		if( icon != NULL ){
			wxBitmap bitmap( wxT(icon->getPath()), wxBITMAP_TYPE_PNG );
			wxImage img( bitmap.ConvertToImage() );
		
			// set hotspot 
			int width = img.GetWidth();
			int height = img.GetHeight();
			img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, width/2);
			img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, height/2);
			
			// set cursor
			canvas->SetCursor(wxCursor(img));
		}else{
			canvas->SetCursor(wxCursor(wxCURSOR_ARROW)); // 矢印
		}
	}else {							// y range
		if( bbtn ){
			icon = info->getIcon( ICON_NAME_ZOOM_Y );	
		}else{
			icon = info->getIcon( ICON_NAME_TRANSE_Y );
		}
		if( icon != NULL ){
			wxBitmap bitmap( wxT(icon->getPath()), wxBITMAP_TYPE_PNG );
			wxImage img( bitmap.ConvertToImage() );
		
			// set cursor
			canvas->SetCursor(wxCursor(img));
		}else{
			canvas->SetCursor(wxCursor(wxCURSOR_ARROW)); // 矢印
		}
	}

}

//2012/02/28 add by t.okuno -> This plugin displays a range choice screen.
// creates Change Displayed Spectrum Range dialog
void ProfileRangeManager::createChangeSpectrumRange(
	kome::objects::Spectrum* spec,
	wxWindow* canvas,
	const double startMz,
	const double endMz
){
	if( m_pdlgChangeDisplayedRangeMz == NULL ){
		m_pdlgChangeDisplayedRangeMz = new ChangeDisplayedRangeDialog( kome::window::WindowTool::getMainWindow(), spec );
	}
	m_pdlgChangeDisplayedRangeMz->setStartPos( startMz );
	m_pdlgChangeDisplayedRangeMz->setEndPos( endMz );
	m_pdlgChangeDisplayedRangeMz->setSpec( spec );
	m_pdlgChangeDisplayedRangeMz->setCanvas( canvas );
	//2012/03/07 upd by t.okuno -> change dialog type. (Show -> ShowModal)

	if( m_pdlgChangeDisplayedRangeMz->ShowModal() == wxID_OK ){
		// add operation
		ProfileRangeOperation* opt = new ProfileRangeOperation();
		opt->setShortName("spec_range");
		opt->setOperationName( "Change Displaying Range" );
		opt->setOperationType( kome::operation::Operation::TYPE_STANDARD );
		opt->setTargetSample( spec->getSample() );
		opt->setSpec( spec );
		opt->setCanvas( canvas );
		opt->setIndex( 0 );
// >>>>>>	@Date:2013/05/30	<Modify>	A.Ozaki
// 開始、終了点は、ダイアログから取得します
		opt->setMinX( m_pdlgChangeDisplayedRangeMz->getStartPos( ) );
		opt->setMaxX( m_pdlgChangeDisplayedRangeMz->getEndPos( ) );
// <<<<<<	@Date:2013/05/30	<Modify>	A.Ozaki
		opt->setRangeInfo( getRange( canvas ) ) ;

		if( opt->execute() ){
			kome::window::WindowTool::refresh();
		}else{
			delete opt;
		}
	}
		
	if( m_pdlgChangeDisplayedRangeMz != NULL ){
		m_pdlgChangeDisplayedRangeMz->Destroy();
		delete m_pdlgChangeDisplayedRangeMz;
		m_pdlgChangeDisplayedRangeMz = NULL;
	}
	//2012/03/07 upd by t.okuno <- change dialog type. (Show -> ShowModal)
}

// creates Change Displayed Chromatogram Range dialog
void ProfileRangeManager::createChangeChromatogramRange( kome::objects::Chromatogram* chrom,
														 wxWindow* canvas,
														 const double startRT,
														 const double endRT )
{
	if( m_pdlgChangeDisplayedRangeRT == NULL ){
		m_pdlgChangeDisplayedRangeRT = new ChangeDisplayedRangeDialog( kome::window::WindowTool::getMainWindow(), chrom );
	}
	m_pdlgChangeDisplayedRangeRT->setStartPos( startRT );
	m_pdlgChangeDisplayedRangeRT->setEndPos( endRT );
	m_pdlgChangeDisplayedRangeRT->setChrom( chrom );
	m_pdlgChangeDisplayedRangeRT->setCanvas( canvas );
	//2012/03/07 upd by t.okuno -> change dialog type. (Show -> ShowModal)

	if( m_pdlgChangeDisplayedRangeRT->ShowModal() == wxID_OK ){
		// add operation
		ProfileRangeOperation* opt = new ProfileRangeOperation();
		opt->setShortName("chrom_range");
		opt->setOperationName( "Change Displaying Range" );
		opt->setOperationType( kome::operation::Operation::TYPE_STANDARD );
		opt->setTargetSample( chrom->getSample() );
		opt->setChrom( chrom );
		opt->setCanvas( canvas );
		opt->setIndex( 0 );
// >>>>>>	@Date:2013/05/30	<Modify>	A.Ozaki
// 開始、終了点は、ダイアログから取得します
		opt->setMinX( m_pdlgChangeDisplayedRangeRT->getStartPos( ) );
		opt->setMaxX( m_pdlgChangeDisplayedRangeRT->getEndPos( ) );
// <<<<<<	@Date:2013/05/30	<Modify>	A.Ozaki
		opt->setRangeInfo( getRange( canvas ) ) ;
		
		if( opt->execute() ){
			kome::window::WindowTool::refresh();
		}else{
			delete opt;
		}
	}
	if( m_pdlgChangeDisplayedRangeRT != NULL ){
		m_pdlgChangeDisplayedRangeRT->Destroy();
		delete m_pdlgChangeDisplayedRangeRT;
		m_pdlgChangeDisplayedRangeRT = NULL;
	}
	//2012/03/07 upd by t.okuno <- change dialog type. (Show -> ShowModal)
}
//2012/02/28 add by t.okuno <- This plugin displays a range choice screen.

// get canvas
wxWindow* ProfileRangeManager::getCanvas( kome::objects::Variant obj ){
	// @date 2013.04.03 <Mod> M.Izumi ->
	wxWindow* actWindow = NULL;
	unsigned int inum = kome::window::Canvas::getNumberOfCanvases();
	
	for( unsigned int i=0; i < inum && actWindow == NULL; i++ ){
		kome::window::Canvas* canvas = kome::window::Canvas::getCanvas( i );
			
		if( canvas != NULL && (obj.prim.pt == canvas->getActiveObject().prim.pt )){
			actWindow = canvas;
		}
	}
	wxWindow* canvas = NULL; 
	kome::window::ChildFrame* frame = NULL;
	if( actWindow == NULL ){
		frame = kome::window::ChildFrame::getActiveFrame();
		canvas = ( frame != NULL ? frame->getCanvas() : NULL );
	}else{
		canvas = actWindow;
	}
	// @date 2013.04.03 <Mod> M.Izumi<-
	
	return canvas;
}

// reset zoom
void ProfileRangeManager::resetZoom( RangeInfo& rangeInfo, const int index ){
	
	if( &rangeInfo == NULL ){
		return;
	}
	switch( index ){
	case 0:
		// x range
		while( rangeInfo.xRange.size() > 0 ) {
			rangeInfo.xRange.pop();			
		}
		break;

	case 1:
		// y range
		while( rangeInfo.yRange.size() > 0 ) {
			rangeInfo.yRange.pop();
		}
		break;

	case 2:
		// X軸,Y軸全体表示
		// x range
		while( rangeInfo.xRange.size() > 0 ) {
			rangeInfo.xRange.pop();			
		}
		// y range
		while( rangeInfo.yRange.size() > 0 ) {
			rangeInfo.yRange.pop();
		}
		break;

	default:		
		break;
	}
	
	kome::window::WindowTool::refresh();
}
