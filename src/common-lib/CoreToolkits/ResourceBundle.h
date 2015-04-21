/**
 * @file ResourceBundle.h
 * @brief interfarces of ResourceBundle class
 *
 * @author S.Tanaka
 * @date 2009.04.14
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CORE_RESOURCE_BUNDLE_H__
#define __KOME_CORE_RESOURCE_BUNDLE_H__


namespace kome {
	namespace core {

		/**
		 * @class ResourceBundle
		 * @brief resource bundle class
		 */
		class CORE_TOOLKITS_CLASS ResourceBundle {
		protected:
			/**
			 * @fn ResourceBundle()
			 * @brief constructor
			 */
			ResourceBundle();

			/**
			 * @fn virtual ~ResourceBundle()
			 * @brief destructor
			 */
			virtual ~ResourceBundle();

		public:
			/**
			 * @fn std::string searchResource( const char* name, const char* module, const char* ext )
			 * @brief searches resource file
			 * @param[in] name resource name
			 * @param[in] module module name
			 * @param[in] ext file extension
			 * @return resource file path
			 */
			std::string searchResource( const char* name, const char* module, const char* ext );

		protected:
			/**
			 * @fn std::string searchResource(
						const char* path,
						const char* name,
						const char* module,
						const char* ext
					)
			 * @brief searches resource file
			 * @param[in] path search path
			 * @param[in] name resource name
			 * @param[in] module module name
			 * @param[in] ext file extension
			 * @return resource file path
			 */
			std::string searchResource(
				const char* path,
				const char* name,
				const char* module,
				const char* ext
			);


		public:
			/**
			 * @fn static ResourceBundle& getInstance()
			 * @brief gets resource bundle object (This is the only object)
			 * @return resource bundle object
			 */
			static ResourceBundle& getInstance();
		};
	}
}

#endif	// __KOME_CORE_RESOURCE_BUNDLE_H__
