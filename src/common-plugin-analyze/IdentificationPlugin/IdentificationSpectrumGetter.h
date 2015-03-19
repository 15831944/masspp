/**
 * @file IdentificationSpectrumGetter.h
 * @brief interfaces of IdentificationSpectrumGetter class
 *
 * @author S.Tanaka
 * @date 2012.08.02
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_IDENT_IDENTIFICATION_SPECTRUM_GETTER_H__
#define __KOME_IDENT_IDENTIFICATION_SPECTRUM_GETTER_H__


#include "SpectrumGetter.h"


namespace kome {
	namespace ident {

		/**
		 * @class IdentificationSpectrumGetter
		 * @brief spectrum getter class
		 */
		class IdentificationSpectrumGetter : public SpectrumGetter {
		public:
			/**
			 * @fn IdentificationSpectrumGetter( kome::objects::Sample* sample )
			 * @brief constructor
			 * @param[in] sample sample
			 */
			IdentificationSpectrumGetter( kome::objects::Sample* sample );

			/**
			 * @fn virtual ~IdentificationSpectrumGetter()
			 * @brief destructor
			 */
			 virtual ~IdentificationSpectrumGetter();

		protected:
			/** sample */
			kome::objects::Sample* m_sample;

			/** spectra */
			std::vector< kome::objects::Spectrum* > m_spectra;

			/** index */
			int m_index;

		public:
			/**
			 * @fn void addSpectrum( kome::objects::Spectrum* spec )
			 * @brief adds spectrum
			 * @param[in] spec spectrum
			 */
			void addSpectrum( kome::objects::Spectrum* spec );

			/**
			 * @fn void clearSpectra()
			 * @brief clears spectra
			 */
			void clearSpectra();

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


#endif    // __KOME_IDENT_IDENTIFICATION_SPECTRUM_GETTER_H__
