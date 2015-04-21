/**
 * @file ClrObjectTool.h
 * @brief interfaces of ClrObjectTool class
 *
 * @author S.Tanaka
 * @date 2006.10.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_OBJECT_TOOL_H__
#define __KOME_CLR_OBJECT_TOOL_H__


#include <string>


namespace kome {
	namespace clr {

		ref class PropertiesWrapper;
		ref class NumberRestrictionWrapper;
		ref class ProgressWrapper;
		ref class XYDataWrapper;
		ref class SampleSetWrapper;
		ref class SampleWrapper;
		ref class SpectrumWrapper;
		ref class ChromatogramWrapper;
		ref class DataSetWrapper;
		ref class DataGroupNodeWrapper;
		ref class ChromatogramWrapper;
		ref class PeaksWrapper;
		ref class PeakElementWrapper;
		ref class Peaks2DWrapper;
		ref class GraphicsWrapper;
		ref class MouseEventWrapper;
		ref class KeyEventWrapper;
		ref class SizeEventWrapper;
		ref class DataMapInfoWrapper;
		ref class DataManagerWrapper;
		ref class ClrVariant;
		ref class ClrParameters;
		ref class SettingParameterValuesWrapper;

		/**
		 * @class ClrObjectTool
		 * @brief This class has some functions for CLR object.
		 */
		class CLR_MANAGER_CLASS ClrObjectTool {
		public:
			/**
			 * @fn static std::string convertString( gcroot< System::String^ > s )
			 * @brief converts System::String -> std::string
			 * @param[in] s System::String object
			 * @return std::string object
			 */
			static std::string convertString( gcroot< System::String^ > s );

			/**
			 * @fn static System::String^ convertString( const char* s, const char* dfVal )
			 * @brief converts const char* -> System::String
			 * @param[in] s character string
			 * @param[in] dfVal default value (If s is NULL, this method returns dfVal.)
			 * @return Sysmte::String object (If s is NULL and dfVal is NULL, this method returns nullptr.)
			 */
			static gcroot< System::String^ > convertString( const char* s, const char* dfVal );

		public:
			/**
			 * @fn static gcroot< PropertiesWrapper^ > createPropertiesWrapper( kome::core::Properties* props )
			 * @brief creates PropertiesWrapper object
			 * @param[in] props Properties object to be wrapped
			 * @return PropertiesWrapper object
			 */
			static gcroot< PropertiesWrapper^ > createPropertiesWrapper( kome::core::Properties* props );

			/**
			 * @fn static kome::core::Properties* getProperties( gcroot< PropertiesWrapper^ > props )
			 * @brief gets Properties object from wrapper object
			 * @param[in] props PropertiesWrapper object
			 * @return Properties object
			 */
			static kome::core::Properties* getProperties( gcroot< PropertiesWrapper^ > props );

			/**
			 * @fn static gcroot< NumberRestrictionWrapper^ > createNumberRestrictionWrapper( kome::core::NumberRestriction* restriction )
			 * @brief creates NumberRestrictionWrapper object
			 * @param[in] restriction NumberRestriction object to be wrapped
			 * @return NumberRestrictionWrapper object
			 */
			static gcroot< NumberRestrictionWrapper^ > createNumberRestrictionWrapper( kome::core::NumberRestriction* restriction );

			/**
			 * @fn static kome::core::NumberRestriction* getNumberRestriction( gcroot< NumberRestrictionWrapper^ > restriction )
			 * @brief gets NumberRestriction object from wrapper object
			 * @param[in] restriction NumberRestrictionWrapper object
			 * @return NumberRestriction object
			 */
			static kome::core::NumberRestriction* getNumberRestriction( gcroot< NumberRestrictionWrapper^ > restriction );

			/**
			 * @fn static gcroot< ProgressWrapper^ > createProgressWrapper( kome::core::Progress* progress )
			 * @brief creates ProgressWrapper object
			 * @param[in] progress Progress object to be wrapped
			 * @return ProgressWrapper object
			 */
			static gcroot< ProgressWrapper^ > createProgressWrapper( kome::core::Progress* progress );

			/**
			 * @fn kome::core::Progress* getProgress( gcroot< ProgressWrapper^ > progress )
			 * @brief gets Progress object from wrapper object
			 * @param[in] progress ProgressWrapper object
			 * @return Progress object
			 */
			static kome::core::Progress* getProgress( gcroot< ProgressWrapper^ > progress );

			/**
			 * @fn static gcroot< XYDataWrapper^ > createXYDataWrapper( kome::core::XYData* xyData )
			 * @brief creates XYDataWrapper object
			 * @param[in] xyData XYData object to be wrapped
			 * @return XYDataWrapper object
			 */
			static gcroot< XYDataWrapper^ > createXYDataWrapper( kome::core::XYData* xyData );

			/**
			 * @fn static kome::core::XYData* getXYData( gcroot< XYDataWrapper^ > xyData )
			 * @brief gets XYData object from wrapper object
			 * @param[in] xyData XYDataWrapper object
			 * @return XYData object
			 */
			static kome::core::XYData* getXYData( gcroot< XYDataWrapper^ > xyData );

			/**
			 * @fn static gcroot< SampleSetWrapper^ > createSampleSetWrapper( kome::objects::SampleSet* sampleSet )
			 * @brief creates SampleSetWrapper object
			 * @param[in] sampleSet SampleSet object to be wrapped
			 * @return SampleSetWrapper object
			 */
			static gcroot< SampleSetWrapper^ > createSampleSetWrapper( kome::objects::SampleSet* sampleSet );

			/**
			 * @fn static kome::objects::SampleSet* getSampleSet( gcroot< SampleSetWrapper^ > sampleSet )
			 * @brief gets SampleSet object from wrapper object
			 * @param[in] sampleSet sampleSetWrapper object
			 * @return sampleSet object
			 */
			static kome::objects::SampleSet* getSampleSet( gcroot< SampleSetWrapper^ > sampleSet );

			/**
			 * @fn static gcroot< SampleWrapper^ > createSampleWrapper( kome::objects::Sample* sample )
			 * @brief creates SampleWrapper object
			 * @param[in] sample Sample object to be wrapped
			 * @return SampleWrapper object
			 */
			static gcroot< SampleWrapper^ > createSampleWrapper( kome::objects::Sample* sample );

			/**
			 * @fn static kome::objects::Sample* getSample( gcroot< SampleWrapper^ > sample )
			 * @brief gets Sample object from wrapper object
			 * @param[in] sample sampleWrapper object
			 * @return sample object
			 */
			static kome::objects::Sample* getSample( gcroot< SampleWrapper^ > sample );

			/**
			 * @fn static gcroot< SpectrumWrapper^ > createSpectrumWrapper( kome::objects::Spectrum* spec )
			 * @brief creates SpectrumWrapper object
			 * @param[in] spec spectrum object to be wrapped
			 * @return SpectrumWrapper object
			 */
			static gcroot< SpectrumWrapper^ > createSpectrumWrapper( kome::objects::Spectrum* spec );

			/**
			 * @fn static kome::objects::Spectrum* getSpectrum( gcroot< SpectrumWrapper^ > spec )
			 * @brief gets Spectrum object from wrapper object
			 * @param[in] spec SpectrumWrapper object
			 * @return Spectrum object
			 */
			static kome::objects::Spectrum* getSpectrum( gcroot< SpectrumWrapper^ > spec );

			/**
			 * @fn static gcroot< ChromatogramWrapper^ > createChromatogramWrapper( kome::objects::Chromatogram* chrom )
			 * @brief creates ChromatogramWrapper object
			 * @param[in] chrom chromatogram object to be wrapped
			 * @return ChromatogramWrapper object
			 */
			static gcroot< ChromatogramWrapper^ > createChromatogramWrapper( kome::objects::Chromatogram* chrom );

			/**
			 * @fn static kome::objects::Chromatogram* getChromatogram( gcroot< ChromatogramWrapper^ > chrom )
			 * @brief gets Chromatogram object from wrapper object
			 * @param[in] chrom ChromatogramWrapper object
			 * @return Chromatogram object
			 */
			static kome::objects::Chromatogram* getChromatogram( gcroot< ChromatogramWrapper^ > chrom );

			/**
			 * @fn static gcroot< DataSetWrapper^ > createDataSetWrapper( kome::objects::DataSet* dataSet )
			 * @brief creates DataSetWrapper object
			 * @param[in] dataSet data set object to be wrapped 
			 * @return DataSetWrapper object
			 */
			static gcroot< DataSetWrapper^ > createDataSetWrapper( kome::objects::DataSet* dataSet );

			/**
			 * @fn static kome::objects::DataSet* getDataSet( gcroot< DataSetWrapper^ > spectra )
			 * @brief gets DataSet object from wrapper object
			 * @param[in] spectra DataSetWrapper object
			 * @return DataSet object
			 */
			static kome::objects::DataSet* getDataSet( gcroot< DataSetWrapper^ > spectra );

			/**
			 * @fn static gcroot< DataGroupNodeWrapper^ > createDataGroupNodeWrapper( kome::objects::DataGroupNode* group )
			 * @brief gets DataGroupNodeWrapper object
			 * @param[in] group spectrum group object to be wrapped
			 * @return DataGroupNodeWrapper object
			 */
			static gcroot< DataGroupNodeWrapper^ > createDataGroupNodeWrapper( kome::objects::DataGroupNode* group );

			/** 
			 * @fn static kome::objects::DataGroupNode* getDataGroupNode( gcroot< DataGroupNodeWrapper^ > group )
			 * @brief gets DataGroupNode object from wrapper object
			 * @param[in] group DataGroupNodeWrapper object
			 * @return DataGroupNode object
			 */
			static kome::objects::DataGroupNode* getDataGroupNode( gcroot< DataGroupNodeWrapper^ > group );

			/**
			 * @fn static gcroot< PeaksWrapper^ > createPeaksWrapper( kome::objects::Peaks* peaks )
			 * @brief creates PeaksWrapper class
			 * @param[in] peaks Peaks object to be wrapped
			 * @return PeaksWrapper object
			 */
			static gcroot< PeaksWrapper^ > createPeaksWrapper( kome::objects::Peaks* peaks );

			/** 
			 * @fn static kome::objects::Peaks* getPeaks( gcroot< PeaksWrapper^ > peaks )
			 * @brief gets Peaks object from wrapper object
			 * @param[in] peaks PeaksWrapper object
			 * @return Peaks object
			 */
			static kome::objects::Peaks* getPeaks( gcroot< PeaksWrapper^ > peaks );

			/**
			 * @fn static gcroot< PeaksWrapper^ > createPeakElementWrapper( kome::objects::PeakElement* element )
			 * @brief creates PeakElementWrapper class
			 * @param[in] element PeakElement object to be wrapped
			 * @return PeakElementWrapper object
			 */
			static gcroot< PeakElementWrapper^ > createPeakElementWrapper( kome::objects::PeakElement* element );
			
			/** 
			 * @fn static kome::objects::Peaks* getPeakElement( gcroot< PeakElementWrapper^ > element )
			 * @brief gets PeakElement object from wrapper object
			 * @param[in] element PeakElementWrapper object
			 * @return PeakElement object
			 */
			static kome::objects::PeakElement* getPeakElement( gcroot< PeakElementWrapper^ > element );

			/**
			 * @fn static gcroot< Peaks2DWrapper^ > createPeaks2DWrapper( kome::objects::Peaks2D* peaks )
			 * @brief creates Peaks2DWrapper class
			 * @param[in] peaks Peaks2D object to be wrapped
			 * @return Peaks2DWrapper object
			 */
			static gcroot< Peaks2DWrapper^ > createPeaks2DWrapper( kome::objects::Peaks2D* peaks );

			/** 
			 * @fn static kome::objects::Peaks2D* getPeaks2D( gcroot< Peaks2DWrapper^ > peaks )
			 * @brief gets Peaks2D object from wrapper object
			 * @param[in] peaks Peaks2DWrapper object
			 * @return Peaks2D object
			 */
			static kome::objects::Peaks2D* getPeaks2D( gcroot< Peaks2DWrapper^ > peaks );

			/**
			 * @fn static gcroot< GraphicsWrapper^ > createGraphicsWrapper( kome::img::Graphics* graphics )
			 * @brief creates GraphicsWrapper object
			 * @param[in] graphics Graphics object to be wrapped
			 * @return GraphicsWrapper object
			 */
			static gcroot< GraphicsWrapper^ > createGraphicsWrapper( kome::img::Graphics* graphics );

			/**
			 * @fn static kome::img::Graphics* getGraphics( gcroot< GraphicsWrapper^ > graphics )
			 * @brief gets Graphics object from wrapper object
			 * @param[in] graphics GraphicsWrapper object
			 * @return Graphics object
			 */
			static kome::img::Graphics* getGraphics( gcroot< GraphicsWrapper^ > graphics );

			/**
			 * @fn static gcroot< MouseEventWrapper^ > createMouseEventWrapper( kome::evt::MouseEvent* evt )
			 * @brief creates MouseEventWrapper object
			 * @param[in] evt MouseEvent object to be wrapped
			 * @return MouseEventWrapper object
			 */
			static gcroot< MouseEventWrapper^ > createMouseEventWrapper( kome::evt::MouseEvent* evt );

			/** 
			 * @fn static kome::evt::MouseEvent* getMouseEvent( gcroot< MouseEventWrapper^ > evt )
			 * @brief gets MouseEvent object from wrapper object
			 * @param[in] evt MouseEventWrapper object
			 * @return MouseEvent object
			 */
			static kome::evt::MouseEvent* getMouseEvent( gcroot< MouseEventWrapper^ > evt );

			/**
			 * @fn static gcroot< KeyEventWrapper^ > createKeyEventWrapper( kome::evt::KeyEvent* evt )
			 * @brief creates KeyEventWrapper object
			 * @param[in] evt KeyEvent object to be wrapped
			 * @return KeyEventWrapper object
			 */
			static gcroot< KeyEventWrapper^ > createKeyEventWrapper( kome::evt::KeyEvent* evt );

			/** 
			 * @fn static kome::evt::KeyEvent* getKeyEvent( gcroot< KeyEventWrapper^ > evt )
			 * @brief gets KeyEvent object from wrapper object
			 * @param[in] evt KeyEventWrapper object
			 * @return KeyEvent object
			 */
			static kome::evt::KeyEvent* getKeyEvent( gcroot< KeyEventWrapper^ > evt );

			/**
			 * @fn static gcroot< SizeEventWrapper^ > createSizeEventWrapper( kome::evt::SizeEvent* evt )
			 * @brief creates SizeEventWrapper object
			 * @param[in] evt SizeEvent object to be wrapped
			 * @return SizeEventWrapper object
			 */
			static gcroot< SizeEventWrapper^ > createSizeEventWrapper( kome::evt::SizeEvent* evt );

			/** 
			 * @fn static kome::evt::SizeEvent* getSizeEvent( gcroot< SizeEventWrapper^ > evt )
			 * @brief gets SizeEvent object from wrapper object
			 * @param[in] evt SizeEventWrapper object
			 * @return SizeEvent object
			 */
			static kome::evt::SizeEvent* getSizeEvent( gcroot< SizeEventWrapper^ > evt );

			/**
			 * @fn static gcroot< DataMapInfoWrapper^ > createDataMapInfoWrapper( kome::objects::DataMapInfo* dataMap )
			 * @brief creates DataMapInfoWrapper object
			 * @param[in] dataMap DataMapInfo object to be wrapped
			 * @return DataMapInfoWrapper object
			 */
			static gcroot< DataMapInfoWrapper^ > createDataMapInfoWrapper( kome::objects::DataMapInfo* dataMap );

			/**
			 * @fn static kome::objects::DataMapInfo* getDataMapInfo( gcroot< DataMapInfoWrapper^ > dataMap )
			 * @brief gets DataMapInfo object from wrapper objet
			 * @param[in] dataMap DataMapInfoWrapper object
			 * @return DataMapInfo object
			 */
			static kome::objects::DataMapInfo* getDataMapInfo( gcroot< DataMapInfoWrapper^ > dataMap );

			/**
			 * @fn static gcroot< DataManagerWrapper^ > createDataManagerWrapper( kome::objects::DataManager* dataMgr )
			 * @brief creates DataManagerWrapper object
			 * @param[in] dataMgr DataManager object to be wrapped
			 * @return DataManagerWrapper object
			 */
			static gcroot< DataManagerWrapper^ > createDataManagerWrapper( kome::objects::DataManager* dataMgr );

			/**
			 * @fn static kome::objects::DataManager* getDataManager( gcroot< DataManagerWrapper^ > dataMgr )
			 * @brief gets DataManager object from wrapper objet
			 * @param[in] dataMgr DataManagerWrapper object
			 * @return DataManager object
			 */
			static kome::objects::DataManager* getDataManager( gcroot< DataManagerWrapper^ > dataMgr );

			/** 
			 * @fn static gcroot< SettingParameterValuesWrappern^ > createSettingsWrapper( kome::objects::SettingParameterValues* settings )
			 * @brief creates SettingParameterValuesWrapper object
			 * @param[in] settings SettingPrameterValues object to be wrapped
			 * @return SettingParameterValuesWrapper object
			 */
			static gcroot< SettingParameterValuesWrapper^ > createSettingsWrapper( kome::objects::SettingParameterValues* settings );

			/** 
			 * @fn static kome::objects::SettingParameterValues* getSettings( gcroot< SettingParameterValuesWrapper^ > settings )
			 * @brief gets SettingPrameterValues object from wrapper object
			 * @param[in] settings SettingParameterValues object
			 * @return SettingPrameterValues object
			 */
			static kome::objects::SettingParameterValues* getSettings( gcroot< SettingParameterValuesWrapper^ > settings );

		public:
			/**
			 * @fn static void convertVariantToCLR(
					gcroot< ClrVariant^ > var0,
					kome::objects::Variant* var1
				)
			 * @brief converts Variant -> ClrVariant
			 * @param[out] var0 variant object to be settled value
			 * @param[in] var1 variant object that has value.
			 */
			static void convertVariantToCLR(
				gcroot< ClrVariant^ > var0,
				kome::objects::Variant* var1
			);

			/**
			 * @fn static void convertVariantFromCLR(
						kome::objects::Variant* var0,
						gcroot< ClrVariant^ > var1
				)
			 * @brief converts ClrVarinat -> Variant
			 * @param[out] var0 variant object to be settled value
			 * @param[in] var1 variant object that has value.
			 */
			static void convertVariantFromCLR(
				kome::objects::Variant* var0,
				gcroot< ClrVariant^ > var1
			);

			/**
			 * @fn static void convertParamsToCLR(
					gcroot< ClrParameters^ > params0,
					kome::objects::Parameters* params1
				)
			 * @brief converts Parameters -> ClrParameters
			 * @param[out] params0 parameters object to be settled values
			 * @param[in] params1 parameters object that has values
			 */
			static void convertParamsToCLR(
				gcroot< ClrParameters^ > params0,
				kome::objects::Parameters* params1
			);

			/**
			 * @fn static void convertParamsFromCLR( 
					kome::objects::Parameters* params0,
					gcroot< ClrParameters^ > params1
				)
			 * @brief converts ClrParameters -> Parameters
			 * @param[out] params0 parameters object to be settled values
			 * @param[in] params1 parameters object that has values
			 */
			static void convertParamsFromCLR( 
				kome::objects::Parameters* params0,
				gcroot< ClrParameters^ > params1
			);

		public:
			/**
			 * @fn static bool isa( S obj )
			 * @brief judges whether specified object is instance of T class
			 * @param[in] obj object to be checked
			 * @return If obj is instance of T class, this method returns true.
			 */
			template< class T, class S >
			static bool isa( S obj ) {
				return ( dynamic_cast< T >( obj ) != nullptr );
			}
		};
	}
}

#endif	// __KOME_CLR_OBJECT_TOOL_H__
