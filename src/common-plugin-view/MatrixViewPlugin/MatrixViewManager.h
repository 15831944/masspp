/**
 * @file MatrixViewManager.h
 * @brief interfaces of MatrixViewManager class
 *
 * @author S.Tanaka
 * @date 2009.11.24
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_MATRIX_VIEW_MANAGER_H__
#define __KOME_MATRIX_VIEW_MANAGER_H__

typedef std::map<std::string, int> GroupIdTable;	// @date 2013/07/22 <Add> OKADA

// Peak Filter	// @date 2013/07/23 <Add> OKADA
typedef struct {
	std::string strIntensityValueType;
	std::string strIntensityRange;
	std::string strRTRange;
	std::string strMzRange;
} peak_filter_t;

namespace kome {
	namespace matrix {
		namespace view {

			class QuantitationDialog;
			class CreatePeakMatrixDialog;

			/**
			 * @class MatrixViewManager
			 * @brief matrix view manager
			 */
			class MatrixViewManager {
			protected:
				/**
				 * @fn MatrixViewManager()
				 * @brief constructor
				 */
				MatrixViewManager();

				/**
				 * @fn virtual ~MatrixViewManager()
				 * @brief destructor
				 */
				virtual ~MatrixViewManager();

			protected:
				/** matrix dialog */
				QuantitationDialog* m_dlg;

				/** create peak matrix dialo */
				CreatePeakMatrixDialog* m_peakMatrixDlg;

			private:
				// for command line
				/** map group-id */
				GroupIdTable m_map_group_id;	// @date 2013/07/22 <Add> OKADA

				// for command line
				/** peak filter info */
				peak_filter_t m_peak_filter_info;	// @date 2013/07/23 <Add> OKADA

			public:
				// @date 2013/08/14
				// command line等、画面を開かないバージョン
				typedef struct {
					int nColumnNo;
					std::string strTitle;
					kome::window::GridListCtrl::ColumnDataType type;
				}column_t;
			private:
				std::vector< column_t > m_vColumn;

				/** data array */
				std::vector< long long > m_data;

			protected:

				/** appended base column */
				int m_appendedColFMLC;

				/** appended columns */
				std::vector< int > m_appendedColsFMLC;

				/** charge column */
				int m_chargeColFMLC;

				/** m/z column */
				int m_mzColFMLC;

				/** RT column */
				int m_rtColFMLC;

				/** samples */
				std::vector< int > m_samplesFMLC;

				/** substance column */
				int m_substanceColFMLC;

				/** substance map */
				std::map< int, std::string > m_substanceMapFMLC;





			public:
				/**
				 * @fn GroupIdTable& getMapGroupId( void )
				 * @brief get map_group_id
				 * @return reference of m_map_group_id.
				 */
				GroupIdTable& getMapGroupId( void ){ return m_map_group_id; };	// @date 2013/07/22 <Add> OKADA

				/**
				 * @fn peak_filter_t& getPeakFilterInfo( void )
				 * @brief get m_peak_filter_info
				 * @return reference of m_peak_filter_info.
				 */
				peak_filter_t& getPeakFilterInfo( void ){ return m_peak_filter_info; }	// @date 2013/07/23 <Add> OKADA

				/**
				 * @fn void setPeakFilterInfoIntValType( const char* szVal )
				 * @brief set Peak Filter information Intensity Val Type
				 * @param szVal string of Intensity Value Type
				 */
				void setPeakFilterInfoIntValType( const char* szVal ){
					m_peak_filter_info.strIntensityValueType = szVal;
				}

				/**
				 * @fn void setPeakFilterInfoIntensityRange( const char* szVal )
				 * @brief set Peak Filter information Intensity Range
				 * @param szVal string of Intensity Range
				 */
				void setPeakFilterInfoIntensityRange( const char* szVal ){
					m_peak_filter_info.strIntensityRange = szVal;
				}

				/**
				 * @fn void setPeakFilterInfoRTRange( const char* szVal )
				 * @brief set Peak Filter information RT Range
				 * @param szVal string of RT Range
				 */
				void setPeakFilterInfoRTRange( const char* szVal ){
					m_peak_filter_info.strRTRange = szVal;
				}

				/**
				 * @fn void setPeakFilterInfoMzRange( const char* szVal )
				 * @brief set Peak Filter information Mz Range
				 * @param szVal string of m/z Range
				 */
				void setPeakFilterInfoMzRange( const char* szVal ){
					m_peak_filter_info.strMzRange = szVal;
				}

			public:
				/**
				 * @fn void releaseDialog()
				 * @brief releases matrix dialog
				 */
				void releaseDialog();

				/**
				 * @fn void togglenDialog()
				 * @brief toggle visibility of the matrix dialog
				 */
				void toggleDialog();

				/**
				 * @fn bool isShownDialog()
				 * @brief judges the matirx dialog is opened
				 * @return If true, the matrix dialog is opened.
				 */
				bool isShownDialog();

				/**
				 * @fn void updateDialog()
				 * @brief updates dialog
				 */
				void updateDialog();
				
				// MatrixList、MatrixComboBoxがQuontitation Dialogにあるため、Managerクラスで更新、
				// 設定などを取得出来るように変更 @date 2012.11.19 M.Izumi
				/**
				 * @fn void updateMatrixCombo()
				 * @brief upate matrix combobox
				 */
				void updateMatrixCombo();
				/**
				 * @fn void setProject()
				 * @brief set project
				 */
				void setProject();
				/**
				 * @fn void updateMatrixList()
				 * @brief update matrix list
				 */
				void updateMatrixList();

				/**
				 * @fn bool MatrixListIsEmpty()
				 * @brief matrix is empty
				 * @return is empty
				 */
				bool MatrixListIsEmpty();
				/**
				 * @fn void refreshDialog()
				 * @brief refreshes dialog
				 */
				void refreshDialog();

				/**
				 * @fn void activatePeak( const int peakId )
				 * @brief activates the specified matrix peak
				 * @param peakId peak ID
				 */
				void activatePeak( const int peakId );
				/**
				 * @fn bool canCloseDialog()
				 * @brief check whether can close dialog
				 */
				bool canCloseDialog();

				/**
				 * @fn void openPeakMatrixDialog( const bool bedit = false )
				 * @brief open peak matrix dialog
				 * @param bedit 編集用のダイアログの時 TRUEを返す
				 */
				void openPeakMatrixDialog( const bool bedit = false );

				/**
				 * @fn void releasePeakMatrixDialog()
				 * @brief release peak matrix dialog
				 */
				void releasePeakMatrixDialog();

				/**
				 * @fn CreatePeakMatrixDialog* getPeakMatrixDlg()
				 * @brief get the peak matrix dialog
				 * @return createPeakMatrixDialog
				 */
				CreatePeakMatrixDialog* getPeakMatrixDlg();

				/**
				 * @fn void onClosePeakInfoDlg()
				 * @brief 表示中のPeakInfoダイアログのClose処理
				 */
				void onClosePeakInfoDlg();

				/**
				 * @fn kome::objects::Variant saveTextPeakMatrixDataFileDlg( kome::objects::Parameters* params )
				 * @brief ピークマトリクスのテキスト出力。ダイアログを開いて指定ファイルを作成
				 * @return variant型の戻り値
				 */
				kome::objects::Variant saveTextPeakMatrixDataFileDlg( kome::objects::Parameters* params );	// @date 2013/08/14 <Add> OKADA

				/**
				 * @fn kome::objects::Variant saveTextPeakMatrixDataFile( const char *szFilePath, kome::objects::Parameters* params )
				 * @brief ピークマトリクスのテキスト出力
				 * @param[in] path file path
				 * @return variant型の戻り値
				 */
				kome::objects::Variant saveTextPeakMatrixDataFile( const char *szFilePath, kome::objects::Parameters* params=NULL );	// @date 2013/08/14 <Add> OKADA

				/**
				 * @fn void addColumn( const char* title, const ColumnDataType type, const int width, kome::matrix::view::MatrixListCtrl* pMatrixList )
				 * @brief カラムの追加処理
				 * @param[in] title column title
				 * @param[in] type column data type
				 * @param[in] width column width
				 * @param[in] ptr pointer of MatrixListCtrl(画面がない場合はNULL)
				 */
				void addColumn( const char* title, const kome::window::GridListCtrl::ColumnDataType type, void* ptr=NULL, const int width=80 );	// @date 2013/08/14 <Add> OKADA

				/**
				 * @fn void MatrixViewManager::addData( long long data, const bool update, void* ptr )
				 * @brief dataの追加処理
				 * @param[in] data list data
				 * @param[in] update update flag
				 * @param[in] ptr pointer of MatrixListCtrl(画面がない場合はNULL)
				 */
				void MatrixViewManager::addData( long long data, const bool update, void* ptr=NULL );

				/**
				 * @fn void MatrixViewManager::writeData1Line( int nPeekCnt, FILE* fp )
				 * @brief Peak Matrix データ1行分の出力処理
				 * @param[in] nPeekCnt ピークマトリクス行番号
				 * @param[in] fp ファイルポインタ
				 */
				void MatrixViewManager::writeData1Line( int nPeekCnt, FILE* fp );

			public:
				/**
				 * @fn static MatrixViewManager& getInstance()
				 * @brief gets MatrixViewManager object (This is the only object.)
				 * @return matrix view manager object
				 */
				static MatrixViewManager& getInstance();

			};
		}
	}
}


#endif	// __KOME_MATRIX_VIEW_MANAGER_H__
