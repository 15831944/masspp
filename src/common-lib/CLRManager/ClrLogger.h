/**
 * @file ClrLogger.h
 * @brief interfaces of ClrLogger class
 *
 * @author S.Tanaka
 * @date 2006.10.17
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_LOGGER_H__
#define __KOME_CLR_LOGGER_H__


namespace kome {
	namespace clr {

		/**
		 * @class ClrLogger
		 * @brief log management class on CLR environment
		 */
		public ref class ClrLogger {
		protected:
			/**
			 * @fn ClrLogger()
			 * @brief constructor
			 */
			ClrLogger();

			/**
			 * @fn virtual ~ClrLogger()
			 * @brief destructor
			 */
			virtual ~ClrLogger();

		protected:
			/** log level */
			ENUM_CLASS Level {
				NONE =  0,
				FATAL = 1,
				ERROR = 2,
				WARN  = 3,
				INFO  = 4,
				DEBUG = 5,
				TRACE = 6
			};

		public:
			/**
			 * @fn void setLogLevel( Level level )
			 * @brief sets log level
			 * @param[in] level log level
			 */
			void setLogLevel( Level level );

			/**
			 * @fn Level getLogLevel()
			 * @brief gets log level
			 * @return log level
			 */
			Level getLogLevel();

			/**
			 * @fn void fatal( System::String^ msg )
			 * @brief outputs fatal log
			 * @param[in] msg log message
			 */
			void fatal( System::String^ msg );

			/**
			 * @fn void error( System::String^ msg )
			 * @brief outputs error log
			 * @param[in] msg log message
			 */
			void error( System::String^ msg );

			/**
			 * @fn void warn( System::String^ msg )
			 * @brief outputs warning log
			 * @param[in] msg log message
			 */
			void warn( System::String^ msg );

			/**
			 * @fn void info( System::String^ msg )
			 * @brief outputs information log
			 * @return msg log message
			 */
			void info( System::String^ msg );

			/**
			 * @fn void debug( System::String^ msg )
			 * @brief outputs debug log
			 * @param[in] msg log message
			 */
			void debug( System::String^ msg );

			/**
			 * @fn void trace( System::String^ msg )
			 * @brief outputs trace log
			 * @param[in] msg log message
			 */
			void trace( System::String^ msg );

			/**
			 * @fn void fatalCode( System::String^ msg, int code )
			 * @brief outputs fatal log and sets error code			 
			 * @param[in] msg log message
			 * @param[in] code error code
			 */
			void fatalCode( System::String^ msg, int code );

			/**
			 * @fn void errorCode( System::String^ msg, int code )
			 * @brief outputs error log and sets error code
			 * @param[in] msg log message
			 * @param[in] code error code
			 */
			void errorCode( System::String^ msg, int code );

			/**
			 * @fn void warnCode( System::String^ msg, int code )
			 * @brief outputs warning log and sets error code
			 * @param[in] msg log message
			 * @param[in] code error code
			 */
			void warnCode( System::String^ msg, int code );

			/**
			 * @fn void infoCode( System::String^ msg, int code )
			 * @brief outputs information log and sets error code
			 * @param[in] msg log message
			 * @param[in] code error code
			 */
			void infoCode( System::String^ msg, int code );

			/**
			 * @fn void debugCode( System::String^ msg, int code )
			 * @brief outputs debug log and sets error code
			 * @param[in] msg log message
			 * @param[in] code error code
			 */
			void debugCode( System::String^ msg, int code );

			/**
			 * @fn void traceCode( System::String^ msg, int code )
			 * @brief outputs trace log and sets error code
			 * @param[in] msg log message
			 * @param[in] code error code
			 */
			void traceCode( System::String^ msg, int code );

			/**
			 * @fn int getLastErrorCode()
			 * @brief gets last error code
			 * @return last error code
			 */
			int getLastErrorCode();

			/**
			 * @fn System::String^ getLastErrorMessage()
			 * @brief gets last error message
			 * @return last error message
			 */
			System::String^ getLastErrorMessage();

			/**
			 * @fn int getErrorCode( System::String^ name )
			 * @brief gets error code
			 * @param[in] name error name
			 * @return error code
			 */
			int getErrorCode( System::String^ name );

		protected:
			/**
			 * @fn void outputLog( System::String^ msg, Level level )
			 * @brief outputs log
			 * @param[in] msg log message
			 * @param[in] level log level
			 */
			void outputLog( System::String^ msg, Level level );

			/**
			 * @fn void outputLogCode( System::String^ msg, int code, Level level )
			 * @brief outputs log and set error code
			 * @param[in] msg log message
			 * @param[in] code error code
			 * @param[in] level level
			 */
			void outputLogCode( System::String^ msg, int code, Level level );

		protected:
			/** This is the only object. */
			static ClrLogger^ m_logger;

		public:
			/**
			 * @fn static ClrLogger^ getInstance()
			 * @brief gets object
			 * @return ClrLogger object (This is the only object)
			 */
			static ClrLogger^ getInstance();
		};
	}
}

#endif	// __KOME_CLR_LOGGER_H__
