/**
 * @file TppManager.h
 * @brief interfaces of TppManager class
 *
 * @author N.Tashiro
 * @date 2013.03.15
 * 
 * Copyright(C) 2013 Shimadzu. All rights reserved.
 */


#ifndef __KOME_TPP_MANAGER_H__
#define __KOME_TPP_MANAGER_H__

#include "TppXMLHandler.h"

namespace kome {
	namespace Tpp{

		#define TABLE_RESULT_COLUMN_DATE "Date"
		#define TABLE_RESULT_COLUMN_TARGET "Target"
		#define TABLE_RESULT_COLUMN_COMMENT "Comment"
		#define TABLE_RESULT_COLUMN_PATH "Path"

		class TppManager{

			//.mspp
			//	+ other folders
			//	-TPP_FILES
			//		-result_[0 - ...]
			//			-temp
			//				-[name].pep.xml
			//				-[name].peptideprophet.pep.xml
			//				-iprophet.pep.xml
			//				-iprophet.proteinprophet.prot.xml
			//			-html
			//				-result.html
			//				-result([n]).html

		protected:
			/**
			 * @fn ScriptDialog( wxWindow* parent )
			 * @brief constructor
			 * @param parent parent window
			 */
			TppManager();

			/**
			 * @fn virtual ~ScriptDialog()
			 * @brief destructor
			 */
			virtual ~TppManager();

		private:

			/**
			 * @fn bool createTable()
			 * @brief create table of TPP result
			 * @return true if create complete or already exists
			 */
			bool createTable();

			/**
			 * @fn void clearVector
			 * @brief clear pepXML list
			 */
			void clearVector();

			/**
			 * @fn std::string executePeptideProphet(std::string strPath)
			 * @brief execute peptide prophet
			 * @param[in] strPath path of input file
			 * @return path of output file
			 */
			std::string executePeptideProphet(std::string strPath);

			/**
			 * @fn std::string executeiProphet()
			 * @brief execute iProphet
			 * @return path of output file
			 */
			std::string executeiProphet();

			/**
			 * @fn std::string executeProteinProphet()
			 * @brief execute protein prophet
			 * @return path of output file
			 */
			std::string executeProteinProphet();

			/**
			 * @fn void parseXML(TppXMLHandler::T_pep* pep, TppXMLHandler::T_prot* prot)
			 * @brief parse xml
			 * @param[out] pep iProphet result
			 * @param[out] prot protein prophet result
			 */
			void parseXML();

			/**
			 * @fn void createHTML(TppXMLHandler::T_pep* pep, TppXMLHandler::T_prot* prot)
			 * @brief create HTML files
			 * @param[in] pep iProphet result
			 * @param[in] prot protein prophet result
			 */
			void createHTML(TppXMLHandler::T_pep* pep, TppXMLHandler::T_prot* prot);

			/**
			 * @fn void createVennHTML(
			 *	std::vector<std::string>* result,
			 *  std::vector<TppXMLHandler::T_ProphetResult>* prophets)
			 * @brief create HTML of Venn
			 * @param[out] result venn html
			 * @param[in] prophets input
			 */
			void createVennHTML(std::vector<std::string>* result, std::vector<TppXMLHandler::T_ProphetResult>* prophets);

			/**
			 * @fn void createTableGraph1HTML( std::vector<std::string>* table,
			 *	std::vector<std::string>* graph1,
			 *	std::vector<TppXMLHandler::T_ProphetResult>* prophets)
			 * @brief create table and graph1 data of HTML
			 * @return table and graph1 data of HTML
			 */
			void createTableGraph1HTML( std::vector<std::string>* table,
				std::vector<std::string>* graph1,
				std::vector<TppXMLHandler::T_ProphetResult>* prophets);

			/**
			 * @fn void TppManager::createGraph2HTML( std::vector<std::string>* graph2,
			 *	std::vector<TppXMLHandler::T_ProphetResult>* prophets,
			 *	std::vector<TppXMLHandler::T_DetailResultHistgram>* details)
			 * @brief create graph2 data of HTML
			 * @return graph2 data of HTML
			 */
			void TppManager::createGraph2HTML( std::vector<std::string>* graph2,
				std::vector<TppXMLHandler::T_ProphetResult>* prophets,
				std::vector<TppXMLHandler::T_DetailResultHistgram>* details);

			/**
			 * @fn void getColorMarkerNo(int nProteinSeqNo, int &nColorNo, int &nMarkerNo)
			 * @brief get number of color
			 */
			void getColorMarkerNo(int nProteinSeqNo, int &nColorNo, int &nMarkerNo);

			/**
			 * @fn void TppManager::replaceHtmlFile(
				std::string sInputHtmlFile,
				std::string sOutputHtmlFile,
				std::vector<std::string> s_ar_data)
			 * @brief replace between <DATA_INSERT_AREA> tag
			 */
			void TppManager::replaceHtmlFile(
				std::string sInputHtmlFile,
				std::string sOutputHtmlFile,
				std::vector<std::string> s_ar_data);

		public:
			/**
			 * @fn static FilterManager& getInstance()
			 * @brief gets Internal Standard Correction manager
			 * @return Internal Standard Correction manager class (This is the only object.)
			 */
			static TppManager& getInstance();

			/**
			 * @fn std::string createResultFolder();
			 */
			std::string createResultFolder();

			/**
			 * @fn void setPepXML(std::string strPepPath);
			 */
			void setPepXML(std::string strPath);

			/**
			 * @fn bool createResult(std::string strPath)
			 * @brief create reuslt HTML files.
			 */
			bool createResult(std::string strPath);

			/**
			 * @fn bool insertResult(std::string strDate,
			 *  std::string strTargets, std::string strComment, std::string strPath)
			 * @brief insert data to result table.
			 */
			bool insertResult(std::string strDate,
				std::string strTargets, std::string strComment, std::string strPath);

			/**
			 * @fn void openResult(long long llId);
			 * @brief oepn result in browser
			 */
			void openResult(long long llId);
			
		private:

			//list of pep XML
			std::vector<std::string> m_strPepXMLList;

			//current result folder
			std::string m_strCurrentResultFolder;

		};
	}
}

#endif // __KOME_TPP_MANAGER_H__
