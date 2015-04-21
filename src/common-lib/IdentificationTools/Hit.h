/**
 * @file Hit.h
 * @brief interfaces of Hit class
 *
 * @author H.Parry
 * @date 2012.01.25
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */

#pragma once

namespace kome {
	namespace ident {

		class SearchResult;
		class Modification;

		/**
		 * @class SpecInfo
		 * @brief spectrum information
		 */
		class IDENT_TOOLS_CLASS SpecInfo {
		public:
			/**
			 * @fn SpecInfo()
			 * @brief constructor
			 */
			SpecInfo();

			/**
			 * @fn ~SpecInfo()
			 * @brief destructor
			 */
			 ~SpecInfo();

		public:
			/** spectrum ID in DB */
			int                                 id;

			/** spectrum ID in sample*/
			int									specId;

			/** sample index */
			int									sampleIndex;

			/** sample name */
			std::string							sampleName;

			/** sample file path */
			std::string							path;

			/** retention time */
			double								retentionTime;

			/** precursor */
			double								precursor;

			/** MS stage */
            int             					stage;

			/** polarity */
			kome::objects::Spectrum::Polarity	polarity;

			/** peak IDs */
			std::vector< int > 					peakIds;

			/** charges */
			std::vector< int >                  charges;

			/** spectrum name */
			std::string							name;

			/** peak list ID */
			int                                 peaks;

		public:
			/**
			 * @fn clearInfo()
			 * @brief clear spec info
			 */
			void clearInfo();
		};

		/**
		 * @typedef ValueType
		 * @brief hit value type
		 */
		typedef enum ValueType {
			TYPE_PROPERTY = 1,
			TYPE_SCORE = 2,
			TYPE_EXPECTATION_VALUE = 3
		};

		/**
		 * @struct ValueInfo
		 * @brief value information
		 */
		struct ValueInfo {
			std::string		name;
			std::string		value;
			ValueType		valueType;
		};

		/**
		 * @class Hit
		 * @brief hit information class
		 */
		class IDENT_TOOLS_CLASS Hit
		{
		public:
			/**
			 * @fn Hit( SearchResult* pSrchResult, Substance* pSubstance )
			 * @brief constructor
			 * @param[in] pSrchResult search result containing the hit
			 * @param[in] pSubstance identified substance in the hit
			 */
			Hit( SearchResult* pSrchResult, Substance* pSubstance );

			/**
			 * @fn ~Hit( void )
			 * @brief destructor
			 */
			~Hit( void );

		private:
			/** item name of the hit rank */
			static const char* ITEM_HIT_RANK;

			/** item name of the number of total proteins */
			static const char* ITEM_NUM_TOT_PROTEINS;

			/** item name of the number of matched ions */
			static const char* ITEM_NUM_MATCHED_IONS;

			/** item name of the number of fragments */
            static const char* ITEM_TOT_NUM_IONS;

			/** item name of the calculated neutral pepmass */
			static const char* ITEM_CALC_NEUTRAL_PEP_MASS;

			/** item name of the difference of precursor ion and substance */
			static const char* ITEM_MASSDIFF;

			/** item name of the number of peptide termini consistent with cleavage by sample enzyme */
			static const char* ITEM_NUM_TOL_TERM;

			/** item name of the number of missed cleavages */
			static const char* ITEM_NUM_MISSED_CLEAVAGES;

			/** item name of the rejected or not */
			static const char* ITEM_IS_REJECTED;

			/** item name of the calculated pI */
			static const char* ITEM_CALC_PI;

			/** item name of the protein expect */
			static const char* ITEM_PROTEIN_EXPECT;

			/** item name of the peptide expect */
			static const char* ITEM_PEPTIDE_EXPECT;

			/** item name of the protein score */
			static const char* ITEM_PROTEIN_SCORE;

			/** item name of the peptide score */
			static const char* ITEM_PEPTIDE_SCORE;

		private:

			/**
			 * @struct AltSubstanceInfo
			 * @brief alt substance information
			 */
			struct AltSubstanceInfo {
				Substance* substance;
				std::string before;
				std::string after;
				int startPos;
				int endPos;
			};

			/** search result containing the hit */
			SearchResult*							m_pSrchResult;

			/** identified substance in the hit */
			Substance*								m_pSubstance;

			/** alternative substances */
			std::vector< AltSubstanceInfo >         m_altSubstances;

			/** array of spectrum info */
			std::vector<SpecInfo*>					m_spectrumInfoList;

			/** array of value information */
			std::vector<ValueInfo>					m_arrValueInfo;

			/** charge */
			int m_charge;

			/** id */
			int m_id;
		
			/**
			 * @struct ModInfo
			 * @brief modification information
			 */
			struct ModInfo {
				Modification* mod;
				int pos;
			};

			/** modifications */
			std::vector< ModInfo > m_mods;
		
		public:
			/**
			 * @fn void setSubstance( Substance* sub )
			 * @brief set the identified substance information
			 * @param[in] sub pointer to a Substance object
			 */
			void setSubstance( Substance* sub );

			/**
			 * @fn Substance* getSubstance( void ) const
			 * @brief get the identified substance information
			 * @return pointer to a Substance object
			 */
			Substance* getSubstance( void ) const;

			/**
			 * @fn void addAltSubstance( Substance* sub, const char* before, const char* after, int start, int end )
			 * @brief adds alternative substance
			 * @param[in] sub alternative substance
			 * @param[in] before before
			 * @param[in] after after
			 * @param[in] start start position
			 * @param[in] end end position
			 */
			void addAltSubstance( Substance* sub, const char* before, const char* after, int start, int end );

			/**
			 * @fn int getNumberOfAltSubstances()
			 * @brief gets the number of alternative substances
			 * @return the number of alternative substances
			 */
			int getNumberOfAltSubstances();

			/**
			 * @fn int getID()
			 * @brief gets the id
			 * @return the id
			 */
			int getID();

			/**
			 * @fn Substance* getAltSubstance( const int index )
			 * @brief gets the alternative substance
			 * @param[in] index alternative substance
			 * @return alt substance
			 */
			Substance* getAltSubstance( const int index );

			/**
			 * @fn int getAltSubstanceStartPos( const int index )
			 * @brief gets the alt substance start position
			 * @param[in] index alternative substance
			 * @return alt substance start position
			 */
			int getAltSubstanceStartPos( const int index );

			/**
			 * @fn int getAltSubstanceEndPos( const int index )
			 * @brief gets the alt substance end position
			 * @param[in] index alternative substance
			 * @return alt substance end position
			 */
			int getAltSubstanceEndPos( const int index );

			/**
			 * @fn const char* getAltSubstanceBefore( const int index )
			 * @brief gets the alt substance before sequence
			 * @param[in] index alternative substance
			 * @return alt subsntace before sequence
			 */
			const char* getAltSubstanceBefore( const int index );

			/**
			 * @fn const char* getAltSubstanceAfter( const int index )
			 * @brief gets the alt substance after sequence
			 * @param[in] index alternative substance
			 * @return alt substance after sequence
			 */

			const char* getAltSubstanceAfter( const int index );

			/**
			 * @fn void addSpectrum( SpecInfo* spectrumInfo )
			 * @brief add a hit spectrum
			 * @param[in] spectrumInfo pointer to a SpecInfo structure
			 */
			void addSpectrum( SpecInfo* spectrumInfo );

			/**
			 * @fn int getNumberOfSpectra( void ) const
			 * @brief get the number of hit spectra
			 * @return number of spectra
			 */
			int getNumberOfSpectra( void ) const;

			/**
			 * @fn SpecInfo* getSpectrum( const int index )
			 * @brief get the specified hit spectrum
			 * @param[in] index spectrum index
			 * @return hit spectrum
			 */
			SpecInfo* getSpectrum( const int index );

			/**
			 * @fn void clearModifications()
			 * @brief clears modifications
			 */
			void clearModifications();

			/**
			 * @fn void addModification( Modification* m, const int pos )
			 * @brief adds modification
			 * @param[in] m modification
			 * @param[in] pos position
			 */
			void addModification( Modification* m, const int pos );

			/**
			 * @fn unsigned int getNumberOfModifications()
			 * @brief gets the number of modificaitons 
			 * @return the number of modificaitons
			 */
			unsigned int getNumberOfModifications();

			/**
			 * @fn Modification* getModification( const unsigned int idx )
			 * @brief gets the modification
			 * @param[in] idx modificaiton index
			 * @return modification
			 */
			Modification* getModification( const unsigned int idx );

			/**
			 * @fn int getModificationPos( const unsigned int idx )
			 * @brief gets the modification position
			 * @param[in] idx modification index
			 * @return modification position
			 */
			int getModificationPos( const unsigned int idx );

			/**
			 * @fn void addValue( const char* propName, const char* propValue, const ValueType propValueType )
			 * @brief add hit property information
			 * @param[in] propName property name
			 * @param[in] propValue property value
			 * @param[in] propValueType property type
			 */
			void addValue( const char* propName, const char* propValue, const ValueType propValueType );

			/**
			 * @fn int getNumberOfValues( void ) const
			 * @brief get the number of hit property values
			 * @return property value count
			 */
			int getNumberOfValues( void ) const;

			/**
			 * @fn std::string getValueName( const int index ) const
			 * @brief get the specified hit property value name
			 * @param[in] index value index
			 * @return value name
			 */
			std::string getValueName( const int index ) const;

			/**
			 * @fn std::string getValue( const int index ) const
			 * @brief get the specified hit property value
			 * @param[in] index value index
			 * @return value
			 */
			std::string getValue( const int index ) const;

			/**
			 * @fn ValueType getValueType( const int index ) const
			 * @brief get the specified hit property value type
			 * @param[in] index value index
			 * @return value type
			 */
			ValueType getValueType( const int index ) const;

			/**
			 * @fn ValueInfo* getValueInfo( const char* name )
			 * @brief search for the specified hit value
			 * @param[in] name value name
			 * @return value info pointer
			 */
			ValueInfo* getValueInfo( const char* name );

			/**
			 * @fn void setCharge( const int charge )
			 * @brief sets the charge
			 * @param[in] charge charge
			 */
			void setCharge( const int charge );

			/**
			 * @fn int getCharge()
			 * @brief gets the charge
			 * @return charge
			 */
			int getCharge();

			/**
			 * @fn void setRank( const int rank )
			 * @brief set the rank
			 * @param[in] rank rank value
			 */
			void setRank( const int rank );

			/**
			 * @fn int getRank( void )
			 * @brief get the rank
			 * @return rank value
			 */
			int getRank( void );

			/**
			 * @fn void setProteinCount( const int proteincount )
			 * @brief set the protein count
			 * @param[in] proteincount protein count
			 */
			void setProteinCount( const int proteincount );

			/**
			 * @fn int getProteinCount( void )
			 * @brief get the protein count
			 * @return protein count
			 */
			int getProteinCount( void );

			/**
			 * @fn void setMatchedIonCount( const int matchedioncount )
			 * @brief set the matched ion count
			 * @param[in] matchedioncount the matched ion count
			 */
			void setMatchedIonCount( const int matchedioncount );

			/**
			 * @fn int getMatchedIonCount( void )
			 * @brief get the matched ion count
			 * @return matched ion count
			 */
			int getMatchedIonCount( void );

			/**
			 * @fn void setIonCount( const int ioncount )
			 * @brief set the ion count
			 * @param[in] ioncount ion count
			 */
			void setIonCount( const int ioncount );

			/**
			 * @fn int getIonCount( void )
			 * @brief get the ion count
			 * @return ion count
			 */
			int getIonCount( void );

			/**
			 * @fn void setCalculatedMR( const double calculatedMR )
			 * @brief set the calculated MR
			 * @param[in] calculatedMR calculated MR
			 */
			void setCalculatedMR( const double calculatedMR );

			/**
			 * @fn double getCalculatedMR( void )
			 * @brief get the calculated MR
			 * @return calculated MR
			 */
			double getCalculatedMR( void );

			/**
			 * @fn void setDelta( const double delta )
			 * @brief set the delta
			 * @param[in] delta delta value
			 */
			void setDelta( const double delta );

			/**
			 * @fn double getDelta( void )
			 * @brief get the delta
			 * @return delta
			 */
			double getDelta( void );

			/**
			 * @fn void setTerminiConsistentCount( const int terminiConsistentCount )
			 * @brief set the termini consistent count
			 * @param[in] terminiConsistentCount termini consistent count
			 */
			void setTerminiConsistentCount( const int terminiConsistentCount );

			/**
			 * @fn void setID ()
			 * @brief set the id
			 * @param[in] id 
			 */
			void setID( const int id );

			/**
			 * @fn int getTerminiConsistentCount( void )
			 * @brief get the termini consistent count
			 * @return termini consistent count
			 */
			int getTerminiConsistentCount( void );

			/**
			 * @fn void setMissedCleavagesCount( const int missedcleavagescount )
			 * @brief set the missed cleavages count
			 * @param[in] missedcleavagescount missed cleavages count
			 */
			void setMissedCleavagesCount( const int missedcleavagescount );

			/**
			 * @fn int getMissedCleavagesCount( void )
			 * @brief get the missed cleavages count
			 * @return missed cleavages count
			 */
			int getMissedCleavagesCount( void );

			/**
			 * @fn void setRejected( bool rejected )
			 * @brief set the rejected value
			 * @param[in] rejected rejected value
			 */
			void setRejected( bool rejected );

			/**
			 * @fn bool getRejected( void )
			 * @brief get the rejected value
			 * @return rejected
			 */
			bool getRejected( void );

			/**
			 * @fn void setPI( const char* pI )
			 * @brief set the pI value
			 * @param[in] pI pI value
			 */
			void setPI( const char* pI );

			/**
			 * @fn std::string getPI( void )
			 * @brief get the pI value
			 * @return pI
			 */
			std::string getPI( void );

			/**
			 * @fn void setProteinExpect( const double proteinExpect )
			 * @brief set the protein expect value
			 * @param[in] proteinExpect expect value
			 */
			void setProteinExpect( const double proteinExpect );

			/**
			 * @fn double getProteinExpect( void )
			 * @brief get the protein expect value
			 * @return protein expect value
			 */
			double getProteinExpect( void );

			/**
			 * @fn void setPeptideExpect( const double peptideExpect )
			 * @brief set the peptide expect value
			 * @param[in] peptideExpect peptide expect value
			 */
			void setPeptideExpect( const double peptideExpect );

			/**
			 * @fn double getPeptideExpect( void )
			 * @brief get the peptide expect value
			 * @return peptide expect value
			 */
			double getPeptideExpect( void );

			/**
			 * @fn void setProteinScore( const double proteinscore )
			 * @brief set the protein score
			 * @param[in] proteinscore protein score
			 */
			void setProteinScore( const double proteinscore );

			/**
			 * @fn double getProteinScore( void )
			 * @brief get the protein score
			 * @return proteinscore
			 */
			double getProteinScore( void );

			/**
			 * @fn void setPeptideScore( const double peptidescore )
			 * @brief set the peptide score
			 * @param[in] peptidescore peptide score
			 */
			void setPeptideScore( const double peptidescore );

			/**
			 * @fn double getPeptideScore( void )
			 * @brief get the peptide score
			 * @return peptide score
			 */
			double getPeptideScore( void );

		
		};
	}
}
