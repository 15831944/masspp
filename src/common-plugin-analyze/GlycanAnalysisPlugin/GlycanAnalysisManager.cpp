/**
 * @file GlycanAnalysisManager.cpp
 * @brief interfaces of GlycanAnalysisPlugin plug-in function
 *
 * @author N.Tashiro
 * @date 2012.04.02
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"

#include "GlycanAnalysisManager.h"
#include "GlycanAnalysisLabelXMLHandler.h"
#include "GlycanAnalysisAnionXMLHandler.h"

#include <vector>

using namespace kome::glycan;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG

using namespace kome::window;

/** Layout (annotation font size)*/
#define ANNOTATION_MAIN_SIZE 9

/** Layout (annotation sub font size)*/
#define ANNOTATION_SUB_SIZE	7

/** Layout (distance between annotation and mz)*/
#define ANNOTATION_DISTANCE 10

/** Masses of Hexoses including Gal, Glc and Man */
#define GLYCAN_MASS_HEX	162.0528

/** Masses of N-Acetyl Hexosamines includeing GalNAc, GlcNAc and ManNAc */
#define GLYCAN_MASS_HEXNAC	203.0794

/** Masses of Fucose including Fuc and LFuc */
#define GLYCAN_MASS_FUC	146.0579

/** Masses of ling cleavage for HexNAc */
#define GLYCAN_MASS_LC	83.0371

/** Masses of Neu5Ac */
#define GLYCAN_MASS_NEU5AC	291.0954

/** Masses of Neu5Gc */
#define GLYCAN_MASS_NEU5GC	307.0903

/** Masses of H */
#define MASS_H	1.0078

/** Masses of H2O */
#define MASS_WATER	18.0153

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

/** Max number of KCF files in KCF folder */
#define GLYCAN_MAX_NUMBER	13059

#define GLYCOCT_CORE_RES1	"1b:b-dglc-HEX-1:5"
#define GLYCOCT_CORE_RES2	"2s:n-acetyl"
#define GLYCOCT_CORE_RES3	"3b:b-dglc-HEX-1:5"
#define GLYCOCT_CORE_RES4	"4s:n-acetyl"
#define GLYCOCT_CORE_RES5	"5b:b-dman-HEX-1:5"

#define GLYCOCT_CORE_LIN1	"1:1d(2+1)2n"
#define GLYCOCT_CORE_LIN2	"2:1o(4+1)3d"
#define GLYCOCT_CORE_LIN3	"3:3d(2+1)4n"
#define GLYCOCT_CORE_LIN4	"4:3o(4+1)5d"

/** Input char size */
#define INPUT_CHAR_SIZE 2048

// constructor
GlycanAnalysisManager::GlycanAnalysisManager(){
	
	m_bEnable = true;

	kome::core::MsppManager& msppManager = kome::core::MsppManager::getInstance();
	
	std::string strLabelXMLPath = FMT("%s/plugins/GlycanAnalysisPlugin/label.xml",msppManager.getMsppDir());
	std::string strAnionXMLPath = FMT("%s/plugins/GlycanAnalysisPlugin/anion.xml",msppManager.getMsppDir());
	
	// read XML Files
	GlycanAnalysisLabelXMLHandler* labelXMLHdr = new GlycanAnalysisLabelXMLHandler();
	labelXMLHdr->parse(strLabelXMLPath.c_str());
	if(!labelXMLHdr->isIncludeDefault(GLYCAN_ANALYSIS_LABELING_DEFAULT)){
		m_bEnable = false;
	}
	
	GlycanAnalysisAnionXMLHandler* anionXMLHdr = new GlycanAnalysisAnionXMLHandler();
	anionXMLHdr->parse(strAnionXMLPath.c_str());
	if(!anionXMLHdr->isIncludeDefault(GLYCAN_ANALYSIS_ANION_DEFAULT)){
		m_bEnable = false;
	}

	if(m_bEnable){
		m_bEnable &= labelXMLHdr->getLabelList(&m_labelList);
		m_bEnable &= anionXMLHdr->getAnionList(&m_anionList);
	}

	if(!m_bEnable){
		LOG_ERROR( FMT( "Failed to initialize Glycan Analysis.") );
	}
//	maxsearch();

	delete labelXMLHdr;
	delete anionXMLHdr;

}

// destractor
GlycanAnalysisManager::~GlycanAnalysisManager(){
	if(m_dlg != NULL){
		m_dlg->Show(false);
		m_dlg->Destroy();
		delete m_dlg;
		m_dlg = NULL;
	}

	if(m_structureDlg != NULL){
		m_structureDlg->Show(false);
		m_structureDlg->Destroy();
		delete m_structureDlg;
		m_structureDlg = NULL;
	}
}

// get instance
GlycanAnalysisManager& GlycanAnalysisManager::getInstance() {

	// create object (This is the only object.)
	static GlycanAnalysisManager mgr;

	return mgr;
}

// open dialog
bool GlycanAnalysisManager::openDialog( kome::objects::Parameters* params ){

	if(m_dlg != NULL){
		delete m_dlg;
		m_dlg = NULL;
		m_spec = NULL;
		clearAnnotations();
	}

	// create dialog 
	m_dlg = new GlycanAnalysisDialog(kome::window::WindowTool::getMainWindow());
	m_dlg->setAnionList(&m_anionList);
	m_dlg->setLabelList(&m_labelList);

	kome::objects::Variant activeObj
	= kome::plugin::PluginCallTool::getActiveObject( params );
	kome::objects::MsDataVariant msObj( activeObj );
	m_spec = msObj.getSpectrum();



	if( m_dlg != NULL ){
		m_dlg->Show();
	}

	return true;
}

// open structure dialog
bool GlycanAnalysisManager::openStructureDialog( kome::objects::Parameters* params ){
	bool bRet = false;

	// get parameters
	kome::evt::MouseEvent* mouseEvent = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( mouseEvent == NULL ) {
		return bRet;
	}

	if(nGlycans.size() == 0){
		return bRet;
	}

	if(m_dlg == NULL){
		return bRet;
	}

	unsigned int unID;
	
	if(!m_dlg->getSelectedResult(&unID)){
		return bRet;
	}

	for(unsigned int i = 0;i < m_dXList.size();i++){
		if(abs(mouseEvent->x() - (long)m_dXList.at(i)) > (long)m_dWidthList.at(i) / 2){
			continue;
		}
		if((abs(mouseEvent->y() - (long)m_dYList.at(i)) <= (long)m_annotationHeight) && (mouseEvent->y() - (long)m_dYList.at(i) <= 0)){
			// create dialog 
			bRet = true;
			if(m_structureDlg != NULL){
				delete m_structureDlg;
				m_structureDlg = NULL;
			}
			m_structureDlg = new GlycanAnalysisStructureDialog(kome::window::WindowTool::getMainWindow(),nGlycans.at(unID).gNumber,&m_params,&(nGlycans.at(unID).annotations.at(i)));
			if( m_structureDlg != NULL ){
				m_structureDlg->Show(true);
			}
			break;
		}
	}
	
	return bRet;
}

// redraw the spectrum and annotations
void GlycanAnalysisManager::refreshSpectrum(){
	
	kome::plugin::PluginCallTool::openSpectrum( *m_spec );

}

// clear annotations
void GlycanAnalysisManager::clearAnnotations(){
	if(nGlycans.size() > 0){
		for(unsigned int i = 0; i < nGlycans.size(); i++){
			nGlycans.at(i).annotations.clear();
		}
		nGlycans.clear();
	}
}

// close dialog
bool GlycanAnalysisManager::closeDialog(){
	
	if(m_dlg != NULL){
		m_dlg->Show(false);
		m_dlg->SetSizer(NULL);
		m_dlg->Destroy();
		delete m_dlg;
		clearAnnotations();
		m_dlg = NULL;
		m_spec = NULL;
	}
			
	return true;

}

// close dialog(check spectrum)
bool GlycanAnalysisManager::closeDialog( kome::objects::Parameters* params ){

	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	
	if(spec != m_spec){
		return false;
	}

	if(m_dlg != NULL){
		m_dlg->Show(false);
		m_dlg->SetSizer(NULL);
		m_dlg->Destroy();
		delete m_dlg;
		clearAnnotations();
		m_dlg = NULL;
		m_spec = NULL;
	}
		

	return true;

}

// close structure dialog
bool GlycanAnalysisManager::closeStructureDialog(){
	if(m_structureDlg != NULL){
		delete m_structureDlg;
		m_structureDlg = NULL;
	}
	return true;
}

// close structure dialog (check spectrum)
bool GlycanAnalysisManager::closeStructureDialog( kome::objects::Parameters* params ){
	
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );

	if(m_dlg != NULL){
		return false;
	}

	if(m_structureDlg != NULL){
		delete m_structureDlg;
		m_structureDlg = NULL;
	}
	return true;
}

//parsing KCF File
bool GlycanAnalysisManager::parseKCFFile(unsigned int unGNumber, GLYCAN_STRUCTURE* glycanStructure){

	if(glycanStructure == NULL){
		LOG_ERROR(FMT("[GlycanAnalysisManager]glycanStructure is NULL."));
		return false;
	}

	glycanStructure->edgeList.clear();
	glycanStructure->nodeList.clear();
	
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();

	std::string strFilePath = FMT("%s/database/kcf/G%05d.kcf",msppMgr.getMsppDir(),unGNumber);

	if(!fileexists(strFilePath.c_str())) {
		LOG_ERROR(FMT("KCF File is not found.[%s]",strFilePath.c_str()) );
		return false;
	}

	char line[INPUT_CHAR_SIZE];
	char attr_1[INPUT_CHAR_SIZE];
	char attr_2[INPUT_CHAR_SIZE];
	char attr_3[INPUT_CHAR_SIZE];
	FILE *fp;

	fp = fileopen( strFilePath.c_str(), "r" );
	if (fp == NULL){
		LOG_ERROR(FMT("Can't open KCF File.[%s]",strFilePath.c_str()) );
		return false;
	}
	// <tashiro> fixed(12/06/11)
	double x,y;
	while( fgets( line, INPUT_CHAR_SIZE, fp ) != NULL) {
		if(std::string(line).find("ENTRY",0) != std::string::npos){
			memset(attr_1,0,sizeof(attr_1));
			memset(attr_2,0,sizeof(attr_2));
			sscanf(line,"ENTRY %s %s",attr_1,attr_2);
			glycanStructure->strGNumber = attr_1;
			glycanStructure->strCompound = attr_2;
		}
		else if(std::string(line).find("NODE",0) != std::string::npos){
			unsigned int unIndex; 
			GLYCAN_NODE node;
			sscanf(line,"NODE %u",&unIndex);
			for(unsigned int i = 0;i < unIndex;i++){
				fgets( line, INPUT_CHAR_SIZE, fp );
				memset(attr_1,0,sizeof(attr_1));
				sscanf(line,"%u %s %lf %lf",&node.unNodeID,attr_1,&x,&y);
				// <tashiro> fixed(12/06/12)
				if(x > 0){
					node.nodeX = (int)(x + 0.5);
				}
				else{
					node.nodeX = (int)(x - 0.5);
				}
				if(y > 0){
					node.nodeY = (int)(y + 0.5);
				}
				else{
					node.nodeY = (int)(y - 0.5);
				}
				// </tashiro>
				node.strNodeName = std::string(attr_1);
				glycanStructure->nodeList.push_back(node);	
				node.strNodeName = "";
			}
		}
		else if(std::string(line).find("EDGE",0) != std::string::npos){
			unsigned int unIndex;
			GLYCAN_EDGE edge;
			sscanf(line,"EDGE %u",&unIndex);
			for(unsigned int i = 0;i < unIndex;i++){
				fgets( line, INPUT_CHAR_SIZE, fp );
				std::string strEndIDAndCarbon;
				memset(attr_1,0,sizeof(attr_1));
				memset(attr_2,0,sizeof(attr_2));
				memset(attr_2,0,sizeof(attr_3));
				sscanf(line,"%d %u:%s %s",&edge.unEdgeID,&edge.unStartID,attr_1,attr_2);
				edge.strAnomerAndDonor = attr_1;
				strEndIDAndCarbon = attr_2;
				if(strEndIDAndCarbon.find(":",0) != std::string::npos){
					sscanf(strEndIDAndCarbon.c_str(),"%u:%s",&edge.unEndID,attr_3);
					edge.strCarbon = attr_3;
				}
				else{
					sscanf(strEndIDAndCarbon.c_str(),"%u",&edge.unEndID);
					edge.strCarbon = "";
				}
				glycanStructure->edgeList.push_back(edge);
			}
		}
	}
	// </tashiro>
	fclose(fp);

	return true;

}

// calculate
bool GlycanAnalysisManager::calculate( kome::objects::Peaks* peaks, GlycanParams* params ){
	NGlycan glycan;
	Annotation an;

	unsigned int unGNumber = 1;
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();

	std::string strLabelXMLPath = FMT("%s/plugins/GlycanAnalysisPlugin/label.xml",msppMgr.getMsppDir());
	std::string strAnionXMLPath = FMT("%s/plugins/GlycanAnalysisPlugin/anion.xml",msppMgr.getMsppDir());

	// read XML Files
	GlycanAnalysisLabelXMLHandler* labelXMLHdr = new GlycanAnalysisLabelXMLHandler();
	labelXMLHdr->parse(strLabelXMLPath.c_str());
	if(!labelXMLHdr->isIncludeDefault(GLYCAN_ANALYSIS_LABELING_DEFAULT)){
		m_bEnable = false;
	}
	
	GlycanAnalysisAnionXMLHandler* anionXMLHdr = new GlycanAnalysisAnionXMLHandler();
	anionXMLHdr->parse(strAnionXMLPath.c_str());
	if(!anionXMLHdr->isIncludeDefault(GLYCAN_ANALYSIS_ANION_DEFAULT)){
		m_bEnable = false;
	}

	double dAnion = anionXMLHdr->getAnionMass(params->strAnion);
	double dLabel = labelXMLHdr->getLabelMass(params->strLabel);
	double dAnionLabel =dAnion + dLabel;

	delete labelXMLHdr;
	delete anionXMLHdr;

	GLYCAN_STRUCTURE glycanStructure;

	std::string strFilePath;

	char line[INPUT_CHAR_SIZE];
	char attr_1[INPUT_CHAR_SIZE];
	char attr_2[INPUT_CHAR_SIZE];
	char attr_3[INPUT_CHAR_SIZE];
	FILE *fp;

	//TEST construct N-glycan DB
	std::vector<int> iarray;

	// parseModifiedGlycoCTFile(182);

	
	double dMass = dAnionLabel;
	double dSAs = 0.0;
	bool bSuccess = false;
	for(; unGNumber < 13060; unGNumber++){
		strFilePath = FMT("%s/database/kcf/G%05d.kcf",msppMgr.getMsppDir(),unGNumber);
		if(fileexists(strFilePath.c_str())){
			fp = fileopen( strFilePath.c_str(), "r" );
			if (fp == NULL){
				// LOG_ERROR(FMT("Can't open KCF File.[%s]",strFilePath.c_str()) );
				// return false;
				continue;
			}

			bSuccess = true;
			// interpret glycanStructure
			while( fgets( line, 2048, fp ) != NULL) {
				if(!bSuccess){
					break;
				}
				if(std::string(line).find("ENTRY",0) != std::string::npos){
					memset(attr_1,0,sizeof(attr_1));
					memset(attr_2,0,sizeof(attr_2));
					sscanf(line,"ENTRY %s %s",attr_1,attr_2);

					glycanStructure.strGNumber = attr_1;
					glycanStructure.strCompound = attr_2;
				}
				else if(std::string(line).find("NODE",0) != std::string::npos){
					unsigned int unIndex; 
					GLYCAN_NODE node;
					sscanf(line,"NODE %u",&unIndex);
					fgets( line, 2048, fp );
					memset(attr_1,0,sizeof(attr_1));
					sscanf(line,"%u %s %d %d",&node.unNodeID,attr_1,&node.nodeX,&node.nodeY);
					node.strNodeName = std::string(attr_1);
					if(0 == node.strNodeName.compare(GLYCAN_NODENAME_ASN)){
						bSuccess = true;

						//TEST construct N-glycan DB
						iarray.push_back(unGNumber);

					}else{
						bSuccess = false;
						break;
					}
					for(unsigned int i = 1;i < unIndex;i++){
						fgets( line, 2048, fp );
						memset(attr_1,0,sizeof(attr_1));
						node.strNodeName = "";
						sscanf(line,"%u %s %d %d",&node.unNodeID,attr_1,&node.nodeX,&node.nodeY);
						node.strNodeName = std::string(attr_1);
						if(0 == node.strNodeName.compare(GLYCAN_NODENAME_GLC) || 
						   0 == node.strNodeName.compare(GLYCAN_NODENAME_GAL) || 
						   0 == node.strNodeName.compare(GLYCAN_NODENAME_MAN)){
							dMass += GLYCAN_MASS_HEX;
							node.strNodeName = GLYCAN_NODENAME_GLC;
						}else if(0 == node.strNodeName.compare(GLYCAN_NODENAME_GLCNAC) || 
						   0 == node.strNodeName.compare(GLYCAN_NODENAME_GALNAC) || 
						   0 == node.strNodeName.compare(GLYCAN_NODENAME_MANNAC)){
							dMass += GLYCAN_MASS_HEXNAC;
							node.strNodeName = GLYCAN_NODENAME_GLCNAC;
						}else if(0 == node.strNodeName.compare(GLYCAN_NODENAME_FUC) || 
								 0 == node.strNodeName.compare(GLYCAN_NODENAME_LFUC)){
							dMass += GLYCAN_MASS_FUC;
							node.strNodeName = GLYCAN_NODENAME_FUC;
						}else if(0 == node.strNodeName.compare(GLYCAN_NODENAME_NEU5AC)){
							dSAs += GLYCAN_MASS_NEU5AC;
							node.strNodeName = GLYCAN_NODENAME_NEU5AC;
						}else if(0 == node.strNodeName.compare(GLYCAN_NODENAME_NEU5GC)){
							dSAs += GLYCAN_MASS_NEU5GC;
							node.strNodeName = GLYCAN_NODENAME_NEU5GC;
						}else{
							bSuccess = false;
							break;
						}
						glycanStructure.nodeList.push_back(node);
					}
				}
				else if(std::string(line).find("EDGE",0) != std::string::npos){
					unsigned int unIndex;
					GLYCAN_EDGE edge;
					sscanf(line,"EDGE %u",&unIndex);
					for(unsigned int i = 0;i < unIndex;i++){
						fgets( line, 2048, fp );
						std::string strEndIDAndCarbon;
						memset(attr_1,0,sizeof(attr_1));
						memset(attr_2,0,sizeof(attr_2));
						memset(attr_2,0,sizeof(attr_3));
						sscanf(line,"%d %u:%s %s",&edge.unEdgeID,&edge.unStartID,attr_1,attr_2);
						edge.strAnomerAndDonor = attr_1;
						strEndIDAndCarbon = attr_2;
						if(strEndIDAndCarbon.find(":",0) != std::string::npos){
							sscanf(strEndIDAndCarbon.c_str(),"%u:%s",&edge.unEndID,attr_3);
							edge.strCarbon = attr_3;
						}
						else{
							sscanf(strEndIDAndCarbon.c_str(),"%u",&edge.unEndID);
							edge.strCarbon = "";
						}
						glycanStructure.edgeList.push_back(edge);
					}
				}
			}
			if(bSuccess){
				bool bSAs = false;
				if((dMass - params->dTolerance < peaks->getPrecursor()) && (dMass + params->dTolerance > peaks->getPrecursor())){
					bSAs = false;
				}else if((dMass + dSAs - params->dTolerance < peaks->getPrecursor()) && (dMass + dSAs + params->dTolerance > peaks->getPrecursor())){
					bSAs = true;
				}
				// compare precursor mass
				if(((dMass - params->dTolerance < peaks->getPrecursor()) && (dMass + params->dTolerance > peaks->getPrecursor())) || 
					((dMass + dSAs - params->dTolerance < peaks->getPrecursor()) && (dMass + dSAs + params->dTolerance > peaks->getPrecursor()))){

					// 0. annotate basic neutral losses
					assignAnnotation(&an, dMass - MASS_WATER, 1, 3, 3, 3, 3, 3, 3);
					if(bSAs){
						an.mz -= dSAs;
					}
					an.strAnn = "-H2O";
					if(peakCheck(peaks, an.mz, params->dTolerance)){
						glycan.unMatched += 1;
					}
					glycan.unTheoretical += 1;
					glycan.annotations.push_back(an);
					

					
					bSuccess = false;
					// 1. N-glycan core step
					unsigned int currentID = 1;
					unsigned int currentIndex = 0;
					unsigned int startID = 1;
					unsigned int startIndex = 0;
					bool bfuc = false;
					for(unsigned int i = 0; i < glycanStructure.edgeList.size(); i++){
						if(1 == glycanStructure.edgeList[i].unEndID){
							startID = glycanStructure.edgeList[i].unStartID;
							for(unsigned int j = 0; j < glycanStructure.nodeList.size(); j++){
								if(startID == glycanStructure.nodeList[j].unNodeID){
									if(0 == glycanStructure.nodeList[j].strNodeName.compare(GLYCAN_NODENAME_GLCNAC)){
										currentID = startID;
										currentIndex = j;
										break;
									}
								}
							}
						}
					}
					
					// 1.1. search depth
					unsigned int unDepth = 0;
					int uiMaxY = 0;
					unsigned int uiMaxID = 0;
					for(unsigned int i = 0; i < glycanStructure.edgeList.size(); i++){
						if(startID == glycanStructure.edgeList[i].unEndID){
							for(unsigned int j = 0; j < glycanStructure.nodeList.size(); j++){
								if(glycanStructure.edgeList[i].unStartID == glycanStructure.nodeList[j].unNodeID){
									if(0 == glycanStructure.nodeList[j].strNodeName.compare(GLYCAN_NODENAME_FUC)){
										bfuc = true;
										break;
									}else if(0 == glycanStructure.nodeList[j].strNodeName.compare(GLYCAN_NODENAME_GLCNAC)){
										startID = glycanStructure.nodeList[j].unNodeID;
										startIndex = j;
										break;
									}
								}
							}
						}
					}
					for(unsigned int i = 0; i < glycanStructure.nodeList.size(); i++){
						if((glycanStructure.nodeList[i].nodeY >= uiMaxY) && (0 != glycanStructure.nodeList[i].strNodeName.compare(GLYCAN_NODENAME_FUC))){
							uiMaxY = glycanStructure.nodeList[i].nodeY;
							uiMaxID = glycanStructure.nodeList[i].unNodeID;
						}
					}
					for(unsigned int i = 0; i < glycanStructure.nodeList.size(); i++){
						for(unsigned int j = 0; j < glycanStructure.edgeList.size(); j++){
							if(uiMaxID == glycanStructure.edgeList[j].unStartID){
								uiMaxID = glycanStructure.edgeList[j].unEndID;
								unDepth++;
							}
						}
					}

					// 1.2. annotate An GlcNAc				
					assignAnnotation(&an, dMass - dLabel - GLYCAN_MASS_LC, currentIndex + 1, 0, 1, -1, 2, 2, -2);
					if(bSAs){
						// an.mz -= dSAs;
					}
					an.strAnn = FMT("02_A_%d+anion", unDepth);
					if(peakCheck(peaks, an.mz, params->dTolerance)){
						glycan.unMatched += 1;
					}
					glycan.unTheoretical += 1;
					glycan.annotations.push_back(an);

					assignAnnotation(&an, an.mz = dMass - dLabel - dAnion - GLYCAN_MASS_LC - 60 - MASS_H + 42, currentIndex + 1, 0, 1, -1, 2, 2, -2);
					if(bfuc){
						an.mz = dMass - GLYCAN_MASS_FUC - dLabel - dAnion - GLYCAN_MASS_LC - 61 + 188;
					}
					an.strAnn = FMT("02_A_%d - H2O", unDepth);
					if(peakCheck(peaks, an.mz, params->dTolerance)){
						glycan.unMatched += 1;
					}
					glycan.unTheoretical += 1;
					glycan.annotations.push_back(an);
					
					assignAnnotation(&an, dMass - dLabel - dAnion - GLYCAN_MASS_LC - 60 - MASS_H, currentIndex + 1, -1, 1, 0, 2, -2, -2);
					if(bfuc){
						an.mz = dMass - GLYCAN_MASS_FUC - dLabel - dAnion - GLYCAN_MASS_LC - 61;
					}
					if(bSAs){
						// an.mz -= dSAs;
					}
					an.strAnn = FMT("24_A_%d", unDepth);
					if(peakCheck(peaks, an.mz, params->dTolerance)){
						glycan.unMatched += 1;
					}
					glycan.unTheoretical += 1;
					glycan.annotations.push_back(an);

					unDepth--;

					// 1.3. annotate An-1 GlcNAc
					assignAnnotation(&an, dMass - dAnionLabel - GLYCAN_MASS_HEXNAC - MASS_H, startIndex + 1, 0, 2, 2, 3, 3, -3);
					if(bfuc){
						an.mz = dMass - GLYCAN_MASS_FUC - dAnionLabel - GLYCAN_MASS_HEXNAC - MASS_H;
					}
					if(bSAs){
						// an.mz -= dSAs;
					}
					an.strAnn = FMT("B_%d", unDepth);
					if(peakCheck(peaks, an.mz, params->dTolerance)){
						glycan.unMatched += 1;
					}
					glycan.unTheoretical += 1;
					glycan.annotations.push_back(an);

					assignAnnotation(&an, dMass - dAnionLabel - GLYCAN_MASS_LC - GLYCAN_MASS_HEXNAC - MASS_H, startIndex + 1, 0, 1, -1, 2, 2, -2);
					if(bfuc){
						an.mz = dMass - GLYCAN_MASS_FUC - dAnionLabel - GLYCAN_MASS_LC - GLYCAN_MASS_HEXNAC - MASS_H;
					}
					if(bSAs){
						// an.mz -= dSAs;
					}
					an.strAnn = FMT("02_A_%d", unDepth);
					if(peakCheck(peaks, an.mz, params->dTolerance)){
						glycan.unMatched += 1;
					}
					glycan.unTheoretical += 1;
					glycan.annotations.push_back(an);

					assignAnnotation(&an, dMass - dAnionLabel - GLYCAN_MASS_LC - GLYCAN_MASS_HEXNAC - 60 - MASS_H, startIndex + 1, -1, 1, 0, 2, -2, -2);
					if(bfuc){
						an.mz = dMass - GLYCAN_MASS_FUC - dAnionLabel - GLYCAN_MASS_LC - GLYCAN_MASS_HEXNAC - 60 - MASS_H;
					}
					if(bSAs){
						// an.mz -= dSAs;
					}
					an.strAnn = FMT("24_A_%d", unDepth);
					if(peakCheck(peaks, an.mz, params->dTolerance)){
						glycan.unMatched += 1;
					}
					glycan.unTheoretical += 1;
					glycan.annotations.push_back(an);

					currentID = startID;
					currentIndex = startIndex;
					for(unsigned int i = 0; i < glycanStructure.edgeList.size(); i++){
						if(currentID == glycanStructure.edgeList[i].unEndID){
							currentID = glycanStructure.edgeList[i].unStartID;
							break;
						}
					}
					for(unsigned int i = 0; i < glycanStructure.nodeList.size(); i++){
						if(currentID == glycanStructure.nodeList[i].unNodeID){
							currentIndex = i;
							break;
						}
					}
					unDepth--;

					assignAnnotation(&an, dMass - dAnionLabel - GLYCAN_MASS_HEXNAC - GLYCAN_MASS_HEXNAC + MASS_WATER - MASS_H, currentIndex + 1, 0, 2, 2, 3, 3, -3);
					if(bfuc){
						an.mz = dMass - GLYCAN_MASS_FUC - dAnionLabel - GLYCAN_MASS_HEXNAC - GLYCAN_MASS_HEXNAC + MASS_WATER - MASS_H;
					}
					if(bSAs){
						// an.mz -= dSAs;
					}
					an.strAnn = FMT("C_%d", unDepth);
					if(peakCheck(peaks, an.mz, params->dTolerance)){
						glycan.unMatched += 1;
					}
					glycan.unTheoretical += 1;
					glycan.annotations.push_back(an);

					// 2. D and E ion detection step (No bisecting GlcNAc check!)
					std::vector<unsigned int> unDE;
					for(unsigned int i = 0; i < glycanStructure.edgeList.size(); i++){
						if(currentID == glycanStructure.edgeList[i].unEndID){
							unDE.push_back(glycanStructure.edgeList[i].unStartID);
						}
					}
					bool bbisecting = false;
					if(3 == unDE.size()){
						bbisecting = true;
					}
					unDepth--;

					for(unsigned int i = 0; i < unDE.size(); i++){
						int i1 = 0;
						int i2 = 0;
						unsigned int tempIndex = 0;
						for(unsigned int j = 0; j < glycanStructure.nodeList.size(); j++){
							if(currentID == glycanStructure.nodeList[j].unNodeID){
								i1 = glycanStructure.nodeList[j].nodeY;
								currentIndex = j;
							}else if(unDE[i] == glycanStructure.nodeList[j].unNodeID){
								i2 = glycanStructure.nodeList[j].nodeY;
								tempIndex = j;
							}
						}
						if(i2 > i1){
							// 2.1. D ion
							assignAnnotation(&an, GLYCAN_MASS_HEX + sumTree(&glycanStructure.nodeList, &glycanStructure.edgeList, unDE[i]) - MASS_H, currentIndex + 1, 0, 2, -2, 2, -2, -2);
							an.mz = GLYCAN_MASS_HEX + sumTree(&glycanStructure.nodeList, &glycanStructure.edgeList, unDE[i]) - MASS_H;
							an.strAnn = "D";
							glycan.annotations.push_back(an);
							bSuccess = peakCheck(peaks, an.mz, params->dTolerance);

							// temporary scoring
							glycan.dRatio = intensityCheck(peaks, an.mz, params->dTolerance);

							assignAnnotation(&an, GLYCAN_MASS_HEX + sumTree(&glycanStructure.nodeList, &glycanStructure.edgeList, unDE[i]) - MASS_H - MASS_WATER, currentIndex + 1, 0, 2, -2, 2, -2, -2);
							an.strAnn = "D-H2O";
							glycan.annotations.push_back(an);
							bSuccess = peakCheck(peaks, an.mz, params->dTolerance);
							
							for(unsigned int j = 0; j < glycanStructure.nodeList.size(); j++){
								if(unDE[i] == glycanStructure.nodeList[j].unNodeID){
									// currentIndex = i + 1;
									assignAnnotation(&an, sumTree(&glycanStructure.nodeList, &glycanStructure.edgeList, unDE[i]) + MASS_WATER - MASS_H, j+1, 0, 2, 0, 3, 3, -3);
									an.strAnn = FMT("C_%d", unDepth);
									if(peakCheck(peaks, an.mz, params->dTolerance)){
										glycan.unMatched += 1;
									}
									glycan.unTheoretical += 1;
									glycan.annotations.push_back(an);

									assignAnnotation(&an, sumTree(&glycanStructure.nodeList, &glycanStructure.edgeList, unDE[i]) - MASS_H, j+1, 0, 2, 0, 3, 3, -3);
									an.strAnn = FMT("B_%d", unDepth);
									if(peakCheck(peaks, an.mz, params->dTolerance)){
										glycan.unMatched += 1;
									}
									glycan.unTheoretical += 1;
									glycan.annotations.push_back(an);

									// D' ion

								}
							}
						}else if(i2 < i1){
							// 2.2. E ion
							assignAnnotation(&an, sumTree(&glycanStructure.nodeList, &glycanStructure.edgeList, unDE[i]) - 60 - MASS_H, tempIndex + 1, -2, 0, 0, 3, 3, -3);
							// confirm Mr.Nishikaze
							an.strAnn = "E";
							if(peakCheck(peaks, an.mz, params->dTolerance)){
								glycan.unMatched += 1;
							}
							glycan.unTheoretical += 1;
							glycan.annotations.push_back(an);
						}else{
							// bisecting?
						}
					}
					unDE.clear();
					if(bSuccess){
						glycan.gNumber = unGNumber;
						nGlycans.push_back(glycan);
					}
				}
				glycan.unMatched = 0;
				glycan.unTheoretical = 0;
				glycan.dRatio = 0.0;
				glycan.annotations.clear();
			}else{
				// not N-Glycan
			}
			dMass = dAnionLabel;
			dSAs = 0.0;
			glycanStructure.edgeList.clear();
			glycanStructure.nodeList.clear();
		}
		fclose(fp);
	}
	
	/*
	// result example starts
	{
		glycan.gNumber = 3791;
		// annotation 1
		{
			an.mz = 106.334398;
			an.strAnn = "02_A_1+anion";
			an.nodeNum = 4;
			an.x1 = -1;
			an.x2 = 1;
			an.x3 = -1;
			an.y1 = 2;
			an.y2 = 2;
			an.y3 = -2;
			glycan.annotations.push_back(an);
		}
		// annotation 2
		{
			an.mz = 197.009094;
			an.strAnn = "D-H2O";
			an.nodeNum = 2;
			an.x1 = -1;
			an.x2 = 1;
			an.x3 = -1;
			an.y1 = -2;
			an.y2 = -2;
			an.y3 = 2;
			glycan.annotations.push_back(an);
		}
		// annotation 3
		{
			an.mz = 300.334398;
			an.strAnn = "-18";
			an.nodeNum = 2;
			an.x1 = 2;
			an.x2 = 2;
			an.x3 = -2;
			an.y1 = 2;
			an.y2 = -2;
			an.y3 = 0;
			glycan.annotations.push_back(an);
		}
		// annotation 4
		{
			an.mz = 400.812498;
			an.strAnn = "E-18";
			an.nodeNum = 2;
			an.x1 = 2;
			an.x2 = 2;
			an.x3 = -2;
			an.y1 = 2;
			an.y2 = -2;
			an.y3 = 0;
			glycan.annotations.push_back(an);
		}
		// annotation 5
		{
			an.mz = 505.132298;
			an.strAnn = "24_A_5-18";
			an.nodeNum = 2;
			an.x1 = -1;
			an.x2 = 1;
			an.x3 = -1;
			an.y1 = -2;
			an.y2 = -2;
			an.y3 = 2;
			glycan.annotations.push_back(an);
		}
	}
	nGlycans.push_back(glycan);
	// result example ends
	*/
	
	glycan.annotations.clear();

// 2014/02/20 Add by nakaya The addition of a display item ->
	// NGlycan is rearranged into the high order of score. 
	if ( nGlycans.size() > 0 ) {
		std::vector< NGlycan >	glycansTemp;
		std::vector< double > DoubleArray;
		for ( int i = 0; i < nGlycans.size(); i++ ) {
			DoubleArray.push_back( nGlycans.at(i).dRatio );
		}

		std::vector< int > SortIdxArray; 
		SortDouble( DoubleArray, false, SortIdxArray );

		for ( unsigned i = 0; i < SortIdxArray.size(); i++ ) {
			glycansTemp.push_back( nGlycans.at( SortIdxArray[i] ) );
		}

		nGlycans.clear();
		for( int i = 0; i < glycansTemp.size(); i++ ) {
			nGlycans.push_back( glycansTemp.at( i ) );
		}
	}
// 2014/02/20 Add by nakaya The addition of a display item  <-

	if(nGlycans.size() < 1){
		LOG_INFO_CODE(FMT("No matching glycan structures were found."), ERR_OTHER );
		return false;
	}
	return true;
}

bool GlycanAnalysisManager::parseModifiedGlycoCTFile(unsigned int unGNumber){

	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();

	std::string strFilePath = FMT("%s/database/GlycoCT_condenced/%d.txt",msppMgr.getMsppDir(),unGNumber);

	if(!fileexists(strFilePath.c_str())) {
		LOG_ERROR(FMT("KCF File is not found.[%s]",strFilePath.c_str()) );
		return false;
	}

	char line[INPUT_CHAR_SIZE];		// C4101 // @date 2013/06/10 <Del> OKADA
	char attr_1[INPUT_CHAR_SIZE];	// C4101 // @date 2013/06/10 <Del> OKADA
	char attr_2[INPUT_CHAR_SIZE];	// C4101 // @date 2013/06/10 <Del> OKADA
	char attr_3[INPUT_CHAR_SIZE];	// C4101 // @date 2013/06/10 <Del> OKADA
	FILE *fp;							// C4101 // @date 2013/06/10 <Del> OKADA
	
	double x,y;
	while( fgets( line, INPUT_CHAR_SIZE, fp ) != NULL) {
		if(std::string(line).find("RES",0) != std::string::npos){

			// read basic structure of N-glycan
			fgets( line, INPUT_CHAR_SIZE, fp );
			if(std::string(line).find("LIN",0) != std::string::npos){break;}
			


			unsigned int unIndex; 
			GLYCAN_NODE node;



			for(unsigned int i = 0;i < unIndex;i++){
				fgets( line, INPUT_CHAR_SIZE, fp );
				memset(attr_1,0,sizeof(attr_1));
				sscanf(line,"%u %s %lf %lf",&node.unNodeID,attr_1,&x,&y);
			}
		}
		else if(std::string(line).find("LIN",0) != std::string::npos){
			unsigned int unIndex;
			GLYCAN_EDGE edge;
			sscanf(line,"EDGE %u",&unIndex);
			for(unsigned int i = 0;i < unIndex;i++){
				fgets( line, INPUT_CHAR_SIZE, fp );
				std::string strEndIDAndCarbon;
				memset(attr_1,0,sizeof(attr_1));
				memset(attr_2,0,sizeof(attr_2));
				memset(attr_2,0,sizeof(attr_3));
				sscanf(line,"%d %u:%s %s",&edge.unEdgeID,&edge.unStartID,attr_1,attr_2);
				edge.strAnomerAndDonor = attr_1;
				strEndIDAndCarbon = attr_2;
				if(strEndIDAndCarbon.find(":",0) != std::string::npos){
					sscanf(strEndIDAndCarbon.c_str(),"%u:%s",&edge.unEndID,attr_3);
					edge.strCarbon = attr_3;
				}
				else{
					sscanf(strEndIDAndCarbon.c_str(),"%u",&edge.unEndID);
					edge.strCarbon = "";
				}
			}
		}
	}
	fclose(fp);
	
	return true;
}

void GlycanAnalysisManager::assignAnnotation(Annotation *_an, double _mz, int _nodeNum, double _x1, double _x2, double _x3, double _y1, double _y2, double _y3){
	_an->mz = _mz;
	_an->nodeNum = _nodeNum;
	_an->x1 = _x1;
	_an->x2 = _x2;
	_an->x3 = _x3;
	_an->y1 = _y1;
	_an->y2 = _y2;
	_an->y3 = _y3;
}

double GlycanAnalysisManager::sumTree(std::vector<GLYCAN_NODE>* _gn, std::vector<GLYCAN_EDGE>* _ge, unsigned int _startID){
	unsigned int unid = 0;
	for(unsigned int i = 0; i < _gn->size(); i++){
		if(_gn->at(i).unNodeID == _startID){
			unid = i;
		}
	}
	if(unid == 0){
		return 0.0;
	}
	double sumT = 0.0;
	if(0 == _gn->at(unid).strNodeName.compare(GLYCAN_NODENAME_GLC)){
		sumT = GLYCAN_MASS_HEX;
	}else if(0 == _gn->at(unid).strNodeName.compare(GLYCAN_NODENAME_GLCNAC)){
		sumT = GLYCAN_MASS_HEXNAC;
	}else if(0 == _gn->at(unid).strNodeName.compare(GLYCAN_NODENAME_FUC)){
		sumT = GLYCAN_MASS_FUC;
	}
	for(unsigned int i = 0; i < _ge->size(); i++){
		if(_ge->at(i).unEndID == _startID){
			sumT += sumTree(_gn, _ge, _ge->at(i).unStartID);
		}
	}
	return sumT;
}

bool GlycanAnalysisManager::peakCheck(kome::objects::Peaks* peaks, const double _d, const double _tol){
	bool bSuccess = false;
	for(unsigned int i = 0; i < peaks->getLength(); i++){
		if((peaks->getPeak(i)->getX() - _tol < _d) && (peaks->getPeak(i)->getX() + _tol > _d)){
			bSuccess = true;
			break;
		}
	}
	return bSuccess;
}

double GlycanAnalysisManager::intensityCheck(kome::objects::Peaks* peaks, const double _d, const double _tol){
	double d_int = 0.0;
	for(unsigned int i = 0; i < peaks->getLength(); i++){
		if((peaks->getPeak(i)->getX() - _tol < _d) && (peaks->getPeak(i)->getX() + _tol > _d)){
			d_int = peaks->getPeak(i)->getY() / peaks->getMaxY() * 100;		
			break;
		}
	}
	return d_int;
}

// draw annotations
bool GlycanAnalysisManager::drawAnnotation( kome::objects::Parameters* params ){
	if(params == NULL){
		return false;
	}

	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );

	if(spec != m_spec){
		return false;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );

	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	kome::objects::Peaks* peaks = pkMgr.getPeaks( spec );

	if(spec == NULL){
		LOG_TRACE(FMT("[GlycanAnalysisManager] No spectrum."));
		return false;
	}
	
	if(peaks == NULL){
		LOG_TRACE(FMT("[GlycanAnalysisManager] No peaks."));
		return false;
	}

	if(nGlycans.size() == 0){
		return false;
	}
	m_dXList.clear();
	m_dYList.clear();
	m_dWidthList.clear();

	double dX,dY;
	// <tashiro> fixed:120522 SPEC:85990
	double tolerance = m_params.dTolerance;
	// </tashiro>
	unsigned int unIndexAnnotation = 0;
	unsigned int unIndexPeaks = 0;
	
	unsigned int unID;
	m_dlg->getSelectedResult(&unID);

	for(unIndexAnnotation = 0; unIndexAnnotation < nGlycans.at(unID).annotations.size(); unIndexAnnotation++){
		bool bFound = false;
		for(unIndexPeaks = 0; unIndexPeaks < peaks->getLength(); unIndexPeaks++){
			// <tashiro> fixed:120522 SPEC:85990
			if(abs(nGlycans.at(unID).annotations.at(unIndexAnnotation).mz - peaks->getX(unIndexPeaks)) < tolerance){
			// </tashiro>
				bFound = true;
				graphics->setColor(kome::img::ColorTool::GREEN);
				graphics->transformDataToPosition( peaks->getX(unIndexPeaks), peaks->getY(unIndexPeaks), &dX, &dY );
				drawAnnotationString(graphics,unID,unIndexAnnotation,dX,dY);
				break;
			}
		}
		if(!bFound){
			graphics->transformDataToPosition( nGlycans.at(unID).annotations.at(unIndexAnnotation).mz, 0, &dX, &dY );
			graphics->setColor(kome::img::ColorTool::BLUE);
			graphics->drawLine(dX,dY - (graphics->getGraphHeight() / 2),dX,dY);
			drawAnnotationString(graphics,unID,unIndexAnnotation,dX,dY - (graphics->getGraphHeight() / 2) + ANNOTATION_DISTANCE);
		}
	}

	return true;
}

// draw annotations (called by drawAnnotation() )
void GlycanAnalysisManager::drawAnnotationString(kome::img::Graphics* graphics,unsigned int unGlycanID,unsigned int unAnnotationID, unsigned int base_x, unsigned int base_y){

	if(graphics == NULL){
		LOG_ERROR(FMT("[GlycanAnalysisManager] Can't find graphics."));
		return;
	}

	unsigned int x = base_x;
	unsigned int y = base_y;

	// font defination
	Font annotationMainFont = Font(ANNOTATION_MAIN_SIZE,
		Font::FAMILY_SERIF,	Font::STYLE_NORMAL, Font::WEIGHT_LIGHT);
	Font annotationSubFont	= Font(ANNOTATION_SUB_SIZE,
		Font::FAMILY_SERIF,	Font::STYLE_NORMAL, Font::WEIGHT_LIGHT);

	// keep graphic settings
	COLOR oldColor = graphics->getColor();
	kome::img::Graphics::DrawStyle	oldStyle = graphics->getDrawStyle();
	kome::img::Font					oldFont = graphics->getFont();
	
	// parse annotation text
	std::vector< std::string > strTextList;
	std::string strTemp = "";
	for(unsigned int i = 0; i < nGlycans.at(unGlycanID).annotations.at(unAnnotationID).strAnn.size(); i++){
			if(nGlycans.at(unGlycanID).annotations.at(unAnnotationID).strAnn.at(i) == '_'){
				strTextList.push_back(strTemp);
				strTemp = "";
				continue;
			}
			else if(nGlycans.at(unGlycanID).annotations.at(unAnnotationID).strAnn.at(i) == '+'){
				strTextList.push_back(strTemp);
				strTemp = "+";
				std::string strAnion = m_params.strAnion;
				for(unsigned int j = 0;j < strAnion.size(); j++){
					strTextList.push_back(strTemp);
					strTemp = strAnion.at(j);
				}				
				break;
			}
			strTemp += nGlycans.at(unGlycanID).annotations.at(unAnnotationID).strAnn.at(i);
	}
	strTextList.push_back(strTemp);

	// calculate location to draw anotation
	std::vector<unsigned int> nFontXList,nFontYList; 
	std::vector<Font*> fontList;
	int nTextSubHeight,nTextMainHeight;
	int nTempTextWidth, nTempTextHeight;
	unsigned int unTotalTextWidth = 0;

	// get base text height from "|"
	graphics->setFont( annotationMainFont );
	graphics->getTextSize("|", &nTempTextWidth, &nTextMainHeight);
	graphics->setFont( annotationSubFont );
	graphics->getTextSize("|", &nTempTextWidth, &nTextSubHeight);

	// set font and divide number in pattern A
	if(strTextList.size() == 1){
		// pattern D-NN or E
		// disiplay original annotation text
		fontList.push_back(&annotationMainFont);
	}
	else if(strTextList.size() == 2){
		fontList.push_back(&annotationMainFont);
		fontList.push_back(&annotationSubFont);
	}
	else{
		// pattern n     n 
		//          An or An + anion
		// display alphabet and the value with upper and lower font
		strTextList.at(0) = FMT("%c,%c",strTextList.at(0).at(0),strTextList.at(0).at(1));
		fontList.push_back(&annotationSubFont);
		fontList.push_back(&annotationMainFont);
		fontList.push_back(&annotationSubFont);

		if(strTextList.size() >= 4){
			for(unsigned int i = 3;i < strTextList.size();i++){
				if(isdigit(strTextList.at(i).at(0))){
					fontList.push_back(&annotationSubFont);
				}
				else{
					fontList.push_back(&annotationMainFont);
				}
			}
		}
	}

	// calculate text width
	for(unsigned int i = 0;i < strTextList.size();i++){
		graphics->setFont( *(fontList.at(i)) );
		graphics->getTextSize(strTextList.at(i).c_str(), &nTempTextWidth, &nTempTextHeight);	
		unTotalTextWidth += nTempTextWidth;
	}

	//avoid other annotations
	m_annotationHeight = nTextMainHeight;
	avoidAnnotation(&x,&y,unTotalTextWidth,nTextMainHeight,graphics->getGraphMarginTop());
	m_dXList.push_back(x);
	m_dYList.push_back(y);
	m_dWidthList.push_back(unTotalTextWidth);

	// set y
	if(strTextList.size() == 1){
		// pattern D-NN or E
		// disiplay original annotation text
		nFontYList.push_back(y - nTextMainHeight);
	}
	else if(strTextList.size() == 2){
		nFontYList.push_back(y - nTextMainHeight);
		nFontYList.push_back(y - nTextSubHeight);
	}
	else{
		// pattern n     n 
		//          An or An + anion
		// display alphabet and the value with upper and lower font
		nFontYList.push_back(y - nTextMainHeight);
		nFontYList.push_back(y - nTextMainHeight);
		nFontYList.push_back(y - nTextSubHeight);

		if(strTextList.size() >= 4){
			for(unsigned int i = 3;i < strTextList.size();i++){
				if(isdigit(strTextList.at(i).at(0))){
					nFontYList.push_back(y - nTextSubHeight);
				}
				else{
					nFontYList.push_back(y - nTextMainHeight);
				}
			}
		}
	}
	
	// draw annotation string
	graphics->getTextSize(strTextList.at(0).c_str(), &nTempTextWidth, &nTempTextHeight);
	nFontXList.push_back(x - (unTotalTextWidth) / 2);
	for(unsigned int i = 0;i < strTextList.size();i++){
		graphics->setFont( *(fontList.at(i)) );
		graphics->getTextSize(strTextList.at(i).c_str(), &nTempTextWidth, &nTempTextHeight);		
		nFontXList.push_back(nFontXList.at(i) + nTempTextWidth);
		graphics->drawText(strTextList.at(i).c_str(),nFontXList.at(i)+ nTempTextWidth / 2,nFontYList.at(i));
	}

	// reverse settings
	graphics->setFont( oldFont );
	graphics->setDrawStyle( oldStyle );
	graphics->setColor( oldColor );
}

// calculate coord to draw annotation (called by drawAnnotationString() )
void GlycanAnalysisManager::avoidAnnotation(unsigned int* x, unsigned int* y, unsigned int curentWidth, unsigned int nTextMainHeight, unsigned int unRoofHeight){

	if((x == NULL) || (y == NULL)){
		LOG_ERROR(FMT("[GlycanAnalysisManager] x or y is NULL."));
		return;
	}
	
	bool bSwitch = false;
	bool bEmpty	= (m_dXList.size() == 0);
	bool bComplete = false;

	if(bEmpty){
		if(*y - ANNOTATION_DISTANCE - nTextMainHeight < unRoofHeight){
			*y += ANNOTATION_DISTANCE;
		}
		else{
			*y -= ANNOTATION_DISTANCE;
		}
		
		return;
	}

	unsigned int temp_y = *y;

	while(!bComplete){
		if((temp_y - ANNOTATION_DISTANCE - nTextMainHeight < unRoofHeight) && !bSwitch){
			temp_y = *y;
			bSwitch = true;
		}
		
		if(bSwitch){
			temp_y += ANNOTATION_DISTANCE;
		}
		else{
			temp_y -= ANNOTATION_DISTANCE;
		}

		bComplete = true;
		for(unsigned int i = 0;i < m_dYList.size();i++){
			if((abs(((long)temp_y - (long)m_dYList.at(i))) < ANNOTATION_MAIN_SIZE)
				&& (abs(((long)*x - (long)m_dXList.at(i))) < (long)(curentWidth + m_dWidthList.at(i)) / 2)){
				bComplete = false;
				break;
			}
		}
	}

	*y = temp_y;
	return;
}

// return glycans
std::vector< NGlycan >* GlycanAnalysisManager::getGlycans(){
	return &nGlycans;
}

// return params
void GlycanAnalysisManager::setParams(GlycanParams* params){
	if(params == NULL){
		return;
	}
	m_params.dTolerance = params->dTolerance;
	m_params.strAnion = params->strAnion;
	m_params.strLabel = params->strLabel;
}

// return enable
bool GlycanAnalysisManager::isEnable(){
	return m_bEnable;
}

// return spectrum
kome::objects::Spectrum* GlycanAnalysisManager::getSpectrum(){
	return m_spec;
}

// 2014/02/20 Add by nakaya The addition of a display item  ->

// struct for sort double
struct sort_double {
	int nNo;
	double dTarget;
};

// operator for sort double
bool operator<(const sort_double& left, const sort_double& right)
{
	return left.dTarget < right.dTarget ;
}

// operator for sort double
bool operator>(const sort_double& left, const sort_double& right)
{
  return left.dTarget > right.dTarget ;
}

// sort double
void GlycanAnalysisManager::SortDouble( std::vector< double >& SortTarget, bool bType, std::vector< int >& Index ) {
	
	std::vector< struct sort_double > SortData;
	for ( int i = 0; i < SortTarget.size(); i++ ) {
		struct sort_double stSortDouble;
		stSortDouble.nNo = i;
		stSortDouble.dTarget = SortTarget[i];
		SortData.push_back( stSortDouble );
	}

	// increase
	if ( bType == true ) {
		std::sort( SortData.begin(), SortData.end() );
	}
	// decrease
	else {
		std::sort( SortData.begin(), SortData.end(), std::greater<struct sort_double>() );
	}

	// create sort data
	SortTarget.clear();
	Index.clear();
	for ( int i = 0; i < SortData.size(); i++ ) {
		Index.push_back( SortData[i].nNo );
		SortTarget.push_back( SortData[i].dTarget );
	}
}
// 2014/02/20 Add by nakaya The addition of a display item  <-
