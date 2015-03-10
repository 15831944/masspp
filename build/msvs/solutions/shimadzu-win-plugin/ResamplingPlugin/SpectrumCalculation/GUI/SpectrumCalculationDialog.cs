/// <summary>
/// [FILE] SpectrumCalculationDialog.cs
/// [ABSTRACT] Spectrum Calculation Dialog Form class
/// Copyright (C) 2013-04-22 Shimadzu
/// </summary>

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Diagnostics;
using System.Threading;

// namespace to use CLR from Mass++
using kome.clr;
using ResamplingPlugin.SpectrumCalculation.Data;
using ResamplingPlugin.ResamplingTools.Data;
using ResamplingPlugin.ResamplingTools.Other;
using ResamplingPlugin.ResamplingTools.GUI;
using ResamplingPlugin.ResamplingTools.Cal;

namespace ResamplingPlugin.SpectrumCalculation.GUI
{
    /// <summary>SpectrumCalculationDialog class</summary>
    public partial class SpectrumCalculationDialog : Form
    {
        #region --- Win32 API ------------------------------------------
        // SetWindowLong
        [DllImport("user32.dll")]
        static extern int SetWindowLong(IntPtr hWnd, int nIndex, UInt32 dwNewLong);

        const int GWL_HWNDPARENT = (-8);
        #endregion

        #region --- Constants ------------------------------------------

        /// <summary>
        /// GridColumn.
        /// </summary>
        public enum GRID_COLUM
        {
            /// <summary>File Name combobox</summary>
            FILENAME_COMBOBOX = 0,

            /// <summary>FileName selected Index</summary>
            SELID_TEXTBOX,

            /// <summary>Spectrum Name combobox</summary>
            SPECTRUMNAMeE_COMBOBOX,

            /// <summary>RT textbox</summary>
            RT_TEXTBOX,

            /// <summary>Precusor textbox</summary>
            PRECUSOR_TEXTBOX,

            /// <summary>RT combobox</summary>
            RT_COMBOBOX,

            /// <summary>Precusor combobox</summary>
            PRECURSOR_COMBOBOX,

            /// <summary>Spectrum Name  textbox</summary>
            SPECTRUMNAME_TEXTBOX,
        }

        /// <summary> progress ID </summary>
        public enum PROGRESS_PREPARATION_PHASE
        {
            PREPARE_START,
            PREPARE_END,
        }


        #endregion

        #region --- Variables ------------------------------------------

        /// <summary>Parameter</summary>
        private ClrVariant _clrVar;

        /// <summary>DataGridViewComboBoxEditingControl class instance</summary>
        private DataGridViewComboBoxEditingControl _dataGridViewComboBoxEditingCtrl;

        /// <summary>SpectrumCalculation class instance</summary>
        private SpectrumCalculation.Cal.SpectrumCalculation _spectrumCalculation;

        /// <summary>GridDataSpectrumCalculation class instance</summary>
        private GridDataSpectrumCalculation _gridDataSpectrumCalculation;

        /// <summary>HelpMessageControl class instance</summary>
        private HelpMessageControl _helpMessageControl;

        /// <summary>ProgressDlg class instance</summary>
        private ProgressDlg _dlgProgress;

        /// <summary>ResamplingParameter class instance</summary>
        private ResamplingParameter _resamplingParameter;

        /// <summary>ResamplingCalculation class instance</summary>
        private ResamplingCalculation _resamplingCalculation;

        /// <summary>Thread Class Variable for Spectrum Caluculation</summary>
        private Thread _threadCal;

        /// <summary>BindingSource for File Name combobox items</summary>
        private BindingSource _bindFileName;
        /// <summary>BindingSource for File Name combobox items of last row</summary>
        private BindingSource _bindFileNameBottom;
        /// <summary>DataGridViewRow class instance of last row</summary>
        private DataGridViewRow _bottomRow;

        #endregion

        #region --- Construction ---------------------------------------
        /// <summary>
        /// Initializes a new instance of the SpectrumCalculationDialog class
        /// </summary>
        public SpectrumCalculationDialog()
        {
            CreateHandle();

            InitializeComponent();

            _gridDataSpectrumCalculation = new GridDataSpectrumCalculation();
            _helpMessageControl = new HelpMessageControl();

            _resamplingParameter = new ResamplingParameter();

            _bindFileName = _gridDataSpectrumCalculation.GetFileNameList;
            _bindFileNameBottom = _gridDataSpectrumCalculation.GetFileNameListBottom;

            _bindFileName.ListChanged += new ListChangedEventHandler(FileNames_ListChanged);

            _bottomRow = new DataGridViewRow();
            _bottomRow.CreateCells(_spectrumCalculationGrid);
            DataGridViewComboBoxCell file = (DataGridViewComboBoxCell)(_bottomRow.Cells[(int)GRID_COLUM.FILENAME_COMBOBOX]);
            file.DataSource = _bindFileNameBottom;

            _spectrumCalculationGrid.Rows.Add(_bottomRow);
            DataGridViewComboBoxCell scanNameComboBoxCell = (DataGridViewComboBoxCell)_spectrumCalculationGrid.Rows[0].Cells[0];
            DataGridViewComboBoxColumn scanName_comboCol = (DataGridViewComboBoxColumn)_spectrumCalculationGrid.Columns[0];
            scanNameComboBoxCell.Value = scanNameComboBoxCell.Items[0];
        }


        /// <summary>
        /// Initializes a new instance of the SpectrumCalculationDialog class
        /// </summary>
        /// <param name="clrVar">Parameter</param>
        public SpectrumCalculationDialog(ClrVariant clrVar)
            : this()
        {
            // Set parent window
            var current = Process.GetCurrentProcess();
            IntPtr hwnd = current.MainWindowHandle;

            SetWindowLong(this.Handle, GWL_HWNDPARENT, (UInt32)hwnd);



            _spectrumCalculationGrid.DataError += delegate(object sender,
                                    DataGridViewDataErrorEventArgs e)
            {
                e.Cancel = true;
            };

            _clrVar = clrVar;



            ClrMsppManager msObj = ClrMsppManager.getInstance();
            ClrIniFile ini = msObj.getParameters();
            Debug.WriteLine(ini.getSection(0));

        }
        #endregion

        #region --- Destruction ---------------------------------------
        /// <summary>
        /// Destruction
        /// </summary>
        ~SpectrumCalculationDialog()
        {
            _clrVar = null;
        }

        #endregion

        #region --- Events ---------------------------------------------
        /// <summary>
        /// Event(Form closed)
        /// </summary>
        public event Action<object, FormClosedEventArgs> DisplayClosed;

        /// <summary> Event for Update Progressbar (Resampling)
        ///           (PROGRESS ID,max or current spectrum number,current sample number,max sample number)
        /// </summary>
        public event Action<PROGRESS_PREPARATION_PHASE, int, int, int> UpdatePrepareProgress;

        #endregion

        #region --- Private Events -------------------------------------

        /// <summary>
        /// File Name combobox List Changed Event
        /// </summary>
        /// <param name="sender">object</param>
        /// <param name="e">ListChangedEventArgs</param>
        private void FileNames_ListChanged(object sender, ListChangedEventArgs e)
        {
            for (int i = 0; i < _spectrumCalculationGrid.Rows.Count - 1; i++)
            {
                (_spectrumCalculationGrid.Rows[i].Cells[(int)GRID_COLUM.FILENAME_COMBOBOX] as DataGridViewComboBoxCell).DataSource = _bindFileName;
            }
            _spectrumCalculationGrid.EndEdit();
            (_bottomRow.Cells[(int)GRID_COLUM.FILENAME_COMBOBOX] as DataGridViewComboBoxCell).DataSource = _bindFileNameBottom;
        }

        /// <summary>
        /// Resampling Button click event
        /// </summary>
        /// <param name="sender">object</param>
        /// <param name="e">EventArgs</param>
        private void BtnResamling_Click(object sender, EventArgs e)
        {
            ResamplingDialog dlgReSamp = new ResamplingDialog(_resamplingParameter);
            dlgReSamp.ShowDialog(this);

            dlgReSamp.Dispose();

        }


        /// <summary>
        /// SpectrumCalculationDialog Load event handler
        /// </summary>
        /// <param name="sender">object</param>
        /// <param name="e">EventArgs</param>
        private void SpectrumCalculationDialog_Load(object sender, EventArgs e)
        {
            _rbnSpectrumName.Checked = true;
            _rbnAverage.Checked = true;
            _rbnAverage.Enabled = false;
            _rbnSubtract.Enabled = false;
            _rbnSum.Enabled = false;
            _rbnDivide.Enabled = false;

        }


        /// <summary>
        /// SpectrumCalculationDialog Closed event handler
        /// </summary>
        /// <param name="sender">object</param>
        /// <param name="e">EventArgs</param>
        private void SpectrumCalculationDialog_FormClosed(object sender, FormClosedEventArgs e)
        {
            _gridDataSpectrumCalculation.CloseFileManageList();
            _helpMessageControl.Enabled = false;
            DisplayClosed(sender, e);
        }

        /// <summary>
        /// Select Spectra by Radio Button Change event handler
        /// </summary>
        /// <param name="sender">object</param>
        /// <param name="e">EventArgs</param>
        private void SelectRbn_CheckedChanged(object sender, EventArgs e)
        {
            RadioButton rbn = (RadioButton)sender;

            //Select SpectrumName Radio Button
            if (sender == _rbnSpectrumName)
            {
                _spectrumName_ComboBox_Column.Visible = true;
                _rT_TextBox_Column.Visible = true;
                _precusor_TextBox_Column.Visible = true;

                _spectrumName_TextBox_Column.Visible = false;
                _rT_ComboBox_Column.Visible = false;
                _precursor_ComboBox_Column.Visible = false;


                // set help message
                _helpMessageControl.ClearList();
                _helpMessageControl.AddList(_rbnSpectrumName, Resource.HELP_MSG_RBN_SELECT_SPECTRA_BY);
                _helpMessageControl.AddList(_rbnRTmz, Resource.HELP_MSG_RBN_RTMZ);
                _helpMessageControl.AddList(_spectrumCalculationGrid, Resource.HELP_MSG__SCANNAMEGRID_SPECTRUM);
                _helpMessageControl.AddList(_rbnAverage, Resource.HELP_MEG_RBN_OPERATION_SETTINGS);
                _helpMessageControl.AddList(_rbnSubtract, Resource.HELP_MEG_RBN_OPERATION_SETTINGS);
                _helpMessageControl.AddList(_rbnSum, Resource.HELP_MEG_RBN_OPERATION_SETTINGS);
                _helpMessageControl.AddList(_rbnDivide, Resource.HELP_MEG_RBN_OPERATION_SETTINGS);
                _helpMessageControl.AddList(_btnResamling, Resource.HELP_MEG_BTN_RESAMPLING);
                _helpMessageControl.ChangeMessage += HelpCtl_ChangeMessage;
                _helpMessageControl.ScanInterval = 100;
                _helpMessageControl.Enabled = true;


            }
            //Select RT,m/z Radio Button
            else
            {
                _spectrumName_TextBox_Column.Visible = true;
                _rT_ComboBox_Column.Visible = true;
                _precursor_ComboBox_Column.Visible = true;

                _spectrumName_ComboBox_Column.Visible = false;
                _rT_TextBox_Column.Visible = false;
                _precusor_TextBox_Column.Visible = false;

                _helpMessageControl.ClearList();
                _helpMessageControl.AddList(_rbnSpectrumName, Resource.HELP_MSG_RBN_SELECT_SPECTRA_BY);
                _helpMessageControl.AddList(_rbnRTmz, Resource.HELP_MSG_RBN_RTMZ);
                _helpMessageControl.AddList(_spectrumCalculationGrid, Resource.HELP_MSG__RTmzGRID_SPECTRUM);
                _helpMessageControl.AddList(_rbnAverage, Resource.HELP_MEG_RBN_OPERATION_SETTINGS);
                _helpMessageControl.AddList(_rbnSubtract, Resource.HELP_MEG_RBN_OPERATION_SETTINGS);
                _helpMessageControl.AddList(_rbnSum, Resource.HELP_MEG_RBN_OPERATION_SETTINGS);
                _helpMessageControl.AddList(_rbnDivide, Resource.HELP_MEG_RBN_OPERATION_SETTINGS);
                _helpMessageControl.AddList(_btnResamling, Resource.HELP_MEG_BTN_RESAMPLING);
                _helpMessageControl.ChangeMessage += HelpCtl_ChangeMessage;
                _helpMessageControl.ScanInterval = 100;
                _helpMessageControl.Enabled = true;
            }
        }

        /// <summary>
        /// Data Grid View Editing Control Showing event
        /// </summary>
        /// <param name="sender">object</param>
        /// <param name="e">DataGridViewEditingControlShowingEventArgs</param>
        private void DataGridView_EditingControlShowing(object sender, DataGridViewEditingControlShowingEventArgs e)
        {
            if (e.Control is DataGridViewComboBoxEditingControl)
            {
                DataGridView dataGridView = (DataGridView)sender;

                // selectScan Name option
                if (_rbnSpectrumName.Checked)
                {
                    if (dataGridView.CurrentCell.OwningColumn.Index == _fileName_ComboBox_Column.Index)
                    {

                        _dataGridViewComboBoxEditingCtrl = (DataGridViewComboBoxEditingControl)e.Control;
                        _dataGridViewComboBoxEditingCtrl.SelectedIndexChanged += new EventHandler(DataGridView_FileName_ComboBox_SelectedIndexChanged);

                    }
                    else if (dataGridView.CurrentCell.OwningColumn.Index == _spectrumName_ComboBox_Column.Index)
                    {

                        _dataGridViewComboBoxEditingCtrl = (DataGridViewComboBoxEditingControl)e.Control;
                        _dataGridViewComboBoxEditingCtrl.SelectedIndexChanged += new EventHandler(ScanNameComboBoxCtrl_SelectedIndexChanged);

                    }
                }
                //select RT,m/z option
                else
                {
                    if (dataGridView.CurrentCell.OwningColumn.Index == _fileName_ComboBox_Column.Index)
                    {

                        _dataGridViewComboBoxEditingCtrl = (DataGridViewComboBoxEditingControl)e.Control;
                        _dataGridViewComboBoxEditingCtrl.SelectedIndexChanged += new EventHandler(DataGridView_FileName_ComboBox_SelectedIndexChanged);

                    }
                    else if (dataGridView.CurrentCell.OwningColumn.Index == _rT_ComboBox_Column.Index)
                    {

                        _dataGridViewComboBoxEditingCtrl = (DataGridViewComboBoxEditingControl)e.Control;
                        _dataGridViewComboBoxEditingCtrl.SelectedIndexChanged += new EventHandler(RTComboBoxCtrl_SelectIndexChanged);
                    }
                    else if (dataGridView.CurrentCell.OwningColumn.Index == _precursor_ComboBox_Column.Index)
                    {
                        _dataGridViewComboBoxEditingCtrl = (DataGridViewComboBoxEditingControl)e.Control;
                        _dataGridViewComboBoxEditingCtrl.SelectedIndexChanged += new EventHandler(PrecursorComboBoxCtrl_SelectedIndexChanged);
                    }

                }

            }
        }

        /// <summary>
        /// PrecursorComboBoxCtrl SelectedIndexChanged event
        /// </summary>
        /// <param name="sender">object</param>
        /// <param name="e">EventArgs</param>
        void PrecursorComboBoxCtrl_SelectedIndexChanged(object sender, EventArgs e)
        {
            DataGridViewComboBoxEditingControl cb = (DataGridViewComboBoxEditingControl)sender;
            DataGridViewComboBoxCell comboBoxCell, rTComboBoxCell, precursorComboBoxCell;
            DataGridViewTextBoxCell textBoxCell;
            int FileNameComboBoxIndex = -1;

            comboBoxCell = (DataGridViewComboBoxCell)_spectrumCalculationGrid.Rows[_spectrumCalculationGrid.CurrentCell.RowIndex].Cells[(int)GRID_COLUM.FILENAME_COMBOBOX];
            FileNameComboBoxIndex = comboBoxCell.Items.IndexOf(comboBoxCell.Value as string);

            textBoxCell = (DataGridViewTextBoxCell)_spectrumCalculationGrid.Rows[_spectrumCalculationGrid.CurrentCell.RowIndex].Cells[(int)GRID_COLUM.PRECUSOR_TEXTBOX];

            string strPrecursorOrg = cb.Items[cb.SelectedIndex].ToString();
            int startn = strPrecursorOrg.IndexOf("(");
            int endn = strPrecursorOrg.IndexOf(")");
            if ((startn != -1) && (endn != -1))
            {
                strPrecursorOrg = strPrecursorOrg.Substring(0, startn);
            }
            textBoxCell.Value = strPrecursorOrg;

            //SpectrumName
            rTComboBoxCell = (DataGridViewComboBoxCell)_spectrumCalculationGrid.Rows[_spectrumCalculationGrid.CurrentCell.RowIndex].Cells[(int)GRID_COLUM.RT_COMBOBOX];
            precursorComboBoxCell = (DataGridViewComboBoxCell)_spectrumCalculationGrid.Rows[_spectrumCalculationGrid.CurrentCell.RowIndex].Cells[(int)GRID_COLUM.PRECURSOR_COMBOBOX];
            textBoxCell = (DataGridViewTextBoxCell)_spectrumCalculationGrid.Rows[_spectrumCalculationGrid.CurrentCell.RowIndex].Cells[(int)GRID_COLUM.SPECTRUMNAME_TEXTBOX];

            textBoxCell.Value = _gridDataSpectrumCalculation.GetSpecturmName_TextBox(FileNameComboBoxIndex, rTComboBoxCell.Value.ToString(), cb.Items[cb.SelectedIndex].ToString());
            comboBoxCell = (DataGridViewComboBoxCell)_spectrumCalculationGrid.Rows[_spectrumCalculationGrid.CurrentCell.RowIndex].Cells[(int)GRID_COLUM.SPECTRUMNAMeE_COMBOBOX];
            comboBoxCell.Value = textBoxCell.Value;
        }

        /// <summary>
        /// RTComboBoxCtrl SelectedIndexChanged event
        /// </summary>
        /// <param name="sender">object</param>
        /// <param name="e">EventArgs</param>
        void RTComboBoxCtrl_SelectIndexChanged(object sender, EventArgs e)
        {
            DataGridViewComboBoxEditingControl cb = (DataGridViewComboBoxEditingControl)sender;
            DataGridViewComboBoxCell comboBoxCell;
            DataGridViewTextBoxCell textBoxCell;
            int FileNameComboBoxIndex = -1;

            //File Name
            comboBoxCell = (DataGridViewComboBoxCell)_spectrumCalculationGrid.Rows[_spectrumCalculationGrid.CurrentCell.RowIndex].Cells[(int)GRID_COLUM.FILENAME_COMBOBOX];
            FileNameComboBoxIndex = comboBoxCell.Items.IndexOf(comboBoxCell.Value as string);

            //RT(Well Name)
            textBoxCell = (DataGridViewTextBoxCell)_spectrumCalculationGrid.Rows[_spectrumCalculationGrid.CurrentCell.RowIndex].Cells[(int)GRID_COLUM.RT_TEXTBOX];
            textBoxCell.Value = cb.Items[cb.SelectedIndex];

            //Precusor
            ChangePrecusorComboListItem(_spectrumCalculationGrid.CurrentCell.RowIndex, FileNameComboBoxIndex, cb.SelectedIndex);

            comboBoxCell = (DataGridViewComboBoxCell)_spectrumCalculationGrid.Rows[_spectrumCalculationGrid.CurrentCell.RowIndex].Cells[(int)GRID_COLUM.PRECURSOR_COMBOBOX];
            if (comboBoxCell.Items.Count > 0)
            {
                if ((comboBoxCell.Value == null) || (comboBoxCell.Items.IndexOf(comboBoxCell.Value) == -1))
                {
                    comboBoxCell.Value = comboBoxCell.Items[0];
                    textBoxCell = (DataGridViewTextBoxCell)_spectrumCalculationGrid.Rows[_spectrumCalculationGrid.CurrentCell.RowIndex].Cells[(int)GRID_COLUM.PRECUSOR_TEXTBOX];
                    textBoxCell.Value = _gridDataSpectrumCalculation.GetPrecusor_TextBox(FileNameComboBoxIndex, cb.SelectedIndex);
                }
            }
            textBoxCell = (DataGridViewTextBoxCell)_spectrumCalculationGrid.Rows[_spectrumCalculationGrid.CurrentCell.RowIndex].Cells[(int)GRID_COLUM.SPECTRUMNAME_TEXTBOX];
            textBoxCell.Value = _gridDataSpectrumCalculation.GetSpecturmName_TextBox(FileNameComboBoxIndex, cb.Items[cb.SelectedIndex].ToString(), comboBoxCell.Value.ToString());

            comboBoxCell = (DataGridViewComboBoxCell)_spectrumCalculationGrid.Rows[_spectrumCalculationGrid.CurrentCell.RowIndex].Cells[(int)GRID_COLUM.SPECTRUMNAMeE_COMBOBOX];
            comboBoxCell.Value = textBoxCell.Value;
        }

        /// <summary>
        /// DataGridView_FileName_ComboBox SelectedIndexChanged event
        /// </summary>
        /// <param name="sender">object</param>
        /// <param name="e">EventArgs</param>
        private void DataGridView_FileName_ComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            DataGridViewComboBoxEditingControl cb = (DataGridViewComboBoxEditingControl)sender;
            int returnCode;

            // Select "(Select MS File)"
            if (cb.Text == "(Select MS File)")
            {
                ClearColumnItem();
            }
            // Select "Browse..."
            else if (cb.Text == "Browse...")
            {
                OpenFileDialog dlg = new OpenFileDialog();
                dlg.Filter = "All MS Files (*.cdf;*.txt;*.msb;*.mzml;*.mzxml;*.run)|*.cdf;*.txt;*.msb;*.mzml;*.mzxml;*.run";

                DialogResult result = dlg.ShowDialog();
                if (result == DialogResult.OK)
                {
                    //Read the Selected File
                    returnCode = _gridDataSpectrumCalculation.ReadMSBData(dlg.FileName);

                    if (returnCode == -1)
                    {
                        cb.SelectedIndex = 0;
                        return;
                    }


                    DataGridViewRow newRow = new DataGridViewRow();
                    DataGridViewComboBoxCell file = new DataGridViewComboBoxCell();
                    file.DataSource = _bindFileName;
                    file.Value = _bindFileName[_bindFileName.Count - 1];

                    newRow.Cells.Add(file);
                    _spectrumCalculationGrid.Rows.Insert(_spectrumCalculationGrid.Rows.Count - 1, newRow);
                    _spectrumCalculationGrid.CurrentCell.Value = _bindFileNameBottom[0];

                    if (_spectrumCalculationGrid.Rows.Count - 1 >= 2)
                    {
                        _rbnAverage.Enabled = true;
                        _rbnSubtract.Enabled = true;
                        _rbnSum.Enabled = true;
                        _rbnDivide.Enabled = true;
                    }
                    DataGridViewTextBoxCell memRow;
                    memRow = (DataGridViewTextBoxCell)_spectrumCalculationGrid.Rows[_spectrumCalculationGrid.Rows.Count - 2].Cells[(int)GRID_COLUM.SELID_TEXTBOX];
                    int selId = _bindFileName.Count - 1;
                    memRow.Value = selId.ToString();
                    ChangeComboListAtChangingSelFile(_spectrumCalculationGrid.Rows.Count - 2, _bindFileName.Count - 1);
                }
                else
                {
                    // select (Select Ms File)
                    cb.SelectedIndex = 0;
                    ClearColumnItem();
                }

            }
            //(Select MS File) Brows...
            else if (cb.Text != "")
            {
                if (_spectrumCalculationGrid.CurrentCell.RowIndex == _spectrumCalculationGrid.RowCount - 1)
                {
                    DataGridViewRow newRow = new DataGridViewRow();
                    DataGridViewComboBoxCell file = new DataGridViewComboBoxCell();
                    file.DataSource = _bindFileName;
                    file.Value = _bindFileName[cb.SelectedIndex - 1];

                    newRow.Cells.Add(file);
                    _spectrumCalculationGrid.Rows.Insert(_spectrumCalculationGrid.Rows.Count - 1, newRow);
                    _spectrumCalculationGrid.CurrentCell.Value = _bindFileNameBottom[0];

                    if (_spectrumCalculationGrid.Rows.Count - 1 >= 2)
                    {
                        _rbnAverage.Enabled = true;
                        _rbnSubtract.Enabled = true;
                        _rbnSum.Enabled = true;
                        _rbnDivide.Enabled = true;
                    }
                    DataGridViewTextBoxCell memRow;
                    memRow = (DataGridViewTextBoxCell)_spectrumCalculationGrid.Rows[_spectrumCalculationGrid.Rows.Count - 2].Cells[(int)GRID_COLUM.SELID_TEXTBOX];
                    int selId = cb.SelectedIndex - 1;
                    memRow.Value = selId.ToString();
                    ChangeComboListAtChangingSelFile(_spectrumCalculationGrid.Rows.Count - 2, cb.SelectedIndex - 1);
                }
                else
                {
                    DataGridViewTextBoxCell memRow;
                    memRow = (DataGridViewTextBoxCell)_spectrumCalculationGrid.Rows[_spectrumCalculationGrid.CurrentCell.RowIndex].Cells[(int)GRID_COLUM.SELID_TEXTBOX];
                    int selId = cb.SelectedIndex;
                    memRow.Value = selId.ToString();
                    ChangeComboListAtChangingSelFile(_spectrumCalculationGrid.CurrentCell.RowIndex, cb.SelectedIndex);
                }
            }
        }


        /// <summary>
        /// ScanNameComboBoxCtrl SelectedIndexChanged event
        /// </summary>
        /// <param name="sender">object</param>
        /// <param name="e">EventArgs</param>
        void ScanNameComboBoxCtrl_SelectedIndexChanged(object sender, EventArgs e)
        {
            DataGridViewComboBoxEditingControl cb = (DataGridViewComboBoxEditingControl)sender;
            DataGridViewTextBoxCell scanNameTextBoxCell;
            DataGridViewComboBoxCell scanNameComboBoxCell;
            int FileNameComboBoxIndex = -1;

            //File Name
            scanNameComboBoxCell = (DataGridViewComboBoxCell)_spectrumCalculationGrid.Rows[_spectrumCalculationGrid.CurrentCell.RowIndex].Cells[(int)GRID_COLUM.FILENAME_COMBOBOX];
            FileNameComboBoxIndex = scanNameComboBoxCell.Items.IndexOf(scanNameComboBoxCell.Value as string);

            //RT(Well Name)
            scanNameTextBoxCell = (DataGridViewTextBoxCell)_spectrumCalculationGrid.Rows[_spectrumCalculationGrid.CurrentCell.RowIndex].Cells[(int)GRID_COLUM.RT_TEXTBOX];
            scanNameTextBoxCell.Value = _gridDataSpectrumCalculation.GetRT_TextBoxData(FileNameComboBoxIndex, cb.SelectedIndex);

            //Precusor
            scanNameTextBoxCell = (DataGridViewTextBoxCell)_spectrumCalculationGrid.Rows[_spectrumCalculationGrid.CurrentCell.RowIndex].Cells[(int)GRID_COLUM.PRECUSOR_TEXTBOX];
            scanNameTextBoxCell.Value = _gridDataSpectrumCalculation.GetPrecusor_TextBox(FileNameComboBoxIndex, cb.SelectedIndex);

            ChangeRTComboListItem(_spectrumCalculationGrid.CurrentCell.RowIndex, FileNameComboBoxIndex, cb.Items[cb.SelectedIndex].ToString());

            scanNameComboBoxCell = (DataGridViewComboBoxCell)_spectrumCalculationGrid.Rows[_spectrumCalculationGrid.CurrentCell.RowIndex].Cells[(int)GRID_COLUM.RT_COMBOBOX];
            ChangePrecusorComboListItem(_spectrumCalculationGrid.CurrentCell.RowIndex, FileNameComboBoxIndex, scanNameComboBoxCell.Items.IndexOf(scanNameComboBoxCell.Value), cb.Items[cb.SelectedIndex].ToString());

            scanNameTextBoxCell = (DataGridViewTextBoxCell)_spectrumCalculationGrid.Rows[_spectrumCalculationGrid.CurrentCell.RowIndex].Cells[(int)GRID_COLUM.SPECTRUMNAME_TEXTBOX];

            scanNameTextBoxCell.Value = cb.Items[cb.SelectedIndex];

        }

        /// <summary>
        /// Remove DataGridViewComboBox Event
        /// /// </summary>
        /// <param name="sender">sender object</param>
        /// <param name="e">e</param>
        private void DataGridView_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {
            if (_dataGridViewComboBoxEditingCtrl != null)
            {
                _dataGridViewComboBoxEditingCtrl.SelectedIndexChanged -= new EventHandler(DataGridView_FileName_ComboBox_SelectedIndexChanged);
                _dataGridViewComboBoxEditingCtrl.SelectedIndexChanged -= new EventHandler(ScanNameComboBoxCtrl_SelectedIndexChanged);
                _dataGridViewComboBoxEditingCtrl.SelectedIndexChanged -= new EventHandler(RTComboBoxCtrl_SelectIndexChanged);
                _dataGridViewComboBoxEditingCtrl.SelectedIndexChanged -=
                    new EventHandler(PrecursorComboBoxCtrl_SelectedIndexChanged);
                _dataGridViewComboBoxEditingCtrl = null;
            }
        }

        /// <summary>
        /// Calc button click event
        /// </summary>
        /// <param name="sender">object</param>
        /// <param name="e">EventArgs</param>
        private void BtnCalc_Click(object sender, EventArgs e)
        {
            if (_spectrumCalculationGrid.Rows.Count <= 2)
            {
                MessageBox.Show("At least two files are required tosubtract/divide specra.", "File Select Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return;
            }

            if (_threadCal != null)    // check thread exists
            {
                _threadCal.Abort();
                _threadCal = null;
            }

            _dlgProgress = new ProgressDlg();
            _dlgProgress.PrgCancel += ProcCancel;

            // prepare Progress dialog and thread for identification
            _spectrumCalculation = new Cal.SpectrumCalculation(_resamplingParameter);
            _resamplingCalculation = _spectrumCalculation.GetResamplingCalculation;

            _resamplingCalculation.UpdateResamplingProgress += UpdateSpecCalcResamplingProgress;
            this.UpdatePrepareProgress += UpdateSpecCalcPrepareProgress;
            _dlgProgress.CancelEnabled = true;
            _threadCal = new System.Threading.Thread(ThreadCalc);        // create thread
            _threadCal.Priority = System.Threading.ThreadPriority.Lowest; // set lowest priority
            _threadCal.Start();                                           // start thread
            _dlgProgress.ShowDialog();
            _dlgProgress = null;
            _resamplingCalculation.UpdateResamplingProgress -= UpdateSpecCalcResamplingProgress;
            this.UpdatePrepareProgress -= UpdateSpecCalcPrepareProgress;

            _spectrumCalculation.DisplayCalculationResult();



        }

        /// <summary>
        /// Cancel button click event
        /// </summary>
        /// <param name="sender">object</param>
        /// <param name="e">EventArgs</param>
        private void BtnCancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }


        /// <summary>
        /// Delete Spectrum Rows
        /// </summary>
        /// <param name="sender">object</param>
        /// <param name="e">EventArgs</param>
        private void DeleteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (_spectrumCalculationGrid.Rows.Count == 1) return;

            if (_spectrumCalculationGrid.CurrentCell.OwningRow.Index == _spectrumCalculationGrid.Rows.Count - 1) return;

            _spectrumCalculationGrid.Rows.Remove(_spectrumCalculationGrid.CurrentCell.OwningRow);

            if (!(_spectrumCalculationGrid.Rows.Count - 1 >= 2))
            {
                //Disabled if only one column except for the last line
                _rbnAverage.Enabled = false;
                _rbnSubtract.Enabled = false;
                _rbnSum.Enabled = false;
                _rbnDivide.Enabled = false;
            }

        }

        /// <summary>
        /// SpectrumCalculationGrid cell click event
        /// </summary>
        /// <param name="sender">object</param>
        /// <param name="e">DataGridViewCellEventArgs</param>
        private void SpectrumCalculationGrid_CellClick(object sender, DataGridViewCellEventArgs e)
        {
            // only combobox cell clicked
            if ((e.ColumnIndex == _fileName_ComboBox_Column.Index)
              || (e.ColumnIndex == _spectrumName_ComboBox_Column.Index)
              || (e.ColumnIndex == _rT_ComboBox_Column.Index)
              || (e.ColumnIndex == _precursor_ComboBox_Column.Index))
            {
                // change edit mode 
                _spectrumCalculationGrid.BeginEdit(false);
                DataGridViewComboBoxEditingControl edt =
                        _spectrumCalculationGrid.EditingControl as DataGridViewComboBoxEditingControl;
                // dropdown
                edt.DroppedDown = true;
            }

        }

        #endregion

        #region --- Private Methods -------------------------------------

        /// <summary>
        /// Delete the column element
        /// </summary>
        private void ClearColumnItem()
        {
            DataGridViewComboBoxCell scanNameComboBoxCell;
            DataGridViewTextBoxCell scanNameTextBoxCell;
            //Remove elements of the Column that is selected
            foreach (DataGridViewCell c in _spectrumCalculationGrid.SelectedCells)
            {
                //
                for (int i = 1; i < _spectrumCalculationGrid.ColumnCount; i++)
                {
                    if (_spectrumCalculationGrid.Rows[c.RowIndex].Cells[c.ColumnIndex + i] is DataGridViewComboBoxCell)
                    {
                        scanNameComboBoxCell = (DataGridViewComboBoxCell)_spectrumCalculationGrid.Rows[c.RowIndex].Cells[c.ColumnIndex + i];
                        scanNameComboBoxCell.Items.Clear();
                    }
                    else
                    {
                        scanNameTextBoxCell = (DataGridViewTextBoxCell)_spectrumCalculationGrid.Rows[c.RowIndex].Cells[c.ColumnIndex + i];
                        scanNameTextBoxCell.Value = string.Empty;
                    }
                }

            }
        }

        /// <summary>
        /// Help Message Change Message event handler
        /// </summary>
        /// <param name="message">Message</param>
        private void HelpCtl_ChangeMessage(string message)
        {
            string chngMessage = message;
            _textBoxHelpMessage.Text = string.Format(chngMessage);
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


        /// <summary>
        /// Progress Dialog Cancel Button Click event handler
        /// </summary>
        /// <param name="sender">object</param>
        /// <param name="e">EventArgs</param>
        private void ProcCancel(object sender, EventArgs e)
        {
            _dlgProgress.AutoCountEnabled = false;
            _dlgProgress.CancelEnabled = false;
            _dlgProgress.SetLabelText("Stopping Spectrum Calculation thread.");
            _spectrumCalculation.IsThreadStopping = true;
        }

        /// <summary>
        /// Change Combo cell list items at changing selected file combo items
        /// </summary>
        /// <param name="rowIndex">changed row index</param>
        /// <param name="itemIndex">selected file combo index</param>
        private void ChangeComboListAtChangingSelFile(int rowIndex, int itemIndex)
        {
            DataGridViewTextBoxCell memRow;
            int index = -1;
            if (_spectrumCalculationGrid.Rows.Count > 2)
            {
                for (int i = 0; i < _spectrumCalculationGrid.Rows.Count - 1; i++)
                {
                    memRow = (DataGridViewTextBoxCell)_spectrumCalculationGrid.Rows[i].Cells[(int)GRID_COLUM.SELID_TEXTBOX];
                    if ((memRow.Value.ToString() == itemIndex.ToString()) && (i != rowIndex))
                    {
                        index = i;
                        break;
                    }
                }
            }
            if (index != -1)
            {
                DataGridViewComboBoxCell comboBoxCell1;
                DataGridViewComboBoxCell comboBoxCell2;
                DataGridViewComboBoxCell comboBoxCellRT;
                DataGridViewTextBoxCell textBoxCell;

                comboBoxCell1 = (DataGridViewComboBoxCell)_spectrumCalculationGrid.Rows[rowIndex].Cells[(int)GRID_COLUM.SPECTRUMNAMeE_COMBOBOX];
                comboBoxCell2 = (DataGridViewComboBoxCell)_spectrumCalculationGrid.Rows[index].Cells[(int)GRID_COLUM.SPECTRUMNAMeE_COMBOBOX];

                comboBoxCell1.DataSource = null;
                comboBoxCell1.DataSource = comboBoxCell2.DataSource;
                comboBoxCell1.Value = comboBoxCell1.Items[0];

                //RT(Well Name)
                textBoxCell = (DataGridViewTextBoxCell)_spectrumCalculationGrid.Rows[rowIndex].Cells[(int)GRID_COLUM.RT_TEXTBOX];
                textBoxCell.Value = _gridDataSpectrumCalculation.GetRT_TextBoxData(itemIndex, 0);

                //Precusor
                textBoxCell = (DataGridViewTextBoxCell)_spectrumCalculationGrid.Rows[rowIndex].Cells[(int)GRID_COLUM.PRECUSOR_TEXTBOX];
                textBoxCell.Value = _gridDataSpectrumCalculation.GetPrecusor_TextBox(itemIndex, 0);

                comboBoxCellRT = (DataGridViewComboBoxCell)_spectrumCalculationGrid.Rows[rowIndex].Cells[(int)GRID_COLUM.RT_COMBOBOX];
                comboBoxCell2 = (DataGridViewComboBoxCell)_spectrumCalculationGrid.Rows[index].Cells[(int)GRID_COLUM.RT_COMBOBOX];

                comboBoxCellRT.Items.Clear();
                for (int i = 0; i < comboBoxCell2.Items.Count; i++)
                {
                    comboBoxCellRT.Items.Add(comboBoxCell2.Items[i].ToString());
                }
                comboBoxCellRT.Value = comboBoxCellRT.Items[0];

                //Precusor
                comboBoxCell1 = (DataGridViewComboBoxCell)_spectrumCalculationGrid.Rows[rowIndex].Cells[(int)GRID_COLUM.PRECURSOR_COMBOBOX];
                comboBoxCell2 = (DataGridViewComboBoxCell)_spectrumCalculationGrid.Rows[index].Cells[(int)GRID_COLUM.PRECURSOR_COMBOBOX];

                comboBoxCell1.Items.Clear();
                for (int i = 0; i < comboBoxCell2.Items.Count; i++)
                {
                    comboBoxCell1.Items.Add(comboBoxCell2.Items[i].ToString());
                }
                if (comboBoxCell1.Items.Count > 0)
                {
                    if ((comboBoxCell1.Value == null) || (comboBoxCell1.Items.IndexOf(comboBoxCell1.Value) == -1))
                    {
                        comboBoxCell1.Value = comboBoxCell1.Items[0];
                    }
                }
                textBoxCell = (DataGridViewTextBoxCell)_spectrumCalculationGrid.Rows[rowIndex].Cells[(int)GRID_COLUM.SPECTRUMNAME_TEXTBOX];
                textBoxCell.Value = _gridDataSpectrumCalculation.GetSpecturmName_TextBox(itemIndex, comboBoxCellRT.Value.ToString(), comboBoxCell1.Value.ToString());
            }
            else
            {
                ChangeSpecNameComboListItem(rowIndex, itemIndex);
                ChangeRTComboListItem(rowIndex, itemIndex);
            }
        }


        /// <summary>
        /// Change Spectrum name Combo cell list items
        /// </summary>
        /// <param name="rowIndex">changed row index</param>
        /// <param name="itemIndex">selected file combo index</param>
        private void ChangeSpecNameComboListItem(int rowIndex, int itemIndex)
        {
            DataGridViewComboBoxCell comboBoxCell;
            DataGridViewTextBoxCell textBoxCell;

            comboBoxCell = (DataGridViewComboBoxCell)_spectrumCalculationGrid.Rows[rowIndex].Cells[(int)GRID_COLUM.SPECTRUMNAMeE_COMBOBOX];

            comboBoxCell.DataSource = null;
            comboBoxCell.DataSource = _gridDataSpectrumCalculation.GetSpectrumNameList(itemIndex);
            comboBoxCell.Value = comboBoxCell.Items[0];

            //RT(Well Name)
            textBoxCell = (DataGridViewTextBoxCell)_spectrumCalculationGrid.Rows[rowIndex].Cells[(int)GRID_COLUM.RT_TEXTBOX];
            textBoxCell.Value = _gridDataSpectrumCalculation.GetRT_TextBoxData(itemIndex, 0);

            //Precusor
            textBoxCell = (DataGridViewTextBoxCell)_spectrumCalculationGrid.Rows[rowIndex].Cells[(int)GRID_COLUM.PRECUSOR_TEXTBOX];
            textBoxCell.Value = _gridDataSpectrumCalculation.GetPrecusor_TextBox(itemIndex, 0);
        }

        /// <summary>
        /// Change RT Combo cell list items
        /// </summary>
        /// <param name="rowIndex">changed row index</param>
        /// <param name="itemIndex">selected file combo index</param>
        private void ChangeRTComboListItem(int rowIndex, int itemIndex)
        {
            DataGridViewComboBoxCell comboBoxCell;
            DataGridViewComboBoxCell comboBoxCellRT;
            DataGridViewTextBoxCell textBoxCell;
            List<string> tmpList;

            comboBoxCellRT = (DataGridViewComboBoxCell)_spectrumCalculationGrid.Rows[rowIndex].Cells[(int)GRID_COLUM.RT_COMBOBOX];
            //SpectrumNameCombobox Item Clear
            comboBoxCellRT.Items.Clear();

            tmpList = _gridDataSpectrumCalculation.GetRT_Combobox(itemIndex);

            for (int i = 0; i < tmpList.Count; i++)
            {
                comboBoxCellRT.Items.Add(tmpList[i]);
            }
            comboBoxCellRT.Value = comboBoxCellRT.Items[0];

            //Precusor
            ChangePrecusorComboListItem(rowIndex, itemIndex, 0);
            comboBoxCell = (DataGridViewComboBoxCell)_spectrumCalculationGrid.Rows[rowIndex].Cells[(int)GRID_COLUM.PRECURSOR_COMBOBOX];
            if (comboBoxCell.Items.Count > 0)
            {
                if ((comboBoxCell.Value == null) || (comboBoxCell.Items.IndexOf(comboBoxCell.Value) == -1))
                {
                    comboBoxCell.Value = comboBoxCell.Items[0];
                }
            }
            textBoxCell = (DataGridViewTextBoxCell)_spectrumCalculationGrid.Rows[rowIndex].Cells[(int)GRID_COLUM.SPECTRUMNAME_TEXTBOX];
            textBoxCell.Value = _gridDataSpectrumCalculation.GetSpecturmName_TextBox(itemIndex, comboBoxCellRT.Value.ToString(), comboBoxCell.Value.ToString());
        }


        /// <summary>
        /// Change RT Combo cell list items
        /// </summary>
        /// <param name="rowIndex">changed row index</param>
        /// <param name="itemIndex">selected file combo index</param>
        private void ChangeRTComboListItem(int rowIndex, int itemIndex, string SpecName)
        {
            DataGridViewComboBoxCell comboBoxCell;
            string rT = _gridDataSpectrumCalculation.GetRT_Combobox(itemIndex, SpecName);
            comboBoxCell = (DataGridViewComboBoxCell)_spectrumCalculationGrid.Rows[rowIndex].Cells[(int)GRID_COLUM.RT_COMBOBOX];
            if (comboBoxCell.Items.IndexOf(rT) >= 0)
            {
                comboBoxCell.Value = rT;
            }
            else
            {
                comboBoxCell.Value = comboBoxCell.Items[0];
            }
        }

        /// <summary>
        /// Change Precusor Combo cell list items
        /// </summary>
        /// <param name="rowIndex">changed row index</param>
        /// <param name="fileIndex">selected file combo index</param>
        /// <param name="itemIndex">selected RT combo index</param>
        private void ChangePrecusorComboListItem(int rowIndex, int fileIndex, int itemIndex)
        {
            DataGridViewComboBoxCell comboBoxCell;
            List<string> tmpList;

            comboBoxCell = (DataGridViewComboBoxCell)_spectrumCalculationGrid.Rows[rowIndex].Cells[(int)GRID_COLUM.RT_COMBOBOX];
            tmpList = _gridDataSpectrumCalculation.GetPrecursor_ComboBox(fileIndex, comboBoxCell.Items[itemIndex].ToString());

            comboBoxCell = (DataGridViewComboBoxCell)_spectrumCalculationGrid.Rows[rowIndex].Cells[(int)GRID_COLUM.PRECURSOR_COMBOBOX];
            comboBoxCell.Items.Clear();

            for (int i = 0; i < tmpList.Count; i++)
            {
                comboBoxCell.Items.Add(tmpList[i]);
            }
        }

        /// <summary>
        /// Change Precusor Combo cell list items
        /// </summary>
        /// <param name="rowIndex">changed row index</param>
        /// <param name="fileIndex">selected file combo index</param>
        /// <param name="itemIndex">selected RT combo index</param>
        private void ChangePrecusorComboListItem(int rowIndex, int fileIndex, int itemIndex, string SpecName)
        {
            DataGridViewComboBoxCell comboBoxCell;
            List<string> tmpList;

            comboBoxCell = (DataGridViewComboBoxCell)_spectrumCalculationGrid.Rows[rowIndex].Cells[(int)GRID_COLUM.RT_COMBOBOX];
            int selIndex = 0;
            tmpList = _gridDataSpectrumCalculation.GetPrecursor_ComboBox(fileIndex, comboBoxCell.Items[itemIndex].ToString(), SpecName, ref selIndex);

            comboBoxCell = (DataGridViewComboBoxCell)_spectrumCalculationGrid.Rows[rowIndex].Cells[(int)GRID_COLUM.PRECURSOR_COMBOBOX];
            comboBoxCell.Items.Clear();

            for (int i = 0; i < tmpList.Count; i++)
            {
                comboBoxCell.Items.Add(tmpList[i]);
            }
            comboBoxCell.Value = comboBoxCell.Items[selIndex];
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
        /// Update Spectrum Calculation(Prepare) Progress
        /// </summary>
        /// <param name="id">PROGRESS_PREPARE_PHASE id</param>
        /// <param name="value">progress bar value or max</param>
        /// <param name="cursmplnum">current number for text</param>
        /// <param name="maxsmplnum">max number for text</param>
        private void UpdateSpecCalcPrepareProgress(PROGRESS_PREPARATION_PHASE id, int value, int cursmplnum, int maxsmplnum)
        {
            string msg = "";

            if (_dlgProgress == null) return;

            if (this.InvokeRequired)
            {
                try
                {
                    _dlgProgress.Invoke(new MethodInvoker(() => { UpdateSpecCalcPrepareProgress(id, value, cursmplnum, maxsmplnum); }));
                }
                catch
                {

                }
                return;
            }

            switch (id)
            {
                case PROGRESS_PREPARATION_PHASE.PREPARE_START:
                    msg = string.Format("Spectrum Calculation Reading Spectrum Data ({0}/{1}) ", cursmplnum.ToString(), maxsmplnum.ToString());
                    _dlgProgress.SetLabelText(msg);
                    _dlgProgress.PrgValue = 0;
                    _dlgProgress.CancelEnabled = true;
                    _dlgProgress.PrgMax = 10 * maxsmplnum;
                    _dlgProgress.AutoCountInterval = 1000;
                    _dlgProgress.AutoCountAddValue = 1;
                    _dlgProgress.AutoCountEnabled = true;


                    break;
                case PROGRESS_PREPARATION_PHASE.PREPARE_END:
                    msg = string.Format("Spectrum Calculation Reading Spectrum Data End ({0}/{1}) ", cursmplnum.ToString(), maxsmplnum.ToString());
                    _dlgProgress.SetLabelText(msg);
                    _dlgProgress.AutoCountEnabled = false;
                    _dlgProgress.PrgMax = 1;
                    _dlgProgress.PrgValue = _dlgProgress.PrgMax;
                    _dlgProgress.CancelEnabled = true;
                    break;
#if false
                case IdentResults.PROGRESS_IDNT_PHASE.READ_UPDATE:
                    if (_dlgProgress.PrgValue < _dlgProgress.PrgMax)
                    {
                        _dlgProgress.PrgValue++;
                    }
                    break;
#endif
                default:
                    break;
            }
        }





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

        #region --- Thread -------------------------------------

        /// <summary>Thread for spectrum calculation</summary>
        private void ThreadCalc()
        {

            DataGridViewComboBoxCell comboBoxCellFile;
            DataGridViewComboBoxCell comboBoxCellSpecName;
            DataGridViewTextBoxCell textBoxCellRT;
            DataGridViewTextBoxCell textBoxCellPrecusor;
            string strSpecName = null;
            string strRT = null;
            string strPrecusor = null;
            int fileNameIndex;
            SpectrumWrapper _spectrumWrapper;

            for (int i = 0; i < _spectrumCalculationGrid.Rows.Count - 1; i++)
            {
                if (_spectrumCalculation.IsThreadStopping == true)
                {
                    ProgDlgClose();
                    return;
                }
                UpdatePrepareProgress(PROGRESS_PREPARATION_PHASE.PREPARE_START, 0, i + 1, _spectrumCalculationGrid.Rows.Count - 1);
                //FileName Column 
                comboBoxCellFile = (DataGridViewComboBoxCell)_spectrumCalculationGrid.Rows[i].Cells[(int)GRID_COLUM.FILENAME_COMBOBOX];

                fileNameIndex = comboBoxCellFile.Items.IndexOf(comboBoxCellFile.Value);

                comboBoxCellSpecName = (DataGridViewComboBoxCell)_spectrumCalculationGrid.Rows[i].Cells[(int)GRID_COLUM.SPECTRUMNAMeE_COMBOBOX];
                strSpecName = comboBoxCellSpecName.Value.ToString();
                textBoxCellRT = (DataGridViewTextBoxCell)_spectrumCalculationGrid.Rows[i].Cells[(int)GRID_COLUM.RT_TEXTBOX];
                strRT = textBoxCellRT.Value.ToString();
                textBoxCellPrecusor = (DataGridViewTextBoxCell)_spectrumCalculationGrid.Rows[i].Cells[(int)GRID_COLUM.PRECUSOR_TEXTBOX];
                strPrecusor = textBoxCellPrecusor.Value.ToString(); ;


                _spectrumWrapper = _gridDataSpectrumCalculation.GetSpectrumWrapperData(fileNameIndex, strSpecName, strRT, strPrecusor);
                if (_spectrumWrapper != null)
                {
                    _spectrumCalculation.AddSpecData(_spectrumWrapper);
                }
                UpdatePrepareProgress(PROGRESS_PREPARATION_PHASE.PREPARE_END, 0, i + 1, _spectrumCalculationGrid.Rows.Count - 1);
            }

            if (_rbnAverage.Checked)
            {
                _spectrumCalculation.OperationSetting = Cal.SpectrumCalculation.OperationSettings.AVERAGE;
            }
            else if (_rbnSubtract.Checked)
            {
                _spectrumCalculation.OperationSetting = Cal.SpectrumCalculation.OperationSettings.SUBTRACT;
            }
            else if (_rbnSum.Checked)
            {
                _spectrumCalculation.OperationSetting = Cal.SpectrumCalculation.OperationSettings.SUM;
            }
            else
            {
                _spectrumCalculation.OperationSetting = Cal.SpectrumCalculation.OperationSettings.DIVIDE;
            }

            _spectrumCalculation.IsThreadStopping = false;
            try
            {
                _spectrumCalculation.Execute();
            }
            catch (ArgumentNullException ex)
            {
                string a = ex.Message;
            }
            catch (ArgumentException ex)
            {
                string a = ex.Message;
            }
            if (_spectrumCalculation.IsThreadStopping == true)
            {
                ProgDlgClose();
                return;
            }
            ProgDlgClose();

        }

        #endregion


    }
}
