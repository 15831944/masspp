/**
 * @file DataMapRangeManager.h
 * @brief interfaces of DataMapRangeManager class
 *
 * @author S.Tanaka
 * @date 2009.01.07
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_OPERATION_RANGE_DATAMAP_RANGE_MANAGER_H__
#define __KOME_OPERATION_RANGE_DATAMAP_RANGE_MANAGER_H__


#include <utility>
#include <map>
#include <stack>


namespace kome {
	namespace operation {
		namespace range {
			
			/**
			 * @class DataMapRangeManager
			 * @brief change range plug-in management class
			 */
			class DataMapRangeManager : public kome::objects::DefaultDataManager {
			protected:
				/**
				 * @fn DataMapRangeManager()
				 * @brief constructor
				 */
				DataMapRangeManager();

				/**
				 * @fn virtual ~DataMapRangeManager()
				 * @brief destructor
				 */
				virtual ~DataMapRangeManager();
				
			public:
				/**
				* @struct RangeInfo
				* @brief range information
				*/
				struct RangeInfo {
					std::stack< std::pair< double, double > > xRange;
					std::stack< std::pair< double, double > > yRange;
				};
			protected:				

				/** range map */
				std::map< kome::objects::DataMapInfo*, RangeInfo > m_rangeMap;

				/**
				 * @struct DataInfo
				 * @brief data information
				 */
				struct DataInfo {
					kome::objects::DataGroupNode* group;
					double minRt;
					double maxRt;
					double minMz;
					double maxMz;
					std::string path;
				};

				/** data information array */
				std::vector< DataInfo > m_dataArray;

				/** active data */
				struct {
					kome::objects::DataMapInfo* dataMap;
					int startPos;
					int endPos;
				} m_changing;

				/** synchronized data map range flag */
				bool m_syncDataMap;

				/** data map synchronized range information */
				RangeInfo m_syncDataMapRange;

				/** tmp file dir */
				std::string m_tmpDir;

				/** rt range */
				std::pair< double, double > m_rtRange;
				/** m/z range */
				std::pair< double, double > m_mzRange;
				
			private:
				/** right click flag */
				bool m_flgClickRight;

				/** datamap range actions */
				std::vector< kome::plugin::PluginCall* > m_datamapRangeActions;

				/** datamap range menu */
				wxMenu* m_datamapMenu;

			public:				
				/** 左クリック開始がRT領域 */
				bool m_flgRt;

				/** 左クリック開始がm/z領域 */
				bool m_flgMz;

			public:
				/**
				 * @fn bool onOpen( kome::objects::DataMapInfo& dataMap, wxWindow* canvas )
				 * @brief create data map
				 * @param dataMap data map
				 * @return If true, it succeeded to prepare data map.
				 */
				bool onOpen( kome::objects::DataMapInfo& dataMap );

				/**
				 * @fn void prepare(
						kome::objects::DataMapInfo& dataMap,
						kome::img::Graphics& graphics,
						wxWindow* canvas
					)
				 * @brief prepares to draw data map
				 * @param dataMap data map information object
				 * @param graphics graphics object
				 * @param canvas canvas
				 */
				void prepare(
					kome::objects::DataMapInfo& dataMap,
					kome::img::Graphics& graphics,
					wxWindow* canvas
				);

				/**
				 * @fn void close( kome::objects::DataMapInfo* dataMap )
				 * @brief closes
				 * @param dataMap data map information object
				 */
				void close( kome::objects::DataMapInfo* dataMap );

				/**
				 * @fn bool downButton(
						kome::objects::DataMapInfo& dataMap,
						kome::evt::MouseEvent& evt,
						kome::img::Graphics& graphics,
						wxWindow* canvas
					)
				 * @brief button down event
				 * @param dataMap data map object
				 * @param evt mouse event information object
				 * @param graphics graphics object
				 * @param canvas canvas
				 * @return refresh flag
				 */
				bool downButton(
					kome::objects::DataMapInfo& dataMap,
					kome::evt::MouseEvent& evt,
					kome::img::Graphics& graphics,
					wxWindow* canvas
				);

				/**
				 * @fn bool moveMouse(
						kome::objects::DataMapInfo& dataMap,
						kome::evt::MouseEvent& evt,
						kome::img::Graphics& graphics,
						wxWindow* canvas						
					)
				 * @brief mouse move event
				 * @param dataMap data map
				 * @param evt mouse event information object
				 * @param graphics graphics object
				 * @param canvas canvas
				 * @return refresh flag
				 */
				bool moveMouse(
					kome::objects::DataMapInfo& dataMap,
					kome::evt::MouseEvent& evt,
					kome::img::Graphics& graphics,
					wxWindow* canvas						
				);

				
				/**
				 * @fn void loadActions()
				 * @brief loads actions
				 */
				void loadActions();

				/**
				 * @fn static bool actionDatamap( kome::objects::Variant obj, int id )
				 * @brief datamap range action
				 * @param[in] obj active object
				 * @param[in] id menu ID
				 * @return If true, the specified range action is succeeded.
				 */
				static bool actionDatamap( kome::objects::Variant obj, int id );

				/**
				 * @fn void createDatamapRangeMenu( wxMenu& menu )
				 * @brief creates datamap range menu
				 * @param[out] menu menu object
				 */
				void createDatamapRangeMenu( wxMenu& menu );

				/**
				 * @fn wxMenu* getDatamapRangeMenu()
				 * @brief gets the datamap range menu
				 * @return datamap range menu
				 */
				wxMenu* getDatamapRangeMenu();

				/**
				 * @fn bool upButton(
						kome::objects::DataMapInfo& dataMap,
						kome::evt::MouseEvent& evt,
						kome::img::Graphics& graphics,
						wxWindow* canvas						
					)
				 * @brief button up event
				 * @param dataMap data map
				 * @param evt mouse event information object
				 * @param graphics graphics object
				 * @param canvas canvas
				 * @return refresh flag
				 */
				bool upButton(
					kome::objects::DataMapInfo& dataMap,
					kome::evt::MouseEvent& evt,
					kome::img::Graphics& graphics,
					wxWindow* canvas						
				);

				/**
				 * @fn void drawForeground(
						kome::objects::DataMapInfo& dataMap,
						kome::img::Graphics& graphics,
						wxWindow* canvas
					)
				 * @brief draws data map foreground
				 * @param dataMap data map object
				 * @param graphics graphics object
				 * @param canvas canvas
				 */
				void drawForeground(
					kome::objects::DataMapInfo& dataMap,
					kome::img::Graphics& graphics,
					wxWindow* canvas
				);

				/**
				 * @fn void openChangeDispRangeDlg( kome::objects::DataMapInfo& dataMap, wxWindow* canvas )
				 * @brief open change displayed range dialog
				 * @param dataMap data map info
				 * @param canvas
				 */
				void openChangeDispRangeDlg( kome::objects::DataMapInfo& dataMap, wxWindow* canvas );
		
			public:				
				/**
				 * @fn void setChangingDataMapInfo( kome::objects::DataMapInfo* dataMapInfo )
				 * @brief set changing datamap info
				 * @param dataMapInfo
				 */
				void setChangingDataMapInfo( kome::objects::DataMapInfo* dataMapInfo );

				/**
				 * @fn bool setRange( 
					kome::objects::DataMapInfo* dataMap,
					wxWindow* canvas,
					double minX,
					double maxX,
					double minY, 
					double maxY )
				 * @brief set range
				 * @param dataMap datamap info
				 * @param canvas canvas
				 * @param minX min X
				 * @param maxX max X
				 * @param minY min Y
				 * @param maxY max Y
				 */
				bool setRange(// RangeInfo range, 
					kome::objects::DataMapInfo* dataMap,
					wxWindow* canvas,
					double minX,
					double maxX,
					double minY, 
					double maxY );
				
				/**
				 * @fn RangeInfo* getRange()
				 * @brief gets range information
				 * @return range information
				 */
				RangeInfo* getRange( kome::objects::DataMapInfo* dataMap );

			 protected:
				/**
				 * @fn RangeInfo* getRange()
				 * @brief gets range information
				 * @return range information
				 */
				RangeInfo* getRange();
				        
				/**
				 * @fn DataInfo* getData(
							kome::objects::DataGroupNode* group,
							const double minRt,
							const double maxRt,
							const double minMz,
							const double maxMz
						)
				 * @brief gets data information
				 * @param group group
				 * @param minRt minimum RT
				 * @param maxRt maximum RT
				 * @param minMz minimum m/z
				 * @param maxMz maximum m/z
				 */
				DataInfo* getData(
					kome::objects::DataGroupNode* group,
					const double minRt,
					const double maxRt,
					const double minMz,
					const double maxMz
				);

				/**
				 * @fn void saveData( kome::objects::DataMapInfo& dataMap )
				 * @brief saves data
				 * @param dataMap data map object to save
				 */
				void saveData( kome::objects::DataMapInfo& dataMap );
				
			public:
				/**
				 * @fn void toggleDataMapRangeStatus()
				 * @brief toggles data map range status
				 */
				void toggleDataMapRangeStatus();

			protected:
				/** 
				 * @fn bool setDataMapRange( kome::objects::DataMapInfo* dataMap, wxWindow* canvas )
				 * @brief sets data map range
				 * @param dataMap data map
				 * @param canvas canvas
				 * @param bflgプログレスダイアログを表示しないためのフラグ true：非表示
				 * @return If true, it succeeded to set data map range
				 */
				bool setDataMapRange( kome::objects::DataMapInfo* dataMap, wxWindow* canvas, bool bflg=false );

			protected:
				/**
				 * @fn bool setSynchronizedDataMapRange( kome::objects::DataMapInfo* appendedMap )
				 * @brief sets synchronized data map range
				 * @brief appendedMap appended data map
				 * @return If true, it succeeded to set synchronized range
				 */
				bool setSynchronizedDataMapRange( kome::objects::DataMapInfo* appendedMap );

			protected:
				/**
				 * @fn virtual void onCloseSample( kome::objects::Sample* sample, const bool deleting )
				 * @brief This method is called when a sample is closed. (override method)
				 * @param[in] sample sample object to be closed
				 * @param[in] deleting If true, the specified object is being deleted now.
				 */
				virtual void onCloseSample( kome::objects::Sample* sample, const bool deleting );

			public:
				/**
				 * @fn bool resetZoomDatamapFromContextMenuManager( void );
				 * @brief Datamap画面の拡大を初期状態に戻す
				 * @param dataMap data map
				 * @param canvas canvas
				 */
				bool resetZoomDatamapFromContextMenuManager( kome::objects::DataMapInfo& dataMap, wxWindow* canvas );

			private:
				
				/**
				 * @fn void getRtRange( double* dFirst, double* dSecond );
				 * @brief RTの範囲を取得
				 * @param [out] dFirst  RT first
				 * @param [out] dSecond RT second
				 */
				void getRtRange( double* dFirst, double* dSecond );

				/**
				 * @fn void getMzRange( double* dFirst, double* dSecond );
				 * @brief m/zの範囲を取得
				 * @param [out] dFirst  RT first
				 * @param [out] dSecond RT second
				 */
				void getMzRange( double* dFirst, double* dSecond );

			public:
				/**
				 * @fn bool dblClick(
						kome::objects::DataMapInfo& dataMap,
						kome::evt::MouseEvent& evt,
						kome::img::Graphics& graphics,
						wxWindow* canvas						
					)
				 * @brief button double click event
				 * @param dataMap data map
				 * @param evt mouse event information object
				 * @param graphics graphics object
				 * @param canvas canvas
				 * @return refresh flag
				 */
				bool dblClick(
					kome::objects::DataMapInfo& dataMap,
					kome::evt::MouseEvent& evt,
					kome::img::Graphics& graphics,
					wxWindow* canvas						
				);

			public:
				/**
				 * @fn static DataMapRangeManager& getInstance()
				 * @brief gets change range plug-in management object
				 * @return change range plug-in management object (This is the only object.)
				 */
				static DataMapRangeManager& getInstance();
			};
		}
	}
}


#endif		// __KOME_OPERATION_RANGE_DATAMAP_RANGE_MANAGER_H__
