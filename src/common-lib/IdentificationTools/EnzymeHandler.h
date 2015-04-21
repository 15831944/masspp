/**
 * @file EnzymeHandler.h
 * @brief interfaces of EnzymeHandler class
 *
 * @author S.Tanaka
 * @date 2012.03.01
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IDENT_ENZYME_HANDLER_H__
#define __KOME_IDENT_ENZYME_HANDLER_H__


#include "Enzyme.h"


namespace kome {
	namespace ident {


		/**
		 * @class EnzymeHandler
		 * @brief XML handler class to read plug-in file
		 */
		class EnzymeHandler : public kome::xml::XmlHandler {
		public:
			/**
			 * @fn EnzymeHandler( std::vector< Enzyme* >& enzymes )
			 * @brief constructor
			 * @param[in] enzymes the array of enzymes
			 */
			EnzymeHandler( std::vector< Enzyme* >& enzymes );

			/**
			 * @fn virtual ~EnzymeHandler()
			 * @brief destructor
			 */
			virtual ~EnzymeHandler();

		protected:
			/** enzymes */
			std::vector< Enzyme* >& m_enzymes;

		protected:
			/** enzyme name */
			std::string m_name;

			/** term type */
			Enzyme::TermType m_term;

			/** cleave */
			std::string m_cleave;

			/** non-cleave */
			std::string m_noncleave;

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
			 * @param[in] name tag name
			 * @param[in] attrs attributes
			 */
			virtual void onStartElement( const char* name, kome::core::Properties& attrs );

			/**
			 * @fn virtual void onEndElement( const char* name, const char* text )
			 * @brief This method is called by end element method. (override method)
			 * @param[in] name tag name
			 * @param[in] text body text
			 */
			virtual void onEndElement( const char* name, const char* text );			
		};
	}
}

#endif	// __KOME_IDENT_ENZYME_HANDLER_H__
