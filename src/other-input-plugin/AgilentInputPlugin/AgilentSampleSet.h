/**
 * @file AgilentSampleSet.h
 * @brief interfaces of AgilentSampleSet class
 *
 * @author S.Tanaka
 * @date 2009.05.27
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_AGILENT_SAMPLE_SET_H__
#define __KOME_IO_AGILENT_SAMPLE_SET_H__


namespace kome {
	namespace io {
		namespace agilent {

			/**
			 * @class AgilentSampleSet
			 * @brief sample set of Agilent data
			 */
			public ref class AgilentSampleSet : public kome::clr::ClrSampleSetBase {
			public:

				//@brief constructor
				AgilentSampleSet();

				//@brief destructor
				virtual ~AgilentSampleSet();

			protected:
				/** path */
				System::String^ fpath_;

			public:
				System::String^ getPath() {return fpath_;}

			  public:
				/**
				 * @brief This method is called by openFile method. (override method)
				 * @param path file path
				 * @return If true, it succeeded to open the file
				 */
				virtual bool onOpenFile( System::String^ path ) override;

				/**
				 * @brief This method is called by closeFile method. (override method)
				 * @param path file path
				 * @return If true, it succeeded to close the file
				 */
				virtual bool onCloseFile() override;

				/**
				 * @brief This method is called by openSample method. (override method)
				 * @param sample sample object to be opened (If NULL, opens all samples.)
				 * @return If true, it succeeded to open the sample.
				 */
				virtual bool onOpenSample( kome::clr::SampleWrapper^ sample ) override;

				/**
				 * @brief This method is called by closeSample method. (override method)
				 * @param sample sample object to be closed (If NULL, closes all samples.)
				 * @return If true, it succeeded to close the sample.
				 */
				virtual bool onCloseSample( kome::clr::SampleWrapper^ sample ) override;
			private:

				// Sampleのモードの値を返す。
				// モードの定義は　Sample内に記述してある
				int checkSampleMode(
					Agilent::MassSpectrometry::DataAnalysis::IMsdrDataReader^ reader,
					cli::array<double >^% moints);

				// Sampleを追加する
				void addAgilentSample(
					System::String^ fname, 
					System::String^ spName, 
					const int mode, 
					const double moi);

			};
		}
	}
}

#endif		// __KOME_IO_AGILENT_SAMPLE_SET_H__
