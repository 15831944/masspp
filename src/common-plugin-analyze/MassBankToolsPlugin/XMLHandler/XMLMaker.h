/**
 * @file XMLMaker
 * @brief MassBankのXMLを生成する
 *
 * @author M.Fukuda
 * @date 2013.04.24
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#pragma once

namespace kome {
	namespace massbank {

		class XMLMaker {
		public:
			// constructor
			XMLMaker();

			// descructor
			~XMLMaker();
		private:
			// onAppendFixedを使う際に、一時的に使う。
			kome::objects::SettingParameterValues* settings_;

			// PrivateMethod
			std::string xml_;
			std::string method_;
			bool hasExFg_;       // ver 2.6.0 より searchSpectrumEx等 Ex系に対応か否か

		public:
			// 生成したXMLを取得する
			std::string getXML() const { return xml_;}

			// 生成したmethodを取得する
			std::string getMethod() const { return method_;}

			void setHasExFg(const bool fg) {hasExFg_ = fg;}

			// Instrumentsを取得するXMLを作る
			void setGetInstrumentTypes();

			// spectrum searchを取得するXMLを作る
			void setSimilarSpectraXML(
				kome::core::XYData& peaks,
				kome::objects::SettingParameterValues* settings,
				const std::string& tgtinstruments);

			// Batch Searchを行うXMLを作る
			void setBatchSearchXML(
				std::vector<kome::objects::Spectrum*> spectra,
				kome::objects::SettingParameterValues* settings,
				const std::string& tgtinstruments);

			void setBatchSearchXML(
				const std::string& specQuerys,
				kome::objects::SettingParameterValues* settings);


			// search peaks / diff.. を行う XMLを作る
			void setSearchPeaksXML(
				kome::objects::SettingParameterValues* settings,
				const std::string& tgtinstruments);

			void setDiffSearchPeaksXML(
				kome::objects::SettingParameterValues* settings,
				const std::string& tgtinstruments);


			void setGetJobStatus(const std::string& jobId);

			void setGetJobResult(const char* jobId);

			void setGetRecordInfo(const char* jobId);

			void setGetPeak(const char* jobId);

			const std::string getQueryStringsTag(
				const char* scanname,
				kome::objects::Peaks* peaks);

			const std::string onGetInstrumentTypesTag();

			const std::string onGetInstrumentTypesTag(
				const std::vector<std::string>& instrumenttypes);

		private:
			// Job IDのタグを作成する
			const std::string onSetJB_ID(const char* jobId);

			const std::string onAppendFixedDouble(
				const std::string& tgt,
				const double value
				);

			// ルールに則った(sType) XMLを返す
			const std::string onAppendFixedType(
				const std::string& tgt,
				const int sType);

			const std::string onAppendFixedTypes(
				const int sType,
				const std::string& tagPtr,
				const std::string& tgt,
				const bool noParamFg = false
				);


			const std::string onSetMZSArray(const std::string& listKey);

			// MassBank XMLを作成する
			const std::string onSetMSBKFormat();

			// MassBank XMLを作成する（content..を間に挟み込む)
			const std::string onSetMSBKFormat(const std::string& contentReq);

			// Job..系のXMLを作成する(methodが違う）
			void onSetJobStatus(
				const std::string& method,
				const char* jobId);

			// Testの為のXMLコード作成
			const std::string onMakeTestXML_searchSpectrum(const bool exFg);
			const std::string onMakeTestXML_searchPeak(const bool exFg);
			const std::string onMakeTestXML_searchPeakDiff(const bool exFg);

		};
	}
}
