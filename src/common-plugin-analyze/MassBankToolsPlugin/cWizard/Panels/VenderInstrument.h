/**
 * @file VenderInstrument.h
 * @brief Vender&Instrument Select Combo Box Informations
 *
 * @author M.Fukuda
 * @date 2013.01.18
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#pragma once

#include <string>
#include <vector>
#include "../rel_stdafx.h"

/**
 @note
   ベンダーとInstの構造体
   Vender(表記名、内部名、説明）
   Instrument(表記名、内部名、説明、ベンダー情報（ポインタ））を持つ
*/

// Vender Set
namespace obo {

struct venderSt {
	const std::string name;        // Vender name
	const std::string sname;       // short name (for mass++ element)
	const std::string description; // description (from obo file)

	// constructor
	venderSt(
		const std::string& _name, 
		const std::string& _sname, 
		const std::string& _desc
		) : name(_name), sname(_sname), description(_desc){}
};

typedef std::vector<venderSt> VenderArr;

// instrument
struct instrumentSt {
	const std::string name;        // instrument name
	const std::string sname;
	const std::string description; // desctiption(from obo file)
	const std::string svendername;

	//constructor
	instrumentSt(
		const std::string& _name, 
		const std::string& _sname, 
		const std::string& _desc, 
		const std::string& _svendername
		) : name(_name), sname(_sname),description(_desc), svendername(_svendername){}
};

typedef std::vector<instrumentSt> InstArr;

// Vender,InstrumentNameの配列と、
// KeyInstに合致するVender-InstrumentNameを返す。

void getVenderArray (VenderArr& venderArr);

void getInstrumentArray (InstArr& venderArr);

void getVenderAndInstInfo (
	const std::string& keyInst,
	VenderArr& venderArr, 
	InstArr& instArr,
	std::string& foundVender,
	std::string& foundInst
);


}
