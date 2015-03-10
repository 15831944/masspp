/**
 * @file RecordElementSport
 * @brief Recordでつかう配列要素の定義部。
 *  list_of でまとめて初期化されるものはここに入れる。
 *
 * @author M.Fukuda
 * @date 2013.03.12
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "RecordElements.h"
#include "objectsName.h"

#include <boost/assign.hpp>

using namespace kome::massbank::wizard;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// AC$ Mass Spectrum の内容をセットアップする
void RecordElements::onSetUpACSpecSubTags(
	std::vector<std::string>& okArr,
	std::vector<std::string>& ngArr
) {
	okArr = boost::assign::list_of
		(k_AC_Spec_ColEnergy)(k_AC_Spec_ColGas)(k_AC_Spec_Date)
		(k_AC_Spec_DesGasFlow)(k_AC_Spec_DesTemp)(k_AC_Spec_IonEnergy)
		(k_AC_Spec_Laser)(k_AC_Spec_Matrix)(k_AC_Spec_MsAcc)
		(k_AC_Spec_RegGas)(k_AC_Spec_Scanning)(kCanInputSelf);

	// SubTagのListに登録できない
	ngArr = boost::assign::list_of
		(kInstrument)(kInstrumentType)(kMSType)(kIonMode);
}

// AC$ Chromatogram の内容をセットアップする
void RecordElements::onSetUpACChromSubTags(
	std::vector<std::string>& okArr,
	std::vector<std::string>& ngArr
) {
	okArr = boost::assign::list_of
		(k_AC_CH_CapVol)(k_AC_CH_ColName)(k_AC_CH_ColTemp)
		(k_AC_CH_FlowGrad)(k_AC_CH_FlowRate)(k_AC_CH_RetentionTime)
		(k_AC_CH_Solvent)(kCanInputSelf);
}

// MS& ION の内容をセットアップする
void RecordElements::onSetUpMSIonSubTags(
	std::vector<std::string>& okArr,
	std::vector<std::string>& ngArr
) {
	okArr =  boost::assign::list_of
		(k_MS_BasePeak)(k_MS_DerForm)(k_MS_DerMass)
		(k_MS_DerType)(k_MS_PrecMZ)(kCanInputSelf);

	// SubTagのListに登録できない
	ngArr =  boost::assign::list_of(kPrecursorType)(kIonType);
}

// Ion , Precursor Type をセットアップする
void RecordElements::onSetUpMSIonElems(void) 
{
	IonArr_ = boost::assign::list_of(kNone)
		("[M]+")("[M]+*")("[M+H]+")("[2M+H]+")
		("[M+Na]+")("[M-H+Na]+")("[2M+Na]+")
		("[M+2Na-H]+")("[(M+NH3)+H]+")("[M+H-H2O]+")
		("[M+H-C6H10O4]+")("[M+H-C6H10O5]+")
		("[M]-")("[M-H]-")("[M-2H]-")("[M-2H+H2O]-")
		("[M-H+OH]-")("[2M-H]-")("[M+HCOO-]-")
		("[(M+CH3COOH)-H]-")("[2M-H-CO2]-")("[2M-H-C6H10O5]-");
}

// MassSpecturmのエレメントを返す
void RecordElements::getACSpecSubTags(
	std::vector<std::string>& vals,
	std::vector<std::string>& ngVals
){
	vals = subSpecArr_;
	ngVals = subSpecNGArr_;
}

// Chromatogramのエレメントを返す
void RecordElements::getACChromSubTags(
	std::vector<std::string>& vals, 
	std::vector<std::string>& ngVals
){
	vals = subChromArr_;
	ngVals = subChromNGArr_;
}

// Ionのエレメントを返す
void RecordElements::getMSIonSubTags(
	std::vector<std::string>& vals, 
	std::vector<std::string>& ngVals
){
	vals = subIonArr_;
	ngVals = subIonNGArr_;
}

void RecordElements::getMSIonElements(std::vector<std::string>& vals) 
{
	vals = IonArr_;
}
