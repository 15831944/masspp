/**
 * @file PeakEditManager.h
 * @brief interfaces of PeakEditManager class
 *
 * @author S.Tanaka
 * @date 2008.12.17
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_PEAKS_PEAK_EDIT_MANAGER_H__
#define __KOME_PEAKS_PEAK_EDIT_MANAGER_H__

// plugin.xml内で定義されたアイコン名
#define	ICON_NAME_PEAK_ADD		"pkedit_add_cursor"
#define	ICON_NAME_PEAK_EDIT		"pkedit_edit_cursor"
#define	ICON_NAME_PEAK_DEL		"pkedit_del_cursor"

#define	PLUGIN_NAME_PEAK_EDIT	"Peak Edit"

// isInEditArea()関数用
#define CALL_FROM_SPECTRUM		0
#define CALL_FROM_CHROMATOGRAM	1


namespace kome {
	namespace peaks {
		namespace edit {

			/**
			 * @class PeakEditManager
			 * @brief Internal Standard Correction management class
			 */
			class PeakEditManager : kome::objects::DefaultDataManager {
			protected:
				/**
				 * @fn PeakEditManager()
				 * @brief constructor
				 */
				PeakEditManager();

				/**
				 * @fn virtual ~PeakEditManager()
				 * @brief destructor
				 */
				virtual ~PeakEditManager();

			public:
				/**
				 * @typedef LabelMode
				 * @brief label mode
				 */
				typedef enum {
					MODE_NONE,
					MODE_ADD,
					MODE_EDIT
				} LabelMode;

			protected:
				/**
				 * @struct ControlPoint
				 * @brief control point
				 */
				struct ControlPoint {
					double x;
					double y;
					int px;
					int py;
					int edit;
				};

			protected:
				/** selected object */
				kome::objects::Variant m_obj;

				/** selected peak */
				kome::objects::PeakElement* m_peak;

				/** control points */
				std::vector< ControlPoint > m_ctrlPts;		

				/** point data */
				kome::core::Point< double > m_pt;

				/** points data */
				std::vector< kome::core::Point< double > > m_pts;

				/** editing */
				int m_editing;

				/** in adding flag */
				int m_flgInAdd;

			public:
				/** selected peak at click */
				kome::objects::PeakElement* m_peakOnClick;

			public:
				/** select flag in draw peak shape */
				volatile int m_flgSelect;
				/** select flag in memory draw peak shape */
				volatile bool m_flgRedrawMemory;
				/** select flag in DELETE mouse cursor */
				volatile bool m_flgDeleteLast;

				/** select flag in draw peak shape */
				volatile int m_flgSelectSpec;
				/** select flag in draw peak shape */
				volatile int m_flgSelectChrom;

			protected:
				/** label mode */
				LabelMode m_mode;

			protected:
				/** spectrum peaks backup */
				std::map< kome::objects::Spectrum*, kome::objects::Peaks > m_specPeaksMap;

				/** chromatogram peaks backup */
				std::map< kome::objects::Chromatogram*, kome::objects::Peaks > m_chromPeaksMap;

				/** start point */
				kome::core::Point< int > m_startPt;

			public:
				/**
				 * @fn void unselectPeak()
				 * @brief unselectes peak
				 */
				void unselectPeak();

				/**
				 * @fn void setSelectedPeak(
							kome::objects::Spectrum& spec,
							kome::objects::PeakElement& peak
						)
				 * @breif set selected peak
				 * @param spec spectrum
				 * @param peak selected peak
				 */
				void setSelectedPeak(
					kome::objects::Spectrum& spec,
					kome::objects::PeakElement& peak
				);

				/**
				 * @fn void setSelectedPeak(
						kome::objects::Chromatogram& chrom,
						kome::objects::PeakElement& peak
					)
				 * @breif set selected peak
				 * @param spec spectrum
				 * @parma peak selected peak
				 */
				void setSelectedPeak(
					kome::objects::Chromatogram& chrom,
					kome::objects::PeakElement& peak
				);

				/**
				 * @fn kome::objects::PeakElement* getSelectedPeak()
				 * @brief gets selected peak
				 * @return selected peak
				 */
				kome::objects::PeakElement* getSelectedPeak();

				/**
				 * @fn kome::objects::Spectrum* getSelectedSpectrum()
				 * @brief gets selected spectrum
				 * @return selected spectrum
				 */
				kome::objects::Spectrum* getSelectedSpectrum();

				/**
				 * @fn kome::objects::Chromatogram* getSelectedChromatogram()
				 * @brief gets selected chromatogram
				 * @return selected chromatogram
				 */
				kome::objects::Chromatogram* getSelectedChromatogram();

			protected:
				/**
				 * @fn void setControlPoints( kome::core::XYData& xyData )
				 * @brief sets control points
				 * @param xyData data points
				 */				 
				void setControlPoints( kome::core::XYData& xyData );

			// @date 2012/07/04 <Add> OKADA ------->
			private:
				/**
				 * @fn bool isChangeSpectrum( void )
				 * @breif Peakが編集されているかを調べます
				 * @return true=編集されている
				 */
				bool isChangeSpectrum( void );

				/**
				 * @fn bool isChangeChromatogram( void )
				 * @breif Peakが編集されているかを調べます
				 * @return true=編集されている
				 */
				bool isChangeChromatogram( void );
			// @date 2012/07/04 <Add> OKADA <-------

			public:
				/**
				 * @fn void setMode( const LabelMode mode )
				 * @breif sets label mode
				 * @param mode label mode
				 */
				void setMode( const LabelMode mode );

				/**
				 * @fn LabelMode getMode()
				 * @brief gets label mode
				 * @return label mode
				 */
				LabelMode getMode();

			public:
				/**
				 * @fn void deletePeak()
				 * @brief deletes selected peak
				 */
				void deletePeak();

			public:
				/**
				 * @fn static void drawPeakPosition(
							kome::img::Graphics& g
							kome::objects::Peaks& peaks
							kome::core::XYData& xyData
						)
				 * @brief draws peak position
				 * @param g graphics
				 * @param peaks peaks object
				 * @param xyData data points
				 */
				void drawPeakPosition(
					kome::img::Graphics& g,
					kome::objects::Peaks& peaks,
					kome::core::XYData& xyData
				);

				/**
				 * @fn static void drawPeakShape(
							kome::img::Graphics& g
							kome::objects::Peaks& peaks
							kome::core::XYData& xyData
						)
				 * @brief draws peak shape
				 * @param g graphics
				 * @param peaks peaks object
				 * @param xyData data points
				 */
				void drawPeakShape(
					kome::img::Graphics& g,
					kome::objects::Peaks& peaks,
					kome::core::XYData& xyData
				);

			protected:
				/**
				 * @fn void drawPeakPosition(
						kome::img::Graphics& g,
						kome::objects::PeakElement& peak,
						kome::core::XYData& xyData
					)
				 * @brief draws peak position
				 * @param g graphics
				 * @param peak peak
				 * @param xyData data points
				 */
				void drawPeakPosition(
					kome::img::Graphics& g,
					kome::objects::PeakElement& peak,
					kome::core::XYData& xyData
				);

				/**
				 * @fn void drawPeakShape(
						kome::img::Graphics& g,
						kome::objects::PeakElement& peak,
						kome::core::XYData& xyData
					)
				 * @brief draws peak shape
				 * @param g graphics
				 * @param peak peak
				 * @param xyData data points
				 */
				void drawPeakShape(
					kome::img::Graphics& g,
					kome::objects::PeakElement& peak,
					kome::core::XYData& xyData
				);

			public:
				/**
				 * @fn bool startToAdd(
							kome::objects::Spectrum& spec,
							kome::img::Graphics& g,
							kome::evt::MouseEvent& evt
						)
				 * @brief starts to add spectrum peak
				 * @param spec spectrum
				 * @param g graphics
				 * @param evt mouse event
				 * @return refresh flag value
				 */
				bool startToAdd(
					kome::objects::Spectrum& spec,
					kome::img::Graphics& g,
					kome::evt::MouseEvent& evt
				);

				/**
				 * @fn bool startToAdd(
							kome::objects::Chromatogram& chrom,
							kome::img::Graphics& g,
							kome::evt::MouseEvent& evt
						)
				 * @brief starts to add chromatogram peak
				 * @param chrom chromatogram
				 * @param g graphics
				 * @param evt mouse event
				 * @return refresh flag value
				 */
				bool startToAdd(
					kome::objects::Chromatogram& chrom,
					kome::img::Graphics& g,
					kome::evt::MouseEvent& evt
				);

				/**
				 * @fn bool startToAdd(
							kome::core::XYData& xyData,
							kome::objects::Peaks& peaks,
							kome::img::Graphics& g,
							kome::evt::MouseEvent& evt
						)
				 * @brief starts to add spectrum peak
				 * @param xyData xy data
				 * @param peaks peaks
				 * @param g graphics
				 * @param evt mouse event
				 * @return refresh flag value
				 */
				bool startToAdd(
					kome::core::XYData& xyData,
					kome::objects::Peaks& peaks,
					kome::img::Graphics& g,
					kome::evt::MouseEvent& evt
				);

				/**
				 * @fn bool moveAddingCursor(
							kome::objects::Spectrum& spec,
							kome::img::Graphics& g,
							kome::evt::MouseEvent& evt
						)
				 * @brief moves cursor
				 * @param spec spectrum
				 * @param g graphics
				 * @param evt mouse event
				 * @return refresh flag value
				 */
				bool moveAddingCursor(
					kome::objects::Spectrum& spec,
					kome::img::Graphics& g,
					kome::evt::MouseEvent& evt
				);

				/**
				 * @fn bool moveAddingCursor(
							kome::objects::Chromatogram& chrom,
							kome::img::Graphics& g,
							kome::evt::MouseEvent& evt
						)
				 * @brief moves cursor
				 * @param chrom chromatogram
				 * @param g graphics
				 * @param evt mouse event
				 * @param refresh flag value
				 */
				bool moveAddingCursor(
					kome::objects::Chromatogram& chrom,
					kome::img::Graphics& g,
					kome::evt::MouseEvent& evt
				);

				/**
				 * @fn bool moveAddingCursor(
							kome::core::XYData& xyData,
							kome::img::Graphics& g,
							kome::evt::MouseEvent& evt
						)
				 * @brief moves cursor
				 * @param xyData data points
				 * @param g graphics
				 * @param evt mouse event
				 * @return refresh flag value
				 */
				bool moveAddingCursor(
					kome::core::XYData& xyData,
					kome::img::Graphics& g,
					kome::evt::MouseEvent& evt
				);

				/**
				 * @fn bool moveEditingCursor(
							kome::objects::Spectrum& spec,
							kome::img::Graphics& g,
							kome::evt::MouseEvent& evt
						)
				 * @brief moves cursor
				 * @param spec spectrum
				 * @param g graphics
				 * @param evt mouse event
				 * @return refresh flag value
				 */
				bool moveEditingCursor(
					kome::objects::Spectrum& spec,
					kome::img::Graphics& g,
					kome::evt::MouseEvent& evt
				);

				/**
				 * @fn bool moveEditingCursor(
							kome::objects::Chromatogram& chrom,
							kome::img::Graphics& g,
							kome::evt::MouseEvent& evt
						)
				 * @brief moves cursor
				 * @param chrom chromatogram
				 * @param g graphics
				 * @param evt mouse event
				 * @param refresh flag value
				 */
				bool moveEditingCursor(
					kome::objects::Chromatogram& chrom,
					kome::img::Graphics& g,
					kome::evt::MouseEvent& evt
				);

				/**
				 * @fn bool moveEditingCursor(
							kome::core::XYData& xyData,
							kome::img::Graphics& g,
							kome::evt::MouseEvent& evt
						)
				 * @brief moves cursor
				 * @param xyData data points
				 * @param g graphics
				 * @param evt mouse event
				 * @return refresh flag value
				 */
				bool moveEditingCursor(
					kome::core::XYData& xyData,
					kome::img::Graphics& g,
					kome::evt::MouseEvent& evt
				);

				/**
				 * @fn bool finishToAddPeak(
							kome::objects::Spectrum& spec,
							kome::img::Graphics& g,
							komee::evt::MouseEvent& evt
						)
				 * @brief finishes to edit spectrum peak
				 * @param spec spectrum
				 * @param g graphics
				 * @param evt mouse event
				 * @return refresh flag value
				 */
				bool finishToAddPeak(
					kome::objects::Spectrum& spec,
					kome::img::Graphics& g,
					kome::evt::MouseEvent& evt
				);

				/**
				 * @fn bool finishToAddPeak(
							kome::objects::Chromatogram& chrom,
							kome::img::Graphics& g,
							komee::evt::MouseEvent& evt
						)
				 * @brief finishes to edit chromatogram peak
				 * @param chrom chromatogram
				 * @param g graphics
				 * @param evt mouse event
				 * @return refresh flag value
				 */
				bool finishToAddPeak(
					kome::objects::Chromatogram& chrom,
					kome::img::Graphics& g,
					kome::evt::MouseEvent& evt
				);

				/**
				 * @fn bool finishToAddPeak(
							kome::core::XYData& xyData,
							kome::objects::Peaks& peaks,
							kome::img::Graphics& g,
							komee::evt::MouseEvent& evt
						)
				 * @brief finishes to edit peak
				 * @param xyData data points
				 * @param peaks peaks
				 * @param g graphics
				 * @param evt mouse event
				 * @return refresh flag value
				 */
				bool finishToAddPeak(
					kome::core::XYData& xyData,
					kome::objects::Peaks& peaks,
					kome::img::Graphics& g,
					kome::evt::MouseEvent& evt
				);

				/**
				 * @fn bool finishToEditPeak(
							kome::objects::Spectrum& spec,
							kome::img::Graphics& g,
							komee::evt::MouseEvent& evt
						)
				 * @brief finishes to edit spectrum peak
				 * @param spec spectrum
				 * @param g graphics
				 * @param evt mouse event
				 * @return refresh flag value
				 */
				bool finishToEditPeak(
					kome::objects::Spectrum& spec,
					kome::img::Graphics& g,
					kome::evt::MouseEvent& evt
				);

				/**
				 * @fn bool finishToEditPeak(
							kome::objects::Chromatogram& chrom,
							kome::img::Graphics& g,
							komee::evt::MouseEvent& evt
						)
				 * @brief finishes to edit chromatogram peak
				 * @param chrom chromatogram
				 * @param g graphics
				 * @param evt mouse event
				 * @return refresh flag value
				 */
				bool finishToEditPeak(
					kome::objects::Chromatogram& chrom,
					kome::img::Graphics& g,
					kome::evt::MouseEvent& evt
				);

				/**
				 * @fn bool finishToEditPeak(
							kome::core::XYData& xyData,
							kome::objects::Peaks& peaks,
							kome::img::Graphics& g,
							komee::evt::MouseEvent& evt
						)
				 * @brief finishes to edit peak
				 * @param xyData data points
				 * @param peaks peaks
				 * @param g graphics
				 * @param evt mouse event
				 * @return refresh flag value
				 */
				bool finishToEditPeak(
					kome::core::XYData& xyData,
					kome::objects::Peaks& peaks,
					kome::img::Graphics& g,
					kome::evt::MouseEvent& evt
				);

				/**
				 * @fn bool startToEditPeak(
							kome::objects::Spectrum& spec,
							kome::img::Graphics& g,
							kome::evt::MouseEvent& evt
						)
				 * @brief starts to edit spectrum peak
				 * @param spec spectrum
				 * @param g graphics
				 * @param evt mouse event
				 * @return refresh flag value
				 */
				bool startToEditPeak(
					kome::objects::Spectrum& spec,
					kome::img::Graphics& g,
					kome::evt::MouseEvent& evt
				);

				/**
				 * @fn bool startToEditPeak(
							kome::objects::Chromatogram& chrom,
							kome::img::Graphics& g,
							kome::evt::MouseEvent& evt
						)
				 * @brief starts to edit chromatogram peak
				 * @param chrom chromatogram
				 * @param g graphics
				 * @param evt mouse event
				 * @return refresh flag value
				 */
				bool startToEditPeak(
					kome::objects::Chromatogram& chrom,
					kome::img::Graphics& g,
					kome::evt::MouseEvent& evt
				);

				/**
				 * @fn bool startToEditPeak(
							kome::core::XYData& xyData,
							kome::objects::Peaks& peaks,
							kome::img::Graphics& g,
							komee::evt::MouseEvent& evt
						)
				 * @brief starts to edit peak
				 * @param xyData data points
				 * @param peaks peaks
				 * @param g graphics
				 * @param evt mouse event
				 * @return refresh flag value
				 */
				bool startToEditPeak(
					kome::core::XYData& xyData,
					kome::objects::Peaks& peaks,
					kome::img::Graphics& g,
					kome::evt::MouseEvent& evt
				);

			protected:
				/**
				 * @fn kome::objects::PeakElement* pickPeak(
							kome::core::XYData& xyData,
							kome::objects::Peaks& peaks,
							kome::img::Graphics& g,
							const int px,
							const int py
						)
				 * @brief picks peak
				 * @param xyData data points
				 * @param peaks peaks
				 * @param g graphics
				 * @param px the x coordinate of the point
				 * @param py the y coordinate of the point
				 */
				kome::objects::PeakElement* pickPeak(
					kome::core::XYData& xyData,
					kome::objects::Peaks& peaks,
					kome::img::Graphics& g,
					const int px,
					const int py
				);

			public:
				/**
				 * @fn void commitPeaks( kome::objects::Spectrum& spec )
				 * @brief commits spectrum peaks
				 * @param spec spectrum
				 */
				void commitPeaks( kome::objects::Spectrum& spec );

				/**
				 * @fn void commitPeaks( kome::objects::Chromatogram& chrom )
				 * @brief commits chromatogram peaks
				 * @param chrom chromatogram
				 */
				void commitPeaks( kome::objects::Chromatogram& chrom );

				/**
				 * @fn void commitPeaks()
				 * @brief commits peaks
				 */
				void commitPeaks();

				/**
				 * @fn void rollbackPeaks( kome::objects::Spectrum& spec )
				 * @brief rollbacks spectrum peaks
				 * @param spec spectrum
				 */
				void rollbackPeaks( kome::objects::Spectrum& spec );

				/**
				 * @fn void rollbackPeaks( kome::objects::Chromatogram& chrom )
				 * @brief rollbacks chromatogram peaks
				 * @param chrom chromatogram
				 */
				void rollbackPeaks( kome::objects::Chromatogram& chrom );

				/**
				 * @fn void rollbackPeaks()
				 * @brief rollbacks peaks
				 */
				void rollbackPeaks();

			protected:
				/**
				 * @fn void askCommitOrRollback()
				 * @brief asks which commit or rollback
				 */
				void askCommitOrRollback();

				/**
				 * @fn void backupPeaks( kome::objects::Peaks& src, kome::objects::Peaks& dst )
				 * @brief backup peaks
				 * @param src source peaks object
				 * @param dst dest peaks object
				 */
				void backupPeaks( kome::objects::Peaks& src, kome::objects::Peaks& dst );


			public:
				/**
				 * @fn virtual void onExitSample() 
				 * @brief This method is called when a sample is closed.
				 */
				virtual void onExitSample();

			protected:
				/**
				 * @fn virtual void onCloseSample( kome::objects::Sample* sample, const bool deleting ) 
				 * @brief This method is called when a sample is closed. (override method)
				 * @param sample sample object to be closed
				 * @param[in] deleting If true, the specified object is being deleted now.
				 */
				virtual void onCloseSample( kome::objects::Sample* sample, const bool deleting );

				/**
				 * @fn virtual void onCloseSpectrum( kome::objects::Spectrum* spec, const bool deleting ) 
				 * @brief This method is called when a spectrum is closed. (override method)
				 * @param spec spectrum object to be opened
				 * @param[in] deleting If true, the specified object is being deleted now.
				 */
				virtual void onCloseSpectrum( kome::objects::Spectrum* spec, const bool deleting );

				/**
				 * @fn virtual void onCloseChromatogram( kome::objects::Chromatogram* chrom, const bool deleting ) 
				 * @brief This method is called when a chromatogram is closed. (override method)
				 * @param chrom chromatogram object to be closed
				 * @param[in] deleting If true, the specified object is being deleted now.
				 */
				virtual void onCloseChromatogram( kome::objects::Chromatogram* chrom, const bool deleting );

			public:

				/**
				 * @fn int isInAddArea( kome::img::Graphics& g, kome::evt::MouseEvent& evt, int flgCallFromSpecChrom )
				 * @brief identify in/out of Add-Area
				 * @param g Graphics object
				 * @param evt mouse event
				 * @param flgCallFromSpecChrom  0:call from spec  1: call from chromatogram
				 * @return true:in area  false:out of area
				 */
				int isInAddArea( kome::img::Graphics& g, kome::evt::MouseEvent& evt, int flgCallFromSpecChrom	);

				/**
				 * @fn int isInEditArea( kome::img::Graphics& g, kome::evt::MouseEvent& evt, int flgCallFromSpecChrom, int flgNoChkSelect=0 )
				 * @brief identify in/out of Edit-Area
				 * @param g Graphics object
				 * @param evt mouse event
				 * @param flgCallFromSpecChrom  0:call from spec  1: call from chromatogram
				 * @param flgNoChkSelect 1:no check m_flgSelect				 
				 * @return true:in area  false:out of area
				 */
				int isInEditArea( kome::img::Graphics& g, kome::evt::MouseEvent& evt, int flgCallFromSpecChrom, int flgNoChkSelect=0 );

				/**
				 * @fn int isInDeleteArea( kome::img::Graphics& g, kome::evt::MouseEvent& evt, int flgCallFromSpecChrom, kome::objects::Parameters* params = NULL );
				 * @brief identify in/out of Delete-Area
				 * @param g Graphics object
				 * @param evt mouse event
				 * @param flgCallFromSpecChrom  0:call from spec  1: call from chromatogram
				 * @param params  parameter
				 * @return true:in area  false:out of area
				 */
				int isInDeleteArea( kome::img::Graphics& g, kome::evt::MouseEvent& evt, int flgCallFromSpecChrom, kome::objects::Parameters* params = NULL );

				/**
				 * @fn int isInControlArea( kome::img::Graphics& g, kome::evt::MouseEvent& evt );
				 * @brief identify in/out of control-Area
				 * @param g Graphics object
				 * @param evt mouse event
				 * @return true:in area  false:out of area
				 */
				int isInControlArea( kome::img::Graphics& g, kome::evt::MouseEvent& evt );

				/** memory DC for area identify */
				wxMemoryDC m_memDC;

				/** DCGraphics object for area identify of spectrum */
				kome::window::DCGraphics m_graphicsSpec;	// @date 2011/06/27 <Add> OKADA

				/** DCGraphics object for area identify of chromatogram */
				kome::window::DCGraphics m_graphicsChrom;	// @date 2011/06/27 <Add> OKADA

				/** wxImage object for area identify of spectrum */
				wxImage wxiImageSpec;	// @date 2011/06/27 <Add> OKADA

				/** wxImage object for area identify of chromatogram */
				wxImage wxiImageChrom;	// @date 2011/06/27 <Add> OKADA

				/**
				 * @fn int setMouseCursor( kome::img::Graphics& graphics, kome::evt::MouseEvent& evt, const char* pluginName , const char* iconName );
				 * @brief set mouse cursor
				 * @param g Graphics object 
				 * @param evt mouse event 
				 * @param pluginName set plugin-name. define at <name> tag in plugin.xml. For example, in this project, use PLUGIN_NAME_PEAK_EDIT.
				 * @param iconName  set mouse-cursor-name. define at <icon-name> tag in plugin.xml. For example, in this project, use ICON_NAME_PEAK_ADD, ICON_NAME_PEAK_EDIT or ICON_NAME_PEAK_DEL .
				 * @param canvas  set window
				 * @return true:in area  false:out of area
				 */
				int setMouseCursor( kome::img::Graphics& graphics, kome::evt::MouseEvent& evt, const char* pluginName , const char* iconName, wxWindow* canvas = NULL );

				/**
				 * @fn void setModeNone( void );
				 * @brief set mouse mode none
				 */
				void setModeNone( void );

				/**
				 * @fn void setMode ADD( void );
				 * @brief set mouse mode none
				 */
				void setModeAdd( void );

				/**
				 * @fn void setMode EDIT( void );
				 * @brief set mouse mode none
				 */
				void setModeEdit( void );

			public:
				/**
				 * @fn static PeakEditManager& getInstance()
				 * @brief gets Internal Standard Correction manager
				 * @return Internal Standard Correction manager class (This is the only object.)
				 */
				static PeakEditManager& getInstance();

				/**
				 * @fnint getInAddFlag( void );
				 * @brief get In Add flag
				 */
				int getInAddFlag( void );

				/**
				 * @fnint getInAddFlag( void );
				 * @param flg set Add-fllag
				 * @brief set In Add flag
				 */
				int setInAddFlag( int flg );

			protected:
				/**
				 * @fn static double getPeakPosition(
							kome::core::XYData& xyData,
							const int lIdx,
							const int rIdx,
							const double ly,
							const double ry
						)
				* @brief gets the peak position
				* @param[in] xyData data points
				* @param[in] lIdx left index
				* @param[in] rIdx right index
				* @param[in] ly left y
				* @param[in] ry right y
				* @return peak position
				*/
				static double getPeakPosition(
					kome::core::XYData& xyData,
					const int lIdx,
					const int rIdx,
					const double ly,
					const double ry
				);


			};
		}
	}
}

#endif		// __KOME_PEAKS_PEAK_EDIT_MANAGER_H__
