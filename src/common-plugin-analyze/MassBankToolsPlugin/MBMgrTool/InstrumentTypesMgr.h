/**
 * @file InstrumentTypesMgr.h
 * @brief MassBank Instrument Type Manager
 *
 * @author M.Fukuda
 * @date 2011.08.30
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#pragma once

#include <vector>
#include <string>
#include "boost\function.hpp"

namespace kome {
namespace massbank {
class InstrumentMgr {
	
	// Instrument Set Class
	// For STD & BOOST Method Search Class
public:
	class InstrumentSet {
	public:
		InstrumentSet(const std::string& _name, bool _isShown = true) 
			: m_name(_name)
			, isShown(_isShown){}
		~InstrumentSet(){}
		bool operator==( const InstrumentSet& other ) const {
			return m_name == other.m_name;
		}
		std::string m_name;
		bool isShown;
	};

	//--------------------------------------------------------------------------------
//     INSTRUMENT TYPE
//--------------------------------------------------------------------------------

public:
	InstrumentMgr();
	~InstrumentMgr();

private:
	std::vector< InstrumentSet > m_instruments; // Instrument Types
	boost::function<bool(std::vector< std::string >&)> setF_;

public:
	template <typename SFunc> void set_f(SFunc f) { setF_ = f;}

	void clearInstrumentTypes();

	unsigned int getNumberOfInstrumentTypes();

	const char* getInstrumentType( const unsigned int idx );

	const bool isShown( const unsigned int idx );

	void setInstrumentTypeShownFlagsWithIDArray(const std::vector<long long>& idArray );

	void getCheckedInstrumentTypes(std::vector<std::string>&);

	void getAllInstrumentTypes(std::vector<std::string>&);

	bool checkHavingInstrumentTypes(bool& netInfo);
	
	const std::string getAll_ConvertMassBankSorceStr();

	void saveCheckedStatus();

	int numberOfCheckedRow() const;

private:
	bool updateInstrumentTypes();

	bool onLoadInstrumentTypesFromAny(std::vector<std::string>& instArr, bool& readini);

	std::string joinWithPipe(const std::vector<std::string>&) const;

	// Load From Param.ini
	void onLoadCheckedInstLists(std::vector<std::string>& ckArr);

	// List“à‚É‚ ‚é‚·‚×‚Ä‚ÌInsturumentType‚ð•Û‘¶‚·‚é
	void onSaveALLInstrumentTypes();

	// kome.ini ‚É MassBank + Key ‚Æ‚µ‚Ä xStr‚ð•Û‘¶
	void onSetIniParam(const char* key, const std::string& xstr);

	// kome.ini ‚©‚ç MassBank + Key ‚ð Žæ“¾
	const std::string onGetIniParam(const char* key);

	// Parametar.ini ‚É MassBank + Key ‚Æ‚µ‚Ä xStr‚ð•Û‘¶
	void onSetParam(const char* key, const std::string& xstr);

	// Parameter.ini ‚©‚ç MassBank + Key ‚ð Žæ“¾
	const std::string onGetParam(const char* key);
};

}//end of namespace
}
