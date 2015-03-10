/**
 * @file molFile Reader (on RDKits)
 * @brief get mol Structures
 *
 * @author M.Fukuda
 * @date 2013.03.05
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "../stdafx.h"
#include "RDKitAccesser.h"
#include "../cWizard/calcFunc/StrFunc.h"

//RDKit
#pragma comment( lib, "FileParsers" )
#pragma comment( lib, "SmilesParse" )
#pragma comment( lib, "RDInchiLib" )
#pragma comment( lib, "Descriptors" )
#pragma comment( lib, "GraphMol" )
#pragma comment( lib, "RDGeneral" )
#pragma comment( lib, "RDGeometryLib" )
#pragma comment( lib, "Inchi" )
#pragma comment( lib, "SubstructMatch" )
#pragma comment( lib, "Depictor" )

//Cairo
#pragma comment( lib, "cairo" )

#include <GraphMol/FileParsers/FileParsers.h>
#include <GraphMol/SmilesParse/SmilesWrite.h>
#include <GraphMol/Descriptors/MolDescriptors.h>
#include <INCHI-API/inchi.h>

#include <GraphMol/SmilesParse/SmilesParse.h>
#include <GraphMol/Depictor/RDDepictor.h>
#include <GraphMol/MolDrawing/MolDrawing.h>
#include <GraphMol/MolDrawing/DrawingToCairo.h>
#include <GraphMol/MolDrawing/DrawingToSVG.h>

#include <cairo/cairo.h>
#include <sstream>
#include <iostream>

#include <boost/tokenizer.hpp>

const std::string kH_NGGetmol =
	"Getting mol file is failed.";

const std::string kErrConNet = 
	"Getting mol file is failed.\n\
Check your internet connection settings.";

const std::string kErrGetMol = 
	"Getting mol file is failed.\nCheck the accession.";	// @date 2013/06/10 <Mod> OKADA


namespace mol {

// ErrorDialogを呼び出す(非公開)
const bool showErrorDialog(
	const std::string& msg,
	const std::string& title
){
	kome::window::WindowTool::showError(msg.c_str(),title.c_str());
	return false;
}

// Mol から各要素を読み出す（共通関数）(非公開)
const bool readFromMol(
	const RDKit::RWMol* mol,
	std::string& massStr,
	std::string& formula,
	std::string& smiles,
	std::string& inchi
){
	// info
	const double mass = RDKit::Descriptors::calcExactMW( *mol );
	massStr = ext::toStr(mass);
	formula = RDKit::Descriptors::calcMolFormula( *mol );
	smiles = RDKit::MolToSmiles( *mol );

	// inchiKeyの検索はデバッグ時コストが非常に重い。
	// 理由は不明
	RDKit::ExtraInchiReturnValues rv;
	inchi = RDKit::MolToInchi( *mol, rv );
	return true;
}

const bool checkHttpString(
	const std::string& src,
	std::string& dst
){
	typedef boost::char_separator<char> char_separator;
	typedef boost::tokenizer<char_separator> tokenizer;

	// split by \n . keep brank line.	
	char_separator sep("\n","",boost::keep_empty_tokens);
	tokenizer tokens(src, sep);
    
	std::vector<std::string> lines;
    std::copy(tokens.begin(), tokens.end(), std::back_inserter(lines));

	bool molfg(false);
	dst.clear();
	std::string tmpStr;
	
	// Http header and getted data is sparated by brank line.
	// in any case, having space. or mixed \r\n and \n....
	// trim /r space /t .. so on and find brank line.
	// and get only data lines.
	for each(auto& line in lines) {
		if(!molfg) {
			tmpStr = trimstring(line.c_str());
			if(tmpStr.empty()) {
				molfg = true;
			}
		} else {
			if (line.find("\r") == std::string::npos) {
				dst += line + "\r\n";
			} else {
				dst += line + "\n";
			}
		}
	}
	return true;
}

// Folderを選択して、Molファイルを読み込む
const bool getMolDataFromFileSelect(
	std::string& massStr,
	std::string& formula,
	std::string& smiles,
	std::string& inchi)
{
	// file
	wxString path = wxFileSelector(
		wxT( "Select Mol File" ),
		wxEmptyString,
		wxEmptyString,
		wxEmptyString,
		wxT( "Mol Files (*.mol)|*.mol" )
	);

	if(path.empty()) {
		return false;
	}
	try {
		RDKit::RWMol* mol = RDKit::MolFileToMol(path.c_str());
		if (mol == NULL) {
			LOG_ERROR(FMT("."));
			return false;
		}
		readFromMol(mol,massStr,formula,smiles,inchi);
	} catch(std::ostringstream& /* errout */) {				// @date 2013/06/10 <Mod> OKADA	
		LOG_ERROR(FMT("RDKit Throw Error."));
		return false;
	}

	return true;
}

// 指定されたＵＲＬにアクセスをし、Molを受け取る
const bool getMolDataFromURL(
	const std::string& tgtURL,
	std::string& massStr,
	std::string& formula,
	std::string& smiles,
	std::string& inchi
){
	
	kome::net::NetworkManager& netMgr = kome::net::NetworkManager::getInstance();
	kome::net::HttpConnection* con = netMgr.getHttpConnection();

	if( !con->openUrl(tgtURL.c_str())) {
		return showErrorDialog(kErrConNet,"Extarnal DB Connection failed.");
	}

	// send request
	con->sendRequest();

	// get response
	unsigned int len = con->getResponseLength();
	if( len == 0 ) {
		// 改定。ネットワーク接続不備の時にここに入る可能性がある。
		std::string msg = kErrConNet + FMT("\n\nNo Response From Site.\n%s", tgtURL.c_str());
		return showErrorDialog(msg, FMT("NetWorkError"));
	}

	char* res = new char[ len + 1 ];
	memcpy( res, con->getResponse(), len );
	res[len] = '\0';
	std::string allStr(res);
	delete[] res;

	// HTML is not found
	size_t ntpos = allStr.find("404 Not Found");
	if (ntpos != std::string::npos) {
		std::string msg = FMT("\n-404 Not Found-\n%s\n", tgtURL.c_str());
		msg += kErrGetMol;
		return showErrorDialog(msg,FMT("-404 Not Found-"));
	}
	
	std::string trueStr;

	// 受け取った構文をチェックしてMolの形式に修正する
	checkHttpString(allStr, trueStr);

	// convert to stream
	std::istringstream inst(trueStr);

	// Molファイルとして読込む
	RDKit::RWMol* mol = nullptr;
	unsigned int xlf(0);
	try {
		mol = RDKit::MolDataStreamToMol(inst, xlf);
	}catch (...) {
		std::string errmsgs = "(Network Error(Proxy)) or RDKit Read Error";
		errmsgs += FMT("\n\n%s",trueStr.c_str());
		return showErrorDialog(errmsgs,kH_NGGetmol);
	}

	if (mol == NULL) {
		return false;
	}
	readFromMol(mol,massStr,formula,smiles,inchi);
	return true;
}

// Smilesから構造式(PNG)を作成する
const bool drawSmiles(
	const std::string& smiles,
	const std::string& outputFilePath,
	const int width,
	const int height
){
	RDKit::RWMol* mol = nullptr;
	try {
		mol = RDKit::SmilesToMol(smiles);
	} catch(RDKit::MolSanitizeException &msg) {
		std::string errmsg = msg.message();
		if(!errmsg.empty()){
			errmsg = "SMILES ERROR: " + errmsg;
			LOG_ERROR(errmsg);
		}
		// RDKit Error
		return false;
	}
	if (mol == nullptr) {
		return false;
	}

	std::vector<int> drawing = RDKit::Drawing::MolToDrawing(*mol);
	cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
	cairo_t* cr = cairo_create(surface);

	RDKit::Drawing::DrawingToCairo(drawing, cr, width, height);

	cairo_destroy(cr);

	cairo_surface_write_to_png(surface, outputFilePath.c_str());

	cairo_surface_destroy(surface);

	delete mol;
	return true;
}

}
