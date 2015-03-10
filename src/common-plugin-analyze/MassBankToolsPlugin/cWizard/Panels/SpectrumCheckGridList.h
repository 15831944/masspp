/**
 * @file SpectrumCheckGridList.h
 * @brief MassBank's create records
 *
 * @author M.Fukuda
 * @date 2013.01.18
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#pragma once

namespace pane {

	// -- Forward declaration --
class SpectraCheckList;

// Panel & Interface
class SpecCheckListPanel : public kome::window::HelpGetterPanel {
public:
	SpecCheckListPanel(
		wxWindow* parent,
		const std::string& dsc,
		const bool isDisplayMode);

	virtual ~SpecCheckListPanel();

private:
	// inner values
	SpectraCheckList* scList_;
	kome::window::CheckGridListPanel* panel_;
	const bool isDisplayMode_;
	const std::string& mydsc_;

protected:
	// overLs
	virtual wxSizer* createMainSizer();
//				virtual std::string getDescription(wxWindow* wnd);				
public:
	void setSelectedElement(const bool finalFg = false);

	// getter
	const bool getSelectedSpectra(
		std::vector<kome::objects::Spectrum*>& specArr,
		const bool finalFg
		);

	void setBaseSpectra(
		const std::vector<kome::objects::Spectrum*>& specArr);

	void setShowSpectra(
		const std::vector<kome::objects::Spectrum*>& src
		);

	void clearMyList();

private:
	DECLARE_EVENT_TABLE();
};

//// make GridListPanel
//kome::window::CheckGridListPanel* 
//	createSpectrumCheckGridList(
//		wxWindow* parent,
//		const bool isDisplayMode,
//		SpectraCheckList*& clist);

// Class
class SpectraCheckList : public kome::window::CheckGridListCtrl {
public:
	// constructor
	SpectraCheckList(
		wxWindow* parent,
		const unsigned int width,
		const unsigned int height,
		const bool isDisplayMode
		);

	// destroctor
	virtual ~SpectraCheckList();

private:
//	std::vector<kome::objects::Spectrum*> specArr_;

	const bool isDisplayMode_;

	struct specFlag{
		kome::objects::Spectrum* spec;
		bool checked;
		specFlag(
			kome::objects::Spectrum* sp
			) : spec(sp)
			,checked(false){}
	};

	std::vector<specFlag> allspecArr_;
	std::vector<int> showIdx_;
//	std::vector<specFlag*> specArr_;

//	std::map<kome::objects::Spectrum*, bool> specmap_;

protected:
	// Inner Create(called By createMainSizer)
	virtual void onCreate();

	// get for ListCtrl Values (ècIdx : dataidx, â°:column) (virtual)
	virtual std::string getString(const long long dataidx, const int column) const;
	virtual double      getDouble(const long long dataidx, const int column) const;
	virtual int         getInt   (const long long dataidx, const int column) const;
	kome::objects::Spectrum* onGetSpec(const long long dataidx) const;
public:
	// Set SpecList Data Form type No.
	// get spectra
	void setSelectedElement(const bool finalFg);

	const bool getSelectedSpectra(
		std::vector<kome::objects::Spectrum*>&, const bool finalFg);

	void addBaseSpectra(
		const std::vector<kome::objects::Spectrum*>& specArr);

	void setShowSpectra(
		const std::vector<kome::objects::Spectrum*>& src);

	void setShowAll();

	void updateMySpectraList();

	void clearMyList();

	void clearMyAllSpecArr(){allspecArr_.clear();}
private:

	void onSetSelectedElement(const bool finalFg);

	DECLARE_EVENT_TABLE()
};
};
