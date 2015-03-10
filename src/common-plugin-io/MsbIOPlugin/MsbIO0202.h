/**
 * @file MsbIO0202.h
 * @brief interfaces of MsbIO0202 class
 *
 * @author H.Parry
 * @date 2011.04.20
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_IO_MSB_IO_0202_H__
#define __KOME_IO_MSB_IO_0202_H__


#include "MsbManager.h"


namespace kome {
	namespace io {
		namespace msb {

			class MsbSpectrum;
			class MsbChromatogram;

			/**
			 * @class MsbIO0202
			 * @brief msb format ver2.1 IO manager class
			 */
			class MsbIO0202 : public MsbManager {
			public:
				/**
				 * @fn MsbIO0202()
				 * @brief constructor
				 */
				MsbIO0202();

				/**
				 * @fn virtual ~MsbIO0202()
				 * @brief destructor
				 */
				virtual ~MsbIO0202();

			protected:
				/** file header */
				static DataValue m_fileHeader[];
				/** data header */
				static DataValue m_dataHeader[];
				/** spectrum group information */
				static DataValue m_groupInfo[];
				/** chromatogram information */
				static DataValue m_chromatogramInfo[];
				/** spectrum information */
				static DataValue m_spectrumInfo[];
				/** spectrum part information */
				static DataValue m_partInfo[];
				/** property information */
				static DataValue m_prop[];

			protected:
				/**
				 * @fn void createStringTable( char** stringTable )
				 * @brief cerates string table
				 * @param stringTable the pointer to store string table
				 */
				void createStringTable( char** stringTable );

				/**
				 * @fn void getFileHeaderInfo( MsbSample& sample, char* stringTable )
				 * @brief gets file header information
				 * @param file msb sample object
				 * @param stringTable string table
				 */
				void getFileHeaderInfo( MsbSample& sample, char* stringTable );

				/**
				 * @fn void readDataGroupNodes(
						kome::objects::DataGroupNode* parentGroup,
						std::map< unsigned int, kome::objects::DataGroupNode* >& groupMap,
						char* stringTable
					)
				 * @brief read spectrum group information
				 * @param parentGroup parent spectrum group (root group)
				 * @param groupMap spectrum group ID map
				 * @param stringTable string table
				 */
				void readDataGroupNodes(
					kome::objects::DataGroupNode* parentGroup,
					std::map< unsigned int, kome::objects::DataGroupNode* >& groupMap,
					char* stringTable
				);

				/**
				 * @fn void readDataGroupNodeProperties(
						std::map< unsigned int, kome::objects::DataGroupNode* >& groupMap,
						char* stringTable
					)
				 * @brief reads spectrum group properties
				 * @param groupMap spectrum group ID map
				 * @param stringTable string table
				 */
				void readDataGroupNodeProperties(
					std::map< unsigned int, kome::objects::DataGroupNode* >& groupMap,
					char* stringTable
				);

				/**
				 * @fn void readDataGroupNodeUserProperties(
						std::map< unsigned int, kome::objects::DataGroupNode* >& groupMap,
						char* stringTable
					)
				 * @brief reads spectrum group user properties
				 * @param groupMap spectrum group ID map
				 * @param stringTable string table
				 */
				void readDataGroupNodeUserProperties(
					std::map< unsigned int, kome::objects::DataGroupNode* >& groupMap,
					char* stringTable
				);

				/**
				 * @fn void readChromatograms(
						MsbSample& sample,
						std::map< unsigned int, MsbChromatogram* >& chromMap,
						std::map< unsigned int, kome::objects::DataGroupNode* >& groupMap,
						char* stringTable
					)
				 * @brief reads chromatogram information
				 * @param file msb sample object
				 * @param chromMap chromatogram ID map
				 * @param groupMap spectrum group ID map
				 * @param stringTable string table
				 */
				void readChromatograms(
					MsbSample& sample,
					std::map< unsigned int, MsbChromatogram* >& chromMap,
					std::map< unsigned int, kome::objects::DataGroupNode* >& groupMap,
					char* stringTable
				);

				/**
				 * @fn void readChromatogramProperties(
						std::map< unsigned int, MsbChromatogram* >& chromMap,
						char* stringTable
					)
				 * @brief reads chromatogram property information
				 * @param chromMap chromatogram ID map
				 * @param stringTable string table
				 */
				void readChromatogramProperties(
					std::map< unsigned int, MsbChromatogram* >& chromMap,
					char* stringTable
				);

				/**
				 * @fn void readChromatogramUserProperties(
						std::map< unsigned int, MsbChromatogram* >& chromMap,
						char* stringTable
					)
				 * @brief reads chromatogram user property information
				 * @param chromMap chromatogram ID map
				 * @param stringTable string table
				 */
				void readChromatogramUserProperties(
					std::map< unsigned int, MsbChromatogram* >& chromMap,
					char* stringTable
				);

				/**
				 * @fn void readSpectra(
						MsbSample& sample,
						std::map< unsigned int, MsbSpectrum* >& specMap,
						std::map< unsigned int, kome::objects::DataGroupNode* >& groupMap,
						char* stringTable
					)
				 * @brief reads spectrum information
				 * @param file msb sample object
				 * @param specMap spectrum ID map
				 * @param groupMap spectrum group ID map
				 * @param stringTable string table
				 */
				void readSpectra(
					MsbSample& sample,
					std::map< unsigned int, MsbSpectrum* >& specMap,
					std::map< unsigned int, kome::objects::DataGroupNode* >& groupMap,
					char* stringTable
				);

				/**
				 * @fn void readSpectrumParts(
						std::map< unsigned int, MsbSpectrum* >& specMap
					)
				 * @brief reads spectrum part information
				 * @param specMap spectrum ID map
				 */
				void readSpectrumParts(
					std::map< unsigned int, MsbSpectrum* >& specMap
				);

				/**
				 * @fn void readSpectrumProperties(
						std::map< unsigned int, MsbSpectrum* >& specMap,
						char* stringTable
					)
				 * @brief reads spectrum property information
				 * @param specMap spectrum ID map
				 * @param stringTable string table
				 */
				void readSpectrumProperties(
					std::map< unsigned int, MsbSpectrum* >& specMap,
					char* stringTable
				);

				/**
				 * @fn void readSpectrumUserProperties(
						std::map< unsigned int, MsbSpectrum* >& specMap,
						char* stringTable
					)
				 * @brief reads spectrum user property information
				 * @param specMap spectrum ID map
				 * @param stringTable string table
				 */
				void readSpectrumUserProperties(
					std::map< unsigned int, MsbSpectrum* >& specMap,
					char* stringTable
				);

				/**
				 * @fn void writeDataGroupNodes(
							std::vector< kome::objects::DataGroupNode* >& groups,
								std::map< kome::objects::DataGroupNode*, unsigned int >& groupMap
						)
				 * @brief writes spectrum group information
				 * @param groups spectrum group information array
				 * @param groupMap the map to store spctrum group ID
				 */
				void writeDataGroupNodes(
					std::vector< kome::objects::DataGroupNode* >& groups,
					std::map< kome::objects::DataGroupNode*, unsigned int >& groupMap
				);

				/**
				 * @fn void writeDataGroupNodeProperties(
						std::vector< kome::objects::DataGroupNode* >& groups,
						std::map< kome::objects::DataGroupNode*, unsigned int >& groupMap
					)
				 * @brief writes spectrum group properties
				 * @param groups spectrum group information array
				 * @param groupMap the map to store spctrum group ID
				 */
				void writeDataGroupNodeProperties(
					std::vector< kome::objects::DataGroupNode* >& groups,
					std::map< kome::objects::DataGroupNode*, unsigned int >& groupMap
				);

				/**
				 * @fn void writeDataGroupNodeUserProperties(
						std::vector< kome::objects::DataGroupNode* >& groups,
						std::map< kome::objects::DataGroupNode*, unsigned int >& groupMap
					)
				 * @brief writes spectrum group user properties
				 * @param groups spectrum group information array
				 * @param groupMap the map to store spctrum group ID
				 */
				void writeDataGroupNodeUserProperties(
					std::vector< kome::objects::DataGroupNode* >& groups,
					std::map< kome::objects::DataGroupNode*, unsigned int >& groupMap
				);

				/**
				 * @fn void writeChromatogramData(
						unsigned int id,
						kome::objects::Chromatogram& chrom,
						kome::core::DataPoints& points,
						std::vector< ChromatogramDataInfo >& chromData
					)
				  * @brief writes chromatogram data
				  * @param id chromatogram id
				  * @param chrom chromatogram
				  * @param points data points object
				  * @param chromData the array to store chromatogram data 
				  */
				void writeChromatogramData(
					unsigned int id,
					kome::objects::Chromatogram& chrom,
					kome::core::DataPoints& points,
					std::vector< ChromatogramDataInfo >& chromData
				);

				/** 
				 * @fn void writeChromatograms( 
						std::vector< ChromatogramDataInfo >& chromData,
						std::map< kome::objects::Chromatogram*, unsigned int >& chromatogramMap,
						std::map< kome::objects::DataGroupNode*, unsigned int >& groupMap
					)
				 * @brief writes chromatograms information
				 * @param chromData chromatogram information array
				 * @param chromatogramMap chromatogram ID map
				 * @param groupMap spectrum group ID map
				 */
				void writeChromatograms( 
					std::vector< ChromatogramDataInfo >& chromData,
					std::map< kome::objects::Chromatogram*, unsigned int >& chromatogramMap,
					std::map< kome::objects::DataGroupNode*, unsigned int >& groupMap
				);

				/**
				 * @fn void writeChromatogramProperties(
						kome::objects::DataSet& dataSet,
						std::map< kome::objects::Chromatogram*, unsigned int >& chromatogramMap
					)
				 * @brief writes chromatogram property information
				 * @param dataSet chromatogram data set
				 * @param chromatogramMap chromatogram ID map
				 */
				void writeChromatogramProperties(
					kome::objects::DataSet& dataSet,
					std::map< kome::objects::Chromatogram*, unsigned int >& chromatogramMap
				);

				/**
				 * @fn void writeChromatogramUserProperties(
						kome::objects::DataSet& dataSet,
						std::map< kome::objects::Chromatogram*, unsigned int >& chromatogramMap
					)
				 * @brief writes chromatogram user property information
				 * @param dataSet chromatogram data set
				 * @param chromatogramMap chromatogram ID map
				 */
				void writeChromatogramUserProperties(
					kome::objects::DataSet& dataSet,
					std::map< kome::objects::Chromatogram*, unsigned int >& chromatogramMap
				);

				/**
				 * @fn int writeSpectrumData(
						unsigned int id,
						kome::objects::Spectrum& spec,
						kome::core::DataPoints& points,
						FILE* partsFp
					)
				 * @brief writes spectrum data
				 * @param id spectrum id
				 * @param spec spectrum
				 * @param points data points object
				 * @param partsFp file descriptor to write spectrum parts
				 * @return the number of spectrum parts
				 */
				int writeSpectrumData(
					unsigned int id,
					kome::objects::Spectrum& spec,
					kome::core::DataPoints& points,
					FILE* partsFp
				);

				/**
				 * @fn void writeSpectra(
						kome::objects::DataSet& dataSet,
						std::map< kome::objects::Spectrum*, unsigned int >& spectrumMap,
						std::map< kome::objects::DataGroupNode*, unsigned int >& groupMap
					)
				 * @brief writes spectrum information
				 * @param spectra spectra information
				 * @param spectrumMap spectrum ID map
				 * @param groupMap spectrum group ID map
				 */
				void writeSpectra(
					kome::objects::DataSet& dataSet,
					std::map< kome::objects::Spectrum*, unsigned int >& spectrumMap,
					std::map< kome::objects::DataGroupNode*, unsigned int >& groupMap
				);

				/**
				 * @fn void writeSpectrumParts( const int cnt, FILE* partsFp )
				 * @brief writes spectrum part information
				 * @param cnt spectrum parts count
				 * @param partsFp file descriptor of parts data
				 */
				void writeSpectrumParts( const int cnt, FILE* partsFp );

				/**
				 * @fn void writeSpectrumProperties(
						kome::objects::DataSet& dataSet,
						std::map< kome::objects::Spectrum*, unsigned int >& spectrumMap
					)
				 * @brief writes spectrum property information
				 * @param spectra spectrum array
				 * @param spectrumMap spectrum ID map
				 */
				void writeSpectrumProperties(
					kome::objects::DataSet& dataSet,
					std::map< kome::objects::Spectrum*, unsigned int >& spectrumMap
				);

				/**
				 * @fn void writeSpectrumUserProperties(
						kome::objects::DataSet& dataSet,
						std::map< kome::objects::Spectrum*, unsigned int >& spectrumMap
					)
				 * @brief writes spectrum user property information
				 * @param spectra spectrum array
				 * @param spectrumMap spectrum ID map
				 */
				void writeSpectrumUserProperties(
					kome::objects::DataSet& dataSet,
					std::map< kome::objects::Spectrum*, unsigned int >& spectrumMap
				);

			protected:
				/**
				 * @fn virtual void onReadSampleInfo( MsbSample& sample )
				 * @brief This mehtod is called by readSampleInfo methjod. (override method)
				 * @param sample msb sample object
				 */
				virtual void onReadSampleInfo( MsbSample& sample );

				/**
				 * @fn virtual void onReadMsb( MsbSample& sample )
				 * @brief This method is called by MsbSample::onOpenFile. (override method)
				 * @param file msb sample file object
				 */
				virtual void onReadMsb( MsbSample& sample );

				/**
				 * @fn virtual bool onWriteMsb(
						kome::objects::Sample& sample,
						kome::objects::DataSet& dataSet,
						bool op,
						kome::core::Progress& progress
					)
				 * @brief This method is called by writeDataGroupNode method. (override method)
				 * @param sample sample
				 * @param spectra spectrum informations to save
				 * @param op If true, writing operated spectrum data
				 * @param progress progress object
				 * @return If it succeeded to save file, this method returns true.
				 */
				virtual bool onWriteMsb(
					kome::objects::Sample& sample,
					kome::objects::DataSet& dataSet,
					bool op,
					kome::core::Progress& progress
				);
			};
		}
	}
}

#endif	//  __KOME_IO_MSB_IO_0202_H__
