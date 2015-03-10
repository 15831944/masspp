/// <summary>
/// [FILE] ResamplingPanel.cs
/// [ABSTRACT] Resampling plugin - Panel control class of resampling.
/// Copyright (C) 2013-08-09 Shimadzu
/// </summary>

using System;
using System.Windows.Forms;
using ResamplingPlugin.ResamplingTools.Other;
using ResamplingPlugin.ResamplingTools.Data;
using System.Globalization;

namespace ResamplingPlugin.ResamplingTools.GUI
{
    /// <summary>
    /// Panel control class of resampling.
    /// </summary>
    public partial class ResamplingPanel : Form
    {
        #region --- Constants ------------------------------------------

        /// <summary>
        /// Error message for non numericval Interval.
        /// </summary>
        private const string ERROR_INTERVAL = "Specified interval(Da) shold be positive real value( 0 to 10).\n";

        /// <summary>
        /// Error message for non numerical Range max.
        /// </summary>
        private const string ERROR_LOWER_RANGE = "Lower Output range (Da) shold be positive real value.\n";

        /// <summary>
        /// Error message for non numerical Range min.
        /// </summary>
        private const string ERROR_HIGHER_RANGE = "Higher Output range (Da) shold be positive real value.\n";

        /// <summary>
        /// Error message for Min and Max of Range is same.
        /// </summary>
        private const string ERROR_BOTH_ENDS_OF_RANGE_IS_SAME = "The lower bound of output range should be less than upper bound of output range.";

        /// <summary>
        /// Error message for Min or Max of Range is minus.
        /// </summary>
        private const string ERROR_MINUS_RANGE = "(Output range (Da))Please input zero or more.\n";

        #endregion

        #region --- Variables ------------------------------------------

        /// <summary>
        /// Help message control.
        /// </summary>
        private HelpMessageControl _helpMessageControl;

        /// <summary>
        /// Parameters class for resampling.
        /// </summary>
        private ResamplingParameter _parameter;

        /// <summary>
        /// Help message.
        /// </summary>
        private string _helpMessage;

        #endregion

        #region --- Properties -----------------------------------------

        /// <summary>
        /// Gets help message.
        /// </summary>
        public string HelpMessage
        {
            get { return _helpMessage; }
        }

        #endregion

        #region --- Construction ---------------------------------------

        /// <summary>
        /// Initializes a new instance of the ResamplingPanel class
        /// </summary>
        /// <param name="parent">Form parent</param>
        public ResamplingPanel(Form parent, ResamplingParameter parameter)
        {
            InitializeComponent();

            this.TopLevel = false;
            this.Parent = parent;

            _parameter = parameter;
            _helpMessageControl = new HelpMessageControl();

        }

        #endregion

        #region --- Events ---------------------------------------------

        /// <summary>
        /// Event(Help Message Change)
        /// </summary>
        public event EventHandler HelpMessageChanged;

        #endregion

        #region --- Public Methods -------------------------------------

        /// <summary>
        /// Stop Help message timer.
        /// </summary>
        public void HelpMessageTimerStop()
        {
            _helpMessageControl.Enabled = false;
        }

        /// <summary>
        /// Update parameters after validation.
        /// </summary>
        /// <returns>Result of validation.</returns>
        public string UpdateParameter()
        {
            ResamplingParameter.InterpolationAlgorithm algorithm;
            ResamplingParameter.SamplingMode samplingMode;
            double? interval, rangeMin = null, rangeMax = null;
            string errMsg = "";
            string errorMessage = string.Empty;

            //Interpolation
            algorithm = _rbnLinear.Checked
                ? ResamplingParameter.InterpolationAlgorithm.LINEAR
                : ResamplingParameter.InterpolationAlgorithm.PCHIP;

            //Sampling Points
            if (_rbnSame.Checked)
            {
                samplingMode = ResamplingParameter.SamplingMode.FIRST;
            }
            else
            {
                if (_rbnAverage.Checked)
                {
                    samplingMode = ResamplingParameter.SamplingMode.AVERAGE;
                }
                else if (_rbnMinimum.Checked)
                {
                    samplingMode = ResamplingParameter.SamplingMode.MINIMUIM;
                }
                else //_rbnSpecified.Checked
                {
                    samplingMode = ResamplingParameter.SamplingMode.SPECIFIED;
                }
            }

            //Specified Interval
            interval = null;
            if ((_parameter.ParseSpecifiedInterval(_txtSpecifiedInterval.Text, ref errMsg, ref interval, samplingMode) == false)
                || (_parameter.IsValidSpecifiedIntervalRange(0, 10, interval, samplingMode) == false))
            {
                errorMessage += ERROR_INTERVAL;
            }

            //Output Range(Max)
            if (_txtRangeMax.Text == string.Empty)
            {
                rangeMax = null;
            }
            else
            {
                try
                {
                    rangeMax = double.Parse(_txtRangeMax.Text, NumberStyles.Float);
                }
                catch
                {
                    errorMessage += ERROR_LOWER_RANGE;
                }
            }

            //Output Range(Min)
            if (_txtRangeMin.Text == string.Empty)
            {
                rangeMin = null;
            }
            else
            {
                try
                {
                    rangeMin = double.Parse(_txtRangeMin.Text, NumberStyles.Float);
                }
                catch
                {
                    errorMessage += ERROR_HIGHER_RANGE;
                }
            }

            //Range validation(if invalid, exchange both range limit. no return false.)
            if (rangeMin > rangeMax)
            {
                double? temp = rangeMin;
                rangeMin = rangeMax;
                rangeMax = temp;
            }
            //if specified range width is 0, return false.
            else if ((rangeMin != null)
                && (rangeMin == rangeMax))
            {
                errorMessage += ERROR_BOTH_ENDS_OF_RANGE_IS_SAME;
            }

            if (rangeMin < 0 || rangeMax < 0)
            {
                errorMessage += ERROR_MINUS_RANGE;
            }

            if (errorMessage == string.Empty)
            {
                //Update parameters.
                _parameter.Interpolation = algorithm;
                _parameter.ResamplingMode = samplingMode;
                _parameter.SpecifiedInterval = interval;
                _parameter.OutputLowerRange = rangeMin;
                _parameter.OutputHigherRange = rangeMax;
            }

            return errorMessage;
        }

        /// <summary>
        /// Validate all paramters.
        /// </summary>
        /// <returns>Result of validation</returns>
        public bool ValidateParameter()
        {
            ResamplingParameter.InterpolationAlgorithm algorithm;
            double? interval, rangeMin, rangeMax;

            try
            {

                //Specified Interval
                if (_txtSpecifiedInterval.Text == string.Empty)
                {
                    interval = null;
                }
                else
                {
                    interval = double.Parse(_txtSpecifiedInterval.Text, NumberStyles.Float);
                }

                //Output Range(Max)
                if (_txtRangeMax.Text == string.Empty)
                {
                    rangeMax = null;
                }
                else
                {
                    rangeMax = double.Parse(_txtRangeMax.Text, NumberStyles.Float);
                }

                //Output Range(Min)
                if (_txtRangeMin.Text == string.Empty)
                {
                    rangeMin = null;
                }
                else
                {
                    rangeMin = double.Parse(_txtRangeMin.Text, NumberStyles.Float);
                }

                //Range validation(if invalid, exchange both range limit. no return false.)
                if (rangeMin < rangeMax)
                {
                    double? temp = rangeMin;
                    rangeMin = rangeMax;
                    rangeMax = temp;
                }
                //if specified range width is 0, return false.
                else if ((rangeMin != null)
                    && (rangeMin == rangeMax))
                {
                    return false;
                }

                //Interpolation
                algorithm = _rbnLinear.Checked
                    ? ResamplingParameter.InterpolationAlgorithm.LINEAR
                    : ResamplingParameter.InterpolationAlgorithm.PCHIP;

                //Sampling Points
                if (!_rbnSame.Checked)
                {
                    if ((!_rbnAverage.Checked)
                        && (!_rbnMinimum.Checked)
                        && (!_rbnSpecified.Checked))
                    {
                        return false;
                    }
                }
            }
            catch
            {
                return false;
            }
            return true;
        }

        #endregion

        #region --- Private Methods ------------------------------------

        /// <summary>
        /// Occurs when change of Help Message.
        /// </summary>
        /// <param name="message">Message</param>
        private void HelpCtl_ChangeMessage(string message)
        {
            string chngMessage = message;
            _helpMessage = string.Format(chngMessage);
            HelpMessageChanged(this, EventArgs.Empty);
        }

        #endregion

        #region --- Private Events -------------------------------------

        /// <summary>
        /// Occurs when this is loaded.
        /// 
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">EventArgs</param>
        private void ResamplingPanel_Load(object sender, EventArgs e)
        {
            _rbnSame.CheckedChanged += SamplingPoints_CheckedChanged;
            _rbnRegular.CheckedChanged += SamplingPoints_CheckedChanged;

            _rbnAverage.CheckedChanged += RegularInterval_CheckedChanged;
            _rbnMinimum.CheckedChanged += RegularInterval_CheckedChanged;
            _rbnSpecified.CheckedChanged += RegularInterval_CheckedChanged;

            _helpMessageControl.ClearList();
            _helpMessageControl.AddList(_rbnSame, Resource.HELP_MEG_RBN_SAME);
            _helpMessageControl.AddList(_rbnRegular, Resource.HELP_MEG_RBN_REGULAR);
            _helpMessageControl.AddList(_rbnAverage, Resource.HELP_MEG_RBN_AVERAGE);
            _helpMessageControl.AddList(_rbnMinimum, Resource.HELP_MEG_RBN_MINIMUM);
            _helpMessageControl.AddList(_rbnSpecified, Resource.HELP_MEG_RBN_SPECIFIED);
            _helpMessageControl.AddList(_txtSpecifiedInterval, Resource.HELP_MEG_TXT_SPECIFIED);
            _helpMessageControl.AddList(_txtRangeMax, Resource.HELP_MEG_TXT_OUTPUTRANGE);
            _helpMessageControl.AddList(_txtRangeMin, Resource.HELP_MEG_TXT_OUTPUTRANGE);
            _helpMessageControl.AddList(_rbnLinear, Resource.HELP_MEG_RBN_LINEAR);
            _helpMessageControl.AddList(_rbnPCHIP, Resource.HELP_MEG_RBN_PCHIP);
            _helpMessageControl.ChangeMessage += HelpCtl_ChangeMessage;
            _helpMessageControl.ScanInterval = 100;
            _helpMessageControl.Enabled = true;

            _rbnSame.Checked = _parameter.ResamplingMode == ResamplingParameter.SamplingMode.FIRST;
            _rbnRegular.Checked = !_rbnSame.Checked;
            _rbnAverage.Checked = true;
            _rbnAverage.Enabled = !_rbnSame.Checked;
            _rbnMinimum.Checked = _parameter.ResamplingMode == ResamplingParameter.SamplingMode.MINIMUIM;
            _rbnMinimum.Enabled = !_rbnSame.Checked;
            _rbnSpecified.Checked = _parameter.ResamplingMode == ResamplingParameter.SamplingMode.SPECIFIED;
            _rbnSpecified.Enabled = !_rbnSame.Checked;
            _txtSpecifiedInterval.Enabled = _rbnSpecified.Checked;
            _txtSpecifiedInterval.Text = _parameter.SpecifiedInterval.ToString();
            _txtRangeMin.Text = _parameter.OutputLowerRange.ToString();
            _txtRangeMax.Text = _parameter.OutputHigherRange.ToString();
            _rbnLinear.Checked = _parameter.Interpolation == ResamplingParameter.InterpolationAlgorithm.LINEAR;
            _rbnPCHIP.Checked = !_rbnLinear.Checked;
        }

        /// <summary>
        /// Occurs when change of sampling points radiobox is checked.
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">EventArgs</param>
        void SamplingPoints_CheckedChanged(object sender, EventArgs e)
        {
            _rbnAverage.Enabled = !_rbnSame.Checked;
            _rbnMinimum.Enabled = !_rbnSame.Checked;
            _rbnSpecified.Enabled = !_rbnSame.Checked;
            _txtSpecifiedInterval.Enabled = (!_rbnSame.Checked && _rbnSpecified.Checked);
        }

        /// <summary>
        /// Occurs when change of Regular Interval radiobutton is checked.
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">EventArgs</param>
        void RegularInterval_CheckedChanged(object sender, EventArgs e)
        {
            _txtSpecifiedInterval.Enabled = (_rbnRegular.Checked && _rbnSpecified.Checked);
        }

        #endregion

    }
}
