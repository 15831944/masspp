/**
 * @file ClrVariant.h
 * @brief interfaces of ClrVariant class
 *
 * @author S.Tanaka
 * @date 2006.08.25
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_VARIANT_H__
#define __KOME_CLR_VARIANT_H__


namespace kome {
	namespace clr {

		/**
		 * @class ClrVariant
		 * @brief this class treats value of various types on CLR environment
		 */
		public ref class ClrVariant {
		public:
			/**
			 * @fn ClrVariant()
			 * @brief constructor
			 */
			ClrVariant();

			/**
			 * @fn ClrVariant( kome::objects::Variant& val )
			 * @brief constructor
			 * @param[in] val value information
			 */
			ClrVariant( kome::objects::Variant& val );

			/**
			 * @fn virtual ~ClrVariant()
			 * @brief destructor
			 */
			virtual ~ClrVariant();

// >>>>>>	@Date:2013/07/29	<Add>	A.Ozaki
//
			/**
			 * @fn void operator = ( ClrVariant% insSrc )
			 * @brief assignment operator
			 * @param[in] insSrc source object
			 */
			void	operator = ( ClrVariant% insSrc );

			/**
			 * @fn ClrVariant( ClrVariant% insSrc )
			 * @brief copy constructor
			 * @param[in] insSrc source object
			 */
			ClrVariant( ClrVariant% insSrc );
//
// <<<<<<	@Date:2013/07/29	<Add>	A.Ozaki

		public:
			/** data type */
			ENUM_CLASS DataType {
				UNKNOWN,
				BOOL,
				INTEGER,
				DOUBLE,
				STRING,
				ARRAY,
				SAMPLE_SET,
				SAMPLE,
				SPECTRUM,
				DATA_SET,
				DATA_GROUP_NODE,
				CHROMATOGRAM,
				XY_DATA,
				PEAKS,
				PEAKS_2D,
				DRAWING_DATA,
				PROGRESS,
				GRAPHICS,
				FONT,
				MOUSE_EVT,
				KEY_EVT,
				DATA_MAP,
				SIZE_EVT,
				MOVE_EVT,
				SETTING_VALUES,
				SCRIPT,
				FILE_READER,	// @date 2012/08/21 <Add> OKADA
				OTHER
			};

			/** value type */
			DataType type;

			/** object */
			System::Object^ obj;

			/**
			 * @fn void setValue( kome::objects::Variant& val )
			 * @brief sets value
			 * @param[in] val value information
			 */
			void setValue( kome::objects::Variant& val );

			/**
			 * @fn void transmit( kome::objects::Variant& variant )
			 * @brief tramsmits same value to Variant object
			 * @param[out] variant the object stored value
			 */
			void transmit( kome::objects::Variant& variant );
		};
	}
}

#endif	// __KOME_CLR_VARIANT_H__
