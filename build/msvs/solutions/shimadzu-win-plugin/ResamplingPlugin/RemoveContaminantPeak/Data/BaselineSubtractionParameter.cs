/// <summary>
/// [FILE] BaselineSubtractionParameter.cs
/// [ABSTRACT] Resampling plugin - Parameters Class of baseline subtraction.
/// Copyright (C) 2013-08-09 Shimadzu
/// </summary>

using kome.clr;
using System;
using System.Diagnostics;
namespace ResamplingPlugin.RemoveContaminantPeak.Data
{
    /// <summary>
    /// Parameters Class of baseline subtraction.
    /// </summary>
    public class BaselineSubtractionParameter
    {
        #region --- Defines --------------------------------------------

        /// <summary>
        /// Baseline subtraction algorithm : ABC.
        /// </summary>
        public const string ALGORITHM_ABC = "abc";

        /// <summary>
        /// Baseline subtraction algorithm : Linear.
        /// </summary>
        public const string ALGORITHM_LINEAR = "linear";

        /// <summary>
        /// String of section name in .ini file.
        /// </summary>
        protected const string PARAMETERS_INI_SECTION_NAME = "Remove Contaminant Peaks";

        /// <summary>
        /// String of Baseline in .ini file.
        /// </summary>
        protected const string PARAMETERS_INI_BASELINE = "BASELINE";

        /// <summary>
        /// String of ABC baseline window width in .ini file.
        /// </summary>
        protected const string PARAMETERS_INI_ABC_BASELINE_WINDOW_WIDTH = "ABC_WINDOW_WIDTH";

        /// <summary>
        /// String of ABC baseline window width in .ini file.
        /// </summary>
        protected const string PARAMETERS_INI_LINEAR_BASELINE_WINDOW_WIDTH = "LINEAR_WINDOW_WIDTH";

        /// <summary>
        /// String of noise fator in .ini file.
        /// </summary>
        protected const string PARAMETERS_INI_NOISE_FACTOR = "NOISE_FACTOR";

        /// <summary>
        /// String of smoothing width in .ini file.
        /// </summary>
        protected const string PARAMETERS_INI_SMOOTHING_WIDTH = "SMOOTHING_WIDTH";

        /// <summary>
        /// Default value of window width of abc. 
        /// </summary>
        protected const double DEFAULT_ABC_WINDOW_WIDTH = 0.1;

        /// <summary>
        /// Default value of window width of linear.
        /// </summary>
        protected const double DEFAULT_LINEAR_WINDOW_WIDTH = 2.0;

        /// <summary>
        /// Default value of noise factor.
        /// </summary>
        protected const double DEFAULT_NOISE_FACTOR = 3.0;

        /// <summary>
        /// Default value of smoothing width.
        /// </summary>
        protected const int DEFAULT_SMOOTHING_WIDTH = 10;

        /// <summary>
        /// Error message for invalid Baseline.
        /// </summary>
        protected const string ERROR_INVALID_BASELINE = "Invalid BaseLine.\n'ABC BaseLine' was selected as a default.";

        /// <summary>
        /// Error message for invalid Window Width(ABC).
        /// </summary>
        protected const string ERROR_INVALID_ABC_WINDOW_WIDTH = "Invalid Window Width(ABC BaseLine).\n'0.1' was selected as a default.";

        /// <summary>
        /// Error message for invalid Window Width(Linear).
        /// </summary>
        protected const string ERROR_INVALID_LINEAR_WINDOW_WIDTH = "Invalid Window Width(Linear BaseLine).\n'2.0' was selected as a default.";

        /// <summary>
        /// Error message for invalid Noise Factor.
        /// </summary>
        protected const string ERROR_INVALID_NOISE_FACTOR = "Invalid Noise Factor.\n3.0' was selected as a default.";

        /// <summary>
        /// Error message for invalid Smoothing Width.
        /// </summary>
        protected const string ERROR_INVALID_SMOOTHING_WIDTH = "Invalid Smoothing Width.\n'10' was selected as a default.";

        #endregion

        #region --- Variables ------------------------------------------

        /// <summary>
        /// ABC window width.
        /// </summary>
        private double _abcWindowWidth;

        /// <summary>
        /// Linear window width.
        /// </summary>
        private double _linearWindowWidth;

        /// <summary>
        /// Noise factor.
        /// </summary>
        private double _noiseFactor;

        /// <summary>
        /// Smoothing width.
        /// </summary>
        private int _smoothingWidth;

        /// <summary>
        /// Algorithm of baseline subtraction.
        /// </summary>
        private string _algorithm;

        #endregion

        #region --- Construction ---------------------------------------

        public BaselineSubtractionParameter()
        {

        }

        #endregion

        #region --- Properties -----------------------------------------

        /// <summary>
        /// Gets and sets a value indicating window width of abc.
        /// </summary>
        public double ABCWindowWidth
        {
            get
            {
                return _abcWindowWidth;
            }
            set
            {
                if (0 <= value)
                {
                    _abcWindowWidth = value;
                }
            }
        }

        /// <summary>
        /// Gets and sets a value indicating window width of linear.
        /// </summary>
        public double LinearWindowWidth
        {
            get
            {
                return _linearWindowWidth;
            }
            set
            {
                if (0 <= value)
                {
                    _linearWindowWidth = value;
                }
            }
        }

        /// <summary>
        /// Gets and sets a value indicating noise factor.
        /// </summary>
        public double NoiseFactor
        {
            get
            {
                return _noiseFactor;
            }
            set
            {
                if (0 <= value)
                {
                    _noiseFactor = value;
                }
            }
        }

        /// <summary>
        /// Gets and sets a value indicating smooth width.
        /// </summary>
        public int SmoothingWidth
        {
            get
            {
                return _smoothingWidth;
            }
            set
            {
                if (0 <= value)
                {
                    _smoothingWidth = value;
                }
            }
        }

        /// <summary>
        /// Gets and sets a text indicating algorithm of baseline subtraction.
        /// </summary>
        public string Algorithm
        {
            get
            {
                if (_algorithm == null)
                {
                    return ALGORITHM_ABC;
                }
                return _algorithm;
            }
            set
            {
                switch (value)
                {
                    case ALGORITHM_ABC:
                    case ALGORITHM_LINEAR:
                        _algorithm = value;
                        return;
                    default:
                        break;
                }
            }
        }

        //Gets and sets a ErrorMessage.
        public string ErrorMessage
        {
            get;
            set;
        }

        #endregion

        #region --- Public Methods -------------------------------------


        /// <summary>
        /// Read INI file.
        /// </summary>
        /// <returns></returns>
        public string ReadParameterIni()
        {
            ClrMsppManager msObj = ClrMsppManager.getInstance();
            ClrIniFile ini = msObj.getParameters();

            uint sectionNum = ini.getNumberOfSections();
            uint parameterNum;
            string parameterName;
            bool sectionFindFlag = false;
            string errorMessage = string.Empty;

            try
            {
                for (uint i = 0; i < sectionNum; i++)
                {
                    if (ini.getSection(i) == PARAMETERS_INI_SECTION_NAME)
                    {
                        parameterNum = ini.getNumberOfParameters(ini.getSection(i));
                        sectionFindFlag = true;

                        for (uint j = 0; j < parameterNum; j++)
                        {
                            parameterName = ini.getParameterName(ini.getSection(i), j);

                            switch (parameterName)
                            {
                                //Baseline   
                                case PARAMETERS_INI_BASELINE:
                                    if (ALGORITHM_ABC == ini.getParameterValue(ini.getSection(i), j))
                                    {
                                        Algorithm = ALGORITHM_ABC;
                                    }
                                    else if (ALGORITHM_LINEAR == ini.getParameterValue(ini.getSection(i), j))
                                    {
                                        Algorithm = ALGORITHM_LINEAR;
                                    }
                                    else
                                    {
                                        Algorithm = ALGORITHM_ABC;
                                        errorMessage += ERROR_INVALID_BASELINE;
                                    }
                                    break;
                                //ABC_BASELINE_Windows Width
                                case PARAMETERS_INI_ABC_BASELINE_WINDOW_WIDTH:
                                    try
                                    {
                                        ABCWindowWidth = double.Parse(ini.getParameterValue(ini.getSection(i), j));
                                    }
                                    catch
                                    {
                                        ABCWindowWidth = DEFAULT_ABC_WINDOW_WIDTH;
                                        errorMessage += ERROR_INVALID_ABC_WINDOW_WIDTH;
                                    }
                                    break;
                                //LINEAR_BASELINE_Windows width
                                case PARAMETERS_INI_LINEAR_BASELINE_WINDOW_WIDTH:
                                    try
                                    {
                                        LinearWindowWidth = double.Parse(ini.getParameterValue(ini.getSection(i), j));
                                    }
                                    catch
                                    {
                                        LinearWindowWidth = DEFAULT_LINEAR_WINDOW_WIDTH;
                                        errorMessage += ERROR_INVALID_LINEAR_WINDOW_WIDTH;
                                    }
                                    break;
                                //Noise Factor
                                case PARAMETERS_INI_NOISE_FACTOR:
                                    try
                                    {
                                        NoiseFactor = double.Parse(ini.getParameterValue(ini.getSection(i), j));
                                    }
                                    catch
                                    {
                                        NoiseFactor = DEFAULT_NOISE_FACTOR;
                                        errorMessage += ERROR_INVALID_NOISE_FACTOR;
                                    }
                                    break;
                                //Smoothing Width
                                case PARAMETERS_INI_SMOOTHING_WIDTH:
                                    try
                                    {
                                        SmoothingWidth = int.Parse(ini.getParameterValue(ini.getSection(i), j));
                                    }
                                    catch
                                    {
                                        SmoothingWidth = DEFAULT_SMOOTHING_WIDTH;
                                        errorMessage += ERROR_INVALID_SMOOTHING_WIDTH;
                                    }
                                    break;
                                default:
                                    break;
                            }
                        }
                    }
                }
            }
            catch (Exception)
            {

            }

            if (!sectionFindFlag)
            {
                ABCWindowWidth = DEFAULT_ABC_WINDOW_WIDTH;
                LinearWindowWidth = DEFAULT_LINEAR_WINDOW_WIDTH;
                NoiseFactor = DEFAULT_NOISE_FACTOR;
                SmoothingWidth = DEFAULT_SMOOTHING_WIDTH;
            }

            return errorMessage;
        }

        /// <summary>
        /// Write INI file.
        /// </summary>
        public void WriteParameterIni()
        {
            ClrMsppManager msObj = ClrMsppManager.getInstance();
            ClrIniFile ini = msObj.getParameters();

            ini.setString(PARAMETERS_INI_SECTION_NAME, PARAMETERS_INI_BASELINE, Algorithm);
            ini.setString(PARAMETERS_INI_SECTION_NAME, PARAMETERS_INI_ABC_BASELINE_WINDOW_WIDTH, ABCWindowWidth.ToString());
            ini.setString(PARAMETERS_INI_SECTION_NAME, PARAMETERS_INI_LINEAR_BASELINE_WINDOW_WIDTH, LinearWindowWidth.ToString());
            ini.setString(PARAMETERS_INI_SECTION_NAME, PARAMETERS_INI_NOISE_FACTOR, NoiseFactor.ToString());
            ini.setString(PARAMETERS_INI_SECTION_NAME, PARAMETERS_INI_SMOOTHING_WIDTH, SmoothingWidth.ToString());

            ini.save(ini.getFilePath());
        }

        /// <summary>
        /// Set Default values.
        /// </summary>
        public void Default()
        {
            //_algorithm = ALGORITHM_ABC;
            _abcWindowWidth = DEFAULT_ABC_WINDOW_WIDTH;
            _linearWindowWidth = DEFAULT_LINEAR_WINDOW_WIDTH;
            _noiseFactor = DEFAULT_NOISE_FACTOR;
            _smoothingWidth = DEFAULT_SMOOTHING_WIDTH;
        }

        #endregion
    }
}
