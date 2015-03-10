/**
 * @file SampleCheckList.cpp
 * @brief sample check list(use Wizard's sample select)
 *
 * @author M.Fukuda
 * @date 2013.01.18
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.

 */
#pragma once

// -- Forward declaration --
class SampleCheckList;

// make GridListPanel
kome::window::CheckGridListPanel* 
	createSampleCheckList(
		wxWindow* parent,
		const std::vector<kome::objects::Sample*>&,
		SampleCheckList*& clist);

// Class
class SampleCheckList : public kome::window::CheckGridListCtrl {
public:
	// constructor
	SampleCheckList(
		wxWindow* parent,
		const std::vector<kome::objects::Sample*>&);

	// destroctor
	virtual ~SampleCheckList();

private:
	// my Parameters
	std::vector<kome::objects::Sample*> sampleArr_;

protected:
	// Inner Create(called By createMainSizer)
	virtual void onCreate();

	// get for ListCtrl Values (ècIdx : dataidx, â°:column) (virtual)
	virtual std::string getString(const long long dataidx, const int column) const;

public:
	// Set SpecList Data Form type No.
	// get spectra
	const bool getSelectedSample(std::vector<kome::objects::Sample*>&);
	void setupMe();
private:
	DECLARE_EVENT_TABLE()
};
