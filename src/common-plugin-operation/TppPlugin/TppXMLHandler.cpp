/**
 * @file TppXMLHandler.cpp
 * @brief implements of TppXMLHandler class
 *
 * @author N.Tashiro
 * @date 2013.03.15
 * 
 * Copyright(C) 2013 Shimadzu. All rights reserved.
 */

#include "stdafx.h"
#include "TppXMLHandler.h"

using namespace kome::Tpp;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG

using namespace kome::xml;

// constructor
TppXMLHandler::TppXMLHandler(){

}

// destructor
TppXMLHandler::~TppXMLHandler(){

}

// initialize members
void TppXMLHandler::InitMember(){
	m_nCurrentTagLayer = 0;
}

// std::string Å® std::wstring
std::wstring TppXMLHandler::ToWideStr(const std::string &strMult){
	return str2wstr(FMT("%s",strMult.c_str()));
}

// split selected list with delim
std::list<std::string> TppXMLHandler::split(std::string str, std::string delim){
	std::list<std::string> result;
	int cutAt;
	while( (cutAt = str.find_first_of(delim)) != str.npos )
	{
		if(cutAt > 0)
		{
			result.push_back(str.substr(0, cutAt));
		}
		str = str.substr(cutAt + 1);
	}
	if(str.length() > 0)
	{
		result.push_back(str);
	}
	return result;
}

// translate string to double
double TppXMLHandler::string2double(std::string s){
	double dbRet = 0.0;

	dbRet = todouble(s.c_str());
	
	return dbRet;
}

// translate string to int
int TppXMLHandler::string2int(std::string s){
	int intRet = 0.0;
	
	intRet = toint(s.c_str());

	return intRet;
}

// convert csv to string vector
std::string TppXMLHandler::convertCSV (std::vector<std::string> string_ar, int fixed_len){
	std::string sTmp = "";
	int size = std::max ((int)string_ar.size(), fixed_len);

	for (int i = 0; i < size; i++)
	{
		if (i < string_ar.size())
			sTmp+= string_ar[i];

		if (i < fixed_len - 1)
			sTmp+= " ,";
	}
		
	return sTmp;
}

// convert csv to string vector
std::string TppXMLHandler::convertCSV (std::vector<std::string> db_ar){
	return convertCSV (db_ar, db_ar.size());
}

// convert csv to double vector
std::string TppXMLHandler::convertCSV (std::vector<double> db_ar){
	return convertCSV (db_ar, db_ar.size());
}

// convert csv to double vector
std::string TppXMLHandler::convertCSV (std::vector<double> db_ar, int fixed_len){
	std::string sTmp = "";
	int size = std::max ((int)db_ar.size(), fixed_len);

	for (int i = 0; i < size; i++)
	{
		if (i < db_ar.size())
		{
			sTmp += FMT("%f",db_ar[i]);
		}
		if (i < fixed_len - 1)
			sTmp+= ",";
	}
	return sTmp;
}

// convert csv to int vector
std::string TppXMLHandler::convertCSV (std::vector<int> int_ar){
	return convertCSV (int_ar, int_ar.size());
}

// convert csv to int vector
std::string TppXMLHandler::convertCSV (std::vector<int> int_ar, int fixed_len){
	std::string sTmp = "";
	int size = std::max ((int)int_ar.size(), fixed_len);

	for (int i = 0; i < size; i++)
	{
		if (i < int_ar.size())
		{
			sTmp+= FMT("%d", int_ar[i]);
		}
		if (i < fixed_len - 1)
			sTmp+= ",";
	}
	return sTmp;
}

// compare 2 prophet detail result
bool TppXMLHandler::compareInterProphetProbability( 
	kome::Tpp::TppXMLHandler::T_ProphetDetailResult& left,
	kome::Tpp::TppXMLHandler::T_ProphetDetailResult& right)
{
	return left.InterProphetProbability > right.InterProphetProbability;
}

// proteinîzóÒÇprobability>PercentCoverageèáÇ…ï¿Ç◊ÇÈ
bool TppXMLHandler::compareProbability(
	kome::Tpp::TppXMLHandler::T_ProphetResult& left,
	kome::Tpp::TppXMLHandler::T_ProphetResult& right)
{
	if (left.ProteinProphetProbability== right.ProteinProphetProbability)
	{
		return left.PercentCoverage < right.PercentCoverage;
	} else
		return left.ProteinProphetProbability > right.ProteinProphetProbability;
}

bool TppXMLHandler::comareDetailResultHisgam ( 
	TppXMLHandler::T_DetailResultHistgram& left,
	TppXMLHandler::T_DetailResultHistgram& right)
{
	return left.DetailResult->InterProphetProbability
	 > right.DetailResult->InterProphetProbability;
}
