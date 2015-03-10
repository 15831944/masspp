/**
 * @file IdentMgrAcc.cpp
 * @brief Identification Manager Wrapper
 *
 * @author M.Fukuda
 * @date 2013.01.18
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */
#include "rel_stdafx.h"
#include "IdentMgrAcc.h"

#include "ErrorCheck.h"
#include "calcFunc\StrFunc.h"

typedef kome::ident::IdentificationManager kIdentMgr;

namespace Ident {
std::string getPrefixNum_Str(
	const std::string& preval
) {
	int basenum = getPrefixNum(preval);
	return (basenum >= 0) ? ext::toStr(basenum) : "";
}

const int getPrefixNum(
	const std::string& preval
){
	kIdentMgr& identMgr = kIdentMgr::getInstance();
	int basenum = -1;
	if(CHK::checkPrefix(preval)) {
		basenum = identMgr.getAccessionNumber(preval.c_str());
		// 新Prefix : 0 と、一度だけ使ったPrefix : 0　の見分けがつかない為、
		// 取得時に加算を廃止。記録時に加算して、取得時は触らないことにする。
		// ver 2.6.0
		//if (basenum > 0) {
		//	basenum += 1;
		//}
	}
	return basenum;
}

// 最終NUMをして保存する。
// 新Prefix : 0 と、一度だけ使ったPrefix : 0　の見分けがつかない為,
// 記録 - 出力時に数値の調整はしないこととする
void setPrefixAndNum(
	const std::string& prefix,
	const int lastnum
){
	kIdentMgr& identMgr = kIdentMgr::getInstance();
	identMgr.setAccessionNumber(prefix.c_str(), lastnum);
	identMgr.commit();
}


}
