/*********************************************************/
/*!
 @addtoGroup AB3D
 @file NewAB3DLabeling.h
 @brief AB3DLabeling Functions.
 
 @date 2013.07.01
 @author M.Fukuda
 @varsion 1.0.0

 -------------------------------------------------------
 Copyright(C) 2006-2014 
 Eisai Co., Ltd. All rights reserved.
 -------------------------------------------------------
 @note
 */
/*********************************************************/
#pragma once

#include "Thoto/Peaks/Peak3DBase_Def.hpp"
#include "Thoto/Peaks/PeakBase_Def.hpp"
#include "Thoto\Def_Points.hpp"

typedef BS::Peak3D* Ptr;
typedef std::vector<Ptr> vPeak3D;

namespace ALC{
	class TradingPost;
}

class DbAccess;
namespace AB3D{
	class DbController;
}

//typedef DbAccess tDBAC; 
typedef AB3D::DbController tDBAC; 

class NewAB3DLabeling {
//	class TradingPost;
public:
	// constructor
	NewAB3DLabeling(
		const std::map<std::string, std::string>& setmap) 
		: settingMap_(setmap)
	, myProgress_(nullptr){};

	//destructor
	~NewAB3DLabeling(){};

	void bindProgress(kome::core::Progress* progress);

	// DoEvents (With Tgt peaks2D);
	void doAB3D (
		kome::objects::DataSet& dataSet,
		kome::objects::Peaks2D* peaks2d);

	// DoEvents (With DB)
	void doAB3D (
		kome::objects::DataSet& dataSet, 
		tDBAC* dbac);

private:
	kome::core::Progress* myProgress_;
	std::map<std::string, std::string> settingMap_;

private:
	// Do AB3D Innter Called
	bool onDoAB3D (
		kome::objects::DataSet& dataSet,
		kome::objects::Peaks2D* peaks2d,
		tDBAC* dbac);

	const bool isSelected(const char* key);

	const unsigned int setFalseFlagToUsedPeak3D (
		vPeak3D& peaksSortedByIntensity, 
		const double& lowerMz, const double& upperMz,
		const double& lowerRt, const double& upperRt);

	double getSettingMapDouble(const std::string& pname) const;

	int getSettingMapInteger(const std::string& pname) const;

	bool detectMZRangeByPeakPositions(
		Ptr& tgtPeak, 
		vPeak3D& peaksSortedByIntensity,
		const double& rtTol, 
		const double& availableIntensityRatio,
		ELM::LCRP& mzRange);
	
	inline double calcAreaByXIC(
		ELM::vXYP& xic, 
		const double& lowerRT, 
		const double& upperRT);

	void deleteShoulderPeaks(vPeak3D& detectedPeaks);

	vPeak3D::iterator searchMZRange(
		vPeak3D::iterator it, 
		vPeak3D::iterator& endIt, 
		const double& rtTol, 
		const bool& direction, 
		ELM::LCRP& mzRange);

	void createClusters(
		vPeak3D& detectedPeaks, 
		std::map<Ptr, std::pair<int,Ptr>>& clusterMap, 
		vPeak3D& clusterCandidatePeaks,
		ELM::vXYZP& vxyzp);

	void removeContinuousPeaks(
		vPeak3D& detectedPeaks, 
		vPeak3D& clusterCandidatePeaks);

	bool getXICPeak(ELM::vXYP& xic, const double& rt, ELM::PKPT& foundPeak);

	void toMassppPeak2D(
		kome::objects::Peaks2D* peaks2D, 
		std::map<Ptr, std::pair<int,Ptr>>& clusterChargeMap, 
		vPeak3D& detectedVPK3D
	);

	inline int verifyPeak(
		ELM::vXYP& xic, ELM::PKPT& foundPeak, std::vector<double>& intensities);

	const bool writeToDB(
		tDBAC* dbac,
		bool isSingle, 
		ELM::PK3D_ptr pk, 
		const ELM::vXYP& rstXIC, 
		ELM::vPKPT& xicLWAPeaks,
		ALC::TradingPost& tdPost, 
		std::map<Ptr, int>& dbmap_Prof);

	const bool writeToDB(
		tDBAC* dbac,
		ELM::PK3D_ptr pk, 
		ELM::PKPT& foundpeak,
		const ELM::vXYP& rstXIC, 
		std::map<Ptr, int>& dbmapProf);

	const bool onWriteToDB(
		tDBAC* dbac,
		ELM::PK3D_ptr pk, 
		ELM::PKPT& foundpeak,
		const ELM::vXYP& rstXIC, 
		std::map<Ptr, int>& dbmapProf);

	void writeClusterInfotoDB(
		tDBAC* dbac, 
		std::map<Ptr, std::pair<int,Ptr>>& clusterChargeMap, 
		vPeak3D& detectedVPK3D,
		std::map<Ptr, int>& dbmap_Prof);

	void loadParametersFromFile();

	void NewAB3DLabeling::execTest();
};
