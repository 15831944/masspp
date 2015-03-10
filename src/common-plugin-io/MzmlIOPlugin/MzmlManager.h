/**
 * @file MzmlManager.h
 * @brief interfaces of MzmlManager class
 *
 * @author S.Tanaka
 * @date 2009.03.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_MZML_MANAGER_H__
#define __KOME_IO_MZML_MANAGER_H__


namespace kome {
	namespace io {
		namespace mzml {

			class Accession;

			/**
			 * @class MzmlManager
			 * @brief mzML IO management class
			 */
			class MzmlManager {
			protected:
				/**
				 * @fn MzmlManager()
				 * @brief constructor
				 */
				MzmlManager();

				/**
				 * @fn virtual ~MzmlManager()
				 * @brief destructor
				 */
				virtual ~MzmlManager();

			protected:
				/** spectrum index map*/
				std::map< kome::objects::Spectrum*, long long > m_specIdxMap;

				/** chromatogram index map */
				std::map< kome::objects::Chromatogram*, long long > m_chromIdxMap;

				/** reverse flag */
				const bool m_reverseFlg;

			protected:
				/**
				 * @fn void init()
				 * @brief initializes variables
				 */
				void init();

			public:
				/**
				 * @fn bool exportDataSet( const char* path, kome::objects::DataSet& dataSet, kome::core::Progress& progress )
				 * @brief exports spectra data to mzML file
				 * @param path mzML file path
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
				 * @brief exports spectra data to mzML file
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
				 * @fn static bool lessSpec( kome::objects::Spectrum* spec0, kome::objects::Spectrum* spec1 )
				 * @brief compare spectra to sort
				 * @param spec0 spectrum to be compared
				 * @param spec1 spectrum to compare
				 * @return If true, spec0 sorts before spec1.
				 */
				static bool lessSpec( kome::objects::Spectrum* spec0, kome::objects::Spectrum* spec1 );
			protected:
				/**
				 * @fn void writeHeader( FILE* fp )
				 * @brief writes header
				 * @param fp file descriptor
				 */
				void writeHeader( FILE* fp );

				/**
				 * @fn void writeFileDescription( FILE* fp, std::vector< kome::objects::Sample* >& samples )
				 * @brief writes file description
				 * @param fp file descriptor
				 * @param samples the array of samples
				 */
				void writeFileDescription( FILE* fp, std::vector< kome::objects::Sample* >& samples );

				/**
				 * @fn void writeFileContent( FILE* fp, std::vector< kome::objects::Sample* >& samples )
				 * @brief writes file content
				 * @param fp file descriptor
				 * @param samples the array of samples
				 */
				void writeFileContent( FILE* fp, std::vector< kome::objects::Sample* >& samples );

				/**
				 * @fn void writeSourceFileList( FILE* fp, std::vector< kome::objects::Sample* >& samples )
				 * @brief writes source file list
				 * @param fp file descriptor
				 * @param samples the array of samples
				 */
				void writeSourceFileList( FILE* fp, std::vector< kome::objects::Sample* >& samples );

				/**
				 * @fn void writeReferenceableParamGroup( FILE* fp, const bool specFlg, const bool chromFlg )
				 * @brief writes referenceable parameters group
				 * @param fp file descriptor
				 * @param specFlg spectrum flag
				 * @param chromFlg chromatogram flag
				 */
				void writeReferenceableParamGroup( FILE* fp, const bool specFlg, const bool chromFlg );

				/**
				 * @fn void writeSoftwareList( FILE* fp, std::vector< kome::objects::Sample* >& samples )
				 * @brief writes software list
				 * @param fp file descriptor
				 * @param samples the array of samples
				 */
				void writeSoftwareList( FILE* fp, std::vector< kome::objects::Sample* >& samples );

				/**
				 * @fn void writeInstrumentList( FILE* fp, std::vector< kome::objects::Sample* >& samples )
				 * @brief writes instrument list
				 * @param fp file descriptor
				 * @param samples the array of samples
				 */
				void writeInstrumentList( FILE* fp, std::vector< kome::objects::Sample* >& samples );

				/**
				 * @fn void writeProcessingList( FILE* fp )
				 * @brief writes processing list
				 * @param fp file descriptor
				 */
				void writeProcessingList( FILE* fp );

				/**
				 * @fn void writeRun(
							FILE* fp,
							std::vector< kome::objects::Sample* >& samples,
							std::vector< kome::objects::Spectrum* >& spectra,
							std::vector< kome::objects::Chromatogram* >& chroms,
							kome::core::Progress* specPrgs,
							kome::core::Progress* chromPrgs
						)
				 * @brief writes run tag
				 * @param fp file descriptor
				 * @param samples the array of samples
				 * @param spectra the array of spectra
				 * @param chroms the array of chromatograms
				 * @param specPrgs the object to show the progress of writing spectra
				 * @param chromPrgs the object tos show the progress of writing chromatograms
				 */
				void writeRun(
					FILE* fp,
					std::vector< kome::objects::Sample* >& samples,
					std::vector< kome::objects::Spectrum* >& spectra,
					std::vector< kome::objects::Chromatogram* >& chroms,
					kome::core::Progress* specPrgs,
					kome::core::Progress* chromPrgs,
					kome::objects::SettingParameterValues* saveSettings		// @date 2012/08/21 <Add> FUJITA
				);

				/**
				 * @fn void writeSpectrum( FILE* fp, kome::objects::Spectrum& spec )
				 * @brief writes spectrum
				 * @param fp file descriptor
				 * @param spec spectrum
				 */
				void writeSpectrum( 
						FILE* fp,
						kome::objects::Spectrum& spec,
						kome::objects::SettingParameterValues* saveSettings		// @date 2012/08/21 <Add> FUJITA
						);

				/**
				 * @fn void writeChromatogram( FILE* fp, kome::objects::Chromatogram& chrom )
				 * @brief writes chromatogram
				 * @param fp file descriptor
				 * @param chrom chromatogram
				 */
				void writeChromatogram( 
						FILE* fp,
						kome::objects::Chromatogram& chrom,
						kome::objects::SettingParameterValues* saveSettings		// @date 2012/08/21 <Add> FUJITA
						);

				/**
				 * @fn void writeIndexList(
							FILE* fp,
							std::vector< kome::objects::Spectrum* >& spectra,
							std::vector< kome::objects::Chromatogram* >& chroms
						)
				 * @brief writes index list
				 * @param fp file descriptor
				 * @param spectra the array of spectra
				 * @param chroms the array of chromatograms
				 */
				void writeIndexList(
					FILE* fp,
					std::vector< kome::objects::Spectrum* >& spectra,
					std::vector< kome::objects::Chromatogram* >& chroms
				);

			protected:
				/**
				 * @fn static double getSimilarityScore( const char* s0, const char* s1 )
				 * @brief gets similarity score between two character strings
				 * @param s0 character string
				 * @param s1 character string
				 * @return similarity score
				 */
				static double getSimilarityScore( const char* s0, const char* s1 );

				/**
				 * @fn static double getMatchScore( const char* s0, int idx0, const char* s1, int idx1 )
				 * @brief gets match score
				 * @param s0 character string
				 * @param idx0 position
				 * @param s1 character string
				 * @param idx1 position
				 * @return match score
				 */
				static double getMatchScore( const char* s0, int idx0, const char* s1, int idx1 );

			public:
				/**
				 * @fn static MzmlManager& getInstance()
				 * @brief gets mzML IO management object
				 * @return mzML IO management object (This is the only object.)
				 */
				static MzmlManager& getInstance();
			};
		}
	}
}

#endif		// __KOME_IO_MZML_MANAGER_H__
