/**
 * @file MassBankManager.h
 * @brief interfaces of MassBankManager class
 *
 * @author S.Tanaka
 * @date 2009.06.24
 * 
 * @modify M.Fukuda / Eisai Co 
 * @date 2011.08.30
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_MASBANK_MANAGER_H__
#define __KOME_MASBANK_MANAGER_H__

#define MZ_PARAM_NAME              "m/z"
#define SEARCH_TITLE_PARAM_NAME    "title"

#include <wx/aui/aui.h>

class OverlaySpectrumTool;

namespace kome {
namespace massbank {

class SearchResultListPanel;
class RecordPeaksDialog;
class InstrumentMgr;
class XMLMaker;
class NetWorkCtrl;

class MassBankSearchEngine;

struct venderSt;
struct instrumentSt;

/**
	* @class MassBankManager
	* @brief massbank generic format IO management class
	*/
class MassBankManager {
//--------------------------------------------------------------------------------
//     Instance
//--------------------------------------------------------------------------------
public:
	/**
		* @fn static MassBankManager& getInstance()
		* @brief gets object
		* @return MassBankManager object. (This is the only object.)
		*/
	static MassBankManager& getInstance();

protected:
	/**
		* @fn MassBankManager()
		* @brief constructor
		*/
	MassBankManager();

	/**
		* @fn virtual ~MassBankManager()
		* @brief destructor
		*/
	virtual ~MassBankManager();

public:
	/**
		* @struct SearchResult
		* @brief search result
		*/
	typedef std::pair<std::string, double> queryPair;
	typedef std::vector<queryPair> queryVector;

	// SearchResult Cont
	struct SearchResult {
		std::string id;
		std::string title;
		std::string formula;
		double mass;
		double score;
		queryVector queryArr;
		unsigned int querycount;

		// constructor
		SearchResult(void) 
			: id("")
			,title("")
			,formula("")
			,mass(-1.0)
			,score(-1.0)
			,queryArr()
			,querycount(0){}
	};

	/**
 	 * @struct RecordInfo
	 * @brief record information
	 */
	struct RecordInfo {
		std::string id;
		std::string titlename;
		std::string info;
		bool detaileFg;
		kome::core::DataPoints dps;
		std::string png_path;
		// constructor
		RecordInfo(void)
			: id("")
			, titlename("")
			, info("")
			, detaileFg(false)
			, dps()
			, png_path()
		{}
	};
	
	protected:
		// Data Arrays

		std::vector< SearchResult > m_results;// Search Results
		std::vector<unsigned int> show_modal_ids_;
		std::map<int, RecordInfo> record_info_map_;
		RecordInfo* activeInfoPtr_;
		//int active_record_info_map_id_;

		// Search Type Name
		std::string m_searchTypeName;
		bool m_useOverlapSpec; 	   // overlapped

		// Network Info
		bool m_useQuery;
		bool m_bNetInfoChanged;

		// Dialogs
		SearchResultListPanel* m_resultDlg;    // Result List
		RecordPeaksDialog* m_rsPeaksDlg;  // Peak List

		std::unique_ptr<InstrumentMgr> instMgr_;
		std::unique_ptr<XMLMaker> xmlMaker_;
		std::unique_ptr<NetWorkCtrl> networkCtrl_;

		std::unique_ptr<OverlaySpectrumTool> overlaytool_;

		// positions
		kome::core::Point< int > m_searchDlgPos;
		kome::core::Point< int > m_resultDlgPos;

		unsigned int search_result_dialog_show_flag_;

public:
	OverlaySpectrumTool* overlaytool() {return overlaytool_.get();}
	// XML......
	const std::string convMsBankXML(
		kome::objects::SettingParameterValues* setting,
		const char* tgt,
		int sType);

//------ Easy Getters (Like a InLine)
	const bool IsUseQueryData(){ return m_useQuery;}

	// Overlap mode Flags
	void setUseOverlapSpec( const bool& _flg ) { m_useOverlapSpec = _flg;}

	const bool getUseOverlapSpec() {return m_useOverlapSpec;}

	/**
	* @brief Set Batch Search Type Name
	*/
	void setBatchSearchTypeName();

	/**
		* @brief get need Score Column For Result Ctrl List
		* @return if Need Score Type Called. True.
		*/
	const bool needScoreCol();


//--------------------------------------------------------------------------------
//     Result Control
//--------------------------------------------------------------------------------
public:
	/**
		* @fn void clearResults()
		* @brief clears results
		*/
	void clearResults();

	/**
		* @fn unsigned int getNumberOfResults()
		* @brief gets the number of results
		* @return the number of results
		*/
	unsigned int getNumberOfResults();

	/**
		* @fn SearchResult* getResult( const unsigned int idx )
		* @briefi gets result
		* @param idx result index
		* @return result (If NULL, the index is illegal.)
		*/
	SearchResult* getResult( const unsigned int idx );

	/**
		* @fn void updateResults()
		* @brief update results
		*/
	void updateResults();

	const bool getNumberOfSearchedResults(std::vector<unsigned int>& ids);

//--------------------------------------------------------------------------------
//     ACTIVE RECORD
//--------------------------------------------------------------------------------

	const bool getSearchRecordDetailsFromMassBank(
		const int idx,
		const bool needDetailes
		);

	// レコードの詳細をストアする
	void setSearchRecordDetails(
		const unsigned int ctrlIdx,
		const std::string& recordId,
		const std::string& record_title_name,
		const std::string& recordInfo,
		const bool setDetaile);

	void setMoreRecordDetailes(RecordInfo& tgt);
	void setMoreRecordDetailestoActiveRecordInfo();

	//// 対象の情報からPNGを作る。
	const bool createChemicalFormulaPNG(RecordInfo& tgt);

	/**
		* @fn void unsetActiveRecord()
		* @brief unset active record
		*/
	void unsetActiveRecord();

	const char* getActiveRecordInfo();
	const char* getActiveRecordTitleName();
	kome::core::XYData* getActiveRecordPeaks();
	const std::string getActiveRecordPngPath();

	// Search Result Listのステータスを保存
	void setSearchResultDialogShowFlag(const unsigned int flags);

	// Search Result Listのステータスを戻す
	unsigned int getSearchResultDialogShowFlag(void) const;


	// アクティブなIDが存在するかどうか。
	const bool isValidActiveId(const int idx);

	const bool searchKeywordFromDetailes(const std::string& keyword);

//--------------------------------------------------------------------------------
//     Draw Peaks
//--------------------------------------------------------------------------------
	///**
	//	* @fn void drawRecordPeaks( kome::img::Graphics& g, const double scale = 1.0 )
	//	* @brief draws record peaks
	//	* @param g graphics object to draw
	//	* @param scale the scale of y-axis
	//	*/
	void drawRecordPeaks(kome::img::Graphics& g);
	//	const double scale = 1.0);

	///**
	//	* @fn bool setToDrawToActiveRecord( kome::img::Graphics& g, kome::objects::Spectrum* spec )
	//	* @brief draws Active Canvas to target Graphics
	//	* @param g graphics object to draw
	//	* @param called Spectrum object (to use calc scale )
	//	*/
	bool setToDrawToActiveRecord(
		kome::img::Graphics& g,
		kome::objects::Spectrum* spec);

//--------------------------------------------------------------------------------
//     From Dialogs Control Throw MassBank Make XML
//--------------------------------------------------------------------------------

public:
	/**
 	 * @brief searches similar spectra
	 * @param peaks peaks information
	 * @param settings objects
	 * @return If true, searching is succeeded
	 */
	bool searchSimilarSpectra(
		kome::core::XYData& peaks,
		kome::objects::SettingParameterValues* settings
	);

	std::string batchSearch(
		const std::string& queryTags,
		kome::objects::SettingParameterValues* settings);

	/**
		* @brief searches peaks
		* @param diff true: differential peak search, false: peak search
		* @param mzs m/z array
		 * @param settings objects
		*/
	bool searchPeaks(
		const bool diff,
//		std::vector< double >& mzs,
		kome::objects::SettingParameterValues* settings);

	/**
		* @brief gets record information
		* @param id record ID
		* @param info string object to store record information
		*/
	bool getRecordInfo(
		const char* id, 
		std::string& info);

	/**
		* @brief gets peak information
		* @param id record ID
		* @param xyData xy data object to store peak information
		*/
	bool getPeakInfo(
		const char* id,
		kome::core::XYData& xyData);
			
	/**
		* @fn void getBatchResults( const char* id )
		* @brief gets batch results
		* @param id job ID
		*/
	const bool getBatchResults( const char* id );

	const int getJobStatus(const char* jobId);

	MassBankSearchEngine* getEngine();

	const std::string getQueryStringTag(
		const char* specname,
		kome::objects::Peaks* peaks);

//--------------------------------------------------------------------------------
//     Only Manager Methods
//--------------------------------------------------------------------------------
	bool callCreateMassBankRecord( kome::objects::DataGroupNode* group );

	/**
		* @fn bool callSimilarSpectraSearchDialog( kome::objects::Spectrum* spec )
		* @brief searches similar spectra
		* @param spec active spectrum
		* @return If false, searching similar spectra is failed.
		*/
	bool callSimilarSpectraSearch( kome::objects::Spectrum* spec );

	/**
	* @fn bool callPeakSearchDialog( const bool diff )
	* @brief searches peaks
	* @param diff true: differential peak search, false: peak search
	* @return If false, searching peaks is failed.
	*/
	bool callPeakSearch( const bool diff );

//--------------------------------------------------------------------------------
//     Pane Controls
//--------------------------------------------------------------------------------					
	/**
		* @fn void createMassBankSearchResultsPane()
		* @brief create MassBank Search Results Pane
		*/
	void createMassBankSearchResultsPane();

	/**
		* @fn void showMassBankSearchResultsPane
		* @brief show MassBank Search Results Pane
		*/
	void showMassBankSearchResultsPane();

	/**
		* @fn void toggleOverlappingSettingsViewVisibility()
		* @brief toggles the visibility of the MassBank Search Results Pane
		*/
	void toggleMassBankSearchResultsPaneVisibility();

	/**
		* @fn bool isVisibileMassBankSearchResultsPane()
		* @brief checks the visibility of the MassBank Search Results Pane
		* @return If true, the MassBank Search Results Pane is visible.
		*/
	bool isVisibileMassBankSearchResultsPane();

	/**
		* @fn void releaseMassBankSearchResultsPane()
		* @brief releases MassBankSearchResultsPane
		*/
	void releaseMassBankSearchResultsPane();

	/**
		* @fn void createMassRecordPane()
		* @brief create MassBank Record Pane
		*/
	void createMassBankRecordPane(const char* titleStr = NULL);

	/**
		* @fn void showMassBankRecordPane()
		* @brief show MassBank Record Pane
		*/
	void showMassBankRecordPane(const char* titleStr = NULL);

	/**
		* @fn void toggleMassBankRecordPaneVisibility()
		* @brief toggles the visibility of the MassBank Record Pane
		*/
	void toggleMassBankRecordPaneVisibility();

	/**
		* @fn bool isVisibileMassBankRecordPane()
		* @brief checks the visibility of the MassBank Record Pane
		* @return If true, the MassBank Record Pane is visible.
		*/
	bool isVisibileMassBankRecordPane(const char* titleStr = NULL);

	/**
		* @fn void releaseMassBankRecordPane()
		* @brief releases MassBankRecordPane
		*/			
	void releaseMassBankRecordPane();

	/**
		* @fn bool callWarningDialog()
		* @brief call Warning Dialog When do not Connect to MassBank.
		* @return if select YES. 
		*/
	const bool callWarningDialog();
public:
	/**
	//	* @fn bool getInstrumentTypesFromMassBank( std::vector< std::string >& instruments )
	//	* @brief gets instrument types
	//	* @param instruments the array to store instrument types
	//	* @return If false, it failed to get instrument types.
	//	*/
	bool getInstrumentTypesFromMassBank( std::vector< std::string >& instruments );

	// Get 
	bool getVenderArraytoEnumItems(std::vector<kome::plugin::SettingsValue::EnumItem>*&);
	bool getInstrumentArraytoEnumItems(std::vector<kome::plugin::SettingsValue::EnumItem>*&);
	bool getLicenseArraytoEnumItems(std::vector<kome::plugin::SettingsValue::EnumItem>*&);

	bool getGettedInstrumentTypeArraytoEnumItems(std::vector<kome::plugin::SettingsValue::EnumItem>*&, bool hasFree);

	bool getAppendedValuesArraytoEnumItems(std::vector<kome::plugin::SettingsValue::EnumItem>*&);

	bool createMassBankRecordonBatchMode(kome::objects::Parameters* params);

protected:
	
	const bool checkGroupInfo(kome::objects::DataGroupNode* group, bool popWindow = false);
	const bool checkMySetupInfo(unsigned int flags);

//--------------------------------------------------------------------------------
//     ANY Controls
//--------------------------------------------------------------------------------		
protected:

	const bool onCheckMassBankConnect();

	/**
		* @fn wxPoint getDialogStandardPt(kome::core::Point< int >* tgtPos, const int& mgSize)
		* @brief set position target Dialog
		* @param[in] tgtPos target Now Position
		* @param[in] msSize if over Window Range. Reset move Size.
		* @return wxPoint to show Dialog position.
		*/
	wxPoint getDialogStandardPt(kome::core::Point< int >* tgtPos, const int& mgSize);

	/**
		* @fn static bool lessSearchResult( SearchResult r0, SearchResult r1 )
		* @brief compares to sort search results
		* @param r0 search result to compare
		* @param r1 search result to be compared
		* @return If true, r0 sorts before r1.
		*/
	static bool lessSearchResult( const SearchResult& r0, const SearchResult& r1 );

	static void sortSearchResultQuery( SearchResult& r0 );

	/**
		* @fn const std::string getMethodFromMassBank ( const char* method, const char* tmpFileName, const char* contentReq );
		* @brief get Use Method & content Request Call Mass Bank. to Write result tmp.xml
		* @param method Use Method
		* @param tmpFileName temp file Name Head (XXX.xml) : This : XXX
		* @param contentReq content Request String.(Add Status)
		* @return tmpFileName ( Full Path )
		*/

	const std::string onGetMethodFromMassBank(const char* tmpFileName);

public:
	InstrumentMgr* getInstMgr(){return instMgr_.get();}

private:
	kome::objects::Peaks* getHasPeaks(kome::objects::Spectrum* spec);
	kome::objects::Peaks* onGetSpectrumPeak(kome::objects::Spectrum* spec);
	const std::string onCreateTempFilePath(const char* tmpFileName);
	kome::window::MainFrame* onGetMyFrame();

	const bool onAdptSearchDialogCall(
		int Fg,
		kome::objects::SettingParameterValues& settings,
		kome::objects::Spectrum* spec = nullptr);
};

} // end of name space
}

#endif	// __KOME_MASBANK_MANAGER_H__
