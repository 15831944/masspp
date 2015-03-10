/**
 * @file DBAccessClass
 * @brief Data Processing List Box Informations
 *
 * @author M.Fukuda
 * @date 2013.02.26
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 *
 * @ note -=====================================
 *     Link DBの値を統括する。
 *     (has-a) DBACCSelectDialog : 選択のコンボボックス
 */

#pragma once

#include <string>
#include <vector>

namespace kome {
	namespace massbank {
		namespace wizard {

			//-----------------------------------------------
			//  DB Access Class
			//-----------------------------------------------
			class DBAccClass {
				// DB Linksの情報
				struct DBLinks {
					std::string dbName;  //DB 名(CASとか）
					std::string URLHead; //URLの先頭部 (ACCSESSIONの前)
					std::string URLTail; //URLの後半部 (ACCSESSIONの後)
					std::string URL;     //デモURL

					// constructor
					DBLinks(
						const std::string& d,
						const std::string& h,
						const std::string& t
						) : dbName(d), URLHead(h), URLTail(t){}
					
					void setURL(const std::string& acc) {
						URL = URLHead + acc + URLTail;
					}
				};
			public:
				// constructor
				DBAccClass();
				// destructor
				~DBAccClass(){}

			private:
				// DB Linkの構造体群
				std::vector<DBLinks> DBLArr_;
				std::vector<kome::ident::ExternalDB*> extDbs_;
				void init();

				std::string searchedKeyword_;

			public:
				// 格納してあるDB Linkの情報数を返す
				unsigned int getMyNum()const{return DBLArr_.size();}

				// 対象IDのDB名を返す（String)
				std::string getNames(const unsigned int idx) const;

				// 対象IDのDB名を返す（const char*)
				const char* getName(const unsigned int idx) const;

				// demo URLを返す(String)
				std::string getsURL(const unsigned int idx) const;

				// demo URLを返す
				const char* getURL(const unsigned int idx) const;

				// 本体パネルから DB Listの値を受け取る。
				const std::string setListArrStrs(const std::string& listStr);

				const bool callSearchKeyWordtoExDB(
					std::string& searchedKeyword,
					std::vector<std::string>& results);

				const bool getKeyWordSearchResults(
					std::string& rstName,
					std::string& linksVal,
					std::string& accURL
					);

			private:
				const std::string getURL(
					const std::string& exdb,
					const std::string& accession);

			};

		}
	}
}
