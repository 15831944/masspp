/**
 * @file IdentificationPanel.h
 * @brief interfaces of IdentificationPanel class
 *
 * @author M.Izumi
 * @date 2013.01.11
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_IDENT_EDENTIFICATION_PANEL_H__
#define __KOME_IDENT_EDENTIFICATION_PANEL_H__


namespace kome {
	namespace ident {
		
		class SpectrumGetter;
		class IdentificationHelpPanel;

		class IdentificationPanel : public kome::window::PeakMatrixSettingsPageBase {
		    public:
                /**
			     * @fn IdentificationPanel(
				            wxWizard* pParent, 
				            const char* name				           
                        )
			     * @brief constructor
			     * @param[in] pParent parent window
			     * @parma[in] name 
                 * @param[in] sample
			     */
			    IdentificationPanel(
				    wxWizard* pParent, 
				    const char* name				   
			    );

                /**
			     * @fn virtual ~IdentificationPanel()
			     * @brief destructor
			     */
			    virtual ~IdentificationPanel();
		    protected:

			    /** progress */
			    kome::core::Progress* m_progress;
			
			    /** title */
			    std::string m_title;

			    /** comment */
			    std::string m_comment;

			    /** search engine */
			    kome::ident::SearchEngine* m_engine;

			    /** search parameters */
			    kome::objects::SettingParameterValues* m_searchSettings;

			    /** search item */
			    kome::plugin::PluginFunctionItem* m_searchItem;

			    /** search settings flag */
			    bool m_searchFlg;

			    /** peak detector */
			    kome::plugin::PluginFunctionItem* m_peakDetector;

			    /** peak detection settings */
			    kome::objects::SettingParameterValues* m_peakSettings;

			    /** charge detector */
			    kome::plugin::PluginFunctionItem* m_chargeDetector;

			    /** charge settings */
			    kome::objects::SettingParameterValues* m_chargeSettings;

			    /** peak filter */
			    std::string m_peakFilter;

			    /** peak settings flag */
			    bool m_peakFlg;

			    /** charge settins flag */
			    bool m_chargeFlg;

			    /** spectrum getter */
			    SpectrumGetter* m_spectra;			
		
			    /** hepl panel */
			    IdentificationHelpPanel* m_helpPanel;

			    /** radio box book */
			    kome::window::RadioBoxBook* m_book;
							
			protected:
			    /**
			    * @fn virtual void createInnerSizer()
			    * @brief creates inner sizer
			    * @param InnerSizer inner sizer
			    */
			    virtual void createInnerSizer(wxStaticBoxSizer* InnerSizer);

			    /**
			     * @fn virtual bool TransferDataFromWindow()
			     * @brief transfer window control data to value
			     * @return If a transfer failed, this method returns false.
			     */
			    virtual bool TransferDataFromWindow();
	

		    public:
			    /**
			     * @fn virtual bool IsValid()
			     * @brief validate panel settings
			     * @return If validation failed, this method returns false.
			     */
			    virtual bool IsValid();

			    /**
			     * @fn virtual bool PreApplyValidationCheck(std::string& sError)
			     * @brief Perform Apply-time validation checks (i.e. checks that must not be done in IsValid function)
			     * @param sError error string from a single panel's pre-apply validation
			     * @return If validation succeeded, return true, otherwise false.
			     */
			    virtual bool PreApplyValidationCheck(std::string& sError);

			    /**
			     * @fn void SavePanelSettings(kome::core::Progress *pProgress)
			     * @brief This method is called when saving values.
			     * @param pProgress progress
			     * @return If saSavePanelSettingsving values failed, this method returns false.
			     */
			    virtual bool SavePanelSettings(kome::core::Progress *pProgress);

			    /**
			    * @fn void updateStandardSamples(bool bReadFromINIfile = true)
			    * @brief Update standard samples on this panel
			    * @param bReadFromINIfile whether to read from the INI file
			    */
			    virtual void updateStandardSamples(bool bReadFromINIfile = true);

			    /**
			    * @fn void RefreshPanelContentsForMatrix()
			    * @brief refresh panel contents for matrix
			    */
			    virtual void RefreshPanelContentsForMatrix();
			protected:
				/**
				 * @fn void writeAllChildParameters( kome::objects::SettingParameterValues& settings, kome::core::Properties& props, const char* prefix )
				 * @brief writes all child parameters
				 * @param[in] settings parameters information
				 * @param[out] props properties object to store parameters
				 * @param[in] prefix prefix
				 */
				void writeAllChildParameters( kome::objects::SettingParameterValues& settings, kome::core::Properties& props, const char* prefix );
			
		    protected:
			
			    /**
			     * @fn kome::ident::SearchEngine* getSearchEngine()
			     * @brief gets the search engine
			     * @return search engine
			     */
			    kome::ident::SearchEngine* getSearchEngine();						
			
			
			    /**
			     * @fp static bool identify()
			     * @brief identifies with dialog
			     * @return If true, identification is finished successfully.
			     */
			    bool identify();
			
			    /**
			     * @fn bool detectPeaks( kome::core::Progress* progress )
			     * @brief detect pekas
			     * @parma[out] progress the object to show the progress
			     * @return If true, peak detection is finished successfully.
			     */
			    bool detectPeaks( kome::core::Progress* progress );
			
			    /**
			     * @fn virtual SpectrumGetter* createSpectrumGetter()
			     * @brief creates the spectrum getter (override method)
			     * @return spectrum getter
			     */
			    SpectrumGetter* createSpectrumGetter();

		    private:
			    DECLARE_EVENT_TABLE();
		};

		
		class IdentificationHelpPanel : public kome::window::ParametersAndHelpPanel {
		    public:
                /**
                 * @fn IdentificationHelpPanel( wxWindow* parent )
                 * @brief contructor
                 * @param[in] parent parent window
                 */
			    IdentificationHelpPanel( wxWindow* parent );
		
                /**
			     * @fn virtual ~IdentificationHelpPanel()
			     * @brief destructor
			     */
			    virtual ~IdentificationHelpPanel();
		    protected:
			    /** parameters */
			    kome::plugin::ParameterSettings m_params;

			    /** settings */
			    kome::objects::SettingParameterValues m_settings;
		    protected:
			    /**
				    * @fn virtual wxSizer* createMainSizer()
				    * @brief creates main sizer
				    * @return main sizer
				    */
			    virtual wxSizer* createMainSizer();
				    /**
			     * @fn wxSizer* createSearchSizer()
			     * @brief creates search sizer
			     * @return search sizer
			     */
			    wxSizer* createSearchSizer();

			    /**
			     * @fn wxSizer* createSpectraSizer()
			     * @brief creates spectra sizer
			     * @return spectra sizer
			     */
			    wxSizer* createSpectraSizer();
			
			    /**
			     * @fn wxSizer* createSearchSizer()
			     * @brief creates search sizer
			     * @return search sizer
			     */
			    wxSizer* createPeaksSizer();

		    public:
			    /**
			     * @fn kome::plugin::PluginFunctionItem* getSearchItem()
			     * @brief gets the search engine function
			     * @return search engine function
			     */
			    kome::plugin::PluginFunctionItem* getSearchItem();

			    /**
			     * @fn std::string getTitle()
			     * @brief gets the title
			     * @return title
			     */
			    std::string getTitle();

			    /**
			     * @fn std::string getComment()
			     * @brief gets the comment
			     * @return comment
			     */
			    std::string getComment();

			    /**
			     * @fn kome::objects::SettingParameterValues* getSearchSettings()
			     * @brief gets the search settings 
			     * @return search settings
			     */
			    kome::objects::SettingParameterValues* getSearchSettings();

			    /**
			     * @fn kome::plugin::PluginFunctionItem* getPeakDetector()
			     * @brief gets the peak detector
			     * @return peak detector
			     */
			    kome::plugin::PluginFunctionItem* getPeakDetector();

			    /**
			     * @fn kome::plugin::PluginFunctionItem* getPeakDetector()
			     * @brief gets the peak detector settings
			     * @retrun peak detect settings
			     */
			    kome::objects::SettingParameterValues* getPeakDetectSettings();

			    /**
			     * @fn kome::plugin::PluginFunctionItem* getChargeDetector()
			     * @brief gets the charge detector
			     * @return charge detector
			     */
			    kome::plugin::PluginFunctionItem* getChargeDetector();

			    /**
			     * @fn kome::objects::SettingParameterValues* getChargeDetectSettings()
			     * @brief gets the charge detect settings
			     * @return charge detect settings
			     */
			    kome::objects::SettingParameterValues* getChargeDetectSettings();

			    /**
			     * @fn std::string getPeakFilter()
			     * @brief gets the peak filter
			     * @return peak filter
			     */
			    std::string getPeakFilter();

			    /**
			     * @fn double getRtTolerance()
			     * @brief gets the RT tolerance
			     * @return RT tolerance
			     */
			    double getRtTolerance();

			    /**
			     * @fp double getMzTolerance()
			     *@ brief gets the m/z tolerance
			     * @return m/z tolerance
			     */
			    double getMzTolerance();

		
		    private:
			    DECLARE_EVENT_TABLE();
		};
	}
}





#endif // __KOME_IDENT_EDENTIFICATION_PANEL_H__
