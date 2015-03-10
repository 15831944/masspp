/**
 * @file IdentMgrAcc.h
 * @brief Identification Manager Wrapper
 *
 * @author M.Fukuda
 * @date 2013.01.18
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */
#pragma once

namespace Ident {

	std::string getPrefixNum_Str(
		const std::string& preval);

	const int getPrefixNum(
		const std::string& preval);

	void setPrefixAndNum(
		const std::string& prefix,
		const int lastnum);

}
