/**
 * @file Sample.h
 * @brief interfaces of Sample class
 *
 * @author S.Tanaka
 * @date 2007.12.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_OBJECTS_SAMPLE_H__
#define __KOME_OBJECTS_SAMPLE_H__


#include <string>
#include <vector>
#include "DataSet.h"


namespace kome {
	namespace objects {

		class SampleSet;
		class DataGroupNode;
		class Spectrum;		
		class Chromatogram;	
		class Peaks;

		/**
		 * @class Sample
		 * @brief sample information management class
		 */
		class DATA_OBJECTS_CLASS Sample {
		public:
			/**
			 * @fn Sample( SampleSet* sampleSet )
			 * @brief constructor
			 */
			 Sample( SampleSet* sampleSet );

			/**
			 * @fn virtual ~Sample()
			 * @brief destructor
			 */
			virtual ~Sample();

		protected:
			/** sample set */
			SampleSet* m_sampleSet;

			/** sample index */
			int m_sampleIdx;

			/** spectrum ID */
			int m_specId;

			/** name */
			std::string m_name;

			/** type */
			std::string m_type;

			/** instrument name */
			std::string m_instrument;

			/** company name */
			std::string m_company;

			/** software name */
			std::string m_softwareName;

			/** software version */
			std::string m_softwareVersion;

			/** root spectrum group */
			DataGroupNode* m_root;

			/** opened flag */
			bool m_opened;

			/** edited flag */
			bool m_edited;

			/** groups */
			std::vector< DataGroupNode* > m_groups;

			/** parallel readable */
			bool m_parallelReadable;
			
			/** data set */
			DataSet m_hiddenSet;

			/** api peaks support */
			bool m_supportedApiPeaks;

			/** sample id */
			static int m_optSampleId;

		protected:
			/** common properties */
			static std::vector< std::string > m_commonProps;

		public:
			/**
			 * @fn SampleSet* getSampleSet()
			 * @brief gets sample set object
			 * @return sample set
			 */
			SampleSet* getSampleSet();

			/**
			 * @fn void setSampleIndex( const int index )
			 * @brief sets sample index
			 * @param[in] index sample index
			 */
			void setSampleIndex( const int index );

			/**
			 * @fn int getSampleIndex()
			 * @brief gets sample index
			 * @return sample index
			 */
			int getSampleIndex();

			/**
			 * @fn void setName( const char* name )
			 * @brief sets sample name
			 * @param[in] name sample name
			 */
			void setName( const char* name );

			/**
			 * @fn const char* getName()
			 * @brief gets sample name
			 * @return sample name
			 */
			const char* getName();

			/**
			 * @fn void setType( const char* type )
			 * @brief sets sample type
			 * @param[in] type sample type
			 */
			void setType( const char* type );

			/**
			 * @fn const char* getType()
			 * @brief gets sample type
			 * @return sample type
			 */
			const char* getType();

			/**
			 * @fn void setInstrument( const char* instrument )
			 * @brief sets instrument name
			 * @param[in] instrument instrument name
			 */
			void setInstrument( const char* instrument );

			/**
			 * @fn const char* getInstrument()
			 * @brief gets instrument name
			 * @return instrument name
			 */
			const char* getInstrument();

			/**
			 * @fn void setMsCompany( const char* company )
			 * @brief sets MS company
			 * @param[in] company MS company
			 */
			void setMsCompany( const char* company );

			/**
			 * @fn const char* getMsCompany()
			 * @brief gets MS company
			 * @return MS company
			 */
			const char* getMsCompany();

			/**
			 * @fn void setSoftwareName( const char* name )
			 * @brief sets software name
			 * @param[in] name software name
			 */
			void setSoftwareName( const char* name );

			/**
			 * @fn const char* getSoftwareName()
			 * @brief gets software name
			 * @return software name
			 */
			const char* getSoftwareName();

			/**
			 * @fn void setSoftwareVersion( const char* version )
			 * @brief sets software version
			 * @param[in] version software version
			 */
			void setSoftwareVersion( const char* version );

			/**
			 * @fn const char* getSoftwareVersion()
			 * @brief gets software version
			 * @return software version
			 */
			const char* getSoftwareVersion();

			/**
			 * @fn void setOpened( const bool opened )
			 * @brief sets opened flag
			 * @param[in] opened opened flag value
			 */
			void setOpened( const bool opened );

			/**
			 * @fn bool isOpened()
			 * @brief judges whther this sample is opened or not
			 * @return If true, this sample is opened.
			 */
			bool isOpened();

			/**
			 * @fn void setEdited( const bool edited )
			 * @brief sets edited flag
			 * @param[in] edited edited flag value
			 */
			void setEdited( const bool edited );

			/**
			 * @fn bool isEdited()
			 * @brief judges whther this sample is edited or not
			 * @return If true, this sample is edited.
			 */
			bool isEdited();

			/**
			 * @fn DataGroupNode* getRootDataGroupNode()
			 * @brief gets root spectrum group
			 * @return root spectrum group
			 */
			DataGroupNode* getRootDataGroupNode();

			/**
			 * @fn DataSet* getHiddenDataSet()
			 * @brief gets the hidden data set
			 * @return hidden data set
			 */
			DataSet* getHiddenDataSet();

			/**
			 * @fn unsigned int getNumberOfGroups()
			 * @brief gets the number of spectrum groups
			 * @return the number of spectrum groups
			 */
			unsigned int getNumberOfGroups();

			/**
			 * @fn DataGroupNode* getGroup( const unsigned int index )
			 * @brief gets spectrum group
			 * @param[in] index spectrum group index
			 * @return spectrum group (If NULL, the index is illegal.)
			 */
			DataGroupNode* getGroup( const unsigned int index );

			/**
			 * @fn void setParallelReadable( const bool readable )
			 * @brief sets parallel readable or not
			 * @param[in] readable If true, this sample is parallel readabale.
			 */
			void setParallelReadable( const bool readable );

			/**
			 * @fn bool isParallelReadable()
			 * @brief gets whether this sample is parallel readable or not
			 * @return If true, this sample is parallel readable.
			 */
			bool isParallelReadable();

			/**
			 * @fn static void resetId()
			 * @brief Reset the ID of the sample
			 */
			static void resetId();

		protected:
			/**
			 * @fn void getGroups( DataGroupNode* parentGroup = NULL )
			 * @brief gets spectrum groups
			 * @param[in] parentGroup parent spectrum group
			 */
			void getGroups( DataGroupNode* parentGroup = NULL );

		public:
			/**
			 * @fn bool openSample()
			 * @brief opens sample
			 * @paramp[out] progress progressbar to display
			 * @return If true, it succeeded to open this sample.
			 */
			bool openSample( kome::core::Progress* progress = NULL );

			/**
			 * @fn bool closeSample()
			 * @brief closes sample
			 * @return If true, it succeeded to close this sample.
			 */
			bool closeSample();

			/**
			 * @fn void detectPeaksByAPI( Spectrum* spec, Peaks* peaks )
			 * @brief detect peaks using Application Progreamming Interface
			 * @param[in] spec spectrum to detect peaks
			 * @param[out] peaks peaks object to store peaks information
			 */
			void detectPeaksByAPI( Spectrum* spec, Peaks* peaks );

		public:
			/**
			 * @fn static bool isCommonProperty( const char* key )
			 * @brief check whther the specified property key is common property or not
			 * @param[in] key parameter key
			 * @return If true, specified key is common property key
			 */
			static bool isCommonProperty( const char* key );
		
		public:
			/**
			 * @fn static int issueSampleId()
			 * @brief to issue the sample id
			 * @return sample id
			 */
			static int issueSampleId();

			/**
			 * @fn void setSampleId( int id )
			 * @brief set sample id
			 * @param[in] id sample id
			 */
			void setSampleId( int id );
			
			/**
			 * @fn int getSampleId()
			 * @brief get sample id
			 * @return sample id
			 */
			int getSampleId();

			/**
			 * @fn static Sample* getSampleById( int id )
			 * @brief A sample is acquired by id. 
			 * @param[in] id sample id
			 */
			static Sample* getSampleById( int id );
			
			/**
			 * @fn int issueSpecId()
			 * @brief to issue the spectrum id
			 * @return new spectrum ID
			 */
			int issueSpecId();

			/**
			 * @fn Spectrum* getSpectrumById( int id )
			 * @brief A spectrum is acquired by id
			 * @param[in] id spectrum id
			 * @return spectrum object
			 */
			Spectrum* getSpectrumById( int id );
			
			/**
			 * @fn int issueChromId( Sample* sample )
			 * @brief to issue the chromatogram id
			 * @param[in] sample sample object
			 * @return chromatogram id
			 */
			int issueChromId( Sample* sample );

			/**
			 * @fn Chromatogram* getChromatogramById( int id )
			 * @brief A schromatogram is acquired by id
			 * @param[in] id chromatogram id
			 * @return chromatogram object
			 */
			Chromatogram* getChromatogramById( int id );
			
			/**
			 * @fn int issueGroupId( Sample* sample )
			 * @brief to issue the group id
			 * @param[in] sample sample object
			 * @return group id
			 */
			int issueGroupId( Sample* sample );

			/**
			 * @fn DataGroupNode* getGroupById( int id )
			 * @brief A DataGroupNode is qcquired by id
			 * @param[in] id group id
			 * @return DataGroupNode object
			 */
			DataGroupNode* getGroupById( int id );

		public:
			/**
			 * @fn bool isSupportedAPIPeaks()
			 * @brief judges whether API peak detection is supported on this sample.
			 * @return If true, API peak detection is supported.
			 */
			bool isSupportedAPIPeaks();

			/**
			 * @fn void setSupportedAPIPeaks( const bool supported )
			 * @brief sets API peak dtection is supported on this sample.
			 * @param[in] supported If true, API peak detection is supported.
			 */
			void setSupportedAPIPeaks( const bool supported );

		protected:
			/**
		 	 * @fn virtual void onDetectPeaksByAPI( Spectrum* spec, Peaks* peaks )
			 * @brief This methos is called by detectPeakByAPI method.
			 */
			virtual void onDetectPeaksByAPI( Spectrum* spec, Peaks* peaks );

		protected:
			/**
			 * @fn virtual bool onOpenSample( DataGroupNode* rootGroup ) = 0
			 * @brief This method is called by openSample method. (abstract method)
			 * @param[out] rootGroup root spectrum group
			 * @paramp[out] progress progressbar to display
			 * @return If true, it succeeded to open this sample.
			 */
			virtual bool onOpenSample( DataGroupNode* rootGroup, kome::core::Progress* progress ) = 0;

			/**
			 * @fn virtual bool onCloseSample() = 0
			 * @brief This method is called by closeSample method. (abstract method)
			 * @return If true, it succeeded to close this sample.
			 */
			virtual bool onCloseSample() = 0;
		};
	}
}


#endif		// __KOME_OBJECTS_SAMPLE_H__
