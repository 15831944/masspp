/**
 * @file ModificationHandler.h
 * @brief interfaces of ModificationHandler class
 *
 * @author S.Tanaka
 * @date 2012.03.01
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IDENT_MODIFICATION_HANDLER_H__
#define __KOME_IDENT_MODIFICATION_HANDLER_H__


namespace kome {
	namespace ident {

		class Modification;


		/**
		 * @class ModificationHandler
		 * @brief XML handler class to read plug-in file
		 */
		class ModificationHandler : public kome::xml::XmlHandler {
		public:
			/**
			 * @fn ModificationHandler()
			 * @brief constructor
			 */
			ModificationHandler( std::vector< Modification* >& mods );

			/**
			 * @fn virtual ~ModificationHandler()
			 * @brief destructor
			 */
			virtual ~ModificationHandler();

		protected:
			/** modifications */
			std::vector< Modification* >& m_mods;

			/** modification title */
			std::string m_title;

			/** residues */
			std::vector< std::string > m_residues;

			/** monomass */
			double m_monomass;

			/** hidden */
			bool m_hidden;

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

#endif	// __KOME_IDENT_MODIFICATION_HANDLER_H__
