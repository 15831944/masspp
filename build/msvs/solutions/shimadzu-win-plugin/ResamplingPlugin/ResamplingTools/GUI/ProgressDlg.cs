/// <summary>
/// [FILE] ProgressDlg.cs
/// [ABSTRACT] Resampling plugin - Progress dialog form class
/// Copyright (C) 2013-05-23 Shimadzu
/// </summary>

using System;
using System.Windows.Forms;

namespace ResamplingPlugin.ResamplingTools.GUI
{
    /// <summary>
    /// ProgressDlg class
    /// </summary>
    public partial class ProgressDlg : Form
    {
        #region --- Construction ---------------------------------------
        /// <summary>standard construction</summary>
        public ProgressDlg()
        {
            InitializeComponent();
        }
        #endregion

        #region --- Variables ------------------------------------------
        /// <summary>value added to progress bar current value at timer event,if auto count timer is used
        /// </summary>
        private int _autoCountAddValue;
        #endregion

        #region --- Properties -----------------------------------------
        /// <summary>progress bar max value</summary>
        public int PrgMax
        {
            set { _prgBar.Maximum = value; }
            get { return _prgBar.Maximum; }
        }
        /// <summary>progress bar min value</summary>
        public int PrgMin
        {
            set { _prgBar.Minimum = value; }
            get { return _prgBar.Minimum; }
        }
        /// <summary>progress bar current value</summary>
        public int PrgValue
        {
            set { _prgBar.Value = value; }
            get { return _prgBar.Value; }
        }
        /// <summary>cancel button enable</summary>
        public bool CancelEnabled
        {
            set { _btnCancel.Enabled = value; }
            get { return _btnCancel.Enabled; }
        }
        /// <summary>Auto count timer enable</summary>
        public bool AutoCountEnabled
        {
            set { _AutoCountTimer.Enabled = value; }
            get { return _AutoCountTimer.Enabled; }
        }
        /// <summary>Auto count timer interval</summary>
        public int AutoCountInterval
        {
            set { _AutoCountTimer.Interval = value; }
            get { return _AutoCountTimer.Interval; }
        }
        /// <summary>Auto count timer value</summary>
        public int AutoCountAddValue
        {
            set { _autoCountAddValue = value; }
            get { return _autoCountAddValue; }
        }
        #endregion

        #region --- Events ---------------------------------------------
        /// <summary>Cancel button click event(for sending to ohter class)</summary>
        public event EventHandler PrgCancel;
        #endregion

        #region --- Private Events -------------------------------------
        /// <summary>
        /// cancle button click event handler
        /// </summary>
        /// <param name="sender">object</param>
        /// <param name="e">EventArgs</param>
        private void BtnCancel_Click(object sender, EventArgs e)
        {
            if (PrgCancel != null)
                PrgCancel(this, e);
        }

        /// <summary>
        /// AutoCountTimer Tick event handler
        /// </summary>
        /// <param name="sender">object</param>
        /// <param name="e">EventArgs</param>
        private void AutoCountTimer_Tick(object sender, EventArgs e)
        {
            int val = _prgBar.Value + _autoCountAddValue;

            if (val >= _prgBar.Maximum)
            {
                val = _prgBar.Maximum;
                _AutoCountTimer.Enabled = false;
            }
            _prgBar.Value = val;
        }
        #endregion

        #region --- Public Methods -------------------------------------
        /// <summary>set progress bar current value(for thread)</summary>
        /// <param name="value">value</param>
        public void SetValue(int value)
        {
            if (this._prgBar.InvokeRequired)
            {
                _prgBar.Invoke(new MethodInvoker(() => { SetValue(value); }));
                return;
            }
            _prgBar.Value = value;
        }
        /// <summary>set label text(for thread)</summary>
        /// <param name="value">text</param>
        public void SetLabelText(string value)
        {
            if (this._prgBar.InvokeRequired)
            {
                _prgBar.Invoke(new MethodInvoker(() => { SetLabelText(value); }));
                return;
            }
            _lblMessage.Text = value;
        }
        #endregion

    }
}
