/**
 * @file IdentificationManager.h
 * @brief interfaces of IdentificationManager class
 *
 * @author H.Parry
 * @date 2012.01.25
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#pragma once


#include "Substance.h"
#include "Hit.h"
#include "SearchEngine.h"


namespace kome {
	namespace operation {
		class Alignment;
	}

	namespace ident {

		class Enzyme;
		class Modification;
		class SearchEngine;
		class SpecInfo;

		/**
		 * @typedef DbFileType
		 * @brief database file type
		 */
		typedef enum {
			TYPE_OTHER = 0,
			TYPE_FASTA = 1,
			TYPE_FASTA_PRO = 2,
			TYPE_BLAST = 4
		} DbFileType;

		/**
		 * @typedef ColumnDataType
		 * @brief column data type
		 */
		typedef enum {
			TYPE_INT	 = 1,
			TYPE_UINT	= 2,
			TYPE_DOUBLE  = 3,
			TYPE_UDOUBLE = 4,
			TYPE_STRING  = 5
		} ColumnDataType;

		/**
		 * @typedef SubstanceRelationship
		 * @brief relationship of substances
		 */
		typedef enum {
			RELATIONSHIP_PARENT  = 1,
			RELATIONSHIP_SAME = 2
		} SubstanceRelationship;

		/**
		 * @struct SampleReadInfo
		 * @brief sample read information
		 */
		struct SampleReadInfo {
			int id;
			bool openSampleSet;
			bool openSample;
		};

		/**
		 * @struct SubstanceIfno
		 * @brief substance information
		 */
		struct SubstanceInfo {
			std::string accession;
			std::string name;
			int dbId;
			SubstanceType type;
			kome::core::Properties props;
            int parentId;			
		};

		/**
		 * @struct SubstanceValue
		 * @brief substance value
		 */
		struct SubstanceValue {
			std::string name;
			std::string value;
			ValueType type;
		};

		/**
		 * @struct AltSubstance
		 * @brief alt substance
		 */
		struct AltSubstance {
			int id;
			std::string before;
			std::string after;
			int start;
			int end;
		};

		/**
		 * @struct HitInfo
		 * @brief hit information
		 */
		struct HitInfo {
			int substance;
			int charge;
			int id;
			std::vector< int > spectra;	// search spectrum ID
			std::vector< AltSubstance > altSubstances;
			std::vector< SubstanceValue > props;
			std::vector< std::pair< Modification*, int > > mods;
		};

		/**
		 * @struct FormatInfo
		 * @brief format information
		 */
		struct FormatInfo {
			std::string name;
			std::string url;
		};

		/**
		 * @struct LocalDbFileInfo
		 * @brief local database file information
		 */
		struct LocalDbFileInfo {
			std::string name;
			std::vector< std::pair< DbFileType, std::string > > localFiles;
		};

		/**
		 * @struct DbInfo
		 * @brief database information
		 */
		struct DbInfo {
			std::string name;
			std::string description;
			std::string url;
			std::string regx;
			std::string residues;
			std::vector< FormatInfo > formats;
			std::vector< LocalDbFileInfo > localDbs;
		};

		/**
		 * @struct NormalizationMethodInfo
		 * @brief normalization method information
		 */
		struct NormalizationMethodInfo {
			std::string method;
			std::string params;
		};

		/**
		 * @struct SubstanceProperty
		 * @brief substance Property information
		 */
		struct SubstanceProperty {
			std::vector< std::string > vecStrKey;
			std::vector< std::string > vecStrValue;
		};


		/**
		 * @class IdentificationManager
		 * @brief identification object management class
		 */
		class IDENT_TOOLS_CLASS IdentificationManager
		{
		protected:
			/**
			 * @fn IdentificationManager( void )
			 * @brief constructor
			 */
			IdentificationManager( void );

			/**
			 * @fn ~IdentificationManager( void )
			 * @brief destructor
			 */
			~IdentificationManager( void );
		
		private:
			/** insufficient title tags warning code  */
			unsigned int				m_nInsufficientTitleTagsWarningCode;

			/** no valid title tags warning code  */
			unsigned int				m_nNoValidTitleTagsWarningCode;

			/** no valid hit spectra warning code  */
			unsigned int				m_nNoValidHitSpectraWarningCode;

			/** a search result could not be added warning code */
			unsigned int				m_nSearchResultAdditionWarningCode;

			/** array of available search engines */
			std::vector<SearchEngine*>	m_arrSrchEngines;

			/** array of enzymes */
			std::vector< Enzyme* >		m_arrEnzymes;

			/** array of modifications */
			std::vector<Modification*>	m_arrModifications;

			/** array of databases */
			std::vector< DbInfo > m_localDbs;

			/** inner database */
			kome::db::Database* m_db;

			/** database tables */
			std::vector<kome::db::Table> m_tables;

			/** database home directory */
			std::string m_dbHomeDir;

			/** focused peaks */
			std::set< int > m_focusedPeaks;

		public:
			/**
			 * @fn void clearFocusedPeaks()
			 * @brief clears focused peaks
			 */
			void clearFocusedPeaks();

			/**
			 * @fn void addFocusedPeak( const int peakId )
			 * @brief adds focused peak
			 * @param[in] peakId ID number of peak to be added to focused.
			 */
			void addFocusedPeak( const int peakId );

			/**
			 * @fn void setFocusedPeak( const int peakId )
			 * @brief sets the focused peak
			 * @param[in] peakId ID number of peak to be focused.
			 */
			void setFocusedPeak( const int peakId );

			/**
			 * @fn void activatePeak( const int peakId )
			 * @brief activates peak
			 * @param[in] peakId ID number of peak to be activated.
			 */
			void activatePeak( const int peakId );

			/**
			 * @fn void removeFocusedPeak( const int peakId )
			 * @brief removes the focused peak
			 * @param[in] peakId ID number of peak to be removed from focused.
			 */
			void removeFocusedPeak( const int peakId );

			/**
			 * @fn void getFocusedPeaks( std::vector< int >& peakIds )
			 * @brief gets the focused peak IDs.
			 * @param[out] peakIds the object to store focused peak IDs.
			 */
			void getFocusedPeaks( std::vector< int >& peakIds );

			/**
			 * @fn bool isFocusedPeak( const int peakId )
			 * @brief judges whther the specified peak is focused or not.
			 * @param[in] peakId peak ID
			 * @return If true, the specified peak is focused.
			 */
			bool isFocusedPeak( const int peakId );

		public:
			/**
			 * @fn bool writePepXML(
						const char* path,
						SearchResult& srchResult,
						const bool forTpp = false,
						kome::core::Progress* progress = NULL
					)						
			 * @brief write the specified search result to a pepXML file at the specified path
			 * @param[in] path output file path
			 * @param[in] srchResult search result
			 * @param[in] forTpp exports pepXML for TPP (PeptideProphet)
			 * @param[in,out] progress the object to show the progress
			 * @return If true, it succeeded to write the pepXML file.
			 */
			bool writePepXML(
					const char* path,
					SearchResult& srchResult,
					const bool forTpp = false,
					kome::core::Progress* progress = NULL
			);

			/**
			 * @fn bool writeTsv( const char* path, const SearchResult& srchResult, kome::core::Progress* progress = NULL );
			 * @brief write the specified search result to a TSV (tab-separated value) text file at the specified path
			 * @param[in] path output file path
			 * @param[in] srchResult search result
			 * @param[in,out] progress the object to show the progress
			 * @return If true, it succeeded to write the TSV file.
			 */
			bool writeTsv( const char* path, const SearchResult& srchResult, kome::core::Progress* progress = NULL );

			/**
			 * @fn int getNumberOfLocalDbs( void ) const
			 * @brief get the number of installed local databases
			 * @return local database count
			 */
			int getNumberOfLocalDbs( void ) const;

			/**
			 * @fn std::string getLocalDb( const int index ) const
			 * @brief get the specified local database name
			 * @param[in] index local database index
			 * @return local database name
			 */
			std::string getLocalDb( const int index ) const;

			/**
			 * @fn int getNumberOfLocalDbFiles( const int index ) const
			 * @brief get the number of files in the specified local database
			 * @param[in] index local database index
			 * @return file count
			 */
			int getNumberOfLocalDbFiles( const int index ) const;

			/**
			 * @fn std::string getLocalDbFileName( const int dbIndex, const int fileIndex ) const
			 * @brief get the filename of the specified file in the specified local database
			 * @param[in] dbIndex local database index
			 * @param[in] fileIndex file index
			 * @return file name
			 */
			std::string getLocalDbFileName( const int dbIndex, const int fileIndex ) const;

			/**
			 * @fn int getLocalDbFileType( const int dbIndex, const int fileIndex ) const
			 * @brief get the type of the specified file in the specified local database
			 * @param[in] dbIndex local database index
			 * @param[in] fileIndex file index
			 * @return local database file type
			 */
			int getLocalDbFileType( const int dbIndex, const int fileIndex ) const;

			/**
			 * @fn std::string getLocalDbFilePath( const int dbIndex, const int fileIndex, const DbFileType fileType ) const
			 * @brief get the file path of the specified file and type in the specified local database
			 * @param[in] dbIndex local database index
			 * @param[in] fileIndex file index
			 * @param[in] fileType file type
			 * @return local database file path
			 */
			std::string getLocalDbFilePath( const int dbIndex, const int fileIndex, const DbFileType fileType ) const;

			/**
			 * @fn std::string getAccessionRegx( const int index ) const
			 * @brief get the regular expression of Accession
			 * @param[in] index local database index
			 * @return regular expression of Accession
			 */
			std::string getAccessionRegx( const int index ) const;

			/**
			 * @fn std::string getResiduesFile( const int index ) const
			 * @brief gets the residues file
			 * @param[in] index local database index
			 * @return residues file
			 */			 
			std::string getResiduesFile( const int index ) const;

			/**
			 * @fn std::string getLocalDbDesc( const int index ) const
			 * @brief gets the local database description
			 * @param[in] index local database index
			 * @return local db description
			 */
			std::string getLocalDbDesc( const int index ) const;


			/**
			 * @fn int addPublicDb( const char* dbName, const char* url )
			 * @brief add a public (protein, metabolite) database
			 * @param[in] dbName database name
			 * @param[in] url database URL
			 * @return database ID registered in the MSPP_DATABASE table
			 */
			int addPublicDb( const char* dbName, const char* url );

			/**
			 * @fn void setPublicUrl( const int dbId, const char* format, const char* url )
			 * @brief set the URL for a public (protein, metabolite) database
			 * @param[in] dbId database ID
			 * @param[in] format format name
			 * @param[in] url database URL (string includes a %s to correspond with Accession)
			 */
			void setPublicUrl( const int dbId, const char* format, const char* url );

			/**
			 * @fn unsigned int registerResult( SearchResult& srchResult, kome::core::Progress& prog )
			 * @brief register a search result in the internal database
			 * @param[in] srchResult SearchResult object
			 * @param[out] prog progress display object
			 * @return If 0, no warning occurred. Otherwise, it contains a warning code.
			 */
			unsigned int registerResult( SearchResult& srchResult, kome::core::Progress& prog );

			/**
			 * @fn unsigned int registerResult( int searchId, SearchResult& srchResult, kome::core::Progress& prog );
			 * @brief register a search result in the internal database
			 * @param[in] searchId search ID
			 * @param[in] srchResult SearchResult object
			 * @param[out] prog progress display object
			 * @return If 0, no warning occurred. Otherwise, it contains a warning code.
			 */
			unsigned int registerResult( int searchId, SearchResult& srchResult, kome::core::Progress& prog );

        public:			
			/**
			 * @fn void loadEnzymes()
			 * @brief loads enzyme
			 */
			void loadEnzymes();

			/**
			 * @fn int getNumberOfEnzymes( void ) const
			 * @brief get the number of enzymes
			 * @return enzyme count
			 */
			int getNumberOfEnzymes( void ) const;

			/**
			 * @fn Enzyme* getEnzyme( const int index ) const
			 * @brief get the specified enzyme information
			 * @param[in] index enzyme index
			 * @return Enzyme object
			 */
			Enzyme* getEnzyme( const int index ) const;

			/**
			 * @fn Enzyme* getEnzymeFromId( const int id )
			 * @brief gets enzyme from ID
			 * @param[in] id enzyme ID
			 * @return enzyme
			 */
			Enzyme* getEnzymeFromId( const int id );

			/**
			 * @fn void setAllEnzymeHidden()
			 * @brief sets all enzymes hidden
			 */
			void setAllEnzymeHidden();

			/**
		 	 * @fn int updateEnzyme( Enzyme* enzyme )
			 * @brief updates enzyme
			 * @param[in] enzyme enzyme object
			 * @return enzyme ID
			 */
			int updateEnzyme( Enzyme* enzyme );

        protected:
			/**
			 * @fn int searchEnzyme( const char* name )
			 * @brief searches enzyme
			 * @param[in] name enzyme name
			 * @return enzyme index
			 */
			int searchEnzyme( const char* name );

			/**
			 * @fn void deleteEnzymes()
			 * @brief deletes enzymes
			 */
			void deleteEnzymes();

        public:
			/**
			 * @fn void loadModifications( void )
			 * @brief loads modification
			 */
 			void loadModifications( void );

			/**
			 * @fn int getNumberOfModifications( void ) const
			 * @brief get the number of modifications
			 * @return modification count
			 */
			int getNumberOfModifications( void ) const;

			/**
			 * @fn Modification* getModification( const int index ) const
			 * @brief get a pointer to the specified Modification object
			 * @param[in] index modification index
			 * @return pointer to a Modification object
			 */
			Modification* getModification( const int index ) const;

			/**
			 * @fn Modification* getModificationFromId( const int id )
			 * @brief gets modification from ID
			 * @param[in] id modificatio nID
			 * @return modification
			 */
			Modification* getModificationFromId( const int id );

			/**
			 * @fn void setAllModificationHidden()
			 * @brief sets all modification hidden
			 */
			void setAllModificationHidden();

			/**
			 * @fn int updateModification( Modification* modification )
			 * @brief updates modification 
			 * @param[in] modification modification object
			 * @return modification ID
			 */
			int updateModification( Modification* modification );

        protected:
			/**
			 * @fn int searchModification( const char* name )
			 * @brief searches modification
			 * @param[in] name modification name
			 * @return modification ID
			 */
			int searchModification( const char* name );

			/**
			 * @fn void deleteModifications()
			 * @brief deletes modifications
			 */
			void deleteModifications();

        public:
			/**
			 * @fn int addSearchModification( const int search, const int modification, const bool isVariable )
			 * @brief adds search modification
			 * @param[in] search search ID
			 * @param[in] modification modification ID
			 * @param[in] isVariable If true, variable modification
			 * @return search modification ID
			 */
			int addSearchModification( const int search, const int modification, const bool isVariable );

			/**
			 * @fn void getSearchModifications( std::vector< int >& searchMods, const int searchId )
			 * @brief gets search modifications from search ID
			 * @param[out] searchMods the array to store search modification IDs.
			 * @param[in] searchId search ID
			 */
			void getSearchModifications( std::vector< int >& searchMods, const int searchId );

			/**
			 * @fn int getModificationFromSearchModification( const int searchModId )
			 * @brief gets the modification ID from search modification ID
			 * @param[in] searchModId search modification Id
			 * @return modification ID
			 */
			int getModificationFromSearchModification( const int searchModId );

			/**
			 * @fn int getSearchFromSearchModification( const int searchModId )
			 * @brief gets the search ID from search modificaiton ID
			 * @param[in] searchModId search modification ID
			 * @return search ID
			 */
			int getSearchFromSearchModification( const int searchModId );

			/**
			 * @fn bool isVariableModification( const int searchModId )
			 * @brief get the value the specified search modification is variable or not
			 * @param[in] searchModId search modification ID
			 * @return If true, variable modification
			 */
			bool isVariableModification( const int searchModId );

        protected:
			/**
			 * @fn void deleteSearchModifications( const int searchId )			
			 * @brief deletes search modifications
			 */
			void deleteSearchModifications( const int searchId );

        public:
			/**
			 * @fn int addHitModification( const int hit, const int searchMod, const int pos )
			 * @brief adds hit modification
			 * @param[in] hit hid ID
			 * @param[in] searchMod search modification ID
			 * @param[in] pos position
			 */
			int addHitModification( const int hit, const int searchMod, const int pos );

			/**
			 * @fn void getHitModifications( std::vector< int >& hitMods, const int hitId )
			 * @brief gets hit modifications 
			 * @param[out] hitMods the array to store hit modification IDs.
			 * @param[in] hitId hit ID
			 */
			void getHitModifications( std::vector< int >& hitMods, const int hitId );

			/**
			 * @fn int getSearchModificationFromHitModification( const int hitMod )
			 * @brief gets the search modification from hit modification 
			 * @param[in] hitMod hit modification ID
			 * @return search modification ID
			 */
			int getSearchModificationFromHitModification( const int hitMod );

			/**
			 * @fn int getHitFromHitModification( const int hitMod )
			 * @brief gets the hit ID from hit modification ID
			 * @return hit ID
			 */
			int getHitFromHitModification( const int hitMod );

			/**
			 * @fn int getHitModPosition( const int hitMod )
			 * @brief gets the hit modification position
			 * @return hit modification position
			 */
			int getHitModPosition( const int hitMod );

        protected:

			/**
			 * @fn void deleteHitModifications( const int hitId )
			 * @brief deletes hit modifications
			 * @param[in] hitId hit ID
			 */
			void deleteHitModifications( const int hitId );

        public:
			/**
			 * @fn bool ParseSpectrumTitle(const char* spectrumTitle, SpecInfo& spectrumInfo, int* charge)
			 * @brief parse the spectrum title string into a SpecInfo struct
			 * @param[in] spectrumTitle title string to parse
			 * @param[out] spectrumInfo spectrum information object
			 * @param[out] charge the pointer to store charge state
			 * @return If it failed to parse the spectrum title, this method returns false.
			 */
			bool ParseSpectrumTitle(const char* spectrumTitle, SpecInfo& spectrumInfo, int* charge);

			/**
			 * @fn const char* getDbHome()
			 * @brief gets the database home directory
			 * @return database home directory
			 */
			const char* getDbHome();

		public:
			/**
			 * @fn unsigned int InsufficientTitleTagsWarningCode( void )
			 * @brief return the warning code indicating insufficient spectrum title tags in result file
			 * @return the warning code.
			 */
			inline unsigned int InsufficientTitleTagsWarningCode( void ) {return m_nInsufficientTitleTagsWarningCode;}

			/**
			 * @fn unsigned int NoValidTitleTagsWarningCode( void )
			 * @brief return the warning code indicating no valid title tags in result file
			 * @return the warning code.
			 */
			inline unsigned int NoValidTitleTagsWarningCode( void ) {return m_nNoValidTitleTagsWarningCode;}

			/**
			 * @fn unsigned int NoValidHitSpectraWarningCode( void )
			 * @brief return the warning code indicating no valid hit spectra in result file
			 * @return the warning code.
			 */
			inline unsigned int NoValidHitSpectraWarningCode( void ) {return m_nNoValidHitSpectraWarningCode;}

			/**
			 * @fn unsigned int SearchResultAdditionWarningCode( void )
			 * @brief return the warning code indicating a search result could not be added
			 * @return the warning code.
			 */
			inline unsigned int SearchResultAdditionWarningCode( void ) {return m_nSearchResultAdditionWarningCode;}

		protected:
			/**
			 * @fn void loadLocalDbFiles( void )
			 * @brief loads local database files
			 */
			void loadLocalDbFiles( void );

        public:
			/**
			 * @fn void addSearchEngine( SearchEngine* pSrchEngine )
			 * @brief add a search engine
			 * @param[in] pSrchEngine pointer to a SearchEngine object
			 */
			void addSearchEngine( SearchEngine* pSrchEngine );

			/**
			 * @fn int getNumberOfSearchEngines( void ) const
			 * @brief get the number of search engines
			 * @return number of search engines
			 */
			int getNumberOfSearchEngines( void ) const;

			/**
			 * @fn SearchEngine* getSearchEngine( const int index ) const
			 * @brief get the specified search engine
			 * @param[in] index search engine index
			 * @return search engine
			 */
			SearchEngine* getSearchEngine( const int index ) const;

			/**
			 * @fn bool removeSearchEngine( SearchEngine* pSrchEngine )
			 * @brief remove a search engine
			 * @param[in] pSrchEngine pointer to a SearchEngine object
			 * @return If true, it succeeded to remove the search engine.
			 */
			bool removeSearchEngine( SearchEngine* pSrchEngine );

        private:
			/**
			 * @fn void writeAllSubstances( FILE* fp, const SearchResult& srchResult, SubstanceType subType )
			 * @brief write all information for substance of the specified type
			 * @param[out] fp file pointer
			 * @param[in] srchResult search result
			 * @param[in] subType substance type
			 */
			void writeAllSubstances( FILE* fp, const SearchResult& srchResult, SubstanceType subType );

			/**
			 * @fn bool GetStringValue(FILE* fp, const char* tag, std::string& strValue)
			 * @brief get a string value for a specified tag from a file
			 * @param[in] fp pointer to a file object
			 * @param[in] tag name of the tag to get
			 * @param[out] strValue value of the tag
			 * @return If true, it succeeded to read the value.
			 */
			bool GetStringValue(FILE* fp, const char* tag, std::string& strValue);

			/**
			 * @fn void ShowFileReadingProgress(FILE* fp, size_t fileSize, kome::core::Progress& prog, const int nSpectraCount)
			 * @brief set progress message according to current file position
			 * @param[in] fp pointer to a file object
			 * @param[in] fileSize total file size
			 * @param[in] nSpectraCount the number of spectra read in so far
			 * @param[out] prog progress display object
			 */
			void ShowFileReadingProgress(FILE* fp, size_t fileSize, const int nSpectraCount, kome::core::Progress& prog);

			/**
			 * @fn kome::db::Database* getDb()
			 * @brief gets the database object
			 * @return database object.
			 */
			kome::db::Database* getDb();

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

        private:
		   /**
		    * @fn void createTables()
			* @brief create tables
			*/
		   void createTables();

        private:
		   /**
		    * @fn void createSampleSetTable( kome::db::Table& table )
			* @brief creates sample set table
			* @param[out] table sample set table object
			*/
		   void createSampleSetTable( kome::db::Table& table );

		   /**
		    * @fn void createSampleTable( kome::db::Table& table )
			* @brief creates sample table
			* @param[out] table sample table object
			*/
		   void createSampleTable( kome::db::Table& table );

		   /**
		    * @fn void createSpecTable( kome::db::Table& table )
			* @brief creates spectra table
			* @param[out] table spectra table object
			*/
		   void createSpecTable( kome::db::Table& table );

		   /**
		    * @fn void createVersionTable( kome::db::Table& table )
			* @brief creates version table
			* @param[out] table version table object
			*/
		   void createVersionTable( kome::db::Table& table );

		   /**
		    * @fn void createDbTable( kome::db::Table& table )
			* @brief creates database table
			* @param[out] table database table object
			*/
		   void createDbTable( kome::db::Table& table );

		   /**
		    * @fn void createFormatTable( kome::db::Table& table )
			* @brief creates format table
			* @param[out] table format table object
			*/
		   void createFormatTable( kome::db::Table& table );

		   /**
		    * @fn void createUrlTable( kome::db::Table& table )
			* @brief creates URL table
			* @param[out] table URL table object
			*/
		   void createUrlTable( kome::db::Table& table );

		   /**
		    * @fn void createEngineTable( kome::db::Table& table )
			* @brief creates engine table
			* @param[in] table search engine table object
			*/
		   void createEngineTable( kome::db::Table& table );

		   /**
		    * @fn void createSearchTable( kome::db::Table& table )
			* @brief creates search table
			* @param[out] table search table object
			*/
		   void createSearchTable( kome::db::Table& table );

		   /**
		    * @fn void createSubstanceTable( kome::db::Table& table )
			* @brief creates substance table
			* @param[out] table substance table object
			*/
		   void createSubstanceTable( kome::db::Table& table );

		   /**
		    * @fn void createRelationshipTable( kome::db::Table& table )
			* @brief creates substance relationship table
			* @param[out] table substance relationship table object
			*/
		   void createRelationshipTable( kome::db::Table& table );

		   /**
		    * @fn void createSubstanceItemTable( kome::db::Table& table )
			* @brief creates substance item table
			* @param[out] table substance item table object
			*/
		   void createSubstanceItemTable( kome::db::Table& table );

		   /**
		    * @fn void createSubstanceValueTable( kome::db::Table& table )
			* @brief creates substance value table
			* @param[out] table substance value table object
			*/
		   void createSubstanceValueTable( kome::db::Table& table );

		   /**
		    * @fn void createPeakDetectionTable( kome::db::Table& table )
			* @brief creates peak detection table
			* @param[out] table peak detection table object.
			*/
		   void createPeakDetectionTable( kome::db::Table& table );

		   /**
		    * @fn void createPeakParamsTable( kome::db::Table& table )
			* @brief creates peak detection parameters table
			* @param[out] table peak detection parameters table object.
			*/
		   void createPeakParamsTable( kome::db::Table& table );

		   /**
		    * @fn void createHitTable( kome::db::Table& table )
			* @brief creates hit table
			* @param[out] table hit table object
			*/
		   void createHitTable( kome::db::Table& table );

		   /**
		    * @fn void createHitSpecTable( kome::db::Table& table )
			* @brief creates hit spectrum table
			* @param[out] table hit spectrum table object
			*/
		   void createHitSpecTable( kome::db::Table& table );

		   /**
		    * @fn void createHitItemTable( kome::db::Table& table )
			* @brief creates hit item table
			* @param[out] table hit item table object
			*/
		   void createHitItemTable( kome::db::Table& table );

		   /**
		    * @fn void createHitValueTable( kome::db::table& table )
			* @brief create hit value table
			* @param[out] table hit value table object
			*/
		   void createHitValueTable( kome::db::Table& table );

		   /**
		    * @fn void createSearchParamsTable( kome::db::Table& table )
			* @brief creates search parameters table
			* @param[out] table search parameters table object
			*/
		   void createSearchParamsTable( kome::db::Table& table );

		   /**
		    * @fn void createSearchValueTable( kome::db::Table& table )
			* @brief creates search value table
			* @param[out] table search value table object
			*/
		   void createSearchValueTable( kome::db::Table& table );

		   /**
		    * @fn void createSearchSpecTable( kome::db::Table& table )
			* @brief creates search spectrum table
			* @param[out] table search spectrum table object
			*/
		   void createSearchSpecTable( kome::db::Table& table );

		   /**
		    * @fn void createMatrixTable( kome::db::Table& table )
			* @brief creates peak matrix table
			* @param[out] table peak matrix table object
			*/
		   void createMatrixTable( kome::db::Table& table );

		   /**
		    * @fn void createGroupTable( kome::db::Table& table )
			* @brief creates group table
			* @param[out] table group table object
			*/
		   void createGroupTable( kome::db::Table& table );

		   /**
		    * @fn void createMatrixSampleTable( kome::db::Table& table )
			* @brief creates matrxi sample table 
			* @param[out] matrix sample table object
			*/
		   void createMatrixSampleTable( kome::db::Table& table );

		   /**
		    * @fn void createPeakTable( kome::db::Table& table )
			* @brief creates peak table
			* @param[out] table peak table object
			*/
		   void createPeakTable( kome::db::Table& table );

		   /**
		    * @fn void createMatrixColumnTable( kome::db::Table& table )
			* @brief creates matrix column table
			* @param[out] table matrix column table object
			*/
		   void createMatrixColumnTable( kome::db::Table& table );

		   /**
		    * @fn void createMatrixValueTable( kome::db::Table& table )
			* @brief creates matrix value table
			* @param[out] table matrix value table object
			*/
		   void createMatrixValueTable( kome::db::Table& table );

		   /**
		    * @fn void createMatrixSpectrumTable( kome::db::Table& table )
			* @brief creates matrix spectrum table
			* @param[out] table matrix spectrum table
			*/
		   void createMatrixSpectrumTable( kome::db::Table& table );

		   /**
		    * @fn void createProfileTable( kome::db::Table& table )
			* @brief creates profile table
			* @param[out] profile table object
			*/
		   void createProfileTable( kome::db::Table& table );

		   /**
		    * @fn void createPeakValueTable( kome::db::Table& table )
			* @brief creates peak value table
			* @param[out] peak value table object
			*/
		   void createPeakValueTable( kome::db::Table& table );

		   /**
		    * @fn void createAccessionTable( kome::db::Table& table )
			* @brief creates accession table
			* @param[out] table accession table object
			*/
		   void createAccessionTable( kome::db::Table& table );

		   /**
		    * @fn void createStandardTable( kome::db::Table& table )
			* @brief creates standard sample table
			* @param[out] table standard sample table object
			*/
		   void createStandardTable( kome::db::Table& table );

		   /**
		    * @fn void createNormalizationFilterTable( kome::db::Table& table )
			* @brief creates normalization filter table
			* @param[out] table normalization filter table object
			*/
		   void createNormalizationFilterTable( kome::db::Table& table );

		   /**
		    * @fn void createNormalizationMethodTable( kome::db::Table& table )
			* @brief creates normalization method table
			* @param[out] table normalization method table
			*/
		   void createNormalizationMethodTable( kome::db::Table& table );

		   /**
		    * @fn void createRtAlignmentTable( kome::db::Table& table )
			* @brief creates RT Alignment table
			* @param[out] table RT alignment table
			*/
		   void createRtAlignmentTable( kome::db::Table& table );

		   /**
		    * @fn void createAltSubstanceTable( kome::db::Table& table )
			* @brief creates alternative substance table
			* @param[out] table alternative substance table
			*/
		   void createAltSubstanceTable( kome::db::Table& table );

		   /**
		    * @fn void createPeakListTable( kome::db::Table& table )
			* @brief creates peak list table
			* @param[out] peak list table
			*/
		   void createPeakListTable( kome::db::Table& table );

		   /**
		    * @fn void createModTable( kome::db::Table& table )
			* @brief creates modification table
			* @param[out] modification table
			*/
		   void createModTable( kome::db::Table& table );

		   /**
		    * @fn void createSearchModTable( kome::db::Table& table )
			* @brief creates search modification table
			* @parma[out] table search modification table
			*/
		   void createSearchModTable( kome::db::Table& table );

		   /**
		    * @fn void createHitModTable( kome::db::Table& table )
			* @brief creates hit modification table
			* @param[out] table hit modification table
			*/
		   void createHitModTable( kome::db::Table& table );

		   /**
		    * @fn void createEnzymeTable( kome::db::Table& table )
			* @brief creates enzyme table
			* @param[out] table enzyme table
			*/
		   void createEnzymeTable( kome::db::Table& table );

		   /**
		    * @fn void createEnzymeCutterTable( kome::db::Table& table )
			* @brief creates enzyme cutter table
			* @param[out] table enzyme cutter table
			*/
		   void createEnzymeCutterTable( kome::db::Table& table );

        public:
			/**
			 * @fn int addMatrix(
					    const char* name,
						const char* comment,
						const bool rtFlg,
						const bool mzFlg,
						const bool chargeFlg
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
				    const char* name,
					const char* comment,
					const bool rtFlg,
					const bool mzFlg,
					const bool chargeFlg
			);

			/**
			 * @fn void deleteMatrix( const int id )
			 * @brief deletes matrix
			 * @param[in] id matrix ID
			 */
			void deleteMatrix( const int id );

			/**
			 * @fn int getSelectedMatrix()
			 * @brief gets the selected matrix
			 * @return selected matrix ID
			 */
			int getSelectedMatrix();

			/**
			 * @fn void selectMatrix( const int id )
			 * @brief selects matrix
			 * @param[in] id matrix ID
			 */
			void selectMatrix( const int id );

			/**
			 * @fn void updateMatrixFlags( const int id, const bool rtFlg, const bool mzFlg, const bool chargeFlg )
			 * @brief updates matrix flags
			 * @param[in] id matrix ID
			 * @param[in] rtFlg RT flag value
			 * @param[in] mzFlg m/z flag value
			 * @param[in] chargeFlg charge flag value
			 */
			void updateMatrixFlags( const int id, const bool rtFlg, const bool mzFlg, const bool chargeFlg );

			/**
			 * @fn void updateMatrix( const int id, const char* name, const char* comment )
			 * @brief updates matrix
			 * @param[in] id matrix ID
			 * @param[in] name matrix name
			 * @param[in] comment comment
			 */
			void updateMatrix( const int id, const char* name, const char* comment );

			/**
			 * @fn void getMatrices( std::vector< int >& matrices )
			 * @brief gets matrices
			 * @param[out] matrices array object to store matrices.
			 */
			void getMatrices( std::vector< int >& matrices );

			/**
			 * @fn std::string getMatrixName( const int id )
			 * @brief gets the matrix name
			 * @param[in] id matrix ID
			 * @return matrix name
			 */
			std::string getMatrixName( const int id );

			/**
			 * @fn std::string getMatrixComment( const int id )
			 * @brief gets the matrix comment
			 * @param[in] id matrix ID
			 * @return matrix comment
			 */
			std::string getMatrixComment( const int id );

			/**
			 * @fn void getMatrixFlags( const int id, bool* rtFlg, bool* mzFlg, bool* chargeFlg )
			 * @brief gets matrix flags
			 * @param[in] id matrix ID
			 * @param[out] rtFlg the pointer to store rt flag value
			 * @param[out] mzFlg the pointer to store m/z flag value
			 * @param[out] chargeFlg the pointer to store charge flag value
			 */
			void getMatrixFlags( const int id, bool* rtFlg, bool* mzFlg, bool* chargeFlg );

        public:
			/**
			 * @fn int addGroup( const char* name, const int matrix, const COLOR color )
			 * @brief adds group
			 * @param[in] name group name
			 * @param[in] matrix peak matrix ID
			 * @param[in] color group color
			 * @return group ID
			 */
			int addGroup( const char* name, const int matrix, const COLOR color );

			/**
			 * @fn void deleteGroup( const int id )
			 * @brief deletes group 
			 * @param[in] id group ID
			 */
			void deleteGroup( const int id );

			/**
			 * @fn void updateGroup( const int id, const char* name, const int color )
			 * @brief updates group
			 * @param[in] id group ID
			 * @param[in] name group name
			 * @param[in] color group color
			 */
			void updateGroup( const int id, const char* name, const COLOR color );

			/**
			 * @fn void setGroupOrder( const int id, const int order )
			 * @brief sets group order
			 * @param[in] id group ID
			 * @param[in] order group order
			 */
			void setGroupOrder( const int id, const int order );

			/**
			 * @fn void getGroups( std::vector< int >& groups, const int matrixId )
			 * @brief gets groups
			 * @param[out] groups the array object to store group IDs.
			 * @param[in] matrixId peak matrix ID
			 */
			void getGroups( std::vector< int >& groups, const int matrixId );

			/**
			 * @fn int getGroupMatrix( const int id )
			 * @brief gets the matrix ID of the specified group
			 * @param[in] id group ID
			 * @return matrix ID
			 */
			int getGroupMatrix( const int id );

			/**
			 * @fn std::string getGroupName( const int id )
			 * @brief gets the group name
			 * @param[in] id group ID
			 */
			std::string getGroupName( const int id );

			/**
			 * @fn COLOR getGroupColor( const int id )
			 * @brief gets the group color
			 * @param[in] id group ID
			 * @return group color
			 */
			COLOR getGroupColor( const int id );

        public:
			/**
			 * @fn void setStandardSample( const int matrixId, const int  matrixSampleId )
			 * @brief sets the standard sample
			 * @param[in] matrixId peak matrix ID
			 * @param[in] matrixSampleId peak matrix sample ID
			 */
			void setStandardSample( const int matrixId, const int  matrixSampleId );

			/**
			 * @fn int getStandardSample( const int matrixId )
			 * @brief gets the standard sample
			 * @param[in] matrixId peak matrix ID
			 * @return standard peak matrix sample ID
			 */
			int getStandardSample( const int matrixId );

        public:
			/**
			 * @fn void clearNormalization( const int matrix )
			 * @brief clears normalization
			 * @param matrix peak matrix ID
			 */
			void clearNormalization( const int matrix );

			/**
			 * @fn void setNormalizationFilter(
						const int matrix,
						const double minInt,
						const double maxInt,
						const bool relFlg,
						const double minRt,
						const double maxRt,
						const double minMz,
						const double maxMz,
						const char* method,
						const char* params
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
					const int matrix,
					const double minInt,
					const double maxInt,
					const bool relFlg,
					const double minRt,
					const double maxRt,
					const double minMz,
					const double maxMz,
					const char* method,
					const char* params
			);

			/**
			 * @fn bool getNormalizationFilter(
						const int matrix,
						double* minInt,
						double* maxInt,
						bool* relFlg,
						double* minRt,
						double* maxRt,
						double* minMz,
						double* maxMz,
						std::string& method,
						std::string& params
					)
			* @brief gets the normalization filter information
			* @param[in] matrix peak matrix ID
			* @param[out] minInt the pointer to store min intensity
			* @param[out] maxInt the pointer to store max intensity
			* @param[out] relFlg the pointer to store relative flag value
			* @param[out] minRt the pointer to store min RT
			* @param[out] maxRt the pointer to store max RT
			* @param[out] minMz the pointer to store min m/z
			* @param[out] maxMz the pointer to store max m/z
			* @param[out] method string object to store filter method name
			* @param[out] params string object to store filter parameters
			* @return If false, normalization filter is not registered of specified peak matrix
			*/
			bool getNormalizationFilter(
					const int matrix,
					double* minInt,
					double* maxInt,
					bool* relFlg,
					double* minRt,
					double* maxRt,
					double* minMz,
					double* maxMz,
					std::string& method,
					std::string& params
			);

			/**
			 * @fn void addNormalizationMethod(
						const int matrix,
						const char* method,
						const char* params
					)
			 * @brief adds normalization method
			 * @param[in] matrix peak matrix ID
			 * @param[in] method normalization method name
			 * @param[in] params normalization parameters
			 */
			void addNormalizationMethod(
				const int matrix,
				const char* method,
				const char* params
			);

			/**
			 * @fn void getNormalizationMethods( const int matrix, std::vector< NormalizationMethodInfo >& methods )
			 * @brief gets the normalization method list
			 * @param[in] matrix peak matrix ID
			 * @param[out] methods array object to store normalization method information
			 */
			void getNormalizationMethods( const int matrix, std::vector< NormalizationMethodInfo >& methods );

        protected:
			/**
			 * @fn int getNormalizationFilterId( const int matrix )
			 * @brief gets the normalization filter ID
			 * @param[in] matrix peak matrix ID
			 * @return normalization filter ID
			 */
			int getNormalizationFilterId( const int matrix );

        public:
			/**
			 * @fn void setAlignment( const int matrix, const char* method, const char* params )
			 * @brief sets the Alignment information
			 * @param[in] matrix peak matrix ID
			 * @param[in] method alignment method
			 * @param[in] params alignment parameters
			 */
			void setAlignment( const int matrix, const char* method, const char* params );

			/**
			 * @fn kome::operation::Alignment* getAlignment( const int matrix )
			 * @brief gets the Alignment information
			 * @param[in] matrix peak matrix ID
			 * @return Alignment object
			 */
			kome::operation::Alignment* getAlignment( const int matrix );
        
        public:
			/**
			 * @fn int addMatrixColumn( const char* name, const ColumnDataType type, const int matrixId )
			 * @brief adds matrix column
			 * @param[in] name column name
			 * @param[in] type data type
			 * @param[in] matrixId matrix ID
			 * @return matrix column ID
			 */
			int addMatrixColumn( const char* name, const ColumnDataType type, const int matrixId );

			/**
			 * @fn void deleteMatrixColumn( const int id )
			 * @brief deletes matrix column
			 * @param[in] id matrix column ID
			 */
			void deleteMatrixColumn( const int id );

			/**
			 * @fn void updateMatrixColumn( const int id, const char* name, const ColumnDataType type )
			 * @brief updates column
			 * @param[in] id column ID
			 * @param[in] name column name
			 * @param[in] type data type
			 */
			void updateMatrixColumn( const int id, const char* name, const ColumnDataType type );

			/**
			 * @fn void getMatrixColumns( std::vector< int >& columns, const int matrixId )
			 * @brief get matrix columns
			 * @param[out] columns the array object to store matrix column IDs
			 * @param[in] matrixId matrix ID
			 */
			void getMatrixColumns( std::vector< int >& columns, const int matrixId );

			/**
			 * @fn std::string getMatrixColumnName( const int id )
			 * @brief gets the matrix column name
			 * @param[in] id matrix column ID
			 * @return matrix column name
			 */
			std::string getMatrixColumnName( const int id );

			/**
			 * @fn ColumnDataType getMatrixColumnType( const int id )
			 * @brief gets the matrix column data type
			 * @param[in] id matrix column ID
			 * @return matrix column data type
			 */
			ColumnDataType getMatrixColumnType( const int id );

        public:
			/**
			 * @fn int addMatrixPeak( const double rt, const double mz, const int charge, const int matrixId )
			 * @brief adds matrix peak
			 * @param[in] rt RT position of peak
			 * @param[in] mz m/z position of peak
			 * @param[in] charge charge of peak
			 * @param[in] matrixId matrix ID
			 * @return peak ID
			 */
			int addMatrixPeak( const double rt, const double mz, const int charge, const int matrixId );

			/**
			 * @fn void deleteMatrixPeak( const int id )
			 * @brief deletes matrix peak 
			 * @param[in] id peak ID
			 */
			void deleteMatrixPeak( const int id );

			/**
			 * @fn void updateMatrixPeak( const int id, const double rt, const double mz, const int charge )
			 * @brief updates matrix peak 
			 * @param[in] id peak ID
			 * @param[in] rt RT position of peak
			 * @param[in] mz m/z position of peak
			 * @param[in] charge charge of peak
			 */
			void updateMatrixPeak( const int id, const double rt, const double mz, const int charge );

			/**
			 * @fn void getMatrixPeaks( std::vector< int >& peaks, const int matrixId )
			 * @brief gets matrix peaks
			 * @param[out] peaks array object to store matrix peak IDs.
			 * @param[in] matrixId peak matrix ID
			 */
			void getMatrixPeaks( std::vector< int >& peaks, const int matrixId );

			/**
			 * @fn double getMatrixPeakRt( const int id )
			 * @brief get the RT position of peak
			 * @param[in] id peak ID
			 * @return RT position of peak
			 */
			double getMatrixPeakRt( const int id );

			/**
			 * @fn double getMatrixPeakMz( const int id )
			 * @brief get the m/z position of peak
			 * @param[in] id peak ID
			 * @return RT position of peak
			 */
			double getMatrixPeakMz( const int id );

			/**
			 * @fn int getMatrixPeakCharge( const int id )
			 * @brief get the peak charge
			 * @param[in] id peak ID
			 * @return peak charge
			 */
			int getMatrixPeakCharge( const int id );

        public:
			/**
			 * @fn int addSampleSet( const char* path )
			 * @brief adds sample set
			 * @param[in] path sample set file path
			 * @return sample set ID
			 */
			int addSampleSet( const char* path );

			/**
			 * @fn int getSampleSet( const char* path )
			 * @brief gets the sample set ID
			 * @param[in] path sample set file path
			 * @return sample set ID. If negative value, the specified sample set is not contained in the internal database.
			 */
			int getSampleSet( const char* path );

        public:
			/**
			 * @fn int addSample( kome::objects::Sample* sample )
			 * @brief adds sample
			 * @param[in] sample sample objectget
			 * return sample ID
			 */
			int addSample( kome::objects::Sample* sample );

			/**
			 * @fn int addSample( const char* path, const int sampleIndex, const char* name )
			 * @brief adds sample
			 * @param[in] path sample data file path
			 * @param[in] sampleIndex sample index
			 * @param[in] name sample name
			 */
			int addSample( const char* path, const int sampleIndex, const char* name );

			/**
			 * @fn int getSample( kome::objects::Sample* sample )
			 * @brief gets sample ID
			 * @param[in] sample sample
			 * @return sample ID. If negative value, the specified sample is not contained in the internal database.
			 */
			int getSample( kome::objects::Sample* sample );

			/**
			 * @fn int getSample( const char* path, const int sampleIndex );
			 * @brief gets sample ID
			 * @param[in] path sample data file path
			 * @param[in] sampleIndex sample index
			 * @return sample ID. If negative value, the specified sample is not contained in the internal database.
			 */
			int getSample( const char* path, const int sampleIndex );

			/**
			 * @fn void deleteSample( const int id )
			 * @brief deletes sample
			 * @param[in] id sample ID
			 */
			void deleteSample( const int id );

			/**
			 * @fn std::string getSampleName( const int sampleId )
			 * @brief gets the sample name
			 * @param[in] sampleId sample ID
			 * @return sample name
			 */
			std::string getSampleName( const int sampleId );

			/**
			 * @fn std::string getSampleFilePath( const int sampleId )
			 * @brief gets the sample file path
			 * @param[in] sampleId sample ID
			 * return sample file path
			 */
			std::string getSampleFilePath( const int sampleId );

			/**
			 * @fn int getSampleIndex( const int sampleId )
			 * @brief gets the sample index
			 * @param[in] sampleId sample ID
			 * @return sample index
			 */
			int getSampleIndex( const int sampleId );

			/**
			 * @fn kome::objects::Sample* openSample( const int id, SampleReadInfo& readInfo )
			 * @brief opens sample
			 * @param[in] id sample ID
			 * @param[out] readInfo sample open information. (This struct is used in releaseSample method.)
			 */
			kome::objects::Sample* openSample( const int id, SampleReadInfo& readInfo );

			/**
			 * @fn void releaseSample( kome::objects::Sample* sample, SampleReadInfo& readInfo )
			 * @brief releases sample
			 * @param[in] sample sample object
			 * @param[in] readInfo release informatin
			 */
			void releaseSample( kome::objects::Sample* sample, SampleReadInfo& readInfo );

        public:
			/**
			 * @fn int addMatrixSample(
						const int group,
						const int sample,
						const char* name,
						const COLOR color
					)
			 * @brief adds matrix sample
			 * @param[in] group group ID
			 * @param[in] sample sample ID
			 * @param[in] name matrix column name
			 * @param[in] color sample color
			 * @return matrix sample ID
			 */
			int addMatrixSample(
				const int group,
				const int sample,
				const char* name,
				const COLOR color
			);

			/**
			 * @fn void deleteMatrixSample( const int id )
			 * @brief deletes matrix sample
			 * @param[in] id matrix sample ID
			 */
			void deleteMatrixSample( const int id );

			/**
			 * @fn void updateMatrixSample( const int id, const char* name, const COLOR color )
			 * @brief updates matrix sample
			 * @param[in] id matrix sample ID
			 * @param[in] name matrix sample name
			 * @param[in] color matrix sample color
			 */
			void updateMatrixSample( const int id, const char* name, const COLOR color );

			/**
			 * @fn void getMatrixSamplesFromGroup( std::vector< int >& samples, const int group )
			 * @brief gets matrix samples of specified group
			 * @param[out] samples the array object to store matrix sample IDs.
			 * @param[in] group group ID
			 */
			void getMatrixSamplesFromGroup( std::vector< int >& samples, const int group );

			/**
			 * @fn void getMatrixSamplesFromSample( std::vector< int >& samples, const int sample )
			 * @brief gets matrix samples of specified sample
			 * @param[out] samples the array object to store matrix sample IDs.
			 * @param[in] sample sample ID
			 */
			void getMatrixSamplesFromSample( std::vector< int >& samples, const int sample );

			/**
			 * @fn int getMatrixSampleSample( const int id )
			 * @brief gets sample ID of the matrix sample
			 * @param[in] id matrix sample ID
			 * @return sample ID
			 */
			int getMatrixSampleSample( const int id );

			/**
			 * @fn int getMatrixSampleGroup( const int id )
			 * @brief gets group ID of the matrix sample
			 * @param[in] id matrix sample ID
			 * @return group ID
			 */
			int getMatrixSampleGroup( const int id );

			/**
			 * @fn std::string getMatrixSampleName( const int id )
			 * @brief return get matrix sample name
			 * @param[in] id matrix sample ID
			 * @return matrix sample name
			 */
			std::string getMatrixSampleName( const int id );

			/**
			 * @fn COLOR getMatrixSampleColor( const int id )
			 * @brief gets the matrix sample color
			 * @param[in] id matrix sample ID
			 * @return matrix sample color
			 */
			COLOR getMatrixSampleColor( const int id );

        public:
			/**
			 * @fn void setAppendedValue( const int peak, const int col, const char* val )
			 * @brief sets appended value
			 * @param[in] peak peak ID
			 * @param[in] col column ID
			 * @param[in] val value
			 */
			void setAppendedValue( const int peak, const int col, const char* val );

			/**
			 * @fn std::string getAppendedValue( const int peak, const int col )
			 * @brief gets the appended value
			 * @param[in] peak peak ID
			 * @param[in] col column ID
			 * @return appended value
			 */
			std::string getAppendedValue( const int peak, const int col );

			/**                   
			 * @fn void deleteAppendedValueFromPeak( const int peak )
			 * @brief deletes appended value from peak
			 * @param[in] peak peak ID
			 */
			void deleteAppendedValueFromPeak( const int peak );

			/**
			 * @fn void deleteAppendedValueFromMatrixColumn( const int col )
			 * @brief deletes appended value from matrix column
			 * @param[in] col column ID
			 */
			void deleteAppendedValueFromMatrixColumn( const int col );

        public:
			/**
			 * @fn int setProfile( kome::core::XYData& data, const bool isChrom )
			 * @brief sets the profile
			 * @param[in] data data points
			 * @param[in] isChrom chromatogram flag.
			 * @return profile ID
			 */
			int setProfile( kome::core::XYData& data, const bool isChrom );

			/**
			 * @fn void deleteProfile( const int id )
			 * @brief deletes profile
			 * @param[in] id profile ID
			 */
			void deleteProfile( const int id );

			/**
			 * @fn void getProfile( const int id, kome::core::XYData& data )
			 * @brief gets the profile
			 * @param[in] id profile ID
			 * @param[out] data the object to store profile data
			 */
			void getProfile( const int id, kome::core::XYData& data );

			/**
			 * @fn bool isChromatogramProfile( const int id )
			 * @brief gets the chromatogram flag value
			 * @param[in] id profile ID
			 * @return chromatogram flag value
			 */
			bool isChromatogramProfile( const int id );

        public:
			/**
			 * @fn void setPeakValue(
						const int matrixPeak,
						const int matrixSample,
						const double intensity,
						kome::objects::PeakElement* peak,
						const int profile
				);
			 * @brief sets the peak value
			 * @param[in] matrixPeak matrix peak ID
			 * @param[in] matrixSample matrix sample ID
			 * @param[in] intensity peak intensity
			 * @param[in] peak peak information
			 * @param[in] profile profile ID
			 */
			void setPeakValue(
				const int matrixPeak,
				const int matrixSample,
				const double intensity,
				kome::objects::PeakElement* peak,
				const int profile
			 );

			/**
			 * @fn void setNormalizedIntensity(
					const int matrixPeak,
					const int matrixSample,
					const double normalized
				)
			 * @brief sets the normalized intensity
			 * @param[in] matrixPeak matrix peak ID
			 * @param[in] matrixSample matrix sample ID
			 * @param[in] normalized normalized intensity
			 */
			void setNormalizedIntensity(
				const int matrixPeak,
				const int matrixSample,
				const double normalized
			);

			/**
			 * @fn void deletePeakValuesFromMatrixPeak( const int matrixPeak )
			 * @brief delete peak values of specified peak
			 * @param[in] matrixPeak matrix peak ID
			 */
			void deletePeakValuesFromMatrixPeak( const int matrixPeak );

			/**
			 * @fn void deletePeakValuesFromMatrixSample( const int matrixSample )
			 * @brief deletes peak values of specified sample
			 * @param[in] matrixSample matrix sample ID
			 */
			void deletePeakValuesFromMatrixSample( const int matrixSample );

			/**
			 * @fn double getPeakValue( const int matrixPeak, const int matrixSample )
			 * @brief getst the peak value
			 * @param[in] matrixPeak matrix peak ID
			 * @param[in] matrixSample matrix sample ID
			 * @return peak value
			 */
			double getPeakValue( const int matrixPeak, const int matrixSample );

			/**
			 * @fn double getNormalizedPeakValue( const int matrixPeak, const int matrixSample )
			 * @brief gets the normalized peak value
			 * @param[in] matrixPeak matrix peak ID
			 * @param[in] matrixSample matrix sample ID
			 * @return normalized peak value
			 */
			double getNormalizedPeakValue( const int matrixPeak, const int matrixSample );

			/**
			 * @fn bool hasBlankValue( const int matrixId )
			 * @brief judges the specified peak matrix has blank peak value.
			 * @param[in] matrixId peak matrix ID
			 * @return If true, the specified peak matrix has blank peak value
			 */
			bool hasBlankValue( const int matrixId );

			/**
			 * @fn bool isEmptySample( const int matrixSample )
			 * @brief judges the column of the specified sample is empty or not.
			 * @param[in] matrixSample matrix sample ID
			 * @return If true, the column of the specified sample is empty.
			 */
			 bool isEmptySample( const int matrixSample );

			/**
			 * @fn void getPeakInfo(
					 const int matrixPeak,
					 const int matrixSample,
					 kome::objects::PeakElement* peak
				 )
			 * @brief gets the peak information
			 * @param[in] matrixPeak  matrix peak ID
			 * @param[in] matrixSample matrix sample ID
			 * @param[out] peak the object to store peak information
			 */
			 void getPeakInfo(
				const int matrixPeak,
				const int matrixSample,
				kome::objects::PeakElement* peak
			);

			/**
			 * @fn int getProfile( const int matrixPeak, const int matrixSample )
			 * @brief getst the profile
			 * @param[in] matrixPeak  matrix peak ID
			 * @param[in] matrixSample matrix sample ID
			 * @return profile ID
			 */
			int getProfile( const int matrixPeak, const int matrixSample );

        public:
			/**
			 * @fn void getSpecInfo( kome::objects::Spectrum& spec, SpecInfo& specInfo )
			 * @brief gets SpecInfo structure
			 * @param[in] spec spectrum
			 * @param[out] specInfo SpecInfo structure to store spectrum information.
			 */
			void getSpecInfo( kome::objects::Spectrum& spec, SpecInfo& specInfo );

			/**
			 * @fn int addSpectrum( kome::objects::Spectrum* spec )
			 * @brief adds spectrum
			 * @param[in] spec spectrum
			 * @return spectrum ID
			 */
			int addSpectrum( kome::objects::Spectrum* spec );

			/**
			 * @fn int addSpectrum( SpecInfo& spec )
			 * @brief adds spectrum
			 * @param[in] spec spectrum
			 * @return spectrum ID
			 */
			int addSpectrum( SpecInfo& spec );

			/**
			 * @fn int addSpectrum( SpecInfo& spec )
			 * @brief adds spectrum
			 * @param[in] spec spectrum
			 */
			void deleteSpectrum( const int id );

			/**
			 * @fn void getSpectra( std::vector< int >& spectra, const int sampleId )
			 * @brief gets spectra
			 * @param[out] spectra the array object to store spectrum IDs.
			 * @param[in] sampleId sample ID
			 */
			void getSpectra( std::vector< int >& spectra, const int sampleId );

			/**
			 * @fn void getSpectrum( const int id, SpecInfo& spec )
			 * @brief gets the spectrum information
			 * @param[in] id spectrum ID
			 * @param[out] spec the structure to store spectrum infromation.
			 */
			void getSpectrum( const int id, SpecInfo& spec );

        public:
			/**
			 * @fn int addDatabase( const char* name, const char* url )
			 * @brief adds databases
			 * @param[in] name database name
			 * @param[in] url database URL
			 * @return datatbase ID
			 */
			int addDatabase( const char* name, const char* url );

			/**
			 * @fn int getDatabaseId( const char* name )
			 * @brief gets the database ID
			 * @param[in] name database name
			 */
			int getDatabaseId( const char* name );

			/**
			 * @fn void deleteDatabase( const int id )
			 * @brief deletes database
			 * @param[in] id database ID
			 */
			void deleteDatabase( const int id );

			/**
			 * @fn void getDatabases( std::vector< int >& databases )
			 * @brief gets database IDs.
			 * @param[out] databases the array object to store database IDs.
			 */
			void getDatabases( std::vector< int >& databases );

			/**
			 * @fn std::string getDatabaseName( const int id )
			 * @brief gets the database name
			 * @param[in] id database ID
			 * @return database name
			 */
			std::string getDatabaseName( const int id );

			/**
			 * @fn std::string getDatabaseUrl( const int id )
			 * @brief gets the database URL
			 * @param[in] id database ID
			 * @return database URL
			 */
			std::string getDatabaseUrl( const int id );

        protected:
			/**
			 * @fn int getFormatId( const char* format )
			 * @brief gets the format URL
			 * @param[in] format format name
			 * @return format ID
			 */
			int getFormatId( const char* format );

        public:
			/**
			 * @fn void setFormatUrl( const int dbId, const char* format, const char* url )
			 * @brief set format URL
			 * @param[in] dbId database ID
			 * @param[in] format format name
			 * @param[in] url format URL
			 */
			void setFormatUrl( const int dbId, const char* format, const char* url );

			/**
			 * @fn void deleteFormat( const char* format )
			 * @brief  delete format
			 * @param[in] format format name
			 */
			void deleteFormat( const char* format );

			/**
			 * @fn void deleteFormatUrl( const int dbId, const char* format )
			 * @brief deletes format URL
			 * @param[in] dbId database ID
			 * @param[in] format format name
			 */
			void deleteFormatUrl( const int dbId, const char* format );

			/**
			 * @fn void deleteFormatUrl( const int dbId )
			 * @brief deletes fromat URL
			 * @param[in] dbId database ID
			 */
			 void deleteFormatUrl( const int dbId );

			 /**
			  * @fn std::string getFormatUrl( const int dbId, const char* format )
			  * @brief gets format URL
			  * @param[in] dbId database ID
			  * @param[in] format format name
			  * @return format URL
			  */
			std::string getFormatUrl( const int dbId, const char* format );

			/**
			 * @fn void getFormatList( std::vector< std::string >& formats, const int dbId )
			 * @brief gets format list
			 * @param[out] formats the array object to store format list.
			 * @param[in] dbId database ID
			 */
			void getFormatList( std::vector< std::string >& formats, const int dbId );

        protected:
			/**
			 * @fn int addSubstanceItem( const int dbId, const char* name, const SubstanceType type )
			 * @brief adds substance item
			 * @param[in] dbId database ID
			 * @param[in] name item name
			 * @param[in] type substance type
			 * @return item ID
			 */
			int addSubstanceItem( const int dbId, const char* name, const SubstanceType type );

			/**
			 * @fn void deleteSubstanceItem( const int dbId )
			 * @brief deletes substance item
			 * @param[in] dbId database ID
			 */
			void deleteSubstanceItem( const int dbId );

			/**
			 * @fn void setSubstanceValue( const int substanceId, const int itemId, const char* val )
			 * @brief sets the substance value
			 * @param[in] substanceId substance ID
			 * @param[in] itemId item ID
			 * @param[in] val value
			 */
			void setSubstanceValue( const int substanceId, const int itemId, const char* val );

			/**
			 * @fn void deleteSubstanceValue( const int substanceId )
			 * @brief deletes substance value from substance
			 * @param[in] substanceId substance ID
			 */
			void deleteSubstanceValue( const int substanceId );

        public:
			/**
			 * @fn int addSubstance( SubstanceInfo& substance )
			 * @brief adds substance
			 * @param[in] substance information
			 * @return substance ID
			 */
			int addSubstance( SubstanceInfo& substance );

			/**
			 * @fn int getSubstanceId(
						const int dbId,
						const char* accession,
						const char* name,
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
					const int dbId,
					const char* accession,
					const char* name,
					int parentId
			);

			/**
			 * @fn void deleteSubstance( const int id )
			 * @brief deletes substance
			 * @param[in] id substance ID
			 */
			void deleteSubstance( const int id );

			/**
			 * @fn void getSubstances( std::vector< int >& substances, const int dbId )
			 * @brief gets substance IDs
			 * @param[out] substances the array to store substance IDs.
			 * @param[in] dbId database ID
			 */
			void getSubstances( std::vector< int >& substances, const int dbId );

			/**
			 * @fn void getSubstances( std::vector< int >& substances )
			 * @brief gets substance IDs
			 * @param[out] substances the array to store substance IDs.
			 */
			void getSubstances( std::vector< int >& substances );

			/**
			 * @fn void getSubstance( const int id, SubstanceInfo& substance )
			 * @brief gets substance
			 * @param[in] id substance ID
			 * @param[out] substance the structure to store substance information
			 */
			void getSubstance( const int id, SubstanceInfo& substance );

        protected:

			/**
			 * @fn Substance* getSubstanceObject( const int id, SearchResult& result, std::map< int, Substance* >& substanceMap )
			 * @brief gets substance object
			 * @param[in] id substance ID
			 * @param[in] result result object
			 * @param[in,out] substanceMap substance map
			 * @return substance object
			 */
			Substance* getSubstanceObject( const int id, SearchResult& result, std::map< int, Substance* >& substanceMap );

        public:
			/**
			 * @fn int addSearchEngine( const char* name, const char* version )
			 * @brief adds search engine
			 * @param[in] name search engine name
			 * @param[in] version search engine version
			 * @return search engine ID
			 */
			int addSearchEngine( const char* name, const char* version );

			/**
			 * @fn void deleteSearchEngine( const int id )
			 * @brief deletes search engine
			 * @param[in] id search engine ID
			 */
			void deleteSearchEngine( const int id );

			/**
			 * @fn void getSearchEngines( std::vector< int >& engines )
			 * @brief gets search engines
			 * @param[out] engines the array to store search engine IDs.
			 */
			void getSearchEngines( std::vector< int >& engines );

			/**
			 * @fn std::string getSearchEngineName( const int id )
			 * @brief gets the search engine name
			 * @param[in] id search engine ID
			 * @return search engine name
			 */
			std::string getSearchEngineName( const int id );

			/**
			 * @fn std::string getSearchEngineVersion( const int id )
			 * @brief search engine version
			 * @param[in] id search engine ID
			 * @return search engine version
			 */
			std::string getSearchEngineVersion( const int id );

        protected:
			/**
			 * @fn int addSearchItem( const int engineId, const char* name )
			 * @brief adds search item
			 * @param[in] engineId search engine ID
			 * @param name item name
			 * @return item ID
			 */
			int addSearchItem( const int engineId, const char* name );

			/**
			 * @fn void deleteSearchItem( const int engineId )
			 * @brief deletes search item
			 * @param[in] engineId search engine ID
			 */
			void deleteSearchItem( const int engineId );

			/**
			 * @fn void setSearchValue( const int itemId, const int searchId, const char* val )
			 * @brief sets search value
			 * @param[in] itemId item ID
			 * @param[in] searchId search ID
			 * @param[in] val value
			 */
			void setSearchValue( const int itemId, const int searchId, const char* val );

			/**
			 * @fn void deleteSearchValue( const int searchId )
			 * @brief deletes search value
			 * @param[in] searchId search ID
			 */
			void deleteSearchValue( const int searchId );

        public:
			/**
			 * @fn int addPeakDetection( const char* method )
			 * @brief adds peak detection
			 * @param[in] method peak detection method
			 * @return peak detection ID
			 */
			int addPeakDetection( const char* method );

			/**
			 * @fn void setPeakParameter( const int detect, const char* name, const char* value )
			 * @brief sets peak parameters
			 * @param[in] detect peak detection ID
			 * @param[in] name parameter name
			 * @param[in] value parameter value
			 */
			void setPeakParameter( const int detect, const char* name, const char* value );

			/**
			 * @fn void deletePeakDetection( const int detect )
			 * @brief deleted peak detection
			 * @param[in] detect peak detection ID
			 */
			void deletePeakDetection( const int detect );

			/**
			 * @fn std::string getPeakDetectionMethod( const int detect )
			 * @brief gets the peak detection method
			 * @param[in] detect peak detection ID
			 * @return peak detection method
			 */
			std::string getPeakDetectionMethod( const int detect );

			/**
			 * @fn void getPeakDetectionParameters( const int detect, kome::core::Properties& props )
			 * @brief gets peak detection parameters
			 * @param[in] detect peak detection ID
			 * @param[out] props peak detection parameters
			 */
			void getPeakDetectionParameters( const int detect, kome::core::Properties& props );

			/**
			 * @fn void deleteUnlinkedPeakDetection()
			 * @brief deletes unlinked peak detection
			 */
			void deleteUnlinkedPeakDetection();

		public:
			/**
			 * @fn int setPeaks( kome::core::XYData* peaks, const int peakDetection )
			 * @brief set peaks
			 * @param[in] peaks peaks
			 * @param[in] peakDetection peak detection ID
			 * @return peaks ID
			 */
			int setPeaks( kome::core::XYData* peaks, const int peakDetection );

			/**
			 * @fn int getPeaks( const int peaksId, kome::core::XYData* peaks )
			 * @brief gets peaks
			 * @param[in] peaksId peaks ID
			 * @param[out] peaks the object to store peaks information.
			 * @return peak detection ID
			 */
			int getPeaks( const int peaksId, kome::core::XYData* peaks );

			/**
			 * @fn void deletePeaks( const int peaks )
			 * @brief deletes peaks
			 * @param[in] peaks peaks ID
			 */
			void deletePeaks( const int peaks );

			/**
			 * @fn void deleteUnlinkedPeaks()
			 * @brief deletes unlinked peaks
			 */
			void deleteUnlinkedPeaks();

        public:
			/**
			 * @fn int addSearch( SearchInfo& search )
			 * @brief adds search
			 * @param[in] search search information
			 * @return search ID
			 */
			int addSearch( SearchInfo& search );

			/**
			 * @fn void deleteSearch( const int id )
			 * @brief deletes search
			 * @param[in] id search ID
			 */
			void deleteSearch( const int id );

			/**
			 * @fn void updateSearch( const int id, SearchInfo& search )
			 * @brief updates search
			 * @param[in] id search ID
			 * @param[in] search search information
			 */
			void updateSearch( const int id, SearchInfo& search );

			/**
			 * @fn void setSearchStatus( const int id, SearchEngine::SearchStatus status )
			 * @brief sets the search status
			 * @param[in] id search ID
			 * @param[in] status search status
			 */
			void setSearchStatus( const int id, SearchEngine::SearchStatus status );

			/**
			 * @fn SearchEngine::SearchStatus getSearchStatus( const int id )
			 * @brief gets the search status
			 * @param[in] id search ID
			 * @return search status
			 */
			SearchEngine::SearchStatus getSearchStatus( const int id );

			/**
			 * @fn void setResultLocation( const int id, const char* location )
			 * @brief sets the result location
			 * @param[in] id search ID
			 * @param[in] location
			 */
			void setResultLocation( const int id, const char* location );

			/**
			 * @fn std::string getResultLocation( const int id )
			 * @brief gets the result location
			 * @param[in] id
			 * @return result location
			 */
			std::string getResultLocation( const int id );

			/**
			 * @fn void getSearchesFromEngine( std::vector< int >& searches, const int engineId )
			 * @brief gets searches from search engine
			 * @param[out] searches the array object to store search IDs.
			 * @param[in] engineId search engine ID
			 */
			void getSearchesFromEngine( std::vector< int >& searches, const int engineId );

			/**
			 * @fn void getSearchesFromDb( std::vector< int >& searches, const int dbId )
			 * @brief gets searches from the database
			 * @param[out] searches the array object to store search IDs.
			 * @param[in] dbId database ID
			 */
			void getSearchesFromDb( std::vector< int >& searches, const int dbId );

			/**
			 * @fn void getSearchesFromMatrix( std::vector< int >& searches, const int matrixId )
			 * @brief gets searches from the matrix
			 * @param[out] searches the array object to store search IDs.
			 * @param[in] matrixId peak matrix ID
			 */
			void getSearchesFromMatrix( std::vector< int >& searches, const int matrixId );

			/**
			 * @fn void getSearchesFromSample( std::vector< int >& searches, kome::objects::Sample* sample )
			 * @brief gets searches from sample
			 * @param[out] searches the array object to store search IDs.
			 * @param[in] sample
			 */
			void getSearchesFromSample( std::vector< int >& searches, kome::objects::Sample* sample );

			/**
			 * @fn void getSearches( std::vector< int >& searches )
			 * @brief gets all searches
			 * @param[out] searches the array object to store search IDs.
			 */
			void getSearches( std::vector< int >& searches );

			/**
			 * @fn void getSearch( const int id, SearchInfo& search )
			 * @brief gets the search information 
			 * @param[in] id search ID
			 * @param[out] search search structure to store search information
			 */
			void getSearch( const int id, SearchInfo& search );

			/**
			 * @fn void getSearch( const int id, SearchResult& result )
			 * @brief gets the search result
			 * @param[in] id search ID
			 * @param[out] result the object to store search result
			 */
			void getSearch( const int id, SearchResult& result );
			
			/**
			 * @fn int getSearch( const char* engine, const char* version, const char* searchId )
			 * @brief gets the search engine
			 * @param[in] engine search engine name
			 * @param[in] version search engine version
			 * @param[in] searchId search ID
			 * @return ID
			 */
			int getSearch( const char* engine, const char* version, const char* searchId );

			/**
			 * @fn std::string getSearchId( const int id )
			 * @brief gets the string search ID
			 * @param[in] id integer search ID
			 * @return string search ID
			 */
			std::string getSearchId( const int id );

        public:
			/**
			 * @fn int setSearchSpectrum( const int specId, const int searchId, const int peaks, const double precursor, std::vector< int >& charges )
			 * @brief sets search spectrum
			 * @param[in] specId spectrum ID
			 * @param[in] searchId search ID
			 * @param[in] peaks peaks ID
			 * @param[in] precursor precursor mass
			 * @param[in] charges charges
			 * @return search spectrum ID
			 */
			int setSearchSpectrum( const int specId, const int searchId, const int peaks, const double precursor, std::vector< int >& charges );

			/**
			 * @fn void deleteSearchSpectrum( const int id )
			 * @brief deletes search spectrum
			 * @param[in] id search spectrum ID
			 */
			void deleteSearchSpectrum( const int id );

			/**
			 * @fn void getSearchSpectraFromSearch( std::vector< int >& spectra, const int searchId )
			 * @brief gets search spectra from search
			 * @param[out] spectra the array object to store search spectrum IDs.
			 * @param[in] searchId search ID
			 */
			void getSearchSpectraFromSearch( std::vector< int >& spectra, const int searchId );

			/**
			 * @fn void getSearchSpectraFromSpectrum( std::vector< int >& spectra, const int specId )
			 * @brief gets search spectra from sepectrum
			 * @param[out] spectra the array object to store search spectrum IDs.
			 * @param[in] specId spectrum ID
			 */
			void getSearchSpectraFromSpectrum( std::vector< int >& spectra, const int specId );

			/**
			 * @fn int getSearchSpectrum( const int searchId, const int specId )
			 * @brief gets the search spectrum ID
			 * @param[in] searchId search ID
			 * @param[in] specId spectrum ID
			 * @return search spectrum ID
			 */
			int getSearchSpectrum( const int searchId, const int specId );

			/**
			 * @fn void getSearchSpectra( std::vector< int >& spectra )
			 * @brief gets all search spectra
			 * @param[out] spectra the array object to store search spectrum IDs.
			 */
			void getSearchSpectra( std::vector< int >& spectra );

			/**
			 * @fn void getSearchSpectrum( const int id, int* specId, int* searchId, int* peaks, double* precursor )
			 * @brief gets search spectrum information
			 * @param[in] id search spectrum ID
			 * @param[out] specId the pointer to store spectrum ID,
			 * @param[out] searchId the pointer to store search ID
			 * @param[out] peaks the pointer to store peak information
			 * @param[out] precursor the pointer to store precursor
			 */
			void getSearchSpectrum( const int id, int* specId, int* searchId, int* peaks, double* precursor );

			/**
			 * @fn void getCharges( const int searchSpecId, std::vector< int >& charges )
			 * @brief gets charges
			 * @param[in] searchSpecId search spectrum ID
			 * @param[out] charges the vector to store charges
			 */
			void getCharges( const int searchSpecId, std::vector< int >& charges );

        public:
			/**
			 * @fn int setMatrixPeakSpectrum( const int peakId, const int searchSpecId )
			 * @brief sets matrix peak spectrum
			 * @param[in] peakId matrix peak ID
			 * @param[in] searchSpecId search spectrum ID
			 * @return matrix peak spectrum ID
			 */
			int setMatrixPeakSpectrum( const int peakId, const int specId );

			/**
			 * @fn void deleteMatrixPeakSpectrum( const int id )
			 * @brief deletes matix peak spectrum
			 * @param[in] id matrix peak spectrum ID
			 */
			void deleteMatrixPeakSpectrum( const int id );

			/**
			 * @fn void getMatrixPeakSpectraFromSpectrum( std::vector< int >& spectra, const int searchSpecId );
			 * @brief gets matrix peak spectra from spectrum
			 * @param[out] spectra the array object to store matrix peak spectrum IDs.
			 * @param[in] searchSpecId search spectrum ID
			 */
			void getMatrixPeakSpectraFromSpectrum( std::vector< int >& spectra, const int searchSpecId );

			/**
			 * @fn void getMatrixPeakSpectraFromPeak( std::vector< int >& spectra, const int peakId )
			 * @brief gets matrix peak spectra from matrix peak.
			 * @param[out] spectra the array object to store matrix peak spectrum IDs.
			 * @param[in] peakId matrix peak ID
			 */
			void getMatrixPeakSpectraFromPeak( std::vector< int >& spectra, const int peakId );


			/**
			 * @fn void getMatrixPeakSpectraFromSearchAndPeak( std::vector< int >& spectra, const int searchId, const int peakId )
			 * @brief gets matrix peak spectra from search and matrix peak
			 * @param[out] spectra the array object to store matrix peak spectrum IDs.
			 * @param[in] searchId search ID
			 * @param[in] peakId matrix peak ID
			 */
			void getMatrixPeakSpectraFromSearchAndPeak( std::vector< int >& spectra, const int searchId, const int peakId );

			/**
			 * @fn void getMatrixPeakSpectra( std::vector< int >& spectra )
			 * @brief gets all matrix peak spectra
			 * @param[out] spectra the array object to store matrix peak spectrum IDs.
			 */
			void getMatrixPeakSpectra( std::vector< int >& spectra );

			/**
			 * @fn int getMatrixPeakSpectrumPeak( const int id )
			 * @brief gets matrix peak from matrix peak spectrum ID
			 * @param[in] id matrix peak spectrum ID
			 * @return matrix peak ID
			 */
			int getMatrixPeakSpectrumPeak( const int id );

			/**
			 * @fn int getMatrixPeakSpectrumSpectrum( const int id )
			 * @brief gets search spectrum from matrix peak spectrum ID
			 * @param[in] id matrix peak spectrum ID
			 * @return search spectrum ID
			 */
			int getMatrixPeakSpectrumSpectrum( const int id );

        protected:
			/**
			 * @fn int addHitItem(
						const int engineId,
						const char* name,
						const SubstanceType subType,
						const ValueType valType
					)
			 * @brief adds hit item
			 * @param[in] engineId search engine ID
			 * @param[in] name item name
			 * @param[in] subType substance type
			 * @param[in] valType value type
			 * @return item ID
			 */
			int addHitItem(
					const int engineId,
					const char* name,
					const SubstanceType subType,
					const ValueType valType
			);

			/**
			 * @fn void deleteHitItem( const int engineId )
			 * @brief deletes hit item
			 * @param[in] engineId search engine ID
			 */
			void deleteHitItem( const int engineId );

			/**
			 * @fn void setHitValue( const int hitId, const int itemId, const char* val )
			 * @brief sets the hit value
			 * @param[in] hitId hit ID
			 * @param[in] itemId item ID
			 * @param[in] val hit value
			 */
			void setHitValue( const int hitId, const int itemId, const char* val );

			/**
			 * @fn void setHitValue( const int hitId, const int itemId, const char* val )
			 * @brief deletes hit value
			 * @param[in] hitId hit ID
			 */
			void deleteHitValue( const int hitId );

			/**
			 * @fn void addHitSpectrum( const int hitId, const int searchSpecId, const int charge )
			 * @brief adds hit spectrum
			 * @param[in] hitId hit ID
			 * @param[in] searchSpecId search spectrum ID
			 * @param[in] charge charge
			 */
			void addHitSpectrum( const int hitId, const int searchSpecId, const int charge );

			/**
			 * @fn void deleteHitSpectraFromHit( const int hitId )
			 * @brief deletes hit spectra from hit
			 * @param[in] hitId hit ID
			 */
			void deleteHitSpectraFromHit( const int hitId );

			/**
			 * @fn void deleteHitSpectraFromSpectrum( const int searchSpecId )
			 * @brief deletes hit spectra from spectrum
			 * @param[in] searchSpecId search spectrum ID
			 */
			void deleteHitSpectraFromSpectrum( const int searchSpecId );

		public:
			/**
			 * @fn int addHit( HitInfo& hit )
			 * @brief adds hit information
			 * @param[in] hit hit information
			 * @return hit ID
			 */
			int addHit( HitInfo& hit );

			/**
			 * @fn void deleteHit( const int id )
			 * @brief deletes hit item
			 * @param[in] id hit ID
			 */
			void deleteHit( const int id );
			
			/**
			 * @fn void getHitsFromSubstance( std::vector< int >& hits, const int substanceId )
			 * @brief gets hits from substance
			 * @param[out] hits the array object to store hit IDs.
			 * @param[in] substanceId substance ID
			 */
			 void getHitsFromSubstance( std::vector< int >& hits, const int substanceId );

			 /**
			  * @fn void getHitsFromSpectrum( std::vector< int >& hits, const int searchId )
			  * @brief gets hits from search
			  * @param[out] hits the array object to store hit IDs.
			  * @param[in] searchSpecId search spectrum ID
			  */
			void getHitsFromSearch( std::vector< int >& hits, const int searchId );

			/**
			 * @fn void getHitsFromSpectrum( std::vector< int >& hits, const int searchSpecId )
			 * @brief gets hits from spectrum
			 * @param[out] hits the array object to store hit IDs.
			 * @param[in] searchSpecId search spectrum ID
			 */
			void getHitsFromSpectrum( std::vector< int >& hits, const int searchSpecId );

			/**
			 * @fn void getHits( std::vector< int >& hits )
			 * @brief gets all hits
			 * @param[out] hits the array to store hit IDs.
			 */
			void getHits( std::vector< int >& hits );

			/**
			 * @fn void getHit( const int id, HitInfo& hit )
			 * @brief gets hit information
			 * @param[in] id hit ID
			 * @param[out] hit the structure to store hit information.
			 */
			void getHit( const int id, HitInfo& hit );

		protected:
			/**
			 * @fn void addAltSubstance( const int hitId, const int substanceId, const char* before, const char* after, int start, int end )
			 * @brief adds alternative substance
			 * @param[in] hitId hit ID
			 * @param[in] substanceId substance ID
			 * @param[in] before before
			 * @param[in] after after
			 * @param[in] start start position
			 * @param[in] end end position
			 */
			void addAltSubstance( const int hitId, const int substanceId, const char* before, const char* after, int start, int end );

			/**
			 * @fn void deleteAltSubstancesByHit( const int hitId )
			 * @brief deletes alternative substances by hit
			 * @param[in] hitId hit ID
			 */
			void deleteAltSubstancesByHit( const int hitId );

			/**
			 * @fn void deleteAltSubstancesBySubstance( const int substanceId )
			 * @brief deletes alternative subsntances by substance
			 * @param[in] substanceId substance ID
			 */
			void deleteAltSubstancesBySubstance( const int substanceId );

			/**
			 * @fn void getAltSubstances( const int hitId, std::vector< AltSubstance >& substances )
			 * @brief gets alternative substances
			 * @param[in] hitId hit ID
			 * @param[out] substances the array object to store alternative substances.
			 */
			void getAltSubstances( const int hitId, std::vector< AltSubstance >& substances );

        public:
			/**
			 * @fn void setAccessionNumber( const char* prefix, const int num )
			 * @brief sets accession number
			 * @param[in] prefix the prefix of the accession
			 * @param[in] num accession number
			 */
			void setAccessionNumber( const char* prefix, const int num );

			/**
			 * @fn int getAccessionNumber( const char* prefix )
			 * @brief gets the current accession number
			 * @param[in] prefix the prefix of the accession
			 * @return accession number
			 */
			int getAccessionNumber( const char* prefix );

			/**
			 * @fn int issueAccessionNumber( const char* prefix )
			 * @brief count up the accession number
			 * @param[in] prefix the prefix of the accession
			 * @return the current accession number (befor counting up)
			 */
			 int issueAccessionNumber( const char* prefix );

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

		protected:
			/**
			 * @fn void setDbVersion( const int version )
			 * @brief sets the db version.
			 * @param[in] version db version
			 */
			void setDbVersion( const int version );

        public:
			/**
			 * @fn bool convertDbSchema( kome::core::Progress& progress )
			 * @brief converts database table
			 * @param[in] progress the object to show the progress
			 * @return If true, succeeded to convert db schema
			 */
			bool convertDbSchema( kome::core::Progress& progress );

        protected:
			/**
			 * @fn void convertVersion1ToVersion2( kome::core::Progress& progress )
			 * @brief converts database version 1 to version 2
			 * @param[in] progress the object to show the progress
			 */
			void convertVersion1ToVersion2( kome::core::Progress& progress );

			/**
			 * @fn void convertVersion2ToVersion3( kome::core::Progress& progress )
			 * @brief converts database version 2 to version 3
			 * @param[in] progress the object to show the progress
			 */
			void convertVersion2ToVersion3( kome::core::Progress& progress );

			/**
			 * @fn void convertVersion3ToVersion4( kome::core::Progress& progress )
			 * @brief converts database version 3 to version 4
			 * @param[in] progress the object to show the progress
			 */
			void convertVersion3ToVersion4( kome::core::Progress& progress );

			/**
			 * @fn void convertVersion4ToVersion5( kome::core::Progress& progress )
			 * @brief converts database version 4 to version 5
			 * @param[in] progress the object to show the progress
			 */
			void convertVersion4ToVersion5( kome::core::Progress& progress );

			/**
			 * @fn void convertVersion5ToVersion6( kome::core::Progress& progress )
			 * @brief converts database version 5 to version 6
			 * @param[in] progress the object to show the progress
			 */
			void convertVersion5ToVersion6( kome::core::Progress& progress );

			/**
			 * @fn void convertVersion6ToVersion7( kome::core::Progress& progress )
			 * @brief converts database version 6 to version 7
			 * @param[in] progress the object to show the progress
			 */
			void convertVersion6ToVersion7( kome::core::Progress& progress );

			/**
			 * @fn void convertVersion7To8( kome::core::Progress& progress )
			 * @brief converts database version 7 to version 8
			 * @param[in] progress the object to show the progress
			 */
			void convertVersion7To8( kome::core::Progress& progress );

			/**
			 * @fn void convertVersion8To9( kome::core::Progress& progress )
			 * @brief converts database version 8 to version 9
			 * @param[in] progress the object to show the progress
			 */
			void convertVersion8To9( kome::core::Progress& progress );

		public:
			/**
			 * @fn void getPeakList( const char* peakList, kome::core::XYData& xyData )
			 * @brief gets the peak list
			 * @param[in] peakList the character string to get the peak list
			 * @param[out] xyData the object to store peak list
			 */
			void getPeakList( const char* peakList, kome::core::XYData& xyData );

		public:
			/** Substance Property */
			SubstanceProperty m_substanceProperty;

			/**
			 * @fn void setSubstanceProperty( SubstanceProperty& substProp )
			 * @brief set Substance Property
			 * @param[in] substProp Substance Property
			 */
			void setSubstanceProperty( SubstanceProperty substProp );

			/**
			 * @fn void getSubstanceProperty( SubstanceProperty& substProp )
			 * @brief get Substance Property
			 * @param[in] substProp Substance Property
			 */
			void getSubstanceProperty( SubstanceProperty& substProp );

			/** prepare db thread flag */
			bool	m_bPrepareDBThread;
			
		public:
			/**
			 * @fn void	setPrepareDBThreadCondition( const bool bSet )
			 * @brief set prepare db thread condition
			 * @param bSet status flag
			 */
			void	setPrepareDBThreadCondition( const bool bSet );

			/**
			 * @fn bool	getPrepareDBThreadCondition()
			 * @brief get prepare db thread condition
			 * @return thread conditon
			 */
			bool	getPrepareDBThreadCondition();

        protected:
			/**
			 * @fn static bool copyFileIfNew( const char* srcFile, const char* dstFile )
			 * @brief copy file If the original file is newer.
			 * @param[in] srcFile original file path
			 * @param[in] dstFile file path to be copied.
			 * @return If true, copying file is succeeded.
			 */
			static bool copyFileIfNew( const char* srcFile, const char* dstFile );
			
        public:
			/**
			 * @fn static IdentificationManager& getInstance( void )
			 * @brief get identification manager object (This is the only object)
			 * @return identification manager object
			 */
			static IdentificationManager& getInstance( void );
		};
	}
}
