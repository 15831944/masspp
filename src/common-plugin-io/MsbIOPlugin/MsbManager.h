/**
 * @file MsbManager.h
 * @brief interfaces of MabManager class
 *
 * @author S.Tanaka
 * @date 2006.10.05
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_MSB_MANAGER_H__
#define __KOME_IO_MSB_MANAGER_H__


#include <vector>
#include <boost/any.hpp>


#define GET_DATA_SIZE( data )			   getSize( data, sizeof( data ) / sizeof( DataValue ) )
#define READ_FROM_FILE( data )			  readFromFile( data, sizeof( data ) / sizeof( DataValue ) )
#define READ_FROM_BUFFER( data, buffer )	readFromBuffer( data, sizeof( data ) / sizeof( DataValue ), buffer )
#define WRITE_TO_FILE( data )			   writeToFile( data, sizeof( data ) / sizeof( DataValue ) )
#define WRITE_TO_BUFFER( data, buffer )	 writeToBuffer( data, sizeof( data ) / sizeof( DataValue ), buffer )

#define SOURCE_FILE_PATH_PROP_NAME	"source file path"
#define SOURCE_FILE_NAME_PROP_NAME	"source file name"


namespace kome {
	namespace io {
		namespace msb {

			class MsbSample;
			class SpectrumPartArray;

			/**
			 * @class MsbManager
			 * @brief msb file IO manager class
			 */
			class MsbManager {
			public:
				/**
				 * @fn MsbManager()
				 * @brief constructor
				 */
				MsbManager();
	
				/**
				 * @fn virtual ~MsbManager()
				 * @brief destructor
				 */
				virtual ~MsbManager();
	
			public:
				/**
				 * @typedef MsbString
				 * @brief string position of the string table
				 */
				typedef unsigned int MsbString;

				/**
				 * @typedef ValueType
				 * @brief value type definition
				 */
				typedef enum {
					TYPE_BOOL,
					TYPE_CHAR,
					TYPE_UCHAR,
					TYPE_SHORT,
					TYPE_USHORT,
					TYPE_LONG,
					TYPE_ULONG,
					TYPE_LONGLONG,
					TYPE_ULONGLONG,
					TYPE_FLOAT,
					TYPE_DOUBLE,
					TYPE_TIME,
					TYPE_STRING
				} ValueType;

				/**
				 * @struct DataValue
				 * @brief data value to save
				 */
				class DataValue {
				public:
					/**
					 * @fn DataValue( ValueType t, boost::any v );
					 * @brief constructor
					 * @param t data type
					 * @param v value
					 */
					DataValue( ValueType t, boost::any v );

					/**
					 * @fn virtual ~DataValue()
					 * @brief destructor
					 */
					virtual ~DataValue();

				public:
					/** value type */
					ValueType type;

					/** value */
					union {
						bool				 b;
						char				 c;
						unsigned char	   uc;
						short				s;
						unsigned short	  us;
						int				 l;
						unsigned int	   ul;
						long long		   ll;
						unsigned long long ull;
						float				f;
						double			   d;
						time_t			   t;
						MsbString		   ms;
					} val;
				};

			protected:
				/**
				 * @struct SpectrumPart
				 * @brief spectrum part information
				 */
				struct SpectrumPartInfo {
					unsigned int spectrum;
					unsigned int length;
					float start;
					float end;
					float totalIntensity;
					float minIntensity;
					float maxIntensity;
					unsigned long long msAlign;
					unsigned long long intAlign;
				};

				/**
				 * @struct ChromatogramDataInfo
				 * @brief chromatogram data information
				 */
				struct ChromatogramDataInfo {
					kome::objects::Chromatogram* chrom;
					unsigned int length;
					unsigned long long rtAlign;
					unsigned long long intAlign;
					unsigned long long massAlign;
					unsigned long long msStageAlign;
					unsigned long long precursorAlign;
				};

				/**
				 * @struct Prop
				 * @brief property data
				 */
				struct Prop {
					unsigned int id;
					MsbString key;
					MsbString value;
				};

			public:
				/**
				 * @fn void setEndian( unsigned char endian )
				 * @brief sets endian
				 * @param endian (0: Big Endian, 1: Little Endian)
				 */
				void setEndian( unsigned char endian );

				/**
				 * @fn void readSampleInfo( MsbSample& sample )
				 * @brief reads sample information
				 * @param sample msb sample 
				 */
				void readSampleInfo( MsbSample& sample );

				/**
				 * @fn void readMsb( MsbSample& sample )
				 * @brief reads msb sample
				 * @param sample msb sample object to store data
				 */
				void readMsb( MsbSample& sample );

				/**
				 * @fn bool writeMsb(
							const char* path,
							kome::objects::DataSet& dataSet,
							bool op,
							kome::core::Progress& progress
					)
				 * @brief writes file
				 * @param path file path
				 * @param spectra spectra informations to save
				 * @param op If true, writing operated spectrum data
				 * @param progress progress object to tell progress
				 * @return If it succeeded to save the file, this method returns true.
				 */
				bool writeMsb(
						const char* path,
						kome::objects::DataSet& dataSet,
						bool op,
						kome::core::Progress& progress
				);

				/**
				 * @fn void closeFile()
				 * @brief close file
				 */
				void closeFile();

			protected:
				/** string table map */
				std::map< std::string, unsigned int > m_stringMap;

				/**
				 * @fn MsbString st( const char* s )
				 * @brief add character string to string table
				 * @param s character string to add
				 * @return position of string table
				 */
				MsbString st( const char* s );

				/**
				 * @fn unsigned int writeStringTable()
				 * @brief writes string table
				 * @return compressed string table size
				 */
				unsigned int writeStringTable();

			protected:
				/** string table */
				std::vector< char > m_stringTable;

				/** file descriptor */
				FILE* m_fp;

				/** reverse flag */
				bool m_reverse;

				/** big endian flag */
				static const bool m_bigendian;

				/** part range */
				static const float m_partRange;

			public:
				/**
				 * @fn void setFile( FILE* file )
				 * @brief sets file descriptor
				 * @param file file descriptor
				 */
				void setFile( FILE* file );

				/**
				 * @fn FILE* getFile()
				 * @brief gets file descriptor
				 * @return file descriptor
				 */
				FILE* getFile();

			protected:
				/**
				 * @fn void readFromFile( DataValue* values, const unsigned int size )
				 * @brief reads data
				 * @param values data value array to store
				 * @param size data value array size
				 */
				void readFromFile( DataValue* values, const unsigned int size );

				/**
				 * @fn void readFromBuffer( DataValue* values, const unsigned int size, void* buffer )
				 * @brief gets data from buffer
				 * @param values data value array to store
				 * @param size array size
				 * @param buffer data buffer
				 */
				void readFromBuffer( DataValue* values, const unsigned int size, void* buffer );

				/**
				 * @fn void writeToFile( DataValue* values, const unsigned int size )
				 * @brief writes data
				 * @param values data value array to write
				 * @param size array size
				 */
				void writeToFile( DataValue* values, const unsigned int size );

				/**
				 * @fn void writeToBuffer( DataValue* values, const unsigned int size, void* buffer )
				 * @brief writes data to buffer
				 * @param values data value array to write
				 * @param size array size
				 * @param buffer buffer
				 */
				void writeToBuffer( DataValue* values, const unsigned int size, void* buffer );

			protected:
				/**
				 * @fn static void getGroups(
							kome::objects::DataSet& dataSet,
							std::vector< kome::objects::DataGroupNode* >& groups
					)
				 * @brief gets spectrum groups
				 * @param spectra spectra
				 * @param groups the array to store spectrum group information
				 */
				static void getGroups(
					kome::objects::DataSet& dataSet,
					std::vector< kome::objects::DataGroupNode* >& groups
				);

				/**
				 * @fn static void getGroupArray(
						std::vector< kome::objects::DataGroupNode* >& groupArray,
						kome::objects::DataGroupNode* parentGroup
					)
				 * @brief gets spectrum group array
				 * @param groupArray spectrum group array object to store groups
				 * @param parentGroup parent spectrum group
				 */
				static void getGroupArray(
					std::vector< kome::objects::DataGroupNode* >& groupArray,
					kome::objects::DataGroupNode* parentGroup
				);

				/**
				 * @fn static void getGroupSet(
						std::set< kome::objects::DataGroupNode* >& groupSet,
						kome::objects::DataSet& dataSet
					)
				 * @brief gets spectrum group set
				 * @param groupSet spectrum group set object to store groups
				 * @param spectra spectra
				 */
				static void getGroupSet(
					std::set< kome::objects::DataGroupNode* >& groupSet,
					kome::objects::DataSet& dataSet
				);

				/**
				 * @fn static unsigned int getSize( DataValue* values, const unsigned int size )
				 * @brief gets total size
				 * @param values data values array
				 * @param size array size
				 * @return total data size
				 */
				static unsigned int getSize( DataValue* values, const unsigned int size );

				/**
				 * @fn static void getPosition( DataValue& value, void** p, unsigned int* size )
				 * @brief get position and size
				 * @param value data value
				 * @param p pointer to store the pointer to data
				 * @param size pointer to store data size
				 */
				static void getPosition( DataValue& value, void** p, unsigned int* size );

			protected:
				/**
				 * @fn static void getParts( kome::core::XYData& dps, SpectrumPartArray& arr )
				 * @brief gets spectrum parts
				 * @param dps spectrum data points
				 * @param arr the array to store spectrum parts
				 */
				static void getParts( kome::core::XYData& dps, SpectrumPartArray& arr );

			public:
				/**
				 * @fn static void getFormatVersion(
						unsigned short* major,
						unsigned short* minor,
						unsigned char* endian,
						FILE* fp
					)	
				 * @brief get file format version
				 * @param major the pointer to store major version
				 * @param minor the pointer to store minor version
				 * @param endian the pointer to store endian
				 * @param fp file descriptor
				 */
				static void getFormatVersion(
					unsigned short* major,
					unsigned short* minor,
					unsigned char* endian,
					FILE* fp
				);

				/**
				 * @fn static MsbManager* getMsbManager(
							const unsigned short majorVersion,
							const unsigned short minorVersion,
							const unsigned char endian,
							FILE* fp
						)
				 * @brief gets msb manager specified version
				 * @param majorVersion major version
				 * @param minorVersion minor version
				 * @param endian (0: Big Endian, 1: Little Endian)
				 * @param fp file descriptor
				 * @return msb manager object (If specified version is not supported, this method returns NULL.)
				 */
				static MsbManager* getMsbManager(
						const unsigned short majorVersion,
						const unsigned short minorVersion,
						const unsigned char endian,
						FILE* fp
				);

				/**
				 * @fn static float getPartRange()
				 * @brief gets part range
				 * @return part range
				 */
				static float getPartRange();

			protected:
				/**
				 * @fn virtual void onReadSampleInfo( MsbSample& sample ) = 0
				 * @brief This mehtod is called by readSampleInfo methjod. (abstract method)
				 * @param sample msb sample object
				 */
				virtual void onReadSampleInfo( MsbSample& sample ) = 0;

				/**
				 * @fn virtual void onReadMsb( MsbSample& sample ) = 0
				 * @brief This method is called by readMsb method. (abstract method)
				 * @param sample msb sample object
				 */
				virtual void onReadMsb( MsbSample& sample ) = 0;

				/**
				 * @fn virtual bool onWriteMsb(
						kome::objects::Sample& sample,
						kome::objects::DataSet& dataSet,
						bool op,
						kome::core::Progress& progress
					) = 0
				 * @brief This method is called by writeMsb method. (abstract method)
				 * @param sample sample
				 * @param spectra spectrum informations to save
				 * @param op If true, writing operated spectrum data
				 * @param progress progress object
				 * @return If it succeeded to save file, this method returns true.
				 */
				virtual bool onWriteMsb(
					kome::objects::Sample& sample,
					kome::objects::DataSet& dataSet,
					bool op,
					kome::core::Progress& progress
				) = 0;
			};
		}
	}
}

#endif		//  __KOME_IO_MSB_MANAGER_H__
