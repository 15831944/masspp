namespace ResamplingPlugin.ResamplingTools.GUI
{
    partial class ResamplingPanel
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this._grpSamplingPoints = new System.Windows.Forms.GroupBox();
            this._panelRegularInterval = new System.Windows.Forms.Panel();
            this._rbnMinimum = new System.Windows.Forms.RadioButton();
            this._rbnAverage = new System.Windows.Forms.RadioButton();
            this._rbnSpecified = new System.Windows.Forms.RadioButton();
            this._txtSpecifiedInterval = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this._txtRangeMax = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this._txtRangeMin = new System.Windows.Forms.TextBox();
            this._rbnRegular = new System.Windows.Forms.RadioButton();
            this._rbnSame = new System.Windows.Forms.RadioButton();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this._rbnPCHIP = new System.Windows.Forms.RadioButton();
            this._rbnLinear = new System.Windows.Forms.RadioButton();
            this._grpSamplingPoints.SuspendLayout();
            this._panelRegularInterval.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // _grpSamplingPoints
            // 
            this._grpSamplingPoints.Controls.Add(this._panelRegularInterval);
            this._grpSamplingPoints.Controls.Add(this.label2);
            this._grpSamplingPoints.Controls.Add(this._txtRangeMax);
            this._grpSamplingPoints.Controls.Add(this.label1);
            this._grpSamplingPoints.Controls.Add(this._txtRangeMin);
            this._grpSamplingPoints.Controls.Add(this._rbnRegular);
            this._grpSamplingPoints.Controls.Add(this._rbnSame);
            this._grpSamplingPoints.Location = new System.Drawing.Point(13, 13);
            this._grpSamplingPoints.Name = "_grpSamplingPoints";
            this._grpSamplingPoints.Size = new System.Drawing.Size(357, 223);
            this._grpSamplingPoints.TabIndex = 0;
            this._grpSamplingPoints.TabStop = false;
            this._grpSamplingPoints.Text = "Sampling Points";
            // 
            // _panelRegularInterval
            // 
            this._panelRegularInterval.Controls.Add(this._rbnMinimum);
            this._panelRegularInterval.Controls.Add(this._rbnAverage);
            this._panelRegularInterval.Controls.Add(this._rbnSpecified);
            this._panelRegularInterval.Controls.Add(this._txtSpecifiedInterval);
            this._panelRegularInterval.Location = new System.Drawing.Point(47, 77);
            this._panelRegularInterval.Name = "_panelRegularInterval";
            this._panelRegularInterval.Size = new System.Drawing.Size(227, 86);
            this._panelRegularInterval.TabIndex = 2;
            // 
            // _rbnMinimum
            // 
            this._rbnMinimum.AutoSize = true;
            this._rbnMinimum.Location = new System.Drawing.Point(3, 26);
            this._rbnMinimum.Name = "_rbnMinimum";
            this._rbnMinimum.Size = new System.Drawing.Size(212, 16);
            this._rbnMinimum.TabIndex = 3;
            this._rbnMinimum.TabStop = true;
            this._rbnMinimum.Text = "Minimum interval in selected spectra";
            this._rbnMinimum.UseVisualStyleBackColor = true;
            // 
            // _rbnAverage
            // 
            this._rbnAverage.AutoSize = true;
            this._rbnAverage.Location = new System.Drawing.Point(3, 4);
            this._rbnAverage.Name = "_rbnAverage";
            this._rbnAverage.Size = new System.Drawing.Size(209, 16);
            this._rbnAverage.TabIndex = 2;
            this._rbnAverage.TabStop = true;
            this._rbnAverage.Text = "Average interval in selected spectra";
            this._rbnAverage.UseVisualStyleBackColor = true;
            // 
            // _rbnSpecified
            // 
            this._rbnSpecified.AutoSize = true;
            this._rbnSpecified.Location = new System.Drawing.Point(3, 48);
            this._rbnSpecified.Name = "_rbnSpecified";
            this._rbnSpecified.Size = new System.Drawing.Size(138, 16);
            this._rbnSpecified.TabIndex = 4;
            this._rbnSpecified.TabStop = true;
            this._rbnSpecified.Text = "Specified interval (Da)";
            this._rbnSpecified.UseVisualStyleBackColor = true;
            // 
            // _txtSpecifiedInterval
            // 
            this._txtSpecifiedInterval.Location = new System.Drawing.Point(142, 47);
            this._txtSpecifiedInterval.Name = "_txtSpecifiedInterval";
            this._txtSpecifiedInterval.Size = new System.Drawing.Size(59, 19);
            this._txtSpecifiedInterval.TabIndex = 5;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(202, 190);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(17, 12);
            this.label2.TabIndex = 9;
            this.label2.Text = "－";
            // 
            // _txtRangeMax
            // 
            this._txtRangeMax.Location = new System.Drawing.Point(225, 187);
            this._txtRangeMax.Name = "_txtRangeMax";
            this._txtRangeMax.Size = new System.Drawing.Size(65, 19);
            this._txtRangeMax.TabIndex = 8;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(17, 190);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(97, 12);
            this.label1.TabIndex = 7;
            this.label1.Text = "Output range (Da)";
            // 
            // _txtRangeMin
            // 
            this._txtRangeMin.Location = new System.Drawing.Point(129, 187);
            this._txtRangeMin.Name = "_txtRangeMin";
            this._txtRangeMin.Size = new System.Drawing.Size(65, 19);
            this._txtRangeMin.TabIndex = 6;
            // 
            // _rbnRegular
            // 
            this._rbnRegular.AutoSize = true;
            this._rbnRegular.Location = new System.Drawing.Point(17, 55);
            this._rbnRegular.Name = "_rbnRegular";
            this._rbnRegular.Size = new System.Drawing.Size(104, 16);
            this._rbnRegular.TabIndex = 1;
            this._rbnRegular.TabStop = true;
            this._rbnRegular.Text = "Regular interval";
            this._rbnRegular.UseVisualStyleBackColor = true;
            // 
            // _rbnSame
            // 
            this._rbnSame.AutoSize = true;
            this._rbnSame.Location = new System.Drawing.Point(17, 28);
            this._rbnSame.Name = "_rbnSame";
            this._rbnSame.Size = new System.Drawing.Size(143, 16);
            this._rbnSame.TabIndex = 0;
            this._rbnSame.TabStop = true;
            this._rbnSame.Text = "Same as first spectrum";
            this._rbnSame.UseVisualStyleBackColor = true;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this._rbnPCHIP);
            this.groupBox2.Controls.Add(this._rbnLinear);
            this.groupBox2.Location = new System.Drawing.Point(379, 13);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(240, 71);
            this.groupBox2.TabIndex = 1;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Interpolation";
            // 
            // _rbnPCHIP
            // 
            this._rbnPCHIP.AutoSize = true;
            this._rbnPCHIP.Location = new System.Drawing.Point(22, 41);
            this._rbnPCHIP.Name = "_rbnPCHIP";
            this._rbnPCHIP.Size = new System.Drawing.Size(56, 16);
            this._rbnPCHIP.TabIndex = 1;
            this._rbnPCHIP.TabStop = true;
            this._rbnPCHIP.Text = "PCHIP";
            this._rbnPCHIP.UseVisualStyleBackColor = true;
            // 
            // _rbnLinear
            // 
            this._rbnLinear.AutoSize = true;
            this._rbnLinear.Location = new System.Drawing.Point(22, 18);
            this._rbnLinear.Name = "_rbnLinear";
            this._rbnLinear.Size = new System.Drawing.Size(54, 16);
            this._rbnLinear.TabIndex = 0;
            this._rbnLinear.TabStop = true;
            this._rbnLinear.Text = "Linear";
            this._rbnLinear.UseVisualStyleBackColor = true;
            // 
            // ResamplingPanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(632, 244);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this._grpSamplingPoints);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "ResamplingPanel";
            this.Text = "ResamplingPanel";
            this.Load += new System.EventHandler(this.ResamplingPanel_Load);
            this._grpSamplingPoints.ResumeLayout(false);
            this._grpSamplingPoints.PerformLayout();
            this._panelRegularInterval.ResumeLayout(false);
            this._panelRegularInterval.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox _grpSamplingPoints;
        private System.Windows.Forms.TextBox _txtSpecifiedInterval;
        private System.Windows.Forms.RadioButton _rbnSpecified;
        private System.Windows.Forms.RadioButton _rbnMinimum;
        private System.Windows.Forms.RadioButton _rbnAverage;
        private System.Windows.Forms.RadioButton _rbnRegular;
        private System.Windows.Forms.RadioButton _rbnSame;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.RadioButton _rbnPCHIP;
        private System.Windows.Forms.RadioButton _rbnLinear;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox _txtRangeMax;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox _txtRangeMin;
        private System.Windows.Forms.Panel _panelRegularInterval;
    }
}