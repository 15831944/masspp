/**
 * @file HelpGetterPanel.cpp
 * @brief implements of HelpGetterPanel class
 *
 * @author S.Tanaka
 * @date 2012.12.28
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "HelpGetterPanel.h"

#include "ParametersAndHelpPanel.h"

#include "ParametersAndHelpDialog.h"
#include "SettingsPanel.h"
#include "PluginSelectionPanel.h"

#include <wx/choicebk.h>
#include <wx/textctrl.h>

using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define BORDER_SIZE					  	3
#define GRID_GAP				      3


BEGIN_EVENT_TABLE( HelpGetterPanel, StandardPanel )
	EVT_CHILD_FOCUS( HelpGetterPanel::onFocus )
END_EVENT_TABLE()


// constructor
HelpGetterPanel::HelpGetterPanel( wxWindow* parent, const int id ) : StandardPanel( parent, id ) {
}

// destructor
HelpGetterPanel::~HelpGetterPanel() {
}

// child focus event
void HelpGetterPanel::onFocus( wxChildFocusEvent& evt ) {
	// Window
	wxWindow* wnd = evt.GetWindow();
	wxDialog* dlg = dynamic_cast< wxDialog* >( wnd );
	wxPanel* pnl = dynamic_cast< wxPanel* >( wnd );

	// >>>>>>	@Date:2014/03/03	<Add>	A.Ozaki
	//
	// wxChoicebookが居るときには、処理方法を変える必要があるので、
	// その対応
	//
	bool	bChoicebook = false;
	{
		wxWindow* pParent = GetParent( );

		while ( pParent )
		{
			// 子供を捜してchoicebookがいるかを確認
			//
			wxWindowList	children = pParent->GetChildren( );
			for ( wxWindowList::iterator it = children.begin( ) ; it != children.end( ) ; it++ )
			{
				wxChoicebook*	pChoice = dynamic_cast< wxChoicebook* >( *it );
				if  ( pChoice ) {
					bChoicebook = true;
				}

				// 親の持っている階層でも調べます
				// 本来全ての階層で調べる方が良いと思いますが、検索がもったいないのでやめておきます
				// その場合は、再帰呼び出しできる関数で処理した方が良いです
				//
				wxWindowList	children2 = ( *it )->GetChildren( );
				for ( wxWindowList::iterator it2 = children2.begin( ) ; it2 != children2.end( ) ; it2++ )
				{
					wxChoicebook*	pChoice2 = dynamic_cast< wxChoicebook* >( *it2 );
					if  ( pChoice2 ) {
						bChoicebook = true;
					}
				}
			}
			pParent = pParent->GetParent( );
		}
		
		pParent = GetParent( );
		if  ( pParent && bChoicebook )
		{
			kome::window::ParametersAndHelpDialog* dlg = dynamic_cast< ParametersAndHelpDialog* >( pParent );
			if  ( dlg )
			{
				if  ( dynamic_cast< wxChoicebook* >( wnd ) || dynamic_cast< wxChoice* >( wnd ) )
				{
				}
				else
				{
					return;
				}
			}
		}
	}
	//
	// <<<<<<	@Date:2014/03/03	<Add>	A.Ozaki

	if( dlg != NULL || pnl != NULL ) {
		return;
	}

	// text & bitmap
	std::string text = getDescription( wnd );
	std::string bitmap = getImageFilePath( wnd );

	// set
	wxWindow* parent = GetParent();
	ParametersAndHelpPanel* panel = NULL;
	while( parent != NULL && panel == NULL ) {
		ParametersAndHelpPanel* tmp = dynamic_cast< ParametersAndHelpPanel* >( parent );
		if( tmp != NULL && panel == NULL ) {
			panel = tmp;
		}

		dlg = dynamic_cast< wxDialog* >( parent );
		if( dlg != NULL ) {
			wxWindowList children = dlg->GetChildren();
			for( wxWindowList::iterator it = children.begin(); it != children.end(); it++ ) {
				tmp = dynamic_cast< ParametersAndHelpPanel* >( *it );
				if( tmp != NULL && panel == NULL ) {
					panel = tmp;
				}
			}
			if( panel == NULL ) {
				return;
			}
		}

		parent = parent->GetParent();
	}

	if( panel != NULL && !text.empty() ) {
		panel->setDescription( text.c_str() );
		panel->setImageFilePath( bitmap.c_str() );
	}

	evt.Skip();
}

// get the description
std::string HelpGetterPanel::getDescription( wxWindow* wnd ) {
	std::string s;
	return s;
}

// get the image file path
std::string HelpGetterPanel::getImageFilePath( wxWindow* wnd ) {
	std::string s;
	return s;
}
