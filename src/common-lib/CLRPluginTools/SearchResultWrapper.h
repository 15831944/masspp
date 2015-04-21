/**
 * @file SearchResultWrapper.h
 * @brief interfaces of SearchResultWrapper class
 *
 * @author S.Tanaka
 * @date 2012.11.06
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation
 */


#ifndef __KOME_CLR_SEARCH_RESULT_WRAPPER_H__
#define __KOME_CLR_SEARCH_RESULT_WRAPPER_H__


#include "SubstanceWrapper.h"

#include "HitWrapper.h"


namespace kome {
	namespace clr { 

		ref class SearchEngineWrapper;
		ref class EnzymeWrapper;
		ref class ModificationWrapper;
		
		/**
		 * @class SearchResultWrapper
		 * @brief search result wrapper class to use on CLR environment
		 */
		public ref class SearchResultWrapper {
		public:
			/**
			 * @fn SearchResultWrapper( kome::ident::SearchResult& result )
			 * @briefructor
			 * @param[in] result search result object
			 */
			SearchResultWrapper( kome::ident::SearchResult& result );

			/**
			 * @fn SearchResultWrapper()
			 * @brief constructor
			 */
			SearchResultWrapper();

			/**
			 * @fn virtual ~SearchResultWrapper()
			 * @brief destructor
			 */
			virtual ~SearchResultWrapper();

		public:
			/** mass type */
			ENUM_CLASS MassType {
				UNDEFINED = 0,
				MONOISOTOPIC = 1,
				AVERAGE = 2,
				UNKNOWN = 3
			};

		protected:
			/** search result object */
			kome::ident::SearchResult& m_result;

			/** delete flag */
			bool m_deleteFlg;

		public:
			/**
			 * @fn kome::ident::SearchResult& getSearchResult()
			 * @brief gets search result object
			 * @return search result object
			 */
			kome::ident::SearchResult& getSearchResult();

		public:
			/**
			 * @fn void clear()
			 * @brief reset search result
			 */
			void clear();

			/**
			 * @fn cli::array< HitWrapper::SpecInfo^ >^ getSpectrumInfoList()
			 * @brief get the spectrum info list
			 * @return spectrum info list
			 */
			cli::array< HitWrapper::SpecInfo^ >^ getSpectrumInfoList();

			/**
			 * @fn PropertiesWrapper^ getSearchParameters()
			 * @brief get the search parameters object
			 * @return search parameters
			 */
			PropertiesWrapper^ getSearchParameters();

			/**
			 * @fn void setTitle( System::String^ title )
			 * @brief sets the title
			 * @param[in] title title
			 */
			void setTitle( System::String^ title );

			/**
			 * @fn System::String^ getTitle()
			 * @brief get the search title
			 * @return search title
			 */
			System::String^ getTitle();

			/**
			 * @fn void setComment( System::String^ comment )
			 * @brief sets the comment 
			 * @param[in] comment comment
			 */
			void setComment( System::String^ comment );

			/**
			 * @fn System::String^ getComment()
			 * @brief get the search comment
			 * @return search comment
			 */
			System::String^ getComment();

			/**
			 * @fn void setDb( System::String^ dbName )
			 * @brief set the name of the database used
			 * @param[in] dbName database name
			 */
			void setDb( System::String^ dbName );

			/**
			 * @fn System::String^ getDb()
			 * @brief get the name of the database used
			 * @return database name
			 */
			System::String^ getDb();

			/**
			 * @fn void setSearchEngine( SearchEngineWrapper^ srchEngine )
			 * @brief set the search engine used
			 * @param[in] srchEngine pointer to a SearchEngine object
			 */
			void setSearchEngine( SearchEngineWrapper^ srchEngine );

			/**
			 * @fn SearchEngineWrapper^ getSearchEngine()
			 * @brief get the search engine used
			 * @return search engine
			 */
			SearchEngineWrapper^ getSearchEngine();

			/**
			 * @fn void setSearchId( System::String^ id )
			 * @brief sets the search ID
			 * @param[in] id search ID
			 */
			void setSearchId( System::String^ id );

			/**
			 * @fn System::String^ getSearchId()
			 * @brief gets the search ID
			 * @return search ID
			 */
			System::String^ getSearchId();

			/**
			 * @fn void setResultLocation( System::String^ resultLocation )
			 * @brief set the result location string
			 * @param[in] resultLocation string that specifies where the result can be obtained
			 */
			void setResultLocation( System::String^ resultLocation );

			/**
			 * @fn System::String^ getResultLocation()
			 * @brief get the result location string
			 * @return string that specifies where the result can be obtained
			 */
			System::String^ getResultLocation();

			/**
			 * @fn PropertiesWrapper^ getPeakDetectionParameters()
			 * @brief get the peak detection parameters object
			 * @return peak detection parameters
			 */
			PropertiesWrapper^ getPeakDetectionParameters();

			/**
			 * @fnsetDate( System::DateTime^ date )
			 * @brief set the search date/time information
			 * @param[in] date the date/time
			 */
			void setDate( System::DateTime^ date );

			/**
			 * @fn void setDate( System::String^ date )
			 * @brief sets the date
			 * @param[in] date date
			 */
			void setDate( System::String^ date );

			/**
			 * @fn System::String^ getDate()
			 * @brief get the search date/time information
			 * @return date/time
			 */
			System::String^ getDate();

			/**
			 * @fn void clearSubstances()
			 * @brief clear the Substance array
			 */
			void clearSubstances();

			/**
			 * @fn void addSubstance( SubstanceWrapper^ substance )
			 * @brief add substance information
			 * @param[in] substance object to add.
			 */
			void addSubstance( SubstanceWrapper^ substance );

			/**
			 * @fnremoveSubstance( SubstanceWrapper^ substance )
			 * @brief removes substance
			 * @param[in] substance substance object to remove
			 */
			void removeSubstance( SubstanceWrapper^ substance );

			/**
			 * @fn SubstanceWrapper^ searchSubstance( System::String^ accession, System::String^ name, SubstanceWrapper^ parent, SubstanceWrapper::SubstanceType type )
			 * @brief search the substance
			 * @param[in] accession accession
			 * @param[in] name substance name
			 * @param[in] parent parent substance
			 * @param[in] type substance type
			 * @return If same substance is found, this function returns the same substance, otherwise returns NULL.
			 */
			SubstanceWrapper^ searchSubstance( System::String^ accession, System::String^ name, SubstanceWrapper^ parent, SubstanceWrapper::SubstanceType type );

			/**
			 * @fn int getNumberOfSubstances()
			 * @brief get the nunber of substances
			 * @return substance count
			 */
			int getNumberOfSubstances();

			/**
			 * @fn SubstanceWrapper^ getSubstance( int index )
			 * @brief get the specified substance information
			 * @param[in] index substance index
			 * @return substance
			 */
			SubstanceWrapper^ getSubstance( int index );

			/**
			 * @fn void clearHits()
			 * @brief clear the Hit array
			 */
			void clearHits();

			/**
			 * @fn HitWrapper^ addHit( SubstanceWrapper^ pSubstance )
			 * @brief add hit information
			 * @param[in] pSubstance pointer to a Substance object
			 * @return pointer to a Hit object
			 */
			HitWrapper^ addHit( SubstanceWrapper^ pSubstance );

			/**
			 * @fn int getNumberOfHits()
			 * @brief get the nunber of hits
			 * @return hit count
			 */
			int getNumberOfHits();

			/**
			 * @fn HitWrapper^ getHit( int index )
			 * @brief get the specified hit information
			 * @param[in] index hit index
			 * @return hit
			 */
			HitWrapper^ getHit( int index );

			/**
			 * @fn void addSpectrum( HitWrapper::SpecInfo^ spec )
			 * @brief adds spectrum information
			 * @param[in] spec spectrum information
			 */
			void addSpectrum( HitWrapper::SpecInfo^ spec );

			/**
			 * @fn void setPeakListFile( System::String^ path )
			 * @brief sets the peak list file
			 * @param[in] path peak list file path
			 */
			void setPeakListFile( System::String^ path );

			/**
			 * @fn System::String^ getPeakListFile()
			 * @brief gets the peak list file path
			 * @return peak list file path
			 */
			 System::String^ getPeakListFile();

		public:
			/**
			 * @fn void setEnzyme( EnzymeWrapper^ enzyme )
			 * @brief sets enzyme
			 * @param[in] enzyme
			 */
			void setEnzyme( EnzymeWrapper^ enzyme );

			/**
			 * @fn EnzymeWrapper^ getEnzyme()
			 * @brief gets enzyme
			 * @return enzyme
			 */
			EnzymeWrapper^ getEnzyme();

			/**
			 * @fn void setBaseName( System::String^ baseName )
			 * @brief sets base name
			 * @param[in] baseName base name
			 */
			void setBaseName( System::String^ baseName );

			/**
			 * @fn System::String^ getBaseName()
			 * @brief gets base name
			 * @return base name
			 */
			System::String^ getBaseName();

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
			 * @fn ModificationWrapper^ getFixedModification( unsigned int idx )
			 * @brief gets fixed modification
			 * @param[in] idx fixed modification index
			 * @return fixed modification
			 */
			ModificationWrapper^ getFixedModification( unsigned int idx );

			/**
			 * @fn void addFixedModification( ModificationWrapper^ modification )
			 * @brief adds fixed modification
			 * @param[in] modification fixed modification
			 */
			void addFixedModification( ModificationWrapper^ modification );

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
			 * @fn ModificationWrapper^ getVariableModification( unsigned int idx )
			 * @brief gets variable modification
			 * @param[in] idx variable modification index
			 * @return variable modification
			 */
			ModificationWrapper^ getVariableModification( unsigned int idx );

			/**
			 * @fn void addVariableModification( ModificationWrapper^ modification )
			 * @brief adds variable modification
			 * @param[in] modification variable modification
			 */
			void addVariableModification( ModificationWrapper^ modification );
		};
	}
}


#endif		// __KOME_CLR_SEARCH_RESULT_WRAPPER_H__

