/**
 * @file DataProcessing
 * @brief DataProcessing Panel 
 *
 * @author M.Fukuda
 * @date 2013.03.08
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#pragma once

#include "DataProcessing.h"
#include "AuxiliaryToolOfCreatingPanel.h"

// DetaProcessingのデータ構造体
struct dpElm {
	const std::string showname; // 表示名
	const std::string val;      // 実際の値
	const std::string desc;     // 説明

	// constructor
	dpElm(
		const std::string& _showname, 
		const std::string& _val, 
		const std::string& _desc
		) : showname(_showname), val(_val), desc(_desc){}
};

const std::string kDataProcessing("Data Processing");

const std::string k_help_processing = 
	"Input data processing methods before peak detection.";

// -- Forward declaration --
// Data Processingの値を作成する
void setAllDpElms(std::vector<dpElm>& dpElmArr);


kome::window::SettingsPanel* createDataProccessingListBox(
	wxWindow* parent,
	kome::plugin::ParameterSettings& params,
	kome::objects::SettingParameterValues& settings,
	const bool saveVal
) {
	// 内部的（下記)から値を取得。
	std::vector<dpElm> dpElmArr;
	setAllDpElms(dpElmArr);
	
	// Listに展開
	using namespace kome::plugin;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG

	SettingsValue* val = params.addValue();
	val->setType(SettingsValue::TYPE_LIST);

	SettingsValue* subVal = params.addValue();
	subVal->setType(SettingsValue::TYPE_STRING);
	subVal->setRequired(true);
	subVal->setName(kDataProcessing.c_str());

	//取得した内容を割り当てる
	for each(auto& tmp in dpElmArr) {
		subVal->addEnumItem(tmp.showname.c_str(), tmp.val.c_str(), tmp.desc.c_str());
	}

	val->addChildValue(subVal);

	SettingsPage* page = new kome::plugin::SettingsPage(params);
	pfc::setParamAndFormAdpt(params, val, page, kDataProcessing, kDataProcessing, k_help_processing,true);

	kome::window::SettingsPanel* panel1 = new kome::window::SettingsPanel(parent, page, false, &settings, saveVal);
	return panel1;
}

// Data Process の中身を定義する（内容は oboをパースして整形したデータ）
void setAllDpElms(std::vector<dpElm>& dpElmArr) {
dpElmArr.push_back(dpElm("data processing action","data processing action","Data processing attribute used to describe the type of data processing performed on the data file. [PSI:MS]"));
dpElmArr.push_back(dpElm("....deisotoping","deisotoping","The removal of isotope peaks to represent the fragment ion as one data point and is commonly done to reduce complexity. It is done in conjunction with the charge state deconvolution. [PSI:MS]"));
dpElmArr.push_back(dpElm("....charge deconvolution","charge deconvolution","The determination of the mass of an ion based on the mass spectral peaks that represent multiple-charge ions. [PSI:MS]"));
dpElmArr.push_back(dpElm("....peak picking","peak picking","Spectral peak processing conducted on the acquired data to convert profile data to centroided data. [PSI:MS]"));
dpElmArr.push_back(dpElm("........area peak picking","area peak picking","Spectral peak processing conducted on the acquired data to convert profile data to centroided data. The area defined by all raw data points that belong to the peak is reported. [PSI:MS]"));
dpElmArr.push_back(dpElm("............sophisticated numerical annotation procedure","sophisticated numerical annotation procedure","It searches for known patterns in the measured spectrum. [DOI:10.1021/ac951158i, PMID:21619291]"));
dpElmArr.push_back(dpElm("........height peak picking","height peak picking","Spectral peak processing conducted on the acquired data to convert profile data to centroided data. The maximum intensity of all raw data points that belong to the peak is reported. [PSI:MS]"));
dpElmArr.push_back(dpElm("....smoothing","smoothing","A process of reducing spikes of intensity in order to reduce noise while preserving real peak signal. Many algorithms can be applied for this process. [PSI:MS]"));
dpElmArr.push_back(dpElm("........Savitzky-Golay smoothing","Savitzky-Golay smoothing","Reduces intensity spikes by applying local polynomial regression (of degree k) on a distribution (of at least k+1 equally spaced points) to determine the smoothed value for each point. It tends to preserve features of the distribution such as relative maxima, minima and width, which are usually 'flattened' by other adjacent averaging techniques. [PSI:MS]"));
dpElmArr.push_back(dpElm("........LOWESS smoothing","LOWESS smoothing","Reduces intensity spikes by applying a modeling method known as locally weighted polynomial regression. At each point in the data set a low-degree polynomial is fit to a subset of the data, with explanatory variable values near the point whose response is being estimated. The polynomial is fit using weighted least squares, giving more weight to points near the point whose response is being estimated and less weight to points further away. The value of the regression function for the point is then obtained by evaluating the local polynomial using the explanatory variable values for that data point. The LOESS fit is complete after regression function values have been computed for each of the n data points. Many of the details of this method, such as the degree of the polynomial model and the weights, are flexible. [PSI:MS]"));
dpElmArr.push_back(dpElm("........Gaussian smoothing","Gaussian smoothing","Reduces intensity spikes by convolving the data with a one-dimensional Gaussian function. [PSI:MS]"));
dpElmArr.push_back(dpElm("........moving average smoothing","moving average smoothing","Reduces intensity spikes by averaging each point with two or more adjacent points. The more adjacent points that used, the stronger the smoothing effect. [PSI:MS]"));
dpElmArr.push_back(dpElm("........wavelet transformation smoothing","wavelet transformation smoothing","The random noise is removed by using the undecimated wavelet transform. [DOI:10.1093/bioinformatics/btl355, PMID:16820428]"));
dpElmArr.push_back(dpElm("....baseline reduction","baseline reduction","A process of removal of varying intensities generated due to variable energy absorption before further processing can take place. Baseline reduction facilitates meaningful comparision between intensities of m/z values. [PSI:MS]"));
dpElmArr.push_back(dpElm("........top hat baseline reduction","top hat baseline reduction","Top-hat morphological filter based on the basic morphological operations 'erosion' and 'dilatation'. [PSI:MS]"));
dpElmArr.push_back(dpElm("........convex hull baseline reduction","convex hull baseline reduction","Constructs the baseline by fitting multiple parabolas to the spectrum starting with the large scale structures. [PSI:MS]"));
dpElmArr.push_back(dpElm("........median baseline reduction","median baseline reduction","The spectrum that will be baseline subtracted is divided into a number of segments. [PSI:MS]"));
dpElmArr.push_back(dpElm("....retention time alignment","retention time alignment","The correction of the spectrum scan times, as used e.g. in label-free proteomics. [PSI:MS]"));
dpElmArr.push_back(dpElm("....charge state calculation","charge state calculation","A process that infers the charge state of an MSn spectrum's precursor(s) by the application of some algorithm. [PSI:MS]"));
dpElmArr.push_back(dpElm("........below precursor intensity dominance charge state calculation","below precursor intensity dominance charge state calculation","Infers charge state as single or amibiguously multiple by determining the fraction of intensity below the precursor m/z. [PSI:MS]"));
dpElmArr.push_back(dpElm("....precursor recalculation","precursor recalculation","A process that recalculates existing precursor selected ions with one or more algorithmically determined precursor selected ions. [PSI:MS]"));
dpElmArr.push_back(dpElm("........msPrefix precursor recalculation","msPrefix precursor recalculation","Recalculates one or more precursor selected ions by peak detection in the isolation windows of high accuracy MS precursor scans. [PSI:MS]"));
dpElmArr.push_back(dpElm("....intensity normalization","intensity normalization","Normalization of data point intensities. [PSI:MS]"));
dpElmArr.push_back(dpElm("........area normalization","area normalization","Normalization of areas below the curves. [PSI:MS]"));
dpElmArr.push_back(dpElm("....m/z calibration","m/z calibration","Calibration of data point m/z positions. [PSI:MS]"));
dpElmArr.push_back(dpElm("....data filtering","data filtering","Filtering out part of the data. [PSI:MS]"));
dpElmArr.push_back(dpElm("........low intensity data point removal","low intensity data point removal","The removal of very low intensity data points that are likely to be spurious noise rather than real signal. [PSI:MS]"));
dpElmArr.push_back(dpElm("........high intensity data point removal","high intensity data point removal","The removal of very high intensity data points. [PSI:MS]"));
}
