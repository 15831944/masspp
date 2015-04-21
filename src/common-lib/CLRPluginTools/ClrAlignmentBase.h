/**
 * @file ClrAlignmentBase.h
 * @brief interfaces of ClrAlignmentBase class
 *
 * @author S.Tanaka
 * @date 2013.01.22
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_CLR_ALIGNMENTT_BASE_H__
#define __KOME_CLR_ALIGNMENTT_BASE_H__


#include "AlignmentWrapper.h"



namespace kome {
	namespace clr {

		/**
		 * @class ClrAlignmentBase
		 * @brief abstraction class of alignment on CLR environment
		 */
#ifndef _MANAGED	// (for doxygen)
		public ref class ClrAlignmentBase : public AlignmentWrapper {
#else
		public ref class ClrAlignmentBase abstract : public AlignmentWrapper {
#endif	// _MANAGED
		public:
			/**
			 * @fn ClrAlignmentBase()
			 * @brief constructor
			 * @return search engine object
			 */
			ClrAlignmentBase();

			/**
			 * @fn virtual ~ClrAlignmentBase()
			 * @brief destructor
			 */
			virtual ~ClrAlignmentBase();

		public:
			/**
			 * @fn virtual void onPrepare( SampleWrapper^ standard, SampleWrapper^ treatment, ProgressWrapper^ progress ) = 0
			 * @brief This method is called by prepare method. (abstract method)
			 * @param[in] standard standard sample
			 * @param[in] treatment treatment sample
			 * @param[in,out] progress the object to show the progress
			 */
			virtual void onPrepare( SampleWrapper^ standard, SampleWrapper^ treatment, ProgressWrapper^ progress ) = 0;

			/**
			 * @fn virtual double onConvertRt( double rt, double mz ) = 0
			 * @brief This method is called by convertRt method. (abstract method)
			 * @param[in] rt RT before alignment
			 * @param[in] mz m/z before alignment
			 * @return aligned RT
			 */
			virtual double onConvertRt( double rt, double mz ) = 0;

			/**			
			 * @fn virtual double onConvertMz( double rt, double mz ) = 0
			 * @brief This method is called by convertMz method. (abstract method)
			 * @param[in] rt RT before alignment
			 * @param[in] mz m/z before alignment
			 * @return aligned m/z
			 */
			virtual double onConvertMz( double rt, double mz ) = 0;

			/**
			 * @fn virtual double onInvertRt( double rt, double mz ) = 0
			 * @brief This method is called by invertRt method. (abstract method)
			 * @param[in] rt aligned RT
			 * @param[in] mz aligned m/z
			 * @return RT before alignment
			 */
			virtual double onInvertRt( double rt, double mz ) = 0;

			/**
			 * @fn virtual double onInvertMz( double rt, double mz ) = 0
			 * @brief This method is called by invertMz method. (abstract method)
			 * @param[in] rt aligned RT
			 * @param[in] mz aligned m/z
			 * @return m/z before alignemtn
			 */
			virtual double onInvertMz( double rt, double mz ) = 0;
		};
	}
}

#endif	// __KOME_CLR_ALIGNMENTT_BASE_H__
