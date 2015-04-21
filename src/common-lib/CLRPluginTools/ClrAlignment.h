/**
 * @file ClrAlignment.h
 * @brief interfaces of ClrAlignment class
 *
 * @author S.Tanaka
 * @date 2013.01.22
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_CLR_ALIGNMENTT_H__
#define __KOME_CLR_ALIGNMENTT_H__


namespace kome {
	namespace clr {

		ref class ClrAlignmentBase;
		
		/**
		 * @class ClrAlignment
		 * @brief alignment object that has managed class object
		 */
		class ClrAlignment : public kome::operation::Alignment {
		public:
			/**
			 * @fn ClrAlignment()
			 * @brief constructor
			 */
			ClrAlignment();

			/**
			 * @fn virtual ~ClrAlignment()
			 * @brief destructor
			 */
			virtual ~ClrAlignment();

		protected:
			/** base script object */
			gcroot< ClrAlignmentBase^ > m_baseAlignment;

		public:
			/**
			 * @fn void setBaseAlignment( ClrAlignmentBase^ baseAlignment )
			 * @brief sets base script object
			 * @param[in] baseAlignment base script object
			 */
			void setBaseAlignment( ClrAlignmentBase^ baseAlignment );

			/**
			 * @fn ClrAlignmentBase^ getBaseAlignment()
			 * @brief gets base script object
			 * @return base script object
			 */
			ClrAlignmentBase^ getBaseAlignment();

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
		 * @class ClrAlignmentManager
		 * @brief ClrAlignment objects manager class
		 */
		class ClrAlignmentManager {
		protected:
			/**
			 * @fn ClrAlignmentManager()
			 * @brief constructor
			 */
			ClrAlignmentManager();

			/**
			 * @fn virtual ~ClrAlignmentManager()
			 * @brief destructor
			 */
			virtual ~ClrAlignmentManager();

		protected:
			/** alignment object set */
			std::set< ClrAlignment* > m_alignSet;

		public:
			/**
			 * @fn ClrAlignment& createAlignment()
			 * @brief creates alignment object
			 * @return alignment object

			 */
			ClrAlignment& createAlignment();

			/**
			 * @fn void removeAlignment( ClrAlignment& align )
			 * @brief removes alignment
			 * @param[in] align alignment object
			 */
			void removeAlignment( ClrAlignment& align );

			/**
			 * @fn void destroyAlignment( ClrAlignment& align )
			 * @brief destroys alignment object
			 * @param[in] align alignment object
			 */
			void destroyAlignment( ClrAlignment& align );

		public:
			/**
			 * @fn static ClrAlignmentManager& getInstance()
			 * @brief get the ClrAlignmentManager object. (This is the only object.)
			 * @return ClrAlignmentManager object
			 */
			static ClrAlignmentManager& getInstance();

		};
	}
}

#endif	// __KOME_CLR_ALIGNMENTT_H__
