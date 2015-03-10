/**
 * @file constHelp.cpp
 * @brief Wizard help message definition
 *
 * @author M.Fukuda
 * @date 2013.12.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "constHelp.h"

// Trum 01 ------------------------------------------
const std::string HLP::WIZ::SAMPLELIST = 
	"Select target sample(s).";

const std::string HLP::WIZ::SPECLIST = 
	"Select target spectra.";

//[Active Sample]\nSelecting spectra contained in active sample.\n\
//[Opened Samples]\nSelecting spectra contained in opened samples.\n\
//[Displayed]\nSelecting spectra from displayed spectra.";

const std::string HLP::WIZ::AVERAGELIST = 
	"If you want to create averaged spectrum for MassBank record, set average settings.";

const std::string HLP::WIZ::PROCESSINGLIST =
	"If you want to manipulate spectrum waveforms before peak detection, set data processing settings.";

// Trum 02 ------------------------------------------
const std::string HLP::WIZ::PREFIX = 
"[Prefix] consists of two or three alphabetical capital characters.";

const std::string HLP::WIZ::BASENUM = 
"[Base Num] is assigned to the first selected spectrum and incremental numbers are assigned to later.";

const std::string HLP::WIZ::OUT_FOLDER = 
"Select folder to export MassBank record.Each records are exported as [Accession].txt file.";


// Trum 03 --------------------------------------------------------------
const std::string HLP::WIZ::PEAKDETECT = 
	"Select a peak detector to extract peaks.";

const std::string HLP::WIZ::PEAKFILTER = 
	"Set peak list filters.";

// Trum 04 --------------------------------------------------------------
const std::string HLP::WIZ::TITLE =
"The default record title is \n\
""[Chemical Name]; [Instrument Name]; [MS Type];"".\n\
This character string is always included in the record title. \
This form is for appending other information to the record title.";

const std::string HLP::WIZ::DATE =
"Input date of the creation or the last modification of MassBank record.\
This value is set as DATE item in MassBank record.\n\
E.g., 2011.02.21 (Created 2007.07.07)";

const std::string HLP::WIZ::AUTHOR =
"Input authors and affiliations of MassBank record. \
This value is set as AUTHORS item in MassBank record.";

const std::string HLP::WIZ::LICENSE = 
"Select a Creative Commons License.\
This value is set as LICENSE item in MassBank record.";

const std::string HLP::WIZ::COPYRIGHT =
"Input the copyright. This value is set as COPYRIGHT item in MassBank record.";

const std::string HLP::WIZ::PUBLICATION = 
"Input the reference of the mass spectral data. This value is set as PUBLICATION item in MassBank record.";

const std::string HLP::WIZ::COMMENT =
"These values are set as COMMENT items in MassBank record.";

// Trum 05 --------------------------------------------------------------
const std::string HLP::WIZ::N_PRODUCT = 
"Select an item from Natural Product, Non-Natural Product or Unknown.\n\
If ""Natural Product"" or ""Non-Natural Product"" is selected, \n\
it will be contained in CH$COMPOUND_CLASS in MassBank record.";

const std::string HLP::WIZ::CLASSNAME =
"Input the category of chemical compound.\n\
e.g., Carotenoid; Terpenoid; Lipid";

const std::string HLP::WIZ::CH_NAME =
"Input name of the chemical compound.\n\
These values are set as CH$NAME items.\n\
e.g., D-Tartaric acid";

const std::string HLP::WIZ::EX_LINK =
"Input names and identifiers of external databases for chemical compounds.\n\
These values are set as CH$LINK items in the MassBank record.";

const std::string HLP::WIZ::SEARCH_BUTTON = 
	"Search external databases from [Name] information.";

const std::string HLP::WIZ::MOL_BUTTON = 
	"Import compound information from .mol file.";

const std::string HLP::WIZ::EXDB_BUTTON =
	"Import compound information from external database information.";

const std::string HLP::WIZ::FORMULA =
"Input molecular formula of chemical compound.\n\
This value is set as CH$FORMULA item in MassBank record.\n\
e.g., C9H10ClNO3";

const std::string HLP::WIZ::EXACTMASS =
"Input monoisotopic mass of chemical compound. \n\
This value is set as CH$EXACT_MASS item in MassBank record.\n\
e.g., 430.38108";

const std::string HLP::WIZ::SMILES =
"Input SMILES character string.\n\
This value is set as CH$SMILES item in MassBank record.\n\
e.g., NCC(O)=O";

const std::string HLP::WIZ::INCHIKEY =
"Input IUPAC International Chemical Identifier. (InChI Code).\n\
This value is set as CH$IUPAC item in MassBank record.\n\
e.g., InChI=1S/C2H5NO2/c3-1-2(4)5/h1,3H2,(H,4,5)";

// Trum 06 --------------------------------------------------------------
const std::string HLP::WIZ::SCIENTIFICNAME = 
"Input the scientific name of the biological species.\
This value is set as SP$SCIENTIFIC_NAME item in the MassBank record.\n\
e.g., Mus musculus";

const std::string HLP::WIZ::DATABASE =
"Input the name of external databases for the taxonomy. \
This value is set as a subtag of the SP$LINK item in the MassBank record.\n\
e.g., NCBI-TAXONOMY";

const std::string HLP::WIZ::ACCESSION =
"Input the accession number of the external database for the taxonomy. \
This value is set as an identifier of the SP$LINK item in the MassBank record.\n\
e.g., 10090";

const std::string HLP::WIZ::SAMPLES = 
	"Input tissue or cell used for a sample.\n\
e.g., Liver extracts";

// Trum 07 --------------------------------------------------------------

const std::string HLP::WIZ::VENDOR = 
	"Select or input the vendor of instrument.";

const std::string HLP::WIZ::INSTRUMENTS =
	"Select or input the instrument name. This value is set as AC$INSTRUMENT item in MassBank record.";

const std::string HLP::WIZ::INSTTYPE = 
	"Select or input the instrument type. This value is set as AC$INSTRUMENT_TYPE item in MassBank record.";

// Trum 08 --------------------------------------------------------------
const std::string HLP::WIZ::MSSTAGE = 
	"Select a MS stage. This value is set as AC$MASS_SPECTROMETRY: MS_TYPE item in MassBank records.";

const std::string HLP::WIZ::MS::AUTO =
    "If the spectrum data contains the MS stage information, itfs automatically set.";

const std::string HLP::WIZ::MS::NONE = 
	"MS Stage is not set.";

const std::string HLP::WIZ::MS::BASE = 
	"th generation product ion spectrum.";

const std::string HLP::WIZ::POLARITY = 
	"Select the polarity. This value is set as the AC$MASS_SPECTROMETRY: ION_MODE item in the MassBank record.";

const std::string HLP::WIZ::POL::AUTO =
	"If the spectrum data contains the polarity information, itfs automatically set.";

const std::string HLP::WIZ::POL::NONE = 
	"Polarity is not set.";

const std::string HLP::WIZ::POL::POSI = 
	"Positive ion mode.";

const std::string HLP::WIZ::POL::NEGA = 
	"Negative ion mode.";

const std::string HLP::WIZ::SPECADDINFO = 
	"Input additional methods and conditions of mass spectra.";


// Trum 09 --------------------------------------------------------------
const std::string HLP::WIZ::CHROMADDINFO = 
"Input methods and conditions of chromatographic separation. \
These values are set as AC$CHROMATOGRAPHY items.";

// Trum 10 --------------------------------------------------------------
const std::string HLP::WIZ::PREC_TYPE =
"Select a type of precursor ion.";

const std::string HLP::WIZ::PREC_MOLINFO =
"Input additional precursor or molecular ion information. \
This values are set as MS$FOCUSED_ION items in MassBank record.";

// Trum 11 --------------------------------------------------------------
const std::string HLP::WIZ::PROCESSING = 
	"Input data processing methods before peak detection.";

const std::string HLP::WIZ::SOFTWARE = 
	"Input software information.";

const std::string HLP::WIZ::DETECTOR = 
	"Input peak detector information.";

// Trum 12 --------------------------------------------------------------
const std::string HLP::WIZ::ANNOTATION 
	= "Input chemical annnotatin of peaks with molecular formula.";


// --- For Any List
const std::string HLP::WIZ::PROP_LIST =
"You can set spectrum property values with property names such as ""%RT%"".\n\
This is useful for settings information that differs between spectra.\n\
You can confirm property names with the properties pane.([View] > [Pane] > [Properties])";

