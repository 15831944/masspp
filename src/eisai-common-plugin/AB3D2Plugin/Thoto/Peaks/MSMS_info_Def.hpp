//// 

//#pragma once
//#include <vector>
//
//namespace RFN {
//namespace SPEC {
//
//struct MS2Info {
//	// msmsinfo
//	double precursor;
//	double my_rt;
//	int my_scanid;
//	// parent info
//	double parent_rt;
//	int parent_scanid;
//
//	MS2Info() 
//		: precursor(-1.0), my_rt(-1.0), my_scanid(-1), parent_rt(-1.0), parent_scanid(-1){}
//	MS2Info(
//		const double& _precursor, 
//		const double& rt, 
//		const int& scanid,
//		const double& parentRT,
//		const int& parentid) 
//		: precursor(_precursor), my_rt(rt), my_scanid(scanid)
//		, parent_rt(parentRT), parent_scanid(parentid)
//	{}
//};
//
//}
//}
//
//namespace ELM {
//	typedef RFN::SPEC::MS2Info MS2info;
//	typedef std::vector<MS2info> vMS2info;
//}