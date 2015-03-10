/**
 * @file SampleTreeManager.cpp
 * @brief implements of SampleTreeManager class
 *
 * @author S.Tanaka
 * @date 2008.01.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SampleTreeManager.h"
#include "SampleTreeData.h"
#include "SampleTreeCtrl.h"
#include "SampleNotebook.h"

#include <wx/wx.h>
#include <wx/imaglist.h>


using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define ICON_WIDTH				 16
#define ICON_HEIGHT			     16

#define CLOSE_FOLDER_ICON_NAME	 "folder-close"
#define OPEN_FOLDER_ICON_NAME	 "folder-open"
#define MS1_ICON_NAME			 "MS1"
#define MS2_ICON_NAME			 "MS2"
#define MS3_ICON_NAME			 "MS3"
#define MS4_ICON_NAME			 "MS4"
#define MS5_ICON_NAME			 "MS5"
#define MS6_ICON_NAME			 "MS6"
#define MS7_ICON_NAME			 "MS7"
#define MSN_ICON_NAME			 "MSn"

#define ROOT_ITEM_NAME			 "root"


// constructor
SampleTreeManager::SampleTreeManager() {
	m_book = NULL;
	m_page = -1;
	m_treeActionFlg = false;
}

// destructor
SampleTreeManager::~SampleTreeManager() {
}

// set tree action flag
void SampleTreeManager::setTreeActionFlag( const bool flg ) {
	m_treeActionFlg = flg;
}

// create tree ctrl
void SampleTreeManager::createTreeCtrl( kome::objects::Sample* sample ) {
	// check the parameter
	if( sample == NULL ) {
		return;
	}
	if( getSampleIndex( sample ) >= 0 ) {
		return;
	}
	
	// gets icon manager object
	kome::window::IconManager& iconMgr = kome::window::IconManager::getInstance();

	// create tree
	wxTreeCtrl* tree = new SampleTreeCtrl( m_book );

	// image list
	wxImageList* imageList = iconMgr.createImageList( ICON_WIDTH, ICON_HEIGHT );
	tree->AssignImageList( imageList );

	// add item
	wxTreeItemId item = addTreeItem( tree, sample->getRootDataGroupNode(), NULL );
	tree->Expand( item );

	// activate
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();
	aoMgr.setActiveSample( sample );

	kome::window::ChildFrame* child = kome::window::ChildFrame::getActiveFrame();
	if( child == NULL ) {
		kome::objects::Variant obj;
		obj.type = kome::objects::Variant::DATA_GROUP_NODE;
		obj.prim.pt = sample->getRootDataGroupNode();

		kome::plugin::PluginCallTool::onActivate( obj );
	}

	// @date 2011.08.24 <Add> M.Izumi
	// SPEC 82165:Sample name mismatch (Close Dialog)
	aoMgr.addTabSample( sample );

	// add
	m_samples.push_back( std::make_pair( sample, tree ) );

	// add to the notebook
	if( m_book == NULL ) {
		return;
	}
	int i = m_book->GetPageCount()-1;
	
	if( i < 0 ){
		m_page = 1;
	}else{
		m_page++;
	}

	// file path
	kome::objects::SampleSet* ss = sample->getSampleSet();
	std::string name;
	if( ss != NULL ) {
		name = getfilename( ss->getFilePath() );
	}
	if( name.empty() ) {
		name = sample->getName();
	}
	name = shortenstring( name.c_str(), 15 );

	// tag
	std::string tag = FMT( "[%d]", m_page  );
	aoMgr.setSampleTagName( sample, tag.c_str() );

	m_book->AddPage( tree , FMT("%s %s", tag.c_str(), name.c_str() ) );
	
	i = m_book->GetPageCount()-1;
	m_book->SetSelection(i);

	// refresh
	tree->Update();

	// update titles
	const unsigned int childNum = kome::window::ChildFrame::getNumberOfFrames();
	for( unsigned int i = 0; i < childNum; i++ ) {
		kome::window::ChildFrame* frame = kome::window::ChildFrame::getFrame( i );
		if( frame != NULL ){
			frame->setTitle();
		}
	}
}

// judges whether the tree view is created
bool SampleTreeManager::hasView() {
	return ( m_book != NULL );
}

// release the tree view
void SampleTreeManager::releaseView() {
	m_book = NULL;
}

// update nodes
void SampleTreeManager::updateNodes( kome::objects::DataGroupNode* group ) {
	// check objects
	if( group == NULL ) {
		return;
	}
	
	// ID
	wxTreeItemId id = m_groupMap[ group ];
	wxTreeCtrl* tree = getTreeCtrl( group->getSample() );
	if( tree == NULL ) {
		m_groupMap.erase( group ); // @date 2013.06.21 <Add> M.Izumi
		return;
	}

	// sample
	kome::objects::Sample* sample = group->getSample();
	if( sample == NULL ) {
		return;
	}

	// delete children
	tree->DeleteChildren( id );	// @date 2011.04.25 <Mod> M.Izumi

	// add children
	addChildren( tree, group, id );
}

// update sample node
void SampleTreeManager::updateSampleNode( kome::objects::Sample* sample ) {
	// check objects
	if( sample == NULL ) {
		return;
	}

	unsigned int nGroups = sample->getNumberOfGroups();
	kome::objects::DataGroupNode* group = NULL;

	if (nGroups > 0)
	{
		// Get the first group for this sample
		group = sample->getGroup(0);

		// ID
		wxTreeItemId id = m_groupMap[ group ];
		
		wxTreeCtrl* tree = getTreeCtrl( sample ); // @date 2011.04.25 <Mod> M.Izumi

		// check objects
		if( tree == NULL ){
			m_groupMap.erase( group ); // @date 2013.06.21 <Add> M.Izumi
			return;
		}
		if (sample->isEdited())
		{
			char sModifiedGroup[200];

			_snprintf(sModifiedGroup, 200, "*%s", wxT( NVL(group->getName(), "") ));

			// Edit item
			tree->SetItemText(id, sModifiedGroup);
		}
		else
		{
			// Edit item
			tree->SetItemText(id, wxT( NVL( group->getName(), "" ) ));
		}
	}
}

// select sample
void SampleTreeManager::selectSample( kome::objects::Sample* sample ) {
	// check the member
	if( m_samples.empty() || m_book == NULL ){
		return;
	}

	// index
	int idx = getSampleIndex( sample );
	if( idx < 0 ) {
		return;
	}

	// select
	m_book->SetSelection( idx );
}

// get sample
kome::objects::Sample* SampleTreeManager::getSample( const int idx ) {
	// ID
	if( idx < 0 || idx >= (int)m_samples.size() ) {
		return NULL;
	}

	return m_samples[ idx ].first;
}

// get sample index
int SampleTreeManager::getSampleIndex( kome::objects::Sample* sample ) {
	int idx = -1;
	for( int i = 0; i < (int)m_samples.size() && idx < 0; i++ ) {
		if( sample == m_samples[ i ].first ) {
			idx = i;
		}
	}

	return idx;
}

// get tree control
wxTreeCtrl* SampleTreeManager::getTreeCtrl( kome::objects::Sample* sample ) {
	int idx = getSampleIndex( sample );
	if( idx < 0 ) {
		return NULL;
	}

	return m_samples[ idx ].second;
}

// add tree item ID
wxTreeItemId SampleTreeManager::addTreeItem( wxTreeCtrl* tree, kome::objects::DataGroupNode* group, wxTreeItemId* parentId ) {
	// check parameter and member
	if( group == NULL  ) {
		return parentId;
	}

	// create data
	SampleTreeData* groupData = new SampleTreeData();
	groupData->getData().type = kome::objects::Variant::DATA_GROUP_NODE;
	groupData->getData().prim.pt = group;

	// manager
	kome::window::IconManager& iconMgr = kome::window::IconManager::getInstance();

	// get folder image
	int img = iconMgr.getIconIndex( CLOSE_FOLDER_ICON_NAME );

	// check parameter and member
	wxTreeItemId id;

	if( tree == NULL ) {
		return id;
	}
	kome::objects::Sample* currentSample = NULL;
	currentSample = group->getSample();

	// If the current tree item represents the first group in an edited sample,
	// prefix its name with "*"
	std::string groupName = NVL( group->getName(), "" );
	if ( (group->getGroupIndex() == 0) && (currentSample->isEdited()) )
	{
		groupName = FMT( "*%s", groupName.c_str() );
	}

	// add spectrum group item
	if( parentId == NULL ) {
		id = tree->AddRoot( wxT( groupName.c_str() ), img, img, groupData );
	}
	else {
		id = tree->AppendItem(
			*parentId,
			wxT( groupName.c_str() ), 
			img,
			img,
			groupData
		);
	}

	tree->SetItemImage( id, iconMgr.getIconIndex( OPEN_FOLDER_ICON_NAME ), wxTreeItemIcon_Expanded );

	m_groupMap[ group ] = id;

	// add children
	addChildren( tree, group, id );
	
	return id;
}

// add children
void SampleTreeManager::addChildren( wxTreeCtrl* tree, kome::objects::DataGroupNode* group, wxTreeItemId& groupId ) {
	// create child item (call method recursively)
	for( unsigned int i = 0; i < group->getNumberOfChildren(); i++ ) {
		wxTreeItemId item = addTreeItem( tree, group->getChild( i ), &groupId );
		if (i == 0)
		{	// Expand the first child node by default.
			tree->Expand( item );
		}
	}

	// manager
	kome::window::IconManager& iconMgr = kome::window::IconManager::getInstance();

	// check spectra
	int specCnt = 0;
	for( unsigned int i = 0; i < group->getNumberOfSpectra() && specCnt < 2; i++ ) {
		kome::objects::Spectrum* spec = group->getSpectrum( i );
		if( spec->hasChromatogram() ) {
			specCnt++;
		}
	}

	// add chromatogram item
	for( unsigned int i = 0; i < group->getNumberOfChromatograms(); i++ ) {
		// chromatogram
		kome::objects::Chromatogram* chrom = group->getChromatogram( i );

		// icon
		int img = iconMgr.getIconIndex( chrom->getIcon() );
		if( img < 0 ) {
			img = iconMgr.getIconIndex( "chromatogram" );
		}

		// create chromatogram data
		SampleTreeData* chromData = new SampleTreeData();

		chromData->getData().type = kome::objects::Variant::CHROMATOGRAM;
		chromData->getData().prim.pt = chrom;

		tree->AppendItem(
			groupId,
			wxT( chrom->getName() ),
			img,
			img,
			chromData
		);
	}

	// add spectrum item
	const char* iconNames[] = {
		MS1_ICON_NAME,		
		MS2_ICON_NAME,
		MS3_ICON_NAME,
		MS4_ICON_NAME,
		MS5_ICON_NAME,
		MS6_ICON_NAME,
		MS7_ICON_NAME,
		MSN_ICON_NAME
	};
		
	for( unsigned int i = 0; i < group->getNumberOfSpectra(); i++ ) {
		// get spectrum
		kome::objects::Spectrum* spec = group->getSpectrum( i );

		// >>>>>>	@Date:2013/07/17	<Add>	A.Ozaki
		// for AXIMA fille (High-speed read)
		//  store value of first access flag
		//
		bool	bFirst = spec->isFirstAccess( );
		if  ( true == bFirst )
		{
			spec->resetFirstAccess( );
		}
		// <<<<<<	@Date:2013/07/17	<Add>	A.Ozaki

		// get image
		int img = iconMgr.getIconIndex( spec->getIcon() );
		if( img < 0 ) {
			// stage
			int stage = spec->getMsStage() - 1;
			stage = CLAMP( stage, 0, 7 );

			std::string iconName = iconNames[ stage ];

			// get icon
			img = iconMgr.getIconIndex( iconName.c_str() );
			spec->setIcon( iconName.c_str() );
		}

		// create data
		SampleTreeData* specData = new SampleTreeData();
		specData->getData().type = kome::objects::Variant::SPECTRUM;
		specData->getData().prim.pt = spec;

		// name
		std::string name = NVL( spec->getName(), "" );
		if( spec->getRt() >= 0.0 && spec->getStartRt() == spec->getEndRt() ) {
			name += FMT( " (%.4f)", spec->getRt() );
		}
		if( spec->getPrecursor() > 0.0 ) {
			name += FMT( " [%.2f]", spec->getPrecursor() );
		}

		// add item
		wxTreeItemId specId = tree->AppendItem(
			groupId,
			wxT( name.c_str() ),
			img,
			img,
			specData
		);

		// >>>>>>	@Date:2013/07/17	<Add>	A.Ozaki
		// restore value of first access flag
		if  ( true == bFirst )
		{
			spec->setFirstAccess( );
		}
		// <<<<<<	@Date:2013/07/17	<Add>	A.Ozaki
	}
}

// close sample
void SampleTreeManager::onCloseSample( kome::objects::Sample* sample, const bool deleting ) { 
	// initialize
	// @date 2011.04.25 <Mod> M.Izumi ->
	// check the member
	if( m_samples.empty() || m_book == NULL ) {
		return;
	}

	//<-
	// @date 2011.08.24 <Add> M.Izumi
	// SPEC 82165:Sample name mismatch (Close Dialog)
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();
	aoMgr.delTabSample( sample );

	// check the map
	int idx = getSampleIndex( sample );
	if( idx < 0 ) {
		return;
	}

	// delete groups
	std::vector< kome::objects::DataGroupNode* > groups;
	for( std::map< kome::objects::DataGroupNode*, wxTreeItemId >::iterator it = m_groupMap.begin();
			it != m_groupMap.end(); it++ ) {
		kome::objects::DataGroupNode* tmp = (*it).first;
		if( tmp != NULL && tmp->getSample() == sample ) {
			groups.push_back( tmp );
		}
	}

	for( unsigned int i = 0; i < groups.size(); i++ ) {
		m_groupMap.erase( groups[ i ] );
	}

	// delete page
	m_book->DeletePage( idx );

	// delete sample
	m_samples.erase( m_samples.begin() + idx );

	// active sample
	idx = m_book->GetSelection();
	kome::objects::Sample* newSample = getSample( idx );
	if( aoMgr.getActiveSample() == sample ) {
		aoMgr.setActiveSample( newSample );
	}
}

// get instance
SampleTreeManager& SampleTreeManager::getInstance() {
	// create object
	static SampleTreeManager mgr;

	return mgr;
}

// get notebook
wxNotebook* SampleTreeManager::getNoteBook(){
	return m_book;
}

// create notebook
void SampleTreeManager::createNoteBook(){
	if( m_book == NULL ){
		m_book = new SampleNotebook( kome::window::WindowTool::getMainWindow() );
	}
}

// select spectrum
void SampleTreeManager::selectSpectrum( kome::objects::Spectrum* spec ) {
	// check the member
	if( m_samples.empty() || m_book == NULL || m_treeActionFlg ){
		return;
	}

	// check the parameter
	if( spec == NULL ) {
		return;
	}

	// select sample
	kome::objects::Sample* sample = spec->getSample();
	if( sample == NULL ) {
		return;
	}
	selectSample( sample );

	// get ID
	kome::objects::DataGroupNode* group = spec->getGroup();
	wxTreeItemId id = m_groupMap[ group ];
	if (id.IsOk()){
		wxTreeCtrl* tree = getTreeCtrl( group->getSample() );
		tree->Expand( id );

		SampleTreeData* specData = NULL;
		wxTreeItemIdValue cookie;

		bool bSpectrumFound = false;
		wxTreeItemId childId = tree->GetFirstChild(id, cookie);
		while (childId.IsOk() && (!bSpectrumFound))
		{
			specData = dynamic_cast<SampleTreeData*>(tree->GetItemData(childId));

			if ( (specData->getData().type == kome::objects::Variant::SPECTRUM) &&
				(specData->getData().prim.pt == spec) )
			{
				bSpectrumFound = true;
			}
			else {
				childId = tree->GetNextChild(id, cookie);
			}
		}

		// select
		if( bSpectrumFound ) {
			tree->SelectItem( childId );
			tree->ScrollTo( childId );
		}
	}
}

// select chromatogram
void SampleTreeManager::selectChromatogram( kome::objects::Chromatogram* chrom ) {
	// check the member
	if( m_samples.empty() || m_book == NULL || m_treeActionFlg ){
		return;
	}

	// check the parameter
	if( chrom == NULL ) {
		return;
	}

	// select sample
	kome::objects::Sample* sample = chrom->getSample();
	if( sample == NULL ) {
		return;
	}
	selectSample( sample );

	// get ID
	kome::objects::DataGroupNode* group = chrom->getGroup();
	wxTreeItemId id = m_groupMap[ group ];
	if (id.IsOk())
	{
		wxTreeCtrl* tree = getTreeCtrl( group->getSample() );
		if( tree == NULL ){ // NULL チェック @date 2013.06.19 <Add> M.Izumi
			return;
		}
		tree->Expand( id );

		SampleTreeData* chromData = NULL;
		wxTreeItemIdValue cookie;

		bool bSpectrumFound = false;
		wxTreeItemId childId = tree->GetFirstChild(id, cookie);
		while (childId.IsOk() && (!bSpectrumFound))
		{
			chromData = dynamic_cast<SampleTreeData*>(tree->GetItemData(childId));

			if ( (chromData->getData().type == kome::objects::Variant::CHROMATOGRAM) &&
				(chromData->getData().prim.pt == chrom) )
			{
				bSpectrumFound = true;
			}
			else {
				childId = tree->GetNextChild(id, cookie);
			}
		}

		// select
		if( bSpectrumFound ) {
			tree->SelectItem( childId );
			tree->ScrollTo( childId );
		}
	}
}
