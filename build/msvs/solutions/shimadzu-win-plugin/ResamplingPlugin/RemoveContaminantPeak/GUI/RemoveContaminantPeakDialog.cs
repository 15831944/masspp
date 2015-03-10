/// <summary>
/// [FILE] RemoveContaminantPeakDialog.cs
/// [ABSTRACT]  Resampling plugin - Remove Contaminant Peak Dialog Form class
/// Copyright (C) 2013-04-22 Shimadzu
/// </summary>

using System;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Text.RegularExpressions;
using System.Windows.Forms;

// namespace to use CLR from Mass++
using kome.clr;
using ResamplingPlugin.ResamplingTools.GUI;
using ResamplingPlugin.RemoveContaminantPeak.Cal;
using ResamplingPlugin.RemoveContaminantPeak.Data;
using ResamplingPlugin.ResamplingTools.Data;
using ResamplingPlugin.ResamplingTools.Cal;
using System.Threading;

namespace ResamplingPlugin.RemoveContaminantPeak.GUI
{
    /// <summary>
    /// Remove Contaminant Peak Dialog Form class.
    /// </summary>
    public partial class RemoveContaminantPeakDialog : Form
    {
        #region --- Win32 API ------------------------------------------

        /// <summary>
        /// SetWindowLong
        /// </summary>
        /// <param name="hWnd">Window handle</param>
        /// <param name="nIndex">index</param>
        /// <param name="dwNewLong">new long</param>
        /// <returns></returns>
        [DllImport("user32.dll")]
        static extern int SetWindowLong(IntPtr hWnd, int nIndex, UInt32 dwNewLong);

        /// <summary>
        /// Parent window handle.
        /// </summary>
        protected const int GWL_HWNDPARENT = (-8);

        #endregion

        #region --- Defines --------------------------------------------

        /// <summary>
        /// string of linear baseline.
        /// </summary>
        protected const string LINEAR_BASELINE = "Linear Baseline";
        
        /// <summary>
        /// string of abc baseline.
        /// </summary>
        protected const string ABC_BASELINE = "ABC Baseline";

        #endregion

        #region --- Variables ------------------------------------------

        /// <summary>
        /// Remove contaminant peak calculator.
        /// </summary>
        private RemoveContaminantPeakCalculation _calculate;

        /// <summary>
        /// Parameters for baseline subtraction.
        /// </summary>
        private BaselineSubtractionParameter _baselineParam;

        /// <summary>
        /// Parameters for resampling.
        /// </summary>
        private ResamplingParameter _resamplingParam;

        /// <summary>ResamplingPanel instance</summary>
        private ResamplingPanel _panel;

        /// <summary>Parameter</summary>
        private ClrVariant _clrVar;
                
        /// <summary>
        /// Smooth width control.
        /// </summary>
        private System.Windows.Forms.NumericUpDown _numSmoothingWidth;

        /// <summary>
        /// Noise factor control.
        /// </summary>
        private System.Windows.Forms.TextBox _txtNoiseFactor;

        /// <summary>
        /// ABC baseline window width control.
        /// </summary>
        private System.Windows.Forms.TextBox _txtABCBaseLineWindowWidth;

        /// <summary>
        /// Linear baseline window width control.
        /// </summary>
        private System.Windows.Forms.TextBox _txtLinearBaseLineWindowWidth;

        /// <summary>
        /// Smoothing width label.
        /// </summary>
        private System.Windows.Forms.Label _lblSmootingWidth;

        /// <summary>
        /// Noise factor label.
        /// </summary>
        private System.Windows.Forms.Label _lblNoiseFactor;

        /// <summary>
        /// Window width label.
        /// </summary>
        private System.Windows.Forms.Label _lblWindowWidth;

        /// <summary>
        /// Dialog for showing progress of remove contaminant peak.
        /// </summary>
        private ProgressDlg _dlgProgress;

        /// <summary>Thread Class Variable for Spectrum Caluculation</summary>
        private Thread _threadCal;

        #endregion
        
        #region --- Construction ---------------------------------------

        /// <summary>
        /// Initializes a new instance of the SpectrumCalculationDialog class
        /// </summary>
        public RemoveContaminantPeakDialog()
        {
            this.CreateHandle();

            InitializeComponent();
        }

        /// <summary>
        /// Initializes a new instance of the SpectrumCalculationDialog class
        /// </summary>
        /// <param name="clrVar">Parameter</param>
        public RemoveContaminantPeakDialog(ClrVariant clrVar)
            : this()
        {
            // Set parent window
            var current = Process.GetCurrentProcess();
            IntPtr hwnd = current.MainWindowHandle;

            SetWindowLong(this.Handle, GWL_HWNDPARENT, (UInt32)hwnd);

            _clrVar = clrVar;
            _baselineParam = new BaselineSubtractionParameter();
            _resamplingParam = new ResamplingParameter();
            _calculate = new RemoveContaminantPeakCalculation(_clrVar,_baselineParam, _resamplingParam);
        }

        #endregion

        #region --- Events ---------------------------------------------
        
        /// <summary>
        /// Event(Form closed)
        /// </summary>
        public event Action<object, FormClosedEventArgs> DisplayClosed;
        
        #endregion

        #region --- Public Methods -------------------------------------

        /// <summary>
        /// Update
        /// </summary>
        /// <param name="plotData">plot data</param>
        public void UpdateParam(ClrVariant clrVar)
        {
        }

        #endregion

        #region --- Private Methods ------------------------------------

        /// <summary>
        /// Switch visibility of controls for baseline subtractions.
        /// </summary>
        private void ChangeBaseLineShowItem()
        {
            bool mode = (_cmbBaseLine.Text == ABC_BASELINE);
            _lblWindowWidth.Visible = true;
            _txtABCBaseLineWindowWidth.Visible = mode;
            _txtLinearBaseLineWindowWidth.Visible = !mode;
            _txtNoiseFactor.Visible = mode;
            _lblNoiseFactor.Visible = mode;
            _numSmoothingWidth.Visible = mode;
            _lblSmootingWidth.Visible = mode;

        }

        /// <summary>
        /// Check Validate Data
        /// </summary>
        /// <returns>Validate Message</returns>
        private string ValidateParameters()
        {
            string errorMessage = string.Empty;
            double tempDouble = 0;
            int tmpint = 0;

            if (!double.TryParse(_txtABCBaseLineWindowWidth.Text, out tempDouble))
            {
                errorMessage += "(ABC BaseLine Window Width)Please input a number.\n";
            }

            if (tempDouble < 0)
            {
                errorMessage += "(ABC BaseLine Window Width)Please input the positive real number.\n";
            }

            if (!double.TryParse(_txtLinearBaseLineWindowWidth.Text, out tempDouble))
            {
                errorMessage += "(Linear BaseLine Window Width)Please input a number.\n";
            }

            if (tempDouble < 0)
            {
                errorMessage += "(Linear BaseLine Window Width)Please input the positive real number.\n";
            }

            if (!double.TryParse(_txtNoiseFactor.Text, out tempDouble))
            {
                errorMessage += "(NoiseFactor)Please input a number.\n";
            }

            if (tempDouble < 0)
            {
                errorMessage += "(NoiseFactor)Please input the positive real number.\n";
            }

            if (!int.TryParse(_numSmoothingWidth.Value.ToString(), out tmpint))
            {
                errorMessage += "(Smoothing Width)Please input a number.\n";
            }

            return errorMessage;
        }

        /// <summary>
        /// Calculate remove contaminant peak.
        /// </summary>
        private void ThreadCalc()
        {
            try
            {
                _calculate.Execute();
            }
            catch (OutOfMemoryException e)
            {
                if (_calculate.Result != null)
                {
                    _calculate.Result.ClearResult();
                }
                GC.Collect();
                MessageBox.Show("OutOfMemoryException !");
                throw e;
            }
            finally
            {
                ProgDlgClose();
            }
        }

        /// <summary>
        /// close progress dialog
        /// </summary>
        private void ProgDlgClose()
        {
            if (this.InvokeRequired)
            {
                _dlgProgress.Invoke(new MethodInvoker(() => { ProgDlgClose(); }));
                return;
            }

            _dlgProgress.CancelEnabled = true;
            _dlgProgress.AutoCountEnabled = false;
            _dlgProgress.Close();
        }




        #region Create Component
        /// <summary>
        /// Create Component
        /// </summary>
        private void CreateComponent()
        {
            _numSmoothingWidth = new NumericUpDown();
            _numSmoothingWidth.Location = new System.Drawing.Point(136, 149);
            _numSmoothingWidth.Size = new System.Drawing.Size(105, 19);
            _numSmoothingWidth.Visible = false;
            _numSmoothingWidth.KeyPress += new KeyPressEventHandler(_NumSmoothingWidth_KeyPress);
            _numSmoothingWidth.Maximum = 999999999;
            

            _txtNoiseFactor = new TextBox();
            _txtNoiseFactor.Location = new System.Drawing.Point(127, 113);
            _txtNoiseFactor.Size = new System.Drawing.Size(88, 19);
            _txtNoiseFactor.Visible = false;

            _txtABCBaseLineWindowWidth = new TextBox();
            _txtABCBaseLineWindowWidth.Location = new System.Drawing.Point(136, 76);
            _txtABCBaseLineWindowWidth.Size = new System.Drawing.Size(88, 19);
            _txtABCBaseLineWindowWidth.Visible = false;

            _txtLinearBaseLineWindowWidth = new TextBox();
            _txtLinearBaseLineWindowWidth.Location = new System.Drawing.Point(136, 76);
            _txtLinearBaseLineWindowWidth.Size = new System.Drawing.Size(88, 19);
            _txtLinearBaseLineWindowWidth.Visible = false;

            _lblSmootingWidth = new Label();
            _lblSmootingWidth.AutoSize = true;
            _lblSmootingWidth.Location = new System.Drawing.Point(29, 149);
            _lblSmootingWidth.Size = new System.Drawing.Size(90, 12);
            _lblSmootingWidth.Text = "Smoothing Width";
            _lblSmootingWidth.Visible = false;

            _lblNoiseFactor = new Label();
            _lblNoiseFactor.AutoSize = true;
            _lblNoiseFactor.Location = new System.Drawing.Point(29, 113);
            _lblNoiseFactor.Size = new System.Drawing.Size(71, 12);
            _lblNoiseFactor.Text = "Noise Factor";
            _lblNoiseFactor.Visible = false;

            _lblWindowWidth = new Label();
            _lblWindowWidth.AutoSize = true;
            _lblWindowWidth.Location = new System.Drawing.Point(29, 79);
            _lblWindowWidth.Size = new System.Drawing.Size(75, 12);
            _lblWindowWidth.Text = "Window Width";
            _lblWindowWidth.Visible = false;

            this._grpBaseLine.Controls.Add(_numSmoothingWidth);
            this._grpBaseLine.Controls.Add(_txtNoiseFactor);
            this._grpBaseLine.Controls.Add(_txtABCBaseLineWindowWidth);
            this._grpBaseLine.Controls.Add(_txtLinearBaseLineWindowWidth);
            this._grpBaseLine.Controls.Add(_lblNoiseFactor);
            this._grpBaseLine.Controls.Add(_lblWindowWidth);
            this._grpBaseLine.Controls.Add(_lblSmootingWidth);


        }


        #endregion

        #endregion

        #region --- Private Events -------------------------------------
        
        /// <summary>
        /// Occurs when this is loaded.
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">EventArgs</param>
        private void RemoveContaminantPeakDialog_Load(object sender, EventArgs e)
        {
            string errorMessage = string.Empty;
            _panel = new ResamplingPanel(this, _resamplingParam);
            _panel.Left = 5;
            _panel.Top = 11;
            _panel.HelpMessageChanged += Panel_HelpMessageChanged;

            _grpResampling.Controls.Add(_panel);
            CreateComponent();

            if (_resamplingParam.IsIniFileExist && _resamplingParam.ErrorMessage != null)
            {
                errorMessage += _resamplingParam.ErrorMessage;
                //MessageBox.Show(_resamplingParam.ErrorMessage, "INI File Read Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }


            if(_baselineParam.ReadParameterIni() != string.Empty)
            {
                errorMessage += _baselineParam.ReadParameterIni();
                MessageBox.Show(errorMessage, "INI File Read Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }

            _cmbBaseLine.SelectedIndex =
                (_baselineParam.Algorithm == BaselineSubtractionParameter.ALGORITHM_ABC) ? 0 : 1;
            _txtABCBaseLineWindowWidth.Text = _baselineParam.ABCWindowWidth.ToString("#0.0");
            _txtLinearBaseLineWindowWidth.Text = _baselineParam.LinearWindowWidth.ToString("#0.0");
            _txtNoiseFactor.Text = _baselineParam.NoiseFactor.ToString("#0.0");
            if (_baselineParam.SmoothingWidth < _numSmoothingWidth.Maximum && _baselineParam.SmoothingWidth > _numSmoothingWidth.Minimum)
            {
                _numSmoothingWidth.Value = _baselineParam.SmoothingWidth;
            }
            else
            {
                _numSmoothingWidth.Value = 10;
            }
            ChangeBaseLineShowItem();

            //Run Load Event 
            _panel.Show();

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
        /// Occurs when this is closed.
        /// </summary>
        /// <param name="sender">object</param>
        /// <param name="e">EventArgs</param>
        private void RemoveContaminantPeakDialog_FormClosed(object sender, FormClosedEventArgs e)
        {
            _panel.HelpMessageTimerStop();
            DisplayClosed(sender, e);
        }

        /// <summary>
        /// Occurs when text of Baseline algorithm combobox is changed.
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">EventArgs</param>
        private void CmbBaseLine_TextChanged(object sender, EventArgs e)
        {
            ChangeBaseLineShowItem();
        }

        /// <summary>
        /// Occurs when default button is clicked.
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">EventArgs</param>
        private void BtnDefault_Click(object sender, EventArgs e)
        {
            _baselineParam.Default();
            _txtABCBaseLineWindowWidth.Text = _baselineParam.ABCWindowWidth.ToString("#0.0");
            _txtLinearBaseLineWindowWidth.Text = _baselineParam.LinearWindowWidth.ToString("#0.0");
            _txtNoiseFactor.Text = _baselineParam.NoiseFactor.ToString("#0.0");
            _numSmoothingWidth.Value = _baselineParam.SmoothingWidth;
            ChangeBaseLineShowItem();

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
            string errorMessage = string.Empty;
            errorMessage = _panel.UpdateParameter();
            errorMessage += ValidateParameters();

            if (errorMessage != string.Empty)
            {
                MessageBox.Show(errorMessage, "Invalid Parameter", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return;
            }


            //if thread is already exists, abort it.
            if (_threadCal != null)
            {
                _threadCal.Abort();
                _threadCal = null;
            }

            //Save parameters.
            _resamplingParam.WriteParameterIni();
            _baselineParam.WriteParameterIni();

            //Prepare to calculate.
            //Create thread for dialog.
            _threadCal = new System.Threading.Thread(ThreadCalc);
            _threadCal.Priority = System.Threading.ThreadPriority.Lowest;

            //Create dialog.
            if (_dlgProgress == null)
            {
                _dlgProgress = new ProgressDlg();
                _dlgProgress.CancelEnabled = true;
                _dlgProgress.PrgCancel += DlgProgress_PrgCancel;
                _calculate.UpdateProgress += UpdateRemoveCalcProgress;
                _calculate.UpdateResamplingProgressTemp += UpdateSpecCalcResamplingProgress;
            }

            //Start calculate.
            _threadCal.Start(); 
            _dlgProgress.ShowDialog();

            _calculate.UpdateResamplingProgressTemp -= UpdateSpecCalcResamplingProgress;
            _dlgProgress = null;


            if (_calculate.Result != null)
            {
                _calculate.SubmitResultToTree();
            }

        }

        /// <summary>
        /// Progress Dialog Cancel Button Click event handler
        /// </summary>
        /// <param name="sender">object</param>
        /// <param name="e">EventArgs</param>
        private void ProcCancel(object sender, EventArgs e)
        {
            _dlgProgress.AutoCountEnabled = false;
            _dlgProgress.CancelEnabled = false;
            _dlgProgress.SetLabelText("Stopping Remove contaminant peak thread.");
            _calculate.StopFlag = true;
        }

        /// <summary>
        /// Progress Dialog Cancel Button Click event handler
        /// </summary>
        /// <param name="sender">object</param>
        /// <param name="e">EventArgs</param>
        private void DlgProgress_PrgCancel(object sender, EventArgs e)
        {
            _dlgProgress.AutoCountEnabled = false;
            _dlgProgress.CancelEnabled = false;
            _dlgProgress.SetLabelText("Stopping Spectrum Calculation thread.");
            _calculate.StopFlag = true;
        }



         /// <summary>
        /// Update dialog.
        /// </summary>
        /// <param name="id">Progress</param>
        /// <param name="cursmplnum"></param>
        /// <param name="maxsmplnum"></param>
        private void UpdateRemoveCalcProgress(
            RemoveContaminantPeakCalculation.PROGRESS_REMOVECONTAMINANT_PHASE id,
            int cursmplnum,
            int maxsmplnum
            )
        {
            string msg = "";
            if (_dlgProgress == null)
            {
                return;
            }

            if(this.InvokeRequired)
            {
                try
                {
                    _dlgProgress.Invoke(
                        new MethodInvoker(() => UpdateRemoveCalcProgress(id, cursmplnum, maxsmplnum))
                        );
                }
                catch
                {

                }
                return;
            }

            switch (id)
            {
                case RemoveContaminantPeakCalculation.PROGRESS_REMOVECONTAMINANT_PHASE.PREPARE:
                    msg = "Prepare for removing contaminant peak.";
                    _dlgProgress.SetLabelText(msg);
                    _dlgProgress.PrgMax = 10 * 60;
                    _dlgProgress.AutoCountInterval = 1000;
                    _dlgProgress.AutoCountAddValue = 1;
                    _dlgProgress.AutoCountEnabled = true;
                    break;

                case RemoveContaminantPeakCalculation.PROGRESS_REMOVECONTAMINANT_PHASE.REMOVE_CONTAMINANT_PEAK:
                    msg = string.Format(
                        "Removing contaminant peak. ({0}/{1}) ", cursmplnum.ToString(), maxsmplnum.ToString());
                    _dlgProgress.SetLabelText(msg);
                    _dlgProgress.PrgMax = maxsmplnum;
                    _dlgProgress.PrgValue = cursmplnum;
                    _dlgProgress.CancelEnabled = true;
                    _dlgProgress.AutoCountEnabled = false;
                    break;

                case RemoveContaminantPeakCalculation.PROGRESS_REMOVECONTAMINANT_PHASE.WRITE_TO_TEMPORARY:
                    msg = string.Format(
                        "Write to temporary file. ({0}/{1}) ", cursmplnum.ToString(), maxsmplnum.ToString());
                    _dlgProgress.SetLabelText(msg);
                    _dlgProgress.PrgMax = maxsmplnum;
                    _dlgProgress.PrgValue = cursmplnum;
                    _dlgProgress.CancelEnabled = true;
                    _dlgProgress.AutoCountEnabled = false;
                    break;

                case RemoveContaminantPeakCalculation.PROGRESS_REMOVECONTAMINANT_PHASE.READ_FROM_TEMPORARY:
                    msg = string.Format(
                        "Read from temporary file. ({0}/{1}) ", cursmplnum.ToString(), maxsmplnum.ToString());
                    _dlgProgress.SetLabelText(msg);
                    _dlgProgress.PrgMax = maxsmplnum;
                    _dlgProgress.PrgValue = cursmplnum;
                    _dlgProgress.CancelEnabled = true;
                    _dlgProgress.AutoCountEnabled = false;
                    break;

                case RemoveContaminantPeakCalculation.PROGRESS_REMOVECONTAMINANT_PHASE.COMPLETE:
                    msg = "Complete.";
                    _dlgProgress.SetLabelText(msg);
                    _dlgProgress.CancelEnabled = false;
                    _dlgProgress.AutoCountEnabled = false;
                    break;

                default:
                    break;
            }
        }


        /// <summary>
        /// Update Spectrum Calculation(Resampling) Progress
        /// </summary>
        /// <param name="id">PROGRESS_RESAMPLINGT_PHASE id</param>
        /// <param name="value">progress bar value or max</param>
        /// <param name="cursmplnum">current number for text</param>
        /// <param name="maxsmplnum">max number for text</param>
        private void UpdateSpecCalcResamplingProgress(ResamplingCalculation.PROGRESS_RESAMPLING_PHASE id, int value, int cursmplnum, int maxsmplnum)
        {
            string msg = "";

            if (_dlgProgress == null) return;

            if (this.InvokeRequired)
            {
                _dlgProgress.Invoke(new MethodInvoker(() => { UpdateSpecCalcResamplingProgress(id, value, cursmplnum, maxsmplnum); }));
                return;
            }

            switch (id)
            {
                case ResamplingCalculation.PROGRESS_RESAMPLING_PHASE.RESAMPLING_START:
                    msg = string.Format("Resampling ({0}/{1}) ", cursmplnum.ToString(), maxsmplnum.ToString());
                    _dlgProgress.SetLabelText(msg);
                    _dlgProgress.PrgValue = 0;
                    _dlgProgress.CancelEnabled = true;
                    _dlgProgress.PrgMax = 10 * 60;
                    _dlgProgress.AutoCountInterval = 1000;
                    _dlgProgress.AutoCountAddValue = 1;
                    _dlgProgress.AutoCountEnabled = true;
                    break;
                case ResamplingCalculation.PROGRESS_RESAMPLING_PHASE.RESAMPLING_DOING:
                    _dlgProgress.AutoCountEnabled = false;
                    _dlgProgress.PrgMax = 1;
                    _dlgProgress.PrgValue = _dlgProgress.PrgMax;
                    break;
                case ResamplingCalculation.PROGRESS_RESAMPLING_PHASE.RESAMPLING_END:
                    msg = string.Format("Resampling End ({0}/{1}) ", cursmplnum.ToString(), maxsmplnum.ToString());
                    _dlgProgress.SetLabelText(msg);
                    _dlgProgress.AutoCountEnabled = false;
                    _dlgProgress.PrgMax = 1;
                    _dlgProgress.PrgValue = _dlgProgress.PrgMax;
                    _dlgProgress.CancelEnabled = true;
                    break;
                default:
                    break;
            }
        }




        /// <summary>
        /// NumSmoothingWidth KeyPress Event.
        /// </summary>
        /// <param name="sender">object</param>
        /// <param name="e">EventArgs</param>
        void _NumSmoothingWidth_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar < '0' || '9' < e.KeyChar)
            {
                e.Handled = true;
            }
        }


        #endregion
    }
}

