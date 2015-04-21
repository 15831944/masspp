/**
 * @file Substance.h
 * @brief interfaces of Substance class
 *
 * @author H.Parry
 * @date 2012.01.25
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */

#pragma once

namespace kome {
	namespace ident {

		class SearchResult;

		/**
		 * @typedef SubstanceType
		 * @brief substance type
		 */
		typedef enum {
			TYPE_UNKNOWN = 0,
			TYPE_PROTEIN = 1,
			TYPE_PEPTIDE = 2,
			TYPE_METABOLITE = 3
		} SubstanceType;

		/**
		 * @class Substance
		 * @brief class to store substance details
		 */
		class IDENT_TOOLS_CLASS Substance
		{
		public:
			/**
			 * @fn Substance( SearchResult* pSrchResult, const char* accession, const char* name, const SubstanceType type, Substance* parent )
			 * @brief constructor
			 * @param[in] pSrchResult search result
			 * @param[in] accession accession
			 * @param[in] name substance name
			 * @param[in] type substance type
			 * @param[in] parent parent substance
			 */
			Substance( SearchResult* pSrchResult, const char* accession, const char* name, const SubstanceType type, Substance* parent );

			/**
			 * @fn virtual ~Substance( void )
			 * @brief destructor
			 */
			virtual ~Substance( void );

		protected:
			/** search result containing the substance */
			SearchResult* m_pSrchResult;

			/** accession */
			std::string	m_accession;

			/** substance name */
			std::string	m_name;

			/** substance type */
			SubstanceType m_type;

			/** parent substance */
			Substance* m_parent;

			/** properties */
			kome::core::Properties m_properties;

		public:
			/**
			 * @fn std::string getAccession( void ) const
			 * @brief get the accession name
			 * @return accession
			 */
			std::string getAccession( void ) const;

			/**
			 * @fn std::string getName( void ) const
			 * @brief get the substance name
			 * @return name
			 */
			std::string getName( void ) const;

			/**
			 * @fn SubstanceType getType( void ) const
			 * @brief get the substance type
			 * @return type
			 */
			SubstanceType getType( void ) const;

			/**
			 * @fn Substance* getParentSubstance( void ) const
			 * @brief get the parent substance (e.g. peptide -> protein)
			 * @return pointer to Substance object
			 */
			Substance* getParentSubstance( void ) const;

			/**
			 * @fn void setParentSubstance( Substance* parent )
			 * @brief set the parent substance (e.g. peptide -> protein)
			 * @param[in] parent pointer to a Substance object
			 */
			void setParentSubstance( Substance* parent );

			/**
			 * @fn int getNumberOfChildSubstances( void ) const
			 * @brief get the number of child substances (e.g. protein -> peptides)
			 * @return child substance count
			 */
			int getNumberOfChildSubstances( void ) const;

			/**
			 * @fn getChildSubstances( std::vector<Substance*>& substanceList ) const
			 * @brief get the child substances (e.g. protein -> peptide)
			 * @param[out] substanceList array of child substance pointers
			 */
			void getChildSubstances( std::vector<Substance*>& substanceList ) const;

			/**
			 * @fn kome::core::Properties& getProperties( void )
			 * @brief get the substance properties object
			 * @return properties
			 */
			kome::core::Properties& getProperties( void );


	
		};
	}
}
