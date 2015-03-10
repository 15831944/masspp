/**
 * @file LcmsSample.h
 * @brief interfaces of LcmsSample class
 *
 * @author S.Tanaka
 * @date 2008.02.08
 * 
 * Copyright(C) 2006-2013 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_LCMS_SAMPLE_H__
#define __KOME_IO_LCMS_SAMPLE_H__


namespace kome {
	namespace io {
		namespace lcms {

			ref class LcmsSampleSet;

			/**
			 * @struct EventInfo
			 * @brief event information
			 */
			ref struct EventInfo {
				kome::clr::SpectrumWrapper::Polarity pol;
				double startMz;
				double endMz;
				int seg;
				int eve;
				int stage;
				kome::clr::DataGroupNodeWrapper^ node;
			};

			/**
			 * @class LcmsSample
			 * @brief sample of Lcms data
			 */
			public ref class LcmsSample : public kome::clr::ClrSampleBase {
			public:
				/**
				 * @fn LcmsSample( LcmsSampleSet^ sampleSet )
				 * @brief constructor
				 * @param sampleSet sample set object	
				 */
				LcmsSample( LcmsSampleSet^ sampleSet );

				/**
				 * @fn virtual ~LcmsSample()
				 * @brief destructor
				 */
				virtual ~LcmsSample();

			protected:
				/** sample set */
				LcmsSampleSet^ m_sampleSet;

				/** creation data */
				System::DateTime^ m_creationDate;

				/** creation operator */
				System::String^ m_creationOperator;

				/** modification date */
				System::DateTime^ m_modificationDate;

				/** modification operator */
				System::String^ m_modificationOperator;

				/** sample ID */
				System::String^ m_sampleId;

				/** tray name */
				System::String^ m_trayName;

				/** vial */
				System::String^ m_vial;

				/** description */
				System::String^ m_desc;
							
				/** exp */
				TTFLDataExport::ITTFLDataExportEx^ m_exp;

				/** m_rootGroup */
				kome::clr::DataGroupNodeWrapper^ m_rootGroup;

				/** evtTable */
				System::Collections::Hashtable^ m_evtTable;

				/** BPC table */
				System::Collections::Hashtable^ m_bpcTable;

				/** m_countTable */
				System::Collections::Hashtable^ m_countTable;

				/** RT */
				cli::array< long >^ m_rts;

				/** TICs */
				cli::array< long >^ m_tics;

				/** BPCs */
				cli::array< long >^ m_bpcs;

				/** types */
				cli::array< long >^ m_types;

				/** precursors */
				cli::array< long >^ m_precs;

				/** others */
				cli::array< long >^ m_others;

				/** all scan count */
				int m_allScanCnt;

			public:
				/**
				 * @fn LcmsSampleSet^ getLcmsSampleSet()
				 * @brief gets sample set object
				 * @return sample set object
				 */
				LcmsSampleSet^ getLcmsSampleSet();

				/**
				 * @fn void setCreationDate( System::DateTime^ date )
				 * @brief sets creation date
				 * @param date creation date
				 */
				void setCreationDate( System::DateTime^ date );

				/**
				 * @fn System::DateTime^ getCreationDate()
				 * @brief gets creation date
				 * @return creation date
				 */
				System::DateTime^ getCreationDate();

				/**
				 * @fn void setCreationOperator( System::String^ op )
				 * @brief sets creation operator
				 * @param op creation operator
				 */
				void setCreationOperator( System::String^ op );

				/**
				 * @fn System::String^ getCreationOperator()
				 * @brief gets creation operator
				 * @return creation operator
				 */
				System::String^ getCreationOperator();

				/**
				 * @fn void setModificationDate( System::DateTime^ date )
				 * @brief sets modification date
				 * @param date modification date
				 */
				void setModificationDate( System::DateTime^ date );

				/**
				 * @fn System::DateTime^ getModificationDate()
				 * @brief gets modification date
				 * @return modification date
				 */
				System::DateTime^ getModificationDate();

				/**
				 * @fn void setModificationOperator( System::String^ op )
				 * @brief sets modification operator
				 * @param op modification operator
				 */
				void setModificationOperator( System::String^ op );

				/** 
				 * @fn System::String^ getModificationOperator()
				 * @brief gets modification operator
				 * @return modification operator
				 */
				System::String^ getModificationOperator();

				/**
				 * @fn void setSampleId( System::String^ id )
				 * @brief sets sample ID
				 * @param id sample ID
				 */
				void setSampleId( System::String^ id );

				/**
				 * @fn System::String^ getSampleId();
				 * @brief gets sample ID
				 * @return sample ID
				 */
				System::String^ getSampleId();

				/**
				 * @fn void setTrayName( System::String^ name )
				 * @brief sets tray name
				 * @param name tray name
				 */
				void setTrayName( System::String^ name );

				/**
				 * @fn System::String^ getTrayName()
				 * @brief gets tray name
				 * @return tray name
				 */
				System::String^ getTrayName();

				/**
				 * @fn void setVial( System::String^ vial )
				 * @brief sets vial
				 * @param vial vial
				 */
				void setVial( System::String^ vial );

				/**
				 * @fn System::String^ getVial()
				 * @brief gets vial
				 * @return vial
				 */
				System::String^ getVial();

				/**
				 * @fn void setDesc( System::String^ desc )
				 * @brief sets the description
				 * @param[in] desc description
				 */
				void setDesc( System::String^ desc );

				/**
				 * @fn System::String^ getDesc()
				 * @brief gets the description
				 * @return description
				 */
				System::String^ getDesc();

			protected:
				/**
				 * @fn void setProperties( kome::clr::PropertiesWrapper^ props )
				 * @brief sets sample properties
				 * @param props the object to store properties
				 */
				void setProperties( kome::clr::PropertiesWrapper^ props );

				/**
				 * @fn void createSpectra()
				 * @brief creates spectra
				 */
				void createSpectra();

				/**
				 * @fn void createPrecursorSpectra(
							kome::clr::DataGroupNodeWrapper^ rootGroup,
							cli::array< long >^ mzs,
							cli::array< long >^ ints,
							cli::array< long >^ types,
							cli::array< long >^ types2
						)
				 * @brief creates precursor spectra
				 * @param[in] rootGroup root spectrum group
				 * @param[in] mzs m/z array
				 * @param[in] ints intensity array
				 * @param[in] types types array
				 * @param[in] types2 types2 array
				 */
				void createPrecursorSpectra(
					kome::clr::DataGroupNodeWrapper^ rootGroup,
					cli::array< long >^ mzs,
					cli::array< long >^ ints,
					cli::array< long >^ types,
					cli::array< long >^ types2
				);

				/**
				 * @fn void createChromatograms()
				 * @brief creates chromatograms
				 */
				void createChromatograms();

			public:
				/**
				 * @fn virtual bool onOpenSample( kome::clr::DataGroupNodeWrapper^ rootGroup )
				 * @brief This method is called by openSample method. (override method)
				 * @param rootGroup root spectrum group
				 * @return If true, it succeeded to open this sample.
				 */
				virtual bool onOpenSample( kome::clr::DataGroupNodeWrapper^ rootGroup ) override;

				/**
				 * @fn virtual bool onCloseSample()
				 * @brief This method is called by closeSample method. (override method)
				 * @return If true, it succeeded to close this sample.
				 */
				virtual bool onCloseSample() override;

				/**
				 * @fn virtual void onDetectPeaksByAPI( kome::clr::SpectrumWrapper^ spec, kome::clr::PeaksWrapper^ peaks )
				 * @brief detect peaks using Application Programming Interface. (override method)
				 * @param[in] spec spectrum to detect peaks
				 * @param[out] peaks the object to store peaks information.
				 */
				virtual void onDetectPeaksByAPI( kome::clr::SpectrumWrapper^ spec, kome::clr::PeaksWrapper^ peaks ) override;
			};
		}
	}
}

#endif		// __KOME_IO_LCMS_SAMPLE_H__
