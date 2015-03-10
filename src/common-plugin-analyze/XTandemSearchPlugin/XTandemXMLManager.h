/**
 * @file XTandemXMLManager.h
 * @brief interfaces of XTandemXMLManager class
 *
 * @author N.tashiro
 * @date 2012.03.16
 * 
 * Copyright(C) 2012 Shimadzu SD Corporation. All rights reserved.
 */

#ifndef __KOME_XTANDEM_XTANDEM_XMLMANAGER_H__
#define __KOME_XTANDEM_XTANDEM_XMLMANAGER_H__

#include <string>

// XML Tag definitions
#define XTANDEM_XML_VERSION				"<?xml version=\"1.0\"?>"
#define XTANDEM_XML_STYLESHEET			"<?xml-stylesheet type=\"text/xsl\" href=\"tandem-input-style.xsl\"?>"
#define XTANDEM_XML_BIOML_START			"<bioml>"
#define XTANDEM_XML_BIOML_END			"</bioml>"
#define XTANDEM_XML_PATH_HEAD			"\t<note type=\"heading\">Paths</note>"
#define XTANDEM_XML_PATH_TAXONOMY		"\t<note type=\"input\" label=\"list path, taxonomy information\">%s</note>"
#define XTANDEM_XML_PATH_DEFAULT		"\t<note type=\"input\" label=\"list path, default parameters\">%s</note>"
#define XTANDEM_XML_PATH_RESIDUES		"\t<note type=\"input\" label=\"protein, modified residue mass file\">%s</note>"
#define XTANDEM_XML_SPEC_HEAD			"\t<note type=\"heading\">Spectrum general</note>"
#define XTANDEM_XML_SPEC_FRAG_ERR		"\t<note type=\"input\" label=\"spectrum, fragment monoisotopic mass error\">%f</note>"
#define XTANDEM_XML_SPEC_FRAG_UNIT		"\t<note type=\"input\" label=\"spectrum, fragment monoisotopic mass error units\">%s</note>"
#define XTANDEM_XML_SPEC_FRAG_TYPE		"\t<note type=\"input\" label=\"spectrum, fragment mass type\">%s</note>"
#define XTANDEM_XML_SPEC_PARENT_PLUS	"\t<note type=\"input\" label=\"spectrum, parent monoisotopic mass error plus\">%f</note>"
#define XTANDEM_XML_SPEC_PARENT_MINUS	"\t<note type=\"input\" label=\"spectrum, parent monoisotopic mass error minus\">%f</note>"
#define XTANDEM_XML_SPEC_PARENT_ISOTOPE	"\t<note type=\"input\" label=\"spectrum, parent monoisotopic mass isotope error\">%s</note>"
#define XTANDEM_XML_SPEC_PARENT_UNIT	"\t<note type=\"input\" label=\"spectrum, parent monoisotopic mass error units\">%s</note>"
#define XTANDEM_XML_SPEC_USE_NOISE		"\t<note type=\"input\" label=\"spectrum, use noise suppression\">%s</note>"
#define XTANDEM_XML_SPEC_USE_ANGLE		"\t<note type=\"input\" label=\"spectrum, use contrast angle\">%s</note>"
#define XTANDEM_XML_SPEC_USE_NEUTRAL	"\t<note type=\"input\" label=\"spectrum, use neutral loss window\">%s</note>"
#define XTANDEM_XML_SPEC_THREADS		"\t<note type=\"input\" label=\"spectrum, threads\">%d</note>"
#define XTANDEM_XML_SPEC_MAX_PARENT		"\t<note type=\"input\" label=\"spectrum, maximum parent charge\">%d</note>"
#define XTANDEM_XML_SPEC_MIN_PARENT		"\t<note type=\"input\" label=\"spectrum, minimum parent m+h\">%f</note>"
#define XTANDEM_XML_SPEC_MIN_FRAGMENT	"\t<note type=\"input\" label=\"spectrum, minimum fragment mz\">%f</note>"
#define XTANDEM_XML_SPEC_MIN_PEAKS		"\t<note type=\"input\" label=\"spectrum, minimum peaks\">%d</note>"
#define XTANDEM_XML_SPEC_BATCH_SIZE		"\t<note type=\"input\" label=\"spectrum, sequence batch size\">%d</note>"
#define XTANDEM_XML_SPEC_DYNAMIC_RANGE	"\t<note type=\"input\" label=\"spectrum, dynamic range\">%f</note>"
#define XTANDEM_XML_SPEC_ANGLE			"\t<note type=\"input\" label=\"spectrum, contrast angle\">%d</note>"
#define XTANDEM_XML_SPEC_NEUTRAL_WINDOW	"\t<note type=\"input\" label=\"spectrum, neutral loss window\">%f</note>"
#define XTANDEM_XML_SPEC_NEUTRAL_MASS	"\t<note type=\"input\" label=\"spectrum, neutral loss mass\">%f</note>"
#define XTANDEM_XML_SPEC_PATH			"\t<note type=\"input\" label=\"spectrum, path\">%s</note>"
#define XTANDEM_XML_RESI_HEAD			"\t<note type=\"heading\">Residue modification</note>"
#define XTANDEM_XML_RESI_MODIFICATION	"\t<note type=\"input\" label=\"residue, modification mass\">%s</note>"
#define XTANDEM_XML_RESI_POTENTIAL		"\t<note type=\"input\" label=\"residue, potential modification mass\">%s</note>"
#define XTANDEM_XML_PROT_HEAD			"\t<note type=\"heading\">Protein general</note>"
#define XTANDEM_XML_PROT_ANNOTATION		"\t<note type=\"input\" label=\"protein, use annotations\">%s</note>"
#define XTANDEM_XML_PROT_TAXON			"\t<note type=\"input\" label=\"protein, taxon\">%s</note>"
#define XTANDEM_XML_PROT_CLEAVAGE_SITE	"\t<note type=\"input\" label=\"protein, cleavage site\">%s</note>"
#define XTANDEM_XML_PROT_CLEAVAGE_SEMI	"\t<note type=\"input\" label=\"protein, cleavage semi\">%s</note>"
#define XTANDEM_XML_PROT_CLEAVAGE_CTERM	"\t<note type=\"input\" label=\"protein, cleavage C-terminal mass change\">%s%f</note>"
#define XTANDEM_XML_PROT_CLEAVAGE_NTERM	"\t<note type=\"input\" label=\"protein, cleavage N-terminal mass change\">%s%f</note>"
#define XTANDEM_XML_PROT_NTERM_RESIDUE	"\t<note type=\"input\" label=\"protein, N-terminal residue modification mass\">%f</note>"
#define XTANDEM_XML_PROT_CTERM_RESIDUE	"\t<note type=\"input\" label=\"protein, C-terminal residue modification mass\">%f</note>"
#define XTANDEM_XML_PROT_HOMOLOG		"\t<note type=\"input\" label=\"protein, homolog management\">%s</note>"
#define XTANDEM_XML_SCOR_HEAD			"\t<note type=\"heading\">Scoring</note>"
#define XTANDEM_XML_SCOR_IONS			"\t<note type=\"input\" label=\"scoring, %s ions\">%s</note>"
#define XTANDEM_XML_SCOR_MIN_ION		"\t<note type=\"input\" label=\"scoring, minimum ion count\">%d</note>"
#define XTANDEM_XML_SCOR_MAX_CLEAVAGE	"\t<note type=\"input\" label=\"scoring, maximum missed cleavage sites\">%d</note>"
#define XTANDEM_XML_SCOR_CYCLIC_PER		"\t<note type=\"input\" label=\"scoring, cyclic permutation\">%s</note>"
#define XTANDEM_XML_SCOR_INCLUDE		"\t<note type=\"input\" label=\"scoring, include reverse\">%s</note>"
#define XTANDEM_XML_REFI_HEAD			"\t<note type=\"heading\">model refinement paramters</note>"
#define XTANDEM_XML_REFI_REFINE			"\t<note type=\"input\" label=\"refine\">%s</note>"
#define XTANDEM_XML_REFI_MAX			"\t<note type=\"input\" label=\"refine, maximum valid expectation value\">%f</note>"
#define XTANDEM_XML_REFI_SYNTHESIS		"\t<note type=\"input\" label=\"refine, spectrum synthesis\">%s</note>"
#define XTANDEM_XML_REFI_TIC_PER		"\t<note type=\"input\" label=\"refine, tic percent\">%d</note>"
#define XTANDEM_XML_REFI_UN_CLEAVAGE	"\t<note type=\"input\" label=\"refine, unanticipated cleavage\">%s</note>"
#define XTANDEM_XML_REFI_NTERM			"\t<note type=\"input\" label=\"refine, potential N-terminus modifications\">%s%f@[</note>"
#define XTANDEM_XML_REFI_ANNOTATION		"\t<note type=\"input\" label=\"refine, use annotations\">%s</note>"
#define XTANDEM_XML_REFI_CLEAVAGE_SEMI	"\t<note type=\"input\" label=\"refine, cleavage semi\">%s</note>"
#define XTANDEM_XML_REFI_POTENTIAL		"\t<note type=\"input\" label=\"refine, potential modification mass\">%s</note>"
#define XTANDEM_XML_REFI_MUTATION		"\t<note type=\"input\" label=\"refine, point mutations\">%s</note>"
#define XTANDEM_XML_REFI_USE_POTENTIAL	"\t<note type=\"input\" label=\"refine, use potential modifications for full refinement\">%s</note>"

#define XTANDEM_XML_OUT_HEAD			"\t<note type=\"heading\">Output</note>"
#define XTANDEM_XML_OUT_PATH			"\t<note type=\"input\" label=\"output, path\">%s</note>"
#define XTANDEM_XML_OUT_ONE_COPY		"\t<note type=\"input\" label=\"output, one sequence copy\">%s</note>"
#define XTANDEM_XML_OUT_SORT_RESULTS	"\t<note type=\"input\" label=\"output, sort results by\">%s</note>"
#define XTANDEM_XML_OUT_PATH_HASHING	"\t<note type=\"input\" label=\"output, path hashing\">%s</note>"
#define XTANDEM_XML_OUT_PARAMETERS		"\t<note type=\"input\" label=\"output, parameters\">%s</note>"
#define XTANDEM_XML_OUT_PERFORMANCE		"\t<note type=\"input\" label=\"output, performance\">%s</note>"
#define XTANDEM_XML_OUT_SPECTRA			"\t<note type=\"input\" label=\"output, spectra\">%s</note>"
#define XTANDEM_XML_OUT_HISTOGRAMS		"\t<note type=\"input\" label=\"output, histograms\">%s</note>"
#define XTANDEM_XML_OUT_PROTEINS		"\t<note type=\"input\" label=\"output, proteins\">%s</note>"
#define XTANDEM_XML_OUT_SEQUENCE		"\t<note type=\"input\" label=\"output, sequences\">%s</note>"
#define XTANDEM_XML_OUT_RESULTS			"\t<note type=\"input\" label=\"output, results\">%s</note>"
#define XTANDEM_XML_OUT_MAX_VALID		"\t<note type=\"input\" label=\"output, maximum valid expectation value\">%f</note>"
#define XTANDEM_XML_OUT_HISTOGRAM_WIDTH	"\t<note type=\"input\" label=\"output, histogram column width\">%d</note>"
#define XTANDEM_XML_OUT_STYLE			"\t<note type=\"input\" label=\"output, xsl path\">%s</note>"

#define XTANDEM_XML_TAXON_START			"\t<taxon label=\"%s\">"
#define XTANDEM_XML_TAXON_END			"\t</taxon>"
#define XTANDEM_XML_TAXON_FILE			"\t\t<file format=\"peptide\" URL=\"%s\" />"

// XML File name definitions
#define XTANDEM_XML_PREFIX_DEFAULT		"xtandem_default_param"
#define XTANDEM_XML_PREFIX_TAXONOMY		"xtandem_taxonomy"
#define XTANDEM_XML_PREFIX_PARAMETER	"xtandem_parameter"
#define XTANDEM_XML_PREFIX_PEAK			"xtandem_peak"

// XML Parameter name definitions
#define TARGET_TAXONOMY_PARAM_NAME	"db files"
#define ENZYME_PARAM_NAME	"enzyme"
#define USE_ANNOTATION_PARAM_NAME	"use annotations"
#define SEMI_STYLE_CLEAVAGE_PARAM_NAME	"semi-style cleavage"
#define FRAGMENT_MASS_ERROR_PARAM_NAME "fragment mass error"
#define FRAGMENT_MASS_ERROR_UNIT_PARAM_NAME "fragment mass error unit"
#define REMOVE_REDUNDANT_ANGLE_PARAM_NAME	"remove redundant angle"
#define NEUTRAL_LOSS_MASS_PARAM_NAME	"neutral loss mass"
#define NEUTRAL_LOSS_WINDOW_PARAM_NAME	"neutral loss window"
#define PARENT_MONOISOTOPIC_MASS_ERROR_MINUS_PARAM_NAME	"parent monoisotopic mass error minus"
#define PARENT_MONOISOTOPIC_MASS_ERROR_PLUS_PARAM_NAME	"parent monoisotopic mass error plus"
#define PARENT_MONOISOTOPIC_MASS_ERROR_UNIT_PARAM_NAME	"parent monoisotopic mass error unit"
#define IONS_PARAM_NAME	"ions"
#define MAX_MISSED_CLEAVAGES   "missed cleavages"
#define COMPLETE_MODIFICATIONS_PARAM_NAME	"complete modifications"
#define POTENTIAL_MODIFICATIONS_PARAM_NAME	"potential modifications"
#define USE_REFINEMENT						"use refinement"		// @date 2013.07.18 <Add> M.Izumi
#define REFINEMENT_POTENTIAL_MODIFICATIONS_PARAM_NAME	"refinement potential modifications"
#define REFINEMENT_USE_ANNOTATION_PARAM_NAME	"refinement use annotations"
#define REFIMENT_SEMI_STYLE_CLEAVAGE_PARAM_NAME	"refinement semi-style cleavage"
#define CHECK_POINT_MUTATIONS_PARAM_NAME	"check point mutations"
#define VALID_EXPECTION_VALUE_PARAM_NAME	"valid expectation value"
#define MAX_VALID_EXPECTION_VALUE_PARAM_NAME	"max valid expectation value"


namespace kome {
	namespace xtandem {
		class XTandemXMLManager {
		protected:
				/**
				 * @fn XTandemXMLManager()
				 * @brief constructor
				 */
				XTandemXMLManager();

				/**
				 * @fn virtual ~XTandemXMLManager()
				 * @brief destructor
				 */
				virtual ~XTandemXMLManager();

			public:
				/**
				 * @fn bool writeTaxonomyListXML(const char* dbName, const char* path)
				 * @brief write Taxonomy List File for X! Tandem 
				 * @param[in] dbName Database name
				 * @param[in] path output file path
				 * @return If true, it succeeded to write XML file.
				 */
				bool createTaxonomyListXML( const char* dbName, const char* path );

				/**
				 * @fn bool createParameterXML(
							kome::objects::SettingParameterValues* paramSettings,
							const char* path,
							const char* defaultFile,
							const char* peaksFile,
							const char* residuesFile,
							const char* resultFile
						)
				 * @brief write Parameter File for X! Tandem 
				 * @param[in] paramSettings identification parameters
				 * @param[in] path input xml file path
				 * @param[in] defaultFile default parameters file path
				 * @param[in] peaksFile peaks file path
				 * @param[in] residuesFile residues file
				 * @param[in] resultFile result file path
				 * @return If true, it succeeded to write XML file.
				 */
				bool createParameterXML(
						kome::objects::SettingParameterValues* paramSettings,
						const char* path,
						const char* defaultFile,
						const char* peaksFile,
						const char* residuesFile,
						const char* resultFile
				);

				/**
				 * @fn const char* translateTFtoYN(bool target)
				 * @brief translate true or false to yes or no
				 * @param[in] target true or false
				 * @return yes or no
				 */
				const char* translateTFtoYN(bool target);

			public:
				/**
				 * @fn static std::string getModificationString( const char* name )
				 * @breif gets the modification list string
				 * @param[in] name modification name
				 * @return modification string
				 */
				static std::string getModificationString( const char* name );

			public:
				/**
				 * @fn static XTandemXMLManager& getInstance()
				 * @brief gets object
				 * @return XTandemXMLManager object. (This is the only object.)
				 */
				static XTandemXMLManager& getInstance();
		};
	}
}

#endif	// __KOME_XTANDEM_XTANDEM_XMLMANAGER_H__
