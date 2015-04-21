/**
 * @file Spectrum.h
 * @brief interfaces of Spectrum class
 *
 * @author S.Tanaka
 * @date 2006.08.30
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_OBJECTS_SPECTRUM_H__
#define __KOME_OBJECTS_SPECTRUM_H__


#include <string>
#include <boost/optional.hpp>


namespace kome {
	namespace objects {

		class Sample;
		class DataGroupNode;

		/**
		 * @class Spectrum
		 * @brief spectrum information management class
		 */
		class DATA_OBJECTS_CLASS Spectrum {
		public:
			/**
			 * @fn Spectrum( Sample* sample, const char* name )
			 * @brief constructor
			 * @param[in] sample sample object that has this spectrum
			 * @param[in] name spectrum name
			 */
			Spectrum( Sample* sample, const char* name );

			/**
			 * @fn virtual ~Spectrum()
			 * @brief destructor
			 */
			virtual ~Spectrum();

		public:
			/**
			 * @typedef Polarity
			 * @brief polarity definition
			 */
			typedef enum {
				POLARITY_UNKNOWN = 0,
				POLARITY_POSITIVE = 1,
				POLARITY_NEGATIVE = 2
			} Polarity;

		protected:
			/**
			 * @struct PrecursorInfo
			 * @brief precursor information
			 */
			struct PrecursorInfo {
				int stage;
				double precursor;
				double intensity;
			};

		private:
			/** sample object */
			Sample* m_sample;

			/** name */
			std::string m_name;

			/** parent spectrum */
			Spectrum* m_parentSpec;

			/** child spectra */
			std::vector< Spectrum* > m_children;

			/** has chromatogram flag */
			bool m_chrom;

			/** spectrum group */
			DataGroupNode* m_group;

			/** retention time */
			struct {
				double start;
				double end;
			} m_rt;

			/** icon name */
			std::string m_icon;

			/** spectrum type */
			std::string m_specType;

			/** spectrum title */
			std::string m_title;

			/** min x */
			boost::optional< double > m_minX;

			/** max x */
			boost::optional< double > m_maxX;

			/** total intensity */
			boost::optional< double > m_tic;

			/** base peak intensity */
			boost::optional< double > m_bpi;

			/** base peak mass */
			double m_bpm;

			/** MS stage */
			int m_msStage;

			/** precursor */
			std::vector< PrecursorInfo > m_precursor;

			/** precursor charge */
			int m_charge;

			/** scan number */
			int m_scanNumber;

			/** polarity */
			Polarity m_polarity;

			/** centroid mode */
			bool m_centroidMode;

			/** resolution */
			double m_resolution;

			/** collision energy */
			std::string m_collisionEnergy;

			/** visible flag */
			bool m_visible;

			/** properties */
			kome::core::Properties m_props;

			/** user properties */
			kome::core::Properties m_userProps;

			/** auto zero */
			bool m_autoZeroPoints;

			// >>>>>> @Date:2013/07/11	<Add>	A.Ozaki

			/** spot id */
			std::string	m_strSpotId;

			// <<<<<< @Date:2013/07/11	<Add>	A.Ozaki

		protected:
			/** original spectrum */
			Spectrum* m_orgSpec;
			
			/** operation flag */
			bool m_op;

		protected:
			/** common properties */
			static std::vector< std::string > m_commonProps;
			
			/** spectrum id */
			int m_specId;

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
			 * @fn void setOperationFlag( const bool op )
			 * @brief sets the operation flag value
			 * @param[in] op operation flag value
			 */
			void setOperationFlag( const bool op ){ m_op = op; }

			/**
			 * @fn bool getOperationFlag()
			 * @brief gets the operation flag value
			 * @return operation flag value
			 */
			bool getOperationFlag(){ return m_op; }
		public:
			/**
			 * @fn Sample* getSample()
			 * @brief gets the sample that has this spectrum
			 * @return sample object
			 */
			Sample* getSample();

			/**
			 * @fn void setName( const char* name )
			 * @brief sets spectrum name
			 * @param[in] name spectrum name
			 */
			void setName( const char* name );

			/**
			 * @fn const char* getName()
			 * @brief gets spectrum name
			 * @return spectrum name
			 */
			const char* getName();

			/**
			 * @fn void setRt( const double rt )
			 * @brief sets retention time
			 * @param[in] rt retention time
			 */
			void setRt( const double rt );

			/**
			 * @fn void setRt( const double startRt, const double endRt )
			 * @brief gets retention time
			 * @param[in] startRt the start retention time
			 * @param[in] endRt the end retention time
			 */
			void setRt( const double startRt, const double endRt );

			/**
			 * @fn void setStartRt( const double rt )
			 * @brief sets start retention time
			 * @param[in] rt start retention time
			 */
			void setStartRt( const double rt );

			/**
			 * @fn void setEndRt( const double rt )
			 * @brief sets end retention time
			 * @param[in] rt end retention time
			 */
			void setEndRt( const double rt );

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
			 * @fn void setSpecType( const char* type )
			 * @brief sets spectrum type
			 * @param[in] type spectrum type
			 */
			void setSpecType( const char* type );

			/**
			 * @fn const char* getSpecType()
			 * @brief gets spectrum type
			 * @return spectrum type
			 */
			const char* getSpecType();

			/**
			 * @fn void setTitle( const char* title )
			 * @brief sets spectrum title
			 * @param[in] title spectrum title
			 */
			void setTitle( const char* title );

			/**
			 * @fn const char* getTitle()
			 * @brief gets spectrum title
			 * @return spectrum title
			 */
			const char* getTitle();

			/**
			 * @fn void setIcon( const char* icon )
			 * @brief sets icon name
			 * @return icon name
			 */
			void setIcon( const char* icon );

			/**
			 * @fn const char* getIcon()
			 * @brief gets icon name
			 * @return icon name
			 */
			const char* getIcon();

			/**
			 * @fn kome::core::XYData* getXYData()
			 * @brief gets xy data from data manager
			 * @return xy data object
			 */
			kome::core::XYData* getXYData();

			/**
			 * @fn void deleteXYData()
			 * @brief deletes xy data of data manager
			 */
			void deleteXYData();

			/**
			 * @fn void getXYData( kome::core::XYData* const xyData, const bool op )
			 * @brief gets data points
			 * @param[out] xyData the object to store data points
			 * @param[in] op If true, getting operated data points
			 */
			void getXYData( kome::core::XYData* const xyData, const bool op );

			/**
			 * @fn void getXYData( kome::core::XYData* const xyData, const double minX, const double maxX, const bool zero )
			 * @brief gets data points that spectrum has
			 * @param[out] xyData the object to store data points
			 * @param[in] minX minimum x value. (If minX is negative number, minimum x value is not unbounded.)
			 * @param[in] maxX maximum x value. (If maxX is negative number, maximum x value is not unbounded.)
			 * @param[in] zero zero intensities insertion flag
			 */
			void getXYData( kome::core::XYData* const xyData, const double minX, const double maxX, const bool zero );

			/**
			 * @fn void setXRange( const double minX, const double maxX )
			 * @brief sets x range
			 * @param[in] minX min x
			 * @param[in] maxX max x
			 */
			void setXRange( const double minX, const double maxX );

			/**
			 * @fn void setMinX( const double minX )
			 * @brief sets min x
			 * @param[in] minX min x
			 */
			void setMinX( const double minX );

			/**
			 * @fn double getMinX()
			 * @brief gets min x
			 * @return min x
			 */
			double getMinX();

			/**
			 * @fn void setMaxX( const double maxX )
			 * @brief sets max x
			 * @param[in] maxX max x
			 */
			void setMaxX( const double maxX );

			/**
			 * @fn double getMaxX()
			 * @brief gets max x
			 * @return max x
			 */
			double getMaxX();

			/**
			 * @fn void setTotalIntensity( const double intensity )
			 * @brief sets total intensity of spectrum
			 * @param[in] intensity total intensity.
			 */
			void setTotalIntensity( const double intensity );

			/**
			 * @fn double getTotalIntensity( const double minX = -1.0, const double maxX = -1.0 )
			 * @brief gets total intensity in specified range
			 * @param[in] minX minimum x of range (If minX is negative number, minimum x value is unbounded.)
			 * @param[in] maxX maximum x of range (If maxX is negative number, maximum x value is unbounded.)
			 * @return total intensity
			 */			 
			double getTotalIntensity( const double minX = -1.0, const double maxX = -1.0 );

			/**
			 * @fn void setMaxIntensity( const double intensity )
			 * @brief sets max intensity of spectrum
			 * @param[in] intensity max intensity
			 */
			void setMaxIntensity( const double intensity );

			/**
			 * @fn double getMaxIntensity( const double minX = -1.0, const double maxX = -1.0 )
			 * @brief gets max intensity in specified range
			 * @param[in] minX minimum x of range (If minX is negative number, minimum x value is unbounded.)
			 * @param[in] maxX maximum x of range (If maxX is negative number, maximum x value is unbounded.)
			 * @return max intensity
			 */
			double getMaxIntensity( const double minX = -1.0, const double maxX = -1.0 );

			/**
			 * @fn void setBasePeakMass( const double mass )
			 * @brief sets base peak mass
			 * @param[in] mass base peak mass
			 */
			void setBasePeakMass( const double mass );

			/**
			 * @fn double getBasePeakMass()
			 * @brief gets base peak mass
			 * @return base peak mass
			 */
			double getBasePeakMass();

			/**
			 * @fn void setMsStage( const int stage )
			 * @brief sets ms stage
			 * @param[in] stage ms stage
			 */
			void setMsStage( const int stage );

			/**
			 * @fn int getMsStage();
			 * @brief gets ms stage
			 * @return ms stage
			 */
			int getMsStage();

			/**
			 * @fn void setPrecursor( const int stage, const double precursor )
			 * @brief sets precursor
			 * @param[in] stage MS stage of precursor ion spectrum.
			 * @param[in] precursor precursor mass
			 */
			void setPrecursor( const int stage, const double precursor );

			/**
			 * @fn void setPrecursor( const double precursor )
			 * @brief sets precursor
			 * @param[in] precursor precursor mass
			 */
			void setPrecursor( const double precursor );

			/**
			 * @fn double getPrecursor( const int stage )
			 * @brief gets precursor
			 * @param[in] stage MS stage of precursor ion spectrum
			 * @return precursor
			 */
			double getPrecursor( const int stage );

			/**
			 * @fn double getPrecursor()
			 * @brief gets the precursor mass of precursor ion spectrum.
			 * @return precursor mass
			 */
			double getPrecursor();

			/**
			 * @fn void setPrecursorIntensity( const int stage, const double intensity )
			 * @brief sets precursor intensity 
			 * @param[in] stage MS stage of precursor ion spectrum
			 * @param[in] intensity precursor intensity
			 */
			void setPrecursorIntensity( const int stage, const double intensity );

			/**
			 * @fn void setPrecursorIntensity( const double intensity )
			 * @brief sets precursor intensity 
			 * @param[in] intensity precursor intensity
			 */
			void setPrecursorIntensity( const double intensity );

			/**
			 * @fn double getPrecursorIntensity( const int stage )
			 * @brief gets the precursor intensity
			 * @param[in] stage MS stage of precursor ion spectrum
			 * @return precursor intensity
			 */
			double getPrecursorIntensity( const int stage );

			/**
			 * @fn double getPrecursorIntensity()
			 * @brief gets the precursor intensity of the precursor ion spectrum.
			 * @return the precursor intensity
			 */
			double getPrecursorIntensity();

			/**
			 * @fn void setPrecursorCharge( const int charge )
			 * @brief sets the precursor charge
			 * @param[in] charge precursor charge
			 */
			void setPrecursorCharge( const int charge );

			/**
			 * @fn int getPrecursorCharge()
			 * @brief gets the precursor charge
			 * @return precursor charge
			 */
			int getPrecursorCharge();

			/**
			 * @fn void setParentSpectrum( Spectrum* const parent )
			 * @brief sets parent spectrum
			 * @param[in] parent parent spectrum oebject. (If this spectrum has no parent, this parameter is NULL.)
			 */
			void setParentSpectrum( Spectrum* const parent );

			/**
			 * @fn Spectrum* getParentSpectrum()
			 * @brief gets parent spectrum
			 * @return parent spectrum. (If this spectrum has no parent, this method returns NULL.)
			 */
			Spectrum* getParentSpectrum();

			/**
			 * @fn void getChildSpectra( std::vector< Spectrum* >& children )
			 * @brief gets child spectra
			 * @param[out] children the array to store child spectra
			 */
			void getChildSpectra( std::vector< Spectrum* >& children );

			/**
			 * @fn void setHasChromatogram( const bool chromatogram )
			 * @brief sets wheher this spectrum has chromatogram
			 * @param[in] chromatogram whether this spectrum has chromatogram
			 */
			void setHasChromatogram( const bool chromatogram );

			/**
			 * @fn bool hasChromatogram()
			 * @brief judges whether this spectrum has chromatogram
			 * @return where this spectrum has chromatogram
			 */
			bool hasChromatogram();

			/**
			 * @fn void setGroup( DataGroupNode* group )
			 * @brief sets spectrum group
			 * @param[in] group spectrum group
			 */
			void setGroup( DataGroupNode* group );

			/**
			 * @fn DataGroupNode* getGroup()
			 * @brief gets spectrum group
			 * @return spectrum group
			 */
			DataGroupNode* getGroup();

			/**
			 * @fn void setScanNumber( const int scan )
			 * @brief sets scan number
			 * @param[in] scan scan number
			 */
			void setScanNumber( const int scan );

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
			 * @fn void setCentroidMode( const bool centroidMode )
			 * @brief sets centroid mode or not
			 * @param[in] centroidMode If true, this spectrum is centroid mode.
			 */
			void setCentroidMode( const bool centroidMode );

			/**
			 * @fn bool isCentroidMode()
			 * @brief judget wheter this spectrum is centroid mode
			 * @return If true, this spectrum is centroid mode.
			 */
			bool isCentroidMode();

			/**
			 * @fn void setResolution( const double resolution )
			 * @brief sets resolution
			 * @param[in] resolution resolution
			 */
			void setResolution( const double resolution );

			/**
			 * @fn double getResolution()
			 * @brief gets resolution
			 * @return resolution
			 */
			double getResolution();

			/**
			 * @fn void setCollisionEnergy( const char* collisionEnergy )
			 * @brief sets the collision energy
			 * @param[in] collisionEnergy collision energy
			 */
			void setCollisionEnergy( const char* collisionEnergy );

			/**
			 * @fn const char* getCollisionEnergy()
			 * @brief gets the collision energy
			 * @return collision energy
			 */
			const char* getCollisionEnergy();

			/**
			 * @fn void setVisible( const bool visible )
			 * @brief sets the visible flag
			 * @param[in] visible visible flag value
			 */
			void setVisible( const bool visible );

			/**
			 * @fn bool isVisible()
			 * @brief gets the visible flag value
			 * @return visible flag value
			 */
			 bool isVisible();

			 /**
			  * @fn void setAutoZeroPoints( const bool autoZero )
			  * @brief sets auto zero points flag value
			  * @param[in] autoZero auto zero points flag value.
			  */
			 void setAutoZeroPoints( const bool autoZero );

			 /**
			  * @fn bool isAutoZeroPoints()
			  * @brief gets auto zero points flag value
			  * @return auto zero points flag value
			  */
			 bool isAutoZeroPoints();

			/**
			 * @fn kome::core::Properties& getProperties()
			 * @brief gets properties
			 * @return properties
			 */
			kome::core::Properties& getProperties();

			/**
			 * @fn kome::core::Properties& getUserProperties()
			 * @brief gets user properties
			 * @return user properties
			 */
			kome::core::Properties& getUserProperties();

			/**
			 * @fn void setOrgSpectrum( Spectrum* spec )
			 * @brief sets original spectrum
			 * @param[in] spec orignal spectrum
			 */
			void setOrgSpectrum( Spectrum* spec );

			/**
			 * @fn Spectrum* getOrgSpectrum()
			 * @brief gets original spectrum
			 * @return original spectrum
			 */
			Spectrum* getOrgSpectrum();

			/**
			 * @fn void getProperties( kome::core::Properties& properties )
			 * @brief gets spectrum and spectrum group properties
			 * @param[out] properties properties object to store properties
			 */
			void getProperties( kome::core::Properties& properties );

			/**
			 * @fn void getUserProperties( kome::core::Properties& userProperties )
			 * @brief gets spectrum and spectrum group user properties
			 * @param[out] userProperties userProperties object to store user properties
			 */
			void getUserProperties( kome::core::Properties& userProperties );

			// >>>>>> @Date:2013/07/16	<Add>	A.Ozaki
			//
			/**
			 * @fn void	setSpotId( const char *pcSpotId )
			 * @brief set spot id
			 * param[in] pcSpotId SpotId information
			 */
			void	setSpotId( const char *pcSpotId );

			/**
			 * @fn const char	*getSpotId( void )
			 * @brief get spot id
			 * @return string of spot id
			 */
			const char	*getSpotId( void );
			//
			// <<<<<< @Date:2013/07/16	<Add>	A.Ozaki

		public:
			/**
			 * @fn static bool isCommonProperty( const char* key )
			 * @brief check whther the specified property key is common property or not
			 * @param[in] key parameter key
			 * @return If true, specified key is common property key
			 */
			static bool isCommonProperty( const char* key );

		protected:
			/**
			 * @fn virtual void onGetXYData( kome::core::XYData* const xyData, const double minX, const double maxX ) = 0
			 * @brief This method is called by getXYData method. (abstract method)
			 * @param[out] xyData the object to store data points
			 * @param[in] minX minimum x value. (If minX is negative number, minimum x value is not unbounded.)
			 * @param[in] maxX maximum x value. (If maxX is negative number, maximum x value is not unbounded.)
			 */
			virtual void onGetXYData( kome::core::XYData* const xyData, const double minX, const double maxX ) = 0;

			/**
			 * @fn virtual void onGetXRange( double* minX, double* maxX ) = 0
			 * @brief This method is called by getMinX or getMaxX method. (abstract method)
			 * @param[out] minX the pointer to store minimum x value
			 * @param[out] maxX the pointer to store maximum x value
			 */
			virtual void onGetXRange( double* minX, double* maxX ) = 0;

			/**
			 * @fn virtual double onGetTotalIntensity( const double minX, const double maxX ) = 0
			 * @brief This method is called by getTotalIntensity method. (abstract method)
			 * @param[in] minX minimum x of range (If minX is negative number, minimum x value is unbounded.)
			 * @param[in] maxX maximum x of range (If maxX is negative number, maximum x value is unbounded.)
			 * @return total intensity
			 */
			virtual double onGetTotalIntensity( const double minX, const double maxX ) = 0;

			/**
			 * @fn virtual double onGetMaxIntensity( const double minX, const double maxX ) = 0
			 * @brief This method is called by getMaxIntensity method. (abstract method)
			 * @param[in] minX minimum x of range (If minX is negative number, minimum x value is unbounded.)
			 * @param[in] maxX maximum x of range (If maxX is negative number, maximum x value is unbounded.)
			 * @return max intensity
			 */
			virtual double onGetMaxIntensity( const double minX, const double maxX ) = 0;

			// >>>>>> @Date:2013/07/16	<Add>	A.Ozaki
			//
			/**
			 * @fn virtual  bool onSetRequestLoadData( void )
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
			 * @fn virtual  bool onSetFirstAccess( void )
			 * @brief This method is called by setFirstAccess method. (abstract method)
			 */
			virtual void	onSetFirstAccess( void );

			/**
			 * @fn virtual  bool onResetFirstAccess( void )
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

			/**
			 * @fn bool	firstAccessProcess( void )
			 * @brief method for processing when accessing the first item of this class
			 * @return If true, file reading success.
			 */
			bool	firstAccessProcess( void );

		public:
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
			// <<<<<< @Date:2013/07/16	<Add>	A.Ozaki

		};
	}
}

#endif	// __KOME_OBJECTS_SPECTRUM_H__
