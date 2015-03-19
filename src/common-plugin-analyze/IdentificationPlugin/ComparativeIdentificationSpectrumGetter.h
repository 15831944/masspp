/**
 * @file ComparativeIdentificationSpectrumGetter.h
 * @brief interfaces of ComparativeIdentificationSpectrumGetter class
 *
 * @author S.Tanaka
 * @date 2012.08.02
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_IDENT_COMPARATIVE_IDENTIFICATION_SPECTRUM_GETTER_H__
#define __KOME_IDENT_COMPARATIVE_IDENTIFICATION_SPECTRUM_GETTER_H__


#include "SpectrumGetter.h"


namespace kome {
	namespace ident {

		/**
		 * @class ComparativeIdentificationSpectrumGetter
		 * @brief spectrum getter class
		 */
		class ComparativeIdentificationSpectrumGetter : public SpectrumGetter {
		public:
			/**
			 * @fn ComparativeIdentificationSpectrumGetter( kome::ident::SearchEngine* engine )
			 * @brief constructor
			 * @param[in] engine search engine
			 */
			ComparativeIdentificationSpectrumGetter( kome::ident::SearchEngine* engine );

			/**
			 * @fn virtual ~ComparativeIdentificationSpectrumGetter()
			 * @brief destructor
			 */
			 virtual ~ComparativeIdentificationSpectrumGetter();

		protected:
			/** search engine */
			kome::ident::SearchEngine* m_engine;

			/** paths */
			std::vector< std::string> m_paths;

			/**
			 * @struct PeakInfo
			 * @brief peak information
			 */
			struct PeakInfo {
				int id;
				double rt;
				double mz;
			};

			/** peaks */
			std::vector< PeakInfo > m_peaks;
		
            /** file index */
			int m_fileIdx;

			/** spectrum index */
			int m_specIdx;

			/** spectra */
			std::vector< kome::objects::Spectrum* > m_spectra;

			/** peak ID map */
			std::map< kome::objects::Spectrum*, std::vector< int > > m_peakIdMap;

			/** current samle */
			kome::objects::Sample* m_currSample;

			/** sample set flag */
			bool m_ssFlg;

			/** sample flag */
			bool m_sFlg;

			/** RT tolerance */
			double m_rtTol;

			/** m/z tolerance */
			double m_mzTol;

		public:
			/**
			 * @fn void addFile( const char* path )
			 * @brief adds file
			 * @param[in] path file path
			 */
			void addFile( const char* path );

			/**
			 * @fn void clearFiles()
			 * @brief clears files
			 */
			void clearFiles();

			/**
			 * @fn void addPeak( const int peakId )
			 * @brief adds peak
			 * @param[in] peakId peak ID
			 */
			void addPeak( const int peakId );

			/**
			 * @fn void clearPeaks()
			 * @brief clears peaks
			 */
			void clearPeaks();

		public:
			/**
			 * @fn void setRtTol( const double tol )
			 * @brief sets the RT tolerance
			 * @param[in] tol RT tolerance
			 */
			void setRtTol( const double tol );

			/**
			 * @fn void setMzTol( const double tol )
			 * @brief sets the m/z tolerance
			 * @param[in] tol m/z tolerance
			 */
			void setMzTol( const double tol );

		public:
			/**
			 * @fn virtual bool next( kome::core::Progress& progress )
			 * @brief prepares next spectrum (override method)
			 * @param[out] progress the object to show the progress
			 * @return If true, the next spectrum exists.
			 */
			virtual bool next( kome::core::Progress& progress );

			/**
			 * @fp virtual kome::objects::Spectrum* getSpectrum()
			 * @brief gets the spectrum (override method)
			 * @return spectrum
			 */
			virtual kome::objects::Spectrum* getSpectrum();

			/**
			 * @fn virtual unsigned int getNumberOfPeakIds()
			 * @brief gets the number of peak IDs (override method)
			 * @return the number of peak Ids
			 */
			virtual unsigned int getNumberOfPeakIds();

			/**
			 * @fn virtual int getPeakId( const unsigned int idx )
			 * @brief gets the peak ID (override method)
			 * @param[in] idx peak ID index
			 * @return peak ID
			 */
			virtual int getPeakId( const unsigned int idx );

			/**
			 * @fn virtual void setParameters( const char* s )
			 * @brief sets parameters from character string (override method)
			 * @param[in] s character string
			 */
			virtual void setParameters( const char* s );

			/**
			 * @fn virtual std::string getParameters()
			 * @brief gets parameters (override method)
			 * @return parameter character string
			 */
			virtual std::string getParameters();

		};
	}
}


#endif    // __KOME_IDENT_COMPARATIVE_IDENTIFICATION_SPECTRUM_GETTER_H__
