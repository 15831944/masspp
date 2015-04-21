/**
 * @file ClrSpectrumBase.h
 * @brief interfaces of ClrSpectrumBase class
 *
 * @author S.Tanaka
 * @date 2006.08.30
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_SPECTRUM_BASE_H__
#define __KOME_CLR_SPECTRUM_BASE_H__


#include "SpectrumWrapper.h"


namespace kome {
	namespace clr {

		ref class SampleWrapper;
		ref class XYDataWrapper;
		ref class PropertiesWrapper;

		/**
		 * @class ClrSpectrumBase
		 * @brief abstract class of spectrum information on CLR environment
		 */
#ifndef _MANAGED	// (for doxygen)
		public ref class ClrSpectrumBase : public SpectrumWrapper {
#else
		public ref class ClrSpectrumBase abstract : public SpectrumWrapper {
#endif	// _MANAGED
		public:
			/**
			 * @fn ClrSpectrumBase( SampleWrapper^ sample, System::String^ name )
			 * @brief constructor
			 * @param[in] sample sample object that has this spectrum
			 * @param[in] name spectrum name
			 */
			ClrSpectrumBase( SampleWrapper^ sample, System::String^ name );

			/**
			 * @fn virtual ~ClrSpectrumBase()
			 * @brief destructor
			 */
			virtual ~ClrSpectrumBase();

		public:
			/**
			 * @fn virtual void onGetXYData( XYDataWrapper^ xyData, double minX, double maxX ) = 0
			 * @brief This method is called by getXYData method. (abstract method)
			 * @param[out] xyData the object to store data points
			 * @param[in] minX minimum x value. (If minX is negative number, minimum x value is not unbounded.)
			 * @param[in] maxX maximum x value. (If maxX is negative number, maximum x value is not unbounded.)
			 */
			virtual void onGetXYData( XYDataWrapper^ xyData, double minX, double maxX ) = 0;

			/**
			 * @fn virtual void onGetXRange( double% minX, double% maxX ) = 0
			 * @brief This method is called by getXRange method. (abstract method)
			 * @param[out] minX the pointer to store minimum x value
			 * @param[out] maxX the pointer to store maximum x value
			 */
			virtual void onGetXRange( double% minX, double% maxX ) = 0;

			/**
			 * @fn virtual double onGetTotalIntensity( double minX, double maxX ) = 0
			 * @brief This method is called by getTotalIntensity method. (abstract method)
			 * @param[in] minX minimum x of range (If minX is negative number, minimum x value is unbounded.)
			 * @param[in] maxX maximum x of range (If maxX is negative number, maximum x value is unbounded.)
			 * @return total intensity
			 */
			virtual double onGetTotalIntensity( double minX, double maxX ) = 0;

			/**
			 * @fn virtual double onGetMaxIntensity( double minX, double maxX ) = 0
			 * @brief This method is called by getMaxIntensity method. (abstract method)
			 * @param[in] minX minimum x of range (If minX is negative number, minimum x value is unbounded.)
			 * @param[in] maxX maximum x of range (If maxX is negative number, maximum x value is unbounded.)
			 * @return max intensity
			 */
			virtual double onGetMaxIntensity( double minX, double maxX ) = 0;

// >>>>>> @Date:2013/07/16	<Add>	A.Ozaki
//
			/**
			 * @fn virtual  void onSetRequestLoadData( void )
			 * @brief This method is called by setRequestLoadData method. (abstract method)
			 */
			virtual void	onSetRequestLoadData( void );
			/**
			 * @fn virtual  bool onResetRequestLoadData( void )
			 * @brief This method is called by resetRequestLoadData method. (abstract method)
			 */
			virtual void	onResetRequestLoadData( void );
			
			/**
			 * @fn virtual  bool onIsRequestLoadData( void )
			 * @brief This method is called by isRequestLoadData method. (abstract method)
			 * @return If true, file read request is valid.
			 */
			virtual bool	onIsRequestLoadData( void );

			/**
			 * @fn virtual  void onSetFirstAccess( void )
			 * @brief This method is called by setFirstAccess method. (abstract method)
			 */
			virtual void	onSetFirstAccess( void );

			/**
			 * @fn virtual  void onResetFirstAccess( void )
			 * @brief This method is called by resetFirstAccess method. (abstract method)
			 */
			virtual void	onResetFirstAccess( void );
			
			/**
			 * @fn virtual  bool onIsFirstAccess( void )
			 * @brief This method is called by isFirstAccess method. (abstract method)
			 * @return If true, the first accessing.
			 */
			virtual bool	onIsFirstAccess( void );
			
			/**
			 * @fn virtual  bool onLoadData( void )
			 * @brief This method is called by loadData method. (abstract method)
			 * @return If true, file reading success.
			 */
			virtual bool	onLoadData( void );

//
// <<<<<<	@Date:2013/07/18	<Add>	A.Ozaki
		};
	}
}

#endif		// __KOME_CLR_SPECTRUM_BASE_H__
