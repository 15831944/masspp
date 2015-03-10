/**
 * @file TppProtXMLHandler.h
 * @brief interfaces of TppProtXMLHandler class
 *
 * @author N.Tashiro
 * @date 2013.03.15
 * 
 * Copyright(C) 2013 Shimadzu. All rights reserved.
 */


#ifndef __KOME_TPP_PROTXMLHANDLER_H__
#define __KOME_TPP_PROTXMLHANDLER_H__

#include "TppXMLHandler.h"

namespace kome {
	namespace Tpp{
		class TppProtXMLHandler : public kome::Tpp::TppXMLHandler {

		public:

			TppProtXMLHandler(T_prot *p);

			~TppProtXMLHandler();

		protected:

			virtual void onStartDocument();

			virtual void onStartElement(const char* name, kome::core::Properties& attrs);

			virtual void onEndDocument();

			virtual void onEndElement(const char* name, const char* text);

		private:

			T_prot		*m_prot;
		};
	}
}

#endif // __KOME_TPP_PROTXMLHANDLER_H__
