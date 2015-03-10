/**
 * @file AgilentSample.h
 * @brief interfaces of AgilentSample class
 *
 * @author S.Tanaka
 * @date 2009.05.27
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_AGILENT_SAMPLE_H__
#define __KOME_IO_AGILENT_SAMPLE_H__


namespace kome {
	namespace io {
		namespace agilent {

			// 参照云々警告を回避するため
			namespace AGMODE {
				typedef enum samplemode{
					MIX = 4,
					MRM = 8,
					SEGMENT = 16,
					DEF = 1,
					UNKNOWN = -1,
					ERRORTYPE = -2,
				};
			}

			ref class AgilentSampleSet;
			ref class AgilentSpectrum;
			ref class AgilentChromatogram;
			/**
			 * @class AgilentSample
			 * @brief sample of Agilent data
			 */
			public ref class AgilentSample : public kome::clr::ClrSampleBase {

			public:
				/**
				 * @brief constructor
				 * @param sampleSet sample set object
				 * @param reader data reader object
				 */
				AgilentSample(
					AgilentSampleSet^ sampleSet,
					System::String^ fpath,
					const int mode,
					const double moi
					);

				/**
				 * @brief destructor
				 */
				virtual ~AgilentSample();

			protected:
				// Class Datas-------------------------------------------------

				// sample set
				AgilentSampleSet^ m_sampleSet;

				// reader object
				Agilent::MassSpectrometry::DataAnalysis::IMsdrDataReader^ m_reader_;

				System::String^ fpath_;

				// Openが正常処理なら True
				// Close等で解放されていたら False
				bool isopend_;

				bool mistake_;

				int mymode_;

				// Segmentedモード時使用(絞り込むMZを持たせる)
				double mymoi_;

			public:
				// Getter 
				AgilentSampleSet^ getAgilentSampleSet(){return m_sampleSet;}
				// Setter
				void setMode(const int spmode) {mymode_ = spmode;}
				void setMoI(const double moi) {mymoi_ = moi;}

			protected:
				// 通常、LC-等の平データを読込む方法
				bool setSpecDatas(kome::clr::DataGroupNodeWrapper^ rootGroup);

				// Segment情報のTICを混ぜる目的で使用する方法
				//bool setMixedSpectrums(kome::clr::DataGroupNodeWrapper^ rootGroup);

				// Segment情報を展開する
				bool setSegmentedSpectrums(kome::clr::DataGroupNodeWrapper^ rootGroup);

				// MRMを設定する方法
				bool setMRMChroms(kome::clr::DataGroupNodeWrapper^ rootGroup);

				// RowIDから取得する方法
				AgilentSpectrum^ createSpectrumWithRowId(
					Agilent::MassSpectrometry::DataAnalysis::IMSScanRecord^ pCR,
					const int idx);

				// TICのRTからSpectrumを取得する方法
				AgilentSpectrum^ createSpectrumWithRT(
					const int idx,
					const double tic,
					const double rt);
				
				//スペクトラムの作成
				AgilentSpectrum^ onCreateSpectrum(
					Agilent::MassSpectrometry::DataAnalysis::IBDASpecData^ scan, 
					const int idx);
				
				//MRMオブジェクトの作成
				AgilentChromatogram^ onCreateMRMObjectRng(
					const int idx, 
					Agilent::MassSpectrometry::DataAnalysis::IRange^ rng);

				AgilentChromatogram^ onCreateMRMObject(
					const int idx,
					double q1,
					double q3
				);

			private:
				// Set myproperties
				void onSetRootProps(
					kome::clr::DataGroupNodeWrapper^ rootGroup,
						int segmentIdx
					);
				void setFileInfo( kome::clr::PropertiesWrapper^ props );
				void setDeviceInfo( kome::clr::PropertiesWrapper^ props );

				void setGroupInfo(
					kome::clr::PropertiesWrapper^ prop,
					int segmentIdx
					);

				// Spectrumのプロパティをセットする
				void setSpectrumProp(
					kome::clr::PropertiesWrapper^ specProps,
					Agilent::MassSpectrometry::DataAnalysis::IBDASpecData^ scan);

				// ファイル読込のポインタ不正を防止
				bool resetMyReader();

				// APIのテスト用
				void checkDoingAPIs();

				// テスト用コード（記述のみ)
				void TestCodes();

			public:
				/**
				 * @param rootGroup root spectrum group
				 * @return If true, it succeeded to open this sample.
				 */
				virtual bool onOpenSample(
					kome::clr::DataGroupNodeWrapper^ rootGroup) override;

				/**
				 * @brief This method is called by closeSample method. (override method)
				 * @return If true, it succeeded to close this sample.
				 */
				virtual bool onCloseSample() override;
			};
		}
	}
}

#endif		// __KOME_IO_AGILENT_SAMPLE_H__
