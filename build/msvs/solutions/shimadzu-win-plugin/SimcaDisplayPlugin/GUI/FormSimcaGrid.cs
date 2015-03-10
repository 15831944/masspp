/// <summary>
/// [FILE] FormSimcaGrid.cs
/// [ABSTRACT] SIMCA Plugin Grid display form class
/// Copyright (C) 2012-12-18 Shimadzu
/// </summary>

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using SimcaDisplay.Data;
using Microsoft.VisualBasic.FileIO;

namespace SimcaDisplay.Gui
{
    /// <summary>
    /// Grid display form class
    /// </summary>
    internal partial class FormSimcaGrid : Form
    {
        #region --- Constants ------------------------------------------
        /// <summary>
        /// Tab ID
        /// </summary>
        private enum TAB_ID
        {
            /// <summary>
            /// Score plot tab ID
            /// </summary>
            SCORE_PLOT,

            /// <summary>
            /// Loading plot tab ID
            /// </summary>
            LOADING_PLOT,

            /// <summary>
            /// S plot tab ID
            /// </summary>
            S_PLOT,

            /// <summary>
            /// tab ID max value
            /// </summary>
            MAX
        }

        /// <summary>
        /// The number of lines which validates virtual mode.
        /// </summary>
        private const int ENABLE_VIRTUAL_MODE_LINE_COUNT = 100;

        /// <summary>
        /// Message format(Convert grid value)
        /// </summary>
        private const string MSG_FMT_CONVERT_GRID_VALUE = "Convert grid value to {0} plot.";
        
        /// <summary>
        /// Error message format(Data)
        /// </summary>
        private const string ERR_MSG_FMT_FILE_READ_DATA = "{0} plot file ({1}) should contain at least 2 components.";
        
        /// <summary>
        /// Error message format(Data format)
        /// </summary>
        private const string ERR_MSG_FMT_FILE_READ_FMT = "Failed to parse {1} as a {0} plot file.";
        
        #endregion

        #region --- Events ---------------------------------------------
        /// <summary>
        /// Event(Form closed)
        /// </summary>
        public event Action<object, FormClosedEventArgs> GridClosed;
        
        /// <summary>
        /// Event(Form closed)
        /// </summary>
        public event Action<object, ApplyClickEventArgs> ApplyClick;
        
        /// <summary>
        /// ApplyClick EventArgs
        /// </summary>
        internal class ApplyClickEventArgs : EventArgs
        {
            /// <summary>
            /// Gets or sets data type
            /// </summary>
            public SimcaData.SIMCA_DATA_TYPE _dataType { get; set; }
            
            /// <summary>
            /// Gets or sets paste data
            /// </summary>
            public string _applyData { get; set; }
        }

        #endregion

        #region --- Variables ------------------------------------------
        /// <summary>
        /// Display data for virtual modes.
        /// </summary>
        private List<List<string>>[] _dataList = new List<List<string>>[(int)TAB_ID.MAX];
        
        /// <summary>
        /// Paste data
        /// </summary>
        private string[] _pasteValue = new string[(int)TAB_ID.MAX];
        
        /// <summary>
        /// Apply button enable flag
        /// </summary>
        private bool[] _enableApply = new bool[(int)TAB_ID.MAX];
        
        /// <summary>
        /// Save button enable flag
        /// </summary>
        private bool[] _enableSave = new bool[(int)TAB_ID.MAX];
        #endregion

        #region --- Construction ---------------------------------------
        /// <summary>
        /// Initializes a new instance of the FormSimcaGrid class
        /// </summary>
        public FormSimcaGrid()
        {
            InitializeComponent();
        }
        
        /// <summary>
        /// Initializes a new instance of the FormSimcaGrid class
        /// </summary>
        /// <param name="scoreData">Score plot data</param>
        /// <param name="loadingData">Loading plot data</param>
        /// <param name="splotData">S plot data</param>
        /// <param name="selIndex">select index</param>
        public FormSimcaGrid(SimcaData scoreData, SimcaData loadingData, SimcaData splotData, int selIndex)
        {
            InitializeComponent();

            Update(scoreData);
            Update(loadingData);
            Update(splotData);
            _tabControlGrid.SelectedIndex = selIndex;
        }
        #endregion

        #region --- Public Methods -------------------------------------
        /// <summary>
        /// Update
        /// </summary>
        /// <param name="plotData">plot data</param>
        public void Update(SimcaData plotData)
        {
            DataGridView grid = null;
            switch (plotData.DataType)
            {
                case SimcaData.SIMCA_DATA_TYPE.SCORE_PLOT:
                    grid = _dataGridViewScore;
                    break;
                case SimcaData.SIMCA_DATA_TYPE.LOADING_PLOT:
                    grid = _dataGridViewLoading;
                    break;
                case SimcaData.SIMCA_DATA_TYPE.S_PLOT:
                    grid = _dataGridViewSPlot;
                    break;
            }

            if (grid == null)
            {
                return;
            }

            // Virtual mode OFF
            grid.VirtualMode = false;

            // Data clear
            ClearData(grid, plotData);

            if (plotData.GetDataCount() <= 0)
            {
                return;
            }

            SuspendLayout();

            // Set double buffer
            // typeof(DataGridView).GetProperty("DoubleBuffered",
            //        System.Reflection.BindingFlags.Instance | System.Reflection.BindingFlags.NonPublic).
            //        SetValue(grid, true, null);

            // Set grid size
            grid.ColumnCount = plotData.ColumnCount;
            grid.RowCount = plotData.GetDataCount();

            // Get style data
            Color[] backColor = plotData.GetGridBkColor();
            FontStyle[] fontStyle = plotData.GetGridFontStyle();

            // Set title/style
            grid.ColumnHeadersDefaultCellStyle.Font = new Font(this.Font.FontFamily, this.Font.Size, FontStyle.Bold);
            for (int col = 0; col < plotData.ColumnCount; col++)
            {
                grid.Columns[col].SortMode = DataGridViewColumnSortMode.NotSortable;
                grid.Columns[col].HeaderText = plotData.GetStrValue(col, 0);
                grid.Columns[col].DefaultCellStyle.Font
                    = new Font(this.Font.FontFamily, this.Font.Size, fontStyle[col]);
                grid.Columns[col].DefaultCellStyle.BackColor = backColor[col];
            }

            // Set data
            SetData(grid, plotData);

            ResumeLayout(true);
        }

        /// <summary>
        /// Tab select
        /// </summary>
        /// <param name="dataType">data type</param>
        public void TabSelect(SimcaData.SIMCA_DATA_TYPE dataType)
        {
            _tabControlGrid.SelectedIndex = (int)dataType;
        }

        #endregion

        #region --- Private Events -------------------------------------
        /// <summary>
        ///  buttonClose Click event handler
        /// </summary>
        /// <param name="sender">object</param>
        /// <param name="e">EventArgs</param>
        private void ButtonClose_Click(object sender, EventArgs e)
        {
            Close();
        }

        /// <summary>
        /// FormClosed event handler
        /// </summary>
        /// <param name="sender">object</param>
        /// <param name="e">FormClosedEventArgs</param>
        private void FormSimcaGrid_FormClosed(object sender, FormClosedEventArgs e)
        {
            GridClosed(sender, e);
        }

        /// <summary>
        /// dataGridViewScore CellValueNeeded event handler
        /// </summary>
        /// <param name="sender">object</param>
        /// <param name="e">DataGridViewCellValueEventArgs</param>
        private void DataGridViewScore_CellValueNeeded(object sender, DataGridViewCellValueEventArgs e)
        {
            if (((DataGridView)sender).VirtualMode)
            {
                if (_dataList[(int)TAB_ID.SCORE_PLOT].Count() != 0)
                {
                    e.Value = _dataList[(int)TAB_ID.SCORE_PLOT][e.RowIndex][e.ColumnIndex];
                }
                else
                {
                    e.Value = string.Empty;
                }
            }
        }

        /// <summary>
        /// dataGridViewLoading CellValueNeeded event handler
        /// </summary>
        /// <param name="sender">object</param>
        /// <param name="e">DataGridViewCellValueEventArgs</param>
        private void DataGridViewLoading_CellValueNeeded(object sender, DataGridViewCellValueEventArgs e)
        {
            if (((DataGridView)sender).VirtualMode)
            {
                if (_dataList[(int)TAB_ID.LOADING_PLOT].Count() != 0)
                {
                    e.Value = _dataList[(int)TAB_ID.LOADING_PLOT][e.RowIndex][e.ColumnIndex];
                }
                else
                {
                    e.Value = string.Empty;
                }
            }
        }

        /// <summary>
        /// dataGridViewSPlot CellValueNeeded event handler
        /// </summary>
        /// <param name="sender">object</param>
        /// <param name="e">DataGridViewCellValueEventArgs</param>
        private void DataGridViewSPlot_CellValueNeeded(object sender, DataGridViewCellValueEventArgs e)
        {
            if (((DataGridView)sender).VirtualMode)
            {
                if (_dataList[(int)TAB_ID.S_PLOT].Count() != 0)
                {
                    e.Value = _dataList[(int)TAB_ID.S_PLOT][e.RowIndex][e.ColumnIndex];
                }
                else
                {
                    e.Value = string.Empty;
                }
            }
        }

        /// <summary>
        /// menuItemPaste Click event handler
        /// </summary>
        /// <param name="sender">object</param>
        /// <param name="e">EventArgs</param>
        private void MenuItemPaste_Click(object sender, EventArgs e)
        {
            string msg = string.Empty;
            string typeName = string.Empty;
            SimcaData.SIMCA_DATA_TYPE dataType = SimcaData.SIMCA_DATA_TYPE.SCORE_PLOT;

            // Set message
            switch ((TAB_ID)_tabControlGrid.SelectedIndex)
            {
                case TAB_ID.SCORE_PLOT:
                    dataType = SimcaData.SIMCA_DATA_TYPE.SCORE_PLOT;
                    typeName = "score";
                    msg = string.Format(MSG_FMT_CONVERT_GRID_VALUE, typeName);
                    break;
                case TAB_ID.LOADING_PLOT:
                    dataType = SimcaData.SIMCA_DATA_TYPE.LOADING_PLOT;
                    typeName = "loading";
                    msg = string.Format(MSG_FMT_CONVERT_GRID_VALUE, typeName);
                    break;
                case TAB_ID.S_PLOT:
                    dataType = SimcaData.SIMCA_DATA_TYPE.S_PLOT;
                    typeName = "S";
                    msg = string.Format(MSG_FMT_CONVERT_GRID_VALUE, typeName);
                    break;
                default:
                    break;
            }

            if (msg == string.Empty)
            {
                return;
            }

            // A check dialog is opened. 
            if (DialogResult.OK != MessageBox.Show(
                                        msg,
                                        "Question",
                                        MessageBoxButtons.OKCancel,
                                        MessageBoxIcon.Question))
            {
                return;
            }

            // Data will be set if data is in a clipboard. 
            PasteClipboardData(dataType, typeName);
        }

        /// <summary>
        /// tabControlGrid SelectedIndexChanged event handler
        /// </summary>
        /// <param name="sender">object</param>
        /// <param name="e">EventArgs</param>
        private void TabControlGrid_SelectedIndexChanged(object sender, EventArgs e)
        {
            UpdataButton();
        }
        
        /// <summary>
        /// buttonApply Click event handler
        /// </summary>
        /// <param name="sender">object</param>
        /// <param name="e">EventArgs</param>
        private void ButtonApply_Click(object sender, EventArgs e)
        {
            ApplyClickEventArgs pastData = new ApplyClickEventArgs();
            pastData._applyData = _pasteValue[_tabControlGrid.SelectedIndex];
            pastData._dataType = (SimcaData.SIMCA_DATA_TYPE)_tabControlGrid.SelectedIndex;
            ApplyClick(this, pastData);

            _enableApply[_tabControlGrid.SelectedIndex] = false;
            UpdataButton();
        }
        
        /// <summary>
        /// buttonSave Click event handler
        /// </summary>
        /// <param name="sender">object</param>
        /// <param name="e">EventArgs</param>
        private void ButtonSave_Click(object sender, EventArgs e)
        {
            string formatDlgTitle = "Save {0} data file";
            string typeName = string.Empty;
            switch ((TAB_ID)_tabControlGrid.SelectedIndex)
            {
                case TAB_ID.SCORE_PLOT:
                    typeName = "score";
                    break;
                case TAB_ID.LOADING_PLOT:
                    typeName = "loading";
                    break;
                case TAB_ID.S_PLOT:
                    typeName = "S";
                    break;
                default:
                    break;
            }

            // Show file open dialog
            SaveFileDialog sfd = new SaveFileDialog();
            sfd.FileName = typeName;
            sfd.InitialDirectory = string.Empty;
            sfd.Filter = "csv file(*.csv)|*.csv";
            sfd.FilterIndex = 0;
            sfd.Title = string.Format(formatDlgTitle, typeName);
            sfd.RestoreDirectory = true;
            if (sfd.ShowDialog() == DialogResult.OK)
            {
                // Write file data
                try
                {
                    using (StreamWriter sw = new StreamWriter(
                        sfd.FileName, false, Encoding.GetEncoding("Shift_JIS")))
                    {
                        sw.Write(_pasteValue[_tabControlGrid.SelectedIndex].Replace('\t', ','));
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }

        /// <summary>
        /// FormSimcaGrid KeyDown event handler
        /// </summary>
        /// <param name="sender">object</param>
        /// <param name="e">KeyEventArgs</param>
        private void FormSimcaGrid_KeyDown(object sender, KeyEventArgs e)
        {
            // Ctrl+V (Paste) event handling
            if (e.KeyCode != Keys.V)
            {
                return;
            }

            if ((e.Modifiers & Keys.Control) == Keys.Control)
            {
                MenuItemPaste_Click(sender, null);
            }
        }

        #endregion

        #region --- Private Methods ------------------------------------
        /// <summary>
        /// Update button
        /// </summary>
        private void UpdataButton()
        {
            _buttonApply.Enabled = _enableApply[_tabControlGrid.SelectedIndex];
            _buttonSave.Enabled = _enableSave[_tabControlGrid.SelectedIndex];
        }

        /// <summary>
        /// Clear data
        /// </summary>
        /// <param name="grid">grid object</param>
        /// <param name="plotData">plot data</param>
        private void ClearData(DataGridView grid, SimcaData plotData)
        {
            grid.Rows.Clear();
            grid.Columns.Clear();
            int index = (int)plotData.DataType;
            if (_dataList[index] == null)
            {
                _dataList[index] = new List<List<string>>();
            }

            foreach (List<string> item in _dataList[index])
            {
                item.Clear();
            }

            _dataList[index].Clear();
            _pasteValue[index] = string.Empty;
            _enableApply[index] = false;
            _enableSave[index] = false;
            UpdataButton();
        }

        /// <summary>
        /// Set data
        /// </summary>
        /// <param name="grid">DataGridView</param>
        /// <param name="plotData">SimcaData</param>
        private void SetData(DataGridView grid, SimcaData plotData)
        {
            if (plotData.GetDataCount() > ENABLE_VIRTUAL_MODE_LINE_COUNT)
            {
                grid.VirtualMode = true;
                int index = (int)plotData.DataType;
                for (int row = 0; row < grid.RowCount; row++)
                {
                    List<string> itemList = new List<string>();
                    itemList.Clear();

                    for (int col = 0; col < grid.ColumnCount; col++)
                    {
                        itemList.Add(plotData.GetStrValue(col, row + 1));
                    }

                    _dataList[index].Add(itemList);
                }
            }
            else
            {
                grid.VirtualMode = false;
                for (int row = 0; row < grid.RowCount; row++)
                {
                    for (int col = 0; col < grid.ColumnCount; col++)
                    {
                        grid[col, row].Value = plotData.GetStrValue(col, row + 1);
                    }
                }
            }
        }

        /// <summary>
        /// Paste of clipboard data
        /// </summary>
        /// <param name="dataType">Data type</param>
        /// <param name="typeName">Data type name</param>
        private void PasteClipboardData(SimcaData.SIMCA_DATA_TYPE dataType, string typeName)
        {
            // Set wait Cursor
            Cursor preCursor = Cursor.Current;
            Cursor.Current = Cursors.WaitCursor;

            int index = _tabControlGrid.SelectedIndex;

            // Data will be set if data is in a clipboard. 
            if (Clipboard.ContainsText())
            {
                string clipboardText = Clipboard.GetText();
                SimcaData.SIMCA_READ_ERR_CODE errorCode = SimcaData.SIMCA_READ_ERR_CODE.OK;

                // Convert TSV to CSV
                string workPaste = ConvTsvToCsv(clipboardText);
                if (workPaste == null)
                {
                    errorCode = SimcaData.SIMCA_READ_ERR_CODE.NG_FORMAT;
                }

                SimcaData tempData = new SimcaData(dataType);
                if (errorCode == SimcaData.SIMCA_READ_ERR_CODE.OK)
                {
                    errorCode = tempData.SetPasteData(workPaste, null);
                }

                string formatErrMsg;
                if (errorCode == SimcaData.SIMCA_READ_ERR_CODE.NG_DATA)
                {   // NG_DATA
                    formatErrMsg = ERR_MSG_FMT_FILE_READ_DATA;
                }
                else if (errorCode == SimcaData.SIMCA_READ_ERR_CODE.NG_FORMAT)
                {   // NG_FORMAT
                    formatErrMsg = ERR_MSG_FMT_FILE_READ_FMT;
                }
                else
                {   // OK
                    Update(tempData);
                    _pasteValue[index] = workPaste;
                    _enableApply[index] = true;
                    _enableSave[index] = true;
                    UpdataButton();
                    Cursor.Current = preCursor;
                    return;
                }

                string workMsg = string.Format(formatErrMsg, typeName, "paste data");
                workMsg = char.ToUpper(workMsg[0]) + workMsg.Substring(1);
                MessageBox.Show(
                    workMsg,
                    "Error",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Error);
                SimcaData clrData = new SimcaData(dataType);
                Update(clrData);
                _pasteValue[index] = string.Empty;
                _enableApply[index] = false;
                _enableSave[index] = false;
                UpdataButton();
                Cursor.Current = preCursor;
                return;
            }
        }

        /// <summary>
        /// Convert TSV to CSV
        /// </summary>
        /// <param name="src">TSV string</param>
        /// <returns>CSV string</returns>
        private string ConvTsvToCsv(string src)
        {
            if (src.IndexOf('\t') == -1)
            {
                return src;
            }

            src.Trim();
            if (src == string.Empty)
            {
                return src;
            }
            StringBuilder res = new StringBuilder();

            try
            {
                Stream stream = new MemoryStream(System.Text.Encoding.GetEncoding("Shift_JIS").GetBytes(src));

                using (TextFieldParser parser = new TextFieldParser(
                     stream,
                     System.Text.Encoding.GetEncoding("Shift_JIS")))
                {
                    parser.TextFieldType = FieldType.Delimited;
                    parser.SetDelimiters("\t");
                    while (parser.EndOfData == false)
                    {
                        string[] column = parser.ReadFields();

                        foreach (string item in column)
                        {
                            if (item.IndexOf(',') != -1)
                            {
                                res.Append("\"");
                                res.Append(item);
                                res.Append("\"");
                            }
                            else
                            {
                                res.Append(item);
                            }
                            res.Append(",");
                        }
                        if (column.Count() != 0)
                        {
                            res.Remove(res.Length - 1, 1);
                            res.Append("\r\n");
                        }
                    }
                }
            }
            catch (Exception)
            {
                return src;
            }

            return res.ToString();
        }

        #endregion
    }
}
