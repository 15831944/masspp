/**
 * @file ProfileRangeManager.h
 * @brief interfaces of ProfileRangeManager class
 *
 * @author S.Tanaka
 * @date 2009.01.07
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_OPERATION_RANGE_PROFILE_RANGE_MANAGER_H__
#define __KOME_OPERATION_RANGE_PROFILE_RANGE_MANAGER_H__


#include <utility>
#include <map>
#include <stack>
#include <vector>

//2012/02/28 add by t.okuno -> This plugin displays a range choice screen.
#include "ChangeDisplayedRangeDialog.h"
using namespace kome::operation::sel_range;
//2012/02/28 add by t.okuno <- This plugin displays a range choice screen.


namespace kome {
	namespace operation {
		namespace range {
					
			//class ProfileRangeOperation;
			class DataMapRangeManager;	// @date 2012/11/30 <Add> OKADA

			/**
			 * @class ProfileRangeManager
			 * @brief change range plug-in management class
			 */
			class ProfileRangeManager : public kome::objects::DefaultDataManager {
	
			protected:
				/**
				 * @fn ProfileRangeManager()
				 * @brief constructor
				 */
				ProfileRangeManager();

				/**
				 * @fn virtual ~ProfileRangeManager()
				 * @brief destructor
				 */
				virtual ~ProfileRangeManager();

			public:
				/**
				 * @struct RangeInfo
				 * @brief range information
				 */
				struct RangeInfo {
					std::stack< std::pair< double, double > > xRange;
					std::stack< std::pair< double, double > > yRange;
				};
			
				// dialog type
				enum DIALOG_TYPE{
					DIALOG_MZ	= 0,
					DIALOG_RT,
					DIALOG_TYPE_NUM
				};
				
			protected:
				/** range map */
				std::map< wxWindow*, RangeInfo > m_rangeMap;

				/** changing range data */
				struct {
					wxWindow* canvas;
					bool chromFlag;
					bool yAxisFlag;
					int startPos;
					int endPos;
				} m_changing;

				/** key event data*/
				struct {
					bool up;
					bool down;
					bool left;
					bool right;
				}m_key_flag;

				/** Data Range Store */
				struct datarange {
					wxWindow* canvas;
					double    minX;
					double    maxX;
					double    minY;
					double    maxY;
				};

				/** Chromatgram Window Ranges */
				std::vector<datarange> m_chromdata;
				
				/** Spectram Window Ranges */
				std::vector<datarange> m_specdata;

				/** Ranges Pointer */
				datarange* m_range;
								
				/** synchronized range flag */
				bool m_sync;

				wxWindow* m_canvas;

				/** chromatogram synchronized range information */
				RangeInfo m_syncChromRange;

				/** spectrum synchronized range information */
				RangeInfo m_syncSpecRange;
			
				//2012/02/28 add by t.okuno -> This plugin displays a range choice screen.
				/** Change Displayed Range dialog(m/z) */
				ChangeDisplayedRangeDialog*	m_pdlgChangeDisplayedRangeMz;

				/** Change Displayed Range dialog(RT) */
				ChangeDisplayedRangeDialog*	m_pdlgChangeDisplayedRangeRT;
				//2012/02/28 add by t.okuno <- This plugin displays a range choice screen.

			public:
				/**
				 * @fn void prepare(
						kome::objects::Spectrum& spec,
						kome::img::Graphics& graphics
						wxWindow* canvas
					)
				 * @brief prepares to draw spectrum
				 * @param graphics graphics object
				 * @param canvas canvas
				 */
				void prepare(
					kome::objects::Spectrum& spec,
					kome::img::Graphics& graphics,
					wxWindow* canvas
				);

				/**
				 * @fn void prepare(
						kome::objects::Chromatogram& chrom,
						kome::img::Graphics& graphics
						wxWindow* canvas
					)
				 * @brief prepares to draw chromatogram
				 * @param graphics graphics object
				 * @param canvas canvas
				 */
				void prepare(
					kome::objects::Chromatogram& chrom,
					kome::img::Graphics& graphics,
					wxWindow* canvas
				);


				/**
				 * @fn void close( wxWindow* canvas )
				 * @brief closes
				 * @param canvas canvas
				 */
				void close( wxWindow* canvas );

				/**
				 * @fn bool downButton(
						kome::objects::Spectrum& spec,
						kome::evt::MouseEvent& evt,
						kome::img::Graphics& graphics,
						wxWindow* canvas
					)
				 * @brief button down event
				 * @param spec spectrum object
				 * @param evt mouse event information object
				 * @param graphics graphics object
				 * @param canvas canvas ID
				 * @return refresh flag 
				 */
				bool downButton(
					kome::objects::Spectrum& spec,
					kome::evt::MouseEvent& evt,
					kome::img::Graphics& graphics,
					wxWindow* canvas
				);

				/**
				 * @fn bool downButton(
						kome::objects::Chromatogram& chrom,
						kome::evt::MouseEvent& evt,
						kome::img::Graphics& graphics,
						wxWindow* canvas
					)
				 * @brief button down event
				 * @param chrom chromatogram object
				 * @param evt mouse event information object
				 * @param graphics graphics object
				 * @param canvas canvas ID
				 * @return refresh flag 
				 */
				bool downButton(
					kome::objects::Chromatogram& chrom,
					kome::evt::MouseEvent& evt,
					kome::img::Graphics& graphics,
					wxWindow* canvas
				);

				/**
				 * @fn bool moveMouse(
						kome::objects::Spectrum& spec,
						kome::evt::MouseEvent& evt,
						kome::img::Graphics& graphics,
						wxWindow* canvas
					)
				 * @brief mouse move event
				 * @param spec spectrum object
				 * @param evt mouse event information object
				 * @param graphics graphics object
				 * @param canvas canvas
				 * @return refresh flag
				 */
				bool moveMouse(
					kome::objects::Spectrum& spec,
					kome::evt::MouseEvent& evt,
					kome::img::Graphics& graphics,
					wxWindow* canvas
				);

				/**
				 * @fn bool moveMouse(
						kome::objects::Spectrum& spec,
						kome::evt::MouseEvent& evt,
						kome::img::Graphics& graphics,
						wxWindow* canvas
					)
				 * @brief mouse move event
				 * @param chromatogram object
				 * @param evt mouse event information object
				 * @param graphics graphics object
				 * @param canvas canvas
				 * @return refresh flag
				 */
				bool moveMouse(
					kome::objects::Chromatogram& chrom,
					kome::evt::MouseEvent& evt,
					kome::img::Graphics& graphics,
					wxWindow* canvas
				);

				/**
				 * @fn bool upButton(
						kome::objects::Spectrum& spec,
						kome::evt::MouseEvent& evt,
						kome::img::Graphics& graphics,
						wxWindow* canvas						
					)
				 * @brief button up event
				 * @param spec spectrum object
				 * @param evt mouse event information object
				 * @param graphics graphics object
				 * @param canvas canvas
				 * @return refresh flag
				 */
				bool upButton(
					kome::objects::Spectrum& spec,
					kome::evt::MouseEvent& evt,
					kome::img::Graphics& graphics,
					wxWindow* canvas						
				);

				/**
				 * @fn bool upButton(
						kome::objects::Chromatogram& chrom,
						kome::evt::MouseEvent& evt,
						kome::img::Graphics& graphics,
						wxWindow* canvas						
					)
				 * @brief button up event
				 * @param chrom chromatogram object
				 * @param evt mouse event information object
				 * @param graphics graphics object
				 * @param canvas canvas
				 * @return refresh flag
				 */
				bool upButton(
					kome::objects::Chromatogram& chrom,
					kome::evt::MouseEvent& evt,
					kome::img::Graphics& graphics,
					wxWindow* canvas						
				);

				/**
				 * @fn bool dblClick(
						kome::objects::Spectrum& spec,
						kome::evt::MouseEvent& evt,
						kome::img::Graphics& graphics,
						wxWindow* canvas
					)
				 * @brief double click event
				 * @param spec spectrum object
				 * @param evt mouse event information object
				 * @param graphics graphics object
				 * @param canvas canvas
				 * @return refresh flag
				 */
				bool dblClick(
					kome::objects::Spectrum& spec,
					kome::evt::MouseEvent& evt,
					kome::img::Graphics& graphics,
					wxWindow* canvas
				);

				/**
				 * @fn bool dblClick(
						kome::objects::Chromatogram& chrom,
						kome::evt::MouseEvent& evt,
						kome::img::Graphics& graphics,
						wxWindow* canvas
					)
				 * @brief double click event
				 * @param chrom chromatogram object
				 * @param evt mouse event information object
				 * @param graphics graphics object
				 * @param canvas canvas
				 * @return refresh flag
				 */
				bool dblClick(
					kome::objects::Chromatogram& chrom,
					kome::evt::MouseEvent& evt,
					kome::img::Graphics& graphics,
					wxWindow* canvas
				);

				/**
				 * @fn bool keyDown(
						kome::evt::KeyEvent& evt,
						kome::img::Graphics& graphics,
						wxWindow* canvas
					)
				 * @brief keybord down event
				 * @param evt keybord event information object
				 * @param graphics graphics object
				 * @param canvas canvas
				 * @return refresh flag
				 */
				bool keyDown(
					kome::evt::KeyEvent& evt,
					kome::img::Graphics& graphics,
					wxWindow* canvas
				);

				/**
				 * @fn void drawForeground(
						kome::objects::Spectrum& spec,
						kome::img::Graphics& graphics,
						wxWindow* canvas
					)
				 * @brief draws spectrum foreground
				 * @param spec spectrum object
				 * @param canvas canvas
				 * @param graphics graphics object
				 */
				void drawForeground(
					kome::objects::Spectrum& spec,
					kome::img::Graphics& graphics,
					wxWindow* canvas
				);

				/**
				 * @fn void drawForeground(
						kome::objects::Chromatogram& chrom,
						kome::img::Graphics& graphics,
						wxWindow* canvas
					)
				 * @brief draws chromatogram foreground
				 * @param chrom chromatogram object
				 * @param canvas canvas
				 * @param graphics graphics object
				 */
				void drawForeground(
					kome::objects::Chromatogram& chrom,
					kome::img::Graphics& graphics,
					wxWindow* canvas
				);

				/**
				 * @fn void drawBackground(
						kome::objects::Spectrum& spec,
						kome::img::Graphics& graphics,
						wxWindow* canvas
					)
				 * @brief draws spectrum background
				 * @param spec spectrum object
				 * @param graphics graphics object
				 * @param canvas canvas
				 */
				void drawBackground(
					kome::objects::Spectrum& spec,
					kome::img::Graphics& graphics,
					wxWindow* canvas
				);

				/**
				 * @fn void drawBackground(
						kome::objects::Chromatogram& chrom,
						kome::img::Graphics& graphics,
						wxWindow* canvas
					)
				 * @brief draws chromatogram background
				 * @param chrom chromatogram object
				 * @param graphics graphics object
				 * @param canvas canvas
				 */
				void drawBackground(
					kome::objects::Chromatogram& chrom,
					kome::img::Graphics& graphics,
					wxWindow* canvas
				);

				/** 
				 * @fn bool setValStatusBar( kome::evt::MouseEvent& evt, kome::img::Graphics& graphics, wxWindow* canvas )
				 * @brief ステータスバーの表示設定	
				 * @param evt mouse event information object
				 * @param graphics graphics object
				 * @return refresh flag
				 */
				bool setValStatusBar( 
					kome::evt::MouseEvent& evt, 
					kome::img::Graphics& graphics,
					wxWindow* canvas
				);

				//2012/02/28 add by t.okuno -> This plugin displays a range choice screen.
				/**
				 * @fn void createChangeSpectrumRange( kome::objects::Spectrum* spec,
													   const double startMz,
													   const double endMz )
				 * @brief creates Change Displayed Spectrum Range dialog
				 * @param spec source spectrum
				 * @param canvas target canvas id
				 * @param startMz start m/z
				 * @param endMz end m/z
				 */
				void createChangeSpectrumRange( kome::objects::Spectrum* spec,
												wxWindow* canvas,
												const double startMz,
												const double endMz );

				/**
				 * @fn void createChangeChromatogramRange( kome::objects::Chromatogram* chrom,
														   const double startRT,
														   const double endRT )
				 * @brief creates Change Displayed Chromatogram Range dialog
				 * @param chrom source chromatogram
				 * @param canvas target canvas id
				 * @param startRT start RT
				 * @param endRT end RT
				 */
				void createChangeChromatogramRange( kome::objects::Chromatogram* chrom,
													wxWindow* canvas,
													const double startRT,
													const double endRT );
				//2012/02/28 add by t.okuno <- This plugin displays a range choice screen.

			public:
				/**
				 * @fn RangeInfo* getRange(wxWindow* canvas)
				 * @brief gets range information
				 * @param canvas target canvas id
				 * @return range information
				 */
				RangeInfo* getRange(wxWindow* canvas);

			protected:
				/**
				 * @fn RangeInfo* getRange()
				 * @brief gets range information
				 * @return range information
				 */
				RangeInfo* getRange();				

				/**
				 * @fn bool setDownButtonInfo(
						kome::evt::MouseEvent& evt,
						kome::img::Graphics& graphics
					)
				 * @brief sets button down information
				 * @param evt mouse event information object
				 * @param graphics graphics object
				 * @return refresh flag
				 */
				bool setDownButtonInfo(
					kome::evt::MouseEvent& evt,
					kome::img::Graphics& graphics
				);

				/**
				 * @fn bool setMoveMouseInfo( kome::evt::MouseEvent& evt, kome::img::Graphics& graphics )
				 * @brief sets mouse move information
				 * @param evt mouse event information object
				 * @param graphics graphics object
				 * @return refresh flag
				 */
				bool setMoveMouseInfo( kome::evt::MouseEvent& evt, kome::img::Graphics& graphics );

				/**
				 * @fn bool setUpButtonInfo( kome::evt::MouseEvent& evt, kome::img::Graphics& graphics )
				 * @brief sets button up information
				 * @param evt mouse event information object
				 * @param graphics graphics object
				 * @param canvas target canvas id
				 * @return refresh flag
				 */
				bool setUpButtonInfo( kome::evt::MouseEvent& evt, kome::img::Graphics& graphics, wxWindow* canvas );

				/**
				 * @fn bool keyEvtControler(
						kome::evt::KeyEvent& evt,
						kome::img::Graphics& graphics,
						wxWindow* canvas
					)
				 * @brief Press Keybord event control
				 * @param evt keybord event information object
				 * @param graphics graphics object
				 * @param canvas canvas
				 * @return refresh flag
				 */
				bool KeyEvtControler(
					kome::evt::KeyEvent& evt,
					kome::img::Graphics& graphics,
					wxWindow* canvas
				);

				/**
				 * @fn bool KeyEvtMoveWindow(
						kome::evt::KeyEvent& evt,
						kome::img::Graphics& graphics,
						wxWindow* canvas
					)
				 * @brief Window Move from Keybord Event
				 * @param evt keybord event information object
				 * @param graphics graphics object
				 * @param canvas canvas
				 * @return refresh flag
				 */
				bool KeyEvtMoveWindow(
					kome::evt::KeyEvent& evt,
					kome::img::Graphics& graphics,
					wxWindow* canvas
				);

				/**
				 * @fn bool KeyEvtReSizeWindow(
						kome::evt::KeyEvent& evt,
						kome::img::Graphics& graphics,
						wxWindow* canvas
					)
				 * @brief resize Window  from Keybord Event
				 * @param evt keybord event information object
				 * @param graphics graphics object
				 * @param canvas canvas
				 * @return refresh flag
				 */				
				bool KeyEvtReSizeWindow(
					kome::evt::KeyEvent& evt,
					kome::img::Graphics& graphics,
					wxWindow* canvas
				);

				/**
				 * @fn void drawForeground( kome::img::Graphics& graphics )
				 * @brief draws foreground
				 * @param graphics graphics object
				 */
				void drawForeground( kome::img::Graphics& graphics );

				/**
				 * @fn void drawBackground( kome::img::Graphics& graphics )
				 * @brief draws background
				 * @param graphics graphics object
				 */
				void drawBackground( kome::img::Graphics& graphics );

			public:
				/**
				 * @fn void toggleChromRangeStatus()
				 * @brief toggles chromatogram range status
				 */
				void toggleRangeStatus();

			protected:
				/**
				 * @fn static void getIntensityRange(
						kome::core::XYData& xyData,
						const double minX,
						const double maxX,
						double* minY,
						double* maxY
					)
				 * @brief get max intensity in the specified x range
				 * @param xyData data points information object
				 * @param minX minimum x value
				 * @param maxX maximum x value
				 * @param minY the pointer to store minimum intensity
				 * @param maxY the pointer to store maximum intensity
				 */
				static void getIntensityRange(
					kome::core::XYData& xyData,
					const double minX,
					const double maxX,
					double* minY,
					double* maxY
				);

				// @date 2011.05.19<Add> M.Izumi ->
				// 2.0.0で1つ前の拡大範囲に戻す処理をしないので、スタックでためている必要がなくなったため
				// 範囲が1以上の時は削除してから追加するように変更
				/**
				 * @fn void clearStack()
				 * @brief clear stack
				 */
				void  clearStack();
				// @date 2011.05.19<Add> M.Izumi <-

				/**
				 * @fn void setRangeStatusMsg( double min, double max )
				 * @brief ステータスバーに指定範囲を表示する
				 * @param min minimum x or y value
				 * @param max max maximum x or y value
				 */
				void setRangeStatusMsg( double min, double max );

				/**
				 * @fn bool TranseProc( RangeInfo* rangeInfo,  double moveRatio )
				 * @brief パン処理
				 * @param rangeInfo range information
				 * @param graphics graphics object
				 */
				bool TranseProc( RangeInfo* rangeInfo, kome::img::Graphics& graphics );

				/**
				 * @fn bool ZoomProc( kome::img::Graphics& graphics, RangeInfo* rangeInfo,  double zoomRatio, int iePos )
				 * @brief ズーム処理
				 * @param graphics graphics object
				 * @param rangeInfo range information
				 * @param iePos
				 */
				bool ZoomProc( kome::img::Graphics& graphics, RangeInfo* rangeInfo, int iePos );

				/**
				 * @fn void setMouseCursor( bool bbtn )
				 * @brief マウスカーソルのアイコン設定
				 * @param bbtn　true:ズーム / false:パン
				 */
				void setMouseCursor( bool bbtn );
				
				/**
				 * @fn bool calcXRange( kome::img::Graphics& graphics, double& minPt, double& maxPt, int iePos )
				 * @brief X軸ズーム処理の計算部分
				 * @param graphics graphics object
				 * @parma minPt min point
				 * @param maxPt max point
				 * @param iePos
				 */
				bool calcXRange( kome::img::Graphics& graphics, double& minPt, double& maxPt, int iePos );
				
				/**
				 * @fn bool calcYRange( kome::img::Graphics& graphics, double& minPt, double& maxPt, int iePos )
				 * @brief Y軸ズーム処理の計算部分
				 * @param graphics graphics object
				 * @parma minPt min point
				 * @param maxPt max point
				 * @param iePos
				 */
				bool calcYRange( kome::img::Graphics& graphics, double& minPt, double& maxPt, int iePos );
				
				/**
				 * @fn bool calcXRange( kome::img::Graphics& graphics, RangeInfo* rangeInfo, double&min, double& max )
				 * @brief X軸パン処理の計算部分
				 * @param graphics graphics object
				 * @param rangeInfo range information
				 * @parma min min point
				 * @param max max point
				 */
				bool calcXRange( kome::img::Graphics& graphics, RangeInfo* rangeInfo, double&min, double& max );
				
				/**
				 * @fn bool calcYRange( kome::img::Graphics& graphics, RangeInfo* rangeInfo, double&min, double& max )
				 * @brief Y軸パン処理の計算部分
				 * @param graphics graphics object
				 * @param rangeInfo range information
				 * @parma min min point
				 * @param max max point
				 */
				bool calcYRange( kome::img::Graphics& graphics, RangeInfo* rangeInfo, double&min, double& max );
			public:
				/**
				 * @fn void pushSpectrumXRange( wxWindow* canvas, const double start, const double end )
				 * @brief pushes spectrum x range
				 * @param canvas canvas
				 * @param start the start of range
				 * @param end the end of range
				 */
				void pushSpectrumXRange( RangeInfo* rangeInfo, wxWindow* canvas, const double start, const double end );

				/**
				 * @fn void pushSpectrumYRange( wxWindow* canvas, const double start, const double end )
				 * @brief pushes spectrum y range
				 * @param canvas canvas
				 * @param start the start of range 
				 * @param end the end of range
				 */
				void pushSpectrumYRange( RangeInfo* rangeInfo, wxWindow* canvas, const double start, const double end );

				/**
				 * @fn void pushChromatogramXRange( wxWindow* canvas, const double start, const double end )
				 * @brief pushes chromatogram x range
				 * @param canvas canvas
				 * @param start the start of range
				 * @param end the end of range
				 */
				void pushChromatogramXRange( RangeInfo* rangeInfo, wxWindow* canvas, const double start, const double end );

				/**
				 * @fn void pushChromatogramYRange( wxWindow* canvas, const double start, const double end )
				 * @brief pushes chromatogram y range
				 * @param canvas Canvas ID
				 * @param start the start of range
				 * @param end the end of range
				 */
				void pushChromatogramYRange( RangeInfo* rangeInfo, wxWindow* canvas, const double start, const double end );
			
			protected:
				/**
				 * @fn int getIndex( kome::evt::MouseEvent& evt, kome::img::Graphics& graphics )
				 * @brief get index
				 * @parma evt
				 * @param graphics
				 * @return index
				 */
				int getIndex( kome::evt::MouseEvent& evt, kome::img::Graphics& graphics );
		
			public:
				/**
				 * @fn wxWindow* getCanvas( kome::objects::Variant obj )
				 * @brief get canvas
				 * @param obj
				 * @return active canvas
				 */
				wxWindow* getCanvas( kome::objects::Variant obj );

				/**
				 * @fn void resetZoom( RangeInfo& rangeInfo, const int index )
				 * @brief rest zoom 
				 * @param rangeInfo
				 * @param index
				 */
				void resetZoom( RangeInfo& rangeInfo, const int index );

			public:
				/**
				 * @fn static ProfileRangeManager& getInstance()
				 * @brief gets change range plug-in management object
				 * @return change range plug-in management object (This is the only object.)
				 */
				static ProfileRangeManager& getInstance();
			};
		}
	}
}


#endif		// __KOME_OPERATION_RANGE_PROFILE_RANGE_MANAGER_H__
