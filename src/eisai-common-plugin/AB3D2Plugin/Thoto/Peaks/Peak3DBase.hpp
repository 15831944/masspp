/****************************************************************
  Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
-----------------------------------------------------------------
	@addtogroup Refine_Thoto_Peaks
	@file       Peak3DBase_Def.hpp
	@brief      3DPeak の実装部
	@date       2012.12.13
	@author     M.Fukuda
	@version    1.2.0
****************************************************************/

#pragma once

#include "Peak3DBase_Def.hpp"

#include <vector>
#include <boost/intrusive_ptr.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm_ext/erase.hpp>

#include "../Standardinline.hpp"
#include "../Points/LRPoint.hpp"

typedef BS::Peak3D* Ptr;
typedef std::vector<Ptr> vPeak3D;

struct compPeak3DIntensity {
	bool operator()(const Ptr& p, const double& rx) const { return p->intensity > rx; }
	bool operator()(const double& lx, const Ptr& p) const { return lx > p->intensity; }
	bool operator()(const Ptr& lp, const Ptr& rp) const { return lp->intensity > rp->intensity;} 
};

struct compPeak3DLogIntensity {
	bool operator()(const Ptr& p, const double& rx) const { return p->logintensity > rx; }
	bool operator()(const double& lx, const Ptr& p) const { return lx > p->logintensity; }
	bool operator()(const Ptr& lp, const Ptr& rp) const { return lp->logintensity > rp->logintensity;} 
};

struct compPeak3DRT {
	bool operator()(const Ptr& p, const double& rx) const { return p->rt.c < rx; }
	bool operator()(const double& lx, const Ptr& p) const { return lx < p->rt.c; }
	bool operator()(const Ptr& lp, const Ptr& rp) const { return lp->rt.c < rp->rt.c;} 
};

struct compPeak3DMZ {
	bool operator()(const Ptr& p, const double& rx) const { return p->mz.c < rx; }
	bool operator()(const double& lx, const Ptr& p) const { return lx < p->mz.c; }
	bool operator()(const Ptr& lp, const Ptr& rp) const { return lp->mz.c < rp->mz.c;} 
};

class overInt : public std::unary_function<const Ptr&, bool>{
private:
	const double lessInt;
public:
	overInt (const double& _lessint) : lessInt(_lessint){}
	bool operator()(const Ptr& p) const {
		if (!p->sflag) return false;
		if (p->intensity < lessInt) return true;
		return false;
	}
};

class compIntensity : public std::unary_function<const Ptr&, bool>{
private:
	const double threshold;
	const bool isAbove;
	const bool equal;
public:
	compIntensity (const double& threshold, const bool& isAbove, const bool& equal) : threshold(threshold), isAbove(isAbove), equal(equal){}
	bool operator()(const Ptr& p) const {
		if (!p->sflag) return false;
		return ((p->intensity > threshold) == isAbove || (equal && p->intensity == threshold));
	}
};

class InsideMZRange : public std::unary_function<const Ptr&, bool>{
private:
	RFN::LR::LRP_ mz;
public:
	InsideMZRange (
		const double& mz_left,
		const double& mz_right
		) : mz(mz_left, mz_right){}
	bool operator()(const Ptr& p) const {
		if (!p->sflag) return false;
		if (!RFN::LR::withinLimits(p->mz.c, mz)) return false;
		return true;
	}
};

class InsideRTRange {
private:
	RFN::LR::LRP_ rt;
public:
	InsideRTRange (
		const double& rt_left,
		const double& rt_right
		) : rt(rt_left, rt_right) {}
	bool operator()(const Ptr& p) const {
		if (!p->sflag) return false;
		if (!RFN::LR::withinLimits(p->rt.c, rt)) return false;
		return true;
	}
};

class InsideMZRTRange {
private:
	RFN::LR::MZRTP_ mzrt;
public:
	InsideMZRTRange (
		const double& mz_left,
		const double& mz_right,
		const double& rt_left,
		const double& rt_right
		) : mzrt(mz_left, mz_right, rt_left, rt_right) {}
	bool operator()(const Ptr& p) const {
		if (!p->sflag) return false;
		if (!RFN::LR::withinMZLimits(p->mz.c, mzrt)) return false;
		if (!RFN::LR::withinRTLimits(p->rt.c, mzrt)) return false;
		return true;
	}
};

class OverlapRTRange {
private:
	double rt_;
public:
	OverlapRTRange (
		const double& rt
	) : rt_(rt) {}
	bool operator()(const Ptr& p) const {
		if (!p->sflag) return false;
		if (p->rt.l < rt_ && rt_ < p->rt.r) return true;
		//if (p->rt.l <= rt_ && rt_ <= p->rt.r) return true;
		return false;
	}
};

class NearestMZ {
private:
	double mz_;
public:
	NearestMZ (
		const double& mz
		) : mz_(mz) {}
	bool operator()(const Ptr& p1, const Ptr& p2) const {
		return std::abs(p1->mz.c - mz_) < std::abs(p2->mz.c - mz_);
	}
};

namespace Cnum {

	struct greaterZ : public std::binary_function<const Ptr&, const Ptr&, bool> {
	bool operator()(const Ptr& lxyz, const Ptr& rxyz) const {
		return lxyz->intensity > rxyz->intensity;
		}
	};

	void sortByIntensity_DESC(vPeak3D& src) {
		std::sort( src.begin(), src.end(), greaterZ());
	}

	struct lessRTC : public std::binary_function<const Ptr&, const Ptr&, bool> {
	bool operator()(const Ptr& lxyz, const Ptr& rxyz) const {
		return lxyz->rt.c < rxyz->rt.c;
		}
	};

	inline void sortByRTcenter(vPeak3D& src) {
		std::sort(src.begin(), src.end(), lessRTC());
	}
	
	struct lessMZC : public std::binary_function<const Ptr&, const Ptr&, bool> {
	bool operator()(const Ptr& lxyz, const Ptr& rxyz) const {
		return lxyz->mz.c < rxyz->mz.c;
		}
	};

	inline void sortByMZcenter(vPeak3D& src) {
		std::sort(src.begin(), src.end(), lessMZC());
	}

	struct deletePtrOnly : public std::unary_function<const Ptr&, bool> {
		bool operator()(const Ptr& ptr) const {
			return ! ptr->sflag;
		}
	};

	struct deletePtr : public std::unary_function<const Ptr&, bool> {
		bool operator()(const Ptr& ptr) const {
			if (ptr->sflag) { return false;}
			delete ptr;
			return true;
		}
	};

	struct isUsePtr : public std::unary_function<const Ptr&, bool> {
		bool operator()(const Ptr& ptr) const {
			return ptr->sflag;
		}
	};


	inline void deletePtrB(Ptr& ptr) {
		if (!ptr->sflag) {
			delete ptr;
			ptr = 0;
		}
	}

	struct deletePtrOn : public std::binary_function<const Ptr&, const Ptr&, bool> {
		bool operator()(const Ptr& ptr, const Ptr& lptr) const {
			return ptr->sflag > lptr->sflag;
		}
	};

	inline void deleteFalseFlagObject(vPeak3D& src) {
//		std::sort(src.begin(), src.end(), deletePtrOn());
//		src.erase(std::remove(src.begin(),src.end(), static_cast<Ptr>(0)),src.end());
		boost::remove_erase_if(src, deletePtrOnly());
	}

//	inline void deleteFalseFlagObject2(vPeak3D& src) {
//		vPeak3D tmp = src;
//		src.clear();
//		std::copy_if(tmp.begin(), tmp.end(), std::back_inserter(src), isUsePtr());
////		std::sort(src.begin(), src.end(), deletePtrOn());
////		src.erase(std::remove(src.begin(),src.end(), static_cast<Ptr>(0)),src.end());
////		boost::remove_erase_if(src, deletePtrOnly());
//		src.shrink_to_fit();
//	}

	inline void removePeaksFromList(vPeak3D& src) {
		std::for_each(src.begin(),src.end(), deletePtrB);
		src.erase(std::remove(src.begin(), src.end(), static_cast<Ptr>(0)), src.end());
		src.shrink_to_fit();
//		boost::remove_erase_if(src, deletePtr());
	}

	inline void deleteAllElm(vPeak3D& src) {
		vPeak3D::iterator it = src.begin();
		vPeak3D::iterator eit = src.end();
		while(it != eit) {
			delete *it;
			++it;
		}
	}

//	inline bool getTrueTop(const vPeak3D& src, Ptr& rst) {
//		vPeak3D::const_iterator it = boost::find_if(src, isUsePtr());
//		if (it == src.end()) return false;
//		rst = *it;
//		return true;
//	}


	inline bool setForeFront(vPeak3D& src, Ptr& rst, vPeak3D::iterator& oldit) {
		vPeak3D::iterator it = std::find_if(oldit, src.end(), isUsePtr());
		if (it == src.end()) return false;
		rst = *it;
		oldit = it;
		return true;
	}

	class DeletelessInt : public std::unary_function<const ELM::PK3D_ptr&, bool > {
	private:
		const double tgtInt;
	public :
		DeletelessInt(const double& lessInt) : tgtInt(lessInt){}
		bool operator()(const ELM::PK3D_ptr& p) const {
			if (p->intensity < tgtInt) {
				delete p;
				return true;
			}
			return false;
		}
	};

	inline void eraseRemoveFalseFlagObject(vPeak3D& src, const double& lessInt) {
		boost::remove_erase_if(src, DeletelessInt(lessInt));
		src.shrink_to_fit();
	}

/*
	inline bool getInsideElements(
		vPeak3D& src,
		const double& limitintensity,
		vPeak3D& rst
	) {
		// Intensity の　End Point
		vPeak3D::iterator endIt = boost::lower_bound(src, limitintensity, compPeak3DIntensity());
		std::copy(src.begin(), endIt, std::back_inserter(rst));
		return (!rst.empty());
	}
*/
	inline void getPeaksAboveIntensity(vPeak3D& peaksSortedByIntensity,
				const double& intensity, vPeak3D& dstPeaks) {
		std::copy_if(
			peaksSortedByIntensity.begin(), peaksSortedByIntensity.end(),
			std::back_inserter(dstPeaks), compIntensity(intensity, true, true));
	}

/*
	inline bool getInsideElements(
		vPeak3D& src,
		const double& limitintensity,
		const double& mz_left,
		const double& mz_right,
		vPeak3D& rst
	) {
		// Intensity の　End Point
		vPeak3D::iterator endIt = boost::lower_bound(src, limitintensity, compPeak3DIntensity());
	//	if (endIt == src.end()) return false;
		std::copy_if(src.begin(), endIt, std::back_inserter(rst), InsideMZRange(mz_left,mz_right));
		return (!rst.empty());
	}

	inline bool getInsideElements_withItr(
		vPeak3D& src,
		const ITR::vPeak3D_Itr& begIt,
		const double& limitintensity,
		const double& mz_left,
		const double& mz_right,
		vPeak3D& rst
	) {
		// Intensity の　End Point
		vPeak3D::iterator endIt = std::lower_bound(begIt, src.end(), limitintensity, compPeak3DIntensity());
	//	if (endIt == src.end()) return false;
		size_t ff = std::distance(begIt,endIt);
		rst.reserve(ff);
		std::copy_if(begIt, endIt, std::back_inserter(rst), InsideMZRange(mz_left,mz_right));
		return (!rst.empty());
	}
*/
/*
	inline bool getInsideElements_withItrEx(
		vPeak3D& src,
		const double& limitintensity,
		const double& mz_left,
		const double& mz_right,
		vPeak3D& rst
	) {
		ITR::vPeak3D_Itr fstIt = std::lower_bound(src.begin(), src.end(), mz_left, compPeak3DMZ());
		// fst 以降にしかないので、先頭をsrc.begin -> fstItに変更。若干短縮可能？
		ITR::vPeak3D_Itr endIt = std::lower_bound(fstIt, src.end(), mz_right, compPeak3DMZ());
		rst.reserve(std::distance(fstIt, endIt));
		std::copy_if(fstIt, endIt, std::back_inserter(rst), overInt(limitintensity));
		return (!rst.empty());
	}
*/
/*
	inline bool getInsideElements(
		vPeak3D& src,
		const double& lower_intensity,
		const double& upper_intensity,
		const double& mz_left,
		const double& mz_right,
		vPeak3D& rst
	) {
		vPeak3D::iterator begIt = boost::upper_bound(src, upper_intensity, compPeak3DIntensity());
		vPeak3D::iterator endIt = boost::lower_bound(src, lower_intensity, compPeak3DIntensity());
		std::copy_if(begIt, endIt, std::back_inserter(rst), InsideMZRange(mz_left, mz_right));
		return (!rst.empty());
	}
/*
	inline bool getInsideElements(
		vPeak3D& src,
		const double& limitintensity,
		const double& mz_left,
		const double& mz_right,
		const double& rt_left,
		const double& rt_right,
		vPeak3D& rst
	) {
		// Intensity の　End Point
		vPeak3D::iterator endIt = boost::lower_bound(src, limitintensity, compPeak3DIntensity());
	//	if (endIt == src.end()) return false;
		std::copy_if(src.begin(), endIt, std::back_inserter(rst), InsideMZRTRange(mz_left,mz_right, rt_left, rt_right));
		return (!rst.empty());
	}
*/



	inline bool getPeaksFromSortedByIntensityDesc(
			vPeak3D& peaksSortedByIntensity,
			const double& lowerIntensity, const double& upperIntensity,
			const double& lowerMz, const double& upperMz,
			vPeak3D& dstPeaks) {
		vPeak3D::iterator upperItr = boost::lower_bound(peaksSortedByIntensity, upperIntensity, compPeak3DIntensity());
		vPeak3D::iterator lowerItr = boost::upper_bound(peaksSortedByIntensity, lowerIntensity, compPeak3DIntensity());
		dstPeaks.reserve(std::distance(upperItr, lowerItr));
		if (lowerMz > 0 && upperMz > 0) {
			std::copy_if(upperItr, lowerItr, std::back_inserter(dstPeaks), InsideMZRange(lowerMz, upperMz));
		} else {
			std::copy(upperItr, lowerItr, std::back_inserter(dstPeaks));
		}
		dstPeaks.shrink_to_fit();
		return (!dstPeaks.empty());
	}

	// RTの範囲指定を無効にする場合は、lowerRtとupperRtのいずれかを-1と指定する。
	inline bool getPeaksFromSortedByMz(
			vPeak3D& peaksSortedByMz,
			const double& lowerMz, const double& upperMz,
			const double& lowerRt, const double& upperRt,
			vPeak3D& dstPeaks) {

		vPeak3D::iterator lowerItr = std::lower_bound(peaksSortedByMz.begin(), peaksSortedByMz.end(), lowerMz, compPeak3DMZ());
		vPeak3D::iterator upperItr = std::upper_bound(lowerItr, peaksSortedByMz.end(), upperMz, compPeak3DMZ());
		//vPeak3D::iterator lowerItr = boost::lower_bound(peaksSortedByMz, lowerMz, compPeak3DMZ());
		//vPeak3D::iterator upperItr = boost::upper_bound(peaksSortedByMz, upperMz, compPeak3DMZ());
		dstPeaks.reserve(std::distance(lowerItr, upperItr));
		if (lowerRt > 0 && upperRt > 0) {
			std::copy_if(lowerItr, upperItr, std::back_inserter(dstPeaks), InsideRTRange(lowerRt, upperRt));
		} else {
			std::copy(lowerItr, upperItr, std::back_inserter(dstPeaks));
		}
		dstPeaks.shrink_to_fit();
		return (!dstPeaks.empty());
	}

	inline bool getPeaksBelowIntensityFromSortedByMz(
			vPeak3D& peaksSortedByMz,
			const double& intensity,
			const double& lowerMz, const double& upperMz,
			const double& lowerRt, const double& upperRt,
			vPeak3D& dstPeaks) {
		vPeak3D tmpPeaks;
		if (!getPeaksFromSortedByMz(peaksSortedByMz, lowerMz, upperMz, lowerRt, upperRt, tmpPeaks)) {
			return false;
		}
		std::copy_if(tmpPeaks.begin(), tmpPeaks.end(), std::back_inserter(dstPeaks),
			compIntensity(intensity, false, true));
		return (!dstPeaks.empty());
	}
/*
	inline	bool getInsideElementsFullRT(
		vPeak3D& src,
		const double& mz_left,
		const double& mz_right,
		const double& rt_left,
		const double& rt_right,
		vPeak3D& rst
	) {
		// Intensity の　End Point
		vPeak3D::iterator fstIt = boost::lower_bound(src, mz_left, compPeak3DMZ());
		vPeak3D::iterator endIt = boost::lower_bound(src, mz_right, compPeak3DMZ());
		rst.reserve(std::distance(fstIt, endIt));
		std::copy_if(fstIt, endIt, std::back_inserter(rst), InsideRTRange(rt_left,rt_right));
		rst.shrink_to_fit();
		//vPeak3D::iterator fstIt = boost::lower_bound(src, rt_left, compPeak3DRT());
		//vPeak3D::iterator endIt = boost::lower_bound(src, rt_right, compPeak3DRT());
		//rst.reserve(std::distance(fstIt,endIt));
		//std::copy_if(fstIt, endIt, std::back_inserter(rst), InsideMZRange(mz_left,mz_right));
		//rst.shrink_to_fit();
		return (!rst.empty());
	}
*/
/*
	inline bool getInsideElementsByLogIntensity(
		vPeak3D& src,
		const double& limitlogintensity,
		vPeak3D& rst
	) {
		// LogIntensity の　End Point
		vPeak3D::iterator endIt = boost::lower_bound(src, limitlogintensity, compPeak3DLogIntensity());
		std::copy(src.begin(), endIt, std::back_inserter(rst));
		return (!rst.empty());
	}
*/
//	inline bool getInsideElementsByMZ(
//		vPeak3D& peaksSortedByMz,
//		const double& lowerMz,
//		const double& upperMz,
//		vPeak3D& dstPeaks
//	) {
//		// Intensity の　End Point
//		dstPeaks.reserve(peaksSortedByMz.size());
////		rst.reserve(256);
//		std::copy_if(
//			peaksSortedByMz.begin(), peaksSortedByMz.end(),
//			std::back_inserter(dstPeaks), InsideMZRange(lowerMz, upperMz));
//		dstPeaks.shrink_to_fit();
//		return (!dstPeaks.empty());
//	}
/*
	inline bool getInsideElementsByRT(
		vPeak3D& src,
		const double& rt_left,
		const double& rt_right,
		vPeak3D& rst
	) {
		rst.reserve(256);
		std::copy_if(src.begin(), src.end(), std::back_inserter(rst), InsideRTRange(rt_left, rt_right));
		rst.shrink_to_fit();
		return (!rst.empty());
	}
*/

//	inline bool getInsideElementsByRT(
	inline bool getPeaksFromSortedByRT(
			vPeak3D& peaksSortedByRT,
			const double& lowerMz, const double& upperMz,
			const double& lowerRt, const double& upperRt,
			vPeak3D& dstPeaks) {

		vPeak3D::iterator lowerItr = std::lower_bound(peaksSortedByRT.begin(), peaksSortedByRT.end(), lowerRt, compPeak3DRT());
		vPeak3D::iterator upperItr = std::upper_bound(lowerItr, peaksSortedByRT.end(), upperRt, compPeak3DRT());
//		vPeak3D::iterator lowerItr = boost::lower_bound(peaksSortedByRT, lowerRt, compPeak3DRT());
//		vPeak3D::iterator upperItr = boost::upper_bound(peaksSortedByRT, upperRt, compPeak3DRT());
		dstPeaks.reserve(std::distance(lowerItr, upperItr));
		if (lowerMz > 0 && upperMz > 0) {
			std::copy_if(lowerItr, upperItr, std::back_inserter(dstPeaks), InsideMZRange(lowerMz, upperMz));
		} else {
			std::copy(lowerItr, upperItr, std::back_inserter(dstPeaks));
		}
		dstPeaks.shrink_to_fit();
		return (!dstPeaks.empty());
	}

//	inline bool getOverlappedElementsByRT(
	inline bool getRTOverlappedPeaks(
		vPeak3D& srcPeaks,
		const double& rt,
		vPeak3D& rstPeaks
	) {
		rstPeaks.reserve(srcPeaks.size());
		std::copy_if(srcPeaks.begin(), srcPeaks.end(),
			std::back_inserter(rstPeaks), OverlapRTRange(rt));
		rstPeaks.shrink_to_fit();
		return (!rstPeaks.empty());
	}

	inline void getNearestMzPeak(
		vPeak3D& src,
		const double& mz,
		Ptr& rst
	) {
		vPeak3D::iterator it = std::min_element(src.begin(), src.end(), NearestMZ(mz));
		rst = *it;
	}

	inline void setFalse(Ptr& ptr) { ptr->sflag = false; }
	inline void setTrue(Ptr& ptr) { ptr->sflag = true; }

	inline void setFlagToPeaks(vPeak3D& peaks, bool flg) {
		std::for_each(peaks.begin(), peaks.end(), flg?setTrue:setFalse);
	}
}