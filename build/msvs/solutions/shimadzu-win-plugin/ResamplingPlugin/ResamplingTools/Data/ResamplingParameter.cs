/// <summary>
/// [FILE] ResamplingParameter.cs
/// [ABSTRACT] Resampling plugin - Parameters Class of Resampling.
/// Copyright (C) 2013-08-09 Shimadzu
/// </summary>

using System;
using System.Collections.Generic;
using System.Linq;
using kome.clr;

namespace ResamplingPlugin.ResamplingTools.Data
{
    /// <summary>
    /// Parameters Class of Resampling.
    /// </summary>
    public class ResamplingParameter
    {
        #region --- Defines --------------------------------------------

        /// <summary>
        /// Patterns of sampling.
        /// </summary>
        public enum SamplingMode
        {
            /// <summary>
            /// Same as first spectrum.
            /// </summary>
            FIRST,

            /// <summary>
            /// Average interval in selected spectra.
            /// </summary>
            AVERAGE,

            /// <summary>
            /// Minimum interval in selected specrta.
            /// </summary>
            MINIMUIM,

            /// <summary>
            /// Specified interval.
            /// </summary>
            SPECIFIED
        }

        /// <summary>
        /// Algorithms of Interpolation.
        /// </summary>
        public enum InterpolationAlgorithm
        {
            /// <summary>
            /// Linear Interpolation.
            /// </summary>
            LINEAR,

            /// <summary>
            /// PCHIP Interpolatiion.
            /// </summary>
            PCHIP
        }

        /// <summary>
        /// String of Resampling Settings Section in .ini file.
        /// </summary>
        protected const string PARAMETERS_INI_SECTION_NAME = "Resampling Settings";

        /// <summary>
        /// String of sampling points in .ini file.
        /// </summary>
        protected const string PARAMETERS_INI_SAMPLINGPOINTS = "SAMPLING_POINTS";

        /// <summary>
        /// String of interpolation in .ini file.
        /// </summary>
        protected const string PARAMETERS_INI_INTERPOLATION = "INTERPOLATION";

        /// <summary>
        /// String of specified in .ini file.
        /// </summary>
        protected const string PARAMETERS_INI_SPECIFIED_INTERVAL = "SPECIFIED_INTERVAL";

        /// <summary>
        /// String of max output range in .ini file.
        /// </summary>
        protected const string PARAMETERS_INI_OUTPUTRANGEMIN = "OUTPUTRANGE_MIN";

        /// <summary>
        /// String of min output range in .ini file.
        /// </summary>
        protected const string PARAMETERS_INI_OUTPUTRANGEMAX = "OUTPUTRANGE_MAX";

        /// <summary>
        /// Error message for invalid Resampling Mode.
        /// </summary>
        protected const string ERROR_RESAMPLING_MODE = "Invalid Resampling mode.\n 'Same as first sprctrm' was selected as a default.";

        /// <summary>
        /// Error message for invalid Interpolation.
        /// </summary>
        protected const string ERROR_INTERPOLATION = "Invalid Interpolation.\n 'Linear' was selected as a default.";

        /// <summary>
        /// Error message for invalid output range min.
        /// </summary>
        protected const string ERROR_LOWER_OUTPUT_RANGE = "Invalid Lower Output Range.\n 'Null' was selected as a default.";

        /// <summary>
        /// Error message for invalid output range max.
        /// </summary>
        protected const string ERROR_HIGHER_OUTPUT_RANGE = "Invalid Higher Output Range.\n 'Null' was selected as a default.";

        /// <summary>
        /// Error message for invalid Specified interval.
        /// </summary>
        protected const string ERROR_SPECIFIED_INTERVAL = "Invalid Specified Interval.\n '0.5' was selected as a default.";

        #endregion

        #region --- Construction ---------------------------------------

        public ResamplingParameter()
        {
            IsIniFileExist = ReadParameterIni();
        }

        #endregion

        #region --- Variables ------------------------------------------

        /// <summary>XYDataWrapper List</summary>
        private List<ClrDataPoints> _xyDataList = new List<ClrDataPoints>();

        #endregion

        #region --- Properties -----------------------------------------

        /// <summary>
        /// Gets and sets a value indicating resampling mode.
        /// </summary>
        public SamplingMode ResamplingMode
        {
            get;
            set;
        }

        /// <summary>
        /// Gets and sets a value indicating lower output range for.
        /// </summary>
        public double? OutputLowerRange
        {
            get;
            set;
        }

        /// <summary>
        /// Gets and sets a value indicating higher output range.
        /// </summary>
        public double? OutputHigherRange
        {
            get;
            set;
        }

        /// <summary>
        /// Gets and sets a value indicating specifed interval.
        /// </summary>
        public double? SpecifiedInterval
        {
            get;
            set;
        }

        /// <summary>
        /// Gets and sets a value indicating Interpolation.
        /// </summary>
        public InterpolationAlgorithm Interpolation
        {
            get;
            set;
        }

        /// <summary>
        /// Gets and Sets a Error Message.
        /// </summary>
        public String ErrorMessage
        {
            get;
            set;
        }

        /// <summary>
        /// Gets and Sets a IniFileExist.
        /// </summary>
        public bool IsIniFileExist
        {
            get;
            set;
        }

        #endregion

        #region --- Public Methods -------------------------------------

        /// <summary>
        /// add xyData to XYDataWrapper list
        /// </summary>
        /// <param name="xyData">XYDataWrapper</param>
        public void AddXYData(ClrDataPoints xyData)
        {
            _xyDataList.Add(xyData);
        }

        /// <summary>
        /// get number of XYDataWrapper list
        /// </summary>
        /// <returns>number of XYDataWrapper list</returns>
        public int GetXYDataNum()
        {
            return _xyDataList.Count;
        }

        /// <summary>clear XYDataWrapper list</summary>
        public void ClearXYDataList()
        {
            foreach (ClrDataPoints xydw in _xyDataList)
            {
                //Clear points before dispose
                //(XYDatawrapper is Unmanaged).
                xydw.Dispose();
            }
            _xyDataList.Clear();
        }

        /// <summary>
        /// Get XYData.
        /// </summary>
        /// <param name="index">index</param>
        /// <returns>XYData</returns>
        public ClrDataPoints GetXYData(int index)
        {
            if ((index < 0)
                || (_xyDataList.Count <= index))
            {
                throw new ArgumentOutOfRangeException("index");
            }
            return _xyDataList.ElementAt(index);
        }

        /// <summary>
        /// Read ini file.
        /// </summary>
        /// <returns>result</returns>
        public bool ReadParameterIni()
        {
            ClrMsppManager msObj = ClrMsppManager.getInstance();
            ClrIniFile ini = msObj.getParameters();

            uint sectionNum = ini.getNumberOfSections();
            uint parameterNum;
            string parameterName;
            bool flgSectionFound = default(bool);
            string errMsg = "";

            for (uint i = 0; i < sectionNum; i++)
            {
                if (ini.getSection(i) == PARAMETERS_INI_SECTION_NAME)
                {
                    parameterNum = ini.getNumberOfParameters(ini.getSection(i));
                    flgSectionFound = true;

                    for (uint j = 0; j < parameterNum; j++)
                    {
                        parameterName = ini.getParameterName(ini.getSection(i), j);
                        string parseResult = ini.getParameterValue(ini.getSection(i), j);
                        switch (parameterName)
                        {
                            //Sampling Points
                            case PARAMETERS_INI_SAMPLINGPOINTS:
                                SamplingMode mode = SamplingMode.FIRST;
                                if (ParseResamplingMode(parseResult, ref errMsg, ref mode) == false)
                                {
                                    ErrorMessage += (errMsg + "\n");
                                }
                                else
                                {
                                    ResamplingMode = mode;
                                }

                                break;
                            //Interpolation
                            case PARAMETERS_INI_INTERPOLATION:
                                InterpolationAlgorithm pol = InterpolationAlgorithm.LINEAR;
                                if (ParseInterpolation(parseResult, ref errMsg, ref pol) == false)
                                {
                                    ErrorMessage += (errMsg + "\n");
                                }
                                Interpolation = pol;

                                break;
                            //Output range MIN
                            case PARAMETERS_INI_OUTPUTRANGEMIN:
                                double? min = null;
                                if (ParseOutputRangeMin(parseResult, ref errMsg, ref min) == false)
                                {
                                    ErrorMessage += (errMsg + "\n");
                                }
                                OutputLowerRange = min;

                                break;
                            //Output range MAX
                            case PARAMETERS_INI_OUTPUTRANGEMAX:
                                double? max = null;
                                if (ParseOutputRangeMax(parseResult, ref errMsg, ref max) == false)
                                {
                                    ErrorMessage += (errMsg + "\n");
                                }
                                OutputHigherRange = max;

                                break;
                            //Specified Interval
                            case PARAMETERS_INI_SPECIFIED_INTERVAL:
                                double? Interval = null;
                                if (ParseSpecifiedInterval(parseResult, ref errMsg, ref Interval, ResamplingMode) == false)
                                {
                                    ErrorMessage = ErrorMessage + errMsg + "\n";
                                }
                                SpecifiedInterval = Interval;

                                break;
                            default:
                                break;
                        }
                    }
                }
            }


            //Debug.WriteLine(ini.getSection(0));

            return flgSectionFound;
        }

        /// <summary>
        /// Write ini file.
        /// </summary>
        public void WriteParameterIni()
        {
            ClrMsppManager msObj = ClrMsppManager.getInstance();
            ClrIniFile ini = msObj.getParameters();

            ini.setString(PARAMETERS_INI_SECTION_NAME, PARAMETERS_INI_SAMPLINGPOINTS, ResamplingMode.ToString());
            ini.setString(PARAMETERS_INI_SECTION_NAME, PARAMETERS_INI_INTERPOLATION, Interpolation.ToString());

            ini.setString(PARAMETERS_INI_SECTION_NAME, PARAMETERS_INI_SPECIFIED_INTERVAL, SpecifiedInterval.ToString());
            ini.setString(PARAMETERS_INI_SECTION_NAME, PARAMETERS_INI_OUTPUTRANGEMIN, OutputLowerRange.ToString());
            ini.setString(PARAMETERS_INI_SECTION_NAME, PARAMETERS_INI_OUTPUTRANGEMAX, OutputHigherRange.ToString());

            ini.save(ini.getFilePath());
        }

        /// <summary>
        /// Parsing Resampling Mode.
        /// </summary>
        /// <param name="data">Resampling Text</param>
        /// <param name="errMsg">Error Message</param>
        /// <param name="mode">SamplingMode</param>
        /// <returns></returns>
        public bool ParseResamplingMode(string data, ref string errMsg, ref SamplingMode mode)
        {
            bool ret = true;
            try
            {
                mode = (SamplingMode)Enum.Parse(typeof(SamplingMode), data);
            }
            catch
            {
                ret = false;
                errMsg = ERROR_RESAMPLING_MODE;
                mode = SamplingMode.FIRST;
            }
            return ret;
        }

        /// <summary>
        /// Parsing Interpolation
        /// </summary>
        /// <param name="data">Interpolation</param>
        /// <param name="errMsg">Error Message</param>
        /// <param name="pol">Interpolation</param>
        /// <returns></returns>
        public bool ParseInterpolation(string data, ref string errMsg, ref InterpolationAlgorithm pol)
        {
            bool ret = true;
            try
            {
                pol = (InterpolationAlgorithm)Enum.Parse(typeof(InterpolationAlgorithm), data);
            }
            catch
            {
                ret = false;
                errMsg = ERROR_INTERPOLATION;
                pol = InterpolationAlgorithm.LINEAR;
            }
            return ret;
        }

        /// <summary>
        /// Parsing OutputRangeMin.
        /// </summary>
        /// <param name="data">OutputRangeMin</param>
        /// <param name="errMsg">ErrorMessage</param>
        /// <param name="min">Min</param>
        /// <returns></returns>
        public bool ParseOutputRangeMin(string data, ref string errMsg, ref double? min)
        {
            bool ret = true;

            if (data == string.Empty)
            {
                min = null;
                return ret;
            }

            try
            {
                min = double.Parse(data);
            }
            catch
            {
                ret = false;
                errMsg = ERROR_LOWER_OUTPUT_RANGE;
                min = null;
            }
            return ret;
        }

        /// <summary>
        /// Parsing OutputRangeMax.
        /// </summary>
        /// <param name="data">OutputRangeMax</param>
        /// <param name="errMsg">Error Message</param>
        /// <param name="max">Max</param>
        /// <returns></returns>
        public bool ParseOutputRangeMax(string data, ref string errMsg, ref double? max)
        {
            bool ret = true;

            if (data == string.Empty)
            {
                max = null;
                return ret;
            }

            try
            {
                max = double.Parse(data);
            }
            catch
            {
                ret = false;
                errMsg = ERROR_HIGHER_OUTPUT_RANGE;
                max = null;
            }
            return ret;
        }


        /// <summary>
        /// Checking OutputRange.
        /// </summary>
        /// <param name="min">min</param>
        /// <param name="max">max</param>
        /// <param name="datamin">datamin</param>
        /// <param name="datamax">datamax</param>
        /// <returns></returns>
        public bool IsValidOutputRange(double? min, double? max, double? datamin, double? datamax)
        {
            if ((min != null)
                && (datamin != null)
                && (min < datamin))
            {
                return false;
            }

            if ((max != null)
                && (datamax != null)
                && (max > datamax))
            {
                return false;
            }

            if ((min != null)
                && (max != null)
                && (min > max))
            {
                return false;
            }

            if ((datamin != null)
                && (datamax != null)
                && (datamin > datamax))
            {
                return false;
            }

            return true;
        }

        /// <summary>
        /// Parsing SpecifiedInterval
        /// </summary>
        /// <param name="data">SpecifiedInterval</param>
        /// <param name="errMsg">Error Message</param>
        /// <param name="Interval">Interval</param>
        /// <param name="mode">Mode</param>
        /// <returns></returns>
        public bool ParseSpecifiedInterval(string data, ref string errMsg, ref double? Interval, SamplingMode mode)
        {
            bool ret = true;
            try
            {
                Interval = double.Parse(data);
            }
            catch
            {
                if (mode == SamplingMode.SPECIFIED)
                {
                    ret = false;
                    errMsg = ERROR_SPECIFIED_INTERVAL;
                    Interval = 0.5;
                }
                else
                {
                    Interval = null;
                }
            }
            return ret;
        }

        /// <summary>
        /// Checking SpecifiedIntervalRange
        /// </summary>
        /// <param name="min">min</param>
        /// <param name="max">max</param>
        /// <param name="interval">interval</param>
        /// <param name="mode">mode</param>
        /// <returns></returns>
        public bool IsValidSpecifiedIntervalRange(double min, double max, double? interval, SamplingMode mode)
        {
            if (mode == SamplingMode.SPECIFIED)
            {
                if ((min > max)
                    || (interval == null)
                    || (min > interval)
                    || (interval > max))
                {
                    return false;
                }
            }
            return true;
        }

        #endregion

    }

}
