/**
 * @file PluginCallTool.h
 * @brief interfaces of PluginCallTool class
 *
 * @author S.Tanaka
 * @date 2007.03.07
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_PLUGIN_CALL_TOOL_H__
#define __KOME_PLUGIN_CALL_TOOL_H__


class wxWindow;

namespace kome {

	namespace img {
		class Graphics;
		class DataMapInfo;
	}

	namespace evt {
		class MouseEvent;
		class KeyEvent;
		class SizeEvent;
		class MoveEvent;
	}
	
	namespace ident{
		class SampleGetter;
		class IdentificationManager;
	}
	namespace plugin {

		class PluginCall;

		/**
		 * @class PluginCallTool
		 * @brief calling plug-in function class
		 */
		class PLUGIN_TOOLS_CLASS PluginCallTool {
		public:
			/**
			 * @fn static void openFile( const char* path )
			 * @brief opens raw data file
			 * @param[in] path raw data file path
			 */
			static void openFile( const char* path );

			/**
			 * @fn static void onSaveSample( kome::objects::Sample& sample )
			 * @brief broadcasts the sample is saved
			 * @param[in] sample saved sample
			 */
			static void onSaveSample( kome::objects::Sample& sample );

			/**
			 * @fn static void openSpectrum( kome::objects::Spectrum& spec )
			 * @brief opens spectrum
			 * @param[in] spec spectrum to be opened
			 */
			static void openSpectrum( kome::objects::Spectrum& spec );

			/**
			 * @fn static void openChromatogram( kome::objects::Chromatogram& chrom )
			 * @brief opens chromatogram
			 * @param[in] chrom chromatogram to be opened
			 */
			static void openChromatogram( kome::objects::Chromatogram& chrom );

			/**
			 * @fn static void openDataMap( kome::objects::DataMapInfo& dataMap )
			 * @brief opens data map
			 * @param[in] dataMap data map to be opened
			 */
			static void openDataMap( kome::objects::DataMapInfo& dataMap );

			/**
			 * @fn static wxWindow* createSpectrumWindow( kome::objects::Spectrum& spec, wxWindow* parent )
			 * @brief creates spectrum window
			 * @param[in] spec spectrum object
			 * @param[in] parent parent window object
			 * @return spectrum window object
			 */
			static wxWindow* createSpectrumWindow( kome::objects::Spectrum& spec, wxWindow* parent );

			/**
			 * @fn static wxWindow* createChromatogramWindow( kome::objects::Chromatogram& chrom, wxWindow* parent )
			 * @brief creates chromatogram window
			 * @param[in] chrom chromatogram object
			 * @param[in] parent parent window object
			 * @return chromatogram window object
			 */
			static wxWindow* createChromatogramWindow( kome::objects::Chromatogram& chrom, wxWindow* parent );

			/**
			 * @fn static wxWindow* createDataMapWindow( kome::objects::DataMapInfo& dataMap, wxWindow* parent )
			 * @brief creates data map window
			 * @param[in] dataMap data map object
			 * @param[in] parent parent window object
			 * @return data map window object
			 */
			static wxWindow* createDataMapWindow( kome::objects::DataMapInfo& dataMap, wxWindow* parent );
		
			/**
			 * @fn static void onOpenSampleSet( kome::objects::SampleSet& sampleSet, kome::core::Progress* progress )
			 * @brief broadcasts opening a sample set
			 * @param[in] sampleSet opened sample set
			 * @param[in] progress the object to show the progress
			 */
			static void onOpenSampleSet( kome::objects::SampleSet& sampleSet, kome::core::Progress* progress = NULL );

			/**
			 * @fn static void onOpenSample( kome::objects::Sample& sample )
			 * @brief broadcasts opening a sample
			 * @param[in] sample opened sample
			 */
			static void onOpenSample( kome::objects::Sample& sample );

			/**
			 * @fn static void onCloseSample( kome::objects::Sample& sample )
			 * @brief broadcasts closing a sample
			 * @param[in] sample closed sample
			 */
			static void onCloseSample( kome::objects::Sample& sample );

			/**
			 * @fn static bool canExit()
			 * @brief broadcasts Mass++ wants to exit
			 */
			static bool canExit();

			/**
			 * @fn static void onUpdateSpectrum( kome::objects::Spectrum& spec )
			 * @brief broadcasts a spectrum is updated
			 * @param[in] spec updated spectrum
			 */
			static void onUpdateSpectrum( kome::objects::Spectrum& spec );

			/**
			 * @fn static void onUpdateChromatogram( kome::objects::Chromatogram& chrom )
			 * @brief broadcasts a chromatogram is updated
			 * @param[in] chrom updated chromatogram
			 */
			static void onUpdateChromatogram( kome::objects::Chromatogram& chrom );

			/**
			 * @fn static void onUpdateDataMap( kome::objects::DataMapInfo& dataMap )
			 * @brief broadcasts a datamap is updated
			 * @param[in] dataMap updated data map
			 */
			static void onUpdateDataMap( kome::objects::DataMapInfo& dataMap );

			/**
			 * @fn static void onUpdateGroup( kome::objects::DataGroupNode& group )
			 * @brief broadcasts a group is updated
			 * @param[in] group updated group
			 */
			static void onUpdateGroup( kome::objects::DataGroupNode& group );

			/**
			 * @fn static void onUpdateSample( kome::objects::Sample& sample )
			 * @brief broadcasts a sample is updated
			 * @param[in] sample updated sample
			 */
			static void onUpdateSample( kome::objects::Sample& sample );

			/**
			 * @fn static void onUpdateDataSet( kome::objects::DataSet& dataSet )
			 * @brief broadcasts a data set is updated
			 * @param[in] dataSet updated data set
			 */
			static void onUpdateDataSet( kome::objects::DataSet& dataSet );

			/**
			 * @fn static void detectSpecPeaks( kome::objects::DataSet& dataSet, kome::core::Progress* progress = NULL )
			 * @brief spectrum peak detection
			 * @param[in] dataSet spectra to detect peaks
			 * @param[in] progress the object to show the progress
			 */
			static void detectSpecPeaks( kome::objects::DataSet& dataSet, kome::core::Progress* progress = NULL );

			/**
			 * @fn static void filterSpecPeaks(
						kome::objects::Spectrum** spectra,
						kome::objects::Peaks** peaks,
						const unsigned int num,
						const char* filter
					)
			 * @brief filters spectrum peaks
			 * @param[in] spectra the array of spectra
			 * @param[in,out] peaks the array of peak lists
			 * @param[in] num the array size of spectra and peak lists
			 * @param[in] filter character string of the filter
			 */
			static void filterSpecPeaks(
				kome::objects::Spectrum** spectra,
				kome::objects::Peaks** peaks,
				const unsigned int num,
				const char* filter
			);

			/**
			 * @fn static void onUpdateSpecPeaks( kome::objects::Spectrum& spec )
			 * @brief broadcasts spectrum peaks are updated
			 * @param[in] spec updated spectrum
			 */
			static void onUpdateSpecPeaks( kome::objects::Spectrum& spec );

			/**
			 * @fn static void detectChromPeaks( kome::objects::DataSet& dataSet, kome::core::Progress* progress = NULL )
			 * @brief chromatogram peak detection
			 * @param[in] dataSet chromatograms to detect peaks
			 * @param[in] progress the object to show the progress
			 */
			static void detectChromPeaks( kome::objects::DataSet& dataSet, kome::core::Progress* progress = NULL );

			/**
			 * @fn static void onUpdateChromPeaks( kome::objects::Chromatogram& chrom )
			 * @brief broadcasts chromatogram peaks are updated
			 * @param[in] chrom updated chromatogram
			 */
			static void onUpdateChromPeaks( kome::objects::Chromatogram& chrom );

			/**
			 * @fn static void detect2DPeaks( kome::objects::DataGroupNode& group, kome::core::Progress* progress = NULL )
			 * @brief detects 2D peaks
			 * @param[in] group the group to detect 2D peaks
			 * @param[in] progress the object to show the progress
			 */
			static void detect2DPeaks( kome::objects::DataGroupNode& group, kome::core::Progress* progress = NULL );

			/**
			 * @fn static void onUpdate2DPeaks( kome::objects::DataGroupNode& group )
			 * @brief broadcasts 2D peaks are updated
			 * @param[in] group spectrum group
			 */
			static void onUpdate2DPeaks( kome::objects::DataGroupNode& group );

			/**
			 * @fn static void onUpdatePeaksMatrix()
			 * @brief broadcasts peaks matrix is updated
			 */
			static void onUpdatePeaksMatrix();

			/**
			 * @fn static void onMatrixPeak( const int peakId, wxWindow* parent )
			 * @brief matrix peak action
			 * @param[in] peakId matrix peak ID
			 * @param[in] parent parent window
			 */
			static void onMatrixPeak( const int peakId, wxWindow* parent );
			
			/**
			 * @fn void onQuantitationWindow( wxWindow* parent, const char* name )
			 * @brief open quantitation window
			 * @param[in] parent parent window
			 * @param[in] name name
			 */
			void onQuantitationWindow( wxWindow* parent, const char* name );

		   /**
			* @fn static void openMatrix()
			* @brief opens matrix
			*/
			static void openMatrix();

			/**
			 * @fn static void closeMatrix()
			 * @brief closes matrix
			 */
			static void closeMatrix();

			/**
			 * @fn static void onCommitSpecPeaks(
						kome::objects::Spectrum& spec,
						kome::core::XYData& xyData,
						kome::objects::Peaks& src,
						kome::objects::Peaks& dst
					)
			 * @brief broadcasts peaks are commited
			 * @param[in] spec spectrum 
			 * @param[in] xyData data points
			 * @param[in] src peaks before commiting
			 * @param[in] dst updated peaks
			 */
			static void onCommitSpecPeaks(
				kome::objects::Spectrum& spec,
				kome::core::XYData& xyData,
				kome::objects::Peaks& src,
				kome::objects::Peaks& dst
			);

			/**
			 * @fn static void onCommitChromPeaks(
						kome::objects::Chromatogram& chrom,
						kome::core::XYData& xyData,
						kome::objects::Peaks& src,
						kome::objects::Peaks& dst
					)
			 * @brief broadcasts peaks are commited
			 * @param[in] chrom chromatogram
			 * @param[in] xyData data points
			 * @param[in] src peaks before commiting
			 * @param[in] dst updated peaks
			 */
			static void onCommitChromPeaks(
				kome::objects::Chromatogram& chrom,
				kome::core::XYData& xyData,
				kome::objects::Peaks& src,
				kome::objects::Peaks& dst
			);

			/**
			 * @fn static void onRollbackSpecPeaks(
						kome::objects::Spectrum& spec,
						kome::core::XYData& xyData,
						kome::objects::Peaks& peaks,
						kome::objects::Peaks& canceledPeaks
					)
			 * @brief broadcasts peaks are rollbacked
			 * @param[in] spec spectrum 
			 * @param[in] xyData data points
			 * @param[in] peaks peaks
			 * @param[in] canceledPeaks canceled peaks
			 */
			static void onRollbackSpecPeaks(
				kome::objects::Spectrum& spec,
				kome::core::XYData& xyData,
				kome::objects::Peaks& peaks,
				kome::objects::Peaks& canceledPeaks
			);

			/**
			 * @fn static void onRollbackChromPeaks(
						kome::objects::Chromatogram& chrom,
						kome::core::XYData& xyData,
						kome::objects::Peaks& peaks,
						kome::objects::Peaks& canceledPeaks
					)
			 * @brief broadcasts peaks are rollbacked
			 * @param[in] chrom chromatogram
			 * @param[in] xyData data points
			 * @param[in] peaks peaks
			 * @param[in] canceledPeaks canceled peaks
			 */
			static void onRollbackChromPeaks(
				kome::objects::Chromatogram& chrom,
				kome::core::XYData& xyData,
				kome::objects::Peaks& peaks,
				kome::objects::Peaks& canceledPeaks
			);

			/**
			 * @fn static void commitSpecPeaks( kome::objects::Spectrum& spec )
			 * @brief commits spectrum peaks
			 * @param[in] spec spectrum
			 */
			static void commitSpecPeaks( kome::objects::Spectrum& spec );

			/**
			 * @fn static void commitChromPeaks( kome::objects::Chromatogram& chrom )
			 * @brief commits chromatogram peaks
			 * @param[in] chrom chromatogram
			 */
			static void commitChromPeaks( kome::objects::Chromatogram& chrom );

			/**
			 * @fn static void rollbackSpecPeaks( kome::objects::Spectrum& spec )
			 * @brief rollbacks spectrum peaks
			 * @param[in] spec spectrum
			 */
			static void rollbackSpecPeaks( kome::objects::Spectrum& spec );

			/**
			 * @fn static void rollbackChromPeaks( kome::objects::Chromatogram& chrom )
			 * @brief rollbacks chromatogram peaks
			 * @param[in] chrom chromatogram
			 */
			static void rollbackChromPeaks( kome::objects::Chromatogram& chrom ); 

			/**
			 * @fn static bool onOpenSpectrum( kome::objects::Spectrum& spec )
			 * @brief broadcasts a spectrum is open
			 * @param[in] spec opened spectrum
			 * @return If false, it is impossible to open spectrum.
			 */
			static bool onOpenSpectrum( kome::objects::Spectrum& spec );

			/**
			 * @fn static bool onOpenChromatogram( kome::objects::Chromatogram& chrom )
			 * @brief broadcasts a chromatogram is opened
			 * @param[in] chrom opened chromatogram
			 * @return If false, it is impossible to open chromatogram
			 */
			static bool onOpenChromatogram( kome::objects::Chromatogram& chrom );

			/**
			 * @fn static bool onOpenDataMap( kome::objects::DataMapInfo& dataMap )
			 * @brief broadcasts a data map is opened
			 * @param[in] dataMap opened data map
			 * @return If false, it is impossible to open data map
			 */
			static bool onOpenDataMap( kome::objects::DataMapInfo& dataMap );
			
			/**
			 * @fn static void onCloseSpectrum( kome::objects::Spectrum& spec, wxWindow* canvas )
			 * @brief broadcasts a spectrum is close
			 * @param[in] spec closed spectrum
			 * @param[in] canvas canvas ID
			 */
			static void onCloseSpectrum( kome::objects::Spectrum& spec, wxWindow* canvas );

			/**
			 * @fn static void onCloseChromatogram( kome::objects::Chromatogram& chrom, wxWindow* canvas )
			 * @brief broadcasts a chromatogram is closed
			 * @param[in] chrom closed chromatogram
			 * @param[in] canvas canvas ID
			 */
			static void onCloseChromatogram( kome::objects::Chromatogram& chrom, wxWindow* canvas );

			/**
			 * @fn static void onCloseDataMap( kome::objects::DataMapInfo& dataMap, wxWindow* canvas )
			 * @brief broadcasts a data map is closed
			 * @param[in] dataMap closed data map
			 * @param[in] canvas canvas ID
			 */
			static void onCloseDataMap( kome::objects::DataMapInfo& dataMap, wxWindow* canvas );

			/**
			 * @fn static void onActivate( kome::objects::Variant& obj )
			 * @brief broadcasts active object is changed
			 * @param[in] obj active object
			 */
			static void onActivate( kome::objects::Variant& obj );

			/**
			 * @fn static void onSettings()
			 * @brief broadcasts settings parameter is updated
			 */
			static void onSettings();

			/**
			 * @fn static void refresh()
			 * @brief refreshes windows
			 */
			static void refresh();

			/**
			 * @fn static void changeSpectrumRange(
						kome::objects::Spectrum& spec,
						const double start,
						const double end,
						const long long canvas
					)
			 * @brief changes spectrum range
			 * @param[in] spec spectrum
			 * @param[in] start the start of m/z range
			 * @param[in] end the end of m/z range
			 * @param[in] canvas canvas object
			 */
			static void changeSpectrumRange(
				kome::objects::Spectrum& spec,
				const double start,
				const double end,
				const long long canvas
			);

			/**
			 * @fn static void changeChromatogramRange(
						kome::objects::Chromatogram& chrom,
						const double start,
						const double end,
						const long long canvas
					)
			 * @brief changes chromatogram range
			 * @param[in] chrom chromatogram
			 * @param[in] start the start of RT range
			 * @param[in] end the end of RT range
			 * @param[in] canvas canvas object
			 */
			static void changeChromatogramRange(
				kome::objects::Chromatogram& chrom,
				const double start,
				const double end,
				const long long canvas
			);

			/**
			 * @fn static void actSpectrumRange(
					kome::objects::Spectrum& spec,
					const double start,
					const double end
				)
			 * @brief acts spectrum range
			 * @param[in] spec spectrum
			 * @param[in] start the start of m/z range
			 * @param[in] end the end of m/z range
			 */
			static void actSpectrumRange(
				kome::objects::Spectrum& spec,
				const double start,
				const double end
			);

			/**
			 * @fn static void actChromatogramRange(
					kome::objects::Chromatogram& chrom,
					const double start,
					const double end,
					kome::core::Progress* progress = NULL
				)
			 * @brief acts chromatogram range
			 * @param[in] chrom chromatogram
			 * @param[in] start the start of RT range
			 * @param[in] end the end of RT range
			 * @param[in] progress the object to show the progress
			 */
			static void actChromatogramRange(
				kome::objects::Chromatogram& chrom,
				const double start,
				const double end,
				kome::core::Progress* progress = NULL
			);

			/**
			 * @fn static void closeAllSpectrum()
			 * @brief closes all spectrum window
			 */
			static void closeAllSpectrum();

			/**
			 * @fn static void closeAllChromatogram()
			 * @brief closes all chromatogram window
			 */
			static void closeAllChromatogram();

			/**
			 * @fn static void closeAllDataMap()
			 * @brief closes all data map window
			 */
			static void closeAllDataMap();

		public:
			/**
			 * @fn static void drawSpectrum(
						kome::objects::Spectrum& spec,
						kome::img::Graphics& g,
						wxWindow* canvas
					)
			 * @brief draws spectrum
			 * @param[in] spec spectrum object to draw
			 * @param[in,out] g graphics object to be drawed
			 * @param[in] canvas canvas ID
			 */
			static void drawSpectrum(
				kome::objects::Spectrum& spec,
				kome::img::Graphics& g,
				wxWindow* canvas
			);

			/**
			 * @fn static void prepareDrawSpectrum(
						kome::objects::Spectrum& spec,
						kome::img::Graphics& g,
						wxWindow* canvas
					)
			 * @brief prepares to draw spectrum
			 * @param[in] spec spectrum object to draw
			 * @param[in,out] g graphics object to be drawed
			 * @param[in] canvas canvas ID
			 */
			static void prepareDrawSpectrum(
				kome::objects::Spectrum& spec,
				kome::img::Graphics& g,
				wxWindow* canvas
			);

			/**
			 * @fn static void drawSpectrumFg(
						kome::objects::Spectrum& spec,
						kome::img::Graphics& g,
						wxWindow* canvas
					)
			 * @brief draws spectrum foreground
			 * @param[in] spec spectrum object to draw
			 * @param[in,out] g graphics object to be drawed
			 * @param[in] canvas canvas ID
			 */
			static void drawSpectrumFg(
				kome::objects::Spectrum& spec,
				kome::img::Graphics& g,
				wxWindow* canvas
			);

			/**
			 * @fn static void drawSpectrumBg(
						kome::objects::Spectrum& spec,
						kome::img::Graphics& g,
						wxWindow* canvas
					)
			 * @brief draws spectrum background
			 * @param[in] spec spectrum object to draw
			 * @param[in,out] g graphics object to be drawed
			 * @param[in] canvas canvas ID
			 */
			static void drawSpectrumBg(
				kome::objects::Spectrum& spec,
				kome::img::Graphics& g,
				wxWindow* canvas
			);

			/**
			 * @fn static void drawSpectrumWindowFg(
						kome::objects::Spectrum& spec,
						kome::img::Graphics& g,
						wxWindow* canvas
					)
			 * @brief draws spectrum canvas foreground
			 * @param[in] spec spectrum object to draw
			 * @param[in,out] g graphics object to be drawed
			 * @param[in] canvas canvas ID
			 */
			static void drawSpectrumWindowFg(
				kome::objects::Spectrum& spec,
				kome::img::Graphics& g,
				wxWindow* canvas
			);

			/**
			 * @fn static void drawSpectrumWindowBg(
						kome::objects::Spectrum& spec,
						kome::img::Graphics& g,
						wxWindow* canvas
					)
			 * @brief draws spectrum canvas background
			 * @param[in] spec spectrum object to draw
			 * @param[in,out] g graphics object to be drawed
			 * @param[in] canvas canvas ID
			 */
			static void drawSpectrumWindowBg(
				kome::objects::Spectrum& spec,
				kome::img::Graphics& g,
				wxWindow* canvas
			);

			/**
			 * @fn static void drawChromatogram(
						kome::objects::Chromatogram& chrom,
						kome::img::Graphics& g,
						wxWindow* canvas
					)
			 * @brief draws chromatogram
			 * @param[in] chrom chromatogram object to draw
			 * @param[in,out] g graphics object to be drawed
			 * @param[in] canvas canvas ID
			 */
			static void drawChromatogram(
				kome::objects::Chromatogram& chrom,
				kome::img::Graphics& g,
				wxWindow* canvas
			);

			/**
			 * @fn static void prepareDrawChromatogram(
						kome::objects::Chromatogram& chrom,
						kome::img::Graphics& g,
						wxWindow* canvas
					)
			 * @brief prepares to draw chromatogram
			 * @param[in] chrom chromatogram object to draw
			 * @param[in,out] g graphics object to be drawed
			 * @param[in] canvas canvas ID
			 */
			static void prepareDrawChromatogram(
				kome::objects::Chromatogram& chrom,
				kome::img::Graphics& g,
				wxWindow* canvas
			);

			/**
			 * @fn static void drawChromatogramFg(
						kome::objects::Chromatogram& chrom,
						kome::img::Graphics& g,
						wxWindow* canvas
					)
			 * @brief draws chromatogram foreground
			 * @param[in] chrom chromatogram object to draw
			 * @param[in,out] g graphics object to be drawed
			 * @param[in] canvas canvas ID
			 */
			static void drawChromatogramFg(
				kome::objects::Chromatogram& chrom,
				kome::img::Graphics& g,
				wxWindow* canvas
			);

			/**
			 * @fn static void drawChromatogramBg(
						kome::objects::Chromatogram& chrom,
						kome::img::Graphics& g,
						wxWindow* canvas
					) 
			 * @brief draws chromatogram background
			 * @param[in] chrom chromatogram object to draw
			 * @param[in,out] g graphics object to be drawed
			 * @param[in] canvas canvas ID
			 */
			static void drawChromatogramBg(
				kome::objects::Chromatogram& chrom,
				kome::img::Graphics& g,
				wxWindow* canvas
			);

			/**
			 * @fn static void drawChromatogramWindowFg(
						kome::objects::Chromatogram& chrom,
						kome::img::Graphics& g,
						wxWindow* canvas
					)
			 * @brief draws chromatogram canvas foreground
			 * @param[in] chrom chromatogram object to draw
			 * @param[in,out] g graphics object to be drawed
			 * @param[in] canvas canvas ID
			 */
			static void drawChromatogramWindowFg(
				kome::objects::Chromatogram& chrom,
				kome::img::Graphics& g,
				wxWindow* canvas
			);

			/**
			 * @fn static void drawChromatogramWindowBg(
						kome::objects::Chromatogram& chrom,
						kome::img::Graphics& g,
						wxWindow* canvas
					)
			 * @brief draws chromatogram canvas background
			 * @param[in] chrom chromatogram object to draw
			 * @param[in,out] g graphics object to be drawed
			 * @param[in] canvas canvas ID
			 */
			static void drawChromatogramWindowBg(
				kome::objects::Chromatogram& chrom,
				kome::img::Graphics& g,
				wxWindow* canvas
			);

			/**
			 * @fn static void drawDataMap(
						kome::objects::DataMapInfo& dataMap,
						kome::img::Graphics& g,
						wxWindow* canvas
					)
			 * @brief draws data map
			 * @param[in] dataMap data map object to draw
			 * @param[in,out] g graphics object to be drawed
			 * @param[in] canvas canvas ID
			 */
			static void drawDataMap(
				kome::objects::DataMapInfo& dataMap,
				kome::img::Graphics& g,
				wxWindow* canvas
			);

			/**
			 * @fn static void prepareDrawDataMap(
						kome::objects::DataMapInfo& dataMap,
						kome::img::Graphics& g,
						wxWindow* canvas
					)
			 * @brief prepares to draw data map
			 * @param[in] dataMap data map object to draw
			 * @param[in,out] g graphics object to be drawed
			 * @param[in] canvas canvas ID
			 */
			static void prepareDrawDataMap(
				kome::objects::DataMapInfo& dataMap,
				kome::img::Graphics& g,
				wxWindow* canvas
			);

			/**
			 * @fn static void drawDataMapFg(
						kome::objects::DataMapInfo& dataMap,
						kome::img::Graphics& g,
						wxWindow* canvas
					)
			 * @brief draws data map foreground
			 * @param[in] dataMap data map object to draw
			 * @param[in,out] g graphics object to be drawed
			 * @param[in] canvas canvas ID
			 */
			static void drawDataMapFg(
				kome::objects::DataMapInfo& dataMap,
				kome::img::Graphics& g,
				wxWindow* canvas
			);

			/**
			 * @fn static void drawDataMapBg(
						kome::objects::DataMapInfo& dataMap,
						kome::img::Graphics& g,
						wxWindow* canvas
					)
			 * @brief draws data map background
			 * @param[in] dataMap data map object to draw
			 * @param[in,out] g graphics object to be drawed
			 * @param[in] canvas canvas ID
			 */
			static void drawDataMapBg(
				kome::objects::DataMapInfo& dataMap,
				kome::img::Graphics& g,
				wxWindow* canvas
			);

			/**
			 * @fn static void drawDataMapWindowFg(
						kome::objects::DataMapInfo& dataMap,
						kome::img::Graphics& g,
						wxWindow* canvas
					)
			 * @brief draws data map canvas foreground
			 * @param[in] dataMap data map object to draw
			 * @param[in,out] g graphics object to be drawed
			 * @param[in] canvas canvas ID
			 */
			static void drawDataMapWindowFg(
				kome::objects::DataMapInfo& dataMap,
				kome::img::Graphics& g,
				wxWindow* canvas
			);

			/**
			 * @fn static void drawDataMapWindowBg(
						kome::objects::DataMapInfo& dataMap,
						kome::img::Graphics& g,
						wxWindow* canvas
					)
			 * @brief draws data map canvas background
			 * @param[in] dataMap data map object to draw
			 * @param[in,out] g graphics object to be drawed
			 * @param[in] canvas canvas ID
			 */
			static void drawDataMapWindowBg(
				kome::objects::DataMapInfo& dataMap,
				kome::img::Graphics& g,
				wxWindow* canvas
			);

		public:
			/**
			 * @fn static bool onSpectrumButtonDown(
					kome::objects::Spectrum& spec,
					kome::img::Graphics* g,
					kome::evt::MouseEvent& evt,
					wxWindow* canvas
				)
			 * @brief broadcasts mouse button is down on spectrum window
			 * @param[in] spec spectrum object
			 * @param[in] g canvas graphics object
			 * @param[in] evt mouse event information
			 * @param[in] canvas canvas ID
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onSpectrumButtonDown(
				kome::objects::Spectrum& spec,
				kome::img::Graphics* g,
				kome::evt::MouseEvent& evt,
				wxWindow* canvas
			);

			/**
			 * @fn static bool onSpectrumButtonUp(
					kome::objects::Spectrum& spec,
					kome::img::Graphics* g,
					kome::evt::MouseEvent& evt,
					wxWindow* canvas
				)
			 * @brief broadcasts mouse button is up on spectrum window
			 * @param[in] spec spectrum object
			 * @param[in] g canvas graphics object
			 * @param[in] evt mouse event information
			 * @param[in] canvas canvas ID
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onSpectrumButtonUp(
				kome::objects::Spectrum& spec,
				kome::img::Graphics* g,
				kome::evt::MouseEvent& evt,
				wxWindow* canvas
			);

			/**
			 * @fn static bool onSpectrumDoubleClick(
					kome::objects::Spectrum& spec,
					kome::img::Graphics* g,
					kome::evt::MouseEvent& evt,
					wxWindow* canvas
				)
			 * @brief broadcasts mouse button is double clicked on spectrum window
			 * @param[in] spec spectrum object
			 * @param[in] g canvas graphics object
			 * @param[in] evt mouse event information
			 * @param[in] canvas canvas ID
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onSpectrumDoubleClick(
				kome::objects::Spectrum& spec,
				kome::img::Graphics* g,
				kome::evt::MouseEvent& evt,
				wxWindow* canvas
			);

			/**
			 * @fn static bool onSpectrumMouseMove(
					kome::objects::Spectrum& spec,
					kome::img::Graphics* g,
					kome::evt::MouseEvent& evt,
					wxWindow* canvas
				)
			 * @brief broadcasts mouse cursor moves on spectrum window
			 * @param[in] spec spectrum object
			 * @param[in] g canvas graphics object
			 * @param[in] evt mouse event information
			 * @param[in] canvas canvas ID
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onSpectrumMouseMove(
				kome::objects::Spectrum& spec,
				kome::img::Graphics* g,
				kome::evt::MouseEvent& evt,
				wxWindow* canvas
			);

			/**
			 * @fn static bool onSpectrumMouseLeave(
					kome::objects::Spectrum& spec,
					kome::img::Graphics* g,
					kome::evt::MouseEvent& evt,
					wxWindow* canvas
				)
			 * @brief broadcasts mouse cursor leaves from spectrum window
			 * @param[in] spec spectrum object
			 * @param[in] g canvas graphics object
			 * @param[in] evt mouse event information
			 * @param[in] canvas canvas ID
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onSpectrumMouseLeave(
				kome::objects::Spectrum& spec,
				kome::img::Graphics* g,
				kome::evt::MouseEvent& evt,
				wxWindow* canvas
			);

			/**
			 * @fn static bool onSpectrumMouseEnter(
					kome::objects::Spectrum& spec,
					kome::img::Graphics* g,
					kome::evt::MouseEvent& evt,
					wxWindow* canvas
				)
			 * @brief broad casts mouse cursor enters into spectrum window
			 * @param[in] spec spectrum object
			 * @param[in] g canvas graphics object
			 * @param[in] evt mouse event information
			 * @param[in] canvas canvas ID
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onSpectrumMouseEnter(
				kome::objects::Spectrum& spec,
				kome::img::Graphics* g,
				kome::evt::MouseEvent& evt,
				wxWindow* canvas
			);

			/**
			 * @fn static bool onChromatogramButtonDown(
					kome::objects::Chromatogram& chrom,
					kome::img::Graphics* g,
					kome::evt::MouseEvent& evt,
					wxWindow* canvas
				)
			 * @brief broadcasts mouse button is down on chromatogram window
			 * @param[in] chrom chromatogram object
			 * @param[in] g canvas graphics object
			 * @param[in] evt mouse event information
			 * @param[in] canvas canvas ID
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onChromatogramButtonDown(
				kome::objects::Chromatogram& chrom,
				kome::img::Graphics* g,
				kome::evt::MouseEvent& evt,
				wxWindow* canvas
			);

			/**
			 * @fn static bool onChromatogramButtonUp(
					kome::objects::Chromatogram& chrom,
					kome::img::Graphics* g,
					kome::evt::MouseEvent& evt,
					wxWindow* canvas
				)
			 * @brief broadcasts mouse button is up on chromatogram window
			 * @param[in] chrom chromatogram object
			 * @param[in] g canvas graphics object
			 * @param[in] evt mouse event information
			 * @param[in] canvas canvas ID
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onChromatogramButtonUp(
				kome::objects::Chromatogram& chrom,
				kome::img::Graphics* g,
				kome::evt::MouseEvent& evt,
				wxWindow* canvas
			);

			/**
			 * @fn static bool onChromatogramDoubleClick(
					kome::objects::Chromatogram& chrom,
					kome::img::Graphics* g,
					kome::evt::MouseEvent& evt,
					wxWindow* canvas
				)
			 * @brief broadcasts mouse button is double clicked on chromatogram window
			 * @param[in] chrom chromatogram object
			 * @param[in] g canvas graphics object
			 * @param[in] evt mouse event information
			 * @param[in] canvas canvas ID
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onChromatogramDoubleClick(
				kome::objects::Chromatogram& chrom,
				kome::img::Graphics* g,
				kome::evt::MouseEvent& evt,
				wxWindow* canvas
			);

			/**
			 * @fn static bool onChromatogramMouseMove(
					kome::objects::Chromatogram& chrom,
					kome::img::Graphics* g,
					kome::evt::MouseEvent& evt,
					wxWindow* canvas
				)
			 * @brief broadcasts mouse cursor moves on chromatogram window
			 * @param[in] chrom chromatogram object
			 * @param[in] g canvas graphics object
			 * @param[in] evt mouse event information
			 * @param[in] canvas canvas ID
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onChromatogramMouseMove(
				kome::objects::Chromatogram& chrom,
				kome::img::Graphics* g,
				kome::evt::MouseEvent& evt,
				wxWindow* canvas
			);

			/**
			 * @fn static bool onChromatogramMouseLeave(
					kome::objects::Chromatogram& chrom,
					kome::img::Graphics* g,
					kome::evt::MouseEvent& evt,
					wxWindow* canvas
				)
			 * @brief broadcasts mouse cursor leaves from chromatogram window
			 * @param[in] chrom chromatogram object
			 * @param[in] g canvas graphics object
			 * @param[in] evt mouse event information
			 * @param[in] canvas canvas ID
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onChromatogramMouseLeave(
				kome::objects::Chromatogram& chrom,
				kome::img::Graphics* g,
				kome::evt::MouseEvent& evt,
				wxWindow* canvas
			);

			/**
			 * @fn static bool onChromatogramMouseEnter(
					kome::objects::Chromatogram& chrom,
					kome::img::Graphics* g,
					kome::evt::MouseEvent& evt,
					wxWindow* canvas
				)
			 * @brief broadcasts mouse cursor enters into chromatogram window
			 * @param[in] chrom chromatogram object
			 * @param[in] g canvas graphics object
			 * @param[in] evt mouse event information
			 * @param[in] canvas canvas ID
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onChromatogramMouseEnter(
				kome::objects::Chromatogram& chrom,
				kome::img::Graphics* g,
				kome::evt::MouseEvent& evt,
				wxWindow* canvas
			);

			/**
			 * @fn static bool onDataMapButtonDown(
					kome::objects::DataMapInfo& dataMap,
					kome::img::Graphics* g,
					kome::evt::MouseEvent& evt,
					wxWindow* canvas
				)
			 * @brief broadcasts mouse button is down on data map window
			 * @param[in] dataMap data map object
			 * @param[in] g canvas graphics object
			 * @param[in] evt mouse event information
			 * @param[in] canvas canvas ID
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onDataMapButtonDown(
				kome::objects::DataMapInfo& dataMap,
				kome::img::Graphics* g,
				kome::evt::MouseEvent& evt,
				wxWindow* canvas
			);

			/**
			 * @fn static bool onDataMapButtonUp(
					kome::objects::DataMapInfo& dataMap,
					kome::img::Graphics* g,
					kome::evt::MouseEvent& evt,
					wxWindow* canvas
				)
			 * @brief broadcasts mouse button is up on data map window
			 * @param[in] dataMap data map object
			 * @param[in] g canvas graphics object
			 * @param[in] evt mouse event information
			 * @param[in] canvas canvas ID
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onDataMapButtonUp(
				kome::objects::DataMapInfo& dataMap,
				kome::img::Graphics* g,
				kome::evt::MouseEvent& evt,
				wxWindow* canvas
			);

			/**
			 * @fn static bool onDataMapDoubleClick(
					kome::objects::DataMapInfo& dataMap,
					kome::img::Graphics* g,
					kome::evt::MouseEvent& evt,
					wxWindow* canvas
				)
			 * @brief broadcasts mouse button is double clicked on data map window
			 * @param[in] dataMap data map object
			 * @param[in] g canvas graphics object
			 * @param[in] evt mouse event information
			 * @param[in] canvas canvas ID
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onDataMapDoubleClick(
				kome::objects::DataMapInfo& dataMap,
				kome::img::Graphics* g,
				kome::evt::MouseEvent& evt,
				wxWindow* canvas
			);

			/**
			 * @fn static bool onDataMapMouseMove(
					kome::objects::DataMapInfo& dataMap,
					kome::img::Graphics* g,
					kome::evt::MouseEvent& evt,
					wxWindow* canvas
				)
			 * @brief broadcasts mouse cursor moves on data map window
			 * @param[in] dataMap data map object
			 * @param[in] g canvas graphics object
			 * @param[in] evt mouse event information
			 * @param[in] canvas canvas ID
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onDataMapMouseMove(
				kome::objects::DataMapInfo& dataMap,
				kome::img::Graphics* g,
				kome::evt::MouseEvent& evt,
				wxWindow* canvas
			);

			/**
			 * @fn static bool onDataMapMouseLeave(
					kome::objects::DataMapInfo& dataMap,
					kome::img::Graphics* g,
					kome::evt::MouseEvent& evt,
					wxWindow* canvas
				)
			 * @brief broadcasts mouse cursor leaves from data map window
			 * @param[in] dataMap data map object
			 * @param[in] g canvas graphics object
			 * @param[in] evt mouse event information
			 * @param[in] canvas canvas ID
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onDataMapMouseLeave(
				kome::objects::DataMapInfo& dataMap,
				kome::img::Graphics* g,
				kome::evt::MouseEvent& evt,
				wxWindow* canvas
			);

			/**
			 * @fn static bool onDataMapMouseEnter(
					kome::objects::DataMapInfo& dataMap,
					kome::img::Graphics* g,
					kome::evt::MouseEvent& evt,
					wxWindow* canvas
				)
			 * @brief broad casts mouse cursor enters into data map window
			 * @param[in] dataMap data map object
			 * @param[in] g canvas graphics object
			 * @param[in] evt mouse event information
			 * @param[in] canvas canvas ID
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onDataMapMouseEnter(
				kome::objects::DataMapInfo& dataMap,
				kome::img::Graphics* g,
				kome::evt::MouseEvent& evt,
				wxWindow* canvas
			);

		public:
			/**
			 * @fn static bool onSpectrumKeyDown(
						kome::objects::Spectrum& spec,
						kome::img::Graphics* graphic,
						kome::evt::KeyEvent& evt,
						wxWindow* canvas
					)
			 * @brief broadcasts key is down on spectrum window
			 * @param[in] spec spectrum object
			 * @param[in] graphic canvas graphics object
			 * @param[in] evt key event information
			 * @param[in] canvas canvas ID
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onSpectrumKeyDown(
				kome::objects::Spectrum& spec,
				kome::img::Graphics* graphic,
				kome::evt::KeyEvent& evt,
				wxWindow* canvas
			);

			/**
			 * @fn static bool onSpectrumKeyUp(
						kome::objects::Spectrum& spec,
						kome::img::Graphics* graphic,
						kome::evt::KeyEvent& evt,
						wxWindow* canvas
					)
			 * @brief broadcasts key is up on spectrum window
			 * @param[in] spec spectrum object
			 * @param[in] graphic canvas graphics object
			 * @param[in] evt key event information
			 * @param[in] canvas canvas ID
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onSpectrumKeyUp(
				kome::objects::Spectrum& spec,
				kome::img::Graphics* graphic,
				kome::evt::KeyEvent& evt,
				wxWindow* canvas
			);

			/**
			 * @fn static bool onChromatogramKeyDown(
						kome::objects::Chromatogram& chrom,
						kome::img::Graphics* graphic,
						kome::evt::KeyEvent& evt,
						wxWindow* canvas
					)
			 * @brief broadcasts key is down on chromatogram window
			 * @param[in] chrom chromatogram object
			 * @param[in] graphic canvas graphics object
			 * @param[in] evt key event information
			 * @param[in] canvas canvas ID
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onChromatogramKeyDown(
				kome::objects::Chromatogram& chrom,
				kome::img::Graphics* graphic,
				kome::evt::KeyEvent& evt,
				wxWindow* canvas
			);

			/**
			 * @fn static bool onChromatogramKeyUp(
						kome::objects::Chromatogram& chrom,
						kome::img::Graphics* graphic,
						kome::evt::KeyEvent& evt,
						wxWindow* canvas
					)
			 * @brief broadcasts key is up on chromatogram window
			 * @param[in] chrom chromatogram object
			 * @param[in] graphic canvas graphics object
			 * @param[in] evt key event information
			 * @param[in] canvas canvas ID
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onChromatogramKeyUp(
				kome::objects::Chromatogram& chrom,
				kome::img::Graphics* graphic,
				kome::evt::KeyEvent& evt,
				wxWindow* canvas
			);

			/**
			 * @fn static bool onDataMapKeyDown(
						kome::objects::DataMapInfo& dataMap,
						kome::img::Graphics* graphic,
						kome::evt::KeyEvent& evt,
						wxWindow* canvas
					)
			 * @brief broadcasts key is down on data map window
			 * @param[in] dataMap data map object
			 * @param[in] graphic canvas graphics object
			 * @param[in] evt key event information
			 * @param[in] canvas canvas ID
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onDataMapKeyDown(
				kome::objects::DataMapInfo& dataMap,
				kome::img::Graphics* graphic,
				kome::evt::KeyEvent& evt,
				wxWindow* canvas
			);

			/**
			 * @fn static bool onDataMapKeyUp(
						kome::objects::DataMapInfo& dataMap,
						kome::img::Graphics* graphic,
						kome::evt::KeyEvent& evt,
						wxWindow* canvas
					)
			 * @brief broadcasts key is up on data map window
			 * @param[in] dataMap data map object
			 * @param[in] graphic canvas graphics object
			 * @param[in] evt key event information
			 * @param[in] canvas canvas ID
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onDataMapKeyUp(
				kome::objects::DataMapInfo& dataMap,
				kome::img::Graphics* graphic,
				kome::evt::KeyEvent& evt,
				wxWindow* canvas
			);

		protected:
			/**
			 * @fn static void onOpenEvent(
					PluginCall* call,
					kome::objects::Parameters* params,
					bool* enable
				)
			 * @brief calls input event plug-in function
			 * @param[in] call plug-in function object
			 * @param[in] params function parameters
			 * @param[in] enable the pointer to store enable flag
			 */
			static void onOpenEvent(
				PluginCall* call,
				kome::objects::Parameters* params,
				bool* enable
			);

			/**
			 * @fn static bool callOpenFunctions( kome::objects::Parameters& params, const char* type )
			 * @brief calls plug-in functions for open event
			 * @param[in] params function parameters
			 * @param[in] type plug-in function type
			 * @return If false, it is impossible to open
			 */
			static bool callOpenFunctions( kome::objects::Parameters& params, const char* type );

			/**
			 * @fn static void onDrawProfile(
					PluginCall* call,
					kome::objects::Parameters* params,
					kome::img::Graphics* graphics
				)
			 * @brief calls drawing profile plug-in function
			 * @param[in] call plug-in function object
			 * @param[in] params function parameters
			 * @param[in,out] graphics graphics object
			 */
			static void onDrawProfile(
				PluginCall* call,
				kome::objects::Parameters* params,
				kome::img::Graphics* graphics
			);

			/**
			 * @fn static void onOpenWindow(
					PluginCall* call,
					kome::objects::Parameters* params,
					wxWindow** wnd
				)
			 * @brief calls open window plug-in function
			 * @param[in] call plug-in function object
			 * @param[in] params function parameters
			 * @param[out] wnd the pointer to store window object
			 */
			static void onOpenWindow(
				PluginCall* call,
				kome::objects::Parameters* params,
				wxWindow** wnd
			);

			/**
			 * @fn static wxWindow* callOpenWindowFunctions( kome::objects::Parameters& params, const char* type )
			 * @brief calls plug-in functions for opening window
			 * @param[in] params function parameters
			 * @param[in] type plug-in function type
			 * @return window object
			 */
			static wxWindow* callOpenWindowFunctions( kome::objects::Parameters& params, const char* type );

			/**
			 * @fn static void onInputEvent(
					PluginCall* call,
					kome::objects::Parameters* params,
					bool* refresh
				)
			 * @brief calls input event plug-in function
			 * @param[in] call plug-in function object
			 * @param[in] params function parameters
			 * @param[in] refresh the pointer to store refresh flag
			 */
			static void onInputEvent(
				PluginCall* call,
				kome::objects::Parameters* params,
				bool* refresh
			);

			/**
			 * @fn static bool callInputFunctions( kome::objects::Parameters& params, const char* type )
			 * @brief calls plug-in functions for input event
			 * @param[in] params function parameters
			 * @param[in] type plug-in function type
			 * @return If it has to refresh, this method returns true.
			 */
			static bool callInputFunctions( kome::objects::Parameters& params, const char* type );

		public:
			/**
			 * @fn static void setSampleSet( kome::objects::Parameters& params, kome::objects::SampleSet& sampleSet )
			 * @brief sets sample set information to parameters object
			 * @param[out] params parameters object
			 * @param[in] sampleSet sample set object
			 */
			static void setSampleSet( kome::objects::Parameters& params, kome::objects::SampleSet& sampleSet );

			/**
			 * @fn static void setSample( kome::objects::Parameters& params, kome::objects::Sample& sample )
			 * @brief sets sample information to parameters object
			 * @param[out] params parameters object
			 * @param[in] sample sample object
			 */
			static void setSample( kome::objects::Parameters& params, kome::objects::Sample& sample );

			/**
			 * @fn static void setSpectrum( kome::objects::Parameters& params, kome::objects::Spectrum& spec )
			 * @brief sets spectrum information to parameters object
			 * @param[out] params parameters object
			 * @param[in] spec spectrum object
			 */
			static void setSpectrum( kome::objects::Parameters& params, kome::objects::Spectrum& spec );

			/**
			 * @fn static void setChromatogram( kome::objects::Parameters& params, kome::objects::Chromatogram& chrom )
			 * @brief sets chromatogram information to parameters object
			 * @param[out] params parameters object
			 * @param[in] chrom chromatogram object
			 */
			static void setChromatogram( kome::objects::Parameters& params, kome::objects::Chromatogram& chrom );

			/**
			 * @fn static void setXYData( kome::objects::Parameters& params, kome::core::XYData& xyData )
			 * @brief sets xy data to parameters object
			 * @param[out] params parameters object
			 * @param[in] xyData xy data object
			 */
			static void setXYData( kome::objects::Parameters& params, kome::core::XYData& xyData );

			/**
			 * @fn static void setGroup( kome::objects::Parameters& params, kome::objects::DataGroupNode& group )
			 * @brief sets spectrum group
			 * @param[out] params parameters object
			 * @param[in] group spectrum group object
			 */
			static void setGroup( kome::objects::Parameters& params, kome::objects::DataGroupNode& group );

			/**
			 * @fn static void setDataSet( kome::objects::Parameters& params, kome::objects::DataSet& dataSet )
			 * @brief sets data set to parameters object
			 * @param[out] params parameters object
			 * @param[in] dataSet data set
			 */
			static void setDataSet( kome::objects::Parameters& params, kome::objects::DataSet& dataSet );

			/**
			 * @fn static void setSpectraFilter( kome::objects::Parameters& params, kome::objects::DataSet& dataSet )
			 * @brief sets data set to parameters object
			 * @param[out] params parameters object
			 * @param[in] dataSet data set
			 */
			static void setSpectraFilter( kome::objects::Parameters& params, kome::objects::DataSet& dataSet );
			
			/**
			 * @fn static void setPeaks( kome::objects::Parameters& params, kome::objects::Peaks& peaks )
			 * @brief sets peaks data to parameters object
			 * @param[out] params parameters object
			 * @param[in] peaks peaks object
			 */
			static void setPeaks( kome::objects::Parameters& params, kome::objects::Peaks& peaks );

			/**
			 * @fn static void setParentPeaks( kome::objects::Parameters& params, kome::objects::Peaks& parentPeaks )
			 * @brief sets parent peaks to parameters object
			 * @param[out] params parameters object
			 * @param[in] parentPeaks parent peaks object
			 */
			static void setParentPeaks( kome::objects::Parameters& params, kome::objects::Peaks& parentPeaks );

			/**
			 * @fn static void setPeaks2D( kome::objects::Parameters& params, kome::objects::Peaks2D& peaks )
			 * @brief sets 2D peaks data to parameters object
			 * @param[out] params parameters object
			 * @param[in] peaks peaks object
			 */
			static void setPeaks2D( kome::objects::Parameters& params, kome::objects::Peaks2D& peaks );

			/**
			 * @fn static void setBaseline( kome::objects::Parameters& params, kome::core::XYData& baseline )
			 * @brief sets baseline object to parameters object
			 * @param[out] params parameters object
			 * @param[in] baseline baseline object
			 */
			static void setBaseline( kome::objects::Parameters& params, kome::core::XYData& baseline );

			/**
			 * @fn static void setUpdatedXYData( kome::objects::Parameters& params, kome::core::XYData& updated )
			 * @brief sets updated xy data object to prameters object
			 * @param[out] params parameters object
			 * @param[in] updated updated xy data object
			 */
			static void setUpdatedXYData( kome::objects::Parameters& params, kome::core::XYData& updated );

			/**
			 * @fn static void setUpdatedPeaks( kome::objects::Parameters& params, kome::objects::Peaks& updated )
			 * @brief sets updated peaks object to parameters object
			 * @param[out] params paramters object
			 * @param[in] updated updated peaks object
			 */
			static void setUpdatedPeaks( kome::objects::Parameters& params, kome::objects::Peaks& updated );

			/**
			 * @fn static void setUpdatedPeaks2d( kome::objects::Parameters& params,kome::objects::Peaks2D& updated )
			 * @brief sets updated 2d peaks object to parameters object
			 * @param[out] params paramters object
			 * @param[in] updated updated 2d peaks object
			 */
			static void setUpdatedPeaks2d( kome::objects::Parameters& params, kome::objects::Peaks2D& updated );
			
			/**
			 * @fn static void setControlSpectrum( kome::objects::Parameters& params, kome::objects::Spectrum& spec )
			 * @brief sets control spectrum
			 * @param[out] params parameter object
			 * @param[in] spec control spectrum
			 */
			static void setControlSpectrum( kome::objects::Parameters& params, kome::objects::Spectrum& spec );

			/**
			 * @fn static void setControlChromatogram( kome::objects::Parameters& params, kome::objects::Chromatogram& chrom )
			 * @brief sets control chromatogram
			 * @param[out] params parameters object
			 * @param[in] chrom control chromatogram
			 */
			static void setControlChromatogram( kome::objects::Parameters& params, kome::objects::Chromatogram& chrom );

			/**
			 * @fn static void setControlGroup( kome::objects::Parameters& params, kome::objects::DataGroupNode& group )
			 * @brief sets control data set
			 * @param[out] params parameters object
			 * @param[in] group control group
			 */
			static void setControlGroup( kome::objects::Parameters& params, kome::objects::DataGroupNode& group );

			/**
			 * @fn static void setControlXYData( kome::objects::Parameters& params, kome::core::XYData& xyData )
			 * @brief sets control xy data
			 * @param[out] params parameters object
			 * @param[in] xyData control xy data
			 */
			static void setControlXYData( kome::objects::Parameters& params, kome::core::XYData& xyData );

			/**
			 * @fn static void setPath( kome::objects::Parameters& params, const char* path )
			 * @brief sets file path
			 * @param[out] params parameters object
			 * @param[in] path file path
			 */
			static void setPath( kome::objects::Parameters& params, const char* path );

			/**
			 * @fn static void setStartOfRange( kome::objects::Parameters& params, const double start )
			 * @brief sets start of range to parameters object
			 * @param[out] params parameters object
			 * @param[in] start start of range
			 */
			static void setStartOfRange( kome::objects::Parameters& params, const double start );

			/**
			 * @fn static void setEndOfRange( kome::objects::Parameters& params, const double end )
			 * @brief sets end of range to parameters object
			 * @param[out] params parameters object
			 * @param[in] end end of range
			 */
			static void setEndOfRange( kome::objects::Parameters& params, const double end );

			/**
			 * @fn static void setRange( kome::objects::Parameters& params, const double start, const double end )
			 * @brief sets range to parameters object
			 * @param[out] params parameters object
			 * @param[in] start start of range
			 * @param[in] end end of range
			 */
			static void setRange( kome::objects::Parameters& params, const double start, const double end );

			/**
			 * @fn static void setProgress( kome::objects::Parameters& params, kome::core::Progress& progress )
			 * @brief sets progress to parameters object
			 * @param[out] params parameters object
			 * @param[in] progress progress object
			 */
			static void setProgress( kome::objects::Parameters& params, kome::core::Progress& progress );

			/**
			 * @fn static void setMouseEvent( kome::objects::Parameters& params, kome::evt::MouseEvent& evt )
			 * @brief sets mouse event
			 * @param[out] params parameters object
			 * @param[in] evt mouse event object
			 */
			static void setMouseEvent( kome::objects::Parameters& params, kome::evt::MouseEvent& evt );

			/**
			 * @fn static void setKeyEvent( kome::objects::Parameters& params, kome::evt::KeyEvent& evt )
			 * @brief sets key event
			 * @param[out] params parameters object
			 * @param[in] evt key event object
			 */
			static void setKeyEvent( kome::objects::Parameters& params, kome::evt::KeyEvent& evt );

			/**
			 * @fn static void setSizeEvent( kome::objects::Parameters& params, kome::evt::SizeEvent& evt )
			 * @brief sets size event
			 * @param[out] params parameters object
			 * @param[in] evt size event object
			 */
			static void setSizeEvent( kome::objects::Parameters& params, kome::evt::SizeEvent& evt );


			/**
			 * @fn static void setMoveEvent( kome::objects::Parameters& params, kome::evt::MoveEvent& evt )
			 * @brief sets size event
			 * @param[out] params parameters object
			 * @param[in] evt move event object
			 */
			static void setMoveEvent( kome::objects::Parameters& params, kome::evt::MoveEvent& evt );

			/**
			 * @fn static void setGraphics( kome::objects::Parameters& params, kome::img::Graphics& g )
			 * @brief sets graphics
			 * @param[out] params parameters object
			 * @param[in] g graphics object
			 */
			static void setGraphics( kome::objects::Parameters& params, kome::img::Graphics& g );

			/**
			 * @fn static void setDataMapInfo( kome::objects::Parameters& params, kome::objects::DataMapInfo& dataMap )
			 * @brief sets data map
			 * @param[out] params parameters object
			 * @param[in] dataMap data map object
			 */
			static void setDataMapInfo( kome::objects::Parameters& params, kome::objects::DataMapInfo& dataMap );

			/**
			 * @fn static void setWindow( kome::objects::Parameters& params, wxWindow* wnd )
			 * @brief sets window
			 * @param[out] params parameters object
			 * @param[in] wnd window object
			 */
			static void setWindow( kome::objects::Parameters& params, wxWindow* wnd );

			/**
			 * @fn static void setParentWindow( kome::objects::Parameters& params, wxWindow* parent )
			 * @brief sets parent window
			 * @param[out] params parameters object
			 * @param[in] parent parent window object
			 */
			static void setParentWindow( kome::objects::Parameters& params, wxWindow* parent );

			/**
			 * @fn static void setCanvas( kome::objects::Parameters& params, wxWindow* canvas )
			 * @brief sets canvas ID
			 * @param[out] params parameters object
			 * @param[in] canvas canvas ID
			 */
			static void setCanvas( kome::objects::Parameters& params, wxWindow* canvas );

			/** 
			 * @fn static void setSettingValues( kome::objects::Parameters& params, kome::objects::SettingParameterValues& vals )
			 * @brief sets setting parameter values
			 * @param[out] params parameters object
			 * @param[in] vals setting parameter values object
			 */
			static void setSettingValues( kome::objects::Parameters& params, kome::objects::SettingParameterValues& vals );

			/**
			 * @fn static void setActiveObject( kome::objects::Parameters& params, kome::objects::Variant& obj )
			 * @brief sets active object
			 * @param[out] params parameters object
			 * @param[in] obj active object
			 */
			static void setActiveObject( kome::objects::Parameters& params, kome::objects::Variant& obj );

			/**
			 * @fn static void setSampleGetter( kome::objects::Parameters& params, kome::ident::SampleGetter& getter )
			 * @brief sets sample getter
			 * @param[out] params parameters object
			 * @param[in] getter sample getter object
			 */
			static void setSampleGetter( kome::objects::Parameters& params, kome::ident::SampleGetter& getter );

		public:
			/**
			 * @fn static kome::objects::SampleSet* getSampleSet( kome::objects::Parameters* params )
			 * @brief get sample set from parameters information
			 * @param[in] params parameters information
			 * @return sample set
			 */
			static kome::objects::SampleSet* getSampleSet( kome::objects::Parameters* params );

			/**
			 * @fn static kome::objects::Sample* getSample( kome::objects::Parameters* params )
			 * @brief get sample from parameters information
			 * @param[in] params parameters information
			 * @return sample
			 */
			static kome::objects::Sample* getSample( kome::objects::Parameters* params );

			/**
			 * @fn static kome::objects::Spectrum* getSpectrum( kome::objects::Parameters* params )
			 * @brief gets spectrum from parameters information
			 * @param[in] params parameters information
			 * @return spectrum
			 */
			static kome::objects::Spectrum* getSpectrum( kome::objects::Parameters* params );

			/**
			 * @fn static kome::objects::Chromatogram* getChromatogram( kome::objects::Parameters* params )
			 * @brief gets chromatogram from parameters information
			 * @param[in] params parameters information
			 * @return chromatogram
			 */
			static kome::objects::Chromatogram* getChromatogram( kome::objects::Parameters* params );

			/**
			 * @fn static kome::core::XYData* getXYData( kome::objects::Parameters* params )
			 * @brief gets xy data from parameters information
			 * @param[in] params parameters information
			 * @return xy data
			 */
			static kome::core::XYData* getXYData( kome::objects::Parameters* params );

			/**
			 * @fn static kome::objects::DataSet* getDataSet( kome::objects::Parameters* params )
			 * @brief gets data set from parameters information
			 * @param[in] params parameters information
			 * @return data set
			 */
			static kome::objects::DataSet* getDataSet( kome::objects::Parameters* params );

			
			/**
			 * @fn static kome::objects::DataSet* getSpectraFilter( kome::objects::Parameters* params )
			 * @brief gets data set from parameters information
			 * @param[in] params parameters information
			 * @return data set
			 */
			static kome::objects::DataSet* getSpectraFilter( kome::objects::Parameters* params );

			/**
			 * @fn static kome::objects::DataGroupNode* getGroup( kome::objects::Parameters* params )
			 * @brief gets spectrum group
			 * @param[in] params parameters information
			 * @return spectrum group
			 */
			static kome::objects::DataGroupNode* getGroup( kome::objects::Parameters* params );

			/**
			 * @fn static kome::objects::Peaks* getPeaks( kome::objects::Parameters* params )
			 * @brief gets peaks from parameters information
			 * @param[in] params parameters information
			 * @return peaks
			 */
			static kome::objects::Peaks* getPeaks( kome::objects::Parameters* params );

			/**
			 * @fn static kome::objects::Peaks* getParentPeaks( kome::objects::Parameters* params )
			 * @brief gets parent peaks from parameters information
			 * @param[in] params parameters information
			 * @return parent peaks
			 */
			static kome::objects::Peaks* getParentPeaks( kome::objects::Parameters* params );

			/**
			 * @fn static kome::objects::Peaks2D* getPeaks2D( kome::objects::Parameters* params )
			 * @brief gets 2D peaks from parameters information
			 * @param[in] params parameters information
			 * @return peaks
			 */
			static kome::objects::Peaks2D* getPeaks2D( kome::objects::Parameters* params );

			/**
			 * @fn static kome::core::XYData* getBaseline( kome::objects::Parameters* params )
			 * @brief gets baseline object from parameters information
			 * @param[in] params parameters information
			 * @return baseline object
			 */
			static kome::core::XYData* getBaseline( kome::objects::Parameters* params );

			/**
			 * @fn static kome::core::XYData* getUpdatedXYData( kome::objects::Parameters* params )
			 * @brief gets updated xy data object from parameters information
			 * @param[in] params parameters information
			 * @return updated xy data object
			 */
			static kome::core::XYData* getUpdatedXYData( kome::objects::Parameters* params );

			/**
			 * @fn static kome::objects::Peaks* getUpdatedPeaks( kome::objects::Parameters* params )
			 * @brief gets updated peaks
			 * @param[in] params parameters information
			 * @return updated peaks data object
			 */
			static kome::objects::Peaks* getUpdatedPeaks( kome::objects::Parameters* params );

			/**
			 * @fn static kome::objects::Peaks2D* getUpdatedPeaks2D( kome::objects::Parameters* params )
			 * @brief gets updated 2d peaks
			 * @param[in] params parameters information
			 * @return updated peaks data object
			 */
			static kome::objects::Peaks2D* getUpdatedPeaks2D( kome::objects::Parameters* params );

			/**
			 * @fn static kome::objects::Spectrum* getControlSpectrum( kome::objects::Parameters* params )
			 * @brief gets control spectrum
			 * @param[in] params parameter information
			 * @return control spectrum
			 */
			static kome::objects::Spectrum* getControlSpectrum( kome::objects::Parameters* params );

			/**
			 * @fn static kome::objects::Chromatogram* getControlChromatogram( kome::objects::Parameters* params )
			 * @brief gets control chromatogram
			 * @param[in] params parameter information
			 * @return control chromatogram
			 */
			static kome::objects::Chromatogram* getControlChromatogram( kome::objects::Parameters* params );

			/**
			 * @fn static kome::objects::DataGroupNode* getControlGroup( kome::objects::Parameters* params )
			 * @brief gets control data set
			 * @param[in] params parameter information
			 * @return control group
			 */
			static kome::objects::DataGroupNode* getControlGroup( kome::objects::Parameters* params );

			/**
			 * @fn static kome::core::XYData* getControlXYData( kome::objects::Parameters* params )
			 * @brief gets control xy data
			 * @param[in] params parameter information
			 * @return control xy data
			 */
			static kome::core::XYData* getControlXYData( kome::objects::Parameters* params );

			/**
			 * @fn static char* getPath( kome::objects::Parameters* params )
			 * @brief gets file path from parameters information
			 * @param[in] params parameters information
			 * @return file path
			 */
			static char* getPath( kome::objects::Parameters* params );

			/**
			 * @fn static bool getStartOfRange( kome::objects::Parameters* params, double* start )
			 * @brief gets start of range from parameters object
			 * @param[in] params parameters object
			 * @param[in] start the pointer to store start of range
			 * @return If true, it succeeded to get the start of range
			 */
			static bool getStartOfRange( kome::objects::Parameters* params, double* start );

			/**
			 * @fn static bool getEndOfRange( kome::objects::Parameters* params, double* end )
			 * @brief gets end of range from parameters object
			 * @param[in] params parameters object
			 * @param[in] end the pointer to store end of range
			 * @return If true, it succeeded to get the end of range
			 */
			static bool getEndOfRange( kome::objects::Parameters* params, double* end );

			/**
			 * @fn static bool getRange( kome::objects::Parameters* params, double* start, double* end )
			 * @brief gets range from parameters object
			 * @param[in] params parameters object
			 * @param[in] start the pointer to store start of range
			 * @param[in] end the pointer to store end of range
			 * @return If true, it succeeded to get the range
			 */
			static bool getRange( kome::objects::Parameters* params, double* start, double* end );

			/**
			 * @fn static kome::core::Progress* getProgress( kome::objects::Parameters* params )
			 * @brief gets progress from parameters object
			 * @param[in] params parameters object
			 * @return progress object 
			 */
			static kome::core::Progress* getProgress( kome::objects::Parameters* params );

			/**
			 * @fn static kome::evt::MouseEvent* getMouseEvent( kome::objects::Parameters* params )
			 * @brief gets mouse event
			 * @param[in] params parameters object
			 * @return mouse event object
			 */
			static kome::evt::MouseEvent* getMouseEvent( kome::objects::Parameters* params );

			/**
			 * @fn static kome::evt::KeyEvent* getKeyEvent( kome::objects::Parameters* params )
			 * @brief gets key event
			 * @param[in] params paramters object
			 * @return key event object
			 */
			static kome::evt::KeyEvent* getKeyEvent( kome::objects::Parameters* params );

			/**
			 * @fn static kome::evt::SizeEvent* getSizeEvent( kome::objects::Parameters* params )
			 * @brief gets size event
			 * @param[in] params paramters object
			 * @return size event object
			 */
			static kome::evt::SizeEvent* getSizeEvent( kome::objects::Parameters* params );

			/**
			 * @fn static kome::evt::MoveEvent* getMoveEvent( kome::objects::Parameters* params )
			 * @brief gets move event
			 * @param[in] params paramters object
			 * @return move event object
			 */
			static kome::evt::MoveEvent* getMoveEvent( kome::objects::Parameters* params );

			/**
			 * @fn static kome::img::Graphics* getGraphics( kome::objects::Parameters* params )
			 * @brief gets graphics
			 * @param[in] params parameters object
			 * @return graphics
			 */
			static kome::img::Graphics* getGraphics( kome::objects::Parameters* params );

			/**
			 * @fn static kome::objects::DataMapInfo* getDataMapInfo( kome::objects::Parameters* params )
			 * @brief gets data map
			 * @param[in] params parameters object
			 * @return data map
			 */
			static kome::objects::DataMapInfo* getDataMapInfo( kome::objects::Parameters* params );

			/**
			 * @fn static wxWindow* getWindow( kome::objects::Parameters* params )
			 * @brief gets window
			 * @param[in] params parameters object
			 * @return window
			 */
			static wxWindow* getWindow( kome::objects::Parameters* params );

			/**
			 * @fn static wxWindow* getParentWindow( kome::objects::Parameters* params )
			 * @brief gets parent window
			 * @param[in] params parameters object
			 * @return parent window
			 */
			static wxWindow* getParentWindow( kome::objects::Parameters* params );

			/**
			 * @fn static wxWindow* getCanvas( kome::objects::Parameters* params )
			 * @brief gets canvas ID
			 * @param[in] params paramters object
			 * @return canvas ID
			 */
			static wxWindow* getCanvas( kome::objects::Parameters* params );

			/** 
			 * @fn static kome::objects::SettingParameterValues* getSettingValues( kome::objects::Parameters* params )
			 * @brief gets setting parameter values
			 * @param[in] params parameters object
			 * @return setting parameter values
			 */
			static kome::objects::SettingParameterValues* getSettingValues( kome::objects::Parameters* params );

			/**
			 * @fn static kome::objects::Variant getActiveObject( kome::objects::Parameters* params )
			 * @brief gets active object
			 * @param[in] params parameters object
			 * @return active object
			 */
			static kome::objects::Variant getActiveObject( kome::objects::Parameters* params );

			/**
			 * @fn static kome::ident::SampleGetter* getSampleGetter( kome::objects::Parameters* params )
			 * @brief gets sample getter
			 * @param[in] params parameters object
			 * @return sample getter object
			 */
			static kome::ident::SampleGetter* getSampleGetter( kome::objects::Parameters* params );

		public:
			/**
			 * @fn static void setBool( kome::objects::Parameters& params, const char* name, const bool val )
			 * @brief sets boolean value
			 * @param[out] params parameters object
			 * @param[in] name parameter name
			 * @param[in] val parameter value
			 */
			static void setBool( kome::objects::Parameters& params, const char* name, const bool val );

			/**
			 * @fn static bool getBool( kome::objects::Parameters* params, const char* name, const bool df )
			 * @brief gets boolean value
			 * @param[in] params parameters object
			 * @param[in] name parameter name
			 * @param[in] df default value
			 */
			static bool getBool( kome::objects::Parameters* params, const char* name, const bool df );

			/**
			 * @fn static void setInt( kome::objects::Parameters& params, const char* name, const int val )
			 * @brief sets integer value
			 * @param[out] params parameters object
			 * @param[in] name parameter name
			 * @param[in] val parameter value
			 */
			static void setInt( kome::objects::Parameters& params, const char* name, const int val );

			/**
			 * @fn static int getInt( kome::objects::Parameters* params, const char* name, const int df )
			 * @brief gets integer value
			 * @param[in] params parameters object
			 * @param[in] name parameter name
			 * @param[in] df default value
			 */
			static int getInt( kome::objects::Parameters* params, const char* name, const int df );

			/**
			 * @fn static void setDouble( kome::objects::Parameters& params, const char* name, const double val )
			 * @brief sets double value
			 * @param[out] params parameters object
			 * @param[in] name parameter name
			 * @param[in] val parameter value
			 */
			static void setDouble( kome::objects::Parameters& params, const char* name, const double val );

			/**
			 * @fn static double getDouble( kome::objects::Parameters* params, const char* name, const double df )
			 * @brief gets double value
			 * @param[in] params parameters object
			 * @param[in] name parameter name
			 * @param[in] df default value
			 */
			static double getDouble( kome::objects::Parameters* params, const char* name, const double df );

			/**
			 * @fn static void setString( kome::objects::Parameters& params, const char* name, const char* val )
			 * @brief sets string value
			 * @param[out] params parameters object
			 * @param[in] name parameter name
			 * @param[in] val parameter value
			 */
			static void setString( kome::objects::Parameters& params, const char* name, const char* val );

			/**
			 * @fn static const char* getString( kome::objects::Parameters* params, const char* name, const char* df )
			 * @brief gets string value
			 * @param[in] params parameters object
			 * @param[in] name parameter name
			 * @param[in] df default value
			 * @return string value (If specified parameter is not found, returns default value.)
			 */
			static const char* getString( kome::objects::Parameters* params, const char* name, const char* df );


			/**
			 * @fn static void setSearchID( kome::objects::Parameters& params, int nSearchID )
			 * @brief sets SearchID value
			 * @param[out] params parameters object
			 * @param[in] nSearchID Search ID value
			 */
			static void setSearchID( kome::objects::Parameters& params, int nSearchID );

			/**
			 * @fn static int getSearchID( kome::objects::Parameters* params )
			 * @brief gets Search ID
			 * @param[in] params parameters object
			 * @return Search ID (If specified parameter is not found, returns default value.)
			 */
			static int getSearchID( kome::objects::Parameters* params );

			/**
			 * @fn static void setSearchSpectrumID( kome::objects::Parameters& params, int nSearchSpectrumID )
			 * @brief sets SearchSpectrumID value
			 * @param[out] params parameters object
			 * @param[in] nSearchSpectrumID Search Spectrum ID value
			 */
			static void setSearchSpectrumID( kome::objects::Parameters& params, int nSearchSpectrumID );

			/**
			 * @fn static int getSearchSpectrumID( kome::objects::Parameters* params )
			 * @brief gets Search Spectrum ID
			 * @param[in] params parameters object
			 * @return Search Spectrum ID (If specified parameter is not found, returns default value.)
			 */
			static int getSearchSpectrumID( kome::objects::Parameters* params );

			/**
			 * @fn static void setSubstanceID( kome::objects::Parameters& params, int nSubstanceID )
			 * @brief sets SubstanceID value
			 * @param[out] params parameters object
			 * @param[in] nSubstanceID Substance ID value
			 */
			static void setSubstanceID( kome::objects::Parameters& params, int nSubstanceID );

			/**
			 * @fn static int getSubstanceID( kome::objects::Parameters* params )
			 * @brief gets Substance ID
			 * @param[in] params parameters object
			 * @return SubstanceID (If specified parameter is not found, returns default value.)
			 */
			static int getSubstanceID( kome::objects::Parameters* params );

		};
	}	
}


#endif		// __KOME_PLUGIN_CALL_TOOL_H__
