/**
 * @file Objects NAMEs
 * @brief オブジェクト名をまとめたもの
 *
 * @author M.Fukuda
 * @date 2013.03.12
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#pragma once

#include <string>

//　読み替えが必要なリスト
// title -> RECORD_TITLE
// InChI Key -> IUPAC

const std::string kMassBank = "MassBank";

const std::string kSubTag = "SubTag";
const std::string kValue  = "Value";

const std::string kSctSmpNo("SelectTypeNo");
const std::string kUseProc("Using processed data");

const std::string kPrefix    = "Prefix";
const std::string kBaseNum   = "Base Num.";
const std::string kAccession = "Accession";
const std::string kFolder    = "Folder";

const std::string kTitle       = "Title";
const std::string kDate        = "Date";
const std::string kAuthors     = "Authors";
const std::string kLicense     = "License";
const std::string kCopyright   = "Copyright";
const std::string kPublication = "Publication";
const std::string kComment     = "Comments";

const std::string k_list_Comment = "Comment";

// CH TAGs
const std::string kName("Name");

const std::string kCompoundClass = "Compound Class";
const std::string kFormula       = "Formula";
const std::string kExactMass     = "Exact Mass";
const std::string kSMILES        = "SMILES";
const std::string kInchKey       = "InChI Key";
const std::string kIupac         = "IUPAC";
const std::string kLink          = "Link";
const std::string kDB            = "DB";

// category (statix box or grop box name)
const std::string kChmStr        = "Chemical Structure";

// SP TAGs
const std::string kScientificName("Scientific Name");
const std::string kDataBase("Database");
const std::string k_list_Sample("Sample");

// AC Tags
const std::string kVender("Vender");
const std::string kInstrument("Instrument");
const std::string kInstrumentType("Instrument Type");
const std::string kInstrumentName("Instrument Name");

const std::string kMSStage("MS Stage");
const std::string kPolarity("Polarity");

const std::string kRecordTitle("RECORD_TITLE");
const std::string kMSType("MS_TYPE");
const std::string kIonMode("ION_MODE");

const std::string kCanInputSelf        = "###";

const std::string k_AC_Spec_ColEnergy  = "COLLISION_ENERGY";
const std::string k_AC_Spec_ColGas     = "COLLISION_GAS";
const std::string k_AC_Spec_Date       = "DATE";
const std::string k_AC_Spec_DesGasFlow = "DESOLVATION_GAS_FLOW";
const std::string k_AC_Spec_DesTemp	   = "DESOLVATION_TEMPERATURE";
const std::string k_AC_Spec_IonEnergy  = "IONIZATION_ENERGY";
const std::string k_AC_Spec_Laser      = "LASER";
const std::string k_AC_Spec_Matrix     = "MATRIX";
const std::string k_AC_Spec_MsAcc      = "MASS_ACCURACY";
const std::string k_AC_Spec_RegGas     = "REAGENT_GAS";
const std::string k_AC_Spec_Scanning   = "SCANNING";

const std::string k_AC_CH_CapVol        = "CAPILLARY_VOLTAGE";
const std::string k_AC_CH_ColName       = "COLUMN_NAME";
const std::string k_AC_CH_ColTemp       = "COLUMN_TEMPERATURE";
const std::string k_AC_CH_FlowGrad      = "FLOW_GRADIENT";
const std::string k_AC_CH_FlowRate      = "FLOW_RATE";
const std::string k_AC_CH_RetentionTime = "RETENTION_TIME";
const std::string k_AC_CH_Solvent       = "SOLVENT";

// Focused Ion
const std::string kIonType       = "ION_TYPE";
const std::string kPrecursorType = "PRECURSOR_TYPE";

const std::string k_MS_BasePeak  = "BASE_PEAK";
const std::string k_MS_DerForm   = "DERIVATIVE_FORM";
const std::string k_MS_DerMass   = "DERIVATIVE_MASS";
const std::string k_MS_DerType   = "DERIVATIVE_TYPE";
const std::string k_MS_PrecMZ    = "PRECURSOR_M/Z";

const std::string kDataProcessing("Data Processing");
const std::string kSoftware("Software");
const std::string kPeakDetection("PeakDetection");
const std::string kWhole("WHOLE");
const std::string kFindPeak("FIND_PEAK");

// PK Tag
const std::string kAnnotation("ANNOTATION");
const std::string kNumPeak("NUM_PEAK");
const std::string kPeak("PEAK");

// Select Elements

// product element
const std::string kUnknown("UnKnown");
const std::string kNaturalProduct("Natural Product");
const std::string kNonNaturalProduct("Non Natural Product");

// MS Element
const std::string kAuto("Auto");
const std::string kNone("None");
const std::string kPositive("Positive");
const std::string kNegative("Negative");

// Default Parameter (Keyword Replace)
const std::string kDefRetenstionTime  = "%RT%";
const std::string kDefCollesionEnergy = "%Collision Energy%";
const std::string kDefBPM = "%BPM%";
const std::string kDefPrecursor = "%Precursor%";
