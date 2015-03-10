/**
 * @file GlycanAnalysisStructures.h
 * @brief Structure Definitions for Glycan Analysis
 *
 * @author N.Tashiro
 * @date 2012.04.02
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_GLYCAN_ANALYSIS_STRUCTURES_H__
#define __KOME_GLYCAN_ANALYSIS_STRUCTURES_H__

namespace kome {
	namespace glycan{
		/**
		 * @struct GlycanParams
		 * @brief parameters for calculate
		 */
		struct GlycanParams{
			std::string strLabel;
			double dTolerance;
			std::string strAnion;
		};

		/**
		 * @struct Annotation
		 * @brief annotation
		 */
		struct Annotation{
			double	mz;
			std::string	strAnn;
			unsigned int nodeNum;
			double x1;
			double x2;
			double x3;
			double y1;
			double y2;
			double y3;
		};

		/**
		 * @struct NGycan
		 * @brief Annotations for gNumber glycan
		 */
		struct NGlycan{
			std::vector<Annotation> annotations;
			unsigned int gNumber;
			unsigned int unTheoretical;
			unsigned int unMatched;
			double dRatio;
		};

		/**
		 * @struct GLYCAN_LABEL
		 * @brief Labeling parameter
		 */
		struct GLYCAN_LABEL{
			std::string strLabelName;
			double dLabelMz;
		};

		/**
		 * @struct GLYCAN_ANION
		 * @brief Anion parameter
		 */
		struct GLYCAN_ANION{
			std::string strAnionName;
			double dAnionMz;
		};

		/**
		 * @struct GLYCAN_NODE
		 * @brief Node Information
		 */
		struct GLYCAN_NODE{
			unsigned int unNodeID;
			std::string strNodeName;
			int nodeX;
			int nodeY;
		};

		/**
		 * @struct GLYCAN_EDGE
		 * @brief Edge Information
		 */
		struct GLYCAN_EDGE{
			unsigned int unEdgeID;
			unsigned int unStartID;
			std::string strAnomerAndDonor;
			unsigned int unEndID;
			std::string strCarbon;
		};

		/**
		 * @struct GLYCAN_STRUCTURE
		 * @brief Structure of Nodes and Edges
		 */
		struct GLYCAN_STRUCTURE{
			std::string strGNumber;
			std::vector< GLYCAN_NODE > nodeList;
			std::vector< GLYCAN_EDGE > edgeList;
			std::string strCompound;
// 2014/02/20 Add by nakaya The addition of a display item  ->
			std::string strRank;
			std::string strAssigned;
// 2014/02/20 Add by nakaya The addition of a display item  <-
			std::string strScore;
		};

	}
}

#endif	//__KOME_GLYCAN_ANALYSIS_STRUCTURES_H__
