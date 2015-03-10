namespace ResamplingPlugin.ResamplingTools.GUI
{
    partial class ProgressDlg
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
            this.components = new System.ComponentModel.Container();
            this._prgBar = new System.Windows.Forms.ProgressBar();
            this._lblMessage = new System.Windows.Forms.Label();
            this._btnCancel = new System.Windows.Forms.Button();
            this._AutoCountTimer = new System.Windows.Forms.Timer(this.components);
            this.SuspendLayout();
            // 
            // _prgBar
            // 
            this._prgBar.Location = new System.Drawing.Point(27, 43);
            this._prgBar.Name = "_prgBar";
            this._prgBar.Size = new System.Drawing.Size(530, 23);
            this._prgBar.TabIndex = 0;
            // 
            // _lblMessage
            // 
            this._lblMessage.Location = new System.Drawing.Point(25, 13);
            this._lblMessage.Name = "_lblMessage";
            this._lblMessage.Size = new System.Drawing.Size(532, 25);
            this._lblMessage.TabIndex = 1;
            this._lblMessage.Text = "Message";
            // 
            // _btnCancel
            // 
            this._btnCancel.Location = new System.Drawing.Point(245, 84);
            this._btnCancel.Name = "_btnCancel";
            this._btnCancel.Size = new System.Drawing.Size(90, 27);
            this._btnCancel.TabIndex = 2;
            this._btnCancel.Text = "cancel";
            this._btnCancel.UseVisualStyleBackColor = true;
            this._btnCancel.Click += new System.EventHandler(this.BtnCancel_Click);
            // 
            // _AutoCountTimer
            // 
            this._AutoCountTimer.Tick += new System.EventHandler(this.AutoCountTimer_Tick);
            // 
            // ProgressDlg
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(582, 122);
            this.ControlBox = false;
            this.Controls.Add(this._btnCancel);
            this.Controls.Add(this._lblMessage);
            this.Controls.Add(this._prgBar);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "ProgressDlg";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "ProgressDlg";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ProgressBar _prgBar;
        private System.Windows.Forms.Label _lblMessage;
        private System.Windows.Forms.Button _btnCancel;
        private System.Windows.Forms.Timer _AutoCountTimer;
    }
}