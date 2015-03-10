/**
 * @file AgilentSample.cpp
 * @brief implements of AgilentSample class
 *
 * @author S.Tanaka
 * @date 2009.05.27
 * 
 * @modify M.Fukuda@Eisai
 * @modate 2013.12.20
 *
 * Copyright(C) 2006-2013 Eisai Co., Ltd. All rights reserved.
 */
#include "stdafx.h"
#include "AgilentSample.h"

#include "AgilentSampleSet.h"
#include "AgilentSpectrum.h"
#include "AgilentChromatogram.h"

#define SOFTWARE_NAME		 "MassHunter"

using namespace kome::io::agilent;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG

namespace agilentdll = Agilent::MassSpectrometry::DataAnalysis;

//前方宣言
const int setMyMsStage(agilentdll::MSLevel mslv);
kome::clr::SpectrumWrapper::Polarity setMyPolarity(agilentdll::IonPolarity ion);
bool checkMyBPI(agilentdll::IBDASpecData^ scan, double% bpi, double% bpm);
double getMyPrecursor(agilentdll::IBDASpecData^ scan);
// Set Parent and Children
void setPaC(System::Collections::Hashtable% scanTable,AgilentSpectrum^ spec);
agilentdll::IBDAChromFilter^ createChromFilter(const double prc,const double q3);

// constructor
AgilentSample::AgilentSample(
	AgilentSampleSet^ sampleSet,
	System::String^ fpath,
	const int mode,
	const double moi
) : kome::clr::ClrSampleBase(sampleSet) 
	, m_sampleSet(sampleSet)
	, fpath_(fpath)
	, isopend_(false)
	, mymode_(mode)
	, mymoi_(moi)
	, mistake_(false)
{
	m_reader_ = gcnew agilentdll::MassSpecDataReader();
//	m_reader_->OpenDataFile(fpath_);
	setSoftwareName( SOFTWARE_NAME );
	setParallelReadable( false );
}

// destructor
AgilentSample::~AgilentSample() 
{
	isopend_=false;
}

// Agilent Read I/O のプラグインを読込む前に、
// FileがRead状態になっていることを確認する。
// Closeされても、SampleSetとSampleのオブジェクトが残るが、
// ReaderのFileOpenの状態がリセットされてしまうため。
bool AgilentSample::resetMyReader()
{
	if(mistake_) {
		m_reader_->CloseDataFile();
	}
	if(mistake_||!isopend_) {
		try {
			if(m_reader_->OpenDataFile(fpath_)) {
				isopend_ = true;
			} else {
				return false;
			}
		}  catch(System::Exception^ e ) {
			return false;
//			logger->error(e->ToString());
		}
	}
	mistake_ = false;
	return true;
}

// on open sample
bool AgilentSample::onOpenSample(
	kome::clr::DataGroupNodeWrapper^ rootGroup
) {
	// Failed Open Sample File.
	if(!resetMyReader()) {
		return false;
	}
	
	// Sample共通の情報を読込む
	onSetRootProps(rootGroup,-1);
	if(mymode_ == AGMODE::MRM) {
		// MRMの場合
		setMRMChroms(rootGroup);
	//} else if(mymode_ == AGMODE::MIX) {
	//	// PrecursorIonmodeの場合(Segmentを分けた場合)
	//	setMixedSpectrums(rootGroup);
	} else if(mymode_ == AGMODE::SEGMENT) {
	// PrecursorIonmodeの場合(Segmentを分けた場合)
		setSegmentedSpectrums(rootGroup);
	//} else if(mymode_ == AGMODE::ERRORTYPE) {
	//	return false; 
	} else {
	// 通常出力(MS MS2..を合成)
		return setSpecDatas(rootGroup);
	}

	return true;
}

// 汎用フォーマットを読込む際はこちらを使用する
bool AgilentSample::setSpecDatas(
	kome::clr::DataGroupNodeWrapper^ rootGroup
){
	using namespace agilentdll;
	IBDAMSScanFileInformation^ mssfInfo = m_reader_->MSScanFileInformation;

	// each spectra
	System::Collections::Hashtable scanTable;
	int msccnt = mssfInfo->TotalScansPresent;

	for(int idx = 0; idx < msccnt; idx++) {
		IMSScanRecord^ pCR = m_reader_->GetScanRecord(idx);
		AgilentSpectrum^ spec = createSpectrumWithRowId(pCR, idx);
		if(spec == nullptr) {
			return false;
		}
		//親子関係のセット
		setPaC(scanTable, spec);

		spec->setPolarity(setMyPolarity(pCR->IonPolarity));
		spec->setMsStage(setMyMsStage(pCR->MSLevel));

		// add to group
		rootGroup->addSpectrum( spec );
	}
	return true;
}

// RowIDからSpectrumを取得する
AgilentSpectrum^ AgilentSample::createSpectrumWithRowId(
	agilentdll::IMSScanRecord^ pCR,
	const int idx
){
	AgilentSpectrum^ spec = nullptr;

//	ClrLogger^ logger = ClrLogger::getInstance();
	kome::clr::ClrLogger^ logger = kome::clr::ClrLogger::getInstance();

	try {
		agilentdll::IBDASpecData^ scan = m_reader_->GetSpectrum(idx,nullptr,nullptr);
		spec = onCreateSpectrum(scan, idx);
		// set properties
		setSpectrumProp(spec->getProperties(), scan);
	} catch(System::Exception^ e ) {
		logger->error( e->ToString() );
		mistake_ = true;
		return nullptr;
	} 

	//Spectrumの基礎情報を投入
	spec->setRt(pCR->RetentionTime);
	spec->setTotalIntensity(pCR->Tic);
	spec->setBasePeakMass(pCR->BasePeakMZ);
	spec->setMaxIntensity(pCR->BasePeakIntensity);
	if(pCR->MSLevel == agilentdll::MSLevel::MSMS){
		spec->setPrecursor(pCR->MZOfInterest);
	}

	return spec;
}

// Set segment mode spectrum Tree (All & Any segments)
bool AgilentSample::setSegmentedSpectrums(
	kome::clr::DataGroupNodeWrapper^ rootGroup
){
	using namespace agilentdll;
	IBDAMSScanFileInformation^ mssfInfo = m_reader_->MSScanFileInformation;
	
	// Segmentの情報を取得する(MzOfIteresrtにProduct Ionの値が入っている)
	cli::array<IBDAMSScanTypeInformation^ >^ arr = mssfInfo->GetMSScanTypeInformation();
	cli::array<double>^ moints = arr[0]->MzOfInterest;

	int mointslen = moints->Length;
	int rootkey = mointslen;
	// Segment数+ALL分のノードを作成する
	cli::array<kome::clr::DataGroupNodeWrapper^>^ childGroups = 
		gcnew array<kome::clr::DataGroupNodeWrapper^>(rootkey+1);

	// AllSegmentは、セグメント数と一致させる。
	// Segment配列の番兵を兼ねる。
	System::String^ spName = "All Segments";
	childGroups[rootkey] = rootGroup->createChildGroup(spName);
	onSetRootProps(childGroups[rootkey],-1);

	for(int i = 0; i < mointslen; i++) {
		spName = "Segment : " + (i + 1) + " Product Ion [" + moints[i] + "]";
		childGroups[i] = rootGroup->createChildGroup( spName );
		onSetRootProps(childGroups[i],i);
	}

	// each spectra
	System::Collections::Hashtable scanTable;
	int msccnt = mssfInfo->TotalScansPresent;

	for(int idx = 0; idx < msccnt; idx++) {
		IMSScanRecord^ pCR = m_reader_->GetScanRecord(idx);
		int scanId = pCR->ScanID;
		int timeseg = pCR->TimeSegment;
		double moi = pCR->MZOfInterest;
		double mz = pCR->BasePeakMZ;
		int tms = pCR->TimeSegment;
		double rt = pCR->RetentionTime;
		AgilentSpectrum^ spec = createSpectrumWithRowId(pCR, idx);
		//親子関係のセット
		setPaC(scanTable, spec);
		spec->setPolarity(setMyPolarity(pCR->IonPolarity));
		spec->setMsStage(setMyMsStage(pCR->MSLevel));

		// 該当するSegmentのノードにも登録する
		for(int i = 0; i < mointslen; i++) {
			if(moints[i] == moi) {
				AgilentSpectrum^ segspec = createSpectrumWithRowId(pCR, idx);
				segspec->setPolarity(setMyPolarity(pCR->IonPolarity));
				segspec->setMsStage(setMyMsStage(pCR->MSLevel));
				childGroups[i]->addSpectrum(segspec);
				break;
			}
		}
		// add to group
		childGroups[rootkey]->addSpectrum( spec );
	}
	return true;
}

// 対象のRTを元にスペクトラムを作成する
AgilentSpectrum^ AgilentSample::createSpectrumWithRT(
	const int idx,
	const double rt,
	const double tic
){
	agilentdll::IBDASpecData^ scan = m_reader_->GetSpectrum(
		rt,
		agilentdll::MSScanType::All,
		agilentdll::IonPolarity::Mixed,
		agilentdll::IonizationMode::Unspecified,
		nullptr
	);
	AgilentSpectrum^ spec = onCreateSpectrum(scan, idx);
	spec->setRt(rt);
	spec->setTotalIntensity(tic);

	// TIC BPI / BPM
	double bpi = -1.0;
	double bpm = -1.0;
	if(checkMyBPI(scan, bpi, bpm)) {
		spec->setBasePeakMass( bpm );
		spec->setMaxIntensity( bpi );
	}

	double precursor = getMyPrecursor(scan);
	if( precursor >= 0.0 ) {
		spec->setPrecursor( precursor );
	}
	return spec;
}

// Spectrumを作成する
AgilentSpectrum^ AgilentSample::onCreateSpectrum(
	agilentdll::IBDASpecData^ scan,
	const int idx
) {
	System::String^ sname = "Scan " + idx.ToString();
	AgilentSpectrum^ spec = gcnew AgilentSpectrum( this, sname, scan );
	return spec;
}

// MRM由来のクロマトグラムを作成する
bool AgilentSample::setMRMChroms(
	kome::clr::DataGroupNodeWrapper^ rootGroup
){
	using namespace agilentdll;
	using namespace System::Diagnostics;

	IBDAMSScanFileInformation^ mssfInfo = m_reader_->MSScanFileInformation;
	MSScanType msct = mssfInfo->ScanTypes;

	if (msct == MSScanType::MultipleReaction) {
		cli::array<IRange^ >^ mrmtrans = mssfInfo->MRMTransitions;
		cli::array<int> ^ smnums = mssfInfo->ScanMethodNumbers;
		int mrmcnt = mrmtrans->Length;
		for(int cc = 0; cc < mrmcnt; cc++) {
			AgilentChromatogram^ chrom = onCreateMRMObjectRng(smnums[cc], mrmtrans[cc]);
			if (chrom != nullptr) {
				rootGroup->addChromatogram(chrom);
			}
		}
	}
	return true;
}

// MRMのオブジェクトを生成する
AgilentChromatogram^ AgilentSample::onCreateMRMObjectRng(
	const int idx,
	Agilent::MassSpectrometry::DataAnalysis::IRange^ rng
){
	double q1 = rng->Start;
	double q3 = rng->End;
	return onCreateMRMObject(idx, q1,q3);
}

// MRMのオブジェクトを作る
AgilentChromatogram^ AgilentSample::onCreateMRMObject(
	const int idx,
	double q1,
	double q3
){
	agilentdll::IBDAChromFilter^ pCF = createChromFilter(q1, q3);
	cli::array<agilentdll::IBDAChromData^ >^ chroms = m_reader_->GetChromatogram(pCF);
	// プライマリのポインタを持つか否かを確認
	if(!chroms[0]->IsPrimaryMrm) {
		return nullptr;
	}
	System::String^ cname = "Method" + idx;
	cname += " [Q1=" + q1.ToString("F4") + ",Q3=" + q3.ToString("F4") + " ]";
	AgilentChromatogram^ chrom = gcnew AgilentChromatogram(this, cname ,"MRM_chrom", chroms[0]);
	chrom->setQ1(q1);
	chrom->setQ3(q3);
	return chrom;
}

//　クロマトグラムフィルターを作成する(MRMのため)
agilentdll::IBDAChromFilter^ createChromFilter(
	const double q1,
	const double q3
){
	using namespace agilentdll;
	IBDAChromFilter^ pCF = gcnew BDAChromFilter();
	pCF->ChromatogramType = ChromType::MultipleReactionMode;
	pCF->MSScanTypeFilter = MSScanType::MultipleReaction;
	pCF->DoCycleSum = false;
	pCF->SingleChromatogramForAllMasses = false;

	if(q1 > 0.0) {
		cli::array<IRange^>^ moif = {gcnew CenterWidthRange(q1,0)};
		pCF->MzOfInterestFilter = moif;
	}
	if(q3 > 0.0) {
		cli::array<IRange^>^ imrs = {gcnew CenterWidthRange(q3,0)}; 
		pCF->IncludeMassRanges = imrs;
	}
	return pCF;
}

// on close sample
bool AgilentSample::onCloseSample() {
	m_reader_->CloseDataFile();
	isopend_ = false;
	return true;
}

//親子関係をテーブル経由で紐づける
void setPaC(System::Collections::Hashtable% scanTable,AgilentSpectrum^ spec)
{
	scanTable.Add(spec->scanId(), spec );
	int parent = spec->parentScanId();
	if( scanTable.Contains( parent ) ) {
		spec->setParentSpectrum( (AgilentSpectrum^)scanTable[ parent ] );
	}
}

// ------------------------------------------------------ *
//
//    Property Set
//
// ------------------------------------------------------ *

// Sample固有の情報を付加する
void AgilentSample::onSetRootProps(
	kome::clr::DataGroupNodeWrapper^ rootGroup,
	int segmentIdx
){
	// set file properties
	kome::clr::PropertiesWrapper^ props = rootGroup->getProperties();
	setFileInfo( props );
	setGroupInfo(props ,segmentIdx);
	setDeviceInfo( props );
}

// set file information
void AgilentSample::setFileInfo( kome::clr::PropertiesWrapper^ props ) 
{
	// file information object
	agilentdll::IBDAFileInformation^ fileInfo =  m_reader_->FileInformation;

	// properties
	props->setValue( "IRM Status", fileInfo->IRMStatus.ToString() );
	props->setValue( "Measurement Type", fileInfo->MeasurementType.ToString() );
	props->setValue( "Separation Technique", fileInfo->SeparationTechnique.ToString() );
	props->setBoolValue( "MS Data Present", fileInfo->IsMSDataPresent() );
	props->setBoolValue( "Non-MS Data Present", fileInfo->IsNonMSDataPresent() );
	props->setBoolValue( "UV Spectra Present", fileInfo->IsUVSpectralDataPresent() );
}

// set device information
void AgilentSample::setDeviceInfo(
	kome::clr::PropertiesWrapper^ props
) {
	using namespace System::Data;
	// data table
	DataTable^ table = m_reader_->FileInformation->GetDeviceTable(
		agilentdll::StoredDataType::All);

	// rows
	for( int i = 0; i < table->Rows->Count; i++ ) {
		DataRow^ row = table->Rows[i];
		System::String^ rowname =  "(" + i.ToString() + ")";

		for each(DataColumn^ col in table->Columns) {
			System::String^ devname = "Device [" + col->ColumnName + "]" + rowname;
			System::String^ val = row[col]->ToString();
			props->setValue( devname, val );
		}
	}
}

// set group information
void AgilentSample::setGroupInfo( 
	kome::clr::PropertiesWrapper^ props,
	int segmentIdx
) {
	// scan information
	agilentdll::IBDAMSScanFileInformation^ scanInfo
		= m_reader_->FileInformation->MSScanFileInformation;

	// properties
	props->setValue( "Ion Polarity", setMyPolarity(scanInfo->IonPolarity).ToString());

	// Treeの子（SegmentTICを作成するときに使う)
	if(segmentIdx > -1) {
		cli::array<double>^ cvols = scanInfo->CollisionEnergy;
		double coval = (cvols->Length > segmentIdx ) ? cvols[segmentIdx] : cvols[0];
		props->setDoubleValue( "Collision Energy", coval );
		cli::array<double>^ fvols = scanInfo->FragmentorVoltage;
		double frval = (fvols->Length > segmentIdx ) ? fvols[segmentIdx] : fvols[0];
		props->setDoubleValue( "Fragmentor Voltage", frval );
	}

	props->setValue( "Spectral Format", scanInfo->SpectraFormat.ToString() );
	props->setBoolValue( "Is Fixed Cycle Length Data Present", scanInfo->IsFixedCycleLengthDataPresent() );
	props->setBoolValue( "Is Multiple Spectra Per Scan Present", scanInfo->IsMultipleSpectraPerScanPresent() );
	props->setValue( "Ionization Mode", scanInfo->IonModes.ToString() );
	props->setValue( "Device Type", scanInfo->DeviceType.ToString() );

	setInstrument( scanInfo->DeviceType.ToString() );
}

// set Spectrum Proprerty information
void AgilentSample::setSpectrumProp(
	kome::clr::PropertiesWrapper^ specProps,
	Agilent::MassSpectrometry::DataAnalysis::IBDASpecData^ scan
){
	specProps->setDoubleValue( "Collision Energy", scan->CollisionEnergy );
	specProps->setDoubleValue( "Fragmentor Voltage", scan->FragmentorVoltage );
	specProps->setValue( "Ionization Mode", scan->IonizationMode.ToString() );
	specProps->setValue( "Scan Type", scan->MSScanType.ToString() );
	specProps->setIntValue( "Scan ID", scan->ScanId);
	specProps->setValue( "MS Storage Mode", scan->MSStorageMode.ToString() );
	specProps->setIntValue( "Ordinal Number", scan->OrdinalNumber );
	specProps->setDoubleValue( "Sampling Period", scan->SamplingPeriod );
	specProps->setValue( "Spectrum Type", scan->SpectrumType.ToString() );
	specProps->setDoubleValue( "Threshold", scan->Threshold );
}

//Get MSLevel
const int setMyMsStage(agilentdll::MSLevel mslv)
{
	int stage = 0;
	if(mslv == agilentdll::MSLevel::MS) {
		stage = 1;
	}
	else if(mslv == agilentdll::MSLevel::MSMS) {
		stage = 2;
	}
	return stage;
}

//Set Polarity
kome::clr::SpectrumWrapper::Polarity setMyPolarity(agilentdll::IonPolarity ion)
{
	using namespace kome::clr;
	SpectrumWrapper::Polarity polarity = SpectrumWrapper::Polarity::UNKNOWN;
	if(ion == agilentdll::IonPolarity::Positive) {
		polarity = SpectrumWrapper::Polarity::POSITIVE;
	}
	else if(ion == agilentdll::IonPolarity::Negative) {
		polarity = SpectrumWrapper::Polarity::NEGATIVE;
	}
	return polarity;
}

// BPI, BPM をセットする
bool checkMyBPI(
	agilentdll::IBDASpecData^ scan, 
	double% bpi, 
	double% bpm)
{
	// data points
	cli::array< double >^ mzs = scan->XArray;
	cli::array< float >^ ints = scan->YArray;

	int num = scan->TotalDataPoints;
	for( int j = 0; j < num; j++ ) {
		if( ints[ j ] > bpi ) {
			bpi = ints[j];
			bpm  = mzs[j];
		}
	}
	return (bpi >= 0);
}

double getMyPrecursor(
	agilentdll::IBDASpecData^ scan
){
	int precCnt = 0;
	cli::array<double>^ precArr = scan->GetPrecursorIon(precCnt);
	return (precCnt > 0) ? precArr[0] : -1.0;
}

// API Doing Check
void AgilentSample::checkDoingAPIs()
{
}

//// Mixedモードは使わなくなった(2014.02.05)
//// Mixedのデータを出力時に使う（）
//bool AgilentSample::setMixedSpectrums(
//	kome::clr::DataGroupNodeWrapper^ rootGroup
//){
//	// TIC
//	agilentdll::IBDAChromData^ tic = m_reader_->GetTIC();
//
//	int specNum = tic->TotalDataPoints;
//	// logger
//	kome::clr::ClrLogger^ logger = kome::clr::ClrLogger::getInstance();
//	logger->info( specNum.ToString() + " Spectra" );
//	cli::array< double >^ rts = tic->XArray;
//	cli::array< float >^ tics = tic->YArray;
//
//	// each spectra
//	System::Collections::Hashtable scanTable;
//
//	for(int idx = 0; idx < specNum; idx++) {
//		AgilentSpectrum^ spec = createSpectrumWithRT(idx, rts[idx], tics[idx]);
//		//親子関係のセット
//		setPaC(scanTable, spec);
//
//		// add to group
//		rootGroup->addSpectrum( spec );
//	}
//	return true;
//}


// 検証コード。
void AgilentSample::TestCodes()
{
	using namespace agilentdll;
	IBDAFileInformation^ fileInfo = m_reader_->FileInformation;

	// 出力内容のチェック１
	MeasurementType msmtType = fileInfo->MeasurementType;
	SeparationTechnique sptec = fileInfo->SeparationTechnique;
	IRMStatus irmstatu = fileInfo->IRMStatus;

	IBDAMSScanFileInformation^ mssfInfo = m_reader_->MSScanFileInformation;
	int msccnt = mssfInfo->TotalScansPresent;

	for(int i = 0; i < msccnt; i++) {
		IMSScanRecord^ rcc = m_reader_->GetScanRecord(i);
		int scId = rcc->ScanID;
		int tmsg = rcc->TimeSegment;
		double rt = rcc->RetentionTime;
		MSLevel mslv = rcc->MSLevel;
		MSScanType mstp = rcc->MSScanType;
		double tic = rcc->Tic;
		double bpmz = rcc->BasePeakMZ;
		double dpi = rcc->BasePeakIntensity;
		IonizationMode izm = rcc->IonizationMode;
		IonPolarity pol = rcc->IonPolarity;
		double mzofit = rcc->MZOfInterest;
	}
}
