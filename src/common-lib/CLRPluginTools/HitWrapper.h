/**
 * @file HitWrapper.h
 * @brief interfaces of HitWrapper class
 *
 * @author S.Tanaka
 * @date 2012.11.06
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation
 */


#ifndef __KOME_CLR_HIT_WRAPPER_H__
#define __KOME_CLR_HIT_WRAPPER_H__


namespace kome {
	namespace clr { 

		ref class SubstanceWrapper;
		ref class ModificationWrapper;
		ref class EnzymeWrapper;

		/**
		 * @class HitWrapper
		 * @brief hit wrapper class to use on CLR environment
		 */
		public ref class HitWrapper {
		public:
			/**
			 * @fn HitWrapper( kome::ident::Hit& hit )
			 * @briefructor
			 * @param[in] hit hit object
			 */
			HitWrapper( kome::ident::Hit& hit );

			/**
			 * @fn virtual ~HitWrapper()
			 * @brief destructor
			 */
			virtual ~HitWrapper();

		public:
			/** 
			 @struct SpecInfo
			 @brief spectrum informatio
			*/
			ref struct SpecInfo {
                int id;				
				int specId;
				int sampleIndex;
				System::String^ sampleName;
				System::String^ path;
				double retentionTime;
				double precursor;
				int stage;
				SpectrumWrapper::Polarity polarity;
				cli::array< int >^ peakIds;
				cli::array< int >^ charges;
				System::String^ name;
				int peaks;
			};

			/** value type */
			ENUM_CLASS ValueType {
				PROPERTY = 1,
			    SCORE = 2,
				EXPECTATION_VALUE = 3
			};

			/** 
			 @struct ValueInfo
			 @brief value information
			*/
			ref struct ValueInfo {
				System::String^ name;
				System::String^ value;
				ValueType valueType;
			};
			
		protected:
			/** hit object */
			kome::ident::Hit& m_hit;

		public:
			/**
			 * @fn kome::ident::Hit& getHit()
			 * @brief gets hit object
			 * @return hit object
			 */
			kome::ident::Hit& getHit();

		public:
			/**
			 * @fn void setSubstance( SubstanceWrapper^ sub )
			 * @brief set the identified substance information
			 * @param[in] sub pointer to a Substance object
			 */
			void setSubstance( SubstanceWrapper^ sub );

			/**
			 * @fn SubstanceWrapper^ getSubstance()
			 * @brief get the identified substance information
			 * @return pointer to a Substance object
			 */
			SubstanceWrapper^ getSubstance();

			/**
			 * @fn void addAltSubstance( SubstanceWrapper^ sub, System::String^ before, System::String^ after, int start, int end )
			 * @brief adds alternative substance
			 * @param[in] sub alternative substance
			 * @param[in] before before
			 * @param[in] after after
			 * @param[in] start start position
			 * @param[in] end end position
			 */
			void addAltSubstance( SubstanceWrapper^ sub, System::String^ before, System::String^ after, int start, int end );

			/**
			 * @fn int getNumberOfAltSubstances()
			 * @brief gets the number of alternative substances
			 * @return the number of alternative substances
			 */
			int getNumberOfAltSubstances();

			/**
			 * @fn SubstanceWrapper^ getAltSubstance( int index )
			 * @brief gets the alternative substance
			 * @param[in] index alternative substance
			 */
			SubstanceWrapper^ getAltSubstance( int index );

			/**
			 * @fn void addSpectrum( SpecInfo^ spectrumInfo )
			 * @brief add a hit spectrum
			 * @param[in] spectrumInfo pointer to a SpecInfo structure
			 */
			void addSpectrum( SpecInfo^ spectrumInfo );

			/**
			 * @fn int getNumberOfSpectra()
			 * @brief get the number of hit spectra
			 * @return number of spectra
			 */
			int getNumberOfSpectra();

			/**
			 * @fn SpecInfo^ getSpectrum( int index )
			 * @brief get the specified hit spectrum
			 * @param[in] index spectrum index
			 * @return hit spectrum
			 */
			SpecInfo^ getSpectrum( int index );

			/**
			 * @fn void clearModifications()
			 * @brief clears modifications
			 */
			void clearModifications();

			/**
			 * @fn void addModification( ModificationWrapper^ m, int pos )
			 * @brief adds modification
			 * @param[in] m modification
			 * @parma[in] pos position
			 */
			void addModification( ModificationWrapper^ m, int pos );

			/**
			 * @fn unsigned int getNumberOfModifications()
			 * @brief gets the number of modificaitons 
			 * @return the number of modificaitons
			 */
			unsigned int getNumberOfModifications();

			/**
			 * @fn ModificationWrapper^ getModification( unsigned int idx )
			 * @brief gets the modification
			 * @param[in] idx modificaiton index
			 * @return modification
			 */
			ModificationWrapper^ getModification( unsigned int idx );

			/**
			 * @fn int getModificationPos( unsigned int idx )
			 * @brief gets the modification position
			 * @param[in] idx modification index
			 * @return modification position
			 */
			int getModificationPos( unsigned int idx );


			/**
			 * @fn void addValue( System::String^ propName, System::String^ propValue, ValueType propValueType )
			 * @brief add hit property information
			 * @param[in] propName property name
			 * @param[in] propValue property value
			 * @param[in] propValueType property type
			 */
			void addValue( System::String^ propName, System::String^ propValue, ValueType propValueType );

			/**
			 * @fn int getNumberOfValues()
			 * @brief get the number of hit property values
			 * @return property value count
			 */
			int getNumberOfValues();

			/**
			 * @fn System::String^ getValueName( int index )
			 * @brief get the specified hit property value name
			 * @param[in] index value index
			 * @return value name
			 */
			System::String^ getValueName( int index );

			/**
			 * @fn System::String^ getValue( int index )
			 * @brief get the specified hit property value
			 * @param[in] index value index
			 * @return value
			 */
			System::String^ getValue( int index );

			/**
			 * @fn ValueType getValueType( int index )
			 * @brief get the specified hit property value type
			 * @param[in] index value index
			 * @return value type
			 */
			ValueType getValueType( int index );

			/**
			 * @fn ValueInfo^ getValueInfo( System::String^ name )
			 * @brief search for the specified hit value
			 * @param[in] name value name
			 * @return value info pointer
			 */
			ValueInfo^ getValueInfo( System::String^ name );

			/**
			 * @fn void setCharge( int charge )
			 * @brief sets the charge
			 * @param[in] charge charge
			 */
			void setCharge( int charge );

			/**
			 * @fn int getCharge()
			 * @brief gets the charge
			 * @return charge
			 */
			int getCharge();

			/**
			 * @fn void setRank( int rank )
			 * @brief set the rank
			 * @param[in] rank rank value
			 */
			void setRank( int rank );

			/**
			 * @fn int getRank()
			 * @brief get the rank
			 * @return rank value
			 */
			int getRank();

			/**
			 * @fn void setProteinCount( int proteincount )
			 * @brief set the protein count
			 * @param[in] proteincount protein count
			 */
			void setProteinCount( int proteincount );

			/**
			 * @fn int getProteinCount()
			 * @brief get the protein count
			 * @return protein count
			 */
			int getProteinCount();

			/**
			 * @fn void setMatchedIonCount( int matchedioncount )
			 * @brief set the matched ion count
			 * @param[in] matchedioncount the matched ion count
			 */
			void setMatchedIonCount( int matchedioncount );

			/**
			 * @fn int getMatchedIonCount()
			 * @brief get the matched ion count
			 * @return matched ion count
			 */
			int getMatchedIonCount();

			/**
			 * @fn void setIonCount( int ioncount )
			 * @brief set the ion count
			 * @param[in] ioncount ion count
			 */
			void setIonCount( int ioncount );

			/**
			 * @fn int getIonCount()
			 * @brief get the ion count
			 * @return ion count
			 */
			int getIonCount();

			/**
			 * @fn void setCalculatedMR( double calculatedMR )
			 * @brief set the calculated MR
			 * @param[in] calculatedMR calculated MR
			 */
			void setCalculatedMR( double calculatedMR );

			/**
			 * @fn double getCalculatedMR()
			 * @brief get the calculated MR
			 * @return calculated MR
			 */
			double getCalculatedMR();

			/**
			 * @fn void setDelta( double delta )
			 * @brief set the delta
			 * @param[in] delta delta value
			 */
			void setDelta( double delta );

			/**
			 * @fn double getDelta()
			 * @brief get the delta
			 * @return delta
			 */
			double getDelta();

			/**
			 * @fn void setTerminiConsistentCount( int terminiConsistentCount )
			 * @brief set the termini consistent count
			 * @param[in] terminiConsistentCount termini consistent count
			 */
			void setTerminiConsistentCount( int terminiConsistentCount );

			/**
			 * @fn int getTerminiConsistentCount()
			 * @brief get the termini consistent count
			 * @return termini consistent count
			 */
			int getTerminiConsistentCount();

			/**
			 * @fn void setMissedCleavagesCount( int missedcleavagescount )
			 * @brief set the missed cleavages count
			 * @param[in] missedcleavagescount missed cleavages count
			 */
			void setMissedCleavagesCount( int missedcleavagescount );

			/**
			 * @fn int getMissedCleavagesCount()
			 * @brief get the missed cleavages count
			 * @return missed cleavages count
			 */
			int getMissedCleavagesCount();

			/**
			 * @fn void setRejected( bool rejected )
			 * @brief set the rejected value
			 * @param[in] rejected rejected value
			 */
			void setRejected( bool rejected );

			/**
			 * @fn bool getRejected()
			 * @brief get the rejected value
			 * @return rejected
			 */
			bool getRejected();

			/**
			 * @fn void setPI( System::String^ pI )
			 * @brief set the pI value
			 * @param[in] pI pI value
			 */
			void setPI( System::String^ pI );

			/**
			 * @fn System::String^ getPI()
			 * @brief get the pI value
			 * @return pI
			 */
			System::String^ getPI();

			/**
			 * @fn void setProteinExpect( double proteinExpect )
			 * @brief set the protein expect value
			 * @param[in] proteinExpect expect value
			 */
			void setProteinExpect( double proteinExpect );

			/**
			 * @fn double getProteinExpect()
			 * @brief get the protein expect value
			 * @return protein expect value
			 */
			double getProteinExpect();

			/**
			 * @fn void setPeptideExpect( double peptideExpect )
			 * @brief set the peptide expect value
			 * @param[in] peptideExpect peptide expect value
			 */
			void setPeptideExpect( double peptideExpect );

			/**
			 * @fn double getPeptideExpect()
			 * @brief get the peptide expect value
			 * @return peptide expect value
			 */
			double getPeptideExpect();

			/**
			 * @fn void setProteinScore( double proteinscore )
			 * @brief set the protein score
			 * @param[in] proteinscore protein score
			 */
			void setProteinScore( double proteinscore );

			/**
			 * @fn double getProteinScore()
			 * @brief get the protein score
			 * @return proteinscore
			 */
			double getProteinScore();

			/**
			 * @fn void setPeptideScore( double peptidescore )
			 * @brief set the peptide score
			 * @param[in] peptidescore peptide score
			 */
			void setPeptideScore( double peptidescore );

			/**
			 * @fn double getPeptideScore()
			 * @brief get the peptide score
			 * @return peptide score
			 */
			double getPeptideScore();

		public:
			/**
			 * @fn static SpecInfo^ copySpectrum( kome::ident::SpecInfo* spec )
			 * @brief copies spectrum information
			 * @param[in] spec source of spectrum information
			 * @return copied spectrum information
			 */
			static SpecInfo^ copySpectrum( kome::ident::SpecInfo* spec );
			
			/**
			 * @fn static void copySpectrum( SpecInfo^ src, kome::ident::SpecInfo& dst )
			 * @brief copies spectrum information
			 * @param[in] src source of spectrum information
			 * @param[in] dst the structure to be copied
			 */
			static void copySpectrum( SpecInfo^ src, kome::ident::SpecInfo& dst );
		};
	}
}


#endif		// __KOME_CLR_HIT_WRAPPER_H__

