/**
 * @file SpectrumWrapper.h
 * @brief interfaces of SpectrumWrapper class
 *
 * @author S.Tanaka
 * @date 2006.08.30
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_SPECTRUM_WRAPPER_H__
#define __KOME_CLR_SPECTRUM_WRAPPER_H__


namespace kome {
	namespace clr {

		ref class SampleWrapper;
		ref class XYDataWrapper;
		ref class PropertiesWrapper;
		ref class DataGroupNodeWrapper;

		/**
		 * @class SpectrumWrapper
		 * @brief Spectrum object wrapper class to use on CLR environment
		 */
		public ref class SpectrumWrapper {
		public:
			/**
			 * @fn SpectrumWrapper( kome::objects::Spectrum& spec )
			 * @brief constructor
			 * @param[in] spec Spectrum object
			 */
			SpectrumWrapper( kome::objects::Spectrum& spec );

			/**
			 * @fn virtual ~SpectrumWrapper()
			 * @brief destructor
			 */
			virtual ~SpectrumWrapper();

		protected:
			/** Spectrum object */
			kome::objects::Spectrum& m_spec;

		public:
			/**
			 * @fn kome::objects::Spectrum& getSpectrum()
			 * @brief gets the Spectrum object
			 * @return the pointer to the Spectrum object
			 */
			kome::objects::Spectrum& getSpectrum();

		public:
			/** definition of polarity */
			ENUM_CLASS Polarity {
				UNKNOWN,
				POSITIVE,
				NEGATIVE
			};

		public:
			/**
			 * @fn void setId( int id )
			 * @brief sets spectrum id
			 * @param[in] id spectrum id
			 */
			void setId( int id );

			/**
			 * @fn int getId()
			 * @brief gets spectrum id
			 * @return spectrum id
			 */
			int getId();

		public:
			/**
			 * @fn SampleWrapper^ getSample()
			 * @brief gets sample that has this spectrum
			 * @return the pointer to sample object
			 */
			SampleWrapper^ getSample();

			/**
			 * @fn void setName( System::String^ name )
			 * @brief sets spectrum name
			 * @param[in] name spectrum name
			 */
			void setName( System::String^ name );

			/**
			 * @fn System::String^ getName()
			 * @brief gets spectrum name
			 * @return spectrum name
			 */
			System::String^ getName();

			/**
			 * @fn void setRt( double rt )
			 * @brief sets retention time
			 * @param[in] rt retention time
			 */
			void setRt( double rt );

			/**
			 * @fn void setRt( double startRt, double endRt )
			 * @brief gets retention time
			 * @param[in] startRt the start retention time
			 * @param[in] endRt the end retention time
			 */
			void setRt( double startRt, double endRt );

			/**
			 * @fn void setStartRt( double rt )
			 * @brief sets start retention time
			 * @param[in] rt start retention time
			 */
			void setStartRt( double rt );

			/**
			 * @fn void setEndRt( double rt )
			 * @brief sets end retention time
			 * @param[in] rt end retention time
			 */
			void setEndRt( double rt );

			/**
			 * @fn double getRt()
			 * @brief gets retention time
			 * @return retention time
			 */
			double getRt();

			/**
			 * @fn double getStartRt()
			 * @brief gets the start of retention time
			 * @return the start of retention time
			 */
			double getStartRt();

			/**
			 * @fn double getEndRt()
			 * @brief gets the end of retention time
			 * @return the end of retention time
			 */
			double getEndRt();

			/**
			 * @fn void setSpecType( System::String^ type )
			 * @brief sets spectrum type
			 * @param[in] type spectrum type
			 */
			void setSpecType( System::String^ type );

			/**
			 * @fn System::String^ getSpecType()
			 * @brief gets spectrum type
			 * @return spectrum type
			 */
			System::String^ getSpecType();

			/**
			 * @fn void setTitle( System::String^ title )
			 * @brief sets spectrum title
			 * @param[in] title spectrum title
			 */
			void setTitle( System::String^ title );

			/**
			 * @fn System::String^ getTitle()
			 * @brief gets spectrum title
			 * @return spectrum title
			 */
			System::String^ getTitle();

			/**
			 * @fn void setIcon( System::String^ icon )
			 * @brief sets icon name
			 * @param[in] icon
			 */
			void setIcon( System::String^ icon );

			/**
			 * @fn System::String^ getIcon()
			 * @brief gets icon name
			 * @return icon name
			 */
			System::String^ getIcon();

			/**
			 * @fn XYDataWrapper^ getXYData()
			 * @brief gets xy data from data manager
			 * @return xy data object
			 */
			XYDataWrapper^ getXYData();

			/**
			 * @fn void deleteXYData()
			 * @brief deletes xy data of data manager
			 */
			void deleteXYData();

			/**
			 * @fn void getXYData( XYDataWrapper^ xyData, bool op )
			 * @brief gets data points
			 * @param[out] xyData the object to store data points
			 * @param[in] op If true, getting operated data points
			 */
			void getXYData( XYDataWrapper^ xyData, bool op );

			/**
			 * @fn void getXYData( XYDataWrapper^ xyData, double minX, double maxX, bool zero )
			 * @brief gets data points that spectrum has
			 * @param[out] xyData the object to store daata points
			 * @param[in] minX minimum x value. (If minX is negative number, minimum x value is not specified.)
			 * @param[in] maxX maximum x value. (If maxX is negative number, maximum x value is not specified.)
			 * @param[in] zero zero intensities insertion flag
			 */
			void getXYData( XYDataWrapper^ xyData, double minX, double maxX, bool zero );

			/**
			 * @fn void setXRange( double minX, double maxX )
			 * @brief sets mz range
			 * @param[in] minX min x
			 * @param[in] maxX max x
			 */
			void setXRange( double minX, double maxX );

			/**
			 * @fn void setMinX( double minX )
			 * @brief sets min x
			 * @param[in] minX min x
			 */
			void setMinX( double minX );

			/**
			 * @fn double getMinX()
			 * @brief gets min x
			 * @return min x
			 */
			double getMinX();

			/**
			 * @fn void setMaxX( double maxX )
			 * @brief sets max x
			 * @param[in] maxX max x
			 */
			void setMaxX( double maxX );

			/**
			 * @fn double getMaxX()
			 * @brief gets max x
			 * @return max x
			 */
			double getMaxX();

			/**
			 * @fn void setTotalIntensity( double intensity )
			 * @brief sets total intensity of spectrum
			 * @param[in] intensity total intensity.
			 */
			void setTotalIntensity( double intensity );

			/**
			 * @fn double getTotalIntensity( double minX, double maxX )
			 * @brief gets total intensity in specified range
			 * @param[in] minX minimum x of range (If minX is negative number, minimum x value is unbounded.)
			 * @param[in] maxX maximum x of range (If maxX is negative number, maximum x value is unbounded.)
			 * @return total intensity
			 */
			double getTotalIntensity( double minX, double maxX );

			/**
			 * @fn void setMaxIntensity( double intensity )
			 * @brief sets max intensity of spectrum
			 * @param[in] intensity max intensity
			 */
			void setMaxIntensity( double intensity );

			/**
			 * @fn double getMaxIntensity( double minX, double maxX )
			 * @brief gets max intensity in specified range
			 * @param[in] minX minimum x of range (If minX is negative number, minimum x value is unbounded.)
			 * @param[in] maxX maximum x of range (If maxX is negative number, maximum x value is unbounded.)
			 * @return max intensity
			 */
			double getMaxIntensity( double minX, double maxX );

			/**
			 * @fn void setBasePeakMass( double mass )
			 * @brief sets base peak mass
			 * @param[in] mass base peak mass
			 */
			void setBasePeakMass( double mass );

			/**
			 * @fn double getBasePeakMass()
			 * @brief gets base peak mass
			 * @return base peak mass
			 */
			double getBasePeakMass();

			/**
			 * @fn void setMsStage( int stage )
			 * @brief sets ms stage
			 * @param[in] stage ms stage
			 */
			void setMsStage( int stage );

			/**
			 * @fn int getMsStage();
			 * @brief gets ms stage
			 * @return ms stage
			 */
			int getMsStage();

			/**
			 * @fn void setPrecursor( int stage, const double precursor )
			 * @brief sets precursor
			 * @param[in] stage MS stage of precursor ion spectrum.
			 * @param[in] precursor precursor mass
			 */
			void setPrecursor( int stage, double precursor );

			/**
			 * @fn void setPrecursor( double precursor )
			 * @brief sets precursor
			 * @param[in] precursor precursor mass
			 */
			void setPrecursor( double precursor );

			/**
			 * @fn double getPrecursor( int stage )
			 * @brief gets precursor
			 * @param[in] stage MS stage of precursor ion spectrum
			 * @return precursor
			 */
			double getPrecursor( int stage );

			/**
			 * @fn double getPrecursor()
			 * @brief gets the precursor mass of precursor ion spectrum.
			 * @return precursor mass
			 */
			double getPrecursor();

			/**
			 * @fn void setPrecursorIntensity( int stage, double intensity )
			 * @brief sets precursor intensity 
			 * @param[in] stage MS stage of precursor ion spectrum
			 * @param[in] intensity precursor intensity
			 */
			void setPrecursorIntensity( int stage, double intensity );

			/**
			 * @fn void setPrecursorIntensity( double intensity )
			 * @brief sets precursor intensity 
			 * @param[in] intensity precursor intensity
			 */
			void setPrecursorIntensity( double intensity );

			/**
			 * @fn double getPrecursorIntensity( int stage )
			 * @brief gets the precursor intensity
			 * @param[in] stage MS stage of precursor ion spectrum
			 * @return precursor intensity
			 */
			double getPrecursorIntensity( int stage );

			/**
			 * @fn double getPrecursorIntensity()
			 * @brief gets the precursor intensity of the precursor ion spectrum.
			 * @return the precursor intensity
			 */
			double getPrecursorIntensity();

			/**
			 * @fn void setPrecursorCharge( int charge )
			 * @brief sets the precursor charge
			 * @param[in] charge precursor charge
			 */
			void setPrecursorCharge( int charge );

			/**
			 * @fn int getPrecursorCharge()
			 * @brief gets the precursor charge
			 * @return precursor charge
			 */
			int getPrecursorCharge();

			/**
			 * @fn void setParentSpectrum( SpectrumWrapper^ parent )
			 * @brief sets parent spectrum
			 * @param[in] parent parent spectrum oebject. (If this spectrum has no parent, this parameter is NULL.)
			 */
			void setParentSpectrum( SpectrumWrapper^ parent );

			/**
			 * @fn SpectrumWrapper^ getParentSpectrum()
			 * @brief gets parent spectrum
			 * @return parent spectrum. (If this spectrum has no parent, this method returns NULL.)
			 */
			SpectrumWrapper^ getParentSpectrum();

			/**
			 * @fn cli::array< SpectrumWrapper^ >^ getChildSpectra()
			 * @brief gets child spectra
			 * @return the array of child spectra
			 */
			cli::array< SpectrumWrapper^ >^ getChildSpectra();

			/**
			 * @fn void setHasChromatogram( bool chromatogram )
			 * @brief sets wheher this spectrum has chromatogram
			 * @param[in] chromatogram whether this spectrum has chromatogram
			 */
			void setHasChromatogram( bool chromatogram );

			/**
			 * @fn bool hasChromatogram()
			 * @brief judges whether this spectrum has chromatogram
			 * @return where this spectrum has chromatogram
			 */
			bool hasChromatogram();

			/**
			 * @fn void setGroup( DataGroupNodeWrapper^ group )
			 * @brief sets spectrum group
			 * @param[in] group spectrum group
			 */
			void setGroup( DataGroupNodeWrapper^ group );

			/**
			 * @fn DataGroupNodeWrapper^ getGroup()
			 * @brief gets spectrum group
			 * @return spectrum group
			 */
			DataGroupNodeWrapper^ getGroup();

			/**
			 * @fn void setScanNumber( int scan )
			 * @brief sets scan number
			 * @param[in] scan scan number
			 */
			void setScanNumber( int scan );

			/**
			 * @fn int getScanNumber()
			 * @brief gets scan number
			 * @return scan number
			 */
			int getScanNumber();

			/**
			 * @fn void setPolarity( Polarity polarity )
			 * @brief sets polarity
			 * @param[in] polarity polarity
			 */
			void setPolarity( Polarity polarity );

			/**
			 * @fn Polarity getPolarity()
			 * @brief gets polarity
			 * @return polarity
			 */
			Polarity getPolarity();

			/**
			 * @fn void setCentroidMode( bool centroidMode )
			 * @brief sets centroid mode or not
			 * @param[in] centroidMode If true, this spectrum is centroid mode.
			 */
			void setCentroidMode( bool centroidMode );

			/**
			 * @fn bool isCentroidMode()
			 * @brief judget wheter this spectrum is centroid mode
			 * @return If true, this spectrum is centroid mode.
			 */
			bool isCentroidMode();

			/**
			 * @fn void setResolution( double resolution )
			 * @brief sets resolution
			 * @param[in] resolution resolution
			 */
			void setResolution( double resolution );

			/**
			 * @fn double getResolution()
			 * @brief gets resolution
			 * @return resolution
			 */
			double getResolution();

			/**
			 * @fn void setCollisionEnergy( System::String^ collisionEnergy )
			 * @brief sets the collision energy
			 * @param[in] collisionEnergy collision energy
			 */
			void setCollisionEnergy( System::String^ collisionEnergy );

			/**
			 * @fn System::String^ getCollisionEnergy()
			 * @brief gets the collision energy
			 * @return collision energy
			 */
			System::String^ getCollisionEnergy();

			/**
			 * @fn void setVisible( bool visible )
			 * @brief sets the visible flag
			 * @param[in] visible visible flag value
			 */
			void setVisible( bool visible );

			/**
			 * @fn bool isVisible()
			 * @brief gets the visible flag value
			 * @return visible flag value
			 */
			 bool isVisible();

			 /**
			  * @fn void setAutoZeroPoints( bool autoZero )
			  * @brief sets auto zero points flag value
			  * @param[in] autoZero auto zero points flag value.
			  */
			 void setAutoZeroPoints( bool autoZero );

			 /**
			  * @fn bool isAutoZeroPoints()
			  * @brief gets auto zero points flag value
			  * @return auto zero points flag value
			  */
			 bool isAutoZeroPoints();

			/**
			 * @fn PropertiesWrapper^ getProperties()
			 * @brief gets properties
			 * @return properties
			 */
			PropertiesWrapper^ getProperties();

			/**
			 * @fn void setOrgSpectrum( SpectrumWrapper^ spec )
			 * @brief sets original spectrum
			 * @param[in] spec orignal spectrum
			 */
			void setOrgSpectrum( SpectrumWrapper^ spec );

			/**
			 * @fn SpectrumWrapper^ getOrgSpectrum()
			 * @brief gets original spectrum
			 * @return original spectrum
			 */
			SpectrumWrapper^ getOrgSpectrum();

			/**
			 * @fn void getProperties( PropertiesWrapper^ properties )
			 * @brief gets spectrum and spectrum group properties
			 * @param[in] properties properties object to store properties
			 */
			void getProperties( PropertiesWrapper^ properties );

		public:
			/**
			 * @fn static bool isCommonProperty( System::String^ key )
			 * @brief check whther the specified property key is common property or not
			 * @param[in] key parameter key
			 * @return If true, specified key is common property key
			 */
			static bool isCommonProperty( System::String^ key );

// >>>>>>	@Date:2013/07/18	<Add>	A.Ozaki
//
		public:
			/**
			 * @fn void	setSpotId( System::String^ spotId )
			 * @brief set spot id
			 */
			void setSpotId( System::String^ spotId );
			/**
			 * @fn System::String^ getSpotId( )
			 * @brief get spot id
			 * @return string of spot id
			 */
			System::String^ getSpotId( void );

			/**
			 * @fn void	setRequestLoadData( void )
			 * @brief set the flag of request load data
			 */
			 void	setRequestLoadData( void );

			 /**
			 * @fn void	setRequestLoadData( void )
			 * @brief reset the flag of request load data
			 */
			 void	resetRequestLoadData( void );
			
			/**
			 * @fn bool	isRequestLoadData( void )
			 * @brief check the flag of request load data
			 * @return If true, enable flag of request load data
			 */
			bool	isRequestLoadData( void );

			/**
			 * @fn void	setFirstAccess( void )
			 * @brief set the flag of first access
			 */
			void	setFirstAccess( void );

			/**
			 * @fn void	resetFirstAccess( void )
			 * @brief reset the flag of first access
			 */
			void	resetFirstAccess( void );
			
			/**
			 * @fn bool	isFirstAccess( void )
			 * @brief check the flag of first access
			 * @return If true, enable flag of first access
			 */
			bool	isFirstAccess( void );
			
			/**
			 * @fn bool	loadData( void )
			 * @brief load data
			 * @return If true, file reading success.
			 */
			bool	loadData( void );

//
// <<<<<<	@Date:2013/07/18	<Add>	A.Ozaki
		};
	}
}

#endif	// __KOME_CLR_SPECTRUM_WRAPPER_H__
