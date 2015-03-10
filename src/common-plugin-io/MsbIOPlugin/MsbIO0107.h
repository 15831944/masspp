/**
 * @file MsbIO0107.h
 * @brief interfaces of MsbIO0107 class
 *
 * @author S.Tanaka
 * @date 2007.09.04
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_MSB_IO_0107_H__
#define __KOME_IO_MSB_IO_0107_H__


#include "MsbManager.h"


namespace kome {
	namespace io {
		namespace msb {

			class MsbSpectrum;

			/**
			 * @class MsbIO0107
			 * @brief msb format ver1.7 IO manager class
			 */
			class MsbIO0107 : public MsbManager {
			public:
				/**
				 * @fn MsbIO0107()
				 * @brief constructor
				 */
				MsbIO0107();

				/**
				 * @fn virtual ~MsbIO0107()
				 * @brief destructor
				 */
				virtual ~MsbIO0107();

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
				 * @fn void readSpectra(
						MsbSample& sample,
						std::map< unsigned int, MsbSpectrum* >& specMap,
						std::map< unsigned int, kome::objects::DataGroupNode* >& groupMap,
						char* stringTable
					)
				 * @brief reads spectrum information
				 * @param file msb sample file object
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
				 * @fn void writeSpectrumData(
						unsigned int id,
						kome::objects::Spectrum& spec,
						kome::core::DataPoints& points,
						std::vector< SpectrumPartInfo >& parts
					)
				 * @brief writes spectrum data
				 * @param id spectrum id
				 * @param spec spectrum
				 * @param points data points object
				 * @param parts array to store spectrum part information
				 */
				void writeSpectrumData(
					unsigned int id,
					kome::objects::Spectrum& spec,
					kome::core::DataPoints& points,
					std::vector< SpectrumPartInfo >& parts
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
				 * @fn void writeSpectrumParts( std::vector< SpectrumPartInfo >& parts )
				 * @brief writes spectrum part information
				 * @param parts spectrum parts array
				 */
				void writeSpectrumParts( std::vector< SpectrumPartInfo >& parts );

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

			protected:
				/** file header */
				static DataValue m_fileHeader[];
				/** data header */
				static DataValue m_dataHeader[];
				/** spectrum group information */
				static DataValue m_groupInfo[];
				/** spectrum information */
				static DataValue m_spectrumInfo[];
				/** spectrum part information */
				static DataValue m_partInfo[];
				/** property information */
				static DataValue m_prop[];

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

#endif	//  __KOME_IO_MSB_IO_0107_H__
