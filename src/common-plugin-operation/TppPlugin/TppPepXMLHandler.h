/**
 * @file TppXMLHandler.h
 * @brief interfaces of TppXMLHandler class
 *
 * @author N.Tashiro
 * @date 2013.03.15
 * 
 * Copyright(C) 2013 Shimadzu. All rights reserved.
 */


#ifndef __KOME_TPP_PEPXMLHANDLER_H__
#define __KOME_TPP_PEPXMLHANDLER_H__

#include "TppXMLHandler.h"

namespace kome {
	namespace Tpp{
		class TppPepXMLHandler : public kome::Tpp::TppXMLHandler {

		public:
			/**
			 * @brief constructor
			 */
			TppPepXMLHandler(T_pep *p);

			/**
			 * @brief destructor
			 */
			~TppPepXMLHandler();

		protected:

			virtual void onStartDocument();

			virtual void onStartElement(const char* name, kome::core::Properties& attrs);

			virtual void onEndDocument();

			virtual void onEndElement(const char* name, const char* text);

		private:
			// parse result
			T_pep		*m_pep;
		};
	}
}

#endif // __KOME_TPP_PEPXMLHANDLER_H__
