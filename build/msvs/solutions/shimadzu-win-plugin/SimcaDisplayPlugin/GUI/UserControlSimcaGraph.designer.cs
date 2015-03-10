namespace SimcaDisplay.Gui
{
    partial class UserControlSimcaGraph
    {
        /// <summary> 
        /// 必要なデザイナー変数です。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// 使用中のリソースをすべてクリーンアップします。
        /// </summary>
        /// <param name="disposing">マネージ リソースが破棄される場合 true、破棄されない場合は false です。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region コンポーネント デザイナーで生成されたコード

        /// <summary> 
        /// デザイナー サポートに必要なメソッドです。このメソッドの内容を 
        /// コード エディターで変更しないでください。
        /// </summary>
        private void InitializeComponent()
        {
            System.Windows.Forms.DataVisualization.Charting.RectangleAnnotation rectangleAnnotation3 = new System.Windows.Forms.DataVisualization.Charting.RectangleAnnotation();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea3 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend3 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series3 = new System.Windows.Forms.DataVisualization.Charting.Series();
            this._chart = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this._radioButtonScaling = new System.Windows.Forms.RadioButton();
            this._radioButtonSelPoint = new System.Windows.Forms.RadioButton();
            this._buttonSelAxis = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this._chart)).BeginInit();
            this.SuspendLayout();
            // 
            // _chart
            // 
            this._chart.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            rectangleAnnotation3.BackColor = System.Drawing.Color.Transparent;
            rectangleAnnotation3.LineDashStyle = System.Windows.Forms.DataVisualization.Charting.ChartDashStyle.Dot;
            rectangleAnnotation3.Name = "RectangleAnnotation1";
            rectangleAnnotation3.Text = " ";
            rectangleAnnotation3.Visible = false;
            this._chart.Annotations.Add(rectangleAnnotation3);
            chartArea3.AxisX.LineColor = System.Drawing.Color.DarkGray;
            chartArea3.AxisX.MajorGrid.LineColor = System.Drawing.Color.DarkGray;
            chartArea3.AxisX.MajorTickMark.IntervalOffset = 0D;
            chartArea3.AxisX.MajorTickMark.LineColor = System.Drawing.Color.DarkGray;
            chartArea3.AxisX.Maximum = 100D;
            chartArea3.AxisX.Minimum = -100D;
            chartArea3.AxisX.MinorTickMark.IntervalType = System.Windows.Forms.DataVisualization.Charting.DateTimeIntervalType.NotSet;
            chartArea3.AxisY.LineColor = System.Drawing.Color.DarkGray;
            chartArea3.AxisY.MajorGrid.LineColor = System.Drawing.Color.DarkGray;
            chartArea3.AxisY.MajorTickMark.LineColor = System.Drawing.Color.DarkGray;
            chartArea3.AxisY.Maximum = 100D;
            chartArea3.AxisY.Minimum = -100D;
            chartArea3.AxisY.MinorGrid.LineColor = System.Drawing.Color.DarkGray;
            chartArea3.Name = "ChartArea1";
            this._chart.ChartAreas.Add(chartArea3);
            legend3.Name = "Legend1";
            this._chart.Legends.Add(legend3);
            this._chart.Location = new System.Drawing.Point(0, 0);
            this._chart.Name = "_chart";
            series3.ChartArea = "ChartArea1";
            series3.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Point;
            series3.Legend = "Legend1";
            series3.MarkerBorderColor = System.Drawing.Color.Black;
            series3.MarkerSize = 7;
            series3.MarkerStyle = System.Windows.Forms.DataVisualization.Charting.MarkerStyle.Circle;
            series3.Name = "Series1";
            this._chart.Series.Add(series3);
            this._chart.Size = new System.Drawing.Size(630, 300);
            this._chart.TabIndex = 0;
            this._chart.Text = "chart1";
            this._chart.MouseClick += new System.Windows.Forms.MouseEventHandler(this.Chart_MouseClick);
            this._chart.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.Chart_MouseDoubleClick);
            this._chart.MouseDown += new System.Windows.Forms.MouseEventHandler(this.Chart_MouseDown);
            this._chart.MouseLeave += new System.EventHandler(this.Chart_MouseLeave);
            this._chart.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Chart_MouseMove);
            this._chart.MouseUp += new System.Windows.Forms.MouseEventHandler(this.Chart_MouseUp);
            // 
            // _radioButtonScaling
            // 
            this._radioButtonScaling.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this._radioButtonScaling.Location = new System.Drawing.Point(646, 12);
            this._radioButtonScaling.Name = "_radioButtonScaling";
            this._radioButtonScaling.Size = new System.Drawing.Size(60, 16);
            this._radioButtonScaling.TabIndex = 1;
            this._radioButtonScaling.Text = "Scaling";
            this._radioButtonScaling.UseVisualStyleBackColor = true;
            // 
            // _radioButtonSelPoint
            // 
            this._radioButtonSelPoint.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this._radioButtonSelPoint.Checked = true;
            this._radioButtonSelPoint.Location = new System.Drawing.Point(646, 34);
            this._radioButtonSelPoint.Name = "_radioButtonSelPoint";
            this._radioButtonSelPoint.Size = new System.Drawing.Size(91, 16);
            this._radioButtonSelPoint.TabIndex = 2;
            this._radioButtonSelPoint.TabStop = true;
            this._radioButtonSelPoint.Text = "Select Points";
            this._radioButtonSelPoint.UseVisualStyleBackColor = true;
            // 
            // _buttonSelAxis
            // 
            this._buttonSelAxis.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this._buttonSelAxis.Enabled = false;
            this._buttonSelAxis.Location = new System.Drawing.Point(646, 100);
            this._buttonSelAxis.Name = "_buttonSelAxis";
            this._buttonSelAxis.Size = new System.Drawing.Size(91, 23);
            this._buttonSelAxis.TabIndex = 3;
            this._buttonSelAxis.Text = "Select Axis";
            this._buttonSelAxis.UseVisualStyleBackColor = true;
            this._buttonSelAxis.Click += new System.EventHandler(this.ButtonSelAxis_Click);
            // 
            // UserControlSimcaGraph
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this._buttonSelAxis);
            this.Controls.Add(this._radioButtonSelPoint);
            this.Controls.Add(this._radioButtonScaling);
            this.Controls.Add(this._chart);
            this.Name = "UserControlSimcaGraph";
            this.Size = new System.Drawing.Size(750, 300);
            ((System.ComponentModel.ISupportInitialize)(this._chart)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.DataVisualization.Charting.Chart _chart;
        private System.Windows.Forms.RadioButton _radioButtonScaling;
        private System.Windows.Forms.RadioButton _radioButtonSelPoint;
        private System.Windows.Forms.Button _buttonSelAxis;
    }
}
