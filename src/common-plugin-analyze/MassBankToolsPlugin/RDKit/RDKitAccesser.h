/**
 * @file molFile Reader (on RDKits)
 * @brief get mol Structures
 *
 * @author M.Fukuda
 * @date 2013.03.05
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 *
 * @ note -=====================================
 *  Need set [include Path] to RDKit Headers.
 *  Need set [lib Path] to RDKit Libs.
 *  And need Lib,include h, show molFileData.cpp.
 *
 */

#pragma once

#include <string>

namespace mol {

	const bool getMolDataFromFileSelect(
		std::string& massStr,
		std::string& formula,
		std::string& smiles,
		std::string& inchi);

	const bool getMolDataFromURL(
		const std::string& tgtURL,
		std::string& massStr,
		std::string& formula,
		std::string& smiles,
		std::string& inchi);

	const bool drawSmiles(
		const std::string& simles,
		const std::string& outputFilePath,
		const int width,
		const int height);


}// end of namespace
