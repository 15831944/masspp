/// <summary>
/// [FILE] SpectrumCalculationManager.cs
/// [ABSTRACT] Spectrum Calculation Manager Mass++ access class
/// Copyright (C) 2013-08-01 Shimadzu
/// </summary>

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using ResamplingPlugin.ResamplingTools.Data;
using ResamplingPlugin.ResamplingTools.Cal;
using ResamplingPlugin.SpectrumCalculation.Cal;
using ResamplingPlugin.SpectrumCalculation.Data;


// Mass++ namespace
using kome.clr;
using ResamplingPlugin.SpectrumCalculation.GUI;

namespace ResamplingPlugin.SpectrumCalculation
{
    class SpectrumCalculationManager
    {
        #region --- Constants ------------------------------------------

        /// <summary> Types of Operation Type. </summary>
        enum BatchParamOperationType
        {
            /// <summary>average</summary>
            AVERAGE = SpectrumCalculation.Cal.SpectrumCalculation.OperationSettings.AVERAGE,
            /// <summary>subtract</summary>
            SUBTRACT = SpectrumCalculation.Cal.SpectrumCalculation.OperationSettings.SUBTRACT,
            /// <summary>sum</summary>
            SUM = SpectrumCalculation.Cal.SpectrumCalculation.OperationSettings.SUM,
            /// <summary>divide</summary>
            DIVIDE = SpectrumCalculation.Cal.SpectrumCalculation.OperationSettings.DIVIDE,
        }

        /// <summary> Types of Sampling Pitch. </summary>
        enum BatchParamSamplingPitchType
        {
            /// <summary>firstspec</summary>
            FIRSTSPEC = ResamplingParameter.SamplingMode.FIRST,
            /// <summary>minpitch </summary>
            MINPITCH = ResamplingParameter.SamplingMode.MINIMUIM,
            /// <summary>average</summary>
            AVERAGE = ResamplingParameter.SamplingMode.AVERAGE,
            ///<summary>specified</summary>
            SPECIFIED = ResamplingParameter.SamplingMode.SPECIFIED,
        }
        
        /// <summary> Types of Interpolate Algorithm. </summary>
        enum BatchParamInterpolateAlgorithmType
        {
            /// <summary>firstspec</summary>
            LINEAR = ResamplingParameter.InterpolationAlgorithm.LINEAR,
            /// <summary>minpitch </summary>
            PCHIP = ResamplingParameter.InterpolationAlgorithm.PCHIP,
        }

        // Number of max parameters.
        const int NUMBER_OF_MAX_PARAM = 5;

        /// <summary> Types of Parameter. </summary>
        enum BatchParamType
        {
            /// <summary>File Spectrum Name</summary>
            FILE_SPECTRUM_NAME = 0,
            /// <summary>Operation Type</summary>
            OPERATION_TYPE,
            /// <summary>Sampling Pitch</summary>
            SAMPLING_PITCH,
            ///<summary>Output Range</summary>
            OUTPUT_RANGE,
            ///<summary>Interpolate Algorithm</summary>
            INTERPOLATE_ALGORITHM
        }

        /// <summary>
        /// Title of progress.
        /// </summary>
        protected const string PROGRESS_TITLE = "Resampling";

        /// <summary>
        /// Error message for Unexpected error.
        /// </summary>
        protected const string ERROR_UNEXPECTED = "Unexpected Error!";

        /// <summary>
        /// Error message for invalid specified interval.
        /// </summary>
        protected const string ERROR_INVALID_SPECIFIED_INTERVAL = "Parameter error: Invalid Specified Interval. ";

        /// <summary>
        /// Error message format for unexpected string.
        /// </summary>
        protected const string ERROR_FORMAT_INVALID_STRING = "Invalid string is contained in parameters : '{0}'";

        /// <summary>
        /// Error message format for invalid operation type.
        /// </summary>
        protected const string ERROR_FORMAT_OPERATION_TYPE = "Invalid operation type is contained in parameters: '{0}'";

        /// <summary>
        /// Error message format for invalid sampling pitch.
        /// </summary>
        protected const string ERROR_FORMAT_SAMPLING_PITCH = "Invalid sampling pitch is contained in parameters: '{0}'";

        /// <summary>
        /// Error message format for invalid output range.
        /// </summary>
        protected const string ERROR_FORMAT_RANGE = "Invalid output range is contained in parameters: '{0}'";

        /// <summary>
        /// Error message format for invalid interpolate algorithm.
        /// </summary>
        protected const string ERROR_FORMAT_INTERPOLATE = "Invalid interpolate algorithm is contained in parameters: '{0}'";

        /// <summary>
        /// Error message format for invalid parameter.
        /// </summary>
        protected const string ERROR_FORMAT_PARAMETER = "Invalid parameter is contained in parameters: {0}";

        /// <summary>
        /// Debug log format for operation type.
        /// </summary>
        protected const string DEBUG_LOG_OPERATION_TYPE = "Operation Type='{0}'";

        /// <summary>
        /// Debug log format for sampling pitch.
        /// </summary>
        protected const string DEBUG_LOG_SAMPLING_PITCH = "Sampling Pitch='{0}'";

        /// <summary>
        /// Debug log format for specified interval.
        /// </summary>
        protected const string DEBUG_LOG_SPECIFIED_INTERVAL = "Specified Interval='{0}'";

        /// <summary>
        /// Debug log format for lower output range.
        /// </summary>
        protected const string DEBUG_LOG_RANGE_LOW = "Range Low={0}";

        /// <summary>
        /// Debug log format for higher output range.
        /// </summary>
        protected const string DEBUG_LOG_RANGE_HIGH = "Range High={0}";

        /// <summary>
        /// Debug log format for interpolate algorithm.
        /// </summary>
        protected const string DEBUG_LOG_INTERPOLATE = "Interpolate Algorithm={0}";

        /// <summary>
        /// "sampling pitch" string.
        /// </summary>
        protected const string PARAMETER_NAME_SAMPLING_PITCH = "sampling pitch";

        /// <summary>
        /// "SAMPLING PITCH" string.
        /// </summary>
        protected const string FUNCTION_TYPE_SAMPLING_PITCH = "SAMPLING_PITCH";

        /// <summary>
        /// "Sample" string.
        /// </summary>
        protected const string SAMPLE_STRING = "Sample";

        /// <summary>
        /// "Spectrum" string.
        /// </summary>
        protected const string SPECTRUM_STRING = "Spectrum";

        /// <summary>
        /// null string.
        /// </summary>
        protected const string NULL_STRING = "null";

        /// <summary>
        /// Error code for Unexpected error.
        /// </summary>
        protected readonly static int ERROR_CODE_UNEXPECTED = -1;

        /// <summary>
        /// Error code for invalid operation type.
        /// </summary>
        protected readonly static int ERROR_CODE_OPERATION_TYPE = -2;

        /// <summary>
        /// Error code for invalid sampling pitch.
        /// </summary>
        protected readonly static int ERROR_CODE_SAMPLING_PITCH = -3;

        /// <summary>
        /// Error code for invalid output range.
        /// </summary>
        protected readonly static int ERROR_CODE_OUTPUT_RANGE = -4;

        /// <summary>
        /// Error code for invalid interpolate.
        /// </summary>
        protected readonly static int ERROR_CODE_INTERPOLATE = -5;

        /// <summary>
        /// Error code for invalid parameter.
        /// </summary>
        protected readonly static int ERROR_CODE_PARAMETER = -6;

        #endregion

        #region --- Variables ------------------------------------------
        /// <summary>
        /// SpectrumCalculationManager instance
        /// </summary>
        private static SpectrumCalculationManager _manager = new SpectrumCalculationManager();

        /// <summary>
        /// SpectrumCalculationDialog instance
        /// </summary>
        private SpectrumCalculationDialog _dlgSpecCalc = null;

        /// <summary>
        /// sub progress
        /// </summary>
        private static kome.clr.ProgressWrapper subProgress = null;

        /// <summary>
        /// Progress Bar Start Flag
        /// </summary>
        private static bool flgProgressStart = false;

        #endregion

        #region --- Construction ---------------------------------------
        /// <summary>
        /// Initializes a new instance of the SpectrumCalculationManager class
        /// </summary>
        private SpectrumCalculationManager()
        {
        }
        #endregion

        #region --- Public Methods -------------------------------------

        /// <summary>
        /// Display Spectrum Calculation Dialog
        /// </summary>
        public static void DisplayDlgSpecCalc(ClrVariant clrVar)
        {
            if (_manager._dlgSpecCalc == null)
            {
                _manager._dlgSpecCalc = new SpectrumCalculationDialog(clrVar);
                _manager._dlgSpecCalc.DisplayClosed += _manager.SpectrumCalculationDialog_DisplayClosed;
                _manager._dlgSpecCalc.Show();
            }
            else
            {
                _manager._dlgSpecCalc.UpdateParam(clrVar);
                _manager._dlgSpecCalc.Activate();
            }
        }

        /// <summary>
        /// Commandline Spec Calc.
        /// </summary>
        /// <param name="clrParams"></param>
        /// <returns>0:success, 1:fail</returns>
        public static int CommandlineSpecCalc(ClrParameters clrParams)
        {
            kome.clr.ClrLogger log = kome.clr.ClrLogger.getInstance();

            // Return value (Success: true, Failure: false)
            int ret = 0;
            // We initially set ret.prim.boolVal to false so that we can return a false
            // result easily using "return ret" if a failure condition occurs.

            // progress bar
            kome.clr.ProgressWrapper progress = null;
            kome.clr.DisplayProgressWrapper display_progress = new DisplayProgressWrapper(PROGRESS_TITLE);
            progress = display_progress;
            progress.createSubProgresses(2);    // Divide Progress bar into 2 sections.
            subProgress = progress.getSubProgress(0);  // Get first Sub progress.
            subProgress.setRange(0, 5); // Set value between 0 to 5.

            // start
            string[] strArrayParam = new string[NUMBER_OF_MAX_PARAM]; // Store 5 parameters without conversion.

            bool flgTryParseSpecifiedInterval = true; // If failed to convert to dSpecifiedInterval, set true.
            double dSpecifiedInterval = -1;  // Store the parameter of specified[999.99] option.

            kome.clr.SettingParameterValuesWrapper settings
                = kome.clr.ClrPluginCallTool.getSettingValues(clrParams);

            kome.clr.PluginManagerWrapper plgMgr = kome.clr.PluginManagerWrapper.getInstance();

            if (settings == null)
            {
                //Unexpected Error
                log.errorCode(ERROR_UNEXPECTED, ERROR_CODE_UNEXPECTED);
                return 1;    // Failed.
            }

            // Get the number of parameters.
            uint uNumberOfParameters = settings.getNumberOfParameters();

            //Get parameters.
            for (uint i = 0; i < uNumberOfParameters; i++)
            {
                string strParamName = settings.getParameterName(i);
                string strParamValue = settings.getParameterValue(strParamName);

                if (i < NUMBER_OF_MAX_PARAM)
                {
                    strArrayParam[i] = strParamValue;   // Store parameter.
                }

                // If strParamName is not "sampling pitch", it has not potential that has parameter.
                if (strParamName != PARAMETER_NAME_SAMPLING_PITCH)
                {
                    continue;
                }

                // The case that strParamName equals sampling pitch.
                uint uNumberOfFunctionItems = plgMgr.getNumberOfFunctionItems(FUNCTION_TYPE_SAMPLING_PITCH);

                for (uint j = 0; j < uNumberOfFunctionItems; j++)
                {
                    kome.clr.PluginFunctionItemWrapper item = plgMgr.getFunctionItem(FUNCTION_TYPE_SAMPLING_PITCH, j);
                    string strLongName = item.getLongName();

                    if ((item.getShortName() != strParamValue)
                        || (item.getSettingsPage() == null))
                    {
                        continue;
                    }

                    kome.clr.SettingParameterValuesWrapper settingsSub2
                        = settings.getSubParameterValues(strParamName, strParamValue);
                    uint uNumberOfSubParameters = settingsSub2.getNumberOfParameters();
                    string strSubParamName = settingsSub2.getParameterName(0);
                    string strSubParamValue = settingsSub2.getParameterValue(strSubParamName);

                    if (!double.TryParse(strSubParamValue, out dSpecifiedInterval))
                    {
                        // Convert failed.
                        dSpecifiedInterval = -1;
                        flgTryParseSpecifiedInterval = false;
                    }
                }
            }

            // Commandline parameter getting...
            ResamplingParameter param = new ResamplingParameter();

            // (1)File Spectrum Name
            string strSpectrumName = strArrayParam[(int)BatchParamType.FILE_SPECTRUM_NAME];

            // (2)Operation Type
            string strOperationType = strArrayParam[(int)BatchParamType.OPERATION_TYPE];

            // (3)Sampling Pitch
            string strSamplingPitch = strArrayParam[(int)BatchParamType.SAMPLING_PITCH];

            // (4)Output Range
            string strOutputRange = strArrayParam[(int)BatchParamType.OUTPUT_RANGE];

            // (5)Interpolate Algorithm
            string strInterpolateAlgorithm = strArrayParam[(int)BatchParamType.INTERPOLATE_ALGORITHM];

            // Commandline parameter TryParse... ////////
            bool bRet;  // return of TryParse.

            // Postpone Processing for (1)File Spectrum Name.

            // (2)Operation Type
            BatchParamOperationType resultOperationType;
            bRet = Enum.TryParse<BatchParamOperationType>(strOperationType.ToUpper(), out resultOperationType);
            if (bRet == false)
            {
                // error
                log.errorCode(string.Format(ERROR_FORMAT_OPERATION_TYPE, strOperationType), ERROR_CODE_OPERATION_TYPE);
                return 1;   // Failed to convert.
            }
            SpectrumCalculation.Cal.SpectrumCalculation.OperationSettings resultOperationSetting;
            resultOperationSetting = (SpectrumCalculation.Cal.SpectrumCalculation.OperationSettings)resultOperationType;   // AVERAGE, SUBTRACT, SUM, DIVIDE are the operation type option.
            log.debug(string.Format(DEBUG_LOG_OPERATION_TYPE, resultOperationSetting));
            subProgress.setPosition(1);

            // (3)Sampling Pitch
            BatchParamSamplingPitchType resultSamplingPitch;
            bRet = Enum.TryParse<BatchParamSamplingPitchType>(strSamplingPitch.ToUpper(), out resultSamplingPitch);
            if (bRet == false)
            {
                // error
                log.errorCode(string.Format(ERROR_FORMAT_SAMPLING_PITCH, strSamplingPitch), ERROR_CODE_SAMPLING_PITCH);
                return 1;   // Failed to convert.
            }
            ResamplingParameter.SamplingMode resultResamplingMode;
            resultResamplingMode = (ResamplingParameter.SamplingMode)resultSamplingPitch;   // Convert FIRST,MINIMUIM,AVERAGE,SPECIFIED into internal value.
            log.debug(string.Format(DEBUG_LOG_SAMPLING_PITCH, resultResamplingMode));

            param.ResamplingMode = resultResamplingMode;
            if (resultSamplingPitch == BatchParamSamplingPitchType.SPECIFIED)
            {
                if (!flgTryParseSpecifiedInterval)
                {
                    //error
                    return 1;
                }
                param.SpecifiedInterval = dSpecifiedInterval;
                log.debug(string.Format(DEBUG_LOG_SPECIFIED_INTERVAL, dSpecifiedInterval));
            }
            else
            {
                param.SpecifiedInterval = null;
                log.debug(string.Format(DEBUG_LOG_SPECIFIED_INTERVAL, NULL_STRING));
            }
            subProgress.setPosition(2);

            // (4)Output Range  
            string strOutputRangeLow;
            string strOutputRangeHi;
            double dOutputRangeLow = 0;
            double dOutputRangeHi = 0;

            string[] arrystrSeparatedOutputRange = kome.clr.SettingsValueWrapper.separateListValue(strOutputRange);
            if (2 == arrystrSeparatedOutputRange.Length)
            {
                strOutputRangeLow = arrystrSeparatedOutputRange[0];
                strOutputRangeHi = arrystrSeparatedOutputRange[1];
            }
            else
            {
                //error
                log.errorCode(string.Format(ERROR_FORMAT_RANGE, strOutputRange), ERROR_CODE_OUTPUT_RANGE);
                return 1;
            }

            if (double.TryParse(strOutputRangeLow, out dOutputRangeLow))
            {
                param.OutputLowerRange = dOutputRangeLow;
                log.debug(string.Format(DEBUG_LOG_RANGE_LOW, param.OutputLowerRange));
            }
            else
            {
                param.OutputLowerRange = null;
                log.debug(string.Format(DEBUG_LOG_RANGE_LOW, NULL_STRING));
            }

            if (double.TryParse(strOutputRangeHi, out dOutputRangeHi))
            {
                param.OutputHigherRange = dOutputRangeHi;
                log.debug(string.Format(DEBUG_LOG_RANGE_HIGH, param.OutputHigherRange));
            }
            else
            {
                param.OutputHigherRange = null;
                log.debug(string.Format(DEBUG_LOG_RANGE_HIGH, NULL_STRING));
            }
            subProgress.setPosition(3);

            // (5)Interpolate Algorithm
            BatchParamInterpolateAlgorithmType resultInterpolateAlgorithm;
            bRet = Enum.TryParse<BatchParamInterpolateAlgorithmType>(strInterpolateAlgorithm.ToUpper(), out resultInterpolateAlgorithm);
            if (bRet == false)
            {
                // error
                log.errorCode(string.Format(ERROR_FORMAT_INTERPOLATE, strInterpolateAlgorithm), ERROR_CODE_INTERPOLATE);
                return 1;
            }
            log.debug(string.Format(DEBUG_LOG_INTERPOLATE, resultInterpolateAlgorithm));

            ResamplingParameter.InterpolationAlgorithm resultInterpolation;
            resultInterpolation = (ResamplingParameter.InterpolationAlgorithm)resultInterpolateAlgorithm;
            param.Interpolation = resultInterpolation;
            subProgress.setPosition(4);

            //param.ErrorMessage = ERROR_GENERAL_PURPOSE;

            // Create ResamplingParameter is Complete.
            // Assign to SpectrumCalculation().
            SpectrumCalculation.Cal.SpectrumCalculation calc = new Cal.SpectrumCalculation(param);

            calc.OperationSetting = resultOperationSetting; // (2)Operation Type

            // (1)File Spectrum Name
            AddSpectrumFromParameter(calc, strSpectrumName);
            subProgress.setPosition(5);
            subProgress.fill();

            subProgress = progress.getSubProgress(1);  // Get first sub progress.

            calc.GetResamplingCalculation.UpdateResamplingProgress += UpdateSpecCalcResamplingProgressCmd;

            try
            {
                calc.Execute();
            }
            catch (ArgumentException ex)
            {
                log.errorCode(string.Format(ERROR_FORMAT_PARAMETER, ex.Message), ERROR_CODE_PARAMETER);
                return 1;
            }

            subProgress.fill();

            // Set Spectrum calculation result to active object.
            SetResultToActiveObject(calc.GetResultSpecCalculation);

            return ret;   // success.
        }
        
        #endregion

        #region --- Private Methods ------------------------------------

        /// <summary>
        /// Set Spectrum calculation result to active object.
        /// </summary>
        /// <param name="resultSpecCal">Spectrum calculation result</param>
        private static void SetResultToActiveObject(SpectrumCalculationResult resultSpecCal)
        {
            // Preparing Output files.
            SampleSetResampling sampleSet = new SampleSetResampling();
            SampleResampling sample = new SampleResampling(sampleSet);
            sample.openSample();
            DataGroupNodeWrapper root = sample.getRootDataGroupNode();

            // _resultSpecCal.Spectrums[0].getName() returns
            // "Average" or "Summed" or "Subtracted" or "Divided".
            sample.setName(resultSpecCal.Spectrums[0].getName() + " " + SAMPLE_STRING);
            root.setName(resultSpecCal.Spectrums[0].getName() + " " + SAMPLE_STRING);

            int cnt = 0;
            foreach (SpectrumResampling sr in resultSpecCal.Spectrums)
            {
                ClrDataPoints xyd = new ClrDataPoints();
                SpectrumResampling spec;

                if (resultSpecCal.Spectrums.Count == 1)
                {
                    spec = new SpectrumResampling(sample, sr.getName() + " " + SPECTRUM_STRING);
                }
                else
                {
                    cnt++;
                    spec = new SpectrumResampling(sample, sr.getName() + " " + SPECTRUM_STRING + "_" + cnt.ToString());
                }
                ClrDataPoints pts = new ClrDataPoints();
                sr.getXYData(pts, false);
                spec.getXYData(xyd, false);
                spec.SetData(pts);
                spec.setMinX(pts.getMinX());
                spec.setMaxX(pts.getMaxX());
                spec.setMaxIntensity(pts.getMaxY());
                spec.onGetXYData(xyd, -1.0, -1.0);
                spec.setMsStage((int)sr.getMsStage());
                sample.setSampleIndex((int)0);
                root.addSpectrum(spec);
            }
            sampleSet.addSample(sample);

            // -out
            kome.clr.ActiveObjectsManagerWrapper aoMgr = kome.clr.ActiveObjectsManagerWrapper.getInstance();
            aoMgr.setActiveSample(sample);
        }
        
        /// <summary>
        /// Update Spectrum Calculation(Resampling) Progress
        /// </summary>
        /// <param name="id">PROGRESS_RESAMPLINGT_PHASE id</param>
        /// <param name="value">progress bar value or max</param>
        /// <param name="cursmplnum">current number for text</param>
        /// <param name="maxsmplnum">max number for text</param>
        private static void UpdateSpecCalcResamplingProgressCmd(ResamplingCalculation.PROGRESS_RESAMPLING_PHASE id, int value, int cursmplnum, int maxsmplnum)
        {
            if (flgProgressStart == false)
            {
                // Initialize Sub Progress
                subProgress.setRange(0, maxsmplnum);
                flgProgressStart = true;
            }

            switch (id)
            {
                case ResamplingCalculation.PROGRESS_RESAMPLING_PHASE.RESAMPLING_START:
                    break;
                case ResamplingCalculation.PROGRESS_RESAMPLING_PHASE.RESAMPLING_DOING:
                    subProgress.setPosition(cursmplnum);
                    break;
                case ResamplingCalculation.PROGRESS_RESAMPLING_PHASE.RESAMPLING_END:
                default:
                    subProgress.fill();
                    break;
            }

            return;
        }

        /// <summary>
        /// Add Spectrum From Parameter
        /// </summary>
        /// <param name="calc"></param>
        /// <param name="strParameters">1st parameter string of -resample option.
        /// e.g."[[C:\\yeast5_bsa0.msb],[ ]],[[C:\\yeast5_c1.msb],[Scan 2]],[[C:\\yeast5_bsa0.msb],[Scan 3]]"</param>
        /// <returns>true:success, fail:fail</returns>
        private static bool AddSpectrumFromParameter(SpectrumCalculation.Cal.SpectrumCalculation calc, string strParameters)
        {
            string[] arrystrSeparated = kome.clr.SettingsValueWrapper.separateListValue(strParameters);

            foreach (string strFileSpec in arrystrSeparated)
            {
                string[] strFileAndSpec = kome.clr.SettingsValueWrapper.separateListValue(strFileSpec);
                SpectrumWrapper spec;
                try
                {
                    spec = SearchSpectrum(strFileAndSpec[0], strFileAndSpec[1]);
                }
                catch (ArgumentException e)
                {
                    string strMessage = e.Message;
                    string strParamName = e.ParamName;

                    //Console.Write(ERROR_FORMAT_UNEXPECTED_STRING, strParamName);
                    ClrLogger.getInstance().error(String.Format(ERROR_FORMAT_INVALID_STRING, strParamName));

                    return false;
                }

                calc.AddSpecData(spec);
            }

            return true;
        }

        /// <summary>
        /// Search Spectrum from spectrum file name and spectrum name
        /// </summary>
        /// <param name="strFilename">file name</param>
        /// <param name="strSpectrum">spectrum name</param>
        /// <returns>Spectrum</returns>
        /// 
        private static SpectrumWrapper SearchSpectrum(string strFilename, string strSpectrum)
        {
            string strFilenameLower = strFilename.ToLower();

            // get the rawdata that specified by -in option.
            kome.clr.ActiveObjectsManagerWrapper aoMgr = kome.clr.ActiveObjectsManagerWrapper.getInstance();

            uint uNumberOfOpenedSamples = aoMgr.getNumberOfOpenedSamples();

            for (uint i = 0; i < uNumberOfOpenedSamples; i++)
            {
                kome.clr.SampleWrapper opend_sample = aoMgr.getOpenedSample(i);
                kome.clr.SampleSetWrapper sample_set = opend_sample.getSampleSet();

                string strOpendFilePathOrg = sample_set.getFilePath();
                string strOpendFileNameOrg = sample_set.getFileName();

                // To lower.
                string strOpendFilePathLower = strOpendFilePathOrg.ToLower();
                string strOpendFileNameLower = strOpendFileNameOrg.ToLower();

                // Compair strOpendFilePathLower and strFilenameLower.
                if (strOpendFilePathLower != strFilenameLower)
                {
                    continue;
                }

                int nSampleID = opend_sample.getSampleId();
                string strName = opend_sample.getName();

                uint uGroups = opend_sample.getNumberOfGroups();

                for (uint j = 0; j < uGroups; j++)
                {
                    kome.clr.DataGroupNodeWrapper node = opend_sample.getGroup(j);
                    uint numberOfSpectra = node.getNumberOfSpectra();

                    for (uint k = 0; k < numberOfSpectra; k++)
                    {
                        kome.clr.SpectrumWrapper spec = node.getSpectrum(k);
                        string strNameSpec = spec.getName();
                        double dRt = spec.getRt();

                        // Use first spectrum when specname is null string.
                        bool flgUseFirstSpectrum = (strSpectrum == null) || strSpectrum.Length == 0 || strSpectrum.All((s) => (s == ' '));

                        if ((strNameSpec != strSpectrum)
                            && !flgUseFirstSpectrum)
                        {
                            continue;
                        }

                        return spec;
                    }
                }

                // Spectrum names not matches.
                throw new ArgumentException("message", "SpecName");
            }

            // File names not matches.
            throw new ArgumentException("message", "FileName");
        }

        #endregion

        #region --- Private Events -------------------------------------

        /// <summary>
        /// SpectrumCalculationDialog DisplayClosed event handler
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">FormClosedEventArgs</param>
        private void SpectrumCalculationDialog_DisplayClosed(object sender, FormClosedEventArgs e)
        {
            if (_manager._dlgSpecCalc != null)
            {
                _manager._dlgSpecCalc.DisplayClosed -= _manager.SpectrumCalculationDialog_DisplayClosed;
                _manager._dlgSpecCalc.Dispose();
                _manager._dlgSpecCalc = null;
            }
        }


        #endregion

    }
}
