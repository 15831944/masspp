/**
 * @file SpectrumCheckListPanel.h
 * @brief MassBank's create records
 *
 * @author M.Fukuda
 * @date 2013.01.18
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#pragma once

namespace kome {
namespace massbank {
namespace wizard {

// Class
class SpectraGridList : public kome::window::GridListCtrl {
private:

	// inner my datas struct
	struct listDatas {
		kome::objects::Spectrum* spec;
		std::string specname; 
		std::string filename;
		double rt;
		double precursor;
		std::string stage;
		int scannum;
	};

public:
	// constructor
	SpectraGridList(wxWindow* parent,
		wxWindowID thisId,
		const unsigned int width,
		const unsigned int height);
	// destroctor
	virtual ~SpectraGridList();

protected:

	// Inner Create(called By createMainSizer)
	virtual void onCreate();

	// get for ListCtrl Values (ècIdx : dataidx, â°:column) (virtual)
	virtual std::string getString(const long long dataidx, const int column) const;
	virtual double      getDouble(const long long dataidx, const int column) const;
	virtual int         getInt   (const long long dataidx, const int column) const;

public:
	// Set
	void setSelectedSpectra(
		std::vector<kome::objects::Spectrum*>&,
		const bool reset = true
	);

	// get spectra
	kome::objects::Spectrum* getSelectSpectrum(const int& id);
private:
	void onSetSpecData(
		kome::objects::Spectrum* spec, 
		const std::string& tgtfilename, 
		const unsigned int listIdx);

	// inner List
	// data Array
	std::vector<listDatas> listArr_;
	std::string convMsType(const int) const;

	DECLARE_EVENT_TABLE()
};

}}} // end of namespace
