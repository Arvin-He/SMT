﻿namespace SMT_CSharp
{
    partial class SMTDlg
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(SMTDlg));
            this.ccdView = new System.Windows.Forms.PictureBox();
            this.toolBar = new System.Windows.Forms.ToolStrip();
            this.openCCDBtn = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.stopCCDBtn = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.closeCCDBtn = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.savePicBtn = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
            this.saveVideoBtn = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator5 = new System.Windows.Forms.ToolStripSeparator();
            this.closeVideoBtn = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator6 = new System.Windows.Forms.ToolStripSeparator();
            this.saveCCDParamBtn = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator7 = new System.Windows.Forms.ToolStripSeparator();
            this.loadCCDParamBtn = new System.Windows.Forms.ToolStripButton();
            this.label1 = new System.Windows.Forms.Label();
            this.gainTrackBar = new System.Windows.Forms.TrackBar();
            this.label2 = new System.Windows.Forms.Label();
            this.exposureTrackBar = new System.Windows.Forms.TrackBar();
            this.gainUpDown = new System.Windows.Forms.NumericUpDown();
            this.exposureUpDown = new System.Windows.Forms.NumericUpDown();
            ((System.ComponentModel.ISupportInitialize)(this.ccdView)).BeginInit();
            this.toolBar.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.gainTrackBar)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.exposureTrackBar)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.gainUpDown)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.exposureUpDown)).BeginInit();
            this.SuspendLayout();
            // 
            // ccdView
            // 
            this.ccdView.BackColor = System.Drawing.SystemColors.ControlDarkDark;
            this.ccdView.Location = new System.Drawing.Point(5, 89);
            this.ccdView.Name = "ccdView";
            this.ccdView.Size = new System.Drawing.Size(640, 480);
            this.ccdView.TabIndex = 0;
            this.ccdView.TabStop = false;
            // 
            // toolBar
            // 
            this.toolBar.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.toolBar.ImageScalingSize = new System.Drawing.Size(32, 32);
            this.toolBar.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.openCCDBtn,
            this.toolStripSeparator1,
            this.stopCCDBtn,
            this.toolStripSeparator2,
            this.closeCCDBtn,
            this.toolStripSeparator3,
            this.savePicBtn,
            this.toolStripSeparator4,
            this.saveVideoBtn,
            this.toolStripSeparator5,
            this.closeVideoBtn,
            this.toolStripSeparator6,
            this.saveCCDParamBtn,
            this.toolStripSeparator7,
            this.loadCCDParamBtn});
            this.toolBar.Location = new System.Drawing.Point(0, 0);
            this.toolBar.Margin = new System.Windows.Forms.Padding(10, 0, 0, 0);
            this.toolBar.Name = "toolBar";
            this.toolBar.Size = new System.Drawing.Size(1008, 39);
            this.toolBar.Stretch = true;
            this.toolBar.TabIndex = 2;
            this.toolBar.Text = "toolStrip1";
            // 
            // openCCDBtn
            // 
            this.openCCDBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.openCCDBtn.Image = ((System.Drawing.Image)(resources.GetObject("openCCDBtn.Image")));
            this.openCCDBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.openCCDBtn.Margin = new System.Windows.Forms.Padding(5, 1, 0, 2);
            this.openCCDBtn.Name = "openCCDBtn";
            this.openCCDBtn.Size = new System.Drawing.Size(36, 36);
            this.openCCDBtn.Text = "toolStripButton1";
            this.openCCDBtn.Click += new System.EventHandler(this.OpenCCDBtn_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 39);
            // 
            // stopCCDBtn
            // 
            this.stopCCDBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.stopCCDBtn.Image = ((System.Drawing.Image)(resources.GetObject("stopCCDBtn.Image")));
            this.stopCCDBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.stopCCDBtn.Name = "stopCCDBtn";
            this.stopCCDBtn.Size = new System.Drawing.Size(36, 36);
            this.stopCCDBtn.Text = "toolStripButton2";
            this.stopCCDBtn.Click += new System.EventHandler(this.StopCCD_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 39);
            // 
            // closeCCDBtn
            // 
            this.closeCCDBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.closeCCDBtn.Image = ((System.Drawing.Image)(resources.GetObject("closeCCDBtn.Image")));
            this.closeCCDBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.closeCCDBtn.Name = "closeCCDBtn";
            this.closeCCDBtn.Size = new System.Drawing.Size(36, 36);
            this.closeCCDBtn.Text = "toolStripButton3";
            this.closeCCDBtn.Click += new System.EventHandler(this.CloseCCD_Click);
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(6, 39);
            // 
            // savePicBtn
            // 
            this.savePicBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.savePicBtn.Image = ((System.Drawing.Image)(resources.GetObject("savePicBtn.Image")));
            this.savePicBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.savePicBtn.Name = "savePicBtn";
            this.savePicBtn.Size = new System.Drawing.Size(36, 36);
            this.savePicBtn.Text = "toolStripButton4";
            this.savePicBtn.Click += new System.EventHandler(this.SavePic_Click);
            // 
            // toolStripSeparator4
            // 
            this.toolStripSeparator4.Name = "toolStripSeparator4";
            this.toolStripSeparator4.Size = new System.Drawing.Size(6, 39);
            // 
            // saveVideoBtn
            // 
            this.saveVideoBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.saveVideoBtn.Image = ((System.Drawing.Image)(resources.GetObject("saveVideoBtn.Image")));
            this.saveVideoBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.saveVideoBtn.Name = "saveVideoBtn";
            this.saveVideoBtn.Size = new System.Drawing.Size(36, 36);
            this.saveVideoBtn.Text = "toolStripButton5";
            this.saveVideoBtn.Click += new System.EventHandler(this.SaveVideo_Click);
            // 
            // toolStripSeparator5
            // 
            this.toolStripSeparator5.Name = "toolStripSeparator5";
            this.toolStripSeparator5.Size = new System.Drawing.Size(6, 39);
            // 
            // closeVideoBtn
            // 
            this.closeVideoBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.closeVideoBtn.Image = ((System.Drawing.Image)(resources.GetObject("closeVideoBtn.Image")));
            this.closeVideoBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.closeVideoBtn.Name = "closeVideoBtn";
            this.closeVideoBtn.Size = new System.Drawing.Size(36, 36);
            this.closeVideoBtn.Text = "toolStripButton6";
            this.closeVideoBtn.Click += new System.EventHandler(this.StopSaveVideo_Click);
            // 
            // toolStripSeparator6
            // 
            this.toolStripSeparator6.Name = "toolStripSeparator6";
            this.toolStripSeparator6.Size = new System.Drawing.Size(6, 39);
            // 
            // saveCCDParamBtn
            // 
            this.saveCCDParamBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.saveCCDParamBtn.Image = ((System.Drawing.Image)(resources.GetObject("saveCCDParamBtn.Image")));
            this.saveCCDParamBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.saveCCDParamBtn.Name = "saveCCDParamBtn";
            this.saveCCDParamBtn.Size = new System.Drawing.Size(36, 36);
            this.saveCCDParamBtn.Text = "toolStripButton7";
            this.saveCCDParamBtn.Click += new System.EventHandler(this.SaveCCDParam_Click);
            // 
            // toolStripSeparator7
            // 
            this.toolStripSeparator7.Name = "toolStripSeparator7";
            this.toolStripSeparator7.Size = new System.Drawing.Size(6, 39);
            // 
            // loadCCDParamBtn
            // 
            this.loadCCDParamBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.loadCCDParamBtn.Image = ((System.Drawing.Image)(resources.GetObject("loadCCDParamBtn.Image")));
            this.loadCCDParamBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.loadCCDParamBtn.Name = "loadCCDParamBtn";
            this.loadCCDParamBtn.Size = new System.Drawing.Size(36, 36);
            this.loadCCDParamBtn.Text = "toolStripButton1";
            this.loadCCDParamBtn.Click += new System.EventHandler(this.LoadCCDParam_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 54);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(35, 12);
            this.label1.TabIndex = 3;
            this.label1.Text = "增益:";
            // 
            // gainTrackBar
            // 
            this.gainTrackBar.Location = new System.Drawing.Point(49, 42);
            this.gainTrackBar.Maximum = 64;
            this.gainTrackBar.Name = "gainTrackBar";
            this.gainTrackBar.Size = new System.Drawing.Size(160, 45);
            this.gainTrackBar.TabIndex = 4;
            this.gainTrackBar.TickFrequency = 8;
            this.gainTrackBar.TickStyle = System.Windows.Forms.TickStyle.TopLeft;
            this.gainTrackBar.Value = 9;
            this.gainTrackBar.Scroll += new System.EventHandler(this.SetGain_Scroll);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(334, 54);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(47, 12);
            this.label2.TabIndex = 3;
            this.label2.Text = "曝光度:";
            // 
            // exposureTrackBar
            // 
            this.exposureTrackBar.Location = new System.Drawing.Point(379, 42);
            this.exposureTrackBar.Maximum = 1000;
            this.exposureTrackBar.Name = "exposureTrackBar";
            this.exposureTrackBar.Size = new System.Drawing.Size(160, 45);
            this.exposureTrackBar.TabIndex = 4;
            this.exposureTrackBar.TickFrequency = 100;
            this.exposureTrackBar.TickStyle = System.Windows.Forms.TickStyle.TopLeft;
            this.exposureTrackBar.Value = 600;
            this.exposureTrackBar.Scroll += new System.EventHandler(this.SetExposure_Scroll);
            // 
            // gainUpDown
            // 
            this.gainUpDown.Location = new System.Drawing.Point(209, 52);
            this.gainUpDown.Maximum = new decimal(new int[] {
            64,
            0,
            0,
            0});
            this.gainUpDown.Name = "gainUpDown";
            this.gainUpDown.Size = new System.Drawing.Size(80, 21);
            this.gainUpDown.TabIndex = 5;
            this.gainUpDown.Value = new decimal(new int[] {
            9,
            0,
            0,
            0});
            this.gainUpDown.ValueChanged += new System.EventHandler(this.SetGainUpDown);
            // 
            // exposureUpDown
            // 
            this.exposureUpDown.Increment = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.exposureUpDown.Location = new System.Drawing.Point(536, 52);
            this.exposureUpDown.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.exposureUpDown.Name = "exposureUpDown";
            this.exposureUpDown.Size = new System.Drawing.Size(80, 21);
            this.exposureUpDown.TabIndex = 6;
            this.exposureUpDown.Value = new decimal(new int[] {
            600,
            0,
            0,
            0});
            this.exposureUpDown.ValueChanged += new System.EventHandler(this.SetExposureUpDown);
            // 
            // SMTDlg
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1008, 729);
            this.Controls.Add(this.exposureUpDown);
            this.Controls.Add(this.gainUpDown);
            this.Controls.Add(this.exposureTrackBar);
            this.Controls.Add(this.gainTrackBar);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.toolBar);
            this.Controls.Add(this.ccdView);
            this.Name = "SMTDlg";
            this.Text = "贴片机操作软件";
            ((System.ComponentModel.ISupportInitialize)(this.ccdView)).EndInit();
            this.toolBar.ResumeLayout(false);
            this.toolBar.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.gainTrackBar)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.exposureTrackBar)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.gainUpDown)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.exposureUpDown)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox ccdView;
        private System.Windows.Forms.ToolStrip toolBar;
        private System.Windows.Forms.ToolStripButton openCCDBtn;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripButton stopCCDBtn;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripButton closeCCDBtn;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripButton savePicBtn;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
        private System.Windows.Forms.ToolStripButton saveVideoBtn;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator5;
        private System.Windows.Forms.ToolStripButton closeVideoBtn;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator6;
        private System.Windows.Forms.ToolStripButton saveCCDParamBtn;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator7;
        private System.Windows.Forms.ToolStripButton loadCCDParamBtn;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TrackBar gainTrackBar;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TrackBar exposureTrackBar;
        private System.Windows.Forms.NumericUpDown gainUpDown;
        private System.Windows.Forms.NumericUpDown exposureUpDown;
    }
}

