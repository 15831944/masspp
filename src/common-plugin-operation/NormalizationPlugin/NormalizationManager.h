/**
 * @file NormalizationManager.h
 * @brief interfaces of NormalizationPluginManager class
 *
 * @author M.Izumi
 * @date 2012.07.02
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */
 
#define SET_VAL_MZ			 "mz"
#define SET_VAL_MZTOL		 "mz_tolerance"
#define SET_VAL_RT			 "rt"
#define SET_VAL_RTTOL		 "rt_tolerance"
#define SET_VAL_TYPE		 "type"
#define SET_VAL_RTRANGE		 "rt_range"
#define SET_VAL_MZRANGE		 "mz_range"
#define SET_VAL_TOP			 "top"
 
#define INT_STD_FRACT		 "int_std_each_fract"
#define INT_STD_SAMPLE		 "int_std_multi_sample"
#define INTEGRAT_FRACT		 "integrat_each_fact"
#define INTEGRAT_SAMPLE		 "integrat_multi_sample"
#define MAX_INTENT_FRACT	 "distribut_maxIntent_each_fract"
#define MAX_INTENT_SAMPLE	 "distribut_maxIntent_multi_sample"
#define MEDIAN_FRACT		 "distribut_median_each_fract"
#define MEDIAN_SAMPLE		 "distribut_median_multi_sample"
#define PERCENT_FRACT		 "distribut_precentile_each_fract"
#define PERCENT_SAMPLE		 "distribut_precentile_multi_sample"

#define VAL_THRESHOLD		 "THRESHOLDRange"
#define	VAL_RT				 "RTRange"
#define VAL_MZ				 "MZRange"

#define FILTER_SECTION		 "Normaliz Filter"

#ifndef __KOME_NORAMALIZATION_MANAGER_H__
#define __KOME_NORAMALIZATION_MANAGER_H__

#include <vector>
#include <map>
#include <set>

namespace kome {
	namespace normalization {
		
		class NormalizationManager : public kome::objects::DefaultDataManager {
		protected:
			/**
			 * @fn NormalizationManager()
			 * @brief constructor
			 */
			NormalizationManager();

			/**
			 * @fn virtual ~NormalizationManager()
			 * @brief destructor
			 */
			virtual ~NormalizationManager();

		protected:
			
			/** group map */
			std::map< kome::objects::DataGroupNode*, std::vector< kome::objects::Spectrum*> > m_groupMap;

			/** Control group */
			kome::objects::DataGroupNode* m_ctrlGroup;

			/** Treatment group */
			kome::objects::DataGroupNode* m_trmtGroup;
			
			/** peak detection name */
			std::string m_peakDetect;
			
			/** calculated */
			double m_calc;

			double m_startMz;
			double m_endMz;

			/** threshold settings */
			kome::objects::SettingParameterValues m_thresholdSetting;
			
			/** rt range settings */
			kome::objects::SettingParameterValues m_rtRangeSetting;
			
			/** m/z range settings */
			kome::objects::SettingParameterValues m_mzRangeSetting;

			// >>>>>>	@Date:2014/02/19	<Add>	A.Ozaki
			//
			/** normalization partition */
			std::set<double>	m_setPartition;
			//
			// <<<<<<	@Date:2014/02/19	<Add>	A.Ozaki

		protected:
			/**
			 * @fn void addXYDataPoint( kome::core::XYData& src, kome::core::XYData& dst, double calc )
			 * @brief add xyData point
			 * @param[out] src source xy data
			 * @param[out] dst the object to store updated xy data
			 * @param[in] calc calculated value 
			 */
			void addXYDataPoint( kome::core::XYData& src, kome::core::XYData& dst, double calc );

			/**
			 * @fn double getCalcNormalizValue( int index, kome::objects::Spectrum* spec, std::vector< std::pair< kome::objects::Spectrum*, double>>MaxYInfo, bool bEachFract )
			 * @brief The value of the calculated normalization 
			 * @param[in] index Index
			 * @param[in] spec spectrum objects
			 * @param[in] MaxYInfo Maximum peak of each spectrum
			 * @param[in] bEachFract If true, Earch Fraction
			 * @return calculated value 
			 */
			double getCalcNormalizValue( kome::objects::Spectrum* spec, bool bEachFract, double specMax, std::vector<double> maxVal );

			/**
			 * @fn void addOperation( kome::plugin::PluginCall* call, kome::objects::SettingParameterValues* settings )
			 * @brief add xyData Operation
			 * @param[in] call xyData object
			 * @param[in] settings setting parameter values
			 * @param[in] bSample If true, Multi Sample
			 */
			void addOperation( kome::plugin::PluginCall* call, kome::objects::SettingParameterValues* settings, bool bSample );
			
			/**
			 * @fn void removeOperation( kome::objects::DataGroupNode* group )
			 * @brief remove XYDataOperation
			 * @param[in] group group objects
			 */
			void removeOperation( kome::objects::DataGroupNode* group );

			/**
			 * @fn void setGroupMap( kome::objects::DataGroupNode* group )
			 * @brief set group map
			 * @param[in] group group objects
			 */
			void setGroupMap( kome::objects::DataGroupNode* group );
						

			/**
			 * @fn void getMaxY(
					kome::objects::Peaks2D* peak, 
					double minMz,
					double maxMz,
					double minRt,
					double maxRt,
					double rt,
					double& max
				)
			 * @brief get Max Y( 2d )
			 * @param[in] peak peak object to store peaks information
			 * @param[in] minMz The minimum value of m/z
			 * @param[in] maxMz The maximum value of m/z
			 * @param[in] minRt The minimum value of RT
			 * @param[in] maxRt The maximum value of RT
 			 * @param[in] rt RT
			 * @param[out] max max Y 
			 */
			void getMaxY(
				kome::objects::Spectrum* spec,
				kome::objects::Peaks2D* peak, 
				double minMz,
				double maxMz,
				double minRt,
				double maxRt,
				double rt,
				double& max,
				double& specMax
			);
			
			/**
			 * @fn void setIntStdValInfo( kome::objects::SettingParameterValues* settings, bool bEachFract )
			 * @brief The maximum of an internal standard is acquired.
			 * @param[in] settings setting parameter values
			 */
			void setIntStdValInfo( kome::objects::SettingParameterValues* settings, bool bEachFract );
			
			
			/**
			 * @fn void setMaxIntentValInfo( kome::objects::SettingParameterValues* settings, std::vector< double >& maxValues, MethodInfo& info, int index )
			 * @brief The maximum of intensity is acquired.
			 * @param[in] settings setting parameter values
			 * @param[in] index Method Index
			 */
			void setMaxIntentValInfo( kome::objects::SettingParameterValues* settings, bool bEachFract, int index );

			/**
			 * @fn void getIntStdVal(
					double mz, 
					double mzTol,
					double rt, 
					double rtTol,
					bool bEachFract,					
					kome::objects::Spectrum* spec,
					double& specMaxVal,
					std::vector< double >& NormalizeVal
				)
			 * @brief get Internal Standard Max Values
			 * @param[in] mz m/z
			 * @param[in] mzTol m/z torelanse 
			 * @param[in] rt RT
			 * @param[in] rtTol RT torelanse
			 * @param[in] bEachFract If true, Each Fraction
			 * @param[in] spec spectrum object
			 * @param[in] maxSpecVal max spec value
			 * @param[out] NormalizeVal The value used for normalization 
			 */
			void getIntStdVal(
				double mz, 
				double mzTol,
				double rt, 
				double rtTol,
				bool bEachFract,
				kome::objects::Spectrum* spec,
				double& maxSpecVal,
				std::vector< double >& NormalizeVal
			);

			/**
			 * @fn void getIntegratMaxVal( 
					double startMz, 
					double endMz, 
					bool bEachFract,
					kome::objects::Spectrum* spec, 
					double& maxSpecVal, 
					std::vector< double >& NormalizeVal
				)
			 * @brief get Integration max value
			 * @param[in] startMz start m/z
			 * @param[in] endMz end m/z
			 * @param[in] bEachFract If true, Each Fraction
			 * @param[in] spec spectrum object
			 * @param[in] maxSpecVal max spec value
			 * @param[out] NormalizeVal The value used for normalization
			 */
			void getIntegratMaxVal( 
				double startMz, 
				double endMz, 
				bool bEachFract,
				kome::objects::Spectrum* spec, 
				double& maxSpecVal, 
				std::vector< double >& NormalizeVal
				);
			
			/**
			 * @fn void getMedianUseVal( 
					double startMz, 
					double endMz,
					bool bEachFract,
					kome::objects::Spectrum* spec, 
					double& maxSpecVal, 
					std::vector< double >& NormalizeVal
				)
			 * @brief get Median used value
			 * @param[in] startMz start m/z
			 * @param[in] endMz end m/z
			 * @param[in] bEachFract If true, Each Fraction
			 * @param[in] spec spectrum object
			 * @param[in] maxSpecVal max spec value
			 * @param[out] NormalizeVal The value used for normalization
			 */
			void getMedianUseVal( 
				double startMz, 
				double endMz,
				bool bEachFract,
				kome::objects::Spectrum* spec, 
				double& maxSpecVal, 
				std::vector< double >& NormalizeVal
			);
			
			/**
			 * @fn void getPercentileUseVal(
				double startMz, 
					double endMz, 
					int top,
					bool bEachFract,
					kome::objects::Spectrum* spec, 
					double& maxSpecVal,
					std::vector< double >& NormalizeVal
				)
			 * @brief get Percentile used value
			 * @param[in] startMz start m/z
			 * @param[in] endMz end m/z
			 * @param[in] top Top
			 * @param[in] bEachFract If true, Each Fraction
			 * @param[in] spec spectrum object
			 * @param[in] maxSpecVal max spec value
			 * @param[out] NormalizeVal The value used for normalization
			 */
			void getPercentileUseVal(
				double startMz, 
				double endMz, 
				int top,
				bool bEachFract,
				kome::objects::Spectrum* spec, 
				double& maxSpecVal,
				std::vector< double >& NormalizeVal
			);
			
			/**
			 * @fn void getMaxIntentUseVal( 
						double startMz,
						double endMz, 
						bool bEachFract,
						kome::objects::Spectrum* spec, 
						double& maxSpecVal, 
						std::vector< double>& NormalizeVal
					)
			 * @brief get max Integration used Value
			 * @param[in] startMz start m/z
			 * @param[in] endMz end m/z
			 * @param[in] bEachFract If true, Each Fraction
			 * @param[in] spec spectrum object
			 * @param[in] maxSpecVal max spec value
			 * @param[out] NormalizeVal The value used for normalization
			 */
			void getMaxIntentUseVal( 
				double startMz,
				double endMz, 
				bool bEachFract,
				kome::objects::Spectrum* spec, 
				double& maxSpecVal, 
				std::vector< double>& NormalizeVal
			);
			
			
			/**
			 * @fn void calcIntagratVal( kome::objects::Peaks2D* peak, double startMz, double endMz, double& sum )
			 * @brief calculated Intagration value(2d)
			 * @param[in] peak peak object to store peaks information
			 * @param[in] startMz start m/z
			 * @param[in] endMz end m/z
			 * @param[out] sum The value used for normalization
			 */
			void calcIntagratVal( 
				kome::objects::Spectrum* spec,
				kome::objects::Peaks2D* peak,
				double startMz, 
				double endMz, 
				double& sum,
				double& specMaxVal);
			
			
			/**
			 * @fn void calcDistributMaxIntegratVal( kome::objects::Peaks2D* peak, double startMz, double endMz, double& max )
			 * @brief calculated Max Intensity value(2d)
			 * @param[in] peak peak object to store peaks information
			 * @param[in] startMz start m/z
			 * @param[in] endMz end m/z
			 * @param[out] max The value used for normalization
			 */
			void calcMaxIntentVal( 
				kome::objects::Spectrum* spec,
				kome::objects::Peaks2D* peak, 
				double startMz, 
				double endMz, 
				double& max,
				double& specMax);
			
			/**
			 * @fn void calcMedianVal( kome::objects::Peaks2D* peak, double startMz, double endMz, double& max )
			 * @brief calculated Median value(2d)
			 * @param[in] peak peak object to store peaks information
			 * @param[in] startMz start m/z
			 * @param[in] endMz end m/z
			 * @param[out] max The value used for normalization
			 */
			void calcMedianVal( 
				kome::objects::Spectrum* spec,
				kome::objects::Peaks2D* peak,
				double startMz,
				double endMz, 
				double& max,
				double& specMaxVal);
						
			/**
			 * @fn void calcPercentileUseVal( kome::objects::Peaks2D* peak, double startMz, double endMz, int top, double& max )
			 * @brief caluculated percentile value(2d)
			 * @param[in] peak peak object to store peaks information
			 * @param[in] startMz start m/z
			 * @param[in] endMz end m/z
			 * @param[out] max The value used for normalization
			 */
			void calcPercentileUseVal(
				kome::objects::Spectrum* spec,
				kome::objects::Peaks2D* peak,
				double startMz,
				double endMz,
				int top, 
				double& max,
				double& specMaxVal
			);			
						
			/**
			 * @fn void getMaxRt( kome::objects::Peaks2D* peak2d, double& maxRt )
			 * @brief get max RT
			 * @param[in] peak2d peak object to store peaks information
			 * @param[out] maxRt get max RT
			 */
			void getMaxIntensity( kome::objects::Peaks2D* peak2d, double& maxIntensiy );
		
			/**
			 * @fn void executeThreshold( kome::core::Progress& progress )
			 * @brief execute threshold 
			 * @param progress progress
			 */
			void executeThreshold( kome::core::Progress& progress );
			
			/**
			 * @fn void executeRtRange( kome::core::Progress& progress )
			 * @brief execute RT range
			 * @param progress progress
			 */
			void executeRtRange( kome::core::Progress& progress );
			
			/**
			 * @fn void executeMzRange( kome::core::Progress& progress )
			 * @brief execute m/z range
			 * @param progress progress
			 */
			void executeMzRange( kome::core::Progress& progress );
		public:
			// >>>>>>	@Date:2014/02/19	<Add>	A.Ozaki
			//
			/**
			 * @fn void setPartitions( std::set<double>	&partition )
			 * @brief set normalization partitions
			 * @param[in] partition partitions
			 */
			void	setPartitions( std::set<double>	&partition );
			//
			// <<<<<<	@Date:2014/02/19	<Add>	A.Ozaki

			/**
			 * @fn void setPeakDetectName( const char* name )
			 * @brief set peak detect name
			 * @param[in] name peak detection name
			 */
			void setPeakDetectName( const char* name );
			
			/**
			 * @fn std::string getPeakDetectName()
			 * @brief get peak detect name
			 * @return peak detect name
			 */
			std::string getPeakDetectName();

			/**
			 * @fn bool filterThreshold( 
				kome::objects::DataGroupNode* group, 
				std::string type, 
				double low, 
				double height, 
				kome::objects::Peaks2D& peak,
				kome::objects::Peaks2D& dst,
				kome::core::Progress* progress )
			 * @brief check filter Threshold
			 * @param[in] ds DataSet object
			 * @param[in] type type name
			 * @param[in] low low peak
			 * @param[in] height height peak
			 * @return If true, Threshold
			 */
			
			bool filterThreshold( 
				kome::objects::DataGroupNode* group, 
				std::string type, 
				double low, 
				double height, 
				kome::objects::Peaks2D& peak,
				kome::objects::Peaks2D& dst,
				kome::core::Progress* progress );
			
			/**
			 * @fn bool filterRtRange(
				kome::objects::DataGroupNode* group,
				double startRt,
				double endRt,
				kome::objects::Peaks2D& peak,
				kome::objects::Peaks2D& dst,
				kome::core::Progress* progress
			 * @brief check filter Rt Range
			 * @param[in] ds DataSet object
			 * @param[in] startRt start RT
			 * @param[in] endRt end RT
			 * @return If true, RT Range
			 */
			bool filterRtRange(
				kome::objects::DataGroupNode* group,
				double startRt,
				double endRt,
				kome::objects::Peaks2D& peak,
				kome::objects::Peaks2D& dst,
				kome::core::Progress* progress );

			/**
			 * @fn bool filterMzRange(
				kome::objects::DataGroupNode* group,
				double startMz,
				double endMz,
				kome::objects::Peaks2D& peak,
				kome::objects::Peaks2D& dst,
				kome::core::Progress* progress
			 * @brief check filter m/z Range
			 * @param[in] ds DataSet object
			 * @param[in] startMz start m/z
			 * @param[in] endMz end m/z
			 * @return If true, m/z Range
			 */
			bool filterMzRange(
				kome::objects::DataGroupNode* group,
				double startMz,
				double endMz,
				kome::objects::Peaks2D& peak,
				kome::objects::Peaks2D& dst,
				kome::core::Progress* progress );

			/**
			 * @fn void setCtrlGroup( kome::objects::DataGroupNode* group )
			 * @brief set ctrl group
			 * @param[in] group group objects
			 */
			void setCtrlGroup( kome::objects::DataGroupNode* group );

			/**
			 * @fn kome::objects::DataGroupNode* getCtrlGroup()
			 * @brief get control group
			 * @return get control group
			 */
			kome::objects::DataGroupNode* getCtrlGroup();

			/**
			 * @fn void setTrmtGroup( kome::objects::DataGroupNode* group )
			 * @brief set treatment group
			 * @param[in] group group objest
			 */
			void setTrmtGroup( kome::objects::DataGroupNode* group );

			/**
			 * @fn kome::objects::DataGroupNode* getTrmtGroup()
			 * @brief get treatment group
			 * @return get treatment group
			 */
			kome::objects::DataGroupNode* getTrmtGroup();

			/**
			 * @fn void getSpectra( kome::objects::DataSet* ds, std::vector< kome::objects::Spectrum* >&spectra )
			 * @brief get spectra
			 * @param[in] ds DataSet objects
			 * @param[out] spectra spectra array
			 */
			void getSpectra( kome::objects::DataSet* ds, std::vector< kome::objects::Spectrum* >&spectra );

			/**
			 * @fn void executeNormalizFiltering( const char* filterName, kome::core::Progress& progress )
			 * @brief execute normalization Filtering
			 * @param[in] fliterName filter name
			 * @param[out] progress progress display object
			 */
			void executeNormalizFiltering( kome::core::Progress& progress );

			/**
			 * @fn void executeNormalizMethod( const char* methodName, kome::core::Progress& progress )
			 * @brief execute normalization method
			 * @param[in] methodName method name
			 * @param[out] progress progress display object
			 */
			void executeNormalizMethod(	const char* methodName, kome::core::Progress& progress );
		
			/**
			 * @fn bool executePeakDetect(	const char* peakDetect,	kome::objects::SettingParameterValues* settings, kome::core::Progress& progress )
			 * @brief execute peak detection
			 * @param[in] peakDetect peak detection name
			 * @parma[in] settings setting parameter values
			 * @param[out] progress progress display object
			 */
			bool executePeakDetect(	const char* peakDetect,	kome::objects::SettingParameterValues* settings, kome::core::Progress& progress );
					
			/**
			 * @fn oid executeIntStd( 
				kome::objects::DataGroupNode* stdGroup,
				kome::objects::DataGroupNode* trmGroup,
				kome::core::XYData& src, 
				kome::core::XYData& dst, 
				kome::objects::Spectrum* spec, 
				bool bEachFract, 
				double mz, 
				double mzTol,
				double rt, 
				double rtTol )
			 * @brief execute internal standard sample
			 * @param[out] src source xy data
			 * @param[out] dst the object to store updated xy data
			 * @param[in] spec spectrum objects
			 * @param[in] bEachFract If true, Each Fraction
			 * @param[in] mz m/z
			 * @param[in] mzTol m/z torelanse 
			 * @param[in] rt RT
			 * @param[in] rtTol RT torelanse
			 */
			
			void executeIntStd( 
				kome::objects::DataGroupNode* stdGroup,
				kome::objects::DataGroupNode* trmGroup,
				kome::core::XYData& src, 
				kome::core::XYData& dst, 
				kome::objects::Spectrum* spec, 
				bool bEachFract, 
				double mz, 
				double mzTol,
				double rt, 
				double rtTol );
		
			
			/**
			 * @fn void executeIntegrat( kome::core::XYData& src, kome::core::XYData& dst, kome::objects::Spectrum* spec, bool bEachFract, std::string strRange )
			 * @brief execute integration
			 * @param[out] src source xy data
			 * @param[out] dst the object to store updated xy data
			 * @param[in] spec spectrum objects
			 * @param[in] bEachFract If true, Each Fraction
			 * @parma[in] strRange m/z range
			 */
			void executeIntegrat(
				kome::objects::DataGroupNode* stdGroup,
				kome::objects::DataGroupNode* trmGroup,
				kome::core::XYData& src, 
				kome::core::XYData& dst, 
				kome::objects::Spectrum* spec, 
				bool bEachFract, 
				std::string strRange );

			/**
			 * @fn void executeMaxIntensity( kome::core::XYData& src, kome::core::XYData& dst, kome::objects::Spectrum* spec, bool bEachFract, std::string strRange )
			 * @brief execute Distribution Max Integration
			 * @param[out] src source xy data
			 * @param[out] dst the object to store updated xy data
			 * @param[in] spec spectrum objects
			 * @param[in] bEachFract If true, Each Fraction
			 * @parma[in] strRange m/z range
			 */
			void executeMaxIntensity( 
				kome::objects::DataGroupNode* stdGroup,
				kome::objects::DataGroupNode* trmGroup,
				kome::core::XYData& src,
				kome::core::XYData& dst,
				kome::objects::Spectrum* spec,
				bool bEachFract,
				std::string strRange );

			/** 
			 * @fn void executeMedian( kome::core::XYData& src, kome::core::XYData& dst, kome::objects::Spectrum* spec, bool bEarchFract, std::string strRange )
			 * @brief execute Distribution Median
			 * @param[out] src source xy data
			 * @param[out] dst the object to store updated xy data
			 * @param[in] spec spectrum objects
			 * @param[in] bEachFract If true, Each Fraction
			 * @parma[in] strRange m/z range
			 */
			void executeMedian( 
				kome::objects::DataGroupNode* stdGroup,
				kome::objects::DataGroupNode* trmGroup,
				kome::core::XYData& src,
				kome::core::XYData& dst,
				kome::objects::Spectrum* spec,
				bool bEarchFract, 
				std::string strRange );

			/**
			 * @fn void executeDistributePercent(
					kome::core::XYData& src, 
					kome::core::XYData& dst, 
					kome::objects::Spectrum* spec, 
					bool bEachFract, 
					std::string strRange, 
					int top
				)
			 * @brief execute Distribution Percentile
			 * @param[out] src source xy data
			 * @param[out] dst the object to store updated xy data
			 * @param[in] spec spectrum objects
			 * @param[in] bEachFract If true, Each Fraction
			 * @parma[in] strRange m/z range
			 * @param[in] top Top
			 */
			void executeDistributePercent(
				kome::objects::DataGroupNode* stdGroup,
				kome::objects::DataGroupNode* trmGroup,
				kome::core::XYData& src, 
				kome::core::XYData& dst, 
				kome::objects::Spectrum* spec, 
				bool bEachFract, 
				std::string strRange, 
				int top
			);
					
			/**
			 * @fn void clearInfos()
			 * @brief clear infomations
			 */
			void clearInfos();


			/**
			 * @fn void setThresholdSetting( kome::objects::SettingParameterValues settings )
			 * @brief set threshold setting
			 * @param[in] settings setting parameter values object
			 */
			void setThresholdSetting( kome::objects::SettingParameterValues settings );
			
			/**
			 * @fn void setRtRangeSetting( kome::objects::SettingParameterValues settings )
			 * @brief set RT range setting 
			 * @param[in] settings setting parameter values object
			 */
			void setRtRangeSetting( kome::objects::SettingParameterValues settings );
			
			/**
			 * @fn void setMzRangeSetting( kome::objects::SettingParameterValues settings )
			 * @brief set m/z range setting
			 * @param[in] settings setting parameter values object
			 */
			void setMzRangeSetting( kome::objects::SettingParameterValues settings );

		public:
			/**
			 * @fn void setFilterCallFunctions(
				kome::plugin::PluginFunctionItem* fun,
				kome::core::Progress& progress, 
				kome::objects::SettingParameterValues* settings,
				kome::objects::DataGroupNode* group,
				kome::objects::Peaks2D* peaks2d,
				kome::objects::Peaks2D& dst )
			 * @brief
			 * @param
			 * @param
			 * @param
			 * @param
			 */
			void setFilterCallFunctions(
				kome::plugin::PluginFunctionItem* fun,
				kome::core::Progress& progress, 
				kome::objects::SettingParameterValues* settings,
				kome::objects::DataGroupNode* group,
				kome::objects::Peaks2D* peaks2d,
				kome::objects::Peaks2D& dst );

			void ConverPeaks( kome::objects::DataGroupNode* group );
		protected:
			/**
			 * @fn virtual void onCloseSample( kome::objects::Sample* sample, const bool deleting ) 
			 * @brief This method is called when a sample is closed. (override method)
			 * @param[in] sample sample object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void onCloseSample( kome::objects::Sample* sample, const bool deleting ) ;

		public:
			/**
			 * @fn static NormalizationManager& getInstance()
			 * @brief gets normalization manager
			 * @return linear alignment manager class (This is the only object.)
			 */
			static NormalizationManager& getInstance();
		};
	}
}

#endif		// __KOME_NORAMALIZATION_MANAGER_H__
