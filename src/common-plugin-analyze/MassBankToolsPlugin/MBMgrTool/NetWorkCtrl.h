/**
 * @file NetWorkCtrl
 * @brief NetWorkCtrl
 *
 * @author M.Fukuda
 * @date 2013.04.24
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#pragma once

namespace kome {
	namespace massbank {

		class NetWorkCtrl {
		public:
			NetWorkCtrl();
			~NetWorkCtrl(){}

		private:
			// WSDL
			kome::net::Wsdl m_wsdl; // wdsl
			std::string url_;       // massbank URL
			std::string proxy_;     // proxy
			bool useNetFg_;         // networkを使うか否か
			bool changedFg_;        // 変更点があったか否か
			bool showNG_;           // 接続不可の警告を出すか否か
			bool hasExFg_;          // Ex系の命令に接続先が対応しているか否か

		public:
			// Getter
			bool hasEx(void) const {return hasExFg_;}

			// 接続情報を確認する
			const bool checkMyConnect(bool showNGFg = false);

			// method, xmlStrの内容で MassBankAPIを実行する。
			// 結果は resに帰ってくる
			const bool execAPI(
				const std::string& method,
				std::string& xmlStr,
				std::vector<char>& refRes,
				int& errCode
				);

		private:
			// ネットワーク情報(parameter.ini)の変更を監視する。
			// 現在は Proxyとmassbank urlのみ
			const bool onCheckNewInfoChanged();

			// Wdslの結果を覚えておく
			const bool onWsdlConnectControler();

			// Wdslを url_ で接続してみる
			bool onLoadWsdl();

			// SOAPの対応関係を調べる
			const bool onCheckSOAP(
				const std::string& method
				);
		};
	}
}
