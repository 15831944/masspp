/**
 * @file ProteinSubstance.h
 * @brief interfaces of ProteinSubstance class
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
		 * @class ProteinSubstance
		 * @brief class to store substance details
		 */
		class IDENT_TOOLS_CLASS ProteinSubstance : public Substance
		{
		public:
			/**
			 * @fn ProteinSubstance( SearchResult* pSrchResult, const char* accession, const char* name, const Substance* parent )
			 * @brief constructor
			 * @param[in] pSrchResult search result
			 * @param[in] accession accession
			 * @param[in] name substance name
			 * @param[in] parent parent substance
			 */
			ProteinSubstance( SearchResult* pSrchResult, const char* accession, const char* name, Substance* parent );

			/**
			 * @fn ~ProteinSubstance( void )
			 * @brief destructor
			 */
			~ProteinSubstance( void );

		public:
			/**
			 * @fn void setMass( const double mass )
			 * @brief sets mass
			 * @param[in] mass mass
			 */
			void setMass( const double mass );

			/**
			 * @fn double getMass()
			 * @brief gets mass
			 * @return mass
			 */
			double getMass();

		protected:
			/** item name of the protein mw */
			static const char* PROT_ITEM_MW;

		};
	}
}
