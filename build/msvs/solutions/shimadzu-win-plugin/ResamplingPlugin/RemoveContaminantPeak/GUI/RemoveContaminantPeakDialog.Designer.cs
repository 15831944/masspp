namespace ResamplingPlugin.RemoveContaminantPeak.GUI
{
    partial class RemoveContaminantPeakDialog
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
            this._grpResampling = new System.Windows.Forms.GroupBox();
            this._grpBaseLine = new System.Windows.Forms.GroupBox();
            this._btnDefault = new System.Windows.Forms.Button();
            this._cmbBaseLine = new System.Windows.Forms.ComboBox();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this._textBoxHelpMessage = new System.Windows.Forms.TextBox();
            this._btnOK = new System.Windows.Forms.Button();
            this._btnCancel = new System.Windows.Forms.Button();
            this._grpBaseLine.SuspendLayout();
            this.SuspendLayout();
            // 
            // _grpResampling
            // 
            this._grpResampling.Location = new System.Drawing.Point(13, 13);
            this._grpResampling.Name = "_grpResampling";
            this._grpResampling.Size = new System.Drawing.Size(663, 267);
            this._grpResampling.TabIndex = 0;
            this._grpResampling.TabStop = false;
            this._grpResampling.Text = "Resampling";
            // 
            // _grpBaseLine
            // 
            this._grpBaseLine.Controls.Add(this._btnDefault);
            this._grpBaseLine.Controls.Add(this._cmbBaseLine);
            this._grpBaseLine.Location = new System.Drawing.Point(12, 293);
            this._grpBaseLine.Name = "_grpBaseLine";
            this._grpBaseLine.Size = new System.Drawing.Size(448, 191);
            this._grpBaseLine.TabIndex = 1;
            this._grpBaseLine.TabStop = false;
            this._grpBaseLine.Text = "Baseline Subtraction along RT";
            // 
            // _btnDefault
            // 
            this._btnDefault.Location = new System.Drawing.Point(341, 155);
            this._btnDefault.Name = "_btnDefault";
            this._btnDefault.Size = new System.Drawing.Size(75, 23);
            this._btnDefault.TabIndex = 1;
            this._btnDefault.Text = "Default";
            this._btnDefault.UseVisualStyleBackColor = true;
            this._btnDefault.Click += new System.EventHandler(this.BtnDefault_Click);
            // 
            // _cmbBaseLine
            // 
            this._cmbBaseLine.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this._cmbBaseLine.FormattingEnabled = true;
            this._cmbBaseLine.Items.AddRange(new object[] {
            "ABC Baseline",
            "Linear Baseline"});
            this._cmbBaseLine.Location = new System.Drawing.Point(29, 32);
            this._cmbBaseLine.Name = "_cmbBaseLine";
            this._cmbBaseLine.Size = new System.Drawing.Size(268, 20);
            this._cmbBaseLine.TabIndex = 0;
            this._cmbBaseLine.TextChanged += new System.EventHandler(this.CmbBaseLine_TextChanged);
            // 
            // textBox1
            // 
            this.textBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.textBox1.Location = new System.Drawing.Point(380, 493);
            this.textBox1.Multiline = true;
            this.textBox1.Name = "textBox1";
            this.textBox1.ReadOnly = true;
            this.textBox1.Size = new System.Drawing.Size(294, 122);
            this.textBox1.TabIndex = 16;
            // 
            // _textBoxHelpMessage
            // 
            this._textBoxHelpMessage.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this._textBoxHelpMessage.Location = new System.Drawing.Point(12, 493);
            this._textBoxHelpMessage.Multiline = true;
            this._textBoxHelpMessage.Name = "_textBoxHelpMessage";
            this._textBoxHelpMessage.ReadOnly = true;
            this._textBoxHelpMessage.Size = new System.Drawing.Size(362, 122);
            this._textBoxHelpMessage.TabIndex = 15;
            // 
            // _btnOK
            // 
            this._btnOK.Location = new System.Drawing.Point(466, 621);
            this._btnOK.Name = "_btnOK";
            this._btnOK.Size = new System.Drawing.Size(75, 23);
            this._btnOK.TabIndex = 5;
            this._btnOK.Text = "OK";
            this._btnOK.UseVisualStyleBackColor = true;
            this._btnOK.Click += new System.EventHandler(this.BtnOK_Click);
            // 
            // _btnCancel
            // 
            this._btnCancel.Location = new System.Drawing.Point(557, 621);
            this._btnCancel.Name = "_btnCancel";
            this._btnCancel.Size = new System.Drawing.Size(75, 23);
            this._btnCancel.TabIndex = 5;
            this._btnCancel.Text = "Cancel";
            this._btnCancel.UseVisualStyleBackColor = true;
            this._btnCancel.Click += new System.EventHandler(this.BtnCancel_Click);
            // 
            // RemoveContaminantPeakDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(687, 657);
            this.Controls.Add(this._btnCancel);
            this.Controls.Add(this._btnOK);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this._textBoxHelpMessage);
            this.Controls.Add(this._grpBaseLine);
            this.Controls.Add(this._grpResampling);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "RemoveContaminantPeakDialog";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Remove Contaminant Peaks";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.RemoveContaminantPeakDialog_FormClosed);
            this.Load += new System.EventHandler(this.RemoveContaminantPeakDialog_Load);
            this._grpBaseLine.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.GroupBox _grpResampling;
        private System.Windows.Forms.GroupBox _grpBaseLine;
        private System.Windows.Forms.ComboBox _cmbBaseLine;
        private System.Windows.Forms.Button _btnDefault;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.TextBox _textBoxHelpMessage;
        private System.Windows.Forms.Button _btnOK;
        private System.Windows.Forms.Button _btnCancel;
    }
}