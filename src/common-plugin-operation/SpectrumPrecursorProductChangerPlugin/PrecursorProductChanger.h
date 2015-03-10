/**
 * @file SpectrumPrecursorProductChangerPluginFunctions.h
 * @brief interfaces of SpectrumPrecursorProductChangerPluginFunctions class
 *
 * @author H.Parry
 * @date 2011.03.22
 * 
 * Copyright(C) 2014 Shimadzu Corporation. All rights reserved.
 */

#ifdef __unix
	extern "C" {
#endif  // __unix

#ifndef __KOME_SPECTRUM_PRECURSOR_PRODUCT_CHANGER_H__
#define __KOME_SPECTRUM_PRECURSOR_PRODUCT_CHANGER_H__

/**
 * @fn kome::objects::Variant openPrecursorIonSpectrum( kome::objects::Parameters* params )
 * @brief open precursor ion spectrum
 * @return If true, open precursor ion spectrum is available.
 */
kome::objects::Variant openPrecursorIonSpectrum( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant openProductIonSpectrum( kome::objects::Parameters* params )
 * @brief open product ion spectrum
 * @return If true, product ion spectrum is available.
 */
kome::objects::Variant openProductIonSpectrum( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant openPrecursorProductIonSpectrum( kome::objects::Parameters* params, int flgPrecursorProduct )
 * @brief open precursor ion spectrum
 * @return If true, open Precursor Product Ion spectrum is available.
 */
kome::objects::Variant openPrecursorProductIonSpectrum( kome::objects::Parameters* params, int flgPrecursorProduct );

/**
 * @fn kome::objects::Variant enablePrecursorIonSpectrum( kome::objects::Parameters* params )
 * @brief enable precursor ion spectrum
 * @return If true, enable precursor ion spectrum is available.
 */
kome::objects::Variant enablePrecursorIonSpectrum( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant enableProductIonSpectrum( kome::objects::Parameters* params );
 * @brief enable product ion spectrum
 * @return If true, enable product ion spectrum is available.
 */
kome::objects::Variant enableProductIonSpectrum( kome::objects::Parameters* params );

#ifdef __unix
	}
#endif  // __unix

#endif	// __KOME_SPECTRUM_PRECURSOR_PRODUCT_CHANGER_H__
