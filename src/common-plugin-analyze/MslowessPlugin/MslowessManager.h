/**
 * @file MslowessManager.h
 * @brief interfaces of MslowessManager class
 *
 * @author A.Ozaki
 * @date 2013.07.31
 * 
 * Copyright(C) 2013-2013 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_LOWESS_MSLOWESS_MANAGER_H__
#define __KOME_PE_MSLOWESS_MANAGER_H__


namespace kome {
	namespace labeling {
		namespace mslowess {

			/**
			 * @class MslowessManager
			 * @brief MslowessManager class
			 */
			class MslowessManager
			{
			protected:
				/**
				 * @fn MslowessManager( void )
				 * @brief constructor
				 */
				MslowessManager( void );

				/**
				 * @fn virtual ~MslowessManager( void )
				 * @brief destructor
				 */
				virtual ~MslowessManager( void );

			protected:
				/** order */
				int	m_nOrder;

				/** Span */
				double m_dSpan;

				/** kernel */
				int	m_nKernel;

				/** robust iterations */
				double	m_dRobustIterations;

				/** S/N */
				double	m_dSn;

			public:
				/**
				 * @fn void setOrder( const int nOder )
				 * @brief sets order
				 * @param nOrder order
				 */
				void	setOrder( const int nOrder );

				/**
				 * @fn int getOrder( void )
				 * @brief gets order
				 * @return oder
				 */
				int	getOrder( void );

				/**
				 * @fn void setSpan( const double dSpan )
				 * @brief sets span
				 * @param dSpan span
				 */
				void	setSpan( const double dSpan );

				/**
				 * @fn double getSpan( void )
				 * @brief gets span
				 * @return span
				 */
				double	getSpan( void );

				/**
				 * @fn void setKernel( const int nKernel )
				 * @brief sets kernel
				 * @param nKernel kernel
				 */
				void	setKernel( const int nKernel );

				/**
				 * @fn int getKernel( void )
				 * @brief gets kernel
				 * @return kernel
				 */
				int	getKernel( void );

				/**
				 * @fn void setRobustIterations( const double dRobustIterations )
				 * @brief sets robust iterations
				 * @param dRobustIterations robust iterations
				 */
				void	setRobustIterations( const double dRobustIterations );

				/**
				 * @fn double getRobustIterations( void )
				 * @brief gets robust iterations
				 * @return robust iterations
				 */
				double	getRobustIterations( void );

				/**
				 * @fn void setSn( const double dSn )
				 * @brief sets S/N
				 * @param dSn S/N
				 */
				void	setSn( const double dSn );

				/**
				 * @fn double getSn( void )
				 * @brief gets S/N
				 * @return S/N
				 */
				double	getSn( void );

			public:
				/**
				 * @fn void detection( kome::core::XYData& xydata, kome::objects::Peaks& peaks )
				 * @brief detect peaks
				 * @param[in] xyData data points
				 * @pram[out] peaks the object to store peaks
				 */
				kome::objects::Variant	filtering( kome::objects::Parameters* params );

				/**
				 * @fn void releaseMatlab( void )
				 * @brief release MATLAB application
				 */
				void	releaseMatlab( void );

			private:
				/** Progress */
				kome::window::DialogProgress	*m_pProgress;

			private:
				/** initialize application */
				bool	initializeApplication( void );

			public:
				/**
				 * @fn static MslowessManager& getInstance()
				 * @brief gets object
				 * @return MslowessManager object. (This is the only object.)
				 */
				static MslowessManager& getInstance( void );
			};
		}
	}
}

#endif	// __KOME_LABELING_MSLOWESS_MANAGER_H__
