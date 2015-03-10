/**
 * @file SpecElemSetter.h
 * @brief Wizard matrial(Label,ImagePath,..) message declaration
 *
 * @author M.Fukuda
 * @date 2013.12.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#pragma once

namespace elm {
	class SpectrumElementSetter {
	public:
		SpectrumElementSetter(){}
		~SpectrumElementSetter(){}
	private:
		typedef std::pair<std::string, kome::objects::Sample*> kSPair;
		std::map<std::string, kome::objects::Sample*> splMap_;
//		std::vector<kome::objects::Sample*> splArr_;

	public:
		const bool getDisPlayedSpectra(
			std::vector<kome::objects::Spectrum*>&);

		const bool getOpendAllSamples(
			std::vector<kome::objects::Sample*>&);

		const bool getOpendAllSamples(
			std::vector<kSPair>&);

		const bool getAverageSpecElemements(
			const std::string& src,
			std::string& errmsg,
			std::vector<kome::objects::Spectrum*>& ref);

		const bool getSamplesSpectra(
			const std::string& ids,
			std::vector<kome::objects::Spectrum*>&);

		const bool checkAverageSpectraStream(
			const std::string& src,
			std::string& errmsg);

	private:
		void initMe();

		const bool parseGetParameters(
			const std::string& src,
			kome::objects::Sample*&,
			double& rtlow,
			double& rtHigh,
			int& mstype,
			std::string& errmsg);

		const bool onCheckAverageSpectraStream(
			const std::string& src,
			std::string& errmsg,
			std::vector<kome::objects::Spectrum*>&);

		const bool onCheckFindRTRange(
			kome::objects::DataSet& ds,
			double& rtlow,
			double& rthigh,
			std::string& errmsg);

		const bool onCheckFindRange(
			const double fRT,
			const double eRT,
			double& rtlow,
			double& rthigh,
			std::string& errmsg);

		const bool onGetMatchSpectra(
			kome::objects::DataSet& ds,
			const double rtlow,
			const double rthigh,
			const int mstype,
			std::vector<kome::objects::Spectrum*>&,
			std::string& errmsg);

		const bool onGetSampleSpectra(
			kome::objects::Sample*,
			std::vector<kome::objects::Spectrum*>&);

		kome::objects::DataSet onGetDataSet(
			kome::objects::Sample*);


	};
}
