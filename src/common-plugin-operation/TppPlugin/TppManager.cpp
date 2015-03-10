/**
 * @file TppManager.cpp
 * @brief implements of ToolsTppDialog class
 *
 * @author N.Tashiro
 * @date 2013.03.15
 * 
 * Copyright(C) 2013 Shimadzu. All rights reserved.
 */

#include "stdafx.h"
#include "TppManager.h"
#include "TppPepXMLHandler.h"
#include "TppProtXMLHandler.h"

using namespace kome::Tpp;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG

using namespace kome::db;

#define INTERACT_PARSER_EXE			"InteractParser.exe"
#define PEPTIDEPROPHET_PARSER_EXE	"PeptideProphetParser.exe"
#define INTERPROPHET_PARSER_EXE		"InterProphetParser.exe"
#define PROTEINPROPHET_EXE			"ProteinProphet.exe"

#define BINARY_FOLDER_PATH	"C:\\Inetpub\\tpp-bin\\"

#define RESULT_PREFIX "result_"
#define INTERPROPHET_FILENAME "iprophet.pep.xml"
#define PROTEINPROPHET_FILENAME "iprophet_proteinprophet.prot.xml"
#define FASTA_FILENAME "uniprot_sprot.fasta"
#define PEPXML_SUFFIX "pep.xml"
#define INDEX_FILENAME "index.html"

#define INTERACT_PARSER_PREFIX "interact"
#define PEPTIDEPROPHET_PARSER_PREFIX "peptideprophet."
#define INTERACT_PARSER_OPTION		"-L7"
#define PEPTIDEPROPHET_PARSER_OPTION "MINPROB=0.00"

#define TEMPLETE_PATH "opt\\tpp\\"
#define TEMPLETE_INDEX_NAME "Result.tmpl"
#define TEMPLETE_DETAIL_NAME "ResultDetail.tmpl"


// constructor 
TppManager::TppManager() {
	createTable();
	clearVector();
	
	/*test*/
	//return;
	
	//std::string p = "C:\\Users\\tashiro\\.mspp\\TPP_FILES\\test";
	//m_strCurrentResultFolder = p;

	//parseXML();

	//insertResult("aa","aaa","aaaa","C:\\Users\\tashiro\\.mspp\\TPP_FILES\\test");
	
	/*/test*/
}

// destructor
TppManager::~TppManager() {
	m_strPepXMLList.clear();
}

// get instance
TppManager& TppManager::getInstance() {
	// create object (This is the only object.)
	static TppManager mgr;
	return mgr;
}

// clear pep.xml list
void TppManager::clearVector(){
	m_strPepXMLList.clear();
}

//create result folder
std::string TppManager::createResultFolder(){

	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	
	if(!isdirectory( FMT("%s\\%s", msppMgr.getConfDir(), TPP_PERSONAL_FOLDER).c_str() )){
		makedirectory( FMT("%s\\%s", msppMgr.getConfDir(), TPP_PERSONAL_FOLDER).c_str() );
	}

	long long resultIndex = 0;

	//search result_n directory not exists
	while(isdirectory( FMT("%s\\%s\\%s%d", msppMgr.getConfDir(), TPP_PERSONAL_FOLDER ,
		RESULT_PREFIX, resultIndex).c_str() )){
		// nothing
		resultIndex++;
	}

	//path of result
	std::string strResult = FMT("%s\\%s\\%s%d",
		msppMgr.getConfDir(),
		TPP_PERSONAL_FOLDER ,
		RESULT_PREFIX,
		resultIndex);

	// create directory .mspp\TPP_FILES\result_[no]
	makedirectory( strResult.c_str() );

	// create directory .mspp\TPP_FILES\result_[no]\temp
	makedirectory( (strResult + "\\temp").c_str() );
	
	// create directory .mspp\TPP_FILES\result_[no]\html
	makedirectory( (strResult + "\\html").c_str() );

	m_strCurrentResultFolder = strResult;
	
	clearVector();

	return strResult;
}

// set pep xml
void TppManager::setPepXML(std::string strPepPath){
	std::string result = executePeptideProphet(strPepPath);

	if(result.compare("") != 0){
		m_strPepXMLList.push_back(result);
	}
	else{
		LOG_ERROR(FMT("Failed to PeptideProphet for %s.",strPepPath.c_str()));
	}
}

// create result
bool TppManager::createResult(std::string strPath){

	executeiProphet();
	executeProteinProphet();

	parseXML();

	return true;
}

//insert data to result table
bool TppManager::insertResult(std::string strDate,std::string strTargets, std::string strComment, std::string strPath){

	DbManager& dbMgr = DbManager::getInstance();

	Database* db = dbMgr.getDb();

	if(db == NULL){
		LOG_ERROR(FMT("Failed to get database."));
		return false;
	}
	
	//check if table already exists.
	if(db->hasTable(TABLE_NAME)){
		kome::db::Statement* stmt = db->prepare(
			FMT("insert into %s(%s, %s, %s, %s) values(?,?,?,?);",
				TABLE_NAME,
				TABLE_RESULT_COLUMN_DATE,
				TABLE_RESULT_COLUMN_TARGET,
				TABLE_RESULT_COLUMN_COMMENT,
				TABLE_RESULT_COLUMN_PATH).c_str());
		stmt->bindString(1, strDate.c_str());
		stmt->bindString(2,	strTargets.c_str());
		stmt->bindString(3, strComment.c_str());
		stmt->bindString(4, strPath.c_str());
		bool result = stmt->execute();
		stmt->close();
		db->commit();
		return result;
	}
	return false;
}

// open selected index result
void TppManager::openResult(long long llId){

	DbManager& dbMgr = DbManager::getInstance();
	Database* db = dbMgr.getDb();

	if(db == NULL){
		LOG_ERROR(FMT("Failed to get database."));
		return;
	}
	
	std::string strFilePath = "";

	//check if table already exists.
	if(db->hasTable(TABLE_NAME)){
		ResultSet* rs = db->executeQuery(
			FMT("SELECT %s from %s;",TABLE_RESULT_COLUMN_PATH, TABLE_NAME).c_str()
		);
		for(long long i = 0;i <= llId;i++){
			rs->next();
		}
		strFilePath = rs->getString(0, "");
	}

	if(strFilePath.compare("") == 0){
		LOG_ERROR(FMT("Failed to get data."));
		return;
	}

	wxString	strBrowser = FMT( "explorer \"%s\\%s%s%s\"",
		strFilePath.c_str(), TPP_PERSONAL_HTML_FOLDERNAME, RESULT_PREFIX, INDEX_FILENAME );
	::wxExecute( strBrowser, wxEXEC_MAKE_GROUP_LEADER );

}

//create TPPRESULT table
bool TppManager::createTable(){

	DbManager& dbMgr = DbManager::getInstance();

	Database* db = dbMgr.getDb();

	if(db == NULL){
		LOG_ERROR(FMT("Failed to get database."));
		return false;
	}

	//check if table already exists.
	if(!db->hasTable(TABLE_NAME)){
		Table table(TABLE_NAME);
		
		TableColumn* col;
		
		col = table.addColumn(TABLE_RESULT_COLUMN_DATE);
		col->setDataType( TableColumn::DataType::VARCHAR );
		col->setNotNull(true);

		col = table.addColumn(TABLE_RESULT_COLUMN_TARGET);
		col->setDataType( TableColumn::DataType::VARCHAR );
		col->setNotNull(true);

		col = table.addColumn(TABLE_RESULT_COLUMN_COMMENT);
		col->setDataType( TableColumn::DataType::VARCHAR );
		col->setNotNull(false);

		col = table.addColumn(TABLE_RESULT_COLUMN_PATH);
		col->setDataType( TableColumn::DataType::VARCHAR );
		col->setNotNull(true);

		bool bResult = db->createTable(table);
		return bResult;
	}

	return true;
}

//execute peptide prophet
std::string TppManager::executePeptideProphet(std::string strPath){
	bool bResult = false;

	std::string strInput = m_strCurrentResultFolder +
		"\\" + TPP_PERSONAL_XML_FOLDERNAME + strPath;
	std::string strOutput = strInput;
	int nInsert = strInput.find(PEPXML_SUFFIX, 0);
	strOutput.insert(
		nInsert, PEPTIDEPROPHET_PARSER_PREFIX);
	
	



	std::string strIPCmdLine = getpath( BINARY_FOLDER_PATH, INTERACT_PARSER_EXE );
	strIPCmdLine = FMT("\"%s\" ",strIPCmdLine.c_str());
	strIPCmdLine = FMT("%s \"%s\" \"%s\"", strIPCmdLine.c_str(),
		strOutput.c_str(), strInput.c_str());
	strIPCmdLine = FMT("%s %s", strIPCmdLine.c_str(), INTERACT_PARSER_OPTION);

	STARTUPINFO ipInfo;
	ZeroMemory(&ipInfo, sizeof(ipInfo));
	PROCESS_INFORMATION ipProcessInfo;
	ZeroMemory(&ipProcessInfo, sizeof(ipProcessInfo));

	if (CreateProcess(NULL, const_cast<char *>(strIPCmdLine.c_str()), NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &ipInfo, &ipProcessInfo))
	{
		// Wait until child process exits.
		if (WaitForSingleObject( ipProcessInfo.hProcess, INFINITE ) != WAIT_FAILED)
		{
			// Close process and thread handles. 
			CloseHandle( ipProcessInfo.hProcess );
			CloseHandle( ipProcessInfo.hThread );

			bResult = true;
		}
	}
	if(!bResult){
		return "";
	}

	bResult = false;

	std::string strPPCmdLine = getpath( BINARY_FOLDER_PATH, PEPTIDEPROPHET_PARSER_EXE );
	strPPCmdLine = FMT("\"%s\" ",strPPCmdLine.c_str());
	strPPCmdLine = FMT("%s \"%s\" %s", strPPCmdLine.c_str(), strOutput.c_str(), PEPTIDEPROPHET_PARSER_OPTION);

	STARTUPINFO ppInfo;
	ZeroMemory(&ppInfo, sizeof(ppInfo));
	PROCESS_INFORMATION ppProcessInfo;
	ZeroMemory(&ppProcessInfo, sizeof(ppProcessInfo));

	if (CreateProcess(NULL, const_cast<char *>(strPPCmdLine.c_str()), NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &ppInfo, &ppProcessInfo))
	{
		// Wait until child process exits.
		if (WaitForSingleObject( ppProcessInfo.hProcess, INFINITE ) != WAIT_FAILED)
		{
			// Close process and thread handles. 
			CloseHandle( ppProcessInfo.hProcess );
			CloseHandle( ppProcessInfo.hThread );

			bResult = true;
		}
	}
	if(bResult){
		return strOutput;
	}

	return "";
}

//execute iProphet
std::string TppManager::executeiProphet(){
	
	bool bResult = false;
	
	std::string strOutput = m_strCurrentResultFolder +
		"\\" + TPP_PERSONAL_XML_FOLDERNAME + INTERPROPHET_FILENAME;
	std::string strCmdLine = getpath( BINARY_FOLDER_PATH, INTERPROPHET_PARSER_EXE );
	strCmdLine = FMT("\"%s\" ",strCmdLine.c_str());
	std::string strInputs = "";

	for(int i = 0;i < m_strPepXMLList.size(); i++){
		strInputs += FMT("\"%s\" ",
			m_strPepXMLList[i].c_str());
	}

	if(strInputs.length() > 8192){
		LOG_ERROR(FMT("Failed to iProphet (too many files)."));
	}
	
	strCmdLine = FMT("%s %s %s", strCmdLine.c_str(),
		strInputs.c_str(), strOutput.c_str());

	STARTUPINFO info;
	ZeroMemory(&info, sizeof(info));
	PROCESS_INFORMATION processInfo;
	ZeroMemory(&processInfo, sizeof(processInfo));

	if (CreateProcess(NULL, const_cast<char *>(strCmdLine.c_str()), NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &info, &processInfo))
	{
		// Wait until child process exits.
		if (WaitForSingleObject( processInfo.hProcess, INFINITE ) != WAIT_FAILED)
		{
			// Close process and thread handles. 
			CloseHandle( processInfo.hProcess );
			CloseHandle( processInfo.hThread );

			bResult = true;
		}
	}
	if(bResult){
		return strOutput;
	}
	return "";

}

//execute protein prophet
std::string TppManager::executeProteinProphet(){
	
	bool bResult = false;
	
	std::string strOutput = m_strCurrentResultFolder +
		"\\" + TPP_PERSONAL_XML_FOLDERNAME + PROTEINPROPHET_FILENAME;

	std::string strInput = m_strCurrentResultFolder +
		"\\" + TPP_PERSONAL_XML_FOLDERNAME + INTERPROPHET_FILENAME;

	std::string strCmdLine = getpath( BINARY_FOLDER_PATH, PROTEINPROPHET_EXE );

	strCmdLine = FMT("\"%s\" \"%s\" \"%s\"", strCmdLine.c_str(),
		strInput.c_str(), strOutput.c_str());

	STARTUPINFO info;
	ZeroMemory(&info, sizeof(info));
	PROCESS_INFORMATION processInfo;
	ZeroMemory(&processInfo, sizeof(processInfo));

	if (CreateProcess(NULL, const_cast<char *>(strCmdLine.c_str()), NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &info, &processInfo))
	{
		// Wait until child process exits.
		if (WaitForSingleObject( processInfo.hProcess, INFINITE ) != WAIT_FAILED)
		{
			// Close process and thread handles. 
			CloseHandle( processInfo.hProcess );
			CloseHandle( processInfo.hThread );

			bResult = true;
		}
	}
	if(bResult){
		return strOutput;
	}
	return "";

}

//parse xml
void TppManager::parseXML(){

	TppXMLHandler::T_pep* pep;
	TppXMLHandler::T_prot* prot;

	std::string pepFilePath = m_strCurrentResultFolder + "\\" +
		+ TPP_PERSONAL_XML_FOLDERNAME + INTERPROPHET_FILENAME;

	std::string protFilePath = m_strCurrentResultFolder + "\\" +
		+ TPP_PERSONAL_XML_FOLDERNAME + PROTEINPROPHET_FILENAME;

	pep = new TppXMLHandler::T_pep();
	TppPepXMLHandler pepXMLHandler(pep);
	pepXMLHandler.parse(pepFilePath.c_str());

	prot = new TppXMLHandler::T_prot();
	TppProtXMLHandler	protXMLHandler(prot);
	protXMLHandler.parse(protFilePath.c_str());		
	
	createHTML(pep, prot);

	delete prot;
	delete pep;
	
}

//create result html files
void TppManager::createHTML(TppXMLHandler::T_pep* pep, TppXMLHandler::T_prot* prot){

	//prepare create html.
	std::vector<TppXMLHandler::T_ProphetResult> ar_result;
	std::vector<TppXMLHandler::T_DetailResultHistgram> ar_detail_histgram;

	//peps & proteins -> T_ProphetResult vector
	for (int i = 0; i < prot->sizeProteinGroup(); i++)
	{
		TppXMLHandler::T_protein_group *protein_group = &prot->ProteinGroup(i);
									
		// the number of protein loop
		// (The number of protein contained in the protein group that is).
		for (int nProteinIndex = 0; nProteinIndex <  protein_group->sizeProtein(); nProteinIndex++)
		{
			TppXMLHandler::T_ProphetResult result;
			result.ProteinGroupNo = TppXMLHandler::string2int(protein_group->getAttrValue("group_number"));

			result.Protein = protein_group->Protein(nProteinIndex).getAttrValue("protein_name");
			result.IndistinguishableProteins = protein_group->Protein(nProteinIndex).getIndistinguishableProteinsNames();

			result.ProteinProphetProbability = protein_group->Protein(nProteinIndex).getProteinProphetProbability();
			result.PercentCoverage = protein_group->Protein(nProteinIndex).getPercentCovrage();
			result.IdentifiedPeptides = protein_group->Protein(nProteinIndex).getIndentifiedPeptide();

			std::vector<std::string> s_ar_peptide_names = protein_group->Protein(nProteinIndex).getPeptideNames();
			
			std::vector<TppXMLHandler::T_search_hit*> ar_search_hit = pep->getSearchHitPtr(s_ar_peptide_names);
			TppXMLHandler::T_ProphetDetailResult detail;
			
			for (int nSearchHitIndex = 0; nSearchHitIndex < ar_search_hit.size(); nSearchHitIndex++)
			{
				std::string sPeptide = ar_search_hit[nSearchHitIndex]->getPeptideName();

				detail.PeptideSequence = sPeptide;

				double db = 0;
				bool bTandem = ar_search_hit[nSearchHitIndex]->isTandem();
				bool bMascot =  ar_search_hit[nSearchHitIndex]->isMascot();
							
				if (bTandem == true)
				{
					detail.type = TppXMLHandler::ResultType::Tandem;
					detail.Probability = ar_search_hit[nSearchHitIndex]->getPeptideProphetTandemProbability();
					detail.Score = ar_search_hit[nSearchHitIndex]->getTandemScore();
				}
				if (bMascot == true)
				{
					detail.type = TppXMLHandler::ResultType::Mascot;
					detail.Probability = ar_search_hit[nSearchHitIndex]->getPeptideProphetMascotProbability();
					detail.Score = ar_search_hit[nSearchHitIndex]->getMascotScore();
				}

				double db_iprophet=  ar_search_hit[nSearchHitIndex]->getInterProphetResultProbability();
				detail.InterProphetProbability = db_iprophet;
						
				detail.SpectrumQuery = ar_search_hit[nSearchHitIndex]->SpectrumQuery;

				result.insertDetail(detail);
			}	// next search index

			result.sortInterProphetProbability();

			ar_result.push_back(result);

		}	// next protein index
	}

	//html data vector
	std::vector<std::string> s_ar_table_data;
	std::vector<std::string> s_ar_graph1_data;
	std::vector<std::string> s_ar_graph2_data;
	std::vector<std::string> s_ar_venn_data;

	//set data of venn to vector 
	createVennHTML(&s_ar_venn_data, &ar_result);

	//set data of Tables and Result of Protein prophet list.
	createTableGraph1HTML(&s_ar_table_data, &s_ar_graph1_data, &ar_result);

	//set data of Histgram of protein and peptide.
	createGraph2HTML(&s_ar_graph2_data, &ar_result, &ar_detail_histgram);

	//chain all data.
	std::vector<std::string> s_ar_html;
	for (int i = 0; i < s_ar_table_data.size(); i++){
		s_ar_html.push_back(s_ar_table_data[i]);
	}
	for (int i = 0; i <  s_ar_venn_data.size(); i++){
		s_ar_html.push_back(s_ar_venn_data[i]);
	}
	for (int i = 0; i < s_ar_graph1_data.size(); i++){
		s_ar_html.push_back(s_ar_graph1_data[i]);
	}
	for (int i = 0; i < s_ar_graph2_data.size(); i++){
		s_ar_html.push_back(s_ar_graph2_data[i]);
	}

	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();

	std::string strOutIndexPath = FMT("%s\\%s%s%s",
		m_strCurrentResultFolder.c_str(),
		TPP_PERSONAL_HTML_FOLDERNAME,
		RESULT_PREFIX, INDEX_FILENAME);

	std::string strTempleteIndexPath = FMT("%s\\%s%s",
		msppMgr.getMsppDir(),
		TEMPLETE_PATH,
		TEMPLETE_INDEX_NAME	
		);
	replaceHtmlFile(strTempleteIndexPath, strOutIndexPath, s_ar_html);
	
	//clear vectors.
	s_ar_table_data.clear();
	s_ar_venn_data.clear();
	s_ar_graph1_data.clear();
	s_ar_graph2_data.clear();
	s_ar_html.clear();

	
	std::string strTempleteDetailPath = FMT("%s\\%s%s",
		msppMgr.getMsppDir(),
		TEMPLETE_PATH,
		TEMPLETE_DETAIL_NAME	
		);

	// create index file from fasta.
	std::string strFastaPath = FMT("%s\\%s%s",
		msppMgr.getMsppDir(),
		TEMPLETE_PATH,
		FASTA_FILENAME	
	);
	TppXMLHandler::T_fasta *fasta = new TppXMLHandler::T_fasta(strFastaPath);	
	fasta ->load();

	// create detail pages.
	int nProteinSeqNo = 0;
	for (int nResultIndex = 0; nResultIndex < ar_result.size(); nResultIndex++){
		int nSeqNo = 1;
		int nProteinGroupNo = ar_result.at(nResultIndex).ProteinGroupNo;
		int nSearchHitNo = 0;

		nProteinSeqNo++;
		std::vector<std::string> s_ar_result;

		std::string strOutDetailPath = "";
	 
		std::string strDetailResultHtmlFile = FMT("%s\\%s%s%d.html",
			m_strCurrentResultFolder.c_str(),
			TPP_PERSONAL_HTML_FOLDERNAME,
			RESULT_PREFIX,
			nProteinSeqNo);

		std::vector<std::string> s_ar_peptide_key;

		// proteinに含まれる全てのindistinguishable_proteinの中から、"sp|"から始まる行を探して
		// それを、fastaファイルから探す、protein keyとする。
		std::string s_protein_key = "";
//		if(ar_result.at(nResultIndex).IndistinguishableProteins.size() > 0){
//			for (int i = 0; i < ar_result.at(nResultIndex).IndistinguishableProteins.size() ;i++){
//				if (strcmp(ar_result.at(nResultIndex).IndistinguishableProteins[i].substr(0, 3).c_str(),"sp|") == 0){
//					s_protein_key = ar_result.at(nResultIndex).IndistinguishableProteins[i];
//					break;
//				}
//			}
//		}
//		else{
			s_protein_key = ar_result.at(nResultIndex).Protein;
//		}

		// peptideの一覧を列挙する
		for (int nDetailResultIndex = 0; nDetailResultIndex < 
			ar_result.at(nResultIndex).sizeDetail();	nDetailResultIndex++)
		{
			// remove both sides "?_" and "_?" from peptide name
			std::string sTmp = ar_result.at(nResultIndex).Detail(nDetailResultIndex).PeptideSequence;
			sTmp = sTmp.substr(2, sTmp.size() - 4);
			s_ar_peptide_key.push_back(sTmp);
		}
		if (s_protein_key.size() > 0 && s_ar_peptide_key.size() > 0)
		{
			std::vector<std::string> s_ar_sequence = fasta->getSequence (s_protein_key);
			std::vector<std::string> s_ar_SeqHtml = fasta->formatHtmlSequence (s_ar_sequence, s_ar_peptide_key);

			// 20130322 tashiro modified
			s_ar_result.push_back("var SEQUENCE = \"\";");
			for each(std::string sSeqHtml in s_ar_SeqHtml){
				s_ar_result.push_back("SEQUENCE += \"" + sSeqHtml + "\";");
			}
			//			std::string sSeqHtml = fasta->formatHtmlSequence (s_ar_sequence, s_ar_peptide_key);
			//
			//sSeqHtml = "var SEQUENCE = \"" + sSeqHtml + "\";";
			//
			//s_ar_result.push_back(sSeqHtml);
			// / 20130322 tashiro modified
			
			int a = 1;
		}

		if(ar_result.at(nResultIndex).IndistinguishableProteins.size() > 0){
			s_ar_result.push_back("var PROTEIN = \"" + TppXMLHandler::convertCSV (ar_result.at(nResultIndex).IndistinguishableProteins) + "\";");
		}
		else{
			s_ar_result.push_back("var PROTEIN = \"" + ar_result.at(nResultIndex).Protein + "\";");
		}
		//s_ar_result.push_back("var PROTEIN = \"" + TppXMLHandler::convertCSV (ar_result.at(nResultIndex).IndistinguishableProteins) + "\";");
		s_ar_result.push_back("var IDENTIFIED_PEPTIDES = [");

		std::string sTmp = "";

		// detail the number of loop
		// (corresponding to the line of the detail).
		for (int nDetailResultIndex = 0; nDetailResultIndex < 
			ar_result.at(nResultIndex).sizeDetail(); nDetailResultIndex++){
				TppXMLHandler::T_ProphetDetailResult *detail = &ar_result.at(nResultIndex).Detail(nDetailResultIndex);

				sTmp = "[";
				
				// Peptide Name
				sTmp += "\"" + ar_result.at(nResultIndex).Detail(nDetailResultIndex).PeptideSequence + "\",";

				double dbPeptideProphetProbability =  detail->InterProphetProbability;

				bool bMascot = detail->type == TppXMLHandler::ResultType::Mascot ? true : false;
				bool bTandem = detail->type == TppXMLHandler::ResultType::Tandem ? true : false;

				// peptide prophet (Mascot)
				if (bMascot == true)
					sTmp += FMT("%f", dbPeptideProphetProbability ) + ",";
				else 
					sTmp +=", ";

				// peptide prophet (X!Tandem)
				if (bTandem == true)
					sTmp+= FMT("%f", dbPeptideProphetProbability ) + ",";
				else
					sTmp+=", ";

				// iProphet
				sTmp+= FMT("%f", detail->InterProphetProbability) + ",";

				// Maccot Score
				if (bMascot == true)
					sTmp+= FMT("%f",detail->Score) + ",";
				else
					sTmp+=",";

				// Tandem Score
				if (bTandem == true)
					sTmp+= FMT("%.3E",detail->Score) + ",";
				else
					sTmp+=",";

				sTmp+= "],";

				s_ar_result.push_back(sTmp);

				nSeqNo++;

		}	// next detail result index

		s_ar_result.push_back("];");
		replaceHtmlFile (strTempleteDetailPath, strDetailResultHtmlFile , s_ar_result);

	} // next result index

	delete fasta;	// ここに、すごく時間がかかる（フリーズしたように見えても、驚かないこと。

	ar_detail_histgram.clear();
	ar_result.clear();
	


	return;
	
}

//replace between <DATA_INSERT_AREA> tag
void TppManager::replaceHtmlFile (std::string sInputHtmlFile, std::string sOutputHtmlFile,
	std::vector<std::string> s_ar_data)
{
	std::wstring wsInputHtmlFile = TppXMLHandler::ToWideStr(sInputHtmlFile);
	std::wstring wsOutputHtmlFile = TppXMLHandler::ToWideStr(sOutputHtmlFile);
			
	// utf-8でhtmlファイルを読み込む
	FILE* fp, *fpW;
	_wfopen_s (&fp, wsInputHtmlFile.c_str(), L"r");		_setmode (_fileno (fp), _O_U8TEXT);
	_wfopen_s (&fpW, wsOutputHtmlFile.c_str(), L"w");	 _setmode (_fileno (fpW), _O_U8TEXT);
	
	std::wofstream fsW (fpW);
	std::wifstream fs (fp);

	std::wstring wstr;
	bool bEof = false;

	bool bDataAreaEnter = false;
	bool bDataInserted = false;
	while (!fs.eof())
	{
		std::getline (fs, wstr);

		if (wcscmp(wstr.c_str(), L"//<DATA_INSERT_AREA>") == 0)	// utf-8文字同士で比較
		{
			fsW << wstr;		fsW << L"\n";
			bDataAreaEnter  = true;
			continue;
		}
		if (wcscmp(wstr.c_str(), L"//</DATA_INSERT_AREA>") == 0)	// utf-8文字同士で比較
		{
			fsW << wstr;		fsW << L"\n";
			bDataAreaEnter  = false;
			continue;
		}
		if (bDataAreaEnter  == true)
		{
			if (bDataInserted == false)
			{
				std::wstring wTmp;
							
				for (int i = 0; i < s_ar_data.size(); i++)
				{
					wTmp = TppXMLHandler::ToWideStr(s_ar_data[i]);
					fsW << wTmp;	fsW << L"\n";
				}
				bDataInserted = true;
			}
		} else {
			fsW << wstr;	fsW << L"\n";
		}
	}
	fclose (fp);
	fclose (fpW);
}

//create graph2 data of HTML
void TppManager::createGraph2HTML( std::vector<std::string>* graph2,
	std::vector<TppXMLHandler::T_ProphetResult>* prophets,
	std::vector<TppXMLHandler::T_DetailResultHistgram>* details){
		
	details->clear();

	int nProteinSeqNo2 = 0;
	for (int nResultIndex = 0; 	nResultIndex <  prophets->size(); nResultIndex++){
		for (int nDetailResultIndex = 0; nDetailResultIndex < 
			prophets->at(nResultIndex).sizeDetail();	nDetailResultIndex++){

			TppXMLHandler::T_DetailResultHistgram his;
			his.ProteinSeqNo = prophets->at(nResultIndex).ProteinGroupNo;
			his.DetailResult = &prophets->at(nResultIndex).Detail(nDetailResultIndex);

			details->push_back(his);
		}
	}

	std::sort(details->begin(), details->end(),
		TppXMLHandler::comareDetailResultHisgam);	

	graph2->push_back("var IPROPHET = [");

	for (int i = 0; i < details->size(); i++)
	{
		std::string sTmpGraph2 = "";

		TppXMLHandler::T_DetailResultHistgram *his = &details->at(i);

		int color_no = 0;
		int marker_no = 0;
		getColorMarkerNo (his->ProteinSeqNo, color_no, marker_no);

		sTmpGraph2 = "[";
		sTmpGraph2 += FMT("%f", his->DetailResult->InterProphetProbability) + ",";	// probability
		sTmpGraph2 += FMT("%d", color_no) + ",";
		sTmpGraph2 += FMT("%d", marker_no) + "";
	
		sTmpGraph2+= "],";
		graph2->push_back(sTmpGraph2);
	}
	graph2->push_back("];");


}

//create table and graph1 data of HTML
void TppManager::createTableGraph1HTML(std::vector<std::string>* table,
	std::vector<std::string>* graph1, std::vector<TppXMLHandler::T_ProphetResult>* prophets){
		std::sort(prophets->begin(), prophets->end(), TppXMLHandler::compareProbability);

	int nProteinSeqNo1 = 1;

	table->push_back("var IDENTIFIED_PROTEINS = [");
	graph1->push_back("var PROTEIN_PROPHET = [");

	for (int nResultIndex = 0; 	nResultIndex < prophets->size(); nResultIndex++)
	{
		int color_no = 0;
		int marker_no = 0;
		
		getColorMarkerNo(nProteinSeqNo1, color_no, marker_no);

		std::string sTableTmp = "[";
		std::string sGraph1Tmp = "[";

		// color no
		sTableTmp+= FMT("%d", color_no) + ",";
		// mark no
		sTableTmp+= FMT("%d", marker_no) + ",";

		// IndistinguishableProteins
		sTableTmp+= "\"" + prophets->at(nResultIndex).Protein;

		if(prophets->at(nResultIndex).IndistinguishableProteins.size() > 0){
			sTableTmp+= " ," + TppXMLHandler::convertCSV(prophets->at(nResultIndex).IndistinguishableProteins);
		}

		sTableTmp += "\", ";

		//sTableTmp+= "\"" + TppXMLHandler::convertCSV(prophets->at(nResultIndex).IndistinguishableProteins) + "\", ";

		// peptide file filename(url link)
		std::string sPeptideFileName = FMT("%s%d.html",
			RESULT_PREFIX, nProteinSeqNo1);
		
		sTableTmp +=  "\"" + sPeptideFileName  +  "\",";
		
		// probability
		float fff = prophets->at(nResultIndex).ProteinProphetProbability ;
		sTableTmp+= FMT("%f", (fff * 100)) + ", ";
		
		// coverage(%)
		sTableTmp+= FMT("%f", prophets->at(nResultIndex).PercentCoverage) + ", ";

		//// identified peptides
		sTableTmp+= FMT("%d", prophets->at(nResultIndex).IdentifiedPeptides) + ", ";

		//// mascot score
		sTableTmp+= FMT("%d", (int)(prophets->at(nResultIndex).getMascotScore())) + ", ";

		//// mascot e-score
		sTableTmp+= FMT("%E", prophets->at(nResultIndex).getMascotScore()) + ", ";

		//// X!Tandem e-score
		sTableTmp+= FMT("%E", prophets->at(nResultIndex).getTandemScore()) + "";

		sTableTmp+= "],";

		// probability
		sGraph1Tmp+= FMT("%f", prophets->at(nResultIndex).ProteinProphetProbability) + ", ";

		// color
		sGraph1Tmp+= FMT("%d",color_no) +  ", ";
						
		// mark	
		sGraph1Tmp+= FMT("%d", marker_no);

		sGraph1Tmp+= "],";

		table->push_back(sTableTmp);

		graph1->push_back(sGraph1Tmp);

		nProteinSeqNo1++;

	}

	table->push_back("];");

	graph1->push_back("];");
}

// create HTML of Venn
void TppManager::createVennHTML(std::vector<std::string>* result, std::vector<TppXMLHandler::T_ProphetResult>* prophets){

	int nVennMascotCount	= 0;
	int nVennUnionCount		= 0;
	int nVennTandemCount	= 0;

	for (int nResultIndex = 0; nResultIndex < prophets->size(); nResultIndex++)
	{
		std::map <std::string, int> map_mascot_cnt;
		std::map <std::string, int> map_tandem_cnt;

		for (int nResultDetailIndex = 0;
			nResultDetailIndex < prophets->at(nResultIndex).sizeDetail();
			nResultDetailIndex++)
		{
			TppXMLHandler::T_ProphetDetailResult *detail = &prophets->at(nResultIndex).Detail(nResultDetailIndex);

			std::string s_key = detail->SpectrumQuery + " + " + detail->PeptideSequence;

			if (detail->type == TppXMLHandler::ResultType::Mascot)
				map_mascot_cnt[s_key]++;

			if (detail->type == TppXMLHandler::ResultType::Tandem)
				map_tandem_cnt[s_key]++;
		}

		if (map_mascot_cnt.size() > 0 && map_tandem_cnt.size()  > 0){
			nVennUnionCount++;
		}
		else if (map_mascot_cnt.size() > 0 && map_tandem_cnt.size() == 0){
			nVennMascotCount++;
		}
		else if (map_mascot_cnt.size() == 0 && map_tandem_cnt.size() > 0){
			nVennTandemCount++;
		}
					
	}

	result->push_back(FMT("var VENN_SCORE= [%d,%d,%d];", nVennMascotCount, nVennUnionCount, nVennTandemCount));
}


// get number of color
void TppManager::getColorMarkerNo (int nProteinSeqNo, int &nColorNo, int &nMarkerNo){
	nProteinSeqNo--;
	nColorNo = ((nProteinSeqNo + nProteinSeqNo / 10) % 10) + 1;
	nMarkerNo = (nProteinSeqNo) % 5 + 1;
	if (nProteinSeqNo >= 50)
		nColorNo = 0;	// black
}
