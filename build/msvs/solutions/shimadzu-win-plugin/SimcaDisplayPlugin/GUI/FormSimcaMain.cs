/// <summary>
/// [FILE] FormSimcaMain.cs
/// [ABSTRACT] SIMCA Plugin main form class
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
using Microsoft.VisualBasic.FileIO;

// Mass++ namespace
using kome.clr;
using SimcaDisplay.Data;

namespace SimcaDisplay.Gui
{
    /// <summary>
    /// SIMCA plug-in main form class
    /// </summary>
    internal partial class FormSimcaMain : Form
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
            /// Tab max count
            /// </summary>
            MAX
        }

        /// <summary>
        /// Error message format(Data)
        /// </summary>
        private const string ERR_MSG_FMT_FILE_READ_DATA = "{0} plot file ({1}) should contain at least 2 components.";
        
        /// <summary>
        /// Error message format(Data format)
        /// </summary>
        private const string ERR_MSG_FMT_FILE_READ_FMT = "Failed to parse {1} as a {0} plot file.";
        
        /// <summary>
        /// Error message format(Read file)
        /// </summary>
        private const string ERR_MSG_FMT_READ_FILE = "the reading of {1} went wrong. "; 
        
        /// <summary>
        /// Warning message format(Class)
        /// </summary>
        private const string WARN_MSG_FMT_MSG_CLASS = "Classification of class in file {0} is not coincident with DB data.";
        #endregion

        #region --- Variables ------------------------------------------
        /// <summary>
        /// Mouse position data (Score plot graph)
        /// </summary>
        private UserControlSimcaGraph.MousePositionEventArgs _graphScoreMousePos = new UserControlSimcaGraph.MousePositionEventArgs();
        
        /// <summary>
        /// Mouse position data (Loading plot graph)
        /// </summary>
        private UserControlSimcaGraph.MousePositionEventArgs _graphLoadingMousePos = new UserControlSimcaGraph.MousePositionEventArgs();
        
        /// <summary>
        /// Mouse position data (S plot graph)
        /// </summary>
        private UserControlSimcaGraph.MousePositionEventArgs _graphSPlotMousePos = new UserControlSimcaGraph.MousePositionEventArgs();

        /// <summary>
        /// DB data
        /// </summary>
        private SimcaDbData _dbData = new SimcaDbData();

        /// <summary>
        /// File data (Score plot)
        /// </summary>
        private SimcaData _scoreData = new SimcaData(SimcaData.SIMCA_DATA_TYPE.SCORE_PLOT);
        
        /// <summary>
        /// File data (Loading plot)
        /// </summary>
        private SimcaData _loadingData = new SimcaData(SimcaData.SIMCA_DATA_TYPE.LOADING_PLOT);
        
        /// <summary>
        /// File data (S plot)
        /// </summary>
        private SimcaData _sPlotData = new SimcaData(SimcaData.SIMCA_DATA_TYPE.S_PLOT);
        
        /// <summary>
        /// FormSimcaGrid instance
        /// </summary>
        private FormSimcaGrid _formGridInstance = null;
        
        /// <summary>
        /// File path
        /// </summary>
        private string[] _filePath = new string[(int)TAB_ID.MAX];

        #endregion

        #region --- Events ---------------------------------------------
        /// <summary>
        /// Event(Form closed)
        /// </summary>
        public event Action<object, FormClosedEventArgs> DisplayClosed;

        #endregion

        #region --- Construction ---------------------------------------
        /// <summary>
        /// Initializes a new instance of the FormSimcaMain class
        /// </summary>
        public FormSimcaMain()
        {
            InitializeComponent();

            // Set graph type
            _graphScore.DataType = SimcaData.SIMCA_DATA_TYPE.SCORE_PLOT;
            _graphLoading.DataType = SimcaData.SIMCA_DATA_TYPE.LOADING_PLOT;
            _graphSPlot.DataType = SimcaData.SIMCA_DATA_TYPE.S_PLOT;

            // Event(Graph ChangeMousePos)
            _graphScore.ChangeMousePos += Graph_ChangeMousePos;
            _graphLoading.ChangeMousePos += Graph_ChangeMousePos;
            _graphSPlot.ChangeMousePos += Graph_ChangeMousePos;

            // Event(Graph SelectAxisBtnClick)
            _graphScore.SelectAxisBtnClick += Graph_SelectAxisBtnClick;
            _graphLoading.SelectAxisBtnClick += Graph_SelectAxisBtnClick;
            _graphSPlot.SelectAxisBtnClick += Graph_SelectAxisBtnClick;

            // Event(Graph ChangeSelectItem)
            _graphScore.ChangeSelectItem += Graph_ChangeSelectItem;
            _graphLoading.ChangeSelectItem += Graph_ChangeSelectItem;
            _graphSPlot.ChangeSelectItem += Graph_ChangeSelectItem;

            // DB read
            _dbData.ReadDb();

            // Initialize display
            _filePath[(int)TAB_ID.SCORE_PLOT] = string.Empty;
            _filePath[(int)TAB_ID.LOADING_PLOT] = string.Empty;
            _filePath[(int)TAB_ID.S_PLOT] = string.Empty;
            _comboBoxSelMatrix.Items.Clear();
            _comboBoxSelMatrix.Items.Add(string.Empty);
            foreach (string item in _dbData.GetMatrixNameArray())
            {
                _comboBoxSelMatrix.Items.Add(item);
            }

            int selectedIndex = _dbData.GetSelectedMatrixIndex();
            _comboBoxSelMatrix.SelectedIndex = selectedIndex + 1;
            _comboBoxSelMatrix.SelectedIndexChanged += ComboBoxSelMatrix_SelectedIndexChanged;

            // Default data Set
            _graphScore.SetDefaultData();
            _graphLoading.SetDefaultData();
            _graphSPlot.SetDefaultData();
        }
        #endregion

        #region --- Private Events -------------------------------------
        /// <summary>
        /// Graph ChangeMousePos event handler
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">event args</param>
        private void Graph_ChangeMousePos(object sender, UserControlSimcaGraph.MousePositionEventArgs e)
        {
            int tabId = (int)TAB_ID.SCORE_PLOT;

            if (sender.Equals(_graphScore))
            {
                _graphScoreMousePos.InsideOfChartArea = e.InsideOfChartArea;
                _graphScoreMousePos.ValueX = e.ValueX;
                _graphScoreMousePos.ValueY = e.ValueY;
                _graphScoreMousePos.LabelFormatX = e.LabelFormatX;
                _graphScoreMousePos.LabelFormatY = e.LabelFormatY;
                tabId = (int)TAB_ID.SCORE_PLOT;
            }

            if (sender.Equals(_graphLoading))
            {
                _graphLoadingMousePos.InsideOfChartArea = e.InsideOfChartArea;
                _graphLoadingMousePos.ValueX = e.ValueX;
                _graphLoadingMousePos.ValueY = e.ValueY;
                _graphLoadingMousePos.LabelFormatX = e.LabelFormatX;
                _graphLoadingMousePos.LabelFormatY = e.LabelFormatY;
                tabId = (int)TAB_ID.LOADING_PLOT;
            }

            if (sender.Equals(_graphSPlot))
            {
                _graphSPlotMousePos.InsideOfChartArea = e.InsideOfChartArea;
                _graphSPlotMousePos.ValueX = e.ValueX;
                _graphSPlotMousePos.ValueY = e.ValueY;
                _graphSPlotMousePos.LabelFormatX = e.LabelFormatX;
                _graphSPlotMousePos.LabelFormatY = e.LabelFormatY;
                tabId = (int)TAB_ID.S_PLOT;
            }

            string workX = "---";
            string workY = "---";
            if (_tabControlPlot.SelectedIndex == tabId)
            {
                if (e.InsideOfChartArea)
                {
                    workX = string.Format(e.LabelFormatX, e.ValueX);
                    workY = string.Format(e.LabelFormatY, e.ValueY);
                }

                _toolStripLabelPos.Text = "(" + workX + "," + workY + ")";
            }
        }
        
        /// <summary>
        /// Tab SelectedIndexChanged event handler
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">event args</param>
        private void TabControlPlot_SelectedIndexChanged(object sender, EventArgs e)
        {
            string workX = "---";
            string workY = "---";

            Font fontData = _labelScore.Font;
            Font fontRegular = new Font(fontData.FontFamily, fontData.Size, FontStyle.Regular);
            Font fontBold = new Font(fontData.FontFamily, fontData.Size, FontStyle.Bold);

            _labelScore.Font = fontRegular;
            _labelLoading.Font = fontRegular;
            _labelSPlot.Font = fontRegular;
            _labelScorePath.Font = fontRegular;
            _labelLoadingPath.Font = fontRegular;
            _labelSPlotPath.Font = fontRegular;
            LabelSetText(_labelScorePath, _filePath[(int)TAB_ID.SCORE_PLOT]);
            LabelSetText(_labelLoadingPath, _filePath[(int)TAB_ID.LOADING_PLOT]);
            LabelSetText(_labelSPlotPath, _filePath[(int)TAB_ID.S_PLOT]);

            switch ((TAB_ID)_tabControlPlot.SelectedIndex)
            {
                case TAB_ID.SCORE_PLOT:
                    _labelScore.Font = fontBold;
                    _labelScorePath.Font = fontBold;
                    LabelSetText(_labelScorePath, _filePath[(int)TAB_ID.SCORE_PLOT]);
                    if (_graphLoadingMousePos.InsideOfChartArea)
                    {
                        workX = string.Format(_graphLoadingMousePos.LabelFormatX, _graphLoadingMousePos.ValueX);
                        workY = string.Format(_graphLoadingMousePos.LabelFormatY, _graphLoadingMousePos.ValueY);
                    }

                    _graphScore.RequestChangeSelectEvent();
                    break;
                case TAB_ID.LOADING_PLOT:
                    _labelLoading.Font = fontBold;
                    _labelLoadingPath.Font = fontBold;
                    LabelSetText(_labelLoadingPath, _filePath[(int)TAB_ID.LOADING_PLOT]);
                    if (_graphLoadingMousePos.InsideOfChartArea)
                    {
                        workX = string.Format(_graphLoadingMousePos.LabelFormatX, _graphLoadingMousePos.ValueX);
                        workY = string.Format(_graphLoadingMousePos.LabelFormatY, _graphLoadingMousePos.ValueY);
                    }

                    _graphLoading.RequestChangeSelectEvent();
                    break;
                case TAB_ID.S_PLOT:
                    _labelSPlot.Font = fontBold;
                    _labelSPlotPath.Font = fontBold;
                    LabelSetText(_labelSPlotPath, _filePath[(int)TAB_ID.S_PLOT]);
                    if (_graphSPlotMousePos.InsideOfChartArea)
                    {
                        workX = string.Format(_graphSPlotMousePos.LabelFormatX, _graphSPlotMousePos.ValueX);
                        workY = string.Format(_graphSPlotMousePos.LabelFormatY, _graphSPlotMousePos.ValueY);
                    }

                    _graphSPlot.RequestChangeSelectEvent();
                    break;
                default:
                    break;
            }

            _toolStripLabelPos.Text = "(" + workX + "," + workY + ")";
        }
        
        /// <summary>
        /// Form Activated event handler
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">event args</param>
        private void FormSimcaMain_Activated(object sender, EventArgs e)
        {
            int selectedMatrixId = _dbData.SelectedMatrixId;

            if (_checkBoxSyncQuant.Checked)
            {
                if (!_dbData.IsChanged())
                {
                    return;
                }
            }
            else
            {
                if (!_dbData.IsChanged(selectedMatrixId))
                {
                    return;
                }
            }

            // Update Selected Matrix ID
            _comboBoxSelMatrix.SelectedIndexChanged -= ComboBoxSelMatrix_SelectedIndexChanged;
            _comboBoxSelMatrix.Items.Clear();
            _comboBoxSelMatrix.Items.Add(string.Empty);
            foreach (string item in _dbData.GetMatrixNameArray())
            {
                _comboBoxSelMatrix.Items.Add(item);
            }

            int selectedIndex = _dbData.GetSelectedMatrixIndex();
            _comboBoxSelMatrix.SelectedIndex = selectedIndex + 1;
            _comboBoxSelMatrix.SelectedIndexChanged += ComboBoxSelMatrix_SelectedIndexChanged;

            // Clear fiile path view
            _labelScorePath.Text = string.Empty;
            _labelLoadingPath.Text = string.Empty;
            _labelSPlotPath.Text = string.Empty;
            _filePath[(int)TAB_ID.SCORE_PLOT] = string.Empty;
            _filePath[(int)TAB_ID.LOADING_PLOT] = string.Empty;
            _filePath[(int)TAB_ID.S_PLOT] = string.Empty;

            // Clear Data
            _scoreData.Clear();
            _loadingData.Clear();
            _sPlotData.Clear();

            // Clear graph
            _graphScore.SetDefaultData();
            _graphLoading.SetDefaultData();
            _graphSPlot.SetDefaultData();

            if (_formGridInstance != null)
            {
                _formGridInstance.Update(_scoreData);
                _formGridInstance.Update(_loadingData);
                _formGridInstance.Update(_sPlotData);
            }
        }
        
        /// <summary>
        /// Select Matrix combo box SelectedIndexChanged event handler
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">event args</param>
        private void ComboBoxSelMatrix_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (_checkBoxSyncQuant.Checked)
            {
                return;
            }

            int selIndex = _comboBoxSelMatrix.SelectedIndex;
            int selId = _dbData.ConvSelectedMatrixIndexToId(selIndex);

            if (selId == _dbData.SelectedMatrixId)
            {
                return;
            }

            // DB read
            _dbData.ReadDb(selId);

            // Clear fiile path view
            _labelScorePath.Text = string.Empty;
            _labelLoadingPath.Text = string.Empty;
            _labelSPlotPath.Text = string.Empty;
            _filePath[(int)TAB_ID.SCORE_PLOT] = string.Empty;
            _filePath[(int)TAB_ID.LOADING_PLOT] = string.Empty;
            _filePath[(int)TAB_ID.S_PLOT] = string.Empty;

            // Clear Data
            _scoreData.Clear();
            _loadingData.Clear();
            _sPlotData.Clear();

            // Clear graph
            _graphScore.SetDefaultData();
            _graphLoading.SetDefaultData();
            _graphSPlot.SetDefaultData();

            if (_formGridInstance != null)
            {
                _formGridInstance.Update(_scoreData);
                _formGridInstance.Update(_loadingData);
                _formGridInstance.Update(_sPlotData);
            }
        }
        
        /// <summary>
        /// buttonSelScore Click event handler
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">event args</param>
        private void ButtonSelScore_Click(object sender, EventArgs e)
        {
            ReadFile(_scoreData, _graphScore);
        }
        
        /// <summary>
        /// buttonSelLoading Click event handler
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">event args</param>
        private void ButtonSelLoading_Click(object sender, EventArgs e)
        {
            ReadFile(_loadingData, _graphLoading);
        }
        
        /// <summary>
        /// buttonSelSPlot Click event handler
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">event args</param>
        private void ButtonSelSPlot_Click(object sender, EventArgs e)
        {
            ReadFile(_sPlotData, _graphSPlot);
        }
        
        /// <summary>
        /// checkBoxSyncQuant CheckedChanged event handler
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">event args</param>
        private void CheckBoxSyncQuant_CheckedChanged(object sender, EventArgs e)
        {
            if (_checkBoxSyncQuant.Checked)
            {
                _comboBoxSelMatrix.Enabled = false;

                int selectedMatrixId = _dbData.SelectedMatrixId;
                if (!_dbData.IsChanged())
                {
                    switch ((TAB_ID)_tabControlPlot.SelectedIndex)
                    {
                        case TAB_ID.SCORE_PLOT:
                            _graphScore.RequestChangeSelectEvent();
                            break;
                        case TAB_ID.LOADING_PLOT:
                            _graphLoading.RequestChangeSelectEvent();
                            break;
                        case TAB_ID.S_PLOT:
                            _graphSPlot.RequestChangeSelectEvent();
                            break;
                        default:
                            break;
                    }

                    return;
                }

                // Update Selected Matrix ID
                _comboBoxSelMatrix.SelectedIndexChanged -= ComboBoxSelMatrix_SelectedIndexChanged;
                _comboBoxSelMatrix.Items.Clear();
                _comboBoxSelMatrix.Items.Add(string.Empty);
                foreach (string item in _dbData.GetMatrixNameArray())
                {
                    _comboBoxSelMatrix.Items.Add(item);
                }

                int selectedIndex = _dbData.GetSelectedMatrixIndex();
                _comboBoxSelMatrix.SelectedIndex = selectedIndex + 1;
                _comboBoxSelMatrix.SelectedIndexChanged += ComboBoxSelMatrix_SelectedIndexChanged;

                // Clear fiile path view
                _labelScorePath.Text = string.Empty;
                _labelLoadingPath.Text = string.Empty;
                _labelSPlotPath.Text = string.Empty;
                _filePath[(int)TAB_ID.SCORE_PLOT] = string.Empty;
                _filePath[(int)TAB_ID.LOADING_PLOT] = string.Empty;
                _filePath[(int)TAB_ID.S_PLOT] = string.Empty;

                // Clear Data
                _scoreData.Clear();
                _loadingData.Clear();
                _sPlotData.Clear();

                // Clear graph
                _graphScore.SetDefaultData();
                _graphLoading.SetDefaultData();
                _graphSPlot.SetDefaultData();

                if (_formGridInstance != null)
                {
                    _formGridInstance.Update(_scoreData);
                    _formGridInstance.Update(_loadingData);
                    _formGridInstance.Update(_sPlotData);
                }
            }
            else
            {
                _comboBoxSelMatrix.Enabled = true;
            }
        }
        
        /// <summary>
        /// checkBoxShowTables CheckedChanged event handler
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">event args</param>
        private void CheckBoxShowTables_CheckedChanged(object sender, EventArgs e)
        {
            if (_checkBoxShowTables.Checked)
            {
                if (_formGridInstance == null)
                {
                    _formGridInstance = new FormSimcaGrid(_scoreData, _loadingData, _sPlotData, _tabControlPlot.SelectedIndex);
                }

                _formGridInstance.GridClosed += FormSimcaGrid_FormClosed;
                _formGridInstance.ApplyClick += FormSimcaGrid_ApplyClick;
                _formGridInstance.Show(this);
            }
            else
            {
                if (_formGridInstance != null)
                {
                    _formGridInstance.Close();
                }
            }
        }
        
        /// <summary>
        /// buttonClose Click event handler
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">event args</param>
        private void ButtonClose_Click(object sender, EventArgs e)
        {
            Close();
        }
        
        /// <summary>
        /// Graph ChangeMousePos event handler
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">event args</param>
        private void Graph_SelectAxisBtnClick(object sender, EventArgs e)
        {
            SimcaData data = null;
            UserControlSimcaGraph graph = null;

            switch ((TAB_ID)_tabControlPlot.SelectedIndex)
            {
                case TAB_ID.SCORE_PLOT:
                    data = _scoreData;
                    graph = _graphScore;
                    break;
                case TAB_ID.LOADING_PLOT:
                    data = _loadingData;
                    graph = _graphLoading;
                    break;
                case TAB_ID.S_PLOT:
                    data = _sPlotData;
                    graph = _graphSPlot;
                    break;
                default:
                    break;
            }

            if (data == null)
            {
                return;
            }

            FormSimcaSelectAxis dlg = new FormSimcaSelectAxis(data);

            if (dlg.ShowDialog(this) != DialogResult.OK)
            {
                dlg.Dispose();
                return;
            }

            if ((dlg.SelectAxisIndexX != data.AxisXDataIndex)
            || (dlg.SelectAxisIndexY != data.AxisYDataIndex))
            {
                data.AxisXDataIndex = dlg.SelectAxisIndexX;
                data.AxisYDataIndex = dlg.SelectAxisIndexY;
                graph.SetData(data);
            }
        }
        
        /// <summary>
        /// Graph ChangeSelectItem event handler
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">event args</param>
        private void Graph_ChangeSelectItem(object sender, UserControlSimcaGraph.SelectItemEventArgs e)
        {
            if (!_checkBoxSyncQuant.Checked)
            {
                return;
            }

            bool activeTab = false;
            switch ((TAB_ID)_tabControlPlot.SelectedIndex)
            {
                case TAB_ID.SCORE_PLOT:
                    if ((UserControlSimcaGraph)sender == _graphScore)
                    {
                        activeTab = true;
                    }

                    break;

                case TAB_ID.LOADING_PLOT:
                    if ((UserControlSimcaGraph)sender == _graphLoading)
                    {
                        activeTab = true;
                    }

                    break;

                case TAB_ID.S_PLOT:
                    if ((UserControlSimcaGraph)sender == _graphSPlot)
                    {
                        activeTab = true;
                    }

                    break;

                default:
                    break;
            }

            if (!activeTab)
            {
                return;
            }

            // Renewal of an emphasis display
            IdentificationManagerWrapper managerWrapper = IdentificationManagerWrapper.getInstance();
            managerWrapper.clearFocusedPeaks();    // clear
            if (e.SelectItemArray != null)
            {
                foreach (int peakId in e.SelectItemArray)
                {
                    managerWrapper.addFocusedPeak(peakId);
                    if (e.DoubleClick)
                    {
                        managerWrapper.activatePeak(peakId);
                    }
                }
            }

            ClrPluginCallTool.refresh();
        }
        
        /// <summary>
        /// FormClosed event handler
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">event args</param>
        private void FormSimcaMain_FormClosed(object sender, FormClosedEventArgs e)
        {
            DisplayClosed(sender, e);
        }
        
        /// <summary>
        /// FormSimcaGrid FormClosed evevt handler
        /// </summary>
        /// <param name="sender">object</param>
        /// <param name="e">event args</param>
        private void FormSimcaGrid_FormClosed(object sender, FormClosedEventArgs e)
        {
            if (_formGridInstance != null)
            {
                _formGridInstance.GridClosed -= FormSimcaGrid_FormClosed;
                _formGridInstance.ApplyClick -= FormSimcaGrid_ApplyClick;
                _formGridInstance.Dispose();
                _formGridInstance = null;
            }

            _checkBoxShowTables.Checked = false;
        }

        /// <summary>
        /// FormSimcaGrid ApplyClick evevt handler
        /// </summary>
        /// <param name="sender">object</param>
        /// <param name="e">ApplyClickEventArgs</param>
        private void FormSimcaGrid_ApplyClick(object sender, FormSimcaGrid.ApplyClickEventArgs e)
        {
            SimcaData.SIMCA_READ_ERR_CODE errorCode = SimcaData.SIMCA_READ_ERR_CODE.OK;

            string typeName = "score";
            Label pathLabel = _labelScorePath;
            UserControlSimcaGraph graph = _graphScore;
            SimcaData data = _scoreData;
            switch (e._dataType)
            {
                case SimcaData.SIMCA_DATA_TYPE.SCORE_PLOT:
                    typeName = "score";
                    pathLabel = _labelScorePath;
                    graph = _graphScore;
                    data = _scoreData;
                    break;
                case SimcaData.SIMCA_DATA_TYPE.LOADING_PLOT:
                    typeName = "loading";
                    pathLabel = _labelLoadingPath;
                    graph = _graphLoading;
                    data = _loadingData;
                    break;
                case SimcaData.SIMCA_DATA_TYPE.S_PLOT:
                    typeName = "S";
                    pathLabel = _labelSPlotPath;
                    graph = _graphSPlot;
                    data = _sPlotData;
                    break;
                default:
                    break;
            }

            // Tab select
            _tabControlPlot.SelectedIndex = (int)e._dataType;

            data.Clear();
            errorCode = data.SetPasteData(e._applyData, _dbData);

            pathLabel.Text = string.Empty;
            _filePath[(int)e._dataType] = string.Empty;

            string workMsg;
            if (errorCode == SimcaData.SIMCA_READ_ERR_CODE.NG_CLASS)
            {
                workMsg = string.Format(WARN_MSG_FMT_MSG_CLASS, "paste data");
                MessageBox.Show(
                    workMsg,
                    "Warning",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Warning);
                errorCode = SimcaData.SIMCA_READ_ERR_CODE.OK;
            }

            if (errorCode == SimcaData.SIMCA_READ_ERR_CODE.OK)
            {
                graph.SetData(data);
                //if (_formGridInstance != null)
                //{
                //    _formGridInstance.Update(data);
                //}

                return;
            }

            string formatErrMsg;
            if (errorCode == SimcaData.SIMCA_READ_ERR_CODE.NG_DATA)
            {
                formatErrMsg = ERR_MSG_FMT_FILE_READ_DATA;
            }
            else if (errorCode == SimcaData.SIMCA_READ_ERR_CODE.NG_READ)
            {
                formatErrMsg = ERR_MSG_FMT_READ_FILE;
            }
            else
            {
                formatErrMsg = ERR_MSG_FMT_FILE_READ_FMT;
            }

            workMsg = string.Format(formatErrMsg, typeName, "paste data");
            workMsg = char.ToUpper(workMsg[0]) + workMsg.Substring(1);
            MessageBox.Show(
                workMsg,
                "Error",
                MessageBoxButtons.OK,
                MessageBoxIcon.Error);
            graph.SetDefaultData();
            data.Clear();
            if (_formGridInstance != null)
            {
                _formGridInstance.Update(data);
            }
        }
        #endregion

        #region --- Private Methods ------------------------------------
        /// <summary>
        /// Read file
        /// </summary>
        /// <param name="data">The target SIMCA data</param>
        /// <param name="graph">The target graph</param>
        private void ReadFile(SimcaData data, UserControlSimcaGraph graph)
        {
            string formatDlgTitle = "Select {0} data file";
            string formatErrMsg = string.Empty;

            // type
            string typeName = "score";
            Label pathLabel = _labelScorePath;
            if (data == _scoreData)
            {
                typeName = "score";
                pathLabel = _labelScorePath;
            }

            if (data == _loadingData)
            {
                typeName = "loding";
                pathLabel = _labelLoadingPath;
            }

            if (data == _sPlotData)
            {
                typeName = "S";
                pathLabel = _labelSPlotPath;
            }

            // Show file open dialog
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.FileName = string.Empty;
            ofd.InitialDirectory = string.Empty;
            ofd.Filter = "csv file(*.csv)|*.csv";
            ofd.FilterIndex = 0;
            ofd.Title = string.Format(formatDlgTitle, typeName);
            ofd.RestoreDirectory = true;
            if (ofd.ShowDialog() == DialogResult.OK)
            {
                // Tab select
                _tabControlPlot.SelectedIndex = (int)data.DataType;

                // Grid tab select
                if (_formGridInstance != null)
                {
                    _formGridInstance.TabSelect(data.DataType);
                }
                
                // Read file data
                SimcaData.SIMCA_READ_ERR_CODE errorCode = SimcaData.SIMCA_READ_ERR_CODE.OK;
                data.Clear();
                try
                {
                    using (TextFieldParser parser = new TextFieldParser(
                         ofd.FileName,
                         System.Text.Encoding.GetEncoding("Shift_JIS")))
                    {
                        parser.TextFieldType = FieldType.Delimited;
                        parser.SetDelimiters(",");
                        while (parser.EndOfData == false)
                        {
                            errorCode = data.Add(parser.ReadFields(), _dbData);
                            if (errorCode != SimcaData.SIMCA_READ_ERR_CODE.OK)
                            {
                                break;
                            }
                        }
                    }
                }
                catch (Exception ex)
                {
                    formatErrMsg = ex.Message;
                    errorCode = SimcaData.SIMCA_READ_ERR_CODE.NG_READ;
                }

                if (errorCode == SimcaData.SIMCA_READ_ERR_CODE.OK)
                {
                    errorCode = data.CheckData(_dbData.GetSelectedMatrixIndex());
                }

                if (errorCode == SimcaData.SIMCA_READ_ERR_CODE.NG_CLASS)
                {
                    string workMsg = string.Format(WARN_MSG_FMT_MSG_CLASS, Path.GetFileName(ofd.FileName));
                    MessageBox.Show(
                        workMsg,
                        "Warning",
                        MessageBoxButtons.OK,
                        MessageBoxIcon.Warning);
                    errorCode = SimcaData.SIMCA_READ_ERR_CODE.OK;
                }

                if (errorCode == SimcaData.SIMCA_READ_ERR_CODE.OK)
                {
                    graph.SetData(data);
                    _filePath[(int)data.DataType] = ofd.FileName;
                    LabelSetText(pathLabel, ofd.FileName);

                    if (_formGridInstance != null)
                    {
                        _formGridInstance.Update(data);
                    }
                    return;
                }

                if (errorCode == SimcaData.SIMCA_READ_ERR_CODE.NG_DATA)
                {
                    formatErrMsg = ERR_MSG_FMT_FILE_READ_DATA;
                }
                else if (errorCode == SimcaData.SIMCA_READ_ERR_CODE.NG_READ)
                {
                    // formatErrMsg = ERR_MSG_FMT_READ_FILE;
                }
                else
                {
                    formatErrMsg = ERR_MSG_FMT_FILE_READ_FMT;
                }

                string work = string.Format(formatErrMsg, typeName, Path.GetFileName(ofd.FileName));
                work = char.ToUpper(work[0]) + work.Substring(1);
                MessageBox.Show(
                    work,
                    "Error",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Error);
                graph.SetDefaultData();
                data.Clear();
                if (_formGridInstance != null)
                {
                    _formGridInstance.Update(data);
                }

                pathLabel.Text = string.Empty;
                _filePath[(int)data.DataType] = string.Empty;
            }
        }

        /// <summary>
        /// Adjust text length
        /// </summary>
        /// <param name="pathLabel">Label</param>
        /// <param name="path">string</param>
        private void LabelSetText(Label pathLabel, string path)
        {
            _labelSizeCheck.Font = pathLabel.Font;
            _labelSizeCheck.Text = path;
            while (_labelSizeCheck.Width > pathLabel.Width)
            {
                _labelSizeCheck.Text = _labelSizeCheck.Text.Remove(0, 1);
            }

            pathLabel.Text = _labelSizeCheck.Text;
        }

        #endregion
    }
}
