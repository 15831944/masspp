/**
 * @file Alignment.h
 * @brief interfaces of Alignment class
 *
 * @author S.Tanaka
 * @date 2009.02.13
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_OPERATION_ALIGNMENT_H__
#define __KOME_OPERATION_ALIGNMENT_H__


#include <set>


namespace kome {
	namespace operation {

		/**
		 * @class Alignment
		 * @brief alignment class
		 */
		class DATA_OPERATION_CLASS Alignment {
		public:
			/**
			 * @fn Alignment()
			 * @brief constructor
			 */
			Alignment();

			/**
			 * @fn virtual ~Alignment()
			 * @brief destructor
			 */
			virtual ~Alignment();

		protected:
			/** standard sample */
			kome::objects::Sample* m_standard;

			/** treatment sample */
			kome::objects::Sample* m_treatment;

		protected:
			/**
			 * @fn kome::objects::Sample* getStandardSample()
			 * @brief gets the standard sample
			 * @return standard sample
			 */
			kome::objects::Sample* getStandardSample();

			/**
			 * @fn kome::objects::Sample* getTreatmentSample()
			 * @brief gets the treatment sample
			 * @return treatment sample
			 */
			kome::objects::Sample* getTreatmentSample();

		public:
			/**
			 * @fn void prepare( kome::objects::Sample* standard, kome::objects::Sample* treatment, kome::core::Progress* progress )
			 * @brief prepares alignment
			 * @param[in] standard standard sample
			 * @param[in] treatment treatment sample
			 * @param[in,out] progress the object to show the progress
			 */
			void prepare( kome::objects::Sample* standard, kome::objects::Sample* treatment, kome::core::Progress* progress );

			/**
			 * @fn double convertRt( const double rt, const double mz )
			 * @brief gets aligned RT
			 * @param[in] rt RT before alignment
			 * @param[in] mz m/z before alignment
			 * @return aligned RT
			 */
			double convertRt( const double rt, const double mz );

			/**			
			 * @fn double convertMz( const double rt, const double mz )
			 * @brief gets aligned m/z
			 * @param[in] rt RT before alignment
			 * @param[in] mz m/z before alignment
			 * @return aligned m/z
			 */
			double convertMz( const double rt, const double mz );

			/**
			 * @fn double invertRt( const double rt, const double mz )
			 * @brief gets RT before alignment
			 * @param[in] rt aligned RT
			 * @param[in] mz aligned m/z
			 * @return RT before alignment
			 */
			double invertRt( const double rt, const double mz );

			/**
			 * @fn double invertMz( const double rt, const double mz )
			 * @brief gets m/z before alignment
			 * @param[in] rt aligned RT
			 * @param[in] mz aligned m/z
			 * @return m/z before alignemtn
			 */
			double invertMz( const double rt, const double mz );

		protected:
			/**
			 * @fn virtual void onPrepare( kome::objects::Sample* standard, kome::objects::Sample* treatment, kome::core::Progress* progress ) = 0
			 * @brief This method is called by prepare method. (abstract method)
			 * @param[in] standard standard sample
			 * @param[in] treatment treatment sample
			 * @param[in,out] progress the object to show the progress
			 */
			virtual void onPrepare( kome::objects::Sample* standard, kome::objects::Sample* treatment, kome::core::Progress* progress ) = 0;

			/**
			 * @fn virtual double onConvertRt( const double rt, const double mz ) = 0
			 * @brief This method is called by convertRt method. (abstract method)
			 * @param[in] rt RT before alignment
			 * @param[in] mz m/z before alignment
			 * @return aligned RT
			 */
			virtual double onConvertRt( const double rt, const double mz ) = 0;

			/**			
			 * @fn virtual double onConvertMz( const double rt, const double mz ) = 0
			 * @brief This method is called by convertMz method. (abstract method)
			 * @param[in] rt RT before alignment
			 * @param[in] mz m/z before alignment
			 * @return aligned m/z
			 */
			virtual double onConvertMz( const double rt, const double mz ) = 0;

			/**
			 * @fn virtual double onInvertRt( const double rt, const double mz ) = 0
			 * @brief This method is called by invertRt method. (abstract method)
			 * @param[in] rt aligned RT
			 * @param[in] mz aligned m/z
			 * @return RT before alignment
			 */
			virtual double onInvertRt( const double rt, const double mz ) = 0;

			/**
			 * @fn virtual double onInvertMz( const double rt, const double mz ) = 0
			 * @brief This method is called by invertMz method. (abstract method)
			 * @param[in] rt aligned RT
			 * @param[in] mz aligned m/z
			 * @return m/z before alignemtn
			 */
			virtual double onInvertMz( const double rt, const double mz ) = 0;

		public:
			/**
			 * @fn static Alignment& getDefaultAlignment()
			 * @brief gets default alignment
			 * @return default alignment
			 */
			static Alignment& getDefaultAlignment();
		};

		/**
		 * @class DefaultAlignment
		 * @brief default alignment class
		 */
		class DefaultAlignment : public Alignment {
		public:
			/**
			 * @fn DefaultAlignment()
			 * @brief constructor
			 */
			DefaultAlignment();

			/**
			 * @fn virtual ~DefaultAlignment()
			 * @brief destructor
			 */
			virtual ~DefaultAlignment();

		protected:
			/**
			 * @fn virtual void onPrepare( kome::objects::Sample* standard, kome::objects::Sample* treatment, kome::core::Progress* progress )
			 * @brief This method is called by prepare method. (override method)
			 * @param[in] standard standard sample
			 * @param[in] treatment treatment sample
			 * @param[in,out] progress the object to show the progress
			 */
			virtual void onPrepare( kome::objects::Sample* standard, kome::objects::Sample* treatment, kome::core::Progress* progress );

			/**
			 * @fn virtual double onConvertRt( const double rt, const double mz )
			 * @brief This method is called by convertRt method. (override method)
			 * @param[in] rt RT before alignment
			 * @param[in] mz m/z before alignment
			 * @return aligned RT
			 */
			virtual double onConvertRt( const double rt, const double mz );

			/**			
			 * @fn virtual double onConvertMz( const double rt, const double mz )
			 * @brief This method is called by convertMz method. (override method)
			 * @param[in] rt RT before alignment
			 * @param[in] mz m/z before alignment
			 * @return aligned m/z
			 */
			virtual double onConvertMz( const double rt, const double mz );

			/**
			 * @fn virtual double onInvertRt( const double rt, const double mz )
			 * @brief This method is called by invertRt method. (override method)
			 * @param[in] rt aligned RT
			 * @param[in] mz aligned m/z
			 * @return RT before alignment
			 */
			virtual double onInvertRt( const double rt, const double mz );

			/**
			 * @fn virtual double onInvertMz( const double rt, const double mz )
			 * @brief This method is called by invertMz method. (override method)
			 * @param[in] rt aligned RT
			 * @param[in] mz aligned m/z
			 * @return m/z before alignemtn
			 */
			virtual double onInvertMz( const double rt, const double mz );
		};


		/**
		 * @class AlignmentManager
		 * @brief alignment object management class
		 */
		class AlignmentManager {
		protected:
			/**
			 * @fn AlignmentManager()
			 * @brief cosntructor
			 */
			AlignmentManager();

			/**
			 * @fn virtual ~AlignmentManager()
			 * @brief destructor
			 */
			virtual ~AlignmentManager();

		public:
			/**
			 * @fn void addAlignment( Alignment* align )
			 * @brief adds alignment object
			 * @param[in] align alignment object to be added
			 */
			void addAlignment( Alignment* align );

			/**
			 * @fn void removeAlignment( Alignment* align )
			 * @brief removes alignment object
			 * @param[in] align alignment object to be removed
			 */
			void removeAlignment( Alignment* align );

		protected:
			/** alignment object set */
			std::set< Alignment* > m_alignSet;

		public:
			/**
			 * @fn static AlignmentManager& getInstance()
			 * @brief gets alignment manager object (This is the only object.)
			 * @return alignemnt manger object
			 */
			static AlignmentManager& getInstance();
		};
	}
}


#endif // __KOME_OPERATION_ALIGNMENT_H__
