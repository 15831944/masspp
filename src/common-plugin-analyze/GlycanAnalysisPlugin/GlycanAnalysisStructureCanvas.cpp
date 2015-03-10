/**
 * @file GlycanAnalysisStructureCanvas.h
 * @brief Canvas class of Glycan Structure
 *
 * @author N.Tashiro
 * @date 2012.04.02
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#include "GlycanAnalysisStructureCanvas.h"
#include "GlycanAnalysisStructureResults.h"
#include <cctype>

/** Node names in KCF Files (Asn)*/
#define GLYCAN_NODENAME_ASN		"Asn"

/** Node names in KCF Files (Glc)*/
#define GLYCAN_NODENAME_GLC		"Glc"

/** Node names in KCF Files (Gal)*/
#define GLYCAN_NODENAME_GAL		"Gal"

/** Node names in KCF Files (Man)*/
#define GLYCAN_NODENAME_MAN		"Man"

/** Node names in KCF Files (GalNAc)*/
#define GLYCAN_NODENAME_GALNAC	"GalNAc"

/** Node names in KCF Files (GlcNAc)*/
#define GLYCAN_NODENAME_GLCNAC	"GlcNAc"

/** Node names in KCF Files (ManNAc)*/
#define GLYCAN_NODENAME_MANNAC	"ManNAc"

/** Node names in KCF Files (LFuc)*/
#define GLYCAN_NODENAME_LFUC	"LFuc"

/** Node names in KCF Files (Fuc)*/
#define GLYCAN_NODENAME_FUC		"Fuc"

/** Node names in KCF Files (Neu5Ac)*/
#define GLYCAN_NODENAME_NEU5AC	"Neu5Ac"

/** Node names in KCF Files (Neu5Gc)*/
#define GLYCAN_NODENAME_NEU5GC	"Neu5Gc"

/** Layout (Node size)*/
#define GLYCAN_NODE_SIZE		8

/** Layout (Canvas margin)*/
#define GLYCAN_CANVAS_MARGIN	30

/** Layout (Total margin)*/
#define GLYCAN_TOTAL_MARGIN		(GLYCAN_CANVAS_MARGIN + GLYCAN_NODE_SIZE)

/** Layout (Label font size)*/
#define GLYCAN_CANVAS_LABEL_SIZE	16

/** Layout (Label number font size)*/
#define GLYCAN_CANVAS_NUMBER_SIZE	10

/** Layout (Annotation sub font size)*/
#define GLYCAN_CANVAS_ANNOTATION_SUB_SIZE	6

/** Layout (Annotation font size)*/
#define GLYCAN_CANVAS_ANNOTATION_MAIN_SIZE	12

BEGIN_EVENT_TABLE(GlycanAnalysisStructureCanvas, CanvasBase)
  EVT_RIGHT_UP(GlycanAnalysisStructureCanvas::onRightClick)
// 2014/02/24 Add by nakaya Change of a selective state ->
  EVT_LEFT_UP(GlycanAnalysisStructureCanvas::onLeftClick)
// 2014/02/24 Add by nakaya Change of a selective state <-
END_EVENT_TABLE()

// constructor
GlycanAnalysisStructureCanvas::GlycanAnalysisStructureCanvas(
	wxWindow* parent, GLYCAN_STRUCTURE* result,
	unsigned int focus, const char* label, bool bLinked, const char* anion,
	Annotation* annotation)
	: CanvasBase(parent, wxID_ANY, GLYCAN_CANVAS_WIDTH, GLYCAN_CANVAS_HEIGHT){

	if(((label == NULL)	||
		(anion == NULL)	||
		(result == NULL)||
		(parent == NULL))){
		LOG_ERROR(FMT("[GlycanAnalysisStructureCanvas] Label or anion or result or parent is NULL."));
		return;
	}
	// copy parameter to member value
	m_structure.strCompound = result->strCompound;
	m_structure.strGNumber = result->strGNumber;
// 2014/02/20 Change by nakaya The addition of a display item ->
	//m_structure.strScore = result->strScore;
	m_structure.strAssigned = result->strAssigned;
	m_structure.strRank = result->strRank;
// 2014/02/20 Change by nakaya The addition of a display item <-
	for(unsigned int i = 0; i < result->edgeList.size();i++){
		m_structure.edgeList.push_back(result->edgeList.at(i));
	}
	for(unsigned int i = 0; i < result->nodeList.size();i++){
		m_structure.nodeList.push_back(result->nodeList.at(i));
	}
	m_bitmap = wxBitmap(GLYCAN_CANVAS_WIDTH,GLYCAN_CANVAS_HEIGHT);

	if(strcmp(label,"None") != 0){
		m_strLabel = std::string(label);
	}
	else{
		m_strLabel = "";
	}
	m_unFocus = focus;
	m_strAnion = std::string(anion);
	m_strAnion = anion;

	// if bLinked is true, disable right click menu
	if(bLinked){
		m_parent = parent;
	}
	else{
		m_parent = NULL;
	}

	if(annotation != NULL){
		m_annotation.mz = annotation->mz;
		m_annotation.nodeNum = annotation->nodeNum;
		m_annotation.strAnn = annotation->strAnn;
		m_annotation.x1 = annotation->x1;
		m_annotation.y1 = annotation->y1;
		m_annotation.x2 = annotation->x2;
		m_annotation.y2 = annotation->y2;
		m_annotation.x3 = annotation->x3;
		m_annotation.y3 = annotation->y3;
	}

	m_bDisplayGNumber = bLinked;

// 2014/02/24 Add by nakaya Change of a selective state ->
	m_bSelect = false;

	m_bCreateBmp = false;
// 2014/02/24 Add by nakaya Change of a selective state <-
}

// destructor
GlycanAnalysisStructureCanvas::~GlycanAnalysisStructureCanvas(){
	
}

// This method is called when draw event (override method)
void GlycanAnalysisStructureCanvas::onDraw(wxDC& dc){
	// set device context
	m_graphics.setDC( &dc );
// 2014/02/24 Change by nakaya Change of a selective state ->
	//drawStructure();
	drawStructure(m_bSelect, false);
	if(!m_bCreateBmp) {
		drawStructure(false, true);
		m_bCreateBmp = true;
	}
// 2014/02/24 Change by nakaya Change of a selective state <-
}

//call the menu function of parent
void GlycanAnalysisStructureCanvas::onRightClick( wxMouseEvent& evt){
	evt.Skip();
	// <tashiro> fixed(12/05/17) SPEC:85992
	if( wxWindow::GetCapture() == this ){
		this->ReleaseMouse();
	}
	// </tashiro>
	if(m_parent != NULL ){
// 2014/02/24 Add by nakaya Change of a selective state ->
		((GlycanAnalysisStructureResults*)m_parent)->select(atoi(m_structure.strRank.c_str()) - 1);
// 2014/02/24 Add by nakaya Change of a selective state <-
		((GlycanAnalysisStructureResults*)m_parent)->openMenu();
	}
}

// 2014/02/24 Add by nakaya Change of a selective state ->
//call the menu function of parent
void GlycanAnalysisStructureCanvas::onLeftClick( wxMouseEvent& evt){
	evt.Skip();

	if( wxWindow::GetCapture() == this ){
		this->ReleaseMouse();
	}
	if(m_parent != NULL ){
		((GlycanAnalysisStructureResults*)m_parent)->select(atoi(m_structure.strRank.c_str()) - 1);
	}
}
// 2014/02/24 Add by nakaya Change of a selective state <-

// draw glycan structure
bool GlycanAnalysisStructureCanvas::drawStructure(bool bSelect, bool bCreateBmp){

	// display nodes & edges list
	std::vector<GLYCAN_NODE> nodeList;
	std::vector<GLYCAN_EDGE> edgeList;

	// keep graphic settings
	kome::window::DCGraphics::DrawMode oldMode = m_graphics.getDrawMode();
	COLOR oldColor = m_graphics.getColor();
	kome::window::DCGraphics::DrawStyle oldStyle = m_graphics.getDrawStyle();
	kome::img::Font oldFont = m_graphics.getFont();

// 2014/02/24 Change by nakaya Change of a selective state ->
	//this->SetBackgroundColour(wxColour("White"));
	if(bSelect){
		this->SetBackgroundColour(wxColour(51,153,255));
	}
	else
	{
		this->SetBackgroundColour(wxColour(255,255,255));
	}
// 2014/02/24 Change by nakaya Change of a selective state <-

	// set graphic settings
	Font labelFont = Font(GLYCAN_CANVAS_LABEL_SIZE, Font::FAMILY_SERIF,	Font::STYLE_NORMAL, Font::WEIGHT_BOLD);
	Font numberFont = Font(GLYCAN_CANVAS_NUMBER_SIZE, Font::FAMILY_SANS_SERIF,	Font::STYLE_NORMAL, Font::WEIGHT_BOLD);

	m_graphics.setDrawMode( kome::window::DCGraphics::DRAW_NORMAL );
	m_graphics.setColor(kome::img::ColorTool::BLACK);

	// first, search the focus node
	if(strcmp(m_strLabel.c_str(),"") != 0){
		for(unsigned int i = 0;i < m_structure.edgeList.size(); i++){
			if(m_unFocus == m_structure.nodeList.at(i).unNodeID){
				nodeList.push_back(m_structure.nodeList.at(i));
			}
		}
	}
	else if(m_unFocus != 1){
		for(unsigned int i = 0;i < m_structure.nodeList.size(); i++){
			if(m_unFocus == m_structure.nodeList.at(i).unNodeID){
				nodeList.push_back(m_structure.nodeList.at(i));
			}
		}
	}
	else{
		for(unsigned int i = 0;i < m_structure.edgeList.size(); i++){
			if(m_unFocus != m_structure.edgeList.at(i).unEndID){
				continue;
			}
			for(unsigned int j = 0;j < m_structure.nodeList.size(); j++){
				if(m_structure.nodeList.at(j).unNodeID == m_structure.edgeList.at(i).unStartID){
					nodeList.push_back(m_structure.nodeList.at(j));
				}
			}
		}
	}
	// get nodes & edges to display
	getRoot(&nodeList, &edgeList);

	// get X and Y range of structure.
// 2014/02/24 Change by nakaya Change of a selective state ->
	//int nMaxX = nodeList.at(0).nodeX;
	//int nMinX = nodeList.at(0).nodeX;
	//int nMaxY = nodeList.at(0).nodeY;
	//int nMinY = nodeList.at(0).nodeY;
	//for(unsigned int i = 1; i < nodeList.size();i++){
	//	if(nMinX > nodeList.at(i).nodeX){
	//		nMinX = nodeList.at(i).nodeX;
	//	}
	//	if(nMaxX < nodeList.at(i).nodeX){
	//		nMaxX = nodeList.at(i).nodeX;
	//	}
	//	if(nMinY > nodeList.at(i).nodeY){
	//		nMinY = nodeList.at(i).nodeY;
	//	}
	//	if(nMaxY < nodeList.at(i).nodeY){
	//		nMaxY = nodeList.at(i).nodeY;
	//	}
	//}
	int nMaxX = 0;
	int nMinX = 0;
	int nMaxY = 0;
	int nMinY = 0;
	if ( 0 < nodeList.size() ) {
		nMaxX = nodeList.at(0).nodeX;
		nMinX = nodeList.at(0).nodeX;
		nMaxY = nodeList.at(0).nodeY;
		nMinY = nodeList.at(0).nodeY;

		for(unsigned int i = 1; i < nodeList.size();i++){
			if(nMinX > nodeList.at(i).nodeX){
				nMinX = nodeList.at(i).nodeX;
			}
			if(nMaxX < nodeList.at(i).nodeX){
				nMaxX = nodeList.at(i).nodeX;
			}
			if(nMinY > nodeList.at(i).nodeY){
				nMinY = nodeList.at(i).nodeY;
			}
			if(nMaxY < nodeList.at(i).nodeY){
				nMaxY = nodeList.at(i).nodeY;
			}
		}
	}
// 2014/02/24 Change by nakaya Change of a selective state <-

	// get size of label
	int nLabelWidth = 0, nLabelHeight = 0;
	if((strcmp(m_strLabel.c_str(),"") != 0) && edgeList.at(0).unEndID == 1){
		m_graphics.setFont( labelFont );
		m_graphics.getTextSize(m_strLabel.c_str(), &nLabelWidth, &nLabelHeight);
	}

	// calculate area for draw edges
	int nInnerWidth		= GLYCAN_CANVAS_WIDTH - GLYCAN_TOTAL_MARGIN * 2 - nLabelWidth;
	int nInnerHeight	= GLYCAN_CANVAS_HEIGHT - GLYCAN_TOTAL_MARGIN * 2;

	// calculate scale
	double dScaleX = double(nInnerWidth) / double(abs(nMaxX - nMinX) + 1);
	double dScaleY = double(nInnerHeight) / double(abs(nMaxY - nMinY) + 1);

	// <tashiro> fixed(12/06/11)
	int nNodeSize		= GLYCAN_NODE_SIZE;
	double dTemp		= -1;
	double dTempX, dTempY;
	for(unsigned int i=0;i < m_structure.nodeList.size(); i++){
		for(unsigned int j=i+1;j < m_structure.nodeList.size(); j++){
			dTempX	= (m_structure.nodeList.at(j).nodeX - m_structure.nodeList.at(i).nodeX) * dScaleX;
			dTempY	= (m_structure.nodeList.at(j).nodeY - m_structure.nodeList.at(i).nodeY) * dScaleY;
			if((dTemp < 0) || (dTemp > dTempX * dTempX + dTempY * dTempY)){
				dTemp = (dTempX * dTempX + dTempY * dTempY);
			}
		}
	}
	if(dTemp > 0){
		if(nNodeSize >= (int)(sqrt(dTemp) / 2)){
			nNodeSize = (int)(sqrt(dTemp) / 2) ;
		}
	}
	// </tashiro>


	// create centering margin
	int	nAlignCenterH = dScaleX / 2;
	int nAlignCenterV = dScaleY / 2;

	// draw edges
	for(unsigned int i = 0; i < edgeList.size();i++){
		unsigned int unStartNodeID, unEndNodeID;
		for(unsigned int j = 0;j < nodeList.size(); j++){
			if(nodeList.at(j).unNodeID == edgeList.at(i).unStartID){
				unStartNodeID = j;
			}
			if(nodeList.at(j).unNodeID == edgeList.at(i).unEndID){
				unEndNodeID = j;
			}
		}
		int nStartX	= (nodeList.at(unStartNodeID).nodeX - nMinX) * dScaleX + GLYCAN_TOTAL_MARGIN + nAlignCenterH;
		int nStartY	= GLYCAN_CANVAS_HEIGHT - ((nodeList.at(unStartNodeID).nodeY - nMinY) * dScaleY + GLYCAN_TOTAL_MARGIN) - nAlignCenterV;
		int nEndX	= (nodeList.at(unEndNodeID).nodeX - nMinX) * dScaleX + GLYCAN_TOTAL_MARGIN + nAlignCenterH;
		int nEndY	= GLYCAN_CANVAS_HEIGHT - ((nodeList.at(unEndNodeID).nodeY - nMinY) * dScaleY + GLYCAN_TOTAL_MARGIN) - nAlignCenterV;
		
		m_graphics.drawLine(nStartX,nStartY,nEndX,nEndY);
	}

	// draw nodes
	for(unsigned int i = 0; i < nodeList.size();i++){
		int nNodeX	= (nodeList.at(i).nodeX - nMinX) * dScaleX + GLYCAN_TOTAL_MARGIN + nAlignCenterH;
		int nNodeY	= GLYCAN_CANVAS_HEIGHT - ((nodeList.at(i).nodeY - nMinY) * dScaleY + GLYCAN_TOTAL_MARGIN) - nAlignCenterV;
		m_graphics.setColor(kome::img::ColorTool::WHITE);
		// Glc, Gal, Man -> Circle
		if((strcmp(nodeList.at(i).strNodeName.c_str(),GLYCAN_NODENAME_GLC) == 0)
			|| (strcmp(nodeList.at(i).strNodeName.c_str(),GLYCAN_NODENAME_GAL) == 0)
			|| (strcmp(nodeList.at(i).strNodeName.c_str(),GLYCAN_NODENAME_MAN) == 0)){
			// <tashiro> fixed(12/06/11)
			m_graphics.drawCircle(nNodeX,nNodeY,nNodeSize,true);
			m_graphics.setColor(kome::img::ColorTool::BLACK);
			m_graphics.drawCircle(nNodeX,nNodeY,nNodeSize,false);
			// </tashiro>
		}
		// GlcNAc, GalNAc, ManNAc -> Square
		else if((strcmp(nodeList.at(i).strNodeName.c_str(),GLYCAN_NODENAME_GLCNAC) == 0)
			|| (strcmp(nodeList.at(i).strNodeName.c_str(),GLYCAN_NODENAME_GALNAC) == 0)
			|| (strcmp(nodeList.at(i).strNodeName.c_str(),GLYCAN_NODENAME_MANNAC) == 0)){
			// <tashiro> fixed(12/06/11)
			int points[] = { nNodeX - nNodeSize	,nNodeY - nNodeSize,
							 nNodeX + nNodeSize	,nNodeY - nNodeSize,
							 nNodeX + nNodeSize	,nNodeY + nNodeSize,
							 nNodeX - nNodeSize	,nNodeY + nNodeSize,
							 nNodeX - nNodeSize	,nNodeY - nNodeSize};
			// </tashiro>
			m_graphics.fillPolygon(points,sizeof(points) / sizeof(int) / 2 - 1);
			m_graphics.setColor(kome::img::ColorTool::BLACK);
			m_graphics.drawLines(points,sizeof(points) / sizeof(int) / 2);
		}
		// LFuc -> Triangle
		else if((strcmp(nodeList.at(i).strNodeName.c_str(),GLYCAN_NODENAME_LFUC) == 0)
			// <tashiro> fixed(12/06/11)
			|| (strcmp(nodeList.at(i).strNodeName.c_str(),GLYCAN_NODENAME_FUC) == 0)){	
			int points[] = { nNodeX						,nNodeY - nNodeSize,
							 nNodeX + nNodeSize	,nNodeY + nNodeSize,
							 nNodeX - nNodeSize	,nNodeY + nNodeSize,
							 nNodeX						,nNodeY - nNodeSize};
			// </tashiro>
			m_graphics.fillPolygon(points,sizeof(points) / sizeof(int) / 2 - 1);
			m_graphics.setColor(kome::img::ColorTool::BLACK);
			m_graphics.drawLines(points,sizeof(points) / sizeof(int) / 2);
		}
	}

	// draw label
	if((strcmp(m_strLabel.c_str(),"") != 0) && edgeList.at(0).unEndID == 1){
		m_graphics.setFont( labelFont );
		int nFontX	= (nodeList.at(0).nodeX - nMinX) * dScaleX + GLYCAN_TOTAL_MARGIN + nAlignCenterH;
		int nFontY	= GLYCAN_CANVAS_HEIGHT - ((nodeList.at(0).nodeY - nMinY) * dScaleY + GLYCAN_TOTAL_MARGIN) - nLabelHeight / 2 - nAlignCenterV;
		m_graphics.drawText(m_strLabel.c_str(),	nFontX, nFontY);

	}

	// draw GNumber
	if(m_bDisplayGNumber){
		int nNumberWidth = 0, nNumberHeight = 0;
		m_graphics.setFont( numberFont );
		m_graphics.getTextSize(m_structure.strGNumber.c_str(), &nNumberWidth, &nNumberHeight);
		m_graphics.setColor(kome::img::ColorTool::BLUE);

		int nFontX	= GLYCAN_CANVAS_WIDTH - nNumberWidth - GLYCAN_CANVAS_MARGIN;
		int nFontY	= GLYCAN_CANVAS_HEIGHT - GLYCAN_CANVAS_MARGIN;
		int nStartX	= nFontX;
		int nStartY	= nFontY + nNumberHeight + 1;
		int nEndX	= nFontX + nNumberWidth;
		int nEndY	= nFontY + nNumberHeight + 1;

		m_graphics.drawText(m_structure.strGNumber.c_str(),	nFontX, nFontY);
		m_graphics.drawLine(nStartX,nStartY,nEndX,nEndY);
	}

	// draw Score
	if(m_bDisplayGNumber){
		int nNumberWidth = 0, nNumberHeight = 0;
		m_graphics.setFont( numberFont );
// 2014/02/20 Change by nakaya The addition of a display item ->
		//m_graphics.getTextSize(m_structure.strScore.c_str(), &nNumberWidth, &nNumberHeight);
		m_graphics.getTextSize(m_structure.strAssigned.c_str(), &nNumberWidth, &nNumberHeight);
// 2014/02/20 Change by nakaya The addition of a display item <-
		m_graphics.setColor(kome::img::ColorTool::RED);

		int nFontX	= GLYCAN_CANVAS_WIDTH - nNumberWidth - GLYCAN_CANVAS_MARGIN;
		int nFontY	= GLYCAN_CANVAS_HEIGHT - 2 * GLYCAN_CANVAS_MARGIN;
		int nStartX	= nFontX;
		int nStartY	= nFontY + nNumberHeight + 1;
		int nEndX	= nFontX + nNumberWidth;
		int nEndY	= nFontY + nNumberHeight + 1;

// 2014/02/20 Change by nakaya The addition of a display item ->
		//m_graphics.drawText(m_structure.strScore.c_str(),	nFontX, nFontY);
		m_graphics.drawText(m_structure.strAssigned.c_str(),	nFontX, nFontY);
// 2014/02/20 Change by nakaya The addition of a display item <-
	}

	// draw Annotation
	if(strcmp(m_annotation.strAnn.c_str(), "") != 0){

		Font annotationMainFont = Font(GLYCAN_CANVAS_ANNOTATION_MAIN_SIZE,
			Font::FAMILY_SERIF,	Font::STYLE_NORMAL, Font::WEIGHT_BOLD);
		Font annotationSubFont	= Font(GLYCAN_CANVAS_ANNOTATION_SUB_SIZE,
			Font::FAMILY_SERIF,	Font::STYLE_NORMAL, Font::WEIGHT_BOLD);
		m_graphics.setColor(kome::img::ColorTool::RED);

		int nTextTotalWidth;
		int nTextMainHeight;
		int nTextSubHeight;
		int nTempTextWidth;
		int nTempTextHeight;

		m_graphics.setFont( annotationMainFont );
		// get base text height from "|"
		m_graphics.getTextSize("|", &nTextTotalWidth, &nTextMainHeight);
		m_graphics.setFont( annotationSubFont );
		m_graphics.getTextSize("|", &nTextTotalWidth, &nTextSubHeight);
		nTextTotalWidth = 0;

		unsigned int unNodeID = 1;
		for(unsigned int i = 0;i < nodeList.size();i++){
			if(m_annotation.nodeNum == nodeList.at(i).unNodeID){
				unNodeID = m_annotation.nodeNum;
				break;
			}
		}

		int nNodeX	= (nodeList.at(unNodeID).nodeX - nMinX) * dScaleX + GLYCAN_TOTAL_MARGIN + nAlignCenterH;
		int nNodeY	= GLYCAN_CANVAS_HEIGHT - ((nodeList.at(unNodeID).nodeY - nMinY) * dScaleY + GLYCAN_TOTAL_MARGIN) - nAlignCenterV;
		// <tashiro> fixed(12/06/11)
		int points[] = { int(nNodeX + m_annotation.x1 * nNodeSize),
							int(nNodeY - m_annotation.y1 * nNodeSize),
							int(nNodeX + m_annotation.x2 * nNodeSize),
							int(nNodeY - m_annotation.y2 * nNodeSize),
							int(nNodeX + m_annotation.x3 * nNodeSize),
							int(nNodeY - m_annotation.y3 * nNodeSize)};
		// </tashiro>
		m_graphics.drawLines(points,sizeof(points) / sizeof(int) / 2);

		int nFontX = points[0];
		int nFontY = points[1] - nTextMainHeight;

		if(m_annotation.y1 < m_annotation.y3){
			nFontY = nNodeY + nTextMainHeight;
		}

		// parse annotation text
		std::vector< std::string > strTextList;
		std::string strTemp = "";

		for(unsigned int i = 0; i < m_annotation.strAnn.size(); i++){
			if( (m_annotation.strAnn.at(i) == '_') ||
				(m_annotation.strAnn.at(i) == '+') ){
				strTextList.push_back(strTemp);
				strTemp = "";
				continue;
			}
			strTemp += m_annotation.strAnn.at(i);
		}
		strTextList.push_back(strTemp);

		if(strTextList.size() == 1){
			// pattern D-NN or E
			// disiplay original annotation text
			m_graphics.setFont( annotationMainFont );
			m_graphics.drawText(strTextList.at(0).c_str(), nFontX, nFontY);
		}
		else if(strTextList.size() == 2){
			// pattern Bn or Cn
			// display alphabet and the value with lower font
			m_graphics.setFont( annotationMainFont );

			m_graphics.drawText(strTextList.at(0).c_str(), nFontX, nFontY);
			m_graphics.getTextSize(strTextList.at(0).c_str(),
				&nTempTextWidth, &nTempTextHeight);
			nTextTotalWidth += nTempTextWidth;

			m_graphics.setFont( annotationSubFont );
			m_graphics.drawText(strTextList.at(1).c_str(), nFontX + nTextTotalWidth,
				nFontY + nTextMainHeight - nTextSubHeight);

		}
		else{
			// pattern n     n 
			//          An or An + anion
			// display alphabet and the value with upper and lower font

			// upper n
			m_graphics.setFont( annotationSubFont );
			m_graphics.getTextSize(strTextList.at(0).c_str(),
				&nTempTextWidth, &nTempTextHeight);	
			m_graphics.drawText(strTextList.at(0).c_str(),
				nFontX, nFontY);
			nTextTotalWidth += nTempTextWidth;

			// A
			m_graphics.setFont( annotationMainFont );
			m_graphics.drawText(strTextList.at(1).c_str(),
				nFontX + nTextTotalWidth, nFontY);
			m_graphics.getTextSize(strTextList.at(1).c_str(),
				&nTempTextWidth, &nTempTextHeight);	
			nTextTotalWidth += nTempTextWidth;

			// lower n
			m_graphics.setFont( annotationSubFont );
			m_graphics.drawText(strTextList.at(2).c_str(),
				nFontX + nTextTotalWidth, nFontY + nTextMainHeight - nTextSubHeight);
			m_graphics.getTextSize(strTextList.at(2).c_str(),
				&nTempTextWidth, &nTempTextHeight);			
			nTextTotalWidth += nTempTextWidth;

			// + anion
			if(strTextList.size() == 4){
				m_graphics.setFont( annotationMainFont );
				m_graphics.drawText("+", nFontX + nTextTotalWidth, nFontY);
				m_graphics.getTextSize("+", &nTempTextWidth, &nTempTextHeight);	
				nTextTotalWidth += nTempTextWidth;
				for(unsigned int i = 0;i < m_strAnion.size(); i++){
					if(isdigit(m_strAnion.at(i))){
						m_graphics.setFont( annotationSubFont );
						std::string strTempCharacter = FMT("%c",m_strAnion.at(i));
						m_graphics.drawText(strTempCharacter.c_str(),
							nFontX + nTextTotalWidth, nFontY + nTextMainHeight - nTextSubHeight);
						m_graphics.getTextSize(strTempCharacter.c_str(),
							&nTempTextWidth, &nTempTextHeight);	
						nTextTotalWidth += nTempTextWidth;
						m_graphics.setFont( annotationMainFont );
					}
					else{
						std::string strTempCharacter = FMT("%c",m_strAnion.at(i));
						m_graphics.drawText(strTempCharacter.c_str(),
							nFontX + nTextTotalWidth, nFontY);
						m_graphics.getTextSize(strTempCharacter.c_str(),
							&nTempTextWidth, &nTempTextHeight);	
						nTextTotalWidth += nTempTextWidth;
					}
				}
			}
		}
	}

// 2014/02/24 Change by nakaya Change of a selective state ->
	//wxMemoryDC memoryDC;
	//memoryDC.SelectObject(m_bitmap);
	//memoryDC.Clear();

	//memoryDC.Blit(2,2,GLYCAN_CANVAS_WIDTH - 4,GLYCAN_CANVAS_HEIGHT - 4,
	//	m_graphics.getDC(),0,0);

	//memoryDC.SelectObject( wxNullBitmap );
	if ( bCreateBmp ) {
		wxMemoryDC memoryDC;
		memoryDC.SelectObject(m_bitmap);
		memoryDC.Clear();

		memoryDC.Blit(2,2,GLYCAN_CANVAS_WIDTH - 4,GLYCAN_CANVAS_HEIGHT - 4,
			m_graphics.getDC(),0,0);

		memoryDC.SelectObject( wxNullBitmap );
	}
// 2014/02/24 Change by nakaya Change of a selective state <-

	// reverse settings
	m_graphics.setFont( oldFont );
	m_graphics.setDrawMode( oldMode );
	m_graphics.setDrawStyle( oldStyle );
	m_graphics.setColor( oldColor );

	return true;
}

// pick nodes to display
bool GlycanAnalysisStructureCanvas::getRoot(std::vector<GLYCAN_NODE> *nodeList, std::vector<GLYCAN_EDGE> *edgeList){
	if((nodeList == NULL) || (edgeList == NULL)){
		return false;
	}
	
	for(unsigned int i = 0;i < nodeList->size(); i++){
		for(unsigned int j = 0;j < m_structure.edgeList.size(); j++){
			if(m_structure.edgeList.at(j).unEndID != nodeList->at(i).unNodeID){
				continue;
			}
			for(unsigned int k = 0;k < m_structure.nodeList.size(); k++){
				if(m_structure.nodeList.at(k).unNodeID != m_structure.edgeList.at(j).unStartID){
					continue;
				}
				if( (strcmp(m_structure.nodeList.at(k).strNodeName.c_str(), GLYCAN_NODENAME_NEU5AC) == 0) ||
					(strcmp(m_structure.nodeList.at(k).strNodeName.c_str(), GLYCAN_NODENAME_NEU5GC) == 0) ){
						break;
				}
				nodeList->push_back(m_structure.nodeList.at(k));
				edgeList->push_back(m_structure.edgeList.at(j));
			}
		}
	}

	return true;

}

// return GNumber text
const char* GlycanAnalysisStructureCanvas::getGNumber(){
	return m_structure.strGNumber.c_str();
}


// 2014/02/20 Add by nakaya The addition of a display item ->

BEGIN_EVENT_TABLE(TextCanvas, CanvasBase)
	EVT_RIGHT_UP(TextCanvas::onRightClick)
	EVT_LEFT_UP(TextCanvas::onLeftClick)
END_EVENT_TABLE()

// constructor
TextCanvas::TextCanvas(
	wxWindow* parent, GLYCAN_STRUCTURE* result, const char* label, const int width, const int height)
	: CanvasBase(parent, wxID_ANY, width, height){

	if((label == NULL)){
		LOG_ERROR(FMT("[TextCanvas] Label is NULL."));
		return;
	}

	// copy parameter to member value
	m_structure.strCompound = result->strCompound;
	m_structure.strGNumber = result->strGNumber;
	m_structure.strAssigned = result->strAssigned;
	m_structure.strRank = result->strRank;

	for(unsigned int i = 0; i < result->edgeList.size();i++){
		m_structure.edgeList.push_back(result->edgeList.at(i));
	}
	for(unsigned int i = 0; i < result->nodeList.size();i++){
		m_structure.nodeList.push_back(result->nodeList.at(i));
	}

	if(strcmp(label,"None") != 0){
		m_strLabel = std::string(label);
	}
	else{
		m_strLabel = "";
	}

	m_nCanvasWidth = width;
	m_nCanvasHeight = height;

	m_parent = parent;

	m_bSelect = false;
}

// destructor
TextCanvas::~TextCanvas(){
	
}

// This method is called when draw event (override method)
void TextCanvas::onDraw(wxDC& dc){
	// set device context
	m_graphics.setDC( &dc );
	drawText(m_bSelect);
}

// draw glycan structure
bool TextCanvas::drawText(bool bSelect){
	// keep graphic settings
	kome::window::DCGraphics::DrawMode oldMode = m_graphics.getDrawMode();
	COLOR oldColor = m_graphics.getColor();
	kome::window::DCGraphics::DrawStyle oldStyle = m_graphics.getDrawStyle();
	kome::img::Font oldFont = m_graphics.getFont();

	if(bSelect){
		this->SetBackgroundColour(wxColour(51,153,255));
	}
	else
	{
		this->SetBackgroundColour(wxColour(255,255,255));
	}

	// set graphic settings
	Font labelFont = Font(GLYCAN_CANVAS_LABEL_SIZE, Font::FAMILY_SERIF,	Font::STYLE_NORMAL, Font::WEIGHT_BOLD);
	Font numberFont = Font(GLYCAN_CANVAS_NUMBER_SIZE, Font::FAMILY_SANS_SERIF,	Font::STYLE_NORMAL, Font::WEIGHT_BOLD);

	m_graphics.setDrawMode( kome::window::DCGraphics::DRAW_NORMAL );
	m_graphics.setColor(kome::img::ColorTool::BLACK);
	
	int nLabelWidth = 0, nLabelHeight = 0, nLabelPosX = 0, nLabelPosY = 0;
	if((strcmp(m_strLabel.c_str(),"") != 0)){
		// get size of label
		m_graphics.setFont( labelFont );
		m_graphics.getTextSize(m_strLabel.c_str(), &nLabelWidth, &nLabelHeight);

		nLabelPosX = (m_nCanvasWidth / 2) - (nLabelWidth / 2);
		nLabelPosY = (m_nCanvasHeight / 2) - (nLabelHeight / 2);

		// draw label
		m_graphics.drawText(m_strLabel.c_str(),	nLabelPosX, nLabelPosY);
	}

	// reverse settings
	m_graphics.setFont( oldFont );
	m_graphics.setDrawMode( oldMode );
	m_graphics.setDrawStyle( oldStyle );
	m_graphics.setColor( oldColor );

	return true;
}

//call the menu function of parent
void TextCanvas::onRightClick( wxMouseEvent& evt){
	evt.Skip();

	if( wxWindow::GetCapture() == this ){
		this->ReleaseMouse();
	}
	if(m_parent != NULL ){
		((GlycanAnalysisStructureResults*)m_parent)->select(atoi(m_structure.strRank.c_str()) - 1);

		((GlycanAnalysisStructureResults*)m_parent)->openMenu();
	}
}

//call the menu function of parent
void TextCanvas::onLeftClick( wxMouseEvent& evt){
	evt.Skip();

	if( wxWindow::GetCapture() == this ){
		this->ReleaseMouse();
	}
	if(m_parent != NULL ){
		((GlycanAnalysisStructureResults*)m_parent)->select(atoi(m_structure.strRank.c_str()) - 1);
	}
}

// 2014/02/20 Add by nakaya The addition of a display item <-
