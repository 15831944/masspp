/**
 * @file BatchXMLHandler.h
 * @brief interfaces of BatchXmlHandler class
 *
 * @author okada_h
 * @date 2012.02.14
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "BatchServicePluginManager.h"

namespace kome {
	namespace batchservice {

		class XmlHandler;

		class BatchXmlHandler : public kome::xml::XmlHandler {

		public:
			/**
			 * @fn BatchXmlHandler()
			 * @brief constructor
			 */
			BatchXmlHandler();

			/**
			 * @fn virtual ~BatchXmlHandler()
			 * @brief destructor
			 */
			virtual ~BatchXmlHandler();

		protected:
			struct Country{
				std::string name;
				std::string date;
				std::string functions;
				int code;
			};    // 構造体定義

			// データ格納配列
			std::vector< Country > m_countries;

			// 作業用構造体
			Country m_country; 

			/** 作業用構造体 */
			Action				m_tempAction;
			std::vector<Action>	m_vectActions;

			/** */
			std::string strMethodName;

		protected:

			/**
			 * @fn void Initialize()
			 * @brief This method is initialization.
			 */
			void Initialize();

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
			 * @fn void onStartElement(
						const char* name,
						kome::core::Properties& attrs
					)
			 * @brief 開始タグ読込時の処理
			 * @param name
			 * @param attrs
			 */
			void onStartElement(
				const char* name,
				kome::core::Properties& attrs
			);

			/**
			 * @fn void onEndElement(
						const char* name,
						const char* text
					)
			 * @brief 終了タグ読込時の処理
			 * @param name
			 * @param text
			 */
			void onEndElement(
				const char* name,
				const char* text
			);

		public:
			/**
			 * @fn std::vector<Action>	getVectActions()
			 * @brief get vect actions
			 * @return
			 */
			std::vector<Action>	getVectActions(){
				return m_vectActions;
			};
		};

	}
}

