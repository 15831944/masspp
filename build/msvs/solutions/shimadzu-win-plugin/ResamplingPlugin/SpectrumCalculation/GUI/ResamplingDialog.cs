/// <summary>
/// [FILE] ResamplingDialog.cs
/// [ABSTRACT] Resampling plugin - Form class for Resampling Dialog
/// Copyright (C) 2013-08-23 Shimadzu
/// </summary>
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using ResamplingPlugin.ResamplingTools.GUI;
using kome.clr;
using System.Diagnostics;
using System.Text.RegularExpressions;
using ResamplingPlugin.ResamplingTools.Data;

namespace ResamplingPlugin.SpectrumCalculation.GUI
{
    /// <summary>ResamplingDialog class</summary>
    public partial class ResamplingDialog : Form
    {
        #region --- Constants --------------------------------------------
        /// <summary>
        /// String of Illegal characters.
        /// </summary>
        protected const string INVALID_CHARACTER_FILTER = @"^([1-9]\d*|0)(\.\d+)?$";

        #endregion

        #region --- Variables ------------------------------------------

        /// <summary>ResamplingPanel instance</summary>
        private ResamplingPanel _panel;

        /// <summary>
        /// Resampling Parameter
        /// </summary>
        private ResamplingParameter _resamplingParameter;

        #endregion

        #region --- Construction ---------------------------------------

        /// <summary>
        /// Initializes a new instance of the ResamplingDialog class
        /// </summary>
        /// <param name="param">ResamplingParameter</param>
        public ResamplingDialog(ResamplingParameter param)
        {
            InitializeComponent();
            this._resamplingParameter = param;
        }

        #endregion

        #region --- Public Methods -------------------------------------

        /// <summary>
        /// Update parameters.
        /// </summary>
        /// <param name="plotData">plot data</param>
        public void UpdateParam(ClrVariant clrVar)
        {
        }

        #endregion

        #region --- Private Events -------------------------------------

        /// <summary>
        /// Occurs when this is loaded.
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">EventArgs</param>
        private void ResamplingDialog_Load(object sender, EventArgs e)
        {
            _panel = new ResamplingPanel(this, _resamplingParameter);

            Controls.Add(_panel);

            _panel.Show();

            _panel.HelpMessageChanged += new EventHandler(Panel_HelpMessageChanged);

            if (_resamplingParameter.IsIniFileExist && _resamplingParameter.ErrorMessage != null)
            {
                MessageBox.Show(_resamplingParameter.ErrorMessage, "INI File Read Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }
        }

        /// <summary>
        /// Occurs when this is closed.
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">FormClosedEventArgs</param>
        private void ResamplingDialog_FormClosed(object sender, FormClosedEventArgs e)
        {
            _panel.HelpMessageTimerStop();
        }

        /// <summary>
        /// Occurs when help message is changed.
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">EventArgs</param>
        private void Panel_HelpMessageChanged(object sender, EventArgs e)
        {
            _textBoxHelpMessage.Text = _panel.HelpMessage;
        }

        /// <summary>
        /// Occurs when cancel button is clicked.
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">EventArgs</param>
        private void BtnCancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        /// <summary>
        /// Occurs when OK button is clicked.
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">EventArgs</param>
        private void BtnOK_Click(object sender, EventArgs e)
        {
            string errorMessage;
            errorMessage = _panel.UpdateParameter();

            if (errorMessage != string.Empty)
            {
                MessageBox.Show(errorMessage, "Invalid Parameter", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return;
            }


            _resamplingParameter.WriteParameterIni();
            this.Close();
        }

        #endregion

    }
}
