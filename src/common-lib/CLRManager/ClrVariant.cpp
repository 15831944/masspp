/**
 * @file ClrVariant.cpp
 * @brief implements of ClrVariant class
 *
 * @author S.Tanaka
 * @date 2006.08.25
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ClrVariant.h"

#include "SampleSetWrapper.h"
#include "SampleWrapper.h"
#include "SpectrumWrapper.h"
#include "DataSetWrapper.h"
#include "DataGroupNodeWrapper.h"
#include "ChromatogramWrapper.h"
#include "XYDataWrapper.h"
#include "PeaksWrapper.h"
#include "Peaks2DWrapper.h"
#include "DrawingXYDataWrapper.h"
#include "ProgressWrapper.h"
#include "ScriptWrapper.h"
#include "GraphicsWrapper.h"
#include "FontWrapper.h"
#include "MouseEventWrapper.h"
#include "KeyEventWrapper.h"
#include "SizeEventWrapper.h"
#include "MoveEventWrapper.h"
#include "DataMapInfoWrapper.h"
#include "SettingParameterValuesWrapper.h"
#include "SampleReaderWrapper.h"			// @date 2012/08/21 <Add> OKADA

#include "ClrObjectTool.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
ClrVariant::ClrVariant() {
	type = DataType::UNKNOWN;
	obj = nullptr;
}

ClrVariant::ClrVariant( kome::objects::Variant& val ) {
	setValue( val );
}

// destructor
ClrVariant::~ClrVariant() {
	obj = nullptr;
}

// Variant -> ClrVariant
void ClrVariant::setValue( kome::objects::Variant& val ) {
	// convert
	if( val.type == kome::objects::Variant::UNKNOWN ) {	// unknown
		type = DataType::UNKNOWN;
	}
	else if( val.type == kome::objects::Variant::BOOL ) {	// boolean
		type = DataType::BOOL;
		obj = val.prim.boolVal;
	}
	else if( val.type == kome::objects::Variant::INTEGER ) {	// integer
		type = DataType::INTEGER;
		obj = val.prim.intVal;
	}
	else if( val.type == kome::objects::Variant::DOUBLE ) {	// double
		type = DataType::DOUBLE;
		obj = val.prim.dblVal;
	}
	else if( val.type == kome::objects::Variant::STRING ) {	// string
		type = DataType::STRING;
		obj = gcnew System::String( NVL( val.getString(), "" ) );
	}
	else if( val.type == kome::objects::Variant::ARRAY ) {  // array
		type = DataType::ARRAY;
		unsigned int size = val.getArraySize();
		if( size > 0 ) {
			cli::array< ClrVariant^ >^ arr = gcnew cli::array< ClrVariant^ >( size );
			for( unsigned int i = 0; i < size; i++ ) {
				arr[ i ]->setValue( val.getArray()[ i ] );
			}

			obj = arr;
		}
	}
	else if( val.type == kome::objects::Variant::SAMPLE_SET ) {		// sample set
		type = DataType::SAMPLE_SET;
		obj = gcnew SampleSetWrapper( *( (kome::objects::SampleSet*)val.prim.pt ) );
	}
	else if( val.type == kome::objects::Variant::SAMPLE ) { // sample
		type = DataType::SAMPLE;
		obj = gcnew SampleWrapper( *( (kome::objects::Sample*)val.prim.pt ) );
	}
	else if( val.type == kome::objects::Variant::SPECTRUM ) {	// spectrum
		type = DataType::SPECTRUM;
		obj = gcnew SpectrumWrapper( *( (kome::objects::Spectrum*)val.prim.pt ) );
	}
	else if( val.type == kome::objects::Variant::DATA_GROUP_NODE ) {		// spectrum group
		type = DataType::DATA_GROUP_NODE;
		obj = gcnew DataGroupNodeWrapper( *( (kome::objects::DataGroupNode*)val.prim.pt ) );
	}
	else if( val.type == kome::objects::Variant::DATA_SET ) {	// spectra
		type = DataType::DATA_SET;
		obj = gcnew DataSetWrapper( *( (kome::objects::DataSet*)val.prim.pt ) );
	}
	else if( val.type == kome::objects::Variant::CHROMATOGRAM ) {	// chromatogram
		type = DataType::CHROMATOGRAM;
		obj = gcnew ChromatogramWrapper( *( (kome::objects::Chromatogram*)val.prim.pt ) );
	}
	else if( val.type == kome::objects::Variant::XY_DATA ) {	// xy data
		type = DataType::XY_DATA;
		obj = gcnew XYDataWrapper( *( (kome::core::XYData*)val.prim.pt ) );
	}
	else if( val.type == kome::objects::Variant::PEAKS ) {	// peaks
		type = DataType::PEAKS;
		obj = gcnew PeaksWrapper( *( (kome::objects::Peaks*)val.prim.pt ) );
	}
	else if( val.type == kome::objects::Variant::PEAKS_2D ) {		// 2D peaks
		type = DataType::PEAKS;
		obj = gcnew Peaks2DWrapper( *( (kome::objects::Peaks2D*)val.prim.pt ) );
	}
	else if( val.type == kome::objects::Variant::DRAWING_DATA ) {   // drawing data
		type = DataType::DRAWING_DATA;
		obj = gcnew DrawingXYDataWrapper( *( (kome::img::DrawingXYData*)val.prim.pt ) );
	}
	else if( val.type == kome::objects::Variant::PROGRESS ) {	// progress
		type = DataType::PROGRESS;
		obj = gcnew ProgressWrapper( *( (kome::core::Progress*)val.prim.pt ) );
	}
	else if( val.type == kome::objects::Variant::SCRIPT ) {    // script
		type = DataType::SCRIPT;
// >>>>>>	@Date:2013/07/24	<Modify>	A.Ozaki
//
// Moved the ScriptClass to DataObjects library from CoreToolkits library.
//
		obj = gcnew ScriptWrapper( *( (kome::objects::Script*)val.prim.pt ) );
//
// <<<<<<	@Date:2013/07/24	<Modify>	A.Ozaki
	}
	else if( val.type == kome::objects::Variant::GRAPHICS ) {	// graphics
		type = DataType::GRAPHICS;
		obj = gcnew GraphicsWrapper( *( (kome::img::Graphics*)val.prim.pt ) );
	}
	else if( val.type == kome::objects::Variant::FONT ) {	// font
		type = DataType::FONT;
		obj = gcnew FontWrapper( *( (kome::img::Font*)val.prim.pt ) );
	}
	else if( val.type == kome::objects::Variant::MOUSE_EVT ) { // mouse event
		type = DataType::MOUSE_EVT;
		obj = gcnew MouseEventWrapper( *( (kome::evt::MouseEvent*)val.prim.pt ) );
	}
	else if( val.type == kome::objects::Variant::KEY_EVT ) { // key event
		type = DataType::KEY_EVT;
		obj = gcnew KeyEventWrapper( *( (kome::evt::KeyEvent*)val.prim.pt ) );
	}
	else if( val.type == kome::objects::Variant::SIZE_EVT ) {		// size event
		type = DataType::SIZE_EVT;
		obj = gcnew SizeEventWrapper( *( (kome::evt::SizeEvent*)val.prim.pt ) );
	}
	else if( val.type == kome::objects::Variant::MOVE_EVT ) {		// move event
		type = DataType::MOVE_EVT;
		obj = gcnew MoveEventWrapper( *( (kome::evt::MoveEvent*)val.prim.pt ) );
	}
	else if( val.type == kome::objects::Variant::DATA_MAP ) {		// data map
		type = DataType::DATA_MAP;
		obj = gcnew DataMapInfoWrapper( *( (kome::objects::DataMapInfo*)val.prim.pt ) );
	}
	else if( val.type == kome::objects::Variant::SETTING_VALUES ) {	// setting values
		type = DataType::SETTING_VALUES;
		obj = gcnew SettingParameterValuesWrapper( *( (kome::objects::SettingParameterValues*)val.prim.pt ) );
	}
	else if( val.type == kome::objects::Variant::FILE_READER ) {	// setting values
		type = DataType::FILE_READER;
		obj = gcnew SampleReaderWrapper( *( (kome::objects::SampleReader*)val.prim.pt ) );
	}
	else {
		if( val.type != kome::objects::Variant::OTHER ) {
			LOG_WARN( FMT( "Conversion to this type is not implemented." ) );
		}

		type = DataType::OTHER;
		obj = (long long)( val.prim.pt );
	}
}

// ClrVariant -> Variant
void ClrVariant::transmit( kome::objects::Variant& variant ) {
	// check object
	if( obj == nullptr ) {
		variant.prim.pt = NULL;
	}

	// convert
	if( type == DataType::UNKNOWN ) {	// unknown
		variant.type = kome::objects::Variant::UNKNOWN;
	}
	else if( type == DataType::BOOL ) {	// boolean
		variant.type = kome::objects::Variant::BOOL;
		if( obj == nullptr ) {
			variant.prim.boolVal = bool();
		}
		else {
			variant.prim.boolVal = (System::Boolean)obj;
		}
	}
	else if (type == DataType::INTEGER ) {	// integer
		variant.type = kome::objects::Variant::INTEGER;
		if( obj == nullptr ) {
			variant.prim.intVal = int();
		}
		else{
			variant.prim.intVal = (System::Int32)obj;
		}
	}
	else if( type == DataType::DOUBLE ) {	// double
		variant.type = kome::objects::Variant::DOUBLE;
		if( obj == nullptr ) {
			variant.prim.dblVal = double();
		}
		else {
			variant.prim.dblVal = (System::Double)obj;
		}
	}
	else if( type == DataType::STRING ) {	// string
		variant.setString(
			ClrObjectTool::convertString(
				(System::String^)obj
			).c_str()
		);
	}
	else if( type == DataType::ARRAY ) {	// array;
		variant.type = kome::objects::Variant::ARRAY;
		if( obj != nullptr ) {
			cli::array< ClrVariant^ >^ arr = (cli::array< ClrVariant^ >^)obj;
			unsigned int size = arr->Length;

			kome::objects::Variant* vals = variant.createArray( size );
			for( unsigned int i = 0; i < size; i++ ) {
				arr[ i ]->transmit( vals[ i ] );
			}
		}
	}
	else if( type == DataType::SAMPLE_SET ) {		// sample set
		variant.type = kome::objects::Variant::SAMPLE_SET;
		if( obj != nullptr ) {
			variant.prim.pt = &( (SampleSetWrapper^)obj )->getSampleSet();
		}
	}
	else if( type == DataType::SAMPLE ) {   // sample
		variant.type = kome::objects::Variant::SAMPLE;
		if( obj != nullptr ) {
			variant.prim.pt = &( (SampleWrapper^)obj )->getSample();
		}
	}
	else if( type == DataType::SPECTRUM ) {	// spectrum
		variant.type = kome::objects::Variant::SPECTRUM;
		if( obj != nullptr ) {
			variant.prim.pt = &( (SpectrumWrapper^)obj )->getSpectrum();
		}
	}
	else if( type == DataType::DATA_SET ) {	// spectra
		variant.type = kome::objects::Variant::DATA_SET;
		if( obj != nullptr ) {
			variant.prim.pt = &( (DataSetWrapper^)obj )->getDataSet();
		}
	}
	else if( type == DataType::DATA_GROUP_NODE ) {	// spectrum group
		variant.type = kome::objects::Variant::DATA_GROUP_NODE;
		if( obj != nullptr ) {
			variant.prim.pt = &( (DataGroupNodeWrapper^)obj )->getDataGroupNode();
		}
	}
	else if( type == DataType::CHROMATOGRAM ) {	// chromatogram
		variant.type = kome::objects::Variant::CHROMATOGRAM;
		if( obj != nullptr ) {
			variant.prim.pt = &( (ChromatogramWrapper^)obj )->getChromatogram();
		}
	}
	else if( type == DataType::XY_DATA ) {	// xy data
		variant.type = kome::objects::Variant::XY_DATA;
		if( obj != nullptr ) {
			variant.prim.pt = &( (XYDataWrapper^)obj )->getXYData();
		}
	}
	else if( type == DataType::PEAKS ) {	// peaks
		variant.type = kome::objects::Variant::PEAKS;
		if( obj != nullptr ) {
			variant.prim.pt = &( (PeaksWrapper^)obj )->getPeaks();
		}
	}
	else if( type == DataType::PEAKS_2D ) {  // 2D peaks
		variant.type = kome::objects::Variant::PEAKS_2D;
		if( obj != nullptr ) {
			variant.prim.pt = &( (Peaks2DWrapper^)obj )->getPeaks();
		}
	}
	else if( type == DataType::DRAWING_DATA ) {		// drawing data
		variant.type = kome::objects::Variant::DRAWING_DATA;
		if( obj != nullptr ) {
			variant.prim.pt = &( (DrawingXYDataWrapper^)obj )->getDrawingXYData();
		}
	}
	else if( type == DataType::PROGRESS ) {	// progress
		variant.type = kome::objects::Variant::PROGRESS;
		if( obj != nullptr ) {
			variant.prim.pt = &( (ProgressWrapper^)obj )->getProgress();
		}
	}
	else if( type == DataType::SCRIPT ) {    // script
		variant.type = kome::objects::Variant::SCRIPT;
		if( obj != nullptr ) {
		}
		variant.prim.pt = &( (ScriptWrapper^)obj )->getScript();
	}
	else if( type == DataType::GRAPHICS ) {		// graphics
		variant.type = kome::objects::Variant::GRAPHICS;
		if( obj != nullptr ) {
			variant.prim.pt = &( (GraphicsWrapper^)obj )->getGraphics();
		}
	}
	else if( type == DataType::FONT ) {		// font
		variant.type = kome::objects::Variant::FONT;
		if( obj != nullptr ) {
			variant.prim.pt = &( (FontWrapper^)obj )->getFont();
		}
	}
	else if( type == DataType::MOUSE_EVT ) {		// mouse event
		variant.type = kome::objects::Variant::MOUSE_EVT;
		if( obj != nullptr ) {
			variant.prim.pt = &( (MouseEventWrapper^)obj )->getMouseEvent();
		}
	}
	else if( type == DataType::KEY_EVT ) {		// key event
		variant.type = kome::objects::Variant::KEY_EVT;
		if( obj != nullptr ) {
			variant.prim.pt = &( (KeyEventWrapper^)obj )->getKeyEvent();
		}
	}
	else if( type == DataType::SIZE_EVT ) { // size event
		variant.type = kome::objects::Variant::SIZE_EVT;
		if( obj != nullptr ) {
			variant.prim.pt = &( (SizeEventWrapper^)obj )->getSizeEvent();
		}
	}
	else if( type == DataType::MOVE_EVT ) { // move event
		variant.type = kome::objects::Variant::MOVE_EVT;
		if( obj != nullptr ) {
			variant.prim.pt = &( (MoveEventWrapper^)obj )->getMoveEvent();
		}
	}
	else if( type == DataType::DATA_MAP ) { // data map
		variant.type = kome::objects::Variant::DATA_MAP;
		if( obj != nullptr ) {
			variant.prim.pt = &( (DataMapInfoWrapper^)obj )->getDataMapInfo();
		}
	}
	else if( type == DataType::SETTING_VALUES ) {    // setting values
		variant.type = kome::objects::Variant::SETTING_VALUES;
		if( obj != nullptr ) {
			variant.prim.pt = &( (SettingParameterValuesWrapper^)obj )->getSettingParameterValues();
		}
	}
	else if( type == kome::clr::ClrVariant::DataType::FILE_READER ) {    // file reader
		variant.type = kome::objects::Variant::FILE_READER;
		if( obj != nullptr ) {
			variant.prim.pt = &( (SampleReaderWrapper^)obj )->getSampleReader();
		}
	}
	else {
		variant.type = kome::objects::Variant::OTHER;
		long long val = (System::Int64)obj;
		variant.prim.pt = (void*)val;
	}
}

// >>>>>>	@Date:2013/07/29	<Add>	A.Ozaki
//
// assignment operator
void	ClrVariant::operator = ( ClrVariant% insSrc )
{
	type = insSrc.type;
	obj = insSrc.obj;
}

// copy constructor
ClrVariant::ClrVariant( ClrVariant% insSrc )
{
	type = insSrc.type;
	obj = insSrc.obj;
}
//
// <<<<<<	@Date:2013/07/29	<Add>	A.Ozaki
