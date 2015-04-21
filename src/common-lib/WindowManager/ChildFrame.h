/**
 * @file ChildFrame.h
 * @brief interfaces of ChildFrame class
 *
 * @author S.Tanaka
 * @date 2006.09.11
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_WINDOW_CHILD_FRAME_H__
#define __KOME_WINDOW_CHILD_FRAME_H__


#include "WindowTool.h"

#include <set>
#include <wx/mdi.h>


namespace kome {
	namespace window {

		class Canvas;

		/**
		 * @class ChildFrame
		 * @brief child frame class
		 */
		class WINDOW_MANAGER_CLASS ChildFrame : public wxMDIChildFrame {
		public:
			/**
			 * @fn ChildFrame( const int px = -1, const int py = -1, const int w = -1, const int h = -1, const long style = wxDEFAULT_FRAME_STYLE )
			 * @brief constructor
			 * @param[in] px the x coordinate of the window position
			 * @param[in] py the x coordinate of the window position
			 * @param[in] w the width of the window
			 * @param[in] h the height of the window
			 * @param[in] style frame style 
			 */
			ChildFrame( const int px = -1, const int py = -1, const int w = -1, const int h = -1, const long style = wxDEFAULT_FRAME_STYLE );

			/**
			 * @fn virtual ~ChildFrame()
			 * @brief destructor
			 */
			virtual ~ChildFrame();

		protected:
			/** canvas */
			wxWindow* m_canvas;

			/** spectra */
			std::vector< kome::objects::Spectrum* > m_spectra;

			/** chromatograms */
			std::vector< kome::objects::Chromatogram* > m_chroms;

			/** data maps */
			std::vector< kome::objects::DataMapInfo* > m_datamaps;

			/** sample */
			kome::objects::Sample* m_sample;

			/** group */
			kome::objects::DataGroupNode* m_group;

			/** active object */
			kome::objects::Variant* m_acObj;

			/** fixed size */
			std::pair< int, int > m_fixedSize;

		protected:
			/** frame array */
			static std::vector< ChildFrame* > m_frames;

		public:
			/**
			 * @fn void setCanvas( wxWindow* canvas )
			 * @brief sets canvas
			 * @param[in] canvas canvas
			 */
			void setCanvas( wxWindow* canvas );

			/**
			 * @fn wxWindow* getCanvas()
			 * @brief sets canvas
			 * @return canvas
			 */
			wxWindow* getCanvas();

		public:
			/**
			 * @fn void setFixedSize( const int width, const int height )
			 * @brief sets the fixed size
			 * @param[in] width the width of the fixed size
			 * @param[in] height the height of the fixed size
			 */
			void setFixedSize( const int width, const int height );

			/**
			 * @fn int getFixedWidth()
			 * @brief the width of the fixed size
			 * @return the width of the fixed size
			 */
			int getFixedWidth();

			/**
			 * @fn int getFixedHeight()
			 * @brief the height of the fixed size
			 * @return the height of the fixed size
			 */
			int getFixedHeight();

		public:
			/**
			 * @fn void clearObjects()
			 * @brief clears objects
			 */
			void clearObjects();

			/**
			 * @fn void clearSpectra()
			 * @brief clears spectra
			 */
			void clearSpectra();

			/**
			 * @fn void clearChromatograms()
			 * @brief clears chromatograms
			 */
			void clearChromatograms();

			/**
			 * @fn void clearDataMaps()
			 * @brief clears data maps
			 */
			void clearDataMaps();

			/**
			 * @fn void setSpectrum( kome::objects::Spectrum* spec )
			 * @brief sets spectrum
			 * @param[in] spec spectrum
			 */
			void setSpectrum( kome::objects::Spectrum* spec );

			/**
			 * @fn void setChromatogram( kome::objects::Chromatogram* chrom )
			 * @brief sets chromatogram
			 * @param[in] chrom chromatogram
			 */
			void setChromatogram( kome::objects::Chromatogram* chrom );

			/**
			 * @fn void setDataMap( kome::objects::DataMapInfo* dataMap )
			 * @brief sets data map
			 * @param[in] dataMap data map
			 */
			void setDataMap( kome::objects::DataMapInfo* dataMap );

			/**
			 * @fn void addSpectrum( kome::objects::Spectrum* spec )
			 * @brief adds spectrum
			 * @param[in] spec spectrum
			 */
			void addSpectrum( kome::objects::Spectrum* spec );

			/**
			 * @fn void addChromatogram( kome::objects::Chromatogram* chrom )
			 * @brief adds chromatogram
			 * @param[in] chrom chromatogram
			 */
			void addChromatogram( kome::objects::Chromatogram* chrom );

			/**
			 * @fn void addDataMap( kome::objects::DataMapInfo* dataMap )
			 * @brief adds data map
			 * @param[in] dataMap data map
			 */
			void addDataMap( kome::objects::DataMapInfo* dataMap );

			/**
			 * @fn unsigned int getNumberOfSpectra()
			 * @brief gets the number of spectra
			 * @return the number of spectra
			 */
			unsigned int getNumberOfSpectra();

			/**
			 * @fn kome::objects::Spectrum* getSpectrum( const unsigned int index )
			 * @brief gets spectrum
			 * @param[in] index spectrum index
			 * @return spectrum (If NULL, any spectra are not or the index is illegal.)
			 */
			kome::objects::Spectrum* getSpectrum( const unsigned int index );

			/**
			 * @fn unsigned int getNumberOfChromatograms()
			 * @brief gets the number of chromatograms
			 * @return the number of chromatograms
			 */
			unsigned int getNumberOfChromatograms();

			/**
			 * @fn kome::objects::Chromatogram* getChromatogram( const unsigned int index )
			 * @brief gets chromatogram
			 * @param[in] index chromtogram index
			 * @return chromatogram (If NULL, any chromatograms are not or the index is illegal.)
			 */
			kome::objects::Chromatogram* getChromatogram( const unsigned int index );

			/**
			 * @fn unsigned int getNumberOfDataMaps()
			 * @brief gets the number of data maps
			 * @return the number of data maps
			 */
			unsigned int getNumberOfDataMaps();

			/**
			 * @fn kome::objects::DataMapInfo* getDataMap( const unsigned int index )
			 * @brief gets data map
			 * @param[in] index data map index
			 * @return data map (If NULL, any data maps are not or the index is illegal.)
			 */
			kome::objects::DataMapInfo* getDataMap( const unsigned int index );

			/**
			 * @fn kome::objects::Sample* getSample()
			 * @brief gets sample
			 * @return sample (If NULL, a sample is not active.)
			 */
			kome::objects::Sample* getSample();

			/**
			 * @fn kome::objects::DataGroupNode* getGroup()
			 * @brief gets spectrum group
			 * @return spectrum group (If NULL, a spectrum group is not active.)
			 */
			kome::objects::DataGroupNode* getGroup();

			/**
			 * @fn kome::objects::DataSet* getFilteredDataSet()
			 * @brief gets filtered spectra
			 * @return filtered spectra (If NULL, a spectrum group is not active.)
			 */
			kome::objects::DataSet* getFilteredDataSet();

			/** 
			 * @fn void removeSpectrum( kome::objects::Spectrum* spec )
			 * @brief remove spectrum
			 * @param[in] spec spectrum to be removed
			 */
			void removeSpectrum( kome::objects::Spectrum* spec );
			
			/** 
			 * @fn void removeChromatogram( kome::objects::Chromatogram* chrom )
			 * @brief remove chromatogram
			 * @param[in] chrom chromatogram to be removed
			 */
			void removeChromatogram( kome::objects::Chromatogram* chrom );
			
			/** 
			 * @fn void removeDataMap( kome::objects::DataMapInfo* dataMap )
			 * @brief remove DataMap
			 * @param[in] dataMap data map to be removed.
			 */
			void removeDataMap( kome::objects::DataMapInfo* dataMap );

			/**
			 * @fn void setActiveObject( kome::objects::Variant& obj )
			 * @brief sets the active object
			 * @param[in] obj active object
			 */
			 void setActiveObject( kome::objects::Variant& obj );

			/**
			 * @fn kome::objects::Variant getActiveObject()
			 * @brief gets the active object
			 * @return active object
			 */
			kome::objects::Variant getActiveObject();

		protected:
			/**
			 * @fn void updateSampleAndGroup()
			 * @brief updates sample and group
			 */
			void updateSampleAndGroup();

		public:
			/**
			 * @fn void setTitle()
			 * @brief sets frame title
			 */
			void setTitle();

		public:
			/**
			 * @fn bool isActive()
			 * @brief judges whether this frame is active
			 * @return - true: activate   - false: deactivate
			 */
			bool isActive();

		protected:
			/**
			 * @fn void onClose( wxCloseEvent& evt )
			 * @brief This method is called when the frame is closed.
			 * @param[in] evt close event
			 */
			void onClose( wxCloseEvent& evt );

			/**
			 * @fn void onActivate( wxActivateEvent& evt )
			 * @brief This method is called when the frame is activated or deactivated
			 * @param[in] evt activate event
			 */
			void onActivate( wxActivateEvent& evt );

			/**
			 * @fn void onMove( wxMoveEvent& evt )
			 * @brief This method is called when the window is moved.
			 * @param[in] evt move event
			 */
			void onMove( wxMoveEvent& evt );

			/**
			 * @fn void onMoving( wxMoveEvent& evt )
			 * @brief This method is called when the window is being moved.
			 * @param[in] evt move event
			 */
			void onMoving( wxMoveEvent& evt );

			/**
			 * @fn void onSize( wxSizeEvent& evt )
			 * @brief This method is called when the window size is changed.
			 * @param[in] evt size event
			 */
			void onSize( wxSizeEvent& evt );

		public:
			/**
			 * @fn static void callActivate( bool activate )
			 * @brief calls plug-in activate functions
			 * @param[in] activate true: activated,  false: deactivated
			 */
			static void callActivate( bool activate );

			/**
			 * @fn static unsigned int getNumberOfFrames()
			 * @brief gets the number of frames
			 * @return the number of frames
			 */
			static unsigned int getNumberOfFrames();

			/**
			 * @fn static ChildFrame* getFrame( const unsigned int index )
			 * @brief gets child frame
			 * @param[in] index frame index
			 * @return child frame (If NULL, specified index is illegal.)
			 */
			static ChildFrame* getFrame( const unsigned int index );

			/**
			 * @fn static ChildFrame* getActiveFrame()
			 * @brief gets active frame
			 * @return active frame (If NULL, there is no active frame.)
			 */
			static ChildFrame* getActiveFrame();

		private:
			DECLARE_EVENT_TABLE()
		};
	}
}

#endif	// __KOME_WINDOW_CHILD_FRAME_H__
