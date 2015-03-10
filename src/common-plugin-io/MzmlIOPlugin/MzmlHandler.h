/**
 * @file MzmlHandler.h
 * @brief interfaces of MzmlHandler class
 *
 * @author S.Tanaka
 * @date 2009.03.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_MZML_HANDLER_H__
#define __KOME_IO_MZML_HANDLER_H__


#include "MzmlSample.h"

#include <map>


namespace kome {
	namespace io {
		namespace mzml {

			class Accession;
			class MzmlSpectrum;
			class MzmlChromatogram;

			/**
			 * @class MzmlHandler
			 * @brief XML handler class to read mzML file
			 */
			class MzmlHandler : public kome::xml::XmlHandler {
			public:
				/**
				 * @fn MzmlHandler( MzmlSample& file )
				 * @brief constructor
				 * @param file mzML file object
				 */
				MzmlHandler( MzmlSample& file );

				/**
				 * @fn virtual ~MzmlHandler()
				 * @brief destructor
				 */
				virtual ~MzmlHandler();

			protected:
				/** mzML file */
				MzmlSample& m_file;

			protected:
				/** machine accession */
				Accession* m_machineAcc;

				/** array accession */
				Accession* m_arrAcc;

				/** spectrum type accession */
				Accession* m_specAcc;

				/** software accession */
				Accession* m_softwareAcc;

				/** position */
				long long m_pos;

			protected:
				/**
				 * @typedef ArrayType
				 * @brief array data type
				 */
				typedef enum {
					TYPE_OTHER,
					TYPE_MZ,
					TYPE_RT,
					TYPE_INT
				} ArrayType;

				/** current spectrum */
				MzmlSpectrum* m_currSpec;

				/** current chromaotgram */
				MzmlChromatogram* m_currChrom;

				/** current array type */
				ArrayType m_currType;

				/** current array information */
				MzmlSample::ArrayInfo m_arrayInfo;

			protected:
				/** scan number */
				int m_scanNum;

				/**
				 * @struct SpecInfo
				 * @brief spectrum information
				 */
				struct SpecInfo {
					int stage;
					int centroid;
					kome::objects::Spectrum::Polarity polarity;
					double mzLowerLimit;
					double mzUpperLimit;
					double tic;
					double bpm;
					double bpi;
					double rt;
					double prec;
					std::string collisionEnergy;
					std::vector< std::pair< std::string, std::string > > props;
				};

				/** spectrum map */
				std::map< std::string, SpecInfo > m_specMap;

				/** current spectrum information */
				SpecInfo* m_currSpecInfo;

			protected:
				/** current software name */
				std::string m_currSoftwareName;

				/** current software version */
				std::string m_currSoftwareVersion;

			protected:
				/** in ion tag flag */
				bool m_inIonTag;

				/** in precursor */
				bool m_inPrecursor;

				/** in product */
				bool m_inProduct;

			protected:
				/** spectrum ID map */
				std::map< std::string, MzmlSpectrum* > m_specIdMap;

				/** scan map */
				std::map< int, MzmlSpectrum* > m_specScanMap;

				/** chromatogram ID map */
				std::map< std::string, MzmlChromatogram* > m_chromIdMap;

				/** offset scan number */
				int m_offsetScan;

				/** offset ID */
				std::string m_offsetId;

			protected:
				/**
				 * @fn SpecInfo* getSpecInfo( const char* name )
				 * @brief gets the spectrum information
				 * @param[in] name spectrum name
				 * @return spectrum information
				 */
				SpecInfo* getSpecInfo( const char* name );

				/**
				 * @fn void setSpecInfo( kome::objects::Spectrum* spec, SpecInfo* info )
				 * @brief sets the spectrum information
				 * @param[out] spec spectrum object
				 * @param[in] info spectrum information
				 */
				void setSpecInfo( kome::objects::Spectrum* spec, SpecInfo* info );
			
			protected:
				/**
				 * @fn virtual void onStartDocument()
				 * @brief This method is called by startDocument method. (override method)
				 */
				virtual void onStartDocument();

				/**
				 * @fn virtual void onEndDocument()
				 * @brief This method is called by endDocument method. (override method)
				 */
				virtual void onEndDocument();

				/**
				 * @fn virtual void onStartElement( const char* name, kome::core::Properties& attrs )
				 * @brief This method is called by startElement method. (override method)
				 * @param name tag name
				 * @param attrs attributes
				 */
				virtual void onStartElement( const char* name, kome::core::Properties& attrs );

				/**
				 * @fn virtual void onEndElement( const char* name, const char* text )
				 * @brief This method is called by end element method. (override method)
				 * @param name tag name
				 * @param text body text
				 */
				virtual void onEndElement( const char* name, const char* text );
			};
		}
	}
}


#endif		// __KOME_IO_MZML_HANDLER_H__
