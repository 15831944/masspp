/// <summary>
/// [FILE] UserControlSimcaGraph.cs
/// [ABSTRACT] SIMCA Plugin Graph User Control class
/// Copyright (C) 2012-12-18 Shimadzu
/// </summary>

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;

using SimcaDisplay.Data;

namespace SimcaDisplay.Gui
{
    /// <summary>
    /// Graph area control
    /// </summary>
    internal partial class UserControlSimcaGraph : UserControl
    {
        #region --- Constants ------------------------------------------
        /// <summary>
        /// The number of grid lines (Axis X)
        /// </summary>
        private const int GRID_COUNT_X = 5;

        /// <summary>
        /// The number of grid lines (Axis Y)
        /// </summary>
        private const int GRID_COUNT_Y = 5;

        /// <summary>
        /// Graph custom property (Selected)
        /// </summary>
        private const string PROPERTY_NAME_SELECTED = "Selected";
        
        /// <summary>
        /// Graph custom property (Number)
        /// </summary>
        private const string PROPERTY_NAME_NUMBER = "Number";
        
        /// <summary>
        /// Graph custom property (PeakId)
        /// </summary>
        private const string PROPERTY_NAME_PEAK_ID = "PeakId";

        /// <summary>
        /// Graph custom property (Selected) selected Value
        /// </summary>
        private const string SELECTED = "Yes";
        
        /// <summary>
        /// Graph custom property (Selected) not selected Value
        /// </summary>
        private const string NOT_SELECTED = "No";

        /// <summary>
        /// Graph custom property (Initial setting data)
        /// </summary>
        private const string INIT_CUSTOM_PROPERTY = PROPERTY_NAME_SELECTED + " = " + NOT_SELECTED + ", "
                                                  + PROPERTY_NAME_NUMBER + " = -1,"
                                                  + PROPERTY_NAME_PEAK_ID + " = -1";

        /// <summary>
        /// Chart properties(ChartType)
        /// Point
        /// </summary>
        private const SeriesChartType CHART_TYPE = SeriesChartType.Point;
        
        /// <summary>
        /// Chart properties(MarkerStyle)
        /// Circle
        /// </summary>
        private const MarkerStyle MARKER_STYLE = MarkerStyle.Circle;

        /// <summary>
        /// Chart properties(MarkerSize)
        /// 7
        /// </summary>
        private const int MARKER_SIZE = 7; 

        /// <summary>
        /// Chart properties(SelectedMarkerBorderColor)
        /// Black
        /// </summary>
        private readonly Color SELECTED_MARKER_BORDER_COLOR = Color.Black; 

        /// <summary>
        /// dummy minimum data
        /// </summary>
        private readonly PointF _dummyDataMinPos = new PointF(-10.0F, -10.0F);

        /// <summary>
        /// dummy maximum data
        /// </summary>
        private readonly PointF _dummyDataMaxPos = new PointF(10.0F, 10.0F);

        #endregion

        #region --- Prototypes -----------------------------------------
        /// <summary>
        /// Gets or sets graph type
        /// </summary>
        public SimcaData.SIMCA_DATA_TYPE DataType { get; set; }

        #endregion

        #region --- Variables ------------------------------------------
        /// <summary>
        /// Flag(during zoom area selection)
        /// </summary>
        private bool _zoomAreaSelect = false;
        
        /// <summary>
        /// Flag(during scrolling)
        /// </summary>
        private bool _scroll = false;
        
        /// <summary>
        /// Flag(during set area selection)
        /// </summary>
        private bool _pointAreaSelect = false;
        
        /// <summary>
        /// Area selection starting position (X)
        /// </summary>
        private double _moveStartX = 0.0;
        
        /// <summary>
        /// Area selection starting position (Y)
        /// </summary>
        private double _moveStartY = 0.0;
        
        /// <summary>
        /// Area selection starting position (zoom)
        /// </summary>
        private Point _areaStartPos = new Point();
        
        /// <summary>
        /// Zoom history data
        /// </summary>
        private SimcaGraphZoomHistory _graphZoomHistory = new SimcaGraphZoomHistory();
        
        /// <summary>
        /// Data count
        /// </summary>
        private int _dataCount = 0;
         
        #endregion

        #region --- Events ---------------------------------------------
        /// <summary>
        /// Event(Select axis button click)
        /// </summary>
        public event EventHandler SelectAxisBtnClick;
  
        /// <summary>
        /// Event(Change mouse position)
        /// </summary>
        public event Action<object, MousePositionEventArgs> ChangeMousePos;

        /// <summary>
        /// Change mouse position event args class
        /// </summary>
        internal class MousePositionEventArgs : EventArgs
        {
            /// <summary>
            /// Gets or sets data type
            /// </summary>
            public SimcaData.SIMCA_DATA_TYPE Type { get; set; }
            
            /// <summary>
            /// Gets or sets a value indicating whether inside of chartArea flag
            /// </summary>
            public bool InsideOfChartArea { get; set; }
            
            /// <summary>
            /// Gets or sets X axis position
            /// </summary>
            public double ValueX { get; set; }
            
            /// <summary>
            /// Gets or sets Y axis position
            /// </summary>
            public double ValueY { get; set; }
            
            /// <summary>
            /// Gets or sets format(X axis)
            /// </summary>
            public string LabelFormatX { get; set; }
            
            /// <summary>
            /// Gets or sets format(Y axis)
            /// </summary>
            public string LabelFormatY { get; set; }

            /// <summary>
            /// Initializes a new instance of the MousePositionEventArgs class
            /// </summary>
            public MousePositionEventArgs()
            {
                this.Type = SimcaData.SIMCA_DATA_TYPE.SCORE_PLOT;
                this.InsideOfChartArea = false;
                this.ValueX = -1.0;
                this.ValueY = -1.0;
                this.LabelFormatX = string.Empty;
                this.LabelFormatY = string.Empty;
            }
        }

        /// <summary>
        /// Event(Change select item)
        /// </summary>
        public event Action<object, SelectItemEventArgs> ChangeSelectItem;
        
        /// <summary>
        /// Change selected item event args class
        /// </summary>
        internal class SelectItemEventArgs : EventArgs
        {
            /// <summary>
            /// Gets or sets data type
            /// </summary>
            public SimcaData.SIMCA_DATA_TYPE Type { get; set; }
            
            /// <summary>
            /// Gets or sets a value indicating whether double click flag
            /// </summary>
            public bool DoubleClick { get; set; }
            
            /// <summary>
            /// Gets or sets selected item No. array
            /// </summary>
            public int[] SelectItemArray { get; set; }

            /// <summary>
            /// Initializes a new instance of the SelectItemEventArgs class
            /// </summary>
            public SelectItemEventArgs()
            {
                this.Type = SimcaData.SIMCA_DATA_TYPE.SCORE_PLOT;
                this.DoubleClick = false;
                this.SelectItemArray = null;
            }
        }
        #endregion

        #region --- Construction ---------------------------------------
        /// <summary>
        /// Initializes a new instance of the UserControlSimcaGraph class
        /// </summary>
        public UserControlSimcaGraph()
        {
            InitializeComponent();

            this.DataType = SimcaData.SIMCA_DATA_TYPE.SCORE_PLOT;
            _graphZoomHistory.Clear();
            SetDefaultData();
        }

        #endregion

        #region --- Public Methods -------------------------------------
        /// <summary>
        /// Set default data
        /// </summary>
        public void SetDefaultData()
        {
            // Clear
            _chart.Series.Clear();   // Clear Series
            _chart.Legends.Clear();  // Clear Legends
            _dataCount = 0;

            // Set Graph Type
            _chart.Series.Add("dummy");  // Add Dummy Series
            Series dummySeries = _chart.Series["dummy"];
            dummySeries.ChartType = SeriesChartType.Point;    // ChartType : Point
            
            // Set dummy data (hide)
            dummySeries.Points.AddXY(_dummyDataMinPos.X, _dummyDataMinPos.Y);
            dummySeries.Points.AddXY(_dummyDataMaxPos.X, _dummyDataMaxPos.Y);
            dummySeries.Points[0].CustomProperties = INIT_CUSTOM_PROPERTY;
            dummySeries.Points[1].CustomProperties = INIT_CUSTOM_PROPERTY;
            dummySeries.Points[0].IsEmpty = true;
            dummySeries.Points[1].IsEmpty = true;

            // Update graph range
            UpdateGraphGridInterval(
                _dummyDataMinPos.X,
                _dummyDataMinPos.Y,
                _dummyDataMaxPos.X,
                _dummyDataMaxPos.Y,
                true);
            _chart.ChartAreas[0].AxisX.Title = string.Empty;
            _chart.ChartAreas[0].AxisY.Title = string.Empty;

            // Select axis button disable
            _buttonSelAxis.Enabled = false;

            ChangeSelect(false);
        }

        /// <summary>
        /// Set data
        /// </summary>
        /// <param name="data">Plot data</param>
        public void SetData(SimcaData data)
        {
            // Clear
            _chart.Series.Clear();   // Clear Series
            _chart.Legends.Clear();  // Clear Legends

            // Set Series / Legend
            string seriesName = string.Empty;
            switch (this.DataType)
            {
                case SimcaData.SIMCA_DATA_TYPE.SCORE_PLOT:
                    seriesName = "(none)";
                    _chart.Legends.Add("Score");
                    _chart.Legends["Score"].BorderColor = Color.Black;
                    break;
                case SimcaData.SIMCA_DATA_TYPE.LOADING_PLOT:
                    seriesName = "Loading";
                    _chart.Series.Add(seriesName);
                    break;
                case SimcaData.SIMCA_DATA_TYPE.S_PLOT:
                    seriesName = "SPlot";
                    _chart.Series.Add(seriesName);
                    break;
                default:
                    break;
            }

            // It cannot be.  (Just to make sure)
            if (seriesName == string.Empty)
            {
                return;
            }

            // Gets and check data count
            _dataCount = data.GetDataCount();
            if (_dataCount <= 0)
            {   // There is no data. 
                SetDefaultData();
                return;
            }

            // The maximum and the minimum are held for display range determination. 
            double minX = double.MaxValue;  // Minimum value (axis X)
            double minY = double.MaxValue;  // Minimum value (axis Y)
            double maxX = double.MinValue;  // Maximum value (axis X)
            double maxY = double.MinValue;  // Maximum value (axis Y)

            // Series relations
            string preSeriesName = string.Empty; // The last series name
            bool seriesExist = false;   // Existence of series
            
            // Add point
            for (int index = 1; index < _dataCount + 1; index++)
            {
                // Make series
                if (this.DataType == SimcaData.SIMCA_DATA_TYPE.SCORE_PLOT)
                {
                    seriesExist = MakeScorePlotSeries(index, preSeriesName, data, out seriesName);
                    preSeriesName = seriesName;
                }

                // Set series property
                Series workSeries = _chart.Series[seriesName];
                if (!seriesExist)
                {
                    workSeries.ChartType = CHART_TYPE;
                    workSeries.MarkerStyle = MARKER_STYLE;
                    workSeries.MarkerSize = MARKER_SIZE;
                    workSeries.MarkerColor = data.GetGroupColor(index);
                    workSeries.MarkerBorderColor = data.GetGroupColor(index);
                }

                // Set max/min value
                double valueX;
                double valueY;
                data.TryGetXData(index, out valueX);
                data.TryGetYData(index, out valueY);
                minX = Math.Min(minX, valueX);
                minY = Math.Min(minY, valueY);
                maxX = Math.Max(maxX, valueX);
                maxY = Math.Max(maxY, valueY);

                // Add point
                workSeries.Points.AddXY(valueX, valueY);

                // Set tool tip
                string work = string.Empty;
                if (this.DataType == SimcaData.SIMCA_DATA_TYPE.SCORE_PLOT)
                {
                    work = seriesName;
                }
                work += string.Format("({0:G3},{1:G3})", valueX, valueY);
                workSeries.Points.Last().ToolTip = work;

                // Set custom properties
                workSeries.Points.Last().CustomProperties = INIT_CUSTOM_PROPERTY;
                workSeries.Points.Last().SetCustomProperty(
                    PROPERTY_NAME_NUMBER,
                    index.ToString());
                workSeries.Points.Last().SetCustomProperty(
                    PROPERTY_NAME_PEAK_ID,
                    data.GetPeakId(index).ToString());
            }

            // In one point, in order that there may be no range,
            // a range setup does not function well.
            // ±5% is set up as a range. 
            if (_dataCount == 1)
            {
                minX = minX - Math.Abs(minX * 0.05);
                maxX = maxX + Math.Abs(maxX * 0.05);
                minY = minY - Math.Abs(minY * 0.05);
                maxY = maxY + Math.Abs(maxY * 0.05);
            }

            // Update graph range
            UpdateGraphGridInterval(minX, minY, maxX, maxY, true);
            _chart.ChartAreas[0].AxisX.Title = data.GetAxisXName();
            _chart.ChartAreas[0].AxisY.Title = data.GetAxisYName();

            // Select axis button disable
            _buttonSelAxis.Enabled = true;

            // Change of a selective points is notified.
            ChangeSelect(false);
        }

        /// <summary>
        /// Request change select event
        /// </summary>
        public void RequestChangeSelectEvent()
        {
            ChangeSelect(false);
        }
        
        #endregion

        #region --- Private Events -------------------------------------
        /// <summary>
        /// Chart MouseMove event handler
        /// Function1:The notice of a mouse position
        /// Function2:Scroll
        /// Function3:Drawing of the selection range(Expansion, Point selection)
        /// </summary>
        /// <param name="sender">object</param>
        /// <param name="e">MouseEventArgs</param>
        private void Chart_MouseMove(object sender, MouseEventArgs e)
        {
            // Get mouse position(Graph coordinates)
            double x;
            double y;

            try
            {
                x = _chart.ChartAreas[0].AxisX.PixelPositionToValue(e.X);
                y = _chart.ChartAreas[0].AxisY.PixelPositionToValue(e.Y);
            }
            catch (ArgumentException)
            {
                return;
            }
            catch (InvalidOperationException)
            {
                return;
            }

            // Event notification(Function1)
            MousePositionEventArgs args = new MousePositionEventArgs();
            args.Type = this.DataType;
            args.InsideOfChartArea = true;
            args.ValueX = x;
            args.ValueY = y;
            args.LabelFormatX = "{0:" + _chart.ChartAreas[0].AxisX.LabelStyle.Format + "00}";
            args.LabelFormatY = "{0:" + _chart.ChartAreas[0].AxisY.LabelStyle.Format + "00}";
            ChangeMousePos(this, args);

            // Scroll(Function2)
            if (_scroll)
            {
                // Axis X
                ScrollAxisParam(
                    _chart.ChartAreas[0].AxisX,
                    _moveStartX,
                    x);

                // Axis Y
                ScrollAxisParam(
                    _chart.ChartAreas[0].AxisY,
                    _moveStartY,
                    y);
            }

            // Drawing of the selection range(Function3)
            if (_zoomAreaSelect || _pointAreaSelect)
            {
                double minX = Math.Min(_areaStartPos.X, e.X);
                double maxX = Math.Max(_areaStartPos.X, e.X);
                double minY = Math.Min(_areaStartPos.Y, e.Y);
                double maxY = Math.Max(_areaStartPos.Y, e.Y);

                Rectangle rectangle = _chart.ClientRectangle;
                _chart.Annotations[0].X = minX / rectangle.Width * 100.0;
                _chart.Annotations[0].Y = minY / rectangle.Height * 100.0;
                _chart.Annotations[0].Width = (maxX - minX) / rectangle.Width * 100.0;
                _chart.Annotations[0].Height = (maxY - minY) / rectangle.Height * 100.0; 
            }
        }

        /// <summary>
        /// Chart MouseDown event handler
        /// Function1:The start of range selection(Expansion)
        /// Function2:Scroll start
        /// Function3:The start of range selection(Point selection)
        /// </summary>
        /// <param name="sender">object</param>
        /// <param name="e">MouseEventArgs</param>
        private void Chart_MouseDown(object sender, MouseEventArgs e)
        {
            Keys key = new Keys();
            key = Control.ModifierKeys & Keys.Shift;

            // ! Left button
            if (e.Button != MouseButtons.Left)
            {
                return;
            }

            InitMouseFunction();
            if (_radioButtonScaling.Checked)
            {
                // Check Shift key
                if (key == Keys.Shift)
                {   // Function2
                    _scroll = true;
                }
                else
                {   // Function1
                    _zoomAreaSelect = true;
                }
            }
            else
            {
                // Check Shift key
                if (key == Keys.Shift)
                {   // Function3
                    _pointAreaSelect = true;
                }
            }

            // Preservation of a starting position
            if (_scroll || _zoomAreaSelect || _pointAreaSelect)
            {
                _moveStartX = _chart.ChartAreas[0].AxisX.PixelPositionToValue(e.X);
                _moveStartY = _chart.ChartAreas[0].AxisY.PixelPositionToValue(e.Y);
            }

            // The start of a range display
            if (_zoomAreaSelect || _pointAreaSelect)
            {
                _areaStartPos.X = e.X;
                _areaStartPos.Y = e.Y;
                Rectangle rectangle = _chart.ClientRectangle;
                _chart.Annotations[0].X = (double)e.X / (double)rectangle.Width * 100.0;
                _chart.Annotations[0].Y = (double)e.Y / (double)rectangle.Height * 100.0;
                _chart.Annotations[0].Width = 0;
                _chart.Annotations[0].Height = 0;
                _chart.Annotations[0].Visible = true;
            }
        }
        
        /// <summary>
        /// Chart MouseUp event handler
        /// Function1:The end of range selection(Expansion)
        /// Function2:The end of range selection(Point selection)
        /// </summary>
        /// <param name="sender">object</param>
        /// <param name="e">MouseEventArgs</param>
        private void Chart_MouseUp(object sender, MouseEventArgs e)
        {
            // Get mouse position(Graph coordinates)
            double x;
            double y;

            try
            {
                x = _chart.ChartAreas[0].AxisX.PixelPositionToValue(e.X);
                y = _chart.ChartAreas[0].AxisY.PixelPositionToValue(e.Y);
            }
            catch (ArgumentException)
            {
                InitMouseFunction();
                return;
            }
            catch (InvalidOperationException)
            {
                InitMouseFunction();
                return;
            }

            double minX = Math.Min(_moveStartX, x);
            double maxX = Math.Max(_moveStartX, x);
            double minY = Math.Min(_moveStartY, y);
            double maxY = Math.Max(_moveStartY, y);

            if (_radioButtonScaling.Checked)
            {
                if (_zoomAreaSelect)
                {
                    // Expansion(Function1)
                    if (((maxX - minX) <= 0.001) || ((maxY - minY) <= 0.001))
                    {
                        InitMouseFunction();
                        return;
                    }

                    SimcaGraphZoomHistoryItem histItem = new SimcaGraphZoomHistoryItem();
                    histItem.MinX = _chart.ChartAreas[0].AxisX.Minimum;
                    histItem.MaxX = _chart.ChartAreas[0].AxisX.Maximum;
                    histItem.MinY = _chart.ChartAreas[0].AxisY.Minimum;
                    histItem.MaxY = _chart.ChartAreas[0].AxisY.Maximum;
                    _graphZoomHistory.PushItem(histItem);
                    UpdateGraphGridInterval(minX, minY, maxX, maxY, false);
                }
            }
            else
            {
                if (_pointAreaSelect)
                {
                    // Point selection(Function2)
                    foreach (Series item in _chart.Series)
                    {
                        foreach (DataPoint point in item.Points)
                        {
                            // Range outside is processing exclusion.
                            if ((minX > point.XValue)
                             || (maxX < point.XValue)
                             || (minY > point.YValues[0])
                             || (maxY < point.YValues[0]))
                            {
                                continue;
                            }

                            // Set "Selected"
                            point.SetCustomProperty(PROPERTY_NAME_SELECTED, SELECTED);
                            point.MarkerBorderColor = SELECTED_MARKER_BORDER_COLOR;
                        }
                    }

                    // Change of a selective points is notified.
                    ChangeSelect(false);
                }
            }

            InitMouseFunction();
        }

        /// <summary>
        /// Chart MouseLeave event handler
        /// Function1:The notice of a mouse position
        /// </summary>
        /// <param name="sender">object</param>
        /// <param name="e">MouseEventArgs</param>
        private void Chart_MouseLeave(object sender, EventArgs e)
        {
            InitMouseFunction();

            // The notice of a mouse position(Function1)
            MousePositionEventArgs eventArgs = new MousePositionEventArgs();
            eventArgs.Type = this.DataType;
            eventArgs.InsideOfChartArea = false;
            ChangeMousePos(this, eventArgs);
        }
        
        /// <summary>
        /// Chart MouseClick event handler
        /// Function1:Expansion is returned before one
        /// Function2:1 point select / all delete
        /// Function3:1 point add select / 1 point delete
        /// </summary>
        /// <param name="sender">object</param>
        /// <param name="e">MouseEventArgs</param>
        private void Chart_MouseClick(object sender, MouseEventArgs e)
        {
            if (_radioButtonScaling.Checked)
            {
                if (e.Button == MouseButtons.Right)
                {   // The right-click of a mouse
                    // Expansion is returned before one(Function1)
                    SimcaGraphZoomHistoryItem histItem = new SimcaGraphZoomHistoryItem();
                    if (_graphZoomHistory.PopItem(out histItem))
                    {   // It succeeds in the position data acquisition in front of one.
                        UpdateGraphGridInterval(histItem.MinX, histItem.MinY, histItem.MaxX, histItem.MaxY, false);

                        //double center = (_chart.ChartAreas[0].AxisX.Maximum - _chart.ChartAreas[0].AxisX.Minimum) / 2.0;
                        //center += _chart.ChartAreas[0].AxisX.Minimum;
                        //double width = histItem.MaxX - histItem.MinX;
                        //double newLeft = center - (width / 2.0);

                        //center = (_chart.ChartAreas[0].AxisY.Maximum - _chart.ChartAreas[0].AxisY.Minimum) / 2.0;
                        //center += _chart.ChartAreas[0].AxisY.Minimum;
                        //double height = histItem.MaxY - histItem.MinY;
                        //double newBottom = center - (height / 2.0);

                        //UpdateGraphGridInterval(newLeft, newBottom, newLeft + width, newBottom + height, false);
                    }
                }
            }
            else
            {
                Keys key = new Keys();
                key = Control.ModifierKeys & (Keys.Control | Keys.Shift);

                if (e.Button == MouseButtons.Left)
                {
                    // Select Points-1 1point select / all delete(Function2)
                    if (key == Keys.None)
                    {
                        // 1 point select / all delete(Function2)
                        // A selective state is initialized. 
                        foreach (Series item in _chart.Series)
                        {
                            foreach (DataPoint point in item.Points)
                            {
                                point.SetCustomProperty(PROPERTY_NAME_SELECTED, NOT_SELECTED);
                                point.MarkerBorderColor = point.MarkerColor;
                            }
                        }

                        // It set "Selected", if it is on a point. 
                        HitTestResult hitResult;
                        hitResult = _chart.HitTest(((MouseEventArgs)e).X, ((MouseEventArgs)e).Y, ChartElementType.DataPoint);

                        if (hitResult.ChartElementType == ChartElementType.DataPoint)
                        {
                            int index = hitResult.PointIndex;
                            DataPoint data = hitResult.Series.Points[index];
                            hitResult.Series.Points[index].SetCustomProperty(PROPERTY_NAME_SELECTED, SELECTED);
                            hitResult.Series.Points[index].MarkerBorderColor = SELECTED_MARKER_BORDER_COLOR;
                        }

                        ChangeSelect(false);
                    }

                    if (key == Keys.Control)
                    {
                        // 1 point add select / 1 point delete(Function3)
                        HitTestResult hitResult;
                        hitResult = _chart.HitTest(((MouseEventArgs)e).X, ((MouseEventArgs)e).Y, ChartElementType.DataPoint);

                        if (hitResult.ChartElementType == ChartElementType.DataPoint)
                        {
                            int index = hitResult.PointIndex;
                            if (hitResult.Series.Points[index].GetCustomProperty(PROPERTY_NAME_SELECTED) == SELECTED)
                            {
                                hitResult.Series.Points[index].SetCustomProperty(PROPERTY_NAME_SELECTED, NOT_SELECTED);
                                hitResult.Series.Points[index].MarkerBorderColor
                                    = hitResult.Series.Points[index].MarkerColor;
                            }
                            else
                            {
                                hitResult.Series.Points[index].SetCustomProperty(PROPERTY_NAME_SELECTED, SELECTED);
                                hitResult.Series.Points[index].MarkerBorderColor = SELECTED_MARKER_BORDER_COLOR;
                            }

                            ChangeSelect(false);
                        }
                    }
                }
            }
        }

        /// <summary>
        /// Chart MouseDoubleClick event handler
        /// </summary>
        /// <param name="sender">Sender</param>
        /// <param name="e">Event args</param>
        private void Chart_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            _scroll = false;
            _zoomAreaSelect = false;
            _pointAreaSelect = false;
            _chart.Annotations[0].Visible = false;

            Keys key = new Keys();
            key = Control.ModifierKeys & (Keys.Control | Keys.Shift);

            if (!_radioButtonScaling.Checked)
            {
                if (key != Keys.None)
                {   // shift or control key
                    return;
                }

                if (e.Button == MouseButtons.Left)
                {   // Left button double click
                    // A selective state is initialized. 
                    foreach (Series item in _chart.Series)
                    {
                        foreach (DataPoint point in item.Points)
                        {
                            point.SetCustomProperty(PROPERTY_NAME_SELECTED, NOT_SELECTED);
                            point.MarkerBorderColor = point.MarkerColor;
                        }
                    }

                    // It set "Selected", if it is on a point. 
                    HitTestResult hitResult;
                    hitResult = _chart.HitTest(((MouseEventArgs)e).X, ((MouseEventArgs)e).Y, ChartElementType.DataPoint);

                    if (hitResult.ChartElementType == ChartElementType.DataPoint)
                    {
                        int index = hitResult.PointIndex;
                        hitResult.Series.Points[index].SetCustomProperty(PROPERTY_NAME_SELECTED, SELECTED);
                        hitResult.Series.Points[index].MarkerBorderColor = SELECTED_MARKER_BORDER_COLOR;
                    }

                    ChangeSelect(true);
                }
            }
        }

        /// <summary>
        /// Select axis button Click  event handler
        /// </summary>
        /// <param name="sender">Sender</param>
        /// <param name="e">Event args</param>
        private void ButtonSelAxis_Click(object sender, EventArgs e)
        {
            if (this.SelectAxisBtnClick != null)
            {
                SelectAxisBtnClick(sender, e);
            }
        }

        #endregion

        #region --- Private Methods ------------------------------------
        /// <summary>
        /// Update graph range
        /// </summary>
        /// <param name="minX">minimum X value</param>
        /// <param name="minY">minimum Y value</param>
        /// <param name="maxX">maximum X value(minX < minY)</param>
        /// <param name="maxY">maximum Y value(minY < maxY)</param>
        /// <param name="init">Initialize Flag</param>
        private void UpdateGraphGridInterval(double minX, double minY, double maxX, double maxY, bool init)
        {
            double gridCntX;
            double gridCntY;

            if (init)
            {   // Initialize
                gridCntX = GRID_COUNT_X;
                gridCntY = GRID_COUNT_Y;
            }
            else
            {   // Update(Zoom)
                gridCntX = GRID_COUNT_X + 1;
                gridCntY = GRID_COUNT_Y + 1;
            }

            // Grid interval
            double[] gridInt = 
            {
                ((maxX - minX) / gridCntX),
                ((maxY - minY) / gridCntY)
            };

            // Minimum Value
            double[] minValue = 
            {
                minX,
                minY
            };

            // Maximum Value
            double[] maxValue = 
            {
                maxX,
                maxY
            };

            // Axis
            Axis[] graphAxis = 
            {
                _chart.ChartAreas[0].AxisX,
                _chart.ChartAreas[0].AxisY
            };

            // X, Y Loop
            int loopCnt = 0;
            for (loopCnt = 0; loopCnt < 2; loopCnt++)
            {
                // Difference is changed into the form of "#.###" 
                int gain = 0;
                while (gridInt[loopCnt] > 10.0)
                {
                    gain++;
                    gridInt[loopCnt] /= 10.0;
                }

                while (gridInt[loopCnt] < 1.0)
                {
                    gain--;
                    gridInt[loopCnt] *= 10.0;
                }

                // Set major grid interval
                graphAxis[loopCnt].MajorGrid.Interval = Math.Round(gridInt[loopCnt]) * Math.Pow(10.0, gain);

                // Set Max / Min value
                double leftbottomValue;
                double rightTopValue;
                if (init)
                {   // Initialize
                    leftbottomValue
                        = ((int)(minValue[loopCnt] / graphAxis[loopCnt].MajorGrid.Interval) - 1)
                        * Math.Round(gridInt[loopCnt]) * Math.Pow(10.0, gain);
                    rightTopValue
                        = ((int)(maxValue[loopCnt] / graphAxis[loopCnt].MajorGrid.Interval) + 1)
                        * Math.Round(gridInt[loopCnt]) * Math.Pow(10.0, gain);
                }
                else
                {   // Update(Zoom)
                    leftbottomValue = minValue[loopCnt];
                    rightTopValue = maxValue[loopCnt];
                }

                graphAxis[loopCnt].IntervalAutoMode = IntervalAutoMode.FixedCount;
                graphAxis[loopCnt].Minimum = leftbottomValue;
                graphAxis[loopCnt].Maximum = rightTopValue;

                // Set label format
                string labelFormat = "0.0";
                if (gain < 0)
                {
                    gain *= -1;
                    for (int loopGain = 0; loopGain < gain; loopGain++)
                    {
                        labelFormat += "0";
                    }

                    graphAxis[loopCnt].LabelStyle.Format = labelFormat;
                }
                else
                {
                    graphAxis[loopCnt].LabelStyle.Format = "0.";
                }

                double majorGridInterval = graphAxis[loopCnt].MajorGrid.Interval;
                int intCount = (int)(graphAxis[loopCnt].Minimum / majorGridInterval);
                double startVal = intCount * majorGridInterval;
                double workOffset = majorGridInterval + (graphAxis[loopCnt].Minimum - startVal);
                workOffset *= -1.0;
                graphAxis[loopCnt].MajorGrid.IntervalOffset = workOffset;
                graphAxis[loopCnt].MajorTickMark.Interval = majorGridInterval;
                graphAxis[loopCnt].MajorTickMark.IntervalOffset = workOffset;
                graphAxis[loopCnt].Interval = majorGridInterval;
                graphAxis[loopCnt].IntervalOffset = workOffset;
            }
        }

        /// <summary>
        /// Change select
        /// </summary>
        /// <param name="dblClk">T = Double click</param>
        private void ChangeSelect(bool dblClk)
        {
            if (this.ChangeSelectItem == null)
            {
                return;
            }

            SelectItemEventArgs eventArgs = new SelectItemEventArgs();
            eventArgs.Type = this.DataType;
            eventArgs.DoubleClick = dblClk;

            List<int> selItem = new List<int>();
            selItem.Clear();

            if (this.DataType == SimcaData.SIMCA_DATA_TYPE.SCORE_PLOT)
            {
                eventArgs.DoubleClick = false;
            }
            else
            {
                foreach (Series item in _chart.Series)
                {
                    foreach (DataPoint point in item.Points)
                    {
                        if (point.GetCustomProperty(PROPERTY_NAME_SELECTED) == SELECTED)
                        {
                            int peakId = int.Parse(point.GetCustomProperty(PROPERTY_NAME_PEAK_ID));
                            if (peakId != -1)
                            {
                                selItem.Add(int.Parse(point.GetCustomProperty(PROPERTY_NAME_PEAK_ID)));
                            }
                        }
                    }
                }
            }

            if (selItem.Count() != 0)
            {
                eventArgs.SelectItemArray = selItem.ToArray();
            }

            ChangeSelectItem(this, eventArgs);
        }

        /// <summary>
        /// Make score plot series
        /// </summary>
        /// <param name="index">Data index</param>
        /// <param name="preSeriesName">The last series name</param>
        /// <param name="data">Plot data</param>
        /// <param name="seriesName">Series name</param>
        /// <returns>T=Existing, F=New creation</returns>
        private bool MakeScorePlotSeries(int index, string preSeriesName, SimcaData data, out string seriesName)
        {
            bool seriesExist = false;
            string group = data.GetGroupName(index);
            if (group != string.Empty)
            {
                seriesName = group;
            }
            else
            {
                seriesName = "(none)";
            }

            try
            {
                if (!preSeriesName.Equals(seriesName))
                {
                    preSeriesName = seriesName;
                    _chart.Series.Add(seriesName);
                }
            }
            catch (Exception)
            {
                seriesExist = true;
            }

            return seriesExist;
        }

        /// <summary>
        /// Axial parameter setup at the time of scrolling 
        /// </summary>
        /// <param name="graphAxis">graph axis</param>
        /// <param name="startPosition">start position</param>
        /// <param name="nowPosition">now position</param>
        private void ScrollAxisParam(Axis graphAxis, double startPosition, double nowPosition)
        {
            graphAxis.IntervalAutoMode = IntervalAutoMode.FixedCount;
            graphAxis.Minimum += startPosition - nowPosition;
            graphAxis.Maximum += startPosition - nowPosition;

            double majorGridInterval = graphAxis.MajorGrid.Interval;

            double workMin = Math.Truncate(graphAxis.Minimum / majorGridInterval) * majorGridInterval;
            double workOffset = majorGridInterval + (graphAxis.Minimum - workMin);
            workOffset *= -1.0;
            graphAxis.MajorGrid.IntervalOffset = workOffset;
            graphAxis.MajorTickMark.Interval = majorGridInterval;
            graphAxis.MajorTickMark.IntervalOffset = workOffset;
            graphAxis.Interval = majorGridInterval;
            graphAxis.IntervalOffset = workOffset;
        }

        /// <summary>
        /// Initialize mouse function
        /// </summary>
        private void InitMouseFunction()
        {
            _scroll = false;
            _zoomAreaSelect = false;
            _pointAreaSelect = false;
            _chart.Annotations[0].Visible = false;
        }

        #endregion
    }
}
