/*********************************************************/
/*!
 @addtoGroup AB3D
 @file TempContainer.h
 @brief Temp Table Accesser Base
 
 @date 2013.07.01
 @author M.Fukuda
 @varsion 1.0.0

 -------------------------------------------------------
 Copyright(C) 2006-2014
 Eisai Co., Ltd. All rights reserved.
 -------------------------------------------------------
 @note
   Temp Tableに保存した結果を
   保存、読み出すためのオブジェクト
 */
/*********************************************************/

#pragma once

namespace AB3D {

// DataPoint(x,y)
struct ElmPoint {
	double x; // Retention Time
	double y; // Intensity

	// constructor
	ElmPoint(void) : x(-1),y(-1){}
};

// Matrix Value Peak
// 
struct TmpMXVal {
	int profId;    // ProfileID (DataBase ID)
	ElmPoint core; // The top of the peak
	ElmPoint left; // The left edge of the peak
	ElmPoint right;// The right edge of the peak
	double value;  // peaks value(Max Intensity or Area)

	// constructor
	TmpMXVal(void)
		: profId(-1)
		, core()
		, left()
		, right()
		, value(-1.0)
	{}
};

// Peak2D Value
struct TmpPeakVal {
	int sampleId;   // SampleID (Database sample Id)
	double rt;      // Retention time
	double mz;      // m/z
	int charge;     // charge
	TmpMXVal mxval; // peak objects

	// constructor
	TmpPeakVal() 
		: sampleId(-1)
		, rt(-1.0)
		, mz(-1.0)
		, charge(-1)
		, mxval()
	{}
};

}
