/**
 * @file ClrPluginCallTool.h
 * @brief interfaces ClrPluginCallTool class
 *
 * @author S.Tanaka
 * @date 2007.05.22
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_PLUGIN_CALL_TOOL_H__
#define __KOME_CLR_PLUGIN_CALL_TOOL_H__


namespace kome {
	namespace clr {

		/**
		 * @class ClrPluginCallTool
		 * @brief PluginCallTool wrapper class to use on CLR environment
		 */
		public ref class ClrPluginCallTool {
		public:
			/**
			 * @fn static void openFile( System::String^ path )
			 * @brief opens raw data file
			 * @param[in] path raw data file path
			 */
			static void openFile( System::String^ path );

			/**
			 * @fn static void onSaveSample( SampleWrapper^ sample )
			 * @brief broadcasts the sample is saved
			 * @param[in] saved sample
			 */
			static void onSaveSample( SampleWrapper^ sample );

			/**
			 * @fn static void openSpectrum( SpectrumWrapper^ spec )
			 * @brief opens spectrum
			 * @param[in] spec spectrum to be opened
			 */
			static void openSpectrum( SpectrumWrapper^ spec );

			/**
			 * @fn static void openChromatogram( ChromatogramWrapper^ chrom )
			 * @brief opens chromatogram
			 * @param[in] chrom chromatogram to be opened
			 */
			static void openChromatogram( ChromatogramWrapper^ chrom );

			/**
			 * @fn static void openDataMap( DataMapInfoWrapper^ dataMap )
			 * @brief opens data map
			 * @param[in] dataMap data map to be opened
			 */
			static void openDataMap( DataMapInfoWrapper^ dataMap );

			/**
			 * @fn static void onOpenSampleSet( SampleSetWrapper^ sampleSet )
			 * @brief broadcasts opening sample set
			 * @param[in] sampleSet opened sample set
			 */
			static void onOpenSampleSet( SampleSetWrapper^ sampleSet );

			/**
			 * @fn static void onOpenSample( SampleWrapper^ sample )
			 * @brief broadcasts opening sample
			 * @param[in] sample opened sample
			 */
			static void onOpenSample( SampleWrapper^ sample );

			/**
			 * @fn static void onCloseSample( SampleWrapper^ sample )
			 * @brief broadcasts closing sample
			 * @param[in] sample closed sample
			 */
			static void onCloseSample( SampleWrapper^ sample );

			/**
			 * @fn static bool canExit()
			 * @brief broadcasts Mass++ wants to exit
			 */
			static bool canExit();

			/**
			 * @fn static void onUpdateSpectrum( SpectrumWrapper^ spec )
			 * @brief broadcasts a spectrum is updated
			 * @param[in] spec updated spectrum
			 */
			static void onUpdateSpectrum( SpectrumWrapper^ spec );

			/**
			 * @fn static void onUpdateChromatogram( ChromatogramWrapper^ chrom )
			 * @brief broadcasts a chromatogram is updated
			 * @param[in] chrom updated chromatogram
			 */
			static void onUpdateChromatogram( ChromatogramWrapper^ chrom );

			/**
			 * @fn static void onUpdateDataMap( DataMapInfoWrapper^ dataMap )
			 * @brief broadcasts a data map is updated
			 * @param[in] dataMap updated data map
			 */
			static void onUpdateDataMap( DataMapInfoWrapper^ dataMap );

			/**
			 * @fn static void onUpdateDataSet( DataSetWrapper^ dataSet )
			 * @brief broadcasts a data set is updated
			 * @param[in] dataSet updated data set
			 */
			static void onUpdateDataSet( DataSetWrapper^ dataSet );

			/**
			 * @fn static void onUpdateGroup( DataGroupNodeWrapper^ group )
			 * @brief broadcasts a group is updated
			 * @param[in] group updated group
			 */
			static void onUpdateGroup( DataGroupNodeWrapper^ group );

			/**
			 * @fn static void onUpdateSample( SampleWrapper^ sample )
			 * @brief broadcasts a sample is updated
			 * @param[in] sample updated sample
			 */
			static void onUpdateSample( SampleWrapper^ sample );


			/**
			 * @fn static void detectSpecPeaks( DataSetWrapper^ dataSet, ProgressWrapper^ progress )
			 * @brief spectrum peak detection
			 * @param[in] dataSet spectra to detect peaks
			 * @param[in] progress the object to show the progress
			 */
			static void detectSpecPeaks( DataSetWrapper^ dataSet, ProgressWrapper^ progress );

			/**
			 * @fn static void onUpdateSpecPeaks( SpectrumWrapper^ spec )
			 * @brief broadcasts spectrum peaks are updated
			 * @param[in] spec updated spectrum
			 */
			static void onUpdateSpecPeaks( SpectrumWrapper^ spec );

			/**
			 * @fn static void detectChromPeaks( DataSetWrapper^ dataSet, ProgressWrapper^ progress )
			 * @brief chromatogram peak detection
			 * @param[in] dataSet the object containing chromatograms fot peak detection
			 * @param[in] progress the object to show the progress
			 */
			static void detectChromPeaks( DataSetWrapper^ dataSet, ProgressWrapper^ progress );

			/**
			 * @fn static void onUpdateChromPeaks( ChromatogramWrapper^ chrom )
			 * @brief broadcasts chromatogram peaks are updated
			 * @param[in] chrom updated chromatogram
			 */
			static void onUpdateChromPeaks( ChromatogramWrapper^ chrom );

			/**
			 * @fn static void detect2DPeaks( DataGroupNodeWrapper^ group, ProgressWrapper^ progress )
			 * @brief detects 2D peaks
			 * @param[in] group the group to detect 2D peaks
			 * @param[in] progress the object to show the progress
			 */
			static void detect2DPeaks( DataGroupNodeWrapper^ group, ProgressWrapper^ progress );

			/**
			 * @fn static void onUpdate2DPeaks( DataGroupNodeWrapper^ group )
			 * @brief broadcasts 2D peaks are updated
			 * @param[in] group spectrum group
			 */
			static void onUpdate2DPeaks( DataGroupNodeWrapper^ group );

			/**
			 * @fn static void onUpdatePeaksMatrix()
			 * @brief broadcasts peaks matrix is updated
			 */
			static void onUpdatePeaksMatrix();

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
						SpectrumWrapper^ spec,
						XYDataWrapper^ xyData,
						PeaksWrapper^ src,
						PeaksWrapper^ dst
					)
			 * @brief broadcasts peaks are commited
			 * @param[in] spec spectrum 
			 * @param[in] xyData data points
			 * @param[in] src peaks before commiting
			 * @param[in] dst updated peaks
			 */
			static void onCommitSpecPeaks(
				SpectrumWrapper^ spec,
				XYDataWrapper^ xyData,
				PeaksWrapper^ src,
				PeaksWrapper^ dst
			);

			/**
			 * @fn static void onCommitChromPeaks(
						ChromatogramWrapper^ chrom,
						XYDataWrapper^ xyData,
						PeaksWrapper^ src,
						PeaksWrapper^ dst
					)
			 * @brief broadcasts peaks are commited
			 * @param[in] chrom chromatogram
			 * @param[in] xyData data points
			 * @param[in] src peaks before commiting
			 * @param[in] dst updated peaks
			 */
			static void onCommitChromPeaks(
				ChromatogramWrapper^ chrom,
				XYDataWrapper^ xyData,
				PeaksWrapper^ src,
				PeaksWrapper^ dst
			);

			/**
			 * @fn static void onRollbackSpecPeaks(
						SpectrumWrapper^ spec,
						XYDataWrapper^ xyData,
						PeaksWrapper^ peaks,
						PeaksWrapper^ canceledPeaks
					)
			 * @brief broadcasts peaks are rollbacked
			 * @param[in] spec spectrum 
			 * @param[in] xyData data points
			 * @param[in] peaks peaks
			 * @param[in] canceledPeaks canceledPeaks
			 */
			static void onRollbackSpecPeaks(
				SpectrumWrapper^ spec,
				XYDataWrapper^ xyData,
				PeaksWrapper^ peaks,
				PeaksWrapper^ canceledPeaks
			);

			/**
			 * @fn static void onRollbackChromPeaks(
						ChromatogramWrapper^ chrom,
						XYDataWrapper^ xyData,
						PeaksWrapper^ peaks,
						PeaksWrapper^ canceledPeaks
					)
			 * @brief broadcasts peaks are rollbacked
			 * @param[in] chrom chromatogram
			 * @param[in] xyData data points
			 * @param[in] peaks peaks
			 * @param[in] canceledPeaks canceled peaks
			 */
			static void onRollbackChromPeaks(
				ChromatogramWrapper^ chrom,
				XYDataWrapper^ xyData,
				PeaksWrapper^ peaks,
				PeaksWrapper^ canceledPeaks
			);


			/**
			 * @fn static void commitSpecPeaks( SpectrumWrapper^ spec )
			 * @brief commits spectrum peaks
			 * @param[in] spec spectrum
			 */
			static void commitSpecPeaks( SpectrumWrapper^ spec );

			/**
			 * @fn static void commitChromPeaks( ChromatogramWrapper^ chrom )
			 * @brief commits chromatogram peaks
			 * @param[in] chrom chromatogram
			 */
			static void commitChromPeaks( ChromatogramWrapper^ chrom );

			/**
			 * @fn static void rollbackSpecPeaks( SpectrumWrapper^ spec )
			 * @brief rollbacks spectrum peaks
			 * @param[in] spec spectrum
			 */
			static void rollbackSpecPeaks( SpectrumWrapper^ spec );

			/**
			 * @fn static void rollbackChromPeaks( ChromatogramWrapper^ chrom )
			 * @brief rollbacks chromatogram peaks
			 * @param[in] chrom chromatogram
			 */
			static void rollbackChromPeaks( ChromatogramWrapper^ chrom ); 

			/**
			 * @fn static bool onOpenSpectrum( SpectrumWrapper^ spec )
			 * @brief broadcasts a spectrum is open
			 * @param[in] spec opened spectrum
			 * @return If false, it is impossible to open spectrum.
			 */
			static bool onOpenSpectrum( SpectrumWrapper^ spec );

			/**
			 * @fn static bool onOpenChromatogram( ChromatogramWrapper^ chrom )
			 * @brief broadcasts a chromatogram is opened
			 * @param[in] chrom opened chromatogram
			 * @return If false, it is impossible to open chromatogram
			 */
			static bool onOpenChromatogram( ChromatogramWrapper^ chrom );

			/**
			 * @fn static bool onOpenDataMap( DataMapInfoWrapper^ dataMap )
			 * @brief broadcasts a data map is opened
			 * @param[in] dataMap opened data map
			 * @return If false, it is impossible to open data map
			 */
			static bool onOpenDataMap( DataMapInfoWrapper^ dataMap );

			/**
			 * @fn static void onCloseSpectrum( SpectrumWrapper^ spec )
			 * @brief broadcasts a spectrum is close
			 * @param[in] spec closed spectrum
			 */
			static void onCloseSpectrum( SpectrumWrapper^ spec );

			/**
			 * @fn static void onCloseChromatogram( ChromatogramWrapper^ chrom )
			 * @brief broadcasts a chromatogram is closed
			 * @param[in] chrom closed chromatogram
			 */
			static void onCloseChromatogram( ChromatogramWrapper^ chrom );

			/**
			 * @fn static void onCloseDataMap( DataMapInfoWrapper^ dataMap )
			 * @brief broadcasts a data map is closed
			 * @param[in] dataMap closed data map
			 */
			static void onCloseDataMap( DataMapInfoWrapper^ dataMap );

			/**
			 * @fn static void onActivate( ClrVariant^ obj )
			 * @brief broadcasts active object is changed
			 * @param[in] active object
			 */
			static void onActivate( ClrVariant^ obj );

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
					SpectrumWrapper^ spec,
					double start,
					double end,
					long long canvas
				)
			 * @brief changes spectrum range
			 * @param[in] spec spectrum
			 * @param[in] start the start of m/z range
			 * @param[in] end the end of m/z range
			 * @param[in] canvas object
			 */
			static void changeSpectrumRange(
				SpectrumWrapper^ spec,
				double start,
				double end,
				long long canvas
			);

			/**
			 * @fn static void changeChromatogramRange(
					ChromatogramWrapper^ chrom,
					double start,
					double end,
					long long canvas
				)
			 * @brief changes chromatogram range
			 * @param[in] chrom chromatogram
			 * @param[in] start the start of RT range
			 * @param[in] end the end of RT range
			 * @param[in] canvas object
			 */
			static void changeChromatogramRange(
				ChromatogramWrapper^ chrom,
				double start,
				double end,
				long long canvas
			);

			/**
			 * @fn static void actSpectrumRange(
					SpectrumWrapper^ spec,
					double start,
					double end
				)
			 * @brief acts spectrum range
			 * @param[in] spec spectrum
			 * @param[in] start the start of m/z range
			 * @param[in] end the end of m/z range
			 */
			static void actSpectrumRange(
				SpectrumWrapper^ spec,
				double start,
				double end
			);

			/**
			 * @fn static void actChromatogramRange(
					ChromatogramWrapper^ chrom,
					double start,
					double end,
					kome::core::Progress* progress = NULL
				)
			 * @brief acts chromatogram range
			 * @param[in] chrom chromatogram
			 * @param[in] start the start of RT range
			 * @param[in] end the end of RT range
			 * @param[in] progress the object to show the progress
			 */
			static void actChromatogramRange(
				ChromatogramWrapper^ chrom,
				double start,
				double end,
				kome::core::Progress* progress
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
						SpectrumWrapper^ spec,
						GraphicsWrapper^ g
					)
			 * @brief draws spectrum
			 * @param[in] spec spectrum object to draw
			 * @param[in] g graphics object to be drawed
			 */
			static void drawSpectrum(
				SpectrumWrapper^ spec,
				GraphicsWrapper^ g
			);

			/**
			 * @fn static void prepareDrawSpectrum(
						SpectrumWrapper^ spec,
						GraphicsWrapper^ g
					)
			 * @brief prepares to draw spectrum
			 * @param[in] spec spectrum object to draw
			 * @param[in] g graphics object to be drawed
			 */
			static void prepareDrawSpectrum(
				SpectrumWrapper^ spec,
				GraphicsWrapper^ g
			);

			/**
			 * @fn static void drawSpectrumFg(
						SpectrumWrapper^ spec,
						GraphicsWrapper^ g
					)
			 * @brief draws spectrum foreground
			 * @param[in] spec spectrum object to draw
			 * @param[in] g graphics object to be drawed
			 */
			static void drawSpectrumFg(
				SpectrumWrapper^ spec,
				GraphicsWrapper^ g
			);

			/**
			 * @fn static void drawSpectrumBg(
						SpectrumWrapper^ spec,
						GraphicsWrapper^ g
					)
			 * @brief draws spectrum background
			 * @param[in] spec spectrum object to draw
			 * @param[in] g graphics object to be drawed
			 */
			static void drawSpectrumBg(
				SpectrumWrapper^ spec,
				GraphicsWrapper^ g
			);

			/**
			 * @fn static void drawSpectrumWindowFg(
						SpectrumWrapper^ spec,
						GraphicsWrapper^ g
					)
			 * @brief draws spectrum canvas foreground
			 * @param[in] spec spectrum object to draw
			 * @param[in] g graphics object to be drawed
			 */
			static void drawSpectrumWindowFg(
				SpectrumWrapper^ spec,
				GraphicsWrapper^ g
			);

			/**
			 * @fn static void drawSpectrumWindowBg(
						SpectrumWrapper^ spec,
						GraphicsWrapper^ g
					)
			 * @brief draws spectrum canvas background
			 * @param[in] spec spectrum object to draw
			 * @param[in] g graphics object to be drawed
			 */
			static void drawSpectrumWindowBg(
				SpectrumWrapper^ spec,
				GraphicsWrapper^ g
			);

			/**
			 * @fn static void drawChromatogram(
						ChromatogramWrapper^ chrom,
						GraphicsWrapper^ g
					)
			 * @brief draws chromatogram
			 * @param[in] chrom chromatogram object to draw
			 * @param[in] g graphics object to be drawed
			 */
			static void drawChromatogram(
				ChromatogramWrapper^ chrom,
				GraphicsWrapper^ g
			);

			/**
			 * @fn static void prepareDrawChromatogram(
						ChromatogramWrapper^ chrom,
						GraphicsWrapper^ g
					)
			 * @brief prepares to draw chromatogram
			 * @param[in] chrom chromatogram object to draw
			 * @param[in] g graphics object to be drawed
			 */
			static void prepareDrawChromatogram(
				ChromatogramWrapper^ chrom,
				GraphicsWrapper^ g
			);

			/**
			 * @fn static void drawChromatogramFg(
						ChromatogramWrapper^ chrom,
						GraphicsWrapper^ g
					)
			 * @brief draws chromatogram foreground
			 * @param[in] chrom chromatogram object to draw
			 * @param[in] g graphics object to be drawed
			 */
			static void drawChromatogramFg(
				ChromatogramWrapper^ chrom,
				GraphicsWrapper^ g
			);

			/**
			 * @fn static void drawChromatogramBg(
						ChromatogramWrapper^ chrom,
						GraphicsWrapper^ g
					) 
			 * @brief draws chromatogram background
			 * @param[in] chrom chromatogram object to draw
			 * @param[in] g graphics object to be drawed
			 */
			static void drawChromatogramBg(
				ChromatogramWrapper^ chrom,
				GraphicsWrapper^ g
			);

			/**
			 * @fn static void drawChromatogramWindowFg(
						ChromatogramWrapper^ chrom,
						GraphicsWrapper^ g
					)
			 * @brief draws chromatogram canvas foreground
			 * @param[in] chrom chromatogram object to draw
			 * @param[in] g graphics object to be drawed
			 */
			static void drawChromatogramWindowFg(
				ChromatogramWrapper^ chrom,
				GraphicsWrapper^ g
			);

			/**
			 * @fn static void drawChromatogramWindowBg(
						ChromatogramWrapper^ chrom,
						GraphicsWrapper^ g
					)
			 * @brief draws chromatogram canvas background
			 * @param[in] chrom chromatogram object to draw
			 * @param[in] g graphics object to be drawed
			 */
			static void drawChromatogramWindowBg(
				ChromatogramWrapper^ chrom,
				GraphicsWrapper^ g
			);

			/**
			 * @fn static void drawDataMap(
						DataMapInfoWrapper^ dataMap,
						GraphicsWrapper^ g
					)
			 * @brief draws data map
			 * @param[in] dataMap data map object to draw
			 * @param[in] g graphics object to be drawed
			 */
			static void drawDataMap(
				DataMapInfoWrapper^ dataMap,
				GraphicsWrapper^ g
			);

			/**
			 * @fn static void prepareDrawDataMap(
						DataMapInfoWrapper^ dataMap,
						GraphicsWrapper^ g
					)
			 * @brief prepares to draw data map
			 * @param[in] dataMap data map object to draw
			 * @param[in] g graphics object to be drawed
			 */
			static void prepareDrawDataMap(
				DataMapInfoWrapper^ dataMap,
				GraphicsWrapper^ g
			);

			/**
			 * @fn static void drawDataMapFg(
						DataMapInfoWrapper^ dataMap,
						GraphicsWrapper^ g
					)
			 * @brief draws data map foreground
			 * @param[in] dataMap data map object to draw
			 * @param[in] g graphics object to be drawed
			 */
			static void drawDataMapFg(
				DataMapInfoWrapper^ dataMap,
				GraphicsWrapper^ g
			);

			/**
			 * @fn static void drawDataMapBg(
						DataMapInfoWrapper^ dataMap,
						GraphicsWrapper^ g
					)
			 * @brief draws data map background
			 * @param[in] dataMap data map object to draw
			 * @param[in] g graphics object to be drawed
			 */
			static void drawDataMapBg(
				DataMapInfoWrapper^ dataMap,
				GraphicsWrapper^ g
			);

			/**
			 * @fn static void drawDataMapWindowFg(
						DataMapInfoWrapper^ dataMap,
						GraphicsWrapper^ g
					)
			 * @brief draws data map canvas foreground
			 * @param[in] dataMap data map object to draw
			 * @param[in] g graphics object to be drawed
			 */
			static void drawDataMapWindowFg(
				DataMapInfoWrapper^ dataMap,
				GraphicsWrapper^ g
			);

			/**
			 * @fn static void drawDataMapWindowBg(
						DataMapInfoWrapper^ dataMap,
						GraphicsWrapper^ g
					)
			 * @brief draws data map canvas background
			 * @param[in] dataMap data map object to draw
			 * @param[in] g graphics object to be drawed
			 */
			static void drawDataMapWindowBg(
				DataMapInfoWrapper^ dataMap,
				GraphicsWrapper^ g
			);

		public:
			/**
			 * @fn static bool onSpectrumButtonDown(
					SpectrumWrapper^ spec,
					GraphicsWrapper^ g,
					MouseEventWrapper^ evt
				)
			 * @brief broadcasts mouse button is down on spectrum window
			 * @param[in] spec spectrum object
			 * @param[in] g canvas graphics object
			 * @param[in] evt mouse event information
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onSpectrumButtonDown(
				SpectrumWrapper^ spec,
				GraphicsWrapper^ g,
				MouseEventWrapper^ evt
			);

			/**
			 * @fn static bool onSpectrumButtonUp(
					SpectrumWrapper^ spec,
					GraphicsWrapper^ g,
					MouseEventWrapper^ evt
				)
			 * @brief broadcasts mouse button is up on spectrum window
			 * @param[in] spec spectrum object
			 * @param[in] g canvas graphics object
			 * @param[in] evt mouse event information
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onSpectrumButtonUp(
				SpectrumWrapper^ spec,
				GraphicsWrapper^ g,
				MouseEventWrapper^ evt
			);

			/**
			 * @fn static bool onSpectrumDoubleClick(
					SpectrumWrapper^ spec,
					GraphicsWrapper^ g,
					MouseEventWrapper^ evt
				)
			 * @brief broadcasts mouse button is double clicked on spectrum window
			 * @param[in] spec spectrum object
			 * @param[in] g canvas graphics object
			 * @param[in] evt mouse event information
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onSpectrumDoubleClick(
				SpectrumWrapper^ spec,
				GraphicsWrapper^ g,
				MouseEventWrapper^ evt
			);

			/**
			 * @fn static bool onSpectrumMouseMove(
					SpectrumWrapper^ spec,
					GraphicsWrapper^ g,
					MouseEventWrapper^ evt
				)
			 * @brief broadcasts mouse cursor moves on spectrum window
			 * @param[in] spec spectrum object
			 * @param[in] g canvas graphics object
			 * @param[in] evt mouse event information
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onSpectrumMouseMove(
				SpectrumWrapper^ spec,
				GraphicsWrapper^ g,
				MouseEventWrapper^ evt
			);

			/**
			 * @fn static bool onSpectrumMouseLeave(
					SpectrumWrapper^ spec,
					GraphicsWrapper^ g,
					MouseEventWrapper^ evt
				)
			 * @brief broadcasts mouse cursor leaves from spectrum window
			 * @param[in] spec spectrum object
			 * @param[in] g canvas graphics object
			 * @param[in] evt mouse event information
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onSpectrumMouseLeave(
				SpectrumWrapper^ spec,
				GraphicsWrapper^ g,
				MouseEventWrapper^ evt
			);

			/**
			 * @fn static bool onSpectrumMouseEnter(
					SpectrumWrapper^ spec,
					GraphicsWrapper^ g,
					MouseEventWrapper^ evt
				)
			 * @brief broad casts mouse cursor enters into spectrum window
			 * @param[in] spec spectrum object
			 * @param[in] g canvas graphics object
			 * @param[in] evt mouse event information
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onSpectrumMouseEnter(
				SpectrumWrapper^ spec,
				GraphicsWrapper^ g,
				MouseEventWrapper^ evt
			);

			/**
			 * @fn static bool onChromatogramButtonDown(
					ChromatogramWrapper^ chrom,
					GraphicsWrapper^ g,
					MouseEventWrapper^ evt
				)
			 * @brief broadcasts mouse button is down on chromatogram window
			 * @param[in] chrom chromatogram object
			 * @param[in] g canvas graphics object
			 * @param[in] evt mouse event information
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onChromatogramButtonDown(
				ChromatogramWrapper^ chrom,
				GraphicsWrapper^ g,
				MouseEventWrapper^ evt
			);

			/**
			 * @fn static bool onChromatogramButtonUp(
					ChromatogramWrapper^ chrom,
					GraphicsWrapper^ g,
					MouseEventWrapper^ evt
				)
			 * @brief broadcasts mouse button is up on chromatogram window
			 * @param[in] chrom chromatogram object
			 * @param[in] g canvas graphics object
			 * @param[in] evt mouse event information
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onChromatogramButtonUp(
				ChromatogramWrapper^ chrom,
				GraphicsWrapper^ g,
				MouseEventWrapper^ evt
			);

			/**
			 * @fn static bool onChromatogramDoubleClick(
					ChromatogramWrapper^ chrom,
					GraphicsWrapper^ g,
					MouseEventWrapper^ evt
				)
			 * @brief broadcasts mouse button is double clicked on chromatogram window
			 * @param[in] chrom chromatogram object
			 * @param[in] g canvas graphics object
			 * @param[in] evt mouse event information
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onChromatogramDoubleClick(
				ChromatogramWrapper^ chrom,
				GraphicsWrapper^ g,
				MouseEventWrapper^ evt
			);

			/**
			 * @fn static bool onChromatogramMouseMove(
					ChromatogramWrapper^ chrom,
					GraphicsWrapper^ g,
					MouseEventWrapper^ evt
				)
			 * @brief broadcasts mouse cursor moves on chromatogram window
			 * @param[in] chrom chromatogram object
			 * @param[in] g canvas graphics object
			 * @param[in] evt mouse event information
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onChromatogramMouseMove(
				ChromatogramWrapper^ chrom,
				GraphicsWrapper^ g,
				MouseEventWrapper^ evt
			);

			/**
			 * @fn static bool onChromatogramMouseLeave(
					ChromatogramWrapper^ chrom,
					GraphicsWrapper^ g,
					MouseEventWrapper^ evt
				)
			 * @brief broadcasts mouse cursor leaves from chromatogram window
			 * @param[in] chrom chromatogram object
			 * @param[in] g canvas graphics object
			 * @param[in] evt mouse event information
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onChromatogramMouseLeave(
				ChromatogramWrapper^ chrom,
				GraphicsWrapper^ g,
				MouseEventWrapper^ evt
			);

			/**
			 * @fn static bool onChromatogramMouseEnter(
					ChromatogramWrapper^ chrom,
					GraphicsWrapper^ g,
					MouseEventWrapper^ evt
				)
			 * @brief broadcasts mouse cursor enters into chromatogram window
			 * @param[in] chrom chromatogram object
			 * @param[in] g canvas graphics object
			 * @param[in] evt mouse event information
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onChromatogramMouseEnter(
				ChromatogramWrapper^ chrom,
				GraphicsWrapper^ g,
				MouseEventWrapper^ evt
			);

			/**
			 * @fn static bool onDataMapButtonDown(
					DataMapInfoWrapper^ dataMap,
					GraphicsWrapper^ g,
					MouseEventWrapper^ evt
				)
			 * @brief broadcasts mouse button is down on data map window
			 * @param[in] dataMap data map object
			 * @param[in] g canvas graphics object
			 * @param[in] evt mouse event information
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onDataMapButtonDown(
				DataMapInfoWrapper^ dataMap,
				GraphicsWrapper^ g,
				MouseEventWrapper^ evt
			);

			/**
			 * @fn static bool onDataMapButtonUp(
					DataMapInfoWrapper^ dataMap,
					GraphicsWrapper^ g,
					MouseEventWrapper^ evt
				)
			 * @brief broadcasts mouse button is up on data map window
			 * @param[in] dataMap data map object
			 * @param[in] g canvas graphics object
			 * @param[in] evt mouse event information
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onDataMapButtonUp(
				DataMapInfoWrapper^ dataMap,
				GraphicsWrapper^ g,
				MouseEventWrapper^ evt
			);

			/**
			 * @fn static bool onDataMapDoubleClick(
					DataMapInfoWrapper^ dataMap,
					GraphicsWrapper^ g,
					MouseEventWrapper^ evt
				)
			 * @brief broadcasts mouse button is double clicked on data map window
			 * @param[in] dataMap data map object
			 * @param[in] g canvas graphics object
			 * @param[in] evt mouse event information
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onDataMapDoubleClick(
				DataMapInfoWrapper^ dataMap,
				GraphicsWrapper^ g,
				MouseEventWrapper^ evt
			);

			/**
			 * @fn static bool onDataMapMouseMove(
					DataMapInfoWrapper^ dataMap,
					GraphicsWrapper^ g,
					MouseEventWrapper^ evt
				)
			 * @brief broadcasts mouse cursor moves on data map window
			 * @param[in] dataMap data map object
			 * @param[in] g canvas graphics object
			 * @param[in] evt mouse event information
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onDataMapMouseMove(
				DataMapInfoWrapper^ dataMap,
				GraphicsWrapper^ g,
				MouseEventWrapper^ evt
			);

			/**
			 * @fn static bool onDataMapMouseLeave(
					DataMapInfoWrapper^ dataMap,
					GraphicsWrapper^ g,
					MouseEventWrapper^ evt
				)
			 * @brief broadcasts mouse cursor leaves from data map window
			 * @param[in] dataMap data map object
			 * @param[in] g canvas graphics object
			 * @param[in] evt mouse event information
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onDataMapMouseLeave(
				DataMapInfoWrapper^ dataMap,
				GraphicsWrapper^ g,
				MouseEventWrapper^ evt
			);

			/**
			 * @fn static bool onDataMapMouseEnter(
					DataMapInfoWrapper^ dataMap,
					GraphicsWrapper^ g,
					MouseEventWrapper^ evt
				)
			 * @brief broad casts mouse cursor enters into data map window
			 * @param[in] dataMap data map object
			 * @param[in] g canvas graphics object
			 * @param[in] evt mouse event information
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onDataMapMouseEnter(
				DataMapInfoWrapper^ dataMap,
				GraphicsWrapper^ g,
				MouseEventWrapper^ evt
			);

		public:
			/**
			 * @fn static bool onSpectrumKeyDown(
						SpectrumWrapper^ spec,
						GraphicsWrapper^ g,
						KeyEventWrapper^ evt
					)
			 * @brief broadcasts key is down on spectrum window
			 * @param[in] spec spectrum object
			 * @param[in] g canvas graphics object
			 * @param[in] evt key event information
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onSpectrumKeyDown(
				SpectrumWrapper^ spec,
				GraphicsWrapper^ g,
				KeyEventWrapper^ evt
			);

			/**
			 * @fn static bool onSpectrumKeyUp(
						SpectrumWrapper^ spec,
						GraphicsWrapper^ g,
						KeyEventWrapper^ evt
					)
			 * @brief broadcasts key is up on spectrum window
			 * @param[in] spec spectrum object
			 * @param[in] g canvas graphics object
			 * @param[in] evt key event information
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onSpectrumKeyUp(
				SpectrumWrapper^ spec,
				GraphicsWrapper^ g,
				KeyEventWrapper^ evt
			);

			/**
			 * @fn static bool onChromatogramKeyDown(
						ChromatogramWrapper^ chrom,
						GraphicsWrapper^ g,
						KeyEventWrapper^ evt
					)
			 * @brief broadcasts key is down on chromatogram window
			 * @param[in] chrom chromatogram object
			 * @param[in] g canvas graphics object
			 * @param[in] evt key event information
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onChromatogramKeyDown(
				ChromatogramWrapper^ chrom,
				GraphicsWrapper^ g,
				KeyEventWrapper^ evt
			);

			/**
			 * @fn static bool onChromatogramKeyUp(
						ChromatogramWrapper^ chrom,
						GraphicsWrapper^ g,
						KeyEventWrapper^ evt
					)
			 * @brief broadcasts key is up on chromatogram window
			 * @param[in] chrom chromatogram object
			 * @param[in] g canvas graphics object
			 * @param[in] evt key event information
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onChromatogramKeyUp(
				ChromatogramWrapper^ chrom,
				GraphicsWrapper^ g,
				KeyEventWrapper^ evt
			);

			/**
			 * @fn static bool onDataMapKeyDown(
						DataMapInfoWrapper^ dataMap,
						GraphicsWrapper^ g,
						KeyEventWrapper^ evt,
					)
			 * @brief broadcasts key is down on data map window
			 * @param[in] dataMap data map object
			 * @param[in] g canvas graphics object
			 * @param[in] evt key event information
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onDataMapKeyDown(
				DataMapInfoWrapper^ dataMap,
				GraphicsWrapper^ g,
				KeyEventWrapper^ evt
			);

			/**
			 * @fn static bool onDataMapKeyUp(
						DataMapInfoWrapper^ dataMap,
						GraphicsWrapper^ g,
						KeyEventWrapper^ evt
					)
			 * @brief broadcasts key is up on data map window
			 * @param[in] dataMap data map object
			 * @param[in] g canvas graphics object
			 * @param[in] evt key event information
			 * @return If true, it has to refresh the canvas.
			 */
			static bool onDataMapKeyUp(
				DataMapInfoWrapper^ dataMap,
				GraphicsWrapper^ g,
				KeyEventWrapper^ evt
			);

		public:
			/**
			 * @fn static void setSampleSet( ClrParameters^ params, SampleSetWrapper^ sampleSet )
			 * @brief sets sample set to parameters object
			 * @param[out] params parametes object
			 * @param[in] sampleSet sample set object
			 */
			static void setSampleSet( ClrParameters^ params, SampleSetWrapper^ sampleSet );

			/**
			 * @fn static void setSample( ClrParameters^ params, SampleWrapper^ sample )
			 * @brief sets sample to parameters object
			 * @param[out] params parameters object
			 * @param[in] sample sample object
			 */
			static void setSample( ClrParameters^ params, SampleWrapper^ sample );

			/**
			 * @fn static void setSpectrum( ClrParameters^ params, SpectrumWrapper^ spec )
			 * @brief sets spectrum information to parameters object
			 * @param[out] params parameters object
			 * @param[in] spec spectrum object
			 */
			static void setSpectrum( ClrParameters^ params, SpectrumWrapper^ spec );

			/**
			 * @fn static void setChromatogram( ClrParameters^ params, ChromatogramWrapper^ chrom )
			 * @brief sets chromatogram information to parameters object
			 * @param[out] params parameters object
			 * @param[in] chrom chromatogram object
			 */
			static void setChromatogram( ClrParameters^ params, ChromatogramWrapper^ chrom );

			/**
			 * @fn static void setXYData( ClrParameters^ params, XYDataWrapper^ xyData )
			 * @brief sets xy data to parameters object
			 * @param[out] params parameters object
			 * @param[in] xyData xy data object
			 */
			static void setXYData( ClrParameters^ params, XYDataWrapper^ xyData );

			/**
			 * @fn static void setDataSet( ClrParameters^ params, DataSetWrapper^ dataSet )
			 * @brief sets data set to parameters object
			 * @param[out] params parameters object
			 * @param[in] dataSet data set object
			 */
			static void setDataSet( ClrParameters^ params, DataSetWrapper^ dataSet );

			/**
			 * @fn static void setGroup( ClrParameters^ params, DataGroupNodeWrapper^ group )
			 * @brief sets spectrum group to parameters object
			 * @param[out] params parameters object
			 * @param[in] group spectrum group object
			 */
			static void setGroup( ClrParameters^ params, DataGroupNodeWrapper^ group );

			/**
			 * @fn static void setPeaks( ClrParameters^ params, PeaksWrapper^ peaks )
			 * @brief sets peaks data to parameters object
			 * @param[out] params parameters object
			 * @param[in] peaks peaks object
			 */
			static void setPeaks( ClrParameters^ params, PeaksWrapper^ peaks );

			/**
			 * @fn static void setParentPeaks( ClrParameters^ params, PeaksWrapper^ parentPeaks )
			 * @brief sets parent peaks to parameters object
			 * @param[out] params parameters object
			 * @param[in] parentPeaks parent peaks object
			 */
			static void setParentPeaks( ClrParameters^ params, PeaksWrapper^ parentPeaks );

			/**
			 * @fn static void setPeaks2D( ClrParameters^ params, Peaks2DWrapper^ peaks )
			 * @brief sets 2D peaks data to parameters object
			 * @param[out] params parameters object
			 * @param[in] peaks peaks object
			 */
			static void setPeaks2D( ClrParameters^ params, Peaks2DWrapper^ peaks );

			/**
			 * @fn static void setBaseline( ClrParameters^ params, XYDataWrapper^ baseline )
			 * @brief sets baseline object to parameters object
			 * @param[out] params parameters object
			 * @param[in] baseline baseline object
			 */
			static void setBaseline( ClrParameters^ params, XYDataWrapper^ baseline );

			/**
			 * @fn static void setUpdatedXYData( ClrParameters^ params, XYDataWrapper^ updated )
			 * @brief sets updated xy data object to prameters object
			 * @param[out] params parameters object
			 * @param[in] updated updated xy data object
			 */
			static void setUpdatedXYData( ClrParameters^ params, XYDataWrapper^ updated );

			/**
			 * @fn static void setUpdatedPeaks( ClrParameters^ params, PeaksWrapper^ updated )
			 * @brief sets updated peaks object to parameters object
			 * @param[out] params paramters object
			 * @param[in] updated updated peaks object
			 */
			static void setUpdatedPeaks( ClrParameters^ params, PeaksWrapper^ updated );
			
			/**
			 * @fn static void setUpdatedPeaks2d( ClrParameters^ params, PeaksWrapper^ updated )
			 * @brief sets updated peaks object to parameters object
			 * @param[out] params paramters object
			 * @param[in] updated updated peaks object
			 */
			static void setUpdatedPeaks2d( ClrParameters^ params, PeaksWrapper^ updated );

			/**
			 * @fn static void setControlSpectrum( ClrParameters^ params, SpectrumWrapper^ spec )
			 * @brief sets control spectrum object to parametes object
			 * @param[out] params parameters object
			 * @param[in] spec control spectrum object
			 */
			static void setControlSpectrum( ClrParameters^ params, SpectrumWrapper^ spec );

			/**
			 * @fn static void setControlChromatogram( ClrParameters^ params, ChromatogramWrapper^ chrom )
			 * @brief sets control chromatogram object to parameters object
			 * @param[out] params parameters object
			 * @param[in] chrom control chromatogram object
			 */
			static void setControlChromatogram( ClrParameters^ params, ChromatogramWrapper^ chrom );

			/**
			 * @fn static void setControlGroup( ClrParameters^ params, DataGroupNodeWrapper^ group )
			 * @brief sets control data set object to parameters object
			 * @param[out] params parameters object
			 * @param[in] group group object
			 */
			static void setControlGroup( ClrParameters^ params, DataGroupNodeWrapper^ group );

			/**
			 * @fn static void setControlXYData( ClrParameters^ params, XYDataWrapper^ xyData )
			 * @brief sets control xy data object to parameters object
			 * @param[out] params parameters object
			 * @param[in] xyData control xy data object
			 */
			static void setControlXYData( ClrParameters^ params, XYDataWrapper^ xyData );

			/**
			 * @fn static void setPath( ClrParameters^ params, System::String^ path )
			 * @brief sets file path
			 * @param[out] params parameters object
			 * @param[in] path file path
			 */
			static void setPath( ClrParameters^ params, System::String^ path );

			/**
			 * @fn static void setStartOfRange( ClrParameters^ params, double start )
			 * @brief sets start of range to parameters object
			 * @param[out] params parameters object
			 * @param[in] start start of range
			 */
			static void setStartOfRange( ClrParameters^ params, double start );

			/**
			 * @fn static void setEndOfRange( ClrParameters^ params, double end )
			 * @brief sets end of range to parameters object
			 * @param[out] params parameters object
			 * @param[in] end end of range
			 */
			static void setEndOfRange( ClrParameters^ params, double end );

			/**
			 * @fn static void setRange( ClrParameters^ params, double start, double end )
			 * @brief sets range to parameters object
			 * @param[out] params parameters object
			 * @param[in] start start of range
			 * @param[in] end end of range
			 */
			static void setRange( ClrParameters^ params, double start, double end );

			/**
			 * @fn static void setProgress( ClrParameters^ params, ProgressWrapper^ progress )
			 * @brief sets progress to parameters object
			 * @param[out] params parameters object
			 * @param[in] progress progress object
			 */
			static void setProgress( ClrParameters^ params, ProgressWrapper^ progress );

			/**
			 * @fn static void setMouseEvent( ClrParameters^ params, MouseEventWrapper^ evt )
			 * @brief sets mouse event
			 * @param[out] params parameters object
			 * @param[in] evt mouse event object
			 */
			static void setMouseEvent( ClrParameters^ params, MouseEventWrapper^ evt );

			/**
			 * @fn static void setKeyEvent( ClrParameters^ params, KeyEventWrapper^ evt )
			 * @brief sets key event
			 * @param[out] params parameters object
			 * @param[in] evt key event object
			 */
			static void setKeyEvent( ClrParameters^ params, KeyEventWrapper^ evt );

			/**
			 * @fn static void setSizeEvent( ClrParameters^ params, SizeEventWrapper^ evt )
			 * @brief sets size event
			 * @param[out] params parameters object
			 * @param[in] evt size event object
			 */
			static void setSizeEvent( ClrParameters^ params, SizeEventWrapper^ evt );

			/**
			 * @fn static void setMoveEvent( ClrParameters^ params, MoveEventWrapper^ evt )
			 * @brief sets move event
			 * @param[out] params parameters object
			 * @param[in] evt move event object
			 */
			static void setMoveEvent( ClrParameters^ params, MoveEventWrapper^ evt );

			/**
			 * @fn static void setGraphics( ClrParameters^ params, GraphicsWrapper^ g )
			 * @brief sets graphics
			 * @param[out] params parameters object
			 * @param[in] g graphics object
			 */
			static void setGraphics( ClrParameters^ params, GraphicsWrapper^ g );

			/**
			 * @fn static void setDataMapInfo( ClrParameters^ params, DataMapInfoWrapper^ dataMap )
			 * @brief sets data map
			 * @param[out] params parameters object
			 * @param[in] dataMap data map object
			 */
			static void setDataMapInfo( ClrParameters^ params, DataMapInfoWrapper^ dataMap );

			/** 
			 * @fn static void setSettingValues( ClrParameters^ params, SettingParameterValuesWrapper^ vals )
			 * @brief sets setting parameter values
			 * @param[out] params parameters object
			 * @param[in] values setting parameter values object
			 */
			static void setSettingValues( ClrParameters^ params, SettingParameterValuesWrapper^ vals );

			/**
			 * @fn static void setActiveObject( ClrParameters^ params, ClrVariant^ obj )
			 * @brief sets active object
			 * @param[out] params parameters object
			 * @param[in] obj active object
			 */
			static void setActiveObject( ClrParameters^ params, ClrVariant^ obj );

		public:
			/**
			 * @fn static SampleSetWrapper^ getSampleSet( ClrParameters^ params )
			 * @brief gets sample set from parameters information
			 * @param[in] params parameter information
			 * @return sample set
			 */
			static SampleSetWrapper^ getSampleSet( ClrParameters^ params );

			/**
			 * @fn static SampleWrapper^ getSample( ClrParameters^ params )
			 * @brief get sample from parameters information
			 * @param[in] params parameters information
			 * @return sample
			 */
			static SampleWrapper^ getSample( ClrParameters^ params );

				/**
			 * @fn static SpectrumWrapper^ getSpectrum( ClrParameters^ params )
			 * @brief gets spectrum from parameters information
			 * @param[in] params parameters information
			 * @return spectrum
			 */
			static SpectrumWrapper^ getSpectrum( ClrParameters^ params );

			/**
			 * @fn static ChromatogramWrapper^ getChromatogram( ClrParameters^ params )
			 * @brief gets chromatogram from parameters information
			 * @param[in] params parameters information
			 * @return chromatogram
			 */
			static ChromatogramWrapper^ getChromatogram( ClrParameters^ params );

			/**
			 * @fn static XYDataWrapper^ getXYData( ClrParameters^ params )
			 * @brief gets xy data from parameters information
			 * @param[in] params parameters information
			 * @return xy data
			 */
			static XYDataWrapper^ getXYData( ClrParameters^ params );

			/**
			 * @fn static DataSetWrapper^ getDataSet( ClrParameters^ params )
			 * @brief gets data set from parameters information
			 * @param[in] params parameters information
			 * @return data set
			 */
			static DataSetWrapper^ getDataSet( ClrParameters^ params );

			/**
			 * @fn static DataGroupNodeWrapper^ getGroup( ClrParameters^ params )
			 * @brief gets spectrum group from parameters information
			 * @param[in] params parameters information
			 * @return spectrum group
			 */
			static DataGroupNodeWrapper^ getGroup( ClrParameters^ params );

			/** 
			 * @fn static PeaksWrapper^ getPeaks( ClrParameters^ params )
			 * @brief gets peaks from parameters information
			 * @param[in] params parameters information
			 * @return peaks
			 */
			static PeaksWrapper^ getPeaks( ClrParameters^ params );

			/**
			 * @fn static PeaksWrapper^ getParentPeaks( ClrParameters^ params )
			 * @brief gets parent peaks from parameters information
			 * @param[in] params parameters information
			 * @return parent peaks
			 */
			static PeaksWrapper^ getParentPeaks( ClrParameters^ params );

			/** 
			 * @fn static Peaks2DWrapper^ getPeaks2D( ClrParameters^ params )
			 * @brief gets 2D peaks from parameters information
			 * @param[in] params parameters information
			 * @return peaks
			 */
			static Peaks2DWrapper^ getPeaks2D( ClrParameters^ params );

			/**
			 * @fn static XYDataWrapper^ getBaseline( ClrParameters^ params )
			 * @brief gets baseline object from parameters information
			 * @param[in] params parameters information
			 * @return baseline object
			 */
			static XYDataWrapper^ getBaseline( ClrParameters^ params );

			/**
			 * @fn static XYDataWrapper^ getUpdatedXYData( ClrParameters^ params )
			 * @brief gets updated xy data object from parameters information
			 * @param[in] params parameters information
			 * @return updated xy data object
			 */
			static XYDataWrapper^ getUpdatedXYData( ClrParameters^ params );

			/**
			 * @fn static PeaksWrapper^ getUpdatedPeaks( ClrParameters^ params )
			 * @brief gets updated peaks
			 * @param[in] params parameters information
			 * @return updated peaks data object
			 */
			static PeaksWrapper^ getUpdatedPeaks( ClrParameters^ params );

			/**
			 * @fn static PeaksWrapper^ getUpdatedPeaks2d( ClrParameters^ params )
			 * @brief gets updated peaks 2d
			 * @param[in] params parameter information
			 * @return updated peaks data object
			 */
			static PeaksWrapper^ getUpdatedPeaks2d( ClrParameters^ params );

			/**
			 * @fn static SpectrumWrapper^ getControlSpectrum( ClrParameters^ params )
			 * @brief gets control spectrum
			 * @param[in] params paramete information
			 * @return control spectrum object
			 */
			static SpectrumWrapper^ getControlSpectrum( ClrParameters^ params );

			/**
			 * @fn static ChromatogramWrapper^ getControlChromatogram( ClrParameters^ params )
			 * @brief gets control chromatogram object
			 * @param[in] params parameters information
			 * @return control chromatogram object
			 */
			static ChromatogramWrapper^ getControlChromatogram( ClrParameters^ params );

			/**
			 * @fn static DataGroupNodeWrapper^ getControlGroup( ClrParameters^ params )
			 * @brief gets control data set object
			 * @param[in] params parameters information
			 * @return control group object
			 */
			static DataGroupNodeWrapper^ getControlGroup( ClrParameters^ params );

			/**
			 * @fn static XYDataWrapper^ getControlXYData( ClrParameters^ params )
			 * @brief gets control xy data object
			 * @param[in] params parameter information
			 * @return control xy data object
			 */
			static XYDataWrapper^ getControlXYData( ClrParameters^ params );

			/**
			 * @fn static System::String^ getPath( ClrParameters^ params )
			 * @brief gets file path from parameters information
			 * @param[in] params parameters information
			 * @return file path
			 */
			static System::String^ getPath( ClrParameters^ params );

			/**
			 * @fn static bool getStartOfRange( ClrParameters^ params, double% start )
			 * @brief gets start of range from parameters object
			 * @param[in] params parameters object
			 * @param[in] start the pointer to store start of range
			 * @return If true, it succeeded to get the start of range
			 */
			static bool getStartOfRange( ClrParameters^ params, double% start );

			/**
			 * @fn static bool getEndOfRange( ClrParameters^ params, double% end )
			 * @brief gets end of range from parameters object
			 * @param[in] params parameters object
			 * @param[in] end the pointer to store end of range
			 * @return If true, it succeeded to get the end of range
			 */
			static bool getEndOfRange( ClrParameters^ params, double% end );

			/**
			 * @fn static bool getRange( ClrParameters^ params, double% start, double% end )
			 * @brief gets range from parameters object
			 * @param[in] params parameters object
			 * @param[in] start the pointer to store start of range
			 * @param[in] end the pointer to store end of range
			 * @return If true, it succeeded to get the range
			 */
			static bool getRange( ClrParameters^ params, double% start, double% end );

			/**
			 * @fn static ProgressWrapper^ getProgress( ClrParameters^ params )
			 * @brief gets progress from parameters object
			 * @param[in] params parameters object
			 * @return progress object 
			 */
			static ProgressWrapper^ getProgress( ClrParameters^ params );

			/**
			 * @fn static MouseEventWrapper^ getMouseEvent( ClrParameters^ params )
			 * @brief gets mouse event
			 * @param[in] params parameters object
			 * @return mouse event object
			 */
			static MouseEventWrapper^ getMouseEvent( ClrParameters^ params );

			/**
			 * @fn static KeyEventWrapper^ getKeyEvent( ClrParameters^ params )
			 * @brief gets key event
			 * @param[in] params parameters object
			 * @return key event object
			 */
			static KeyEventWrapper^ getKeyEvent( ClrParameters^ params );

			/**
			 * @fn static SizeEventWrapper^ getSizeEvent( ClrParameters^ params )
			 * @brief gets size event
			 * @param[in] params parameters object
			 * @return size event object
			 */
			static SizeEventWrapper^ getSizeEvent( ClrParameters^ params );

			/**
			 * @fn static MoveEventWrapper^ getMoveEvent( ClrParameters^ params )
			 * @brief gets move event
			 * @param[in] params parameters object
			 * @return move event object
			 */
			static MoveEventWrapper^ getMoveEvent( ClrParameters^ params );

			/**
			 * @fn static GraphicsWrapper^ getGraphics( ClrParameters^ params )
			 * @brief gets graphics
			 * @param[in] params parameters object
			 * @return graphics
			 */
			static GraphicsWrapper^ getGraphics( ClrParameters^ params );

			/**
			 * @fn static DataMapInfoWrapper^ getDataMapInfo( ClrParameters^ params )
			 * @brief gets data map
			 * @param[in] params parameters object
			 * @return data map
			 */
			static DataMapInfoWrapper^ getDataMapInfo( ClrParameters^ params );

			/** 
			 * @fn static SettingParameterValuesWrapper^ getSettingValues( ClrParameters^ params )
			 * @brief gets setting parameter values
			 * @param[in] params parameters object
			 * @return setting parameter values
			 */
			static SettingParameterValuesWrapper^ getSettingValues( ClrParameters^ params );

			/**
			 * @fn static ClrVariant^ getActiveObject( ClrParameters^ params )
			 * @brief gets active object
			 * @param[in] params parameters object
			 * @return active object
			 */
			static ClrVariant^ getActiveObject( ClrParameters^ params );

		public:
			/**
			 * @fn static void setBool( ClrParameters^ params, System::String^ name, bool val )
			 * @brief sets boolean value
			 * @param[out] params parameters object
			 * @param[in] name parameter name
			 * @param[in] val parameter value
			 */
			static void setBool( ClrParameters^ params, System::String^ name, bool val );

			/**
			 * @fn static bool getBool( ClrParameters^ params, System::String^ name, bool df )
			 * @brief gets boolean value
			 * @param[in] params parameters object
			 * @param[in] name parameter name
			 * @param[in] df default value
			 */
			static bool getBool( ClrParameters^ params, System::String^ name, bool df );

			/**
			 * @fn static void setInt( ClrParameters^ params, System::String^ name, int val )
			 * @brief sets integer value
			 * @param[out] params parameters object
			 * @param[in] name parameter name
			 * @param[in] val parameter value
			 */
			static void setInt( ClrParameters^ params, System::String^ name, int val );

			/**
			 * @fn static int getInt( ClrParameters^ params, System::String^ name, int df )
			 * @brief gets integer value
			 * @param[in] params parameters object
			 * @param[in] name parameter name
			 * @param[in] df default value
			 */
			static int getInt( ClrParameters^ params, System::String^ name, int df );

			/**
			 * @fn static void setDouble( ClrParameters^ params, System::String^ name, double val )
			 * @brief sets double value
			 * @param[out] params parameters object
			 * @param[in] name parameter name
			 * @param[in] val parameter value
			 */
			static void setDouble( ClrParameters^ params, System::String^ name, double val );

			/**
			 * @fn static double getDouble( ClrParameters^ params, System::String^ name, double df )
			 * @brief gets double value
			 * @param[in] params parameters object
			 * @param[in] name parameter name
			 * @param[in] df default value
			 */
			static double getDouble( ClrParameters^ params, System::String^ name, double df );

			/**
			 * @fn static void setString( ClrParameters^ params, System::String^ name, System::String^ val )
			 * @param[out] sets string value
			 * @param[in] params parameters object
			 * @param[in] name parameter name
			 * @param[in] val parameter value
			 */
			static void setString( ClrParameters^ params, System::String^ name, System::String^ val );

			/**
			 * @fn static System::String^ getString( ClrParameters^ params, System::String^ name, System::String^ df )
			 * @brief gets string value
			 * @param[in] params parameters object
			 * @param[in] name parameter name
			 * @param[in] df default value
			 * @return string value (If specified parameter is not found, returns default value.)
			 */
			static System::String^ getString( ClrParameters^ params, System::String^ name, System::String^ df );
		};
	}
}


#endif		// __KOME_CLR_PLUGIN_CALL_TOOL_H__
