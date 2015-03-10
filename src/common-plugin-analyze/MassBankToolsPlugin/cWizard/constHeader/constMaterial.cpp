/**
 * @file constMaterial.cpp
 * @brief Wizard matrial(Label,ImagePath,..) message declaration
 *
 * @author M.Fukuda
 * @date 2013.12.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "constMaterial.h"

typedef const std::string CSTR;

// Wizard Images
CSTR MTR::W01::IMG = "\\images\\massbank_flow01.png";
CSTR MTR::W02::IMG = "\\images\\massbank_flow02.png";
CSTR MTR::W03::IMG = "\\images\\massbank_flow03.png";
CSTR MTR::W04::IMG = "\\images\\massbank_flow04.png";
CSTR MTR::W05::IMG = "\\images\\massbank_flow05.png";
CSTR MTR::W06::IMG = "\\images\\massbank_flow06.png";
CSTR MTR::W07::IMG = "\\images\\massbank_flow07.png";
CSTR MTR::W08::IMG = "\\images\\massbank_flow08.png";
CSTR MTR::W09::IMG = "\\images\\massbank_flow09.png";
CSTR MTR::W10::IMG = "\\images\\massbank_flow10.png";
CSTR MTR::W11::IMG = "\\images\\massbank_flow11.png";
CSTR MTR::W12::IMG = "\\images\\massbank_flow12.png";
CSTR MTR::W15::IMG = "\\images\\massbank_flow00.png";

// Wizard Tilte Messages
CSTR MTR::W01::TITLE = "Select spectra to export MassBank records.";
CSTR MTR::W02::TITLE = "Input information for exporting MassBank records.";
CSTR MTR::W03::TITLE = "Select an algorithm and filters to extract peaks froma a spcectrum.\n\
(MassBank does not have profile data but peak list)";

CSTR MTR::W04::TITLE = "Input record Information";
CSTR MTR::W05::TITLE = "Input chemical information.";
CSTR MTR::W06::TITLE = "Input biological sample information.";
CSTR MTR::W07::TITLE = "Input instrument information.";
CSTR MTR::W08::TITLE = "Input spectra information.";
CSTR MTR::W09::TITLE = "Input Chromatography information.";
CSTR MTR::W10::TITLE = "input information of precursor or molecular ion.";
CSTR MTR::W11::TITLE = "Input software and processing information.";
CSTR MTR::W12::TITLE = "Input annotations informations";
CSTR MTR::W15::TITLE =	"Select spectra to export MassBank records.";

// Label & Inform
CSTR LBL::W01::ALLSAMPLE = "Sample Raw Data";
CSTR LBL::W01::DISPLAYED = "Displayed Data";
CSTR LBL::W01::SAMPLELIST = "Target Sample(s)";
CSTR LBL::W01::SPECLIST = "Target Spectra";
CSTR LBL::W01::AVERAGE = "If you want to create averaged spectrum records, input averaging information.";
CSTR LBL::W01::PROCCED = "Data Processed";

CSTR LBL::W02::ACCETION =
	"Set parameters to assign accessions to each recored automatically.\nThe general format of a MassBank accession value is ""[Prefix][Number]"".";

CSTR LBL::W08::LISTINFO = "Additional Information";
CSTR LBL::W09::LISTINFO = "Chromatography information";
CSTR LBL::W10::LISTINFO = "Precursor / Molecular Ion Information";

// Default Values
CSTR DFT::MGW::TITLE = "%Name%";
CSTR DFT::MGW::FILENAME = "[Raw Data] %File Name%";

// Settings Parameters Key
CSTR PLG::KEY::SpecPeakFilter = "SPEC_PEAK_FILTER";
CSTR PLG::KEY::SpecPeakPicking = "SPEC_PEAK_PICKING";
