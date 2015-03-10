/**
 * @file SelectRangeManager.h
 * @brief interfaces of SelectRangeManager class
 *
 * @author S.Tanaka
 * @date 2007.03.02
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_OPERATION_RANGE_SELECT_RANGE_MANAGER_H__
#define __KOME_OPERATION_RANGE_SELECT_RANGE_MANAGER_H__

#include <utility>
#include <map>
#include <stack>

          
namespace kome {
	namespace operation {		
		namespace range {
			/**
			 * @class SelectRangeManager
			 */
			class SelectRangeManager {
			protected:
				/**
				 * @fn SelectRangeManager()
				 * @brief constructor
				 */
				SelectRangeManager();

				/**
				 * @fn virtual ~SelectRangeManager()
				 * @brief destructor
				 */
				virtual ~SelectRangeManager();

			protected:
				/** spectrum range map */
				std::map< kome::objects::Spectrum*, std::pair< double, double > > m_specRangeMap;
				/** chromatogram range map */
				std::map< kome::objects::Chromatogram*, std::pair< double, double > > m_chromRangeMap;

				/** active data */
				struct {
					void* obj;
					bool chromFlag;
					int startPos;
				} m_activeData;

				/** px */
				int m_px;
				/** py */
				int m_py;
				/** button down flag */
				bool m_btnDown;

				/** spectrum range actions */
				std::vector< kome::plugin::PluginCall* > m_specRangeActions;

				/** chromatogram range actions */
				std::vector< kome::plugin::PluginCall* > m_chromRangeActions;

				/** the start of the range */
				double m_start;

				/** the end of the range */
				double m_end;

				/** spectrum range menu */
				wxMenu* m_specMenu;

				/** chromatogram range menu */
				wxMenu* m_chromMenu;
			
			public:
				/**
				 * @fn void close( kome::objects::Spectrum& spec )
				 * @brief closes spectrum
				 * @param spec spectrum to be closed
				 */
				void close( kome::objects::Spectrum& spec );

				/**
				 * @fn void close( kome::objects::Chromatogram& chrom )
				 * @brief closes chromatogram
				 * @param chrom chromatogram to be closed
				 */
				void close( kome::objects::Chromatogram& chrom );

				/**
				 * @fn bool downButton(
						kome::objects::Spectrum& spec,
						kome::evt::MouseEvent& evt,
						kome::img::Graphics& graphics
					)
				 * @brief button down event
				 * @param spec spectrum object
				 * @param evt mouse event information object
				 * @param graphics graphics object
				 * @return refresh flag
				 */
				bool downButton(
					kome::objects::Spectrum& spec,
					kome::evt::MouseEvent& evt,
					kome::img::Graphics& graphics
				);

				/**
				 * @fn bool downButton(
						kome::objects::Chromatogram& chrom,
						kome::evt::MouseEvent& evt,
						kome::img::Graphics& graphics
					)
				 * @brief button down event
				 * @param chrom chromatogram object
				 * @param evt mouse event information object
				 * @param graphics graphics object
				 * @return refresh flag
				 */
				bool downButton(
					kome::objects::Chromatogram& chrom,
					kome::evt::MouseEvent& evt,
					kome::img::Graphics& graphics
				);

				/**
				 * @fn bool moveMouse(
						kome::objects::Spectrum& spec,
						kome::evt::MouseEvent& evt,
						kome::img::Graphics& graphics
					)
				 * @brief mouse move event
				 * @param spec spectrum object
				 * @param evt mouse event information object
				 * @param graphics graphics object
				 * @return refresh flag
				 */
				bool moveMouse(
					kome::objects::Spectrum& spec,
					kome::evt::MouseEvent& evt,
					kome::img::Graphics& graphics
				);

				/**
				 * @fn bool moveMouse(
						kome::objects::Spectrum& spec,
						kome::evt::MouseEvent& evt,
						kome::img::Graphics& graphics
					)
				 * @brief mouse move event
				 * @param chromatogram object
				 * @param evt mouse event information object
				 * @param graphics graphics object
				 * @return refresh flag
				 */
				bool moveMouse(
					kome::objects::Chromatogram& chrom,
					kome::evt::MouseEvent& evt,
					kome::img::Graphics& graphics
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
						kome::img::Graphics& graphics
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
				 * @fn bool setValStatusBar(
				 		kome::evt::MouseEvent& evt,
				 		kome::img::Graphics& graphics,
				 		const wxWindow* canvas,
						kome::objects::Spectrum* spec,
					    kome::objects::Chromatogram* chrom
				 	)
				 * @brief ステータスバーにマウスの位置を表示する
				 * @param evt mouse event information object
				 * @param graphics graphics object
				 * @param canvas canva ID
				 * @param spec spectrum object
				 * @param chrom chromatogram object
				 */				
				bool setValStatusBar(
					kome::evt::MouseEvent& evt,
					kome::img::Graphics& graphics,
					wxWindow* canvas,
					kome::objects::Spectrum* spec,
					kome::objects::Chromatogram* chrom
				);
				
				/**
				 * @fn bool isPeakLabelEditMode()
				 * @brief Peak EditモードのON/OFF判定
				 * @ return true : ON / false :OFF
				 */
				bool isPeakLabelEditMode();

			protected:
				/**
				 * @fn void setSelectedRange(
				 		std::pair< double, double >& range,
				 		int startPos,
				 		int nowPos,
				 		kome::img::Graphics& graphics
				 	)
				 * @brief sets selected range
				 * @param range pair to store selected range
				 * @param startPos dragging start position (x coordinate)
				 * @param nowPos now dragging position (x coordinate)
				 * @param graphics graphics object
				 */
				void setSelectedRange(
					std::pair< double, double >& range,
					int startPos,
					int nowPos,
					kome::img::Graphics& graphics
				);
				
				/**
				* @fn void setMouseCursor(
				*		kome::img::Graphics& graphics, 
						kome::evt::MouseEvent& evt, 
						const char* pluginName , 
						const char* iconName 
				  )
				* @brief マウスカーソルのアイコンを設定する	
				* @param graphics graphics object
				* @param evt mouse event information object
				* @param 
				*/
				void setMouseCursor(
					kome::img::Graphics& graphics, 
					kome::evt::MouseEvent& evt, 
					const char* pluginName , 
					const char* iconName 
				);

				/**
				 * @fn void setMouseMoveLine( 
				         kome::img::Graphics& graphics,
						 wxWindow* canvas,
						 kome::obects::Spectrum* spec, 
						 kome::objects::Chromatogram* chrom
						)
				 * @brief マウス移動時のラインを設定する
				 * @param graphics graphics object
				 * @param canvas canvas ID
				 * @param spec spectrum object
				 * @param chrom chromatogram object
				 */
				void setMouseMoveLine( 
					kome::img::Graphics& graphics, 
					wxWindow* canvas,
					kome::objects::Spectrum* spec,
					kome::objects::Chromatogram* chrom
					);

			public:

				/**
				 * @fn void drawForeground(
						kome::objects::Spectrum& spec,
						kome::img::Graphics& graphics,
						wxWindow* canvas
					)
				 * @brief draws spectrum foreground
				 * @param spec spectrum object
				 * @param graphics graphics object
				 * @param canvas canvas ID
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
				 * @param graphics graphics object
				 * @param canvas canvas ID
				 */
				void drawForeground(
					kome::objects::Chromatogram& chrom,
					kome::img::Graphics& graphics,
					wxWindow* canvas
				);
				
				/**
				 * @fn void drawBackground(
						kome::objects::Spectrum& spec,
						kome::img::Graphics& graphics
					)
				 * @brief draws spectrum background
				 * @param spec spectrum object
				 * @param graphics graphics object
				 */
				void drawBackground(
					kome::objects::Spectrum& spec,
					kome::img::Graphics& graphics	
				);
			
				/**
				 * @fn void drawBackground(
						kome::objects::Chromatogram& chrom,
						kome::img::Graphics& graphics
					)
				 * @brief draws chromatogram background
				 * @param chrom chromatogram object
				 * @param graphics graphics object
				 */
				void drawBackground(
					kome::objects::Chromatogram& chrom,
					kome::img::Graphics& graphics
				);

				/**
				 * @fn void drawForeground(
						std::pair< double, double >& range,
						kome::img::Graphics& graphics
					)
				 * @brief draws foreground
				 * @param range selected range
				 * @param graphics graphics object
				 */
				void drawForeground(
					std::pair< double, double >& range,
					kome::img::Graphics& graphics
				 );

				/**
				 * @fn void drawBackground(
						std::pair< double, double >& range,
						kome::img::Graphics& graphics
					)
				 * @brief draws background
				 * @param range selected range
				 * @param graphics graphics object
				 */
				void drawBackground(
					std::pair< double, double >& range,
					kome::img::Graphics& graphics
				 );

			protected:
				/**
				 * @fn void loadActions()
				 * @brief loads actions
				 */
				void loadActions();

			public:
				/**
				 * @fn void setCurrentRange( const double start, const double end )
				 * @brief sets the current range
				 * @param[in] start the start of the range.
				 * @param[in] end the end of the range.
				 */
				void setCurrentRange( const double start, const double end );

			protected:
				/**
				 * @fn void createSpecRangeMenu( wxMenu& menu )
				 * @brief creates spectrum range menu
				 * @param[out] menu menu object
				 */
				void createSpecRangeMenu( wxMenu& menu );

				/**
				 * @fn void createChromRangeMenu( wxMenu& menu )
				 * @brief creates chromatogram range menu
				 * @param[out] menu menu object
				 */
				void createChromRangeMenu( wxMenu& menu );

			public:
				/**
				 * @fn wxMenu* getSpecRangeMenu()
				 * @brief gets the spectrum range menu
				 * @return spectrum range menu
				 */
				wxMenu* getSpecRangeMenu();

				/**
				 * @fn wxMenu* getChromRangeMenu()
				 * @brief gets the chromatogram range menu
				 * @return chromatogram range menu
				 */
				 wxMenu* getChromRangeMenu();

			public:
				/**
				 * @fn static bool actionSpecOrChromRange( kome::objects::Variant obj, int id )
				 * @brief spectrum or chromatogram range action
				 * @param[in] obj active object
				 * @param[in] id menu ID
				 * @return If true, the specified range action is succeeded.
				 */
				static bool actionSpecOrChromRange( kome::objects::Variant obj, int id );

			public:
				/**
				 * @fn static SelectRangeManager& getInstance()
				 * @brief gets change range plug-in management object
				 * @return change range plug-in management object (This is the only object.)
				 */
				static SelectRangeManager& getInstance();

			};
		}
	}
}

#endif		// __KOME_OPERATION_RANGE_SELECT_RANGE_MANAGER_H__

