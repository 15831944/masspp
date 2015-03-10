/**
 * @file MzXmlManager.h
 * @brief interfaces of MzXmlManager class
 *
 * @author S.Tanaka
 * @date 2007.03.27
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_MZXML_MANAGER_H__
#define __KOME_IO_MZXML_MANAGER_H__


namespace kome {
	namespace io {
		namespace mzxml {

			/**
			 * @class MzXmlManager
			 * @brief mzXML IO management class
			 */
			class MzXmlManager {
			protected:
				/**
				 * @fn MzXmlManager()
				 * @brief constructor
				 */
				MzXmlManager();

				/**
				 * @fn virtual ~MzXmlManager()
				 * @brief destructor
				 */
				virtual ~MzXmlManager();

			protected:
				/** scan number map */
				std::map< kome::objects::Spectrum*, unsigned int > m_scanNumberMap;

			protected:
				/**
				 * @fn void init()
				 * @brief initializes member variables
				 */
				void init();

			public:
				/**
				 * @fn bool exportDataSet( const char* path, kome::objects::DataSet& dataSet, kome::core::Progress& progress )
				 * @brief exports spectra data to mzXML file
				 * @param path mzXML file path
				 * @param spectra spectra data to be exported
				 * @param progress progress object to show progress
				 * @return If true, it succeeded to export data.
				 */
				bool exportDataSet( const char* path,
									kome::objects::DataSet& dataSet,
									kome::core::Progress& progress,
									kome::objects::SettingParameterValues* saveSettings		// @date 2012/08/21 <Add> FUJITA
									);

			protected:
				/**
				 * @fn void exportDataSet( FILE* fp, kome::objects::DataSet& dataSet, kome::core::Progress& progress )
				 * @brief exports spectra data to mzXML file
				 * @param fp file descriptor
				 * @param spectra spectra data to be exported
				 * @param progress progress object to show progress
				 */
				void exportDataSet( FILE* fp,
									kome::objects::DataSet& dataSet,
									kome::core::Progress& progress,
									kome::objects::SettingParameterValues* saveSettings		// @date 2012/08/21 <Add> FUJITA
									);

				/**
				 * @fn void writeHeader( FILE* fp )
				 * @brief writes mzXML header
				 * @param fp file descriptor
				 */
				void writeHeader( FILE* fp );

				/**
				 * @fn void writeFileData( FILE* fp, std::vector< kome::objects::Spectrum* >& spectra )
				 * @brief writes file data
				 * @param fp file descriptor
				 * @param spectra the array of spectra
				 */
				void writeFileData( FILE* fp, std::vector< kome::objects::Spectrum* >& spectra );

				/**
				 * @fn void writeSoftwareInfo( FILE* fp )
				 * @brief writes software information
				 * @param fp file descriptor
				 */
				void writeSoftwareInfo( FILE* fp );

				/**
				 * @fn void writeSpectrum( FILE* fp, kome::objects::Spectrum& spec )
				 * @brief writes spectrum data
				 * @param fp file descriptor
				 * @param spec spectrum data
				 * @return spectrum offset
				 */
				void writeSpectrum( FILE* fp,
									kome::objects::Spectrum& spec,
									kome::objects::SettingParameterValues* saveSettings		// @date 2012/08/21 <Add> FUJITA
									);

				/**
				 * @fn void writeIndex( FILE* fp, std::vector< unsigned long long >& offsets )
				 * @brief writes index data
				 * @param fp file descriptor
				 * @param offsets offset data
				 */
				void writeIndex( FILE* fp, std::vector< unsigned long long >& offsets );

			protected:
				/**
				 * @fn static bool lessSpec( kome::objects::Spectrum* spec0, kome::objects::Spectrum* spec1 )
				 * @brief compare spectra to sort
				 * @param spec0 spectrum to be compared
				 * @param spec1 spectrum to compare
				 * @return If true, spec0 sorts before spec1.
				 */
				static bool lessSpec( kome::objects::Spectrum* spec0, kome::objects::Spectrum* spec1 );

			public:
				/**
				 * @fn static MzXmlManager& getInstance()
				 * @brief gets mzXML IO management object
				 * @return mzXML IO management object (This is the only object.)
				 */
				static MzXmlManager& getInstance();
			};
		}
	}
}

#endif		// __KOME_IO_MZXML_MANAGER_H__
