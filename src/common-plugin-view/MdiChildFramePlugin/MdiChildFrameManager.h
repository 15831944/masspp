/**
 * @file MdiChildFrameManager.h
 * @brief interfaces of MdiChildFrameManager class
 *
 * @author S.Tanaka
 * @date 2007.09.23
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_VIEW_MDI_CHILD_FRAME_MANAGER_H__
#define __KOME_VIEW_MDI_CHILD_FRAME_MANAGER_H__


#define MDI_WINDOW_STATUS_NAME		 "WINDOW_MODE"
#define MDI_WINDOW_STATUS_VALUE		 "MDI"
#define AUTO_ARRANGE_STATUS_NAME     "MDI_AUTO_ARRANGE_MODE"
#define AUTO_ARRANGE_STATUS_VALUE    "on"


namespace kome {
	namespace view {
		/**
		 * @class MdiChildFrameManager
		 * @brief AppndChildFrame plug-in management class
		 */
		class MdiChildFrameManager : public kome::objects::DefaultDataManager {
		protected:
			/**
			 * @fn MdiChildFrameManager()
			 * @brief constructor
			 */
			MdiChildFrameManager();

			/**
			 * @fn virtual ~MdiChildFrameManager()
			 * @brief destructor
			 */
			virtual ~MdiChildFrameManager();

		protected:
			/** frame set */
			std::set< kome::window::ChildFrame* > m_frameSet;

			/** moving frame */
			kome::window::ChildFrame* m_movingChild;

			/** moving position */
			kome::core::Point< int > m_movingPos;

			/** move position */
			kome::core::Point< int > m_movePos;

			/** start rect */
			kome::core::Rect< int > m_startRect;

			/** client size */
			kome::core::Point< int > m_clientSize;

			/** flag */
			volatile bool m_flg;

			/** moving flag */
			bool m_moving;

			/** free frame set */
			std::set< kome::window::ChildFrame* > m_freeFrameSet;

		public:
			/**
			 * @fn void openSpectrum( kome::objects::Spectrum* spectrum )
			 * @brief opens spectrum data
			 * @param[in] spectrum spectrum object to be opened.
			 */
			void openSpectrum( kome::objects::Spectrum* spectrum );

			/**
			 * @fn void openChromatogram( kome::objects::Chromatogram* chrom )
			 * @brief opens chromatogram data
			 * @param[in] chrom chromatogram object to be opened.
			 */
			void openChromatogram( kome::objects::Chromatogram* chrom );

			/**
			 * @fn void openDataMap( kome::objects::DataMapInfo* dataMap )
			 * @brief opens data map
			 * @param[in] dataMap data map information object to be opened.
			 */
			void openDataMap( kome::objects::DataMapInfo* dataMap );

			/**
			 * @fn void closeAllSpecFrames()
			 * @brief closes spectrum frames
			 */
			void closeAllSpecFrames();

			/**
			 * @fn void closeAllChromFrames()
			 * @brief closes chromatogram frames
			 */
			void closeAllChromFrames();

			/**
			 * @fn void closeDataMapFrames()
			 * @brief closed data map frames
			 */
			void closeDataMapFrames();

		protected:
			/**
			 * @fn kome::window::ChildFrame* getSpectrumFrame( kome::objects::Spectrum* spec )
			 * @brief gets specified spectrum frame
			 * @param spec spectrum
			 * @return spectrum frame (If NULL, specified spectrum frame doesn't exist.)
			 */
			kome::window::ChildFrame* getSpectrumFrame( kome::objects::Spectrum* spec );

			/**
			 * @fn kome::window::ChildFrame* getChromatogramFrame( kome::objects::Chromatogram* chrom )
			 * @brief gets specified chromatogram frame
			 * @param chrom chromatogram 
			 * @return chromatogram frame (If NULL, specified chromatogram frame doesn't exist.)
			 */
			kome::window::ChildFrame* getChromatogramFrame( kome::objects::Chromatogram* chrom );

			/**
			 * @fn kome::window::ChildFrame* getDataMapFrame( kome::objects::DataMapInfo* dataMap )
			 * @brief gets specified data map frame
			 * @param dataMap data map
			 * @return data map frame (If NULL, specified data map frame doesn't exist.)
			 */
			kome::window::ChildFrame* getDataMapFrame( kome::objects::DataMapInfo* dataMap );

			/**
			 * @fn kome::window::ChildFrame* getChildFrame( wxWindow* childWnd )
			 * @brief gets child frame that has specified child window
			 * @param childWnd child window
			 * @return child frame (If NULL, there is no child frame that has specified child window.)
			 */
			kome::window::ChildFrame* getChildFrame( wxWindow* childWnd );

		public:
			/**
			 * @fn void updateStyle()
			 * @brief update child frame style
			 */
			void updateStyle();

		public:
			/**
			 * @fn void setMovingPosition( kome::window::ChildFrame* child, const int px, const int py )
			 * @brief sets the moving position
			 * @param[in] child child frame
			 * @param[in] px x coordinate of the position
			 * @param[in] py y coordinate of the position
			 */
			void setMovingPosition( kome::window::ChildFrame* child, const int px, const int py );

			/**
			 * @fn void setMovePosition( kome::window::ChildFrame* child, const int px, const int py )
			 * @brief sets the move position
			 * @param[in] child child frame
			 * @param[in] px x coordinate of the position
			 * @param[in] py y coordinate of the position
			 */
			void setMovePosition( kome::window::ChildFrame* child, const int px, const int py );

			/**
			 * @fn void setClientSize( const int w, const int h )
			 * @brief sets the client size
			 * @param[in] w the width of the client window
			 * @param[in] h the height of the client window
			 */
			void setClientSize( const int w, const int h );
			
			/**
			 * @fn void layoutChildren( kome::window::ChildFrame* closingChild, const bool sortFlg = true )
			 * @brief layouts child frames
			 * @param[in] closingChild closing child frame
			 * @param[in] sortFlg sort flag
			 */
			void layoutChildren( kome::window::ChildFrame* closingChild, const bool sortFlg = true );

			/**
			 * @fn void moveChild( kome::window::ChildFrame* child, const int px, const int py )
			 * @brief move the child frame
			 * @param[in] child child frame
			 * @param[in] px x coordinate of the position
			 * @param[in] py y coordinate of the position
			 */			 
			void moveChild( kome::window::ChildFrame* child, const int px, const int py );

		public:
			/**
			 * @fn void tileVertically()
			 * @brief tile child frames vertically
			 */
			void tileVertically();

		public:
			/**
			 * @fn bool setAutoArrangeStatus()
			 * @brief sets the auto arrange status
			 * @return If false, the operation is canceled.
			 */
			bool setAutoArrangeStatus();

		protected:
			/**
			 * @fn virtual void onCloseSample( kome::objects::Sample* sample, const bool deleting )
			 * @brief This method is called when a sample is closed. (override method)
			 * @param sample sample object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void onCloseSample( kome::objects::Sample* sample, const bool deleting );

		protected:
			/**
			 * @fn static bool lessChildX( kome::window::ChildFrame* child0, kome::window::ChildFrame* child1 )
			 * @brief compare child frames to sort by x coordinate of frame position
			 * @param[in] child0 child frame object to compare
			 * @param[in] child1 child frame object to be compared
			 * @return If true, child0 sorts before child1
			 */
			static bool lessChildX( kome::window::ChildFrame* child0, kome::window::ChildFrame* child1 );

			/**
			 * @fn static bool lessChildY( kome::window::ChildFrame* child0, kome::window::ChildFrame* child1 )
			 * @brief compare child frames to sort by y coordinate of frame position
			 * @param[in] child0 child frame object to compare
			 * @param[in] child1 child frame object to be compared
			 * @return If true, child0 sorts before child1
			 */
			static bool lessChildY( kome::window::ChildFrame* child0, kome::window::ChildFrame* child1 );

		public:
			/**
			 * @fn static MdiChildFrameManager& getInstance()
			 * @brief gets object
			 * @return object (This is the only object.)
			 */
			static MdiChildFrameManager& getInstance();

		};
	}
}

#endif	// __KOME_VIEW_MDI_CHILD_FRAME_MANAGER_H__
