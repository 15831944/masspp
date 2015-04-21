/**
 * @file SearchResult.h
 * @brief interfaces of SearchResult class
 *
 * @author H.Parry
 * @date 2012.01.25
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */

#pragma once

namespace kome {
	namespace ident {

		class SearchEngine;
		class Hit;
		class Enzyme;
		class Modification;

		/**
		 * @class SearchResult
		 * @brief search result information class
		 */
		class IDENT_TOOLS_CLASS SearchResult
		{
		public:
			/**
			 * @fn SearchResult( void )
			 * @brief constructor
			 */
			SearchResult( void );

			/**
			 * @fn ~SearchResult( void )
			 * @brief destructor
			 */
			~SearchResult( void );

		public:
			/**
			 * @typedef MassType
			 * @brief mass type
			 */
			typedef enum {
				MASS_UNDEFINED = 0,
				MASS_MONOISOTOPIC = 1,
				MASS_AVERAGE = 2,
				MASS_UNKNOWN = 3
			} MassType;
				
		protected:
			/** spectrum info list */
			std::vector<SpecInfo*> m_spectrumInfoList;

			/** search parameters */
			kome::core::Properties m_searchParameters;

			/** std::string */
			std::string m_peakListFile;

			/** Database name */
			std::string m_db;

			/** Search engine used */
			SearchEngine* m_pSrchEngine;

			/** search ID */
			std::string m_searchId;

			/** Result location */
			std::string m_resultLocation;

			/** peak detection parameters */
			kome::core::Properties m_peakdetectionParameters;

			/** Search date/time */
			std::string m_date;

			/** Array of hits */
			std::vector<Hit*>	m_arrHits;

			/** Array of substances */
			std::vector<Substance*>	m_arrSubstances;

			/** enzyme */
			Enzyme* m_enzyme;

			/** base name */
			std::string m_baseName;

			/** precursor mass type */
			MassType m_precursorMassType;

			/** fragment mass type */
			MassType m_fragmentMassType;

			/** fixed modificatinos */
			std::vector< Modification* > m_fixedMods;

			/** variable modifications */
			std::vector< Modification* > m_varMods;

			/** title */
			std::string m_title;

			/** comment */
			std::string m_comment;

		public:
			/**
			 * @fn void clear( void )
			 * @brief reset search result
			 */
			void clear( void );

			/**
			 * @fn std::vector<SpecInfo>& getSpectrumInfoList( void )
			 * @brief get the spectrum info list
			 * @return spectrum info list
			 */
			std::vector<SpecInfo*>& getSpectrumInfoList( void );

			/**
			 * @fn kome::core::Properties& getSearchParameters( void )
			 * @brief get the search parameters object
			 * @return search parameters
			 */
			kome::core::Properties& getSearchParameters( void );

			/**
			 * @fn void setTitle( const char* title )
			 * @brief sets the title
			 * @param[in] title title
			 */
			void setTitle( const char* title );

			/**
			 * @fn std::string getTitle( void )
			 * @brief get the search title
			 * @return search title
			 */
			std::string getTitle( void );

			/**
			 * @fn void setComment( const char* comment )
			 * @brief sets the comment
			 * @param[in] comment comment
			 */
			void setComment( const char* comment );

			/**
			 * @fn std::string getComment( void )
			 * @brief get the search comment
			 * @return search comment
			 */
			std::string getComment( void );

			/**
			 * @fn void setDb( const char* DbName )
			 * @brief set the name of the database used
			 * @param[in] DbName database name
			 */
			void setDb( const char* DbName );

			/**
			 * @fn std::string getDb( void ) const
			 * @brief get the name of the database used
			 * @return database name
			 */
			std::string getDb( void ) const;

			/**
			 * @fn void setSearchEngine( SearchEngine* srchEngine )
			 * @brief set the search engine used
			 * @param[in] srchEngine pointer to a SearchEngine object
			 */
			void setSearchEngine( SearchEngine* srchEngine );

			/**
			 * @fn SearchEngine* getSearchEngine( void ) const
			 * @brief get the search engine used
			 * @return search engine
			 */
			SearchEngine* getSearchEngine( void ) const;

			/**
			 * @fn void setSearchId( const char* id )
			 * @brief sets the search ID
			 * @param[in] id search ID
			 */
			void setSearchId( const char* id );

			/**
			 * @fn std::string getSearchId() const
			 * @brief gets the search ID
			 * @return search ID
			 */
			std::string getSearchId() const;

			/**
			 * @fn void setResultLocation( const char* resultLocation )
			 * @brief set the result location string
			 * @param[in] resultLocation string that specifies where the result can be obtained
			 */
			void setResultLocation( const char* resultLocation );

			/**
			 * @fn std::string getResultLocation( void ) const
			 * @brief get the result location string
			 * @return string that specifies where the result can be obtained
			 */
			std::string getResultLocation( void ) const;

			/**
			 * @fn kome::core::Properties& getPeakDetectionParameters( void )
			 * @brief get the peak detection parameters object
			 * @return peak detection parameters
			 */
			kome::core::Properties& getPeakDetectionParameters( void );

			/**
			 * @fn void setDate( struct tm* date )
			 * @brief set the search date/time information
			 * @param[in] date the date/time
			 */
			void setDate( struct tm* date );

			/**
			 * @fn void setDate( const char* date )
			 * @brief sets the date
			 * @param[in] date date
			 */
			void setDate( const char* date );

			/**
			 * @fn std::string getDate( void ) const
			 * @brief get the search date/time information
			 * @return date/time
			 */
			std::string getDate( void ) const;

			/**
			 * @fn void clearSubstances( void )
			 * @brief clear the Substance array
			 */
			void clearSubstances( void );

			/**
			 * @fn void addSubstance( Substance* substance )
			 * @brief add substance information
			 * @param[in] substance object to add.
			 */
			void addSubstance( Substance* substance );

			/**
			 * @fn void removeSubstance( Substance* substance )
			 * @brief removes substance
			 * @param[in] substance substance object to remove
			 */
			void removeSubstance( Substance* substance );

			/**
			 * @fn Substance* searchSubstance( const char* accession, const char* name, Substance* parent, SubstanceType type )
			 * @brief search the substance
			 * @param[in] accession accession
			 * @param[in] name substance name
			 * @param[in] parent parent substance
			 * @param[in] type substance type
			 * @return If same substance is found, this function returns the same substance, otherwise returns NULL.
			 */
			Substance* searchSubstance( const char* accession, const char* name, Substance* parent, SubstanceType type );

			/**
			 * @fn int getNumberOfSubstances( void ) const
			 * @brief get the nunber of substances
			 * @return substance count
			 */
			int getNumberOfSubstances( void ) const;

			/**
			 * @fn Substance* getSubstance( const int index ) const
			 * @brief get the specified substance information
			 * @param[in] index substance index
			 * @return substance
			 */
			Substance* getSubstance( const int index ) const;

			/**
			 * @fn void clearHits( void )
			 * @brief clear the Hit array
			 */
			void clearHits( void );

			/**
			 * @fn Hit* addHit( Substance* pSubstance = NULL )
			 * @brief add hit information
			 * @param[in] pSubstance pointer to a Substance object
			 * @return pointer to a Hit object
			 */
			Hit* addHit( Substance* pSubstance = NULL );

			/**
			 * @fn int getNumberOfHits( void ) const
			 * @brief get the nunber of hits
			 * @return hit count
			 */
			int getNumberOfHits( void ) const;

			/**
			 * @fn Hit* getHit( const int index ) const
			 * @brief get the specified hit information
			 * @param[in] index hit index
			 * @return hit
			 */
			Hit* getHit( const int index ) const;

			/**
			 * @fn void exportResult( boost::function< int ( void*, int ) > writeFun )
			 * @brief export the identification result
			 * @param[out] writeFun function to save state
			 */
			void exportResult( boost::function< int ( void*, int ) > writeFun );

			/**
			 * @fn void importResult( boost::function< int ( void*, int ) > readFun )
			 * @brief import an identification result
			 * @param[in] readFun function to read saved state
			 */
			void importResult( boost::function< int ( void*, int ) > readFun );

		protected:
			/**
			 * @fn void clearSpectra()
			 * @brief clears spectra
			 */
			void clearSpectra();
			
		public:
			/**
			 * @fn void addSpectrum( SpecInfo* spec )
			 * @brief adds spectrum information
			 * @param[in] spec spectrum information
			 */
			void addSpectrum( SpecInfo* spec );

		public:
			/**
			 * @fn void setPeakListFile( const char* path )
			 * @brief sets the peak list file
			 * @param[in] path peak list file path
			 */
			void setPeakListFile( const char* path );

			/**
			 * @fn const char* getPeakListFile()
			 * @brief gets the peak list file path
			 * @return peak list file path
			 */
			const char* getPeakListFile();

		public:
			/**
			 * @fn void setEnzyme( Enzyme* enzyme )
			 * @brief sets enzyme
			 * @param[in] enzyme
			 */
			void setEnzyme( Enzyme* enzyme );

			/**
			 * @fn Enzyme* getEnzyme()
			 * @brief gets enzyme
			 * @return enzyme
			 */
			Enzyme* getEnzyme();

			/**
			 * @fn void setBaseName( const char* baseName )
			 * @brief sets base name
			 * @param[in] baseName base name
			 */
			void setBaseName( const char* baseName );

			/**
			 * @fn const char* getBaseName()
			 * @brief gets base name
			 * @return base name
			 */
			const char* getBaseName();

			/**
			 * @fn void setPrecursorMassType( MassType type )
			 * @brief sets precursor mass type
			 * @param[in] type precursor mass type
			 */
			void setPrecursorMassType( MassType type );

			/**
			 * @fn MassType getPrecursorMassType()
			 * @brief gets precursor mass type
			 * @return precursor mass type
			 */
			MassType getPrecursorMassType();

			/**
			 * @fn void setFragmentMassType( MassType type )
			 * @brief sets fragment mass type
			 * @param[in] type fragment mass type
			 */
			void setFragmentMassType( MassType type );

			/**
			 * @fn MassType getFragmentMassType()
			 * @brief gets fragment mass type
			 * @return fragment mass type
			 */
			MassType getFragmentMassType();

			/**
			 * @fn void clearFixedModifications()
			 * @brief clears fixed modifications
			 */
			void clearFixedModifications();

			/**
			 * @fn unsigned int getNumberOfFixedModifications()
			 * @brief gets the number of fixed modifications
			 * @return the number of fixed modifications
			 */
			unsigned int getNumberOfFixedModifications();

			/**
			 * @fn Modification* getFixedModification( const unsigned int idx )
			 * @brief gets fixed modification
			 * @param[in] idx fixed modification index
			 * @return fixed modification
			 */
			Modification* getFixedModification( const unsigned int idx );

			/**
			 * @fn void addFixedModification( Modification* modification )
			 * @brief adds fixed modification
			 * @param[in] modification fixed modification
			 */
			void addFixedModification( Modification* modification );

			/**
			 * @fn void clearVariableModifications()
			 * @brief clears variable modifications
			 */
			void clearVariableModifications();

			/**
			 * @fn unsigned int getNumberOfVariableModifications()
			 * @brief gets the number of variable modifications
			 * @return variable modifications
			 */
			unsigned int getNumberOfVariableModifications();

			/**
			 * @fn Modification* getVariableModification( const unsigned int idx )
			 * @brief gets variable modification
			 * @param[in] idx variable modification index
			 * @return variable modification
			 */
			Modification* getVariableModification( const unsigned int idx );

			/**
			 * @fn void addVariableModification( Modification* modification )
			 * @brief adds variable modification
			 * @param[in] modification variable modification
			 */
			void addVariableModification( Modification* modification );
		};
	}
}
