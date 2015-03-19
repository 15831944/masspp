/**
 * @file MascotManager.h
 * @brief interfaces of MascotManager class
 *
 * @author S.Tanaka
 * @date 2011.01.13
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_MASCOT_MASCOT_MANAGER_H__
#define __KOME_MASCOT_MASCOT_MANAGER_H__


#include <vector>
#include <string>
#include <map>


#define SECTION               "Mascot"

#define URL_KEY               "MASCOT_URL"
#define PROXY_KEY             "USE_PROXY"
#define USERNAME_KEY          "USERNAME"
#define PASSWORD_KEY          "PASSWORD"


#define DB_PARAM_NAME                        "DB"
#define TAXONOMY_PARAM_NAME                  "TAXONOMY"
#define USERNAME_PARAM_NAME	                 "USERNAME"
#define EMAIL_PARAM_NAME	                 "USEREMAIL"
#define ALLOWED_MISSED_CLEAVAGES_PARAM_NAME	 "PFA"
#define QUANTITATION_PARAM_NAME	             "QUANTITATION"
#define ENZYME_PARAM_NAME	                 "CLE"
#define MASS_PARAM_NAME	                     "MASS"
#define FIXEDMODS_PARAM_NAME	             "MODS"
#define VARIABLE_PARAM_NAME	                 "IT_MODS"
#define PEPTIDE_TOL_PARAM_NAME	             "TOL"
#define PEPTIDE_UNIT_PARAM_NAME	             "TOLU"
#define PEPTIDE_ISOTOPE_ERROR_PARAM_NAME	 "PEP_ISOTOPE_ERROR"
#define MSMS_TOL_PARAM_NAME	                 "ITOL"
#define MSMS_UNIT_PARAM_NAME	             "ITOLU"
#define PEPTIDE_CHARGE_PARAM_NAME	         "CHARGE"
#define INSTRUMENT_PARAM_NAME	             "INSTRUMENT"
#define ERROR_TOL_DECAY_PARAM_NAME	         "ErrorTolerant/Decoy"
#define DECOY_PARAM_NAME                     "DECOY"
#define REPORTNUM_PARAM_NAME	             "REPORT"
#define PROTEIN_MASS_PARAM_NAME              "SEG"
#define CHARGE_PARAM_NAME                    "CHARGE"


namespace kome {
	namespace mascot {

		/**
		 * @class MascotManager
		 * @brief mascot search management class
		 */
		class MASCOT_LIB_CLASS MascotManager {
		protected:
			/**
			 * @fn MascotManager()
			 * @brief constructor
			 */
			MascotManager();

			/**
			 * @fn virtual ~MascotManager()
			 * @brief destructor
			 */
			virtual ~MascotManager();

		public:
			/**
			 * @struct SearchPageInfo
			 * @brief search page information
			 */
			struct SearchPageInfo {
				std::vector< std::string > dbs;
				std::vector< std::string > taxonomies;
			};

		private:
			/** session */
			std::string m_sSession;

			/** username */
			std::string m_sUsername;

			/** userid */
			std::string m_sUserID;

			/** logged in server URL */
			std::string m_sLoggedInURL;

			/** logged in use proxy setting */
			bool m_bLoggedInUseProxy;

			/** search page information map */
			std::map< std::string, SearchPageInfo > m_pageMap;

			/** url */
			std::string m_url;

		public:
			/**
			 * @fn bool ServerLoginLogout(const bool bLogin)
			 * @brief login to / logout from Mascot server
			 * @return If true, it succeeded to log in / logout from Mascot server.
			 */
			bool ServerLoginLogout(const bool bLogin);
			
			/**
			 * @fn bool login()
			 * @brief login to Mascot server
			 * @return If true, it succeeded to log into Mascot server.
			 */
			bool login();

			/**
			 * @fn bool logout()
			 * @brief logout from Mascot server
			 * @return If true, it succeeded to log out from Mascot server.
			 */
			bool logout();

			/**
			 * @fn void setCookieHeader(kome::net::HttpConnection* con)
			 * @brief sets the Cookie header in the HttpConnection object
			 * @param[in, out] con HTTP connection
			 */
			void setCookieHeader(kome::net::HttpConnection* con);

			/**
			 * @fn bool getMascotParameters(std::string& strPath, const char* search)
			 * @brief gets mascot parameters
			 * @param[out] strPath output filename
			 * @param[in] search search type
			 * @return If true, it succeeded to get mascot parameters
			 */
			bool getMascotParameters(std::string& strPath, const char* search);

			/**
			 * @fn bool splitPeakList()
			 * @brief indicate whether to split the peak list for searching
			 * @return If true, the peak list should be split
			 */
			bool splitPeakList();

			/**
			 * @fn std::string executeSubSearch(
						const char* search,
						const char* title,
						const char* peaksFile,
						kome::objects::SettingParameterValues* settings,
						kome::core::Progress& progress,
						const char* outFile
					)
			 * @brief executes sub search
			 * @param[in] search search type
			 * @param[in] peaksFile peaks file
			 * @param[in] settings search parameters
			 * @param[out] progress the object to show the progress
			 * @param[in] outFile output file path
			 * @return location
			 */
			std::string executeSubSearch(
				const char* search,
				const char* title,
				const char* peaksFile,
				kome::objects::SettingParameterValues* settings,
				kome::core::Progress& progress,
				const char* outFile
			);

			/**
			 * @fn std::string convertPeptideChargeString( const char* peptideChargeValue );
			 * @brief convert the peptide charge string from one using colon separators to commas and "and"
			 * @param[in] peptideChargeValue colon-delimited peptide charge string
			 * @return comma-delimited peptide charge string
			 */
			std::string convertPeptideChargeString( const char* peptideChargeValue );

		public:
			/**
			 * @fn const char* getUrl()
			 * @brief gets mascot url
			 * @return mascot url
			 */
			const char* getUrl();

			/**
			 * @fn bool useProxy()
			 * @brief judges whther the proxy server is used in accessing mascot server
			 * @return If true, using proxy server to mascot server
			 */
			bool useProxy();

			/**
			 * @fn std::string getUsername()
			 * @brief gets mascot username
			 * @return mascot username
			 */
			std::string getUsername();

			/**
			 * @fn std::string getPassword()
			 * @brief gets mascot password
			 * @return mascot password
			 */
			std::string getPassword();

		protected:
			/**
			 * @fn void saveHeader( char* res, const unsigned int size, const char* file )
			 * @brief save the header of http response
			 * @param[in] res the pointer to the response
			 * @param[in] size the response size
			 * @param[in] file save file path
			 */
			void saveHeader( char* res, const unsigned int size, const char* file );

			/**
			 * @fn void saveBody( char* res, const unsigned int size, const char* file )
			 * @brief save the body of http response
			 * @param[in] res the pointer to the response
			 * @param[in] size the response size
			 * @param[in] file save file path
			 */
			void saveBody( char* res, const unsigned int size, const char* file );

		public:
			/**
			 * @fn bool getPageInfo( const char* search, SearchPageInfo& info, kome::core::Progress& progress )
			 * @b rief gets the page information
			 * @param[in] search search string
			 * @param[out] structure to store search page information
			 * @param[in,out] progress the object to show the progress
			 * @return If true, getting search page information finished successfully.
			 */
			bool getPageInfo( const char* search, SearchPageInfo& info, kome::core::Progress& progress );

		protected:
			/**
			 * @fn bool updateModifications()
			 * @brief updates modifications
			 * @return If true, modifications are updated successfully.
			 */
			bool updateModifications();

			/**
			 * @fn bool updateEnzymes()
			 * @brief updates enzymes
			 * @return If true, enzymes are updated successfully.
			 */
			bool updateEnzymes();

		public:
			/**
			 * @fn static MascotManager& getInstance()
			 * @brief gets object
			 * @return MascotManager object. (This is the only object.)
			 */
			static MascotManager& getInstance();
		};
	}
}

#endif	// __KOME_MASCOT_MASCOT_MANAGER_H__
