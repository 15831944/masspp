/**
 * @file ClrObjectPool.h
 * @brief interfaces of CLRObjectPool class
 *
 * @author S.Tanaka
 * @date 2007.04.05
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_CLR_OBJECT_POOL_H__
#define __KOME_CLR_OBJECT_POOL_H__


#include <set>


namespace kome {
	namespace clr {

		class ClrGraphics;
		class ClrProgress;
		class ClrScript;
		class ClrSampleSet;
		class ClrSample;
		class SampleReader;	// @date 2012/08/22 <Add> OKADA
		class ClrSampleReader;	// @date 2012/08/22 <Add> OKADA
		class ClrSpectrum;
		class ClrChromatogram;
		class ClrXYData;
		class ClrXYDataOperation;
		class ClrDataManager;
		class ClrPluginFunction;

		ref class SampleSetWrapper;
		ref class SampleWrapper;
		ref class DataGroupNodeWrapper;
		ref class SampleReaderWrapper;	// @date 2012/08/22 <Add> OKADA

		/**
		 * @class ClrObjectPool
		 * @brief object pool class
		 */
		class ClrObjectPool {
		protected:
			/**		
			 * @fn ClrObjectPool()
			 * @brief constructor
			 */
			ClrObjectPool();

			/**
			 * @fn virtual ~ClrObjectPool()
			 * @brief destructor
			 */
			virtual ~ClrObjectPool();

		protected:
			/** graphics object set */
			std::set< ClrGraphics* > m_graphicsSet;

			/** progress object set */
			std::set< ClrProgress* > m_progressSet;

			/** script object set */
			std::set< ClrScript* > m_scriptSet;

			/** sample set object set */
			std::set< ClrSampleSet* > m_sampleSetSet;

			/** sample object set */
			std::set< ClrSample* > m_sampleSet;

			/** spectrum object set */
			std::set< ClrSpectrum* > m_spectrumSet;

			/** sample reader object set */
			std::set< ClrSampleReader* > m_sampleReaderSet;	// @date 2012/08/22 <Add> OKADA

			/** chromatogram object set */
			std::set< ClrChromatogram* > m_chromatogramSet;

			/** xy data set */
			std::set< ClrXYData* > m_xyDataSet;

			/** xy data operation set */
			std::set< ClrXYDataOperation* > m_operationSet;

			/** data manager set */
			std::set< ClrDataManager* > m_dataMgrSet;

			/** plug-in function set */
			std::set< ClrPluginFunction* > m_functionSet;

			/** spectra set */
			std::set< kome::objects::DataSet* > m_dataSetSet;

		public:
			/**
			 * @fn ClrGraphics& createGraphics()
			 * @brief creates graphics object
			 * @return graphics object
			 */
			ClrGraphics& createGraphics();

			/**
			 * @fn void destroyGraphics( ClrGraphics& graphics )
			 * @brief destroys graphics object
			 * @param[in,out] graphics graphics object to be destroyed
			 */
			void destroyGraphics( ClrGraphics& graphics );
			
			/**
			 * @fn void removeGraphics( ClrGraphics& graphics )
			 * @brief removes graphics object
			 * @param[in] graphics graphics object to be removed
			 */
			void removeGraphics( ClrGraphics& graphics );

			/**
			 * @fn ClrProgress& createProgress()
			 * @brief creates progress object
			 * @return progress object
			 */
			ClrProgress& createProgress();

			/**
			 * @fn void destroyProgress( ClrProgress& progress )
			 * @brief destroys progress object
			 * @param[in,out] progress progress object to be destroyed
			 */
			void destroyProgress( ClrProgress& progress );
			
			/**
			 * @fn void removeProgress( ClrProgress& progress )
			 * @brief removes progress object
			 * @param[in] progress progress object to be removed
			 */
			void removeProgress( ClrProgress& progress );

			/**
			 * @fn ClrScript& createScript()
			 * @brief creates script object
			 * @return script object
			 */
			ClrScript& createScript();

			/**
			 * @fn void destroyScript( ClrScript& script )
			 * @brief destroys script object
			 * @param[in,out] script script object to be destroyed
			 */
			void destroyScript( ClrScript& script );
			
			/**
			 * @fn void removeScript( ClrScript& script )
			 * @brief removes script object
			 * @param[in] script script object to be removed
			 */
			void removeScript( ClrScript& script );

			/**
			 * @fn ClrSampleSet& createSampleSet()
			 * @brief creates sample set object
			 * @return sample set object
			 */
			ClrSampleSet& createSampleSet();

			/**
			 * @fn void destroySampleSet( ClrSampleSet& sampleSet )
			 * @brief destroys sample set object
			 * @param[in,out] sampleSet sample set object to be destroyed
			 */
			void destroySampleSet( ClrSampleSet& sampleSet );

			/**
			 * @fn void removeSampleSet( ClrSampleSet& sampleSet )
			 * @brief removes sample set object
			 * @param[in] sampleSet sample set object to be removed
			 */
			void removeSampleSet( ClrSampleSet& sampleSet );

			/**
			 * @fn ClrSample& createSample( SampleSetWrapper^ sampleSet )
			 * @brief creates sample object
			 * @param[in] sampleSet sample set object that has created sample
			 * @return sample object
			 */
			ClrSample& createSample( SampleSetWrapper^ sampleSet );

			/**
			 * @fn void destroySample( ClrSample& sample )
			 * @brief destroys sample object
			 * @param[in,out] sample sample object to be destroyed
			 */
			void destroySample( ClrSample& sample );

			/**
			 * @fn void removeSample( ClrSample& sample )
			 * @brief removes sample object
			 * @param[in] sample sample object to be removed
			 */
			void removeSample( ClrSample& sample );
								
			/**
			 * @fn ClrSampleReader& createSampleReader()
			 * @brief creates sample Reader  object
			 * @return ClrSampleReader Reader  object
			 */
			ClrSampleReader& createSampleReader();	// @date 2012/08/22 <Add> OKADA

			/**
			 * @fn void destroySampleReader( ClrSampleReader& sampleReader )
			 * @brief destroys sample Reader object
			 * @param[in,out] sampleReader sample Reader set object to be destroyed
			 */
			void destroySampleReader( ClrSampleReader& sampleReader );	// @date 2012/08/22 <Add> OKADA

			/**
			 * @fn void removeSampleReader( ClrSampleReader& sampleReader )
			 * @brief removes sample Reader object
			 * @param[in] sampleReader sample object to be removed
			 */
			void removeSampleReader( ClrSampleReader& sampleReader );	// @date 2012/08/22 <Add> OKADA
			
			/**
			 * @fn ClrSpectrum& createSpectrum( SampleWrapper^ sample, System::String^ name )
			 * @brief creates spectrum object
			 * @param[in] sample sample that has created spectrum
			 * @param[in] name spectrum name
			 * @return spectrum object
			 */
			ClrSpectrum& createSpectrum( SampleWrapper^ sample, System::String^ name );

			/**
			 * @fn void destroySpectrum( ClrSpectrum& spectrum )
			 * @brief destroys spectrum object
			 * @param[in,out] spectrum spectrum object to be destroyed
			 */
			void destroySpectrum( ClrSpectrum& spectrum );

			/**
			 * @fn void removeSpectrum( ClrSpectrum& spectrum )
			 * @brief removes spectrum object
			 * @param[in] spectrum spectrum object to be removed
			 */
			void removeSpectrum( ClrSpectrum& spectrum );

			/**
			 * @fn ClrChromatogram& createChromatogram( SampleWrapper^ sample )
			 * @brief creates chromatogram object
			 * @param[in] sample sample that has created chromatogram
			 * @return chromatogram object
			 */
			ClrChromatogram& createChromatogram( SampleWrapper^ sample );

			/**
			 * @fn void destroyChromatogram( ClrChromatogram& chromatogram )
			 * @brief destroys chromatogram object
			 * @param[in,out] chromatogram chromatogram object to be destroyed
			 */
			void destroyChromatogram( ClrChromatogram& chromatogram );

			/**
			 * @fn void removeChromatogram( ClrChromatogram& chromatogram )
			 * @brief removes chromatogram object
			 * @param[in] chromatogram chromatogram object to be removed
			 */
			void removeChromatogram( ClrChromatogram& chromatogram );

			/**
			 * @fn ClrXYData& createXYData()
			 * @brief creates xy data object
			 * @return xy data object
			 */
			ClrXYData& createXYData();

			/**
			 * @fn void destroyXYData( ClrXYData& xyData )
			 * @brief destroys xy data object
			 * @param[in,out] xyData xy data object to be destroyed
			 */
			void destroyXYData( ClrXYData& xyData );

			/**
			 * @fn void removeXYData( ClrXYData& xyData )
			 * @brief removes xy data object
			 * @param[in] xyData xy data object to be removed
			 */
			void removeXYData( ClrXYData& xyData );

			/**
			 * @fn ClrXYDataOperation& createOperation()
			 * @brief creates xy data operation object
			 * @return xy data oepration object
			 */
			ClrXYDataOperation& createOperation();

			/**
			 * @fn void destroyOperation( ClrXYDataOperation& operation )
			 * @brief destroys xy data operation object
			 * @param[in,out] operation xy data operation object to be destroyed
			 */
			void destroyOperation( ClrXYDataOperation& operation );

			/**
			 * @fn void removeOperation( ClrXYDataOperation& operation )
			 * @brief removes xy data operation object
			 * @param[in] operation xy data operation object to be removed
			 */
			void removeOperation( ClrXYDataOperation& operation );

			/**
			 * @fn ClrDataManager& createDataManager()
			 * @brief creates data manager object
			 * @return data manager object
			 */
			ClrDataManager& createDataManager();

			/**
			 * @fn void destroyDataManager( ClrDataManager& mgr )
			 * @brief destroys data manager object
			 * @param[in,out] mgr data manager object to be destroyed
			 */
			void destroyDataManager( ClrDataManager& mgr );

			/**
			 * @fn void removeDataManager( ClrDataManager& mgr )
			 * @brief removes data manager object
			 * @param[in] mgr data manager object to be removed
			 */
			void removeDataManager( ClrDataManager& mgr );

			/**
			 * @fn ClrPluginFunction& createPluginFunction()
			 * @brief creates plug-in function object
			 * @return plug-in function object
			 */
			ClrPluginFunction& createPluginFunction();

			/**
			 * @fn void destroyPluginFunction( ClrPluginFunction& fun )
			 * @brief destroys plug-in function object
			 * @param[in,out] fun plug-in function object to be destroyed
			 */
			void destroyPluginFunction( ClrPluginFunction& fun );

			/**
			 * @fn void removePluginFunction( ClrPluginFunction& fun )
			 * @brief removes plug-in function object
			 * @param[in] fun plug-in function object to be removed
			 */
			void removePluginFunction( ClrPluginFunction& fun );

		public:
			/**
			 * @fn kome::objects::DataSet& createDataSet( DataGroupNodeWrapper^ group )
			 * @brief creates spectra object
			 * @param[in] group spectrum group
			 * @return spectra object
			 */
			kome::objects::DataSet& createDataSet( DataGroupNodeWrapper^ group );

			/**
			 * @fn void destroyDataSet( kome::objects::DataSet& dataSet )
			 * @brief destroys data set object
			 * @param[in,out] dataSet data set object to be destroyed
			 */
			void destroyDataSet( kome::objects::DataSet& dataSet );

			/**
			 * @fn void removeDataSet( kome::objects::DataSet& dataSet )
			 * @brief removes data set object
			 * @param[in] dataSet data set object to be removed
			 */
			void removeDataSet( kome::objects::DataSet& dataSet );

		public:
			/**
			 * @fn static ClrObjectPool& getInstance()
			 * @brief gets pool object
			 * @return pool object (This is the only object.)
			 */
			static ClrObjectPool& getInstance();
		};
	}
}


#endif		// __KOME_CLR_OBJECT_POOL_H__
