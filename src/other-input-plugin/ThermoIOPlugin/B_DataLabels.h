//---------------------------------------------------------
//
// DataLabel
// Thermo制のデータラベル（Peakの値)の格納に使う。
// 取得できる値は全て構造体に持っている。
//
//---------------------------------------------------------

#pragma once

#include <bitset>
#include <vector>

namespace EMS{
namespace BO{

struct DataLabel {
	double mass;      // Mass (M/Z) 
	double intensity; // Intensity
	double res;       // Resolution
	double baseLine;  // Baseline
	double noise;     // Noise
	int charge;       // Charge
	int Flags;
	std::bitset<6> bflag; //Saturated, Fragmented, Merged, Exception, Reference, Modified
};

class DataLabels {
public:
	DataLabels(void){};
	~DataLabels(void){};

	//Addpoint
	void AddPoint(const double dl[], const unsigned char fgs[]) {
		DataLabel tmp = {dl[0],dl[1],dl[2],dl[3],dl[4]};
		tmp.charge = static_cast<int>(dl[5]);
		tmp.bflag.reset();
		if (fgs != NULL) {
			for (unsigned int i = 0; i < 6; i++) {
				if (fgs[i]) {tmp.bflag.set(i);}
			}
		}
		vdls_.push_back(tmp);
	}

	// Reserve
	void ReserveSize(const unsigned int nSize) {
		vdls_.reserve(nSize);
	}
	
	// Clear
	void ClearAllPoints() {
		vdls_.clear();
		vdls_.shrink_to_fit();
	}

	// GetSize
	size_t GetSize() const {return vdls_.size();}

	// GetLabel Object
	DataLabel GetDLabel(const unsigned int idx);

	void outputMyDataLabels(double*& rstArr, int* rstSize) {
		if(rstArr != nullptr) {
			delete[] rstArr;
			return;
		}
		*rstSize = static_cast<int>(vdls_.size());
		rstArr = new double[vdls_.size() * 4];
		int idx = 0;
		for (std::vector<DataLabel>::iterator it = vdls_.begin(), eit = vdls_.end(); it != eit; ++it) {
			rstArr[idx] = it->mass;
			rstArr[idx + 1] = it->intensity;
			rstArr[idx + 2] = it->res;
			rstArr[idx + 3] = static_cast<double>(it->charge);
			idx += 4;
		}
	}

private:
	std::vector<DataLabel> vdls_;
};

}
}

typedef EMS::BO::DataLabels DLS;
