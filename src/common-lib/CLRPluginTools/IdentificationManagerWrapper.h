/**
 * @file IdentificationManagerWrapper.h
 * @brief interfaces of IdentificationManagerWrapper class
 *
 * @author S.Tanaka
 * @date 2012.11.06
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation
 */


#ifndef __KOME_CLR_IDENTIFICATION_MANAGER_WRAPPER_H__
#define __KOME_CLR_IDENTIFICATION_MANAGER_WRAPPER_H__


#include "HitWrapper.h"
#include "SubstanceWrapper.h"
#include "SearchEngineWrapper.h"


namespace kome {
	namespace clr { 

		ref class EnzymeWrapper;
		ref class ModificationWrapper;
		ref class SearchEngineWrapper;
		ref class SearchResultWrapper;
		ref class AlignmentWrapper;
				
		/**
		 * @class IdentificationManagerWrapper
		 * @brief identification manager wrapper class to use on CLR environment
		 */
		public ref class IdentificationManagerWrapper {
		protected:
			/**
			 * @fn IdentificationManagerWrapper( kome::ident::IdentificationManager& identMgr )
			 * @briefructor
			 * @param[in] identMgr identification manager object
			 */
			IdentificationManagerWrapper( kome::ident::IdentificationManager& identMgr );

			/**
			 * @fn virtual ~IdentificationManagerWrapper()
			 * @brief destructor
			 */
			virtual ~IdentificationManagerWrapper();

		public:
			/** database file type */
			ENUM_CLASS DbFileType {
				OTHER = 0,
				FASTA = 1,
				FASTA_PRO = 2,
				BLAST = 4
			};

			/** column data type */
			ENUM_CLASS ColumnDataType {
				INT     = 1,
				UINT	= 2,
				DOUBLE  = 3,
				UDOUBLE = 4,
				STRING  = 5
			};


			/** substance relationship */
			ENUM_CLASS SubstanceRelationship {
				RELATIONSHIP_PARENT  = 1,
				RELATIONSHIP_SAME = 2
			};

			/** 
			 @struct SampleReadInfo
			 @brief sample read information
			*/
			ref struct SampleReadInfo {
				int id;
				bool openSampleSet;
				bool openSample;
			};

			/** 
			 @struct SubstanceInfo
			 @brief substance information
			*/
			ref struct SubstanceInfo {
				System::String^ accession;
				System::String^ name;
				int dbId;
				SubstanceWrapper::SubstanceType type;
				PropertiesWrapper^ props;
				int parentId;
			};

			/** substance value */
			ref struct SubstanceValue {
				System::String^ name;
				System::String^ value;
				HitWrapper::ValueType type;
			};

			/**
			 * @struct AltSubstance
			 * @brief alt substance
			 */
			ref struct AltSubstance {
				int substance;
				System::String^ before;
				System::String^ after;
				int start;
				int end;
			};

			/**
			 * @struct HitInfo
			 * @brief hit information
			 */
			ref struct HitInfo {
				int substance;
				int charge;
				cli::array< AltSubstance^ >^ altSubstances;
				cli::array< int >^ spectra;
				cli::array< SubstanceValue^ >^ props;
			};

			/** 
			 @struct FormatInfo
			 @brief format information
			*/
			ref struct FormatInfo {
				System::String^ name;
				System::String^ url;
			};

			/** 
			 @structNormalizationFilterInfo
			 @brief normalization filter information
			*/
			ref struct NormalizationFilterInfo {
				double minInt;
				double maxInt;
				bool relFlg;
				double minRt;
				double maxRt;
				double minMz;
				double maxMz;
				System::String^ method;
				System::String^ params;
			};

			/** 
			 @struct NormalizationMethodInfo
			 @brief normalization method information
			*/
			ref struct NormalizationMethodInfo {
				System::String^ method;
				System::String^ params;
			};

			/**
			 @struct SearchSpectrumInfo
			 @brief search spectrum
			*/
			ref struct SearchSpectrumInfo {
				int specId;
				int searchId;
				int peaks;
				double precursor;
			};

		protected:
			/** identification manager object */
			kome::ident::IdentificationManager& m_identMgr;

			/** identification manager wrapper object (This is the only object.) */
			static IdentificationManagerWrapper^ m_mgr;

		public:
			/**
			 * @fn kome::ident::IdentificationManager& getIdentificationManager()
			 * @brief gets identification manager object
			 * @return identification manager object
			 */
			kome::ident::IdentificationManager& getIdentificationManager();

		public:
			/**
			 * @fn void clearFocusedPeaks()
			 * @brief clears focused peaks
			 */
			void clearFocusedPeaks();

			/**
			 * @fn void addFocusedPeak( int peakId )
			 * @brief adds focused peak
			 * @param[in] peakId ID number of peak to be added to focused.
			 */
			void addFocusedPeak( int peakId );

			/**
			 * @fn void setFocusedPeak( int peakId )
			 * @brief sets the focused peak
			 * @param[in] peakId ID number of peak to be focused.
			 */
			void setFocusedPeak( int peakId );

			/**
			 * @fn void activatePeak( int peakId )
			 * @brief activates peak
			 * @param[in] peakId ID number of peak to be activated.
			 */
			void activatePeak( int peakId );

			/**
			 * @fn void removeFocusedPeak( int peakId )
			 * @brief removes the focused peak
			 * @param[in] peakId ID number of peak to be removed from focused.
			 */
			void removeFocusedPeak( int peakId );

			/**
			 * @fn cli::array< int >^ getFocusedPeaks()
			 * @brief gets the focused peak IDs.
			 * @return focused peak IDs.
			 */
			cli::array< int >^ getFocusedPeaks();

			/**
			 * @fn bool isFocusedPeak( int peakId )
			 * @brief judges whther the specified peak is focused or not.
			 * @param[in] peakId peak ID
			 * @return If true, the specified peak is focused.
			 */
			bool isFocusedPeak( int peakId );

		public:
			/**
			 * @fn bool writePepXML( System::String^ path, SearchResultWrapper^ srchResult, bool forTpp, ProgressWrapper^ progress )
			 * @brief write the specified search result to a pepXML file at the specified path
			 * @param[in] path output file path
			 * @param[in] srchResult search result
			 * @param[in] forTpp exports pepXML for TPP (PeptideProphet)
			 * @param[in,out] progress the object to show the progress
			 * @return If true, it succeeded to write the pepXML file.
			 */
			bool writePepXML( System::String^ path, SearchResultWrapper^ srchResult, bool forTpp, ProgressWrapper^ progress );

			/**
			 * @fn bool writeTsv( System::String^ path, SearchResultWrapper^ srchResult, ProgressWrapper^ progress )
			 * @brief write the specified search result to a TSV (tab-separated value) text file at the specified path
			 * @param[in] path output file path
			 * @param[in] srchResult search result
			 * @param[in,out] progress the object to show the progress
			 * @return If true, it succeeded to write the TSV file.
			 */
			bool writeTsv( System::String^ path, SearchResultWrapper^ srchResult, ProgressWrapper^ progress );

			/**
			 * @fn int getNumberOfLocalDbs()
			 * @brief get the number of installed local databases
			 * @return local database count
			 */
			int getNumberOfLocalDbs();

			/**
			 * @fn System::String^ getLocalDb( int index )
			 * @brief get the specified local database name
			 * @param[in] index local database index
			 * @return local database name
			 */
			System::String^ getLocalDb( int index );

			/**
			 * @fn int getNumberOfLocalDbFiles( int index )
			 * @brief get the number of files in the specified local database
			 * @param[in] index local database index
			 * @return file count
			 */
			int getNumberOfLocalDbFiles( int index );

			/**
			 * @fn System::String^ getLocalDbFileName( int dbIndex, int fileIndex )
			 * @brief get the filename of the specified file in the specified local database
			 * @param[in] dbIndex local database index
			 * @param[in] fileIndex file index
			 * @return file name
			 */
			System::String^ getLocalDbFileName( int dbIndex, int fileIndex );

			/**
			 * @fn int getLocalDbFileType( int dbIndex, int fileIndex )
			 * @brief get the type of the specified file in the specified local database
			 * @param[in] dbIndex local database index
			 * @param[in] fileIndex file index
			 * @return local database file type
			 */
			int getLocalDbFileType( int dbIndex, int fileIndex );

			/**
			 * @fn System::String^ getLocalDbFilePath( int dbIndex, int fileIndex, DbFileType fileType )
			 * @brief get the file path of the specified file and type in the specified local database
			 * @param[in] dbIndex local database index
			 * @param[in] fileIndex file index
			 * @param[in] fileType file type
			 * @return local database file path
			 */
			System::String^ getLocalDbFilePath( int dbIndex, int fileIndex, DbFileType fileType );

			/**
			 * @fn int addPublicDb( System::String^ dbName, System::String^ url )
			 * @brief add a public (protein, metabolite) database
			 * @param[in] dbName database name
			 * @param[in] url database URL
			 * @return database ID registered in the MSPP_DATABASE table
			 */
			int addPublicDb( System::String^ dbName, System::String^ url );

			/**
			 * @fn void setPublicUrl( int dbId, System::String^ format, System::String^ url )
			 * @brief set the URL for a public (protein, metabolite) database
			 * @param[in] dbId database ID
			 * @param[in] format format name
			 * @param[in] url database URL (string includes a %s to correspond with Accession)
			 */
			void setPublicUrl( int dbId, System::String^ format, System::String^ url );

			/**
			 * @fn unsigned int registerResult( SearchResultWrapper^ srchResult, ProgressWrapper^ prog )
			 * @brief register a search result in the internal database
			 * @param[in] srchResult SearchResult object
			 * @param[out] prog progress display object
			 * @return If 0, no warning occurred. Otherwise, it contains a warning code.
			 */
			unsigned int registerResult( SearchResultWrapper^ srchResult, ProgressWrapper^ prog );

			/**
			 * @fn unsigned int registerResult( int searchId, SearchResultWrapper^ srchResult, ProgressWrapper^ prog )
			 * @brief register a search result in the internal database
			 * @param[in] searchId search ID
			 * @param[in] srchResult SearchResult object
			 * @param[out] prog progress display object
			 * @return If 0, no warning occurred. Otherwise, it contains a warning code.
			 */
			unsigned int registerResult( int searchId, SearchResultWrapper^ srchResult, ProgressWrapper^ prog );

        public:
			/**
			 * @fn void loadEnzymes()
			 * @brief loads enzyme
			 */
			void loadEnzymes();

			/**
			 * @fn int getNumberOfEnzymes( void )
			 * @brief get the number of enzymes
			 * @return enzyme count
			 */
			int getNumberOfEnzymes( void );

			/**
			 * @fn EnzymeWrapper^ getEnzyme( int index )
			 * @brief get the specified enzyme information
			 * @param[in] index enzyme index
			 * @return Enzyme object
			 */
			EnzymeWrapper^ getEnzyme( int index );

			/**
			 * @fn EnzymeWrapper^ getEnzymeFromId( int id )
			 * @brief gets enzyme from ID
			 * @param[in] id enzyme ID
			 * @return enzyme
			 */
			EnzymeWrapper^ getEnzymeFromId( int id );

			/**
			 * @fn void setAllEnzymeHidden()
			 * @brief sets all enzymes hidden
			 */
			void setAllEnzymeHidden();

			/**
		 	 * @fn int updateEnzyme( EnzymeWrapper^ enzyme )
			 * @brief updates enzyme
			 * @param[in] enzyme enzyme object
			 * @return enzyme ID
			 */
			int updateEnzyme( EnzymeWrapper^ enzyme );

        public:
			/**
			 * @fn void loadModifications( void )
			 * @brief loads modification
			 */
 			void loadModifications( void );

			/**
			 * @fn int getNumberOfModifications( void )
			 * @brief get the number of modifications
			 * @return modification count
			 */
			int getNumberOfModifications( void );

			/**
			 * @fn ModificationWrapper^ getModification( int index )
			 * @brief get a pointer to the specified Modification object
			 * @param[in] index modification index
			 * @return pointer to a Modification object
			 */
			ModificationWrapper^ getModification( int index );

			/**
			 * @fn ModificationWrapper^ getModificationFromId( int id )
			 * @brief gets modification from ID
			 * @param[in] id modificatio nID
			 * @return modification
			 */
			ModificationWrapper^ getModificationFromId( int id );

			/**
			 * @fn void setAllModificationHidden()
			 * @brief sets all modification hidden
			 */
			void setAllModificationHidden();

			/**
			 * @fn int updateModification( ModificationWrapper^ modification )
			 * @breief updates modification 
			 * @param[in] modification modification object
			 * @return modification ID
			 */
			int updateModification( ModificationWrapper^ modification );

        public:
			/**
			 * @fn int addSearchModification( int search, int modification, bool isVariable )
			 * @brief adds search modification
			 * @param[in] search search ID
			 * @param[in] modification modification ID
			 * @param[in] isVariable If true, variable modification
			 * @return search modification ID
			 */
			int addSearchModification( int search, int modification, bool isVariable );

			/**
			 * @fn cli::array< int >^ getSearchModifications( int searchId );
			 * @brief gets search modifications from search ID
			 * @param[in] searchId search ID
			 * @return the array of search modification IDs.
			 */
			cli::array< int >^ getSearchModifications( int searchId );

			/**
			 * @fn int getModificationFromSearchModification( int searchModId )
			 * @brief gets the modification ID from search modification ID
			 * @param[in] searchModId search modification Id
			 * @return modification ID
			 */
			int getModificationFromSearchModification( int searchModId );

			/**
			 * @fn int getSearchFromSearchModification( int searchModId )
			 * @brief gets the search ID from search modificaiton ID
			 * @param[in] searchModId search modification ID
			 * @return search ID
			 */
			int getSearchFromSearchModification( int searchModId );

			/**
			 * @fn bool isVariableModification( int searchModId )
			 * @brief get the value the specified search modification is variable or not
			 * @retrun If true, variable modification
			 */
			bool isVariableModification( int searchModId );

       public:
			/**
			 * @fn int addHitModification( int hit, int searchMod, int pos )
			 * @brief adds hit modification
			 * @param[in] hit hid ID
			 * @param[in] searchMod search modification ID
			 * @param[in] pos position
			 */
			int addHitModification( int hit, int searchMod, int pos );

			/**
			 * @fn cli::array< int >^ getHitModifications( int hitId );
			 * @brief gets hit modifications 
			 * @param[in] hitId hit ID
			 * @return the array of hit modification IDs.
			 */
			cli::array< int >^ getHitModifications( int hitId );

			/**
			 * @fn int getSearchModificationFromHitModification( int hitMod )
			 * @brief gets the search modification from hit modification 
			 * @param[in] hitMod hit modification ID
			 * @return search modification ID
			 */
			int getSearchModificationFromHitModification( int hitMod );

			/**
			 * @fn int getHitFromHitModification( int hitMod )
			 * @brief gets the hit ID from hit modification ID
			 * @return hit ID
			 */
			int getHitFromHitModification( int hitMod );

			/**
			 * @fn int getHitModPosition( int hitMod )
			 * @brief gets the hit modification position
			 * @return hit modification position
			 */
			int getHitModPosition( int hitMod );

        public:
	        /**
			 * @fn bool ParseSpectrumTitle(System::String^ spectrumTitle, HitWrapper::SpecInfo^ spectrumInfo, int% charge)
			 * @brief parse the spectrum title string into a SpecInfo struct
			 * @param[in] spectrumTitle title string to parse
			 * @param[out] spectrumInfo spectrum information object
			 * @param[out] charge the pointer to store charge state
			 * @return If it failed to parse the spectrum title, this method returns false.
			 */
			bool ParseSpectrumTitle(System::String^ spectrumTitle, HitWrapper::SpecInfo^ spectrumInfo, int% charge);

			/**
			 * @fn System::String^ getDbHome()
			 * @brief gets the database home directory
			 * @return database home directory
			 */
			System::String^ getDbHome();

		public:
			/**
			 * @fn unsigned int InsufficientTitleTagsWarningCode()
			 * @brief return the warning code indicating insufficient spectrum title tags in result file
			 * @return the warning code.
			 */
			unsigned int InsufficientTitleTagsWarningCode();

			/**
			 * @fn unsigned int NoValidTitleTagsWarningCode()
			 * @brief return the warning code indicating no valid title tags in result file
			 * @return the warning code.
			 */
			unsigned int NoValidTitleTagsWarningCode();

			/**
			 * @fn unsigned int NoValidHitSpectraWarningCode()
			 * @brief return the warning code indicating no valid hit spectra in result file
			 * @return the warning code.
			 */
			unsigned int NoValidHitSpectraWarningCode();

			/**
			 * @fn unsigned int SearchResultAdditionWarningCode()
			 * @brief return the warning code indicating a search result could not be added
			 * @return the warning code.
			 */
			unsigned int SearchResultAdditionWarningCode();

        public:
			/**
			 * @fn void addSearchEngine( SearchEngineWrapper^ pSrchEngine )
			 * @brief add a search engine
			 * @param[in] pSrchEngine pointer to a SearchEngine object
			 */
			void addSearchEngine( SearchEngineWrapper^ pSrchEngine );

			/**
			 * @fn int getNumberOfSearchEngines()
			 * @brief get the number of search engines
			 * @return number of search engines
			 */
			int getNumberOfSearchEngines();

			/**
			 * @fn SearchEngineWrapper^ getSearchEngine( int index )
			 * @brief get the specified search engine
			 * @param[in] index search engine index
			 * @return search engine
			 */
			SearchEngineWrapper^ getSearchEngine( int index );

			/**
			 * @fn bool removeSearchEngine( SearchEngineWrapper^ pSrchEngine )
			 * @brief remove a search engine
			 * @param[in] pSrchEngine pointer to a SearchEngine object
			 * @return If true, it succeeded to remove the search engine.
			 */
			bool removeSearchEngine( SearchEngineWrapper^ pSrchEngine );

        public:
			/**
			 * @fn void commit()
			 * @brief commits the database
			 */
			void commit();

			/**
			 * @fn void rollback()
			 * @brief rollbacks the database
			 */
			void rollback();

        public:
			/**
			 * @fn int addMatrix(
					    System::String^ name,
						System::String^ comment,
						bool rtFlg,
						bool mzFlg,
						bool chargeFlg
					)
			 * @brief adds peak matrix
			 * @param[in] name matrix name
			 * @param[in] comment comment
			 * @param[in] rtFlg RT flag value
			 * @param[in] mzFlg m/z flag value
			 * @param[in] chargeFlg charge flag value
			 * @return matrix ID
			 */
			int addMatrix(
				    System::String^ name,
					System::String^ comment,
					bool rtFlg,
					bool mzFlg,
					bool chargeFlg
			);

			/**
			 * @fn void deleteMatrix( int id )
			 * @brief deletes matrix
			 * @param[in] id matrix ID
			 */
			void deleteMatrix( int id );

			/**
			 * @fn int getSelectedMatrix()
			 * @brief gets the selected matrix
			 * @return selected matrix ID
			 */
			int getSelectedMatrix();

			/**
			 * @fn void selectMatrix( int id )
			 * @brief selects matrix
			 * @param[in] id matrix ID
			 */
			void selectMatrix( int id );

			/**
			 * @fn void updateMatrixFlags( int id, bool rtFlg, bool mzFlg, bool chargeFlg )
			 * @brief updates matrix flags
			 * @param[in] id matrix ID
			 * @param[in] rtFlg RT flag value
			 * @param[in] mzFlg m/z flag value
			 * @param[in] chargeFlg charge flag value
			 */
			void updateMatrixFlags( int id, bool rtFlg, bool mzFlg, bool chargeFlg );

			/**
			 * @fn void updateMatrix( int id, System::String^ name, System::String^ comment )
			 * @brief updates matrix
			 * @param[in] id matrix ID
			 * @param[in] name matrix name
			 * @param[in] comment comment
			 */
			void updateMatrix( int id, System::String^ name, System::String^ comment );

			/**
			 * @fn cli::array< int >^ getMatrices()
			 * @brief gets matrices
			 * @return the array of matrix IDs.
			 */
			cli::array< int >^ getMatrices();

			/**
			 * @fn System::String^ getMatrixName( int id )
			 * @brief gets the matrix name
			 * @param[in] id matrix ID
			 * @return matrix name
			 */
			System::String^ getMatrixName( int id );

			/**
			 * @fn System::String^ getMatrixComment( int id )
			 * @brief gets the matrix comment
			 * @param[in] id matrix ID
			 * @return matrix comment
			 */
			System::String^ getMatrixComment( int id );

			/**
			 * @fn void getMatrixFlags( int id, bool% rtFlg, bool% mzFlg, bool% chargeFlg )
			 * @brief gets matrix flags
			 * @param[in] id matrix ID
			 * @param[out] rtFlg the pointer to store rt flag value
			 * @param[out] mzFlg the pointer to store m/z flag value
			 * @param[out] chargeFlg the pointer to store charge flag value
			 */
			void getMatrixFlags( int id, bool% rtFlg, bool% mzFlg, bool% chargeFlg );

        public:
			/**
			 * @fn int addGroup( System::String^ name, int matrix, System::Drawing::Color color )
			 * @brief adds group
			 * @param[in] name group name
			 * @param[in] matrix peak matrix ID
			 * @param[in] color group color
			 * @return group ID
			 */
			int addGroup( System::String^ name, int matrix, System::Drawing::Color color );

			/**
			 * @fn void deleteGroup( int id )
			 * @brief deletes group 
			 * @param[in] id group ID
			 */
			void deleteGroup( int id );

			/**
			 * @fn void updateGroup( int id, System::String^ name, System::Drawing::Color color )
			 * @brief updates group
			 * @param[in] id group ID
			 * @param[in] name group name
			 * @param[in] color group color
			 */
			void updateGroup( int id, System::String^ name, System::Drawing::Color color );

			/**
			 * @fn void setGroupOrder( int id, int order )
			 * @brief sets group order
			 * @param[in] id group ID
			 * @param[in] order group order
			 */
			void setGroupOrder( int id, int order );

			/**
			 * @fn cli::array< int >^ getGroups( int matrixId )
			 * @brief gets groups
			 * @param[in] matrixId peak matrix ID
			 * @return the array of group IDs.
			 */
			cli::array< int >^ getGroups( int matrixId );

			/**
			 * @fn int getGroupMatrix( int id )
			 * @brief gets the matrix ID of the specified group
			 * @param[in] id group ID
			 * @return matrix ID
			 */
			int getGroupMatrix( int id );

			/**
			 * @fn System::String^ getGroupName( int id )
			 * @brief gets the group name
			 * @param[in] id group ID
			 */
			System::String^ getGroupName( int id );

			/**
			 * @fn System::Drawing::Color getGroupColor( int id )
			 * @brief gets the group color
			 * @param[in] id group ID
			 * @return group color
			 */
			System::Drawing::Color getGroupColor( int id );

        public:
			/**
			 * @fn void setStandardSample( int matrixId, int  matrixSampleId )
			 * @brief sets the standard sample
			 * @param[in] matrixId peak matrix ID
			 * @param[in] matrixSampleId peak matrix sample ID
			 */
			void setStandardSample( int matrixId, int  matrixSampleId );

			/**
			 * @fn int getStandardSample( int matrixId )
			 * @brief gets the standard sample
			 * @param[in] matrixId peak matrix ID
			 * @return standard peak matrix sample ID
			 */
			int getStandardSample( int matrixId );

        public:
			/**
			 * @fn void clearNormalization( int matrix )
			 * @brief clears normalization
			 * @param matrix peak matrix ID
			 */
			void clearNormalization( int matrix );

			/**
			 * @fn void setNormalizationFilter(
						int matrix,
						double minInt,
						double maxInt,
						bool relFlg,
						double minRt,
						double maxRt,
						double minMz,
						double maxMz,
						System::String^ method,
						System::String^ params
					)
			 * @brief sets the normalization filter
			 * @param[in] matrix peak matrix ID
			 * @param[in] minInt min intensity
			 * @param[in] maxInt max intensity
			 * @param[in] relFlg relative flag. (true: Relative, false: Absolute)
			 * @param[in] minRt min RT
			 * @param[in] maxRt max RT
			 * @param[in] minMz min m/z
			 * @param[in] maxMz max m/z
			 * @param[in] method filter method name
			 * @param[in] params filter parameters
			 */
			void setNormalizationFilter(
					int matrix,
					double minInt,
					double maxInt,
					bool relFlg,
					double minRt,
					double maxRt,
					double minMz,
					double maxMz,
					System::String^ method,
					System::String^ params
			);

			/**
			 * @fn NormalizationFilterInfo^ getNormalizationFilter( int matrix )
			 * @brief gets the normalization filter information
			 * @param[in] matrix peak matrix ID
			 * @return normalization filter information
			 */
			NormalizationFilterInfo^ getNormalizationFilter( int matrix );

			/**
			 * @fn void addNormalizationMethod(
						int matrix,
						System::String^ method,
						System::String^ params
					)
			 * @brief adds normalization method
			 * @param[in] matrix peak matrix ID
			 * @param[in] method normalization method name
			 * @param[in] params normalization parameters
			 */
			void addNormalizationMethod(
				int matrix,
				System::String^ method,
				System::String^ params
			);

			/**
			 * @fn cli::array< NormalizationMethodInfo^ >^ getNormalizationMethods( int matrix )
			 * @brief gets the normalization method list
			 * @param[in] matrix peak matrix ID
			 * @return array object to store normalization method information
			 */
			cli::array< NormalizationMethodInfo^ >^ getNormalizationMethods( int matrix );

        public:
			/**
			 * @fn void setAlignment( int matrix, System::String^ method, System::String^ params )
			 * @brief sets the Alignment information
			 * @param[in] matrix peak matrix ID
			 * @param[in] method alignment method
			 * @param[in] params alignment parameters
			 */
			void setAlignment( int matrix, System::String^ method, System::String^ params );

			/**
			 * @fn AlignmentWrapper^ getAlignment( int matrix )
			 * @brief gets the Alignment information
			 * @param[in] matrix peak matrix ID
			 * @return Alignment object
			 */
			AlignmentWrapper^ getAlignment( int matrix );
        
        public:
			/**
			 * @fn int addMatrixColumn( System::String^ name, ColumnDataType type, int matrixId )
			 * @brief adds matrix column
			 * @param[in] name column name
			 * @param[in] type data type
			 * @param[in] matrixId matrix ID
			 * @return matrix column ID
			 */
			int addMatrixColumn( System::String^ name, ColumnDataType type, int matrixId );

			/**
			 * @fn void deleteMatrixColumn( int id )
			 * @brief deletes matrix column
			 * @param[in] id matrix column ID
			 */
			void deleteMatrixColumn( int id );

			/**
			 * @fn void updateMatrixColumn( int id, System::String^ name, ColumnDataType type )
			 * @brief updates column
			 * @param[in] id column ID
			 * @param[in] name column name
			 * @param[in] type data type
			 */
			void updateMatrixColumn( int id, System::String^ name, ColumnDataType type );

			/**
			 * @fn cli::array< int >^ getMatrixColumns( int matrixId )
			 * @brief get matrix columns
			 * @param[in] matrixId matrix ID
			 * @return the array of column IDs.
			 */
			cli::array< int >^ getMatrixColumns( int matrixId );

			/**
			 * @fn System::String^ getMatrixColumnName( int id )
			 * @brief gets the matrix column name
			 * @param[in] id matrix column ID
			 * @return matrix column name
			 */
			System::String^ getMatrixColumnName( int id );

			/**
			 * @fn ColumnDataType getMatrixColumnType( int id )
			 * @brief gets the matrix column data type
			 * @param[in] id matrix column ID
			 * @return matrix column data type
			 */
			ColumnDataType getMatrixColumnType( int id );

        public:
			/**
			 * @fn int addMatrixPeak( double rt, double mz, int charge, int matrixId )
			 * @brief adds matrix peak
			 * @param[in] rt RT position of peak
			 * @param[in] mz m/z position of peak
			 * @param[in] charge charge of peak
			 * @param[in] matrixId matrix ID
			 * @return peak ID
			 */
			int addMatrixPeak( double rt, double mz, int charge, int matrixId );

			/**
			 * @fn void deleteMatrixPeak( int id )
			 * @brief deletes matrix peak 
			 * @param[in] id peak ID
			 */
			void deleteMatrixPeak( int id );

			/**
			 * @fn void updateMatrixPeak( int id, double rt, double mz, int charge )
			 * @brief updates matrix peak 
			 * @param[in] id peak ID
			 * @param[in] rt RT position of peak
			 * @param[in] mz m/z position of peak
			 * @param[in] charge charge of peak
			 */
			void updateMatrixPeak( int id, double rt, double mz, int charge );

			/**
			 * @fn cli::array< int >^ getMatrixPeaks( int matrixId )
			 * @brief gets matrix peaks
			 * @param[in] matrixId peak matrix ID
			 * @return peak IDs.
			 */
			cli::array< int >^ getMatrixPeaks( int matrixId );

			/**
			 * @fn double getMatrixPeakRt( int id )
			 * @brief get the RT position of peak
			 * @param[in] id peak ID
			 * @return RT position of peak
			 */
			double getMatrixPeakRt( int id );

			/**
			 * @fn double getMatrixPeakMz( int id )
			 * @brief get the m/z position of peak
			 * @param[in] id peak ID
			 * @return RT position of peak
			 */
			double getMatrixPeakMz( int id );

			/**
			 * @fn int getMatrixPeakCharge( int id )
			 * @brief get the peak charge
			 * @param[in] id peak ID
			 * @return peak charge
			 */
			int getMatrixPeakCharge( int id );

        public:
			/**
			 * @fn int addSampleSet( System::String^ path )
			 * @brief adds sample set
			 * @param[in] path sample set file path
			 * @return sample set ID
			 */
			int addSampleSet( System::String^ path );

			/**
			 * @fn int getSampleSet( System::String^ path )
			 * @brief gets the sample set ID
			 * @parma[in] path sample set file path
			 * @return sample set ID. If negative value, the specified sample set is not contained in the internal database.
			 */
			int getSampleSet( System::String^ path );

        public:
			/**
			 * @fn int addSample( SampleWrapper^ sample )
			 * @brief adds sample
			 * @param[in] sample sample object
			 * return sample ID
			 */
			int addSample( SampleWrapper^ sample );

			/**
			 * @fn int addSample( System::String^ path, int sampleIndex, System::String^ name )
			 * @brief adds sample
			 * @param[in] path sample data file path
			 * @param[in] sampleIndex sample index
			 * @param[in] name sample name
			 */
			int addSample( System::String^ path, int sampleIndex, System::String^ name );

			/**
			 * @fn int getSample( SampleWrapper^ sample )
			 * @brief gets sample ID
			 * @param[in] sample sample
			 * @return sample ID. If negative value, the specified sample is not contained in the internal database.
			 */
			int getSample( SampleWrapper^ sample );

			/**
			 * @fn int getSample( System::String^ path, int sampleIndex );
			 * @brief gets sample ID
			 * @param[in] path sample data file path
			 * @param[in] sampleIndex sample index
			 * @return sample ID. If negative value, the specified sample is not contained in the internal database.
			 */
			int getSample( System::String^ path, int sampleIndex );

			/**
			 * @fn void deleteSample( int id )
			 * @brief deletes sample
			 * @param[in] id sample ID
			 */
			void deleteSample( int id );

			/**
			 * @fn System::String^ getSampleName( int sampleId )
			 * @brief gets the sample name
			 * @param[in] sampleId sample ID
			 * @return sample name
			 */
			System::String^ getSampleName( int sampleId );

			/**
			 * @fn System::String^ getSampleFilePath( int sampleId )
			 * @brief gets the sample file path
			 * @param[in] sampleId sample ID
			 * return sample file path
			 */
			System::String^ getSampleFilePath( int sampleId );

			/**
			 * @fn int getSampleIndex( int sampleId )
			 * @brief gets the sample index
			 * @param[in] sampleId sample ID
			 * @return sample index
			 */
			int getSampleIndex( int sampleId );

			/**
			 * @fn SampleWrapper^ openSample( int id, SampleReadInfo^ readInfo )
			 * @brief opens sample
			 * @param[in] id sample ID
			 * @param[out] readInfo sample open information. (This struct is used in releaseSample method.)
			 */
			SampleWrapper^ openSample( int id, SampleReadInfo^ readInfo );

			/**
			 * @fn void releaseSample( SampleWrapper^ sample, SampleReadInfo^ readInfo )
			 * @brief releases sample
			 * @param[in] sample sample object
			 * @param[in] readInfo release informatin
			 */
			void releaseSample( SampleWrapper^ sample, SampleReadInfo^ readInfo );

		public:
			/**
			 * @fn void copySampleInfo( SampleReadInfo^ readInfo, kome::ident::SampleReadInfo& dst )
			 * @brief copy sample info
			 * @param[in] readInfo
			 * @param[out] dst
			 */
			void copySampleInfo( SampleReadInfo^ readInfo, kome::ident::SampleReadInfo& dst );

			/**
			 * @fn SampleReadInfo^ copySampleInfo( kome::ident::SampleReadInfo* readInfo )
			 * @brief copies sample read info
			 * @parma[in] readInfo source of sample read information
			 * @return copied sample read information
			 */
			SampleReadInfo^ copySampleInfo( kome::ident::SampleReadInfo* readInfo );

			/**
			 * @fn void copySubstance( SubstanceInfo^ substance, kome::ident::SubstanceInfo& dst )
			 * @brief copy substance info
			 * @param[in] substance
			 * @param[oub] dst
			 */
			void copySubstance( SubstanceInfo^ substance, kome::ident::SubstanceInfo& dst );

			/**
			 * @fn SubstanceInfo^ copySubstance( kome::ident::SubstanceInfo* substance )
			 * @brief copies substance information
			 * @parma[in] substance source of substance information
			 * @return copied substance information
			 */
			SubstanceInfo^ copySubstance( kome::ident::SubstanceInfo* substance );

			/**
			 * @fn void copyHitInfo( HitInfo^ hit, kome::ident::HitInfo& dst )
			 * @brief copy hit info
			 * @param[in] hit
			 * @param[out] dst
			 */
			void copyHitInfo( HitInfo^ hit, kome::ident::HitInfo& dst );

			/**
			 * @fn HitInfo^ copyHitInfo( kome::ident::HitInfo* hit )
			 * @brief copies hit information
			 * @param[in] hit source of hit information
			 * @return copied hit information
			 */
			HitInfo^ copyHitInfo( kome::ident::HitInfo* hit );

        public:
			/**
			 * @fn void addMatrixSample(
						int group,
						int sample,
						System::String^ name,
						System::Drawing::Color color
					)
			 * @brief adds matrix sample
			 * @param[in] group group ID
			 * @param[in] sample sample ID
			 * @param[in] name matrix column name
			 * @param[in] color sample color
			 * @return matrix sample ID
			 */
			int addMatrixSample(
				int group,
				int sample,
				System::String^ name,
				System::Drawing::Color color
			);

			/**
			 * @fn void deleteMatrixSample( int id )
			 * @brief deletes matrix sample
			 * @param[in] id matrix sample ID
			 */
			void deleteMatrixSample( int id );

			/**
			 * @fn void updateMatrixSample( int id, System::String^ name, System::Drawing::Color color )
			 * @brief updates matrix sample
			 * @param[in] id matrix sample ID
			 * @param[in] name matrix sample name
			 * @param[in] color matrix sample color
			 */
			void updateMatrixSample( int id, System::String^ name, System::Drawing::Color color );

			/**
			 * @fn cli::array< int >^ getMatrixSamplesFromGroup( int group )
			 * @brief gets matrix samples of specified group
			 * @param[in] group group ID
			 * @return the array of sapmle IDs.
			 */
			cli::array< int >^ getMatrixSamplesFromGroup( int group );

			/**
			 * @fn cli::array< int >^ getMatrixSamplesFromSample( int sample )
			 * @brief gets matrix samples of specified sample
			 * @param[in] sample sample ID
			 * @return the array of sapmle IDs.
			 */
			cli::array< int >^ getMatrixSamplesFromSample( int sample );

			/**
			 * @fn int getMatrixSampleSample( int id )
			 * @brief gets sample ID of the matrix sample
			 * @param[in] id matrix sample ID
			 * @return sample ID
			 */
			int getMatrixSampleSample( int id );

			/**
			 * @fn int getMatrixSampleGroup( int id )
			 * @brief gets group ID of the matrix sample
			 * @param[in] id matrix sample ID
			 * @return group ID
			 */
			int getMatrixSampleGroup( int id );

			/**
			 * @fn System::String^ getMatrixSampleName( int id )
			 * @brief return get matrix sample name
			 * @param[in] id matrix sample ID
			 * @return matrix sample name
			 */
			System::String^ getMatrixSampleName( int id );

			/**
			 * @fn System::Drawing::Color getMatrixSampleColor( int id )
			 * @brief gets the matrix sample color
			 * @param[in] id matrix sample ID
			 * @return matrix sample color
			 */
			System::Drawing::Color getMatrixSampleColor( int id );

        public:
			/**
			 * @fn void setAppendedValue( int peak, int col, System::String^ val )
			 * @brief sets appended value
			 * @param[in] peak peak ID
			 * @param[in] col column ID
			 * @param[in] val value
			 */
			void setAppendedValue( int peak, int col, System::String^ val );

			/**
			 * @fn System::String^ getAppendedValue( int peak, int col )
			 * @brief gets the appended value
			 * @param[in] peak peak ID
			 * @param[in] col column ID
			 * @return appended value
			 */
			System::String^ getAppendedValue( int peak, int col );

			/**                   
			 * @fn void deleteAppendedValueFromPeak( int peak )
			 * @brief deletes appended value from peak
			 * @param[in] peak peak ID
			 */
			void deleteAppendedValueFromPeak( int peak );

			/**
			 * @fn void deleteAppendedValueFromMatrixColumn( int col )
			 * @brief deletes appended value from matrix column
			 * @param[in] col column ID
			 */
			void deleteAppendedValueFromMatrixColumn( int col );

        public:
			/**
			 * @fn int setProfile( XYDataWrapper^ data, bool isChrom )
			 * @brief sets the profile
			 * @param[in] data data points
			 * @param[in] isChrom chromatogram flag.
			 * @return profile ID
			 */
			int setProfile( XYDataWrapper^ data, bool isChrom );

			/**
			 * @fn void deleteProfile( int id )
			 * @brief deletes profile
			 * @param[in] id profile ID
			 */
			void deleteProfile( int id );

			/**
			 * @fn void getProfile( int id, XYDataWrapper^ data )
			 * @brief gets the profile
			 * @param[in] id profile ID
			 * @param[out] data the object to store profile data
			 */
			void getProfile( int id, XYDataWrapper^ data );

			/**
			 * @fn bool isChromatogramProfile( int id )
			 * @brief gets the chromatogram flag value
			 * @param[in] id profile ID
			 * @return chromatogram flag value
			 */
			bool isChromatogramProfile( int id );

        public:
			/**
			 * @fn void setPeakValue(
						int matrixPeak,
						int matrixSample,
						double intensity,
						PeakElementWrapper^ peak,
						int profile
				);
			 * @brief sets the peak value
			 * @param[in] matrixPeak matrix peak ID
			 * @param[in] matrixSample matrix sample ID
			 * @param[in] intensity peak intensity
			 * @param[in] peak peak information
			 * @param[in] profile profile ID
			 */
			void setPeakValue(
				int matrixPeak,
				int matrixSample,
				double intensity,
				PeakElementWrapper^ peak,
				int profile
			 );

			/**
			 * @fn void setNormalizedIntensity(
					int matrixPeak,
					int matrixSample,
					double normalized
				)
			 * @brief sets the normalized intensity
			 * @param[in] matrixPeak matrix peak ID
			 * @param[in] matrixSample matrix sample ID
			 * @param[in] normalized normalized intensity
			 */
			void setNormalizedIntensity(
				int matrixPeak,
				int matrixSample,
				double normalized
			);

			/**
			 * @fn void deletePeakValuesFromMatrixPeak( int matrixPeak )
			 * @brief delete peak values of specified peak
			 * @param[in] matrixPeak matrix peak ID
			 */
			void deletePeakValuesFromMatrixPeak( int matrixPeak );

			/**
			 * @fn void deletePeakValuesFromMatrixSample( int matrixSample )
			 * @brief deletes peak values of specified sample
			 * @param[in] matrixSample matrix sample ID
			 */
			void deletePeakValuesFromMatrixSample( int matrixSample );

			/**
			 * @fn double getPeakValue( int matrixPeak, int matrixSample )
			 * @brief getst the peak value
			 * @param[in] matrixPeak matrix peak ID
			 * @param[in] matrixSample matrix sample ID
			 * @return peak value
			 */
			double getPeakValue( int matrixPeak, int matrixSample );

			/**
			 * @fn double getNormalizedPeakValue( int matrixPeak, int matrixSample )
			 * @brief gets the normalized peak value
			 * @param[in] matrixPeak matrix peak ID
			 * @param[in] matrixSample matrix sample ID
			 * @return normalized peak value
			 */
			double getNormalizedPeakValue( int matrixPeak, int matrixSample );

			/**
			 * @fn bool hasBlankValue( int matrixId )
			 * @brief judges the specified peak matrix has blank peak value.
			 * @param[in] matrixId peak matrix ID
			 * @return If true, the specified peak matrix has blank peak value
			 */
			bool hasBlankValue( int matrixId );

			/**
			 * @fn bool isEmptySample( int matrixSample )
			 * @brief judges the column of the specified sample is empty or not.
			 * @param[in] matrixSample matrix sample ID
			 * @return If true, the column of the specified sample is empty.
			 */
			 bool isEmptySample( int matrixSample );

			/**
			 * @fn void getPeakInfo(
					 int matrixPeak,
					 int matrixSample,
					 PeakElementWrapper^ peak
				 )
			 * @brief gets the peak information
			 * @param[in] matrixPeak  matrix peak ID
			 * @param[in] matrixSample matrix sample ID
			 * @param[out] peak the object to store peak information
			 */
			 void getPeakInfo(
				 int matrixPeak,
				 int matrixSample,
				 PeakElementWrapper^ peak
			 );

			/**
			 * @fn int getProfile( int matrixPeak, int matrixSample )
			 * @brief getst the profile
			 * @param[in] matrixPeak  matrix peak ID
			 * @param[in] matrixSample matrix sample ID
			 * @return profile ID
			 */
			int getProfile( int matrixPeak, int matrixSample );

        public:
			/**
			 * @fn HitWrapper::SpecInfo^ getSpecInfo( SpectrumWrapper^ spec )
			 * @brief gets SpecInfo structure
			 * @param[in] spec spectrum
			 * @return spectrum information.
			 */
			HitWrapper::SpecInfo^ getSpecInfo( SpectrumWrapper^ spec );

			/**
			 * @fn int addSpectrum( SpectrumWrapper^ spec )
			 * @brief adds spectrum
			 * @param[in] spec spectrum
			 * @return spectrum ID
			 */
			int addSpectrum( SpectrumWrapper^ spec );

			/**
			 * @fn int addSpectrum( HitWrapper::SpecInfo^ spec )
			 * @brief adds spectrum
			 * @param[in] spec spectrum
			 * @return spectrum ID
			 */
			int addSpectrum( HitWrapper::SpecInfo^ spec );

			/**
			 * @fn int deleteSpectrum( int id )
			 * @brief adds spectrum
			 * @param[in] id spectrum ID
			 */
			void deleteSpectrum( int id );

			/**
			 * @fn cli::array< int >^ getSpectra( int sampleId )
			 * @brief gets spectra
			 * @param[in] sampleId sample ID
			 * @return the array of spectrum IDs
			 */
			cli::array< int >^ getSpectra( int sampleId );

			/**
			 * @fn HitWrapper::SpecInfo^ getSpectrum( int id );
			 * @brief gets the spectrum information
			 * @param[in] id spectrum ID
			 * @return spectrum infromation.
			 */
			HitWrapper::SpecInfo^ getSpectrum( int id );

        public:
			/**
			 * @fn int addDatabase( System::String^ name, System::String^ url )
			 * @brief adds databases
			 * @param[in] name database name
			 * @param[in] url database URL
			 * @return datatbase ID
			 */
			int addDatabase( System::String^ name, System::String^ url );

			/**
			 * @fn int getDatabaseId( System::String^ name )
			 * @brief gets the database ID
			 * @param[in] name database name
			 */
			int getDatabaseId( System::String^ name );

			/**
			 * @fn void deleteDatabase( int id )
			 * @brief deletes database
			 * @param[in] id database ID
			 */
			void deleteDatabase( int id );

			/**
			 * @fn cli::array< int >^ getDatabases()
			 * @brief gets database IDs.
			 * @return the array of database IDs.
			 */
			cli::array< int >^ getDatabases();

			/**
			 * @fn System::String^ getDatabaseName( int id )
			 * @brief gets the database name
			 * @param[in] id database ID
			 * @return database name
			 */
			System::String^ getDatabaseName( int id );

			/**
			 * @fn System::String^ getDatabaseUrl( int id )
			 * @brief gets the database URL
			 * @param[in] id database ID
			 * @return database URL
			 */
			System::String^ getDatabaseUrl( int id );

        public:
			/**
			 * @fn void setFormatUrl( int dbId, System::String^ format, System::String^ url )
			 * @brief set format URL
			 * @param[in] dbId database ID
			 * @param[in] format format name
			 * @param[in] url format URL
			 */
			void setFormatUrl( int dbId, System::String^ format, System::String^ url );

			/**
			 * @fn void deleteFormat( System::String^ format )
			 * @brief  delete format
			 * @param[in] format format name
			 */
			void deleteFormat( System::String^ format );

			/**
			 * @fn void deleteFormatUrl( int dbId, System::String^ format )
			 * @brief deletes format URL
			 * @param[in] dbId database ID
			 * @param[in] format format name
			 */
			void deleteFormatUrl( int dbId, System::String^ format );

			/**
			 * @fn void deleteFormatUrl( int dbId )
			 * @brief deletes fromat URL
			 * @param[in] dbId database ID
			 */
			 void deleteFormatUrl( int dbId );

			 /**
			  * @fn System::String^ getFormatUrl( int dbId, System::String^ format )
			  * @brief gets format URL
			  * @param[in] dbId database ID
			  * @param[in] format format name
			  * @return format URL
			  */
			System::String^ getFormatUrl( int dbId, System::String^ format );

			/**
			 * @fn cli::array< System::String^ >^ getFormatList( int dbId )
			 * @brief gets format list
			 * @param[in] dbId database ID
			 * @return the array of format IDs.
			 */
			cli::array< System::String^ >^ getFormatList( int dbId );

        public:
			/**
			 * @fn int addSubstance( SubstanceInfo^ substance )
			 * @brief adds substance
			 * @param[in] substance information
			 * @return substance ID
			 */
			int addSubstance( SubstanceInfo^ substance );

			/**
			 * @fn int getSubstanceId(
						int dbId,
						System::String^ accession,
						System::String^ name,
						int parentId
					)
			 * @brief gets substance ID
			 * @param[in] dbId database ID
			 * @param[in] accession accession
			 * @param[in] name substance name
			 * @param[in] parentId parent ID
			 * @return substance ID
			 */
			int getSubstanceId(
					int dbId,
					System::String^ accession,
					System::String^ name,
					int parentId
			);

			/**
			 * @fn void deleteSubstance( int id )
			 * @brief deletes substance
			 * @param[in] id substance ID
			 */
			void deleteSubstance( int id );

			/**
			 * @fn cli::array< int >^ getSubstances( int dbId )
			 * @brief gets substance IDs
			 * @param[in] dbId database ID
			 * @return the array of substance IDs.
			 */
			cli::array< int >^ getSubstances( int dbId );

			/**
			 * @fn cli::array< int >^ getSubstances()
			 * @brief gets substance IDs
			 * @return the array of substance IDs.
			 */
			cli::array< int >^ getSubstances();

			/**
			 * @fn SubstanceInfo^ getSubstance( int id )
			 * @brief gets substance
			 * @param[in] id substance ID
			 * @return substance information
			 */
			SubstanceInfo^ getSubstance( int id );
			
        public:
			/**
			 * @fn int addSearchEngine( System::String^ name, System::String^ version )
			 * @brief adds search engine
			 * @param[in] name search engine name
			 * @param[in] version search engine version
			 * @return search engine ID
			 */
			int addSearchEngine( System::String^ name, System::String^ version );

			/**
			 * @fn void deleteSearchEngine( int id )
			 * @brief deletes search engine
			 * @param[in] id search engine ID
			 */
			void deleteSearchEngine( int id );

			/**
			 * @fn cli::array< int >^ getSearchEngines()
			 * @brief gets search engines
			 * @return the array of search engine IDs.
			 */
			cli::array< int >^ getSearchEngines();

			/**
			 * @fn System::String^ getSearchEngineName( int id )
			 * @brief gets the search engine name
			 * @param[in] id search engine ID
			 * @return search engine name
			 */
			System::String^ getSearchEngineName( int id );

			/**
			 * @fn System::String^ getSearchEngineVersion( int id )
			 * @brief search engine version
			 * @param[in] id search engine ID
			 * @return search engine version
			 */
			System::String^ getSearchEngineVersion( int id );

        public:
			/**
			 * @fn int addPeakDetection( System::String^ method )
			 * @brief adds peak detection
			 * @param[in] method peak detection method
			 * @return peak detection ID
			 */
			int addPeakDetection( System::String^ method );

			/**
			 * @fn void setPeakParameter( int detect, System::String^ name, System::String^ value )
			 * @brief sets peak parameters
			 * @param[in] detect peak detection ID
			 * @param[in] name parameter name
			 * @param[in] value parameter value
			 */
			void setPeakParameter( int detect, System::String^ name, System::String^ value );

			/**
			 * @fn void deletePeakDetection( int detect )
			 * @brief deleted peak detection
			 * @param[in] detect peak detection ID
			 */
			void deletePeakDetection( int detect );

			/**
			 * @fn System::String^ getPeakDetectionMethod( int detect )
			 * @brief gets the peak detection method
			 * @param[in] detect peak detection ID
			 * @return peak detection method
			 */
			System::String^ getPeakDetectionMethod( int detect );

			/**
			 * @fn PropertiesWrapper^ getPeakDetectionParameters( int detect )
			 * @brief gets peak detection parameters
			 * @param[in] detect peak detection ID
			 * @return peak detection parameters
			 */
			PropertiesWrapper^ getPeakDetectionParameters( int detect );

			/**
			 * @fn void deleteUnlinkedPeakDetection()
			 * @brief deletes unlinked peak detection
			 */
			void deleteUnlinkedPeakDetection();

		public:
			/**
			 * @fn int setPeaks( XYDataWrapper^ peaks, int peakDetection )
			 * @brief set peaks
			 * @param[in] peaks peaks
			 * @param[in] peakDetection peak detection ID
			 * @return peaks ID
			 */
			int setPeaks( XYDataWrapper^ peaks, int peakDetection );

			/**
			 * @fn int getPeaks( int peaksId, XYDataWrapper^ peaks )
			 * @brief gets peaks
			 * @param[in] peaksId peaks ID
			 * @param[out] peaks the object to store peaks information.
			 * @return peak detection ID
			 */
			int getPeaks( int peaksId, XYDataWrapper^ peaks );

			/**
			 * @fn void deletePeaks( int peaks )
			 * @brief deletes peaks
			 * @param[in] peaks peaks ID
			 */
			void deletePeaks( int peaks );

			/**
			 * @fn void deleteUnlinkedPeaks()
			 * @brief deletes unlinked peaks
			 */
			void deleteUnlinkedPeaks();

        public:
			/**
			 * @fn int addSearch( SearchEngineWrapper::SearchInfo^ search )
			 * @brief adds search
			 * @param[in] search search information
			 * @return search ID
			 */
			int addSearch( SearchEngineWrapper::SearchInfo^ search );

			/**
			 * @fn void deleteSearch( int id )
			 * @brief deletes search
			 * @param[in] id search ID
			 */
			void deleteSearch( int id );

			/**
			 * @fn void updateSearch( int id, SearchEngineWrapper::SearchInfo^ search )
			 * @brief updates search
			 * @param[in] id search ID
			 * @param[in] search search information
			 */
			void updateSearch( int id, SearchEngineWrapper::SearchInfo^ search );

			/**
			 * @fn void setSearchStatus( int id, SearchEngineWrapper::SearchStatus status )
			 * @brief sets the search status
			 * @param[in] id search ID
			 * @param[in] status search status
			 */
			void setSearchStatus( int id, SearchEngineWrapper::SearchStatus status );

			/**
			 * @fn SearchEngineWrapper::SearchStatus getSearchStatus( int id )
			 * @brief gets the search status
			 * @parma[in] id search ID
			 * @return search status
			 */
			SearchEngineWrapper::SearchStatus getSearchStatus( int id );

			/**
			 * @fn void setResultLocation( int id, System::String^ location )
			 * @brief sets the result location
			 * @param[in] id search ID
			 * @param[in] location
			 */
			void setResultLocation( int id, System::String^ location );

			/**
			 * @fn System::String^ getResultLocation( int id )
			 * @brief gets the result location
			 * @param[in] id
			 * @return result location
			 */
			System::String^ getResultLocation( int id );

			/**
			 * @fn cli::array< int >^ getSearchesFromEngine( int engineId )
			 * @brief gets searches from search engine
			 * @param[in] engineId search engine ID
			 * @return the array of seach IDs.
			 */
			cli::array< int >^ getSearchesFromEngine( int engineId );

			/**
			 * @fn cli::array< int >^ getSearchesFromDb( int dbId )
			 * @brief gets searches from the database
			 * @param[in] dbId database ID
			 * @return the array of seach IDs.
			 */
			 cli::array< int >^ getSearchesFromDb( int dbId );

			/**
			 * @fn cli::array< int >^ getSearchesFromMatrix( int matrixId )
			 * @brief gets searches from the matrix
			 * @param[in] matrixId peak matrix ID
			 * @return the array of search IDs.
			 */
			cli::array< int >^ getSearchesFromMatrix( int matrixId );

			/**
			 * @fn cli::array< int >^ getSearchesFromSample( SampleWrapper^ sample )
			 * @brief gets searches from sample
			 * param[in] sample sample
			 * @return the array of search IDs.
			 */
			cli::array< int >^ getSearchesFromSample( SampleWrapper^ sample );

			/**
			 * @fn cli::array< int >^ getSearches()
			 * @brief gets all searches
			 * @return the array of search IDs.
			 */
			cli::array< int >^ getSearches();

			/**
			 * @fn SearchEngineWrapper::SearchInfo^ getSearch( int id )
			 * @brief gets the search information 
			 * @param[in] id search ID
			 * @return search information
			 */
			SearchEngineWrapper::SearchInfo^ getSearch( int id );

			/**
			 * @fn void getSearch( int id, SearchResultWrapper^ result )
			 * @brief gets the search and result information
			 * @param[in] id search ID
			 * @param[out] result result object
			 */
			void getSearch( int id, SearchResultWrapper^ result );

			/**
			 * @fn int getSearch( System::String^ engine, System::String^ version, System::String^ searchId )
			 * @brief gets the search engine
			 * @param[in] engine search engine name
			 * @parma[in] version search engine version
			 * @parma[in] id search ID
			 * @return ID
			 */
			int getSearch( System::String^ engine, System::String^ version, System::String^ searchId );

			/**
			 * @fn System::String^ getSearchId( int id )
			 * @brief gets the string search ID
			 * @param[in] id integer search ID
			 * @return string search ID
			 */
			System::String^ getSearchId( int id );

        public:
			/**
			 * @fn int setSearchSpectrum( int specId, int searchId, int peaks, double precursor, cli::array< int >^ charges )
			 * @brief sets search spectrum
			 * @param[in] specId spectrum ID
			 * @param[in] searchId search ID
			 * @param[in] peaks peaks ID
			 * @param[in] precursor precursor mass
			 * @param[in] charges charges
			 */
			int setSearchSpectrum( int specId, int searchId, int peaks, double precursor, cli::array< int >^ charges );
	   
			/**
			 * @fn void deleteSearchSpectrum( int id )
			 * @brief deletes search spectrum
			 * @param[in] id search spectrum ID
			 */
			void deleteSearchSpectrum( int id );

			/**
			 * @fn cli::array< int >^ getSearchSpectraFromSearch( int searchId )
			 * @brief gets search spectra from search
			 * @param[in] searchId search ID
			 * @return the array of spectrum IDs.
			 */
			cli::array< int >^ getSearchSpectraFromSearch( int searchId );

			/**
			 * @fn cli::array< int >^ getSearchSpectraFromSpectrum( int specId )
			 * @brief gets search spectra from sepectrum
			 * @param[in] specId spectrum ID
			 * @return the array of spectrum IDs.
			 */
			cli::array< int >^ getSearchSpectraFromSpectrum( int specId );

			/**
			 * @fn int getSearchSpectrum( int searchId, int specId )
			 * @brief gets the search spectrum ID
			 * @param[in] searchId search ID
			 * @param[in] specId spectrum ID
			 * @return search spectrum ID
			 */
			int getSearchSpectrum( int searchId, int specId );

			/**
			 * @fn cli::array< int >^ getSearchSpectra()
			 * @brief gets all search spectra
			 * @return the array of spectrum Ids.
			 */
			cli::array< int >^ getSearchSpectra();

			/**
			 * @fn SearchSpectrumInfo^ getSearchSpectrum( int id );
			 * @brief gets search spectrum information
			 * @param[in] id search spectrum ID
			 * @return search spectrum information
			 */
			SearchSpectrumInfo^ getSearchSpectrum( int id );

			/**
			 * @fn cli::array< int >^ getCharges( int searchSpecId )
			 * @brief gets charges
			 * @param[in] searchSpecId search spectrum ID
			 * return the array of charges
			 */
			cli::array< int >^ getCharges( int searchSpecId );

        public:
			/**
			 * @fn int setMatrixPeakSpectrum( int peakId, int specId )
			 * @brief sets matrix peak spectrum
			 * @param[in] peakId matrix peak ID
			 * @param[in] specId search spectrum ID
			 * @return matrix peak spectrum ID
			 */
			int setMatrixPeakSpectrum( int peakId, int specId );

			/**
			 * @fn void deleteMatrixPeakSpectrum( int id )
			 * @brief deletes matix peak spectrum
			 * @param[in] id matrix peak spectrum ID
			 */
			void deleteMatrixPeakSpectrum( int id );

			/**
			 * @fn cli::array< int >^ getMatrixPeakSpectraFromSpectrum( int searchSpecId );
			 * @brief gets matrix peak spectra from spectrum
			 * @param[in] searchSpecId search spectrum ID
			 * @return the array of spectrum IDs.
			 */
			cli::array< int >^ getMatrixPeakSpectraFromSpectrum( int searchSpecId );

			/**
			 * @fn cli::array< int >^ getMatrixPeakSpectraFromPeak( int peakId )
			 * @brief gets matrix peak spectra from matrix peak.
			 * @param[in] peakId matrix peak ID
			 * @return the array of spectrum IDs.
			 */
			cli::array< int >^ getMatrixPeakSpectraFromPeak( int peakId );

			/**
			 * @fn cli::array< int >^ getMatrixPeakSpectraFromSearchAndPeak( int searchId, int peakId )
			 * @brief gets matrix peak spectra from search and matrix peak
			 * @param[in] searchId search ID
			 * @param[in] peakId matrix peak ID
			 * @return the array of spectrum IDs.
			 */
			cli::array< int >^ getMatrixPeakSpectraFromSearchAndPeak( int searchId, int peakId );

			/**
			 * @fn cli::array< int >^ getMatrixPeakSpectra()
			 * @brief gets all matrix peak spectra
			 * @return the array of spectrum IDs.
			 */
			cli::array< int >^ getMatrixPeakSpectra();

			/**
			 * @fn int getMatrixPeakSpectrumPeak( int id )
			 * @brief gets matrix peak from matrix peak spectrum ID
			 * @param[in] id matrix peak spectrum ID
			 * @return matrix peak ID
			 */
			int getMatrixPeakSpectrumPeak( int id );

			/**
			 * @fn int getMatrixPeakSpectrumSpectrum( int id )
			 * @brief gets search spectrum from matrix peak spectrum ID
			 * @param[in] id matrix peak spectrum ID
			 * @return search spectrum ID
			 */
			int getMatrixPeakSpectrumSpectrum( int id );

		public:
			/**
			 * @fn int addHit( HitInfo^ hit )
			 * @brief adds hit information
			 * @param[in] hit hit information
			 * @return hit ID
			 */
			int addHit( HitInfo^ hit );

			/**
			 * @fn void deleteHit( int id )
			 * @brief deletes hit item
			 * @param[in] id hit ID
			 */
			void deleteHit( int id );
			
			/**
			 * @fn cli::array< int >^ getHitsFromSubstance( int substanceId )
			 * @brief gets hits from substance
			 * @param[in] substanceId substance ID
			 * @return the array of hit IDs.
			 */
			cli::array< int >^ getHitsFromSubstance( int substanceId );

			 /**
			  * @fn cli::array< int >^ getHitsFromSearch( int searchId );
			  * @brief gets hits from search
			  * @param[in] searchId search spectrum ID
			  * @return the array of hit IDs.
			  */
			cli::array< int >^ getHitsFromSearch( int searchId );

			/**
			 * @fn cli::array< int >^ getHitsFromSpectrum( int searchSpecId )
			 * @brief gets hits fro spectrum
			 * @param[in] searchSpecId search spectrum ID
			 * @return the array of hit IDs.
			 */
			cli::array< int >^ getHitsFromSpectrum( int searchSpecId );

			/**
			 * @fn cli::array< int >^ getHits()
			 * @brief gets all hits
			 * @return the array of hit IDs.
			 */
			cli::array< int >^ getHits();

			/**
			 * @fn HitInfo^ getHit( int id )
			 * @brief gets hit information
			 * @param[in] id hit ID
			 * @return hit information.
			 */
			HitInfo^ getHit( int id );

        public:
			/**
			 * @fn void setAccessionNumber( System::String^ prefix, int num )
			 * @brief sets accession number
			 * @param[in] prefix the prefix of the accession
			 * @param[in] num accession number
			 */
			void setAccessionNumber( System::String^ prefix, int num );

			/**
			 * @fn int getAccessionNumber( System::String^ prefix )
			 * @brief gets the current accession number
			 * @param[in] prefix the prefix of the accession
			 * @return accession number
			 */
			int getAccessionNumber( System::String^ prefix );

			/**
			 * @fn int issueAccessionNumber( System::String^ prefix )
			 * @brief count up the accession number
			 * @param[in] prefix the prefix of the accession
			 * @return the current accession number (befor counting up)
			 */
			 int issueAccessionNumber( System::String^ prefix );

        public:
			/**
			 * @fn int getDbVersion()
			 * @brief gets the database schema version
			 return database schema version
			 */
			int getDbVersion();

			/**
			 * @fn int getCurrentDbVersion()
			 * @brief gets the current database schema version
			 * @return current database schema version
			 */
			int getCurrentDbVersion();

        public:
			/**
			 * @fn bool convertDbSchema( ProgressWrapper^ progress )
			 * @brief converts database table
			 * @param[in] progress the object to show the progress
			 * @return If true, succeeded to convert db schema
			 */
			bool convertDbSchema( ProgressWrapper^ progress );

        public:
			/**
			 * @fn void getPeakList( System::String^ peakList, XYDataWrapper^ xyData )
			 * @brief gets the peak list
			 * @param[in] peakList the character string to get the peak list
			 * @param[out] xyData the object to store peak list
			 */
			void getPeakList( System::String^ peakList, XYDataWrapper^ xyData );

		protected:
			/**
			 * @fn static cli::array< int >^ makeIntArrayFromVector( std::vector< int >& arr )
			 * @brief makes integer array from vector object
			 * @param[in] arr source vector
			 * @return array object
			 */
			static cli::array< int >^ makeIntArrayFromVector( std::vector< int >& arr );

		public:
			/**
			 * @fn static IdentificationManagerWrapper^ getInstance()
			 * @brief gets identification manager object
			 * @return identification manager object (This is the only object.)
			 */
			static IdentificationManagerWrapper^ getInstance();
		};
	}
}

#endif		// __KOME_CLR_IDENTIFICATION_MANAGER_WRAPPER_H__

