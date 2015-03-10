/**
 * @file PeaksIOManager.h
 * @brief interfaces of PeaksIOManager class
 *
 * @author S.Tanaka
 * @date 2011.02.14
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_PEAKS_PEAKS_IO_MANAGER_H__
#define __KOME_IO_PEAKS_PEAKS_IO_MANAGER_H__


namespace kome {
	namespace io {
		namespace peaks {

			class PeaksIoOperation;
			/**
			 * @class PeaksIOManager
			 * @brief peaks IO manager class
			 */
			class PeaksIOManager {
			protected:
				/**
				 * @fn PeaksIOManager()
				 * @brief constructor
				 */
				PeaksIOManager();

				/**
				 * @fn virtual ~PeaksIOManager()
				 * @brief destructor
				 */
				virtual ~PeaksIOManager();

			protected:
				/** overlapping 2D map */
				std::map< kome::objects::DataMapInfo*, kome::objects::Peaks2D > m_overlapping2dMap;

				/** blink flag */
			    int m_blink;

				/** blink count */
				int m_blinkCnt;
				
				/** peak info */
				typedef struct{
					double x;
					double y;
					double leftX;
					double leftY;
					double rightX;
					double rightY;
				}peaks;

				/** point info */
				std::map< int, std::vector< peaks > > m_point;
				
				/** filter number */
				int m_filter;
			protected:
				/**
				 * @fn bool setMgfFileInfo( const char* path )
				 * @brief set mgf file info
				 * @parma path
				 * @return
				 */
				bool setMgfFileInfo( const char* path );
			public:
				/**
				 * @fn bool importSpecPeaks( kome::objects::Spectrum* spec )
				 * @brief import spectrum peaks
				 * @param spec spectrum objects
				 * @return
				 */
				bool importSpecPeaks( kome::objects::Spectrum* spec );

				/**
				 * @fn bool importChormPeaks( kome::objects::Chromatogram* chrom )
				 * @brief import chromatogram peaks
				 * @param chorm chromatogram objects
				 * @return
				 */
				bool importChormPeaks( kome::objects::Chromatogram* chrom );

				/**
				 * @fn bool importPeaks( kome::objects::Peaks* peaks , const char* path )
				 * @brief import peaks
				 * @param peaks
				 * @parma path
				 */
				bool importPeaks( kome::objects::Peaks* peaks , const char* path );
				
				/**
				 * @fn int getNumberOfPoints()
				 * @brief get number of points
				 * @return point size
				 */
				int getNumberOfPoints();

				/**
				 * @fn bool getDataSet( kome::objects::Variant var, int target, kome::objects::DataSet& dataSet )
				 * @brief get data set
				 * @param[in] var
				 * @param[in] target
				 * @param[out] dataSet DataSet object
				 */
				bool getDataSet( kome::objects::Variant var, int target, kome::objects::DataSet& dataSet );

				/**
				 * @fn bool exportSpecPeaks( kome::objects::DataSet ds, const char* path )
				 * @brief export spectrum peaks
				 * @param ds
				 * @param path
				 */
				bool exportSpecPeaks( kome::objects::DataSet ds, const char* path );
				
				/**
				 * @fn bool exportChromPeaks( kome::objects::DataSet ds, const char* path )
				 * @brief export chromatogram peaks
				 * @param ds
				 * @param path
				 */
				bool exportChromPeaks( kome::objects::DataSet ds, const char* path );
				
				/**
				 * @fn bool export2dPeaks(
						kome::objects::DataGroupNode* group,
						const char* path
					)
				 * @brief exports 2D peaks
				 * @param[in] group group
				 * @param[in] path file path to export
				 * @return If true, it succeeded to export 2d peaks.
				 */
				bool export2dPeaks(
					kome::objects::DataGroupNode* group,
					const char* path
				);

				/**
				 * @fn bool import2dPeaks(
						kome::objects::Peaks2D* peaks,
						const char* path
					)
				 * @brief imports 2d peaks
				 * @param[out] peaks peaks object to store peaks information
				 * @param path import file path
				 * @return If true, it succeeded to import 2d peaks.
				 */
				bool import2dPeaks(
					kome::objects::Peaks2D* peaks,
					const char* path
				);

			public:
				/**
				* @fn bool addOverlapping2dPeaks( kome::objects::DataMapInfo* dataMap, const char* path )
				* @brief adds overlapping 2D peaks
				* @param dataMap data map object
				* @param path import file path
				* @return If true, it succeeded to import overlapping peaks.
				*/
				bool addOverlapping2dPeaks( kome::objects::DataMapInfo* dataMap, const char* path );

				/**
				 * @fn void deleteOverlapping2dPeaks( kome::objects::DataMapInfo* dataMap );
				 * @brief deletes overlapping 2D peaks
				 * @param dataMap data map object
				 */
				void deleteOverlapping2dPeaks( kome::objects::DataMapInfo* dataMap );

				/**
				 * @fn void drawOverlapping2dPeaks( kome::objects::DataMapInfo* dataMap, kome::img::Graphics& g )
				 * @brief draws overlapping 2d peaks
				 * @param dataMap data map object
				 * @param g graphics
				 */
				void drawOverlapping2dPeaks( kome::objects::DataMapInfo* dataMap, kome::img::Graphics& g );
			
				/**
				 * @fn static void blinkOverlappingPeaks()
				 * @brief blinks overlapping peaks
				 */
				static void blinkOverlappingPeaks();

			public:
				/**
				 * @fn std::string showFileOpenDialog()
				 * @brief shows file open dialog
				 * @return selected file path
				 */
				std::string showFileOpenDialog( bool bflg = false );

				/**
				 * @fn std::string showFileSaveDialog()
				 * @brief shows file save dialog
				 * @return selected file path
				 */
				std::string showFileSaveDialog();

			public:
				/**
				 * @fn static PeaksIOManager& getInstance()
				 * @brief get the PeaksIOManager object (This is the only object.)
				 * @return PeaksIOManager object
				 */
				static PeaksIOManager& getInstance();

			};
		}
	}
}


#endif	// __KOME_IO_PEAKS_PEAKS_IO_MANAGER_H__
