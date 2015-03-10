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

			// �Q�Ɖ]�X�x����������邽��
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

				// Open�����폈���Ȃ� True
				// Close���ŉ������Ă����� False
				bool isopend_;

				bool mistake_;

				int mymode_;

				// Segmented���[�h���g�p(�i�荞��MZ����������)
				double mymoi_;

			public:
				// Getter 
				AgilentSampleSet^ getAgilentSampleSet(){return m_sampleSet;}
				// Setter
				void setMode(const int spmode) {mymode_ = spmode;}
				void setMoI(const double moi) {mymoi_ = moi;}

			protected:
				// �ʏ�ALC-���̕��f�[�^��Ǎ��ޕ��@
				bool setSpecDatas(kome::clr::DataGroupNodeWrapper^ rootGroup);

				// Segment����TIC��������ړI�Ŏg�p������@
				//bool setMixedSpectrums(kome::clr::DataGroupNodeWrapper^ rootGroup);

				// Segment����W�J����
				bool setSegmentedSpectrums(kome::clr::DataGroupNodeWrapper^ rootGroup);

				// MRM��ݒ肷����@
				bool setMRMChroms(kome::clr::DataGroupNodeWrapper^ rootGroup);

				// RowID����擾������@
				AgilentSpectrum^ createSpectrumWithRowId(
					Agilent::MassSpectrometry::DataAnalysis::IMSScanRecord^ pCR,
					const int idx);

				// TIC��RT����Spectrum���擾������@
				AgilentSpectrum^ createSpectrumWithRT(
					const int idx,
					const double tic,
					const double rt);
				
				//�X�y�N�g�����̍쐬
				AgilentSpectrum^ onCreateSpectrum(
					Agilent::MassSpectrometry::DataAnalysis::IBDASpecData^ scan, 
					const int idx);
				
				//MRM�I�u�W�F�N�g�̍쐬
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

				// Spectrum�̃v���p�e�B���Z�b�g����
				void setSpectrumProp(
					kome::clr::PropertiesWrapper^ specProps,
					Agilent::MassSpectrometry::DataAnalysis::IBDASpecData^ scan);

				// �t�@�C���Ǎ��̃|�C���^�s����h�~
				bool resetMyReader();

				// API�̃e�X�g�p
				void checkDoingAPIs();

				// �e�X�g�p�R�[�h�i�L�q�̂�)
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
