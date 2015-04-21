/**
 * @file PeptideSubstance.h
 * @brief interfaces of PeptideSubstance class
 *
 * @author S.Tanaka
 * @date 2012.08.01
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#pragma once


#include "Substance.h"


namespace kome {
	namespace ident {

		class SearchResult;

		/**
		 * @class PeptideSubstance
		 * @brief class to store substance details
		 */
		class IDENT_TOOLS_CLASS PeptideSubstance : public Substance
		{
		public:
			/**
			 * @fn PeptideSubstance( SearchResult* pSrchResult, const char* accession, const char* name, Substance* parent )
			 * @brief constructor
			 * @param[in] pSrchResult search result
			 * @param[in] accession accession
			 * @param[in] name substance name
			 * @param[in] parent parent substance
			 */
			PeptideSubstance( SearchResult* pSrchResult, const char* accession, const char* name, Substance* parent );

			/**
			 * @fn ~PeptideSubstance( void )
			 * @brief destructor
			 */
			~PeptideSubstance( void );

		private:
			/** item name of the peptide sequence */
			static const char* PEP_ITEM_SEQUENCE;

			/** item name of the residue before */
			static const char* PEP_ITEM_PREV_AA;

			/** item name of the residue after */
			static const char* PEP_ITEM_NEXT_AA;

			/** item name of the peptide start position */
			static const char* PEP_ITEM_START;

			/** item name of the peptide end position */
			static const char* PEP_ITEM_END;

		public:			
			/**
			 * @struct Modification
			 * @brief modification
			 */
			struct Modification {
				std::string mod;
				int pos;
			};

		protected:
			/** sequence */
			std::string m_sequence;

			/** before */
			std::string m_before;

			/** after */
			std::string m_after;

			/** modifications */
			std::vector< Modification > m_mods;

		public:
			/**
			 * @fn void setSequence( const char* sequence )
			 * @brief set the sequence
			 * @param[in] sequence sequence
			 */
			void setSequence( const char* sequence );

			/**
			 * @fn std::string getSequence( void )
			 * @brief get the sequence
			 * @return the sequence
			 */
			std::string getSequence( void );

			/**
			 * @fn void setResidueBefore( const char* residueBefore )
			 * @brief set the residue before
			 * @param[in] residueBefore residue before
			 */
			void setResidueBefore( const char* residueBefore );

			/**
			 * @fn std::string getResidueBefore( void )
			 * @brief get the residue before
			 * @return the residue before
			 */
			std::string getResidueBefore( void );

			/**
			 * @fn void setResidueAfter( const char* residueAfter )
			 * @brief set the residue after
			 * @param[in] residueAfter residue after
			 */
			void setResidueAfter( const char* residueAfter );

			/**
			 * @fn std::string getResidueAfter( void )
			 * @brief get the residue after
			 * @return residue after
			 */
			std::string getResidueAfter( void );

			/**
			 * @fn void setStart( const int start )
			 * @brief set the start index
			 * @param[in] start start index
			 */
			void setStart( const int start );

			/**
			 * @fn int getStart( void )
			 * @brief get the start index
			 * @return start index
			 */
			int getStart( void );

			/**
			 * @fn void setEnd( const int end )
			 * @brief set the end index
			 * @param[in] end end index
			 */
			void setEnd( const int end );

			/**
			 * @fn int getEnd( void )
			 * @brief get the end index
			 * @return end index
			 */
			int getEnd( void );

			/**
			 * @fn void addModification( const char* modification, const int pos )
			 * @brief adds modification
			 * @param[in] modification modification
			 * @param[in] pos position
			 */
			void addModification( const char* modification, const int pos );

		protected:
			/**
			 * @fn void setName()
			 * @brief sets the modification name
			 */
			void setName();

		public:
			/**
			 * @fn static std::string createPeptideName( const char* seq, const char* before, const char* after, std::vector< Modification >& mods )
			 * @brief creates peptide name
			 * @param[in] seq sequence
			 * @param[in] before residue before
			 * @param[in] after residue after
			 * @paran[in] mods modifications
			 * @return peptide name
			 */
			static std::string createPeptideName( const char* seq, const char* before, const char* after, std::vector< Modification >& mods );


		};
	}
}


