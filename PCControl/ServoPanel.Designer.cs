namespace MegaTracker
{
    partial class ServoPanel
    {
        /// <summary> 
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region 组件设计器生成的代码

        /// <summary> 
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.label12 = new System.Windows.Forms.Label();
            this.testTiltAcc = new System.Windows.Forms.CheckBox();
            this.testTiltNinty = new System.Windows.Forms.CheckBox();
            this.testTiltMin = new System.Windows.Forms.CheckBox();
            this.testTiltMax = new System.Windows.Forms.CheckBox();
            this.testTiltCen = new System.Windows.Forms.CheckBox();
            this.numTiltAcc = new System.Windows.Forms.NumericUpDown();
            this.numTiltNinty = new System.Windows.Forms.NumericUpDown();
            this.numTiltMin = new System.Windows.Forms.NumericUpDown();
            this.numTiltMax = new System.Windows.Forms.NumericUpDown();
            this.numTiltCen = new System.Windows.Forms.NumericUpDown();
            this.label9 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label10 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.testPanAcc = new System.Windows.Forms.CheckBox();
            this.testPanMax = new System.Windows.Forms.CheckBox();
            this.testPanCen = new System.Windows.Forms.CheckBox();
            this.numPanAcc = new System.Windows.Forms.NumericUpDown();
            this.numPanMax = new System.Windows.Forms.NumericUpDown();
            this.numPanCen = new System.Windows.Forms.NumericUpDown();
            this.cbPanRev = new System.Windows.Forms.CheckBox();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.btnSave = new System.Windows.Forms.Button();
            this.btnReset = new System.Windows.Forms.Button();
            this.label11 = new System.Windows.Forms.Label();
            this.numTiltDzone = new System.Windows.Forms.NumericUpDown();
            this.testTiltDzone = new System.Windows.Forms.CheckBox();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numTiltAcc)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numTiltNinty)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numTiltMin)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numTiltMax)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numTiltCen)).BeginInit();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numPanAcc)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numPanMax)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numPanCen)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numTiltDzone)).BeginInit();
            this.SuspendLayout();
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.testTiltDzone);
            this.groupBox2.Controls.Add(this.numTiltDzone);
            this.groupBox2.Controls.Add(this.label11);
            this.groupBox2.Controls.Add(this.label12);
            this.groupBox2.Controls.Add(this.testTiltAcc);
            this.groupBox2.Controls.Add(this.testTiltNinty);
            this.groupBox2.Controls.Add(this.testTiltMin);
            this.groupBox2.Controls.Add(this.testTiltMax);
            this.groupBox2.Controls.Add(this.testTiltCen);
            this.groupBox2.Controls.Add(this.numTiltAcc);
            this.groupBox2.Controls.Add(this.numTiltNinty);
            this.groupBox2.Controls.Add(this.numTiltMin);
            this.groupBox2.Controls.Add(this.numTiltMax);
            this.groupBox2.Controls.Add(this.numTiltCen);
            this.groupBox2.Controls.Add(this.label9);
            this.groupBox2.Controls.Add(this.label8);
            this.groupBox2.Controls.Add(this.label7);
            this.groupBox2.Controls.Add(this.label6);
            this.groupBox2.Controls.Add(this.label5);
            this.groupBox2.Location = new System.Drawing.Point(196, 3);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(188, 199);
            this.groupBox2.TabIndex = 2;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "俯仰舵机";
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(111, 13);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(29, 12);
            this.label12.TabIndex = 14;
            this.label12.Text = "测试";
            // 
            // testTiltAcc
            // 
            this.testTiltAcc.AutoSize = true;
            this.testTiltAcc.Location = new System.Drawing.Point(118, 136);
            this.testTiltAcc.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.testTiltAcc.Name = "testTiltAcc";
            this.testTiltAcc.Size = new System.Drawing.Size(15, 14);
            this.testTiltAcc.TabIndex = 21;
            this.testTiltAcc.UseVisualStyleBackColor = true;
            this.testTiltAcc.CheckedChanged += new System.EventHandler(this.testTilt_CheckedChanged);
            // 
            // testTiltNinty
            // 
            this.testTiltNinty.AutoSize = true;
            this.testTiltNinty.Location = new System.Drawing.Point(118, 111);
            this.testTiltNinty.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.testTiltNinty.Name = "testTiltNinty";
            this.testTiltNinty.Size = new System.Drawing.Size(15, 14);
            this.testTiltNinty.TabIndex = 20;
            this.testTiltNinty.UseVisualStyleBackColor = true;
            this.testTiltNinty.CheckedChanged += new System.EventHandler(this.testTilt_CheckedChanged);
            // 
            // testTiltMin
            // 
            this.testTiltMin.AutoSize = true;
            this.testTiltMin.Location = new System.Drawing.Point(118, 85);
            this.testTiltMin.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.testTiltMin.Name = "testTiltMin";
            this.testTiltMin.Size = new System.Drawing.Size(15, 14);
            this.testTiltMin.TabIndex = 19;
            this.testTiltMin.UseVisualStyleBackColor = true;
            this.testTiltMin.CheckedChanged += new System.EventHandler(this.testTilt_CheckedChanged);
            // 
            // testTiltMax
            // 
            this.testTiltMax.AutoSize = true;
            this.testTiltMax.Location = new System.Drawing.Point(118, 57);
            this.testTiltMax.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.testTiltMax.Name = "testTiltMax";
            this.testTiltMax.Size = new System.Drawing.Size(15, 14);
            this.testTiltMax.TabIndex = 18;
            this.testTiltMax.UseVisualStyleBackColor = true;
            this.testTiltMax.CheckedChanged += new System.EventHandler(this.testTilt_CheckedChanged);
            // 
            // testTiltCen
            // 
            this.testTiltCen.AutoSize = true;
            this.testTiltCen.Location = new System.Drawing.Point(118, 32);
            this.testTiltCen.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.testTiltCen.Name = "testTiltCen";
            this.testTiltCen.Size = new System.Drawing.Size(15, 14);
            this.testTiltCen.TabIndex = 12;
            this.testTiltCen.UseVisualStyleBackColor = true;
            this.testTiltCen.CheckedChanged += new System.EventHandler(this.testTilt_CheckedChanged);
            // 
            // numTiltAcc
            // 
            this.numTiltAcc.Location = new System.Drawing.Point(63, 134);
            this.numTiltAcc.Maximum = new decimal(new int[] {
            3000,
            0,
            0,
            0});
            this.numTiltAcc.Name = "numTiltAcc";
            this.numTiltAcc.Size = new System.Drawing.Size(45, 21);
            this.numTiltAcc.TabIndex = 17;
            this.numTiltAcc.Value = new decimal(new int[] {
            500,
            0,
            0,
            0});
            this.numTiltAcc.ValueChanged += new System.EventHandler(this.numTiltAcc_ValueChanged);
            // 
            // numTiltNinty
            // 
            this.numTiltNinty.Location = new System.Drawing.Point(63, 107);
            this.numTiltNinty.Maximum = new decimal(new int[] {
            2500,
            0,
            0,
            0});
            this.numTiltNinty.Minimum = new decimal(new int[] {
            500,
            0,
            0,
            0});
            this.numTiltNinty.Name = "numTiltNinty";
            this.numTiltNinty.Size = new System.Drawing.Size(45, 21);
            this.numTiltNinty.TabIndex = 16;
            this.numTiltNinty.Value = new decimal(new int[] {
            500,
            0,
            0,
            0});
            this.numTiltNinty.ValueChanged += new System.EventHandler(this.numTiltNinty_ValueChanged);
            // 
            // numTiltMin
            // 
            this.numTiltMin.Location = new System.Drawing.Point(63, 82);
            this.numTiltMin.Maximum = new decimal(new int[] {
            2500,
            0,
            0,
            0});
            this.numTiltMin.Minimum = new decimal(new int[] {
            500,
            0,
            0,
            0});
            this.numTiltMin.Name = "numTiltMin";
            this.numTiltMin.Size = new System.Drawing.Size(45, 21);
            this.numTiltMin.TabIndex = 15;
            this.numTiltMin.Value = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.numTiltMin.ValueChanged += new System.EventHandler(this.numTiltMin_ValueChanged);
            // 
            // numTiltMax
            // 
            this.numTiltMax.Location = new System.Drawing.Point(63, 56);
            this.numTiltMax.Maximum = new decimal(new int[] {
            2500,
            0,
            0,
            0});
            this.numTiltMax.Minimum = new decimal(new int[] {
            500,
            0,
            0,
            0});
            this.numTiltMax.Name = "numTiltMax";
            this.numTiltMax.Size = new System.Drawing.Size(45, 21);
            this.numTiltMax.TabIndex = 7;
            this.numTiltMax.Value = new decimal(new int[] {
            2000,
            0,
            0,
            0});
            this.numTiltMax.ValueChanged += new System.EventHandler(this.numTiltMax_ValueChanged);
            // 
            // numTiltCen
            // 
            this.numTiltCen.Location = new System.Drawing.Point(63, 29);
            this.numTiltCen.Maximum = new decimal(new int[] {
            2500,
            0,
            0,
            0});
            this.numTiltCen.Minimum = new decimal(new int[] {
            500,
            0,
            0,
            0});
            this.numTiltCen.Name = "numTiltCen";
            this.numTiltCen.Size = new System.Drawing.Size(45, 21);
            this.numTiltCen.TabIndex = 9;
            this.numTiltCen.Value = new decimal(new int[] {
            1500,
            0,
            0,
            0});
            this.numTiltCen.ValueChanged += new System.EventHandler(this.numTiltCen_ValueChanged);
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(6, 134);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(53, 12);
            this.label9.TabIndex = 14;
            this.label9.Text = "加 速 度";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(6, 107);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(53, 12);
            this.label8.TabIndex = 12;
            this.label8.Text = "90度校准";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(6, 83);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(53, 12);
            this.label7.TabIndex = 11;
            this.label7.Text = "最小角度";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(6, 57);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(53, 12);
            this.label6.TabIndex = 9;
            this.label6.Text = "最大角度";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(6, 30);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(53, 12);
            this.label5.TabIndex = 1;
            this.label5.Text = "中 立 点";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label10);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.testPanAcc);
            this.groupBox1.Controls.Add(this.testPanMax);
            this.groupBox1.Controls.Add(this.testPanCen);
            this.groupBox1.Controls.Add(this.numPanAcc);
            this.groupBox1.Controls.Add(this.numPanMax);
            this.groupBox1.Controls.Add(this.numPanCen);
            this.groupBox1.Controls.Add(this.cbPanRev);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Location = new System.Drawing.Point(3, 3);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(188, 199);
            this.groupBox1.TabIndex = 3;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "旋转舵机";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(111, 14);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(29, 12);
            this.label10.TabIndex = 13;
            this.label10.Text = "测试";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 110);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(53, 12);
            this.label1.TabIndex = 12;
            this.label1.Text = "逆    转";
            // 
            // testPanAcc
            // 
            this.testPanAcc.AutoSize = true;
            this.testPanAcc.Location = new System.Drawing.Point(119, 83);
            this.testPanAcc.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.testPanAcc.Name = "testPanAcc";
            this.testPanAcc.Size = new System.Drawing.Size(15, 14);
            this.testPanAcc.TabIndex = 11;
            this.testPanAcc.UseVisualStyleBackColor = true;
            this.testPanAcc.CheckedChanged += new System.EventHandler(this.testPan_CheckChanged);
            // 
            // testPanMax
            // 
            this.testPanMax.AutoSize = true;
            this.testPanMax.Location = new System.Drawing.Point(119, 57);
            this.testPanMax.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.testPanMax.Name = "testPanMax";
            this.testPanMax.Size = new System.Drawing.Size(15, 14);
            this.testPanMax.TabIndex = 10;
            this.testPanMax.UseVisualStyleBackColor = true;
            this.testPanMax.CheckedChanged += new System.EventHandler(this.testPan_CheckChanged);
            // 
            // testPanCen
            // 
            this.testPanCen.AutoSize = true;
            this.testPanCen.Location = new System.Drawing.Point(119, 31);
            this.testPanCen.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.testPanCen.Name = "testPanCen";
            this.testPanCen.Size = new System.Drawing.Size(15, 14);
            this.testPanCen.TabIndex = 9;
            this.testPanCen.UseVisualStyleBackColor = true;
            this.testPanCen.CheckedChanged += new System.EventHandler(this.testPan_CheckChanged);
            // 
            // numPanAcc
            // 
            this.numPanAcc.Location = new System.Drawing.Point(62, 82);
            this.numPanAcc.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.numPanAcc.Name = "numPanAcc";
            this.numPanAcc.Size = new System.Drawing.Size(45, 21);
            this.numPanAcc.TabIndex = 8;
            this.numPanAcc.Value = new decimal(new int[] {
            500,
            0,
            0,
            0});
            this.numPanAcc.ValueChanged += new System.EventHandler(this.numPanAcc_ValueChanged);
            // 
            // numPanMax
            // 
            this.numPanMax.Location = new System.Drawing.Point(62, 55);
            this.numPanMax.Maximum = new decimal(new int[] {
            2500,
            0,
            0,
            0});
            this.numPanMax.Minimum = new decimal(new int[] {
            500,
            0,
            0,
            0});
            this.numPanMax.Name = "numPanMax";
            this.numPanMax.Size = new System.Drawing.Size(45, 21);
            this.numPanMax.TabIndex = 7;
            this.numPanMax.Value = new decimal(new int[] {
            2000,
            0,
            0,
            0});
            this.numPanMax.ValueChanged += new System.EventHandler(this.numPanMax_ValueChanged);
            // 
            // numPanCen
            // 
            this.numPanCen.Location = new System.Drawing.Point(62, 29);
            this.numPanCen.Maximum = new decimal(new int[] {
            2500,
            0,
            0,
            0});
            this.numPanCen.Minimum = new decimal(new int[] {
            500,
            0,
            0,
            0});
            this.numPanCen.Name = "numPanCen";
            this.numPanCen.Size = new System.Drawing.Size(45, 21);
            this.numPanCen.TabIndex = 6;
            this.numPanCen.Value = new decimal(new int[] {
            1500,
            0,
            0,
            0});
            this.numPanCen.ValueChanged += new System.EventHandler(this.numPanCen_ValueChanged);
            // 
            // cbPanRev
            // 
            this.cbPanRev.AutoSize = true;
            this.cbPanRev.Location = new System.Drawing.Point(62, 108);
            this.cbPanRev.Name = "cbPanRev";
            this.cbPanRev.Size = new System.Drawing.Size(15, 14);
            this.cbPanRev.TabIndex = 5;
            this.cbPanRev.UseVisualStyleBackColor = true;
            this.cbPanRev.CheckedChanged += new System.EventHandler(this.cbPanRev_CheckedChanged);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(6, 83);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(53, 12);
            this.label4.TabIndex = 2;
            this.label4.Text = "加 速 度";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(6, 57);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(53, 12);
            this.label3.TabIndex = 1;
            this.label3.Text = "最大速度";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 33);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(53, 12);
            this.label2.TabIndex = 0;
            this.label2.Text = "中 立 点";
            // 
            // btnSave
            // 
            this.btnSave.Location = new System.Drawing.Point(267, 208);
            this.btnSave.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(56, 18);
            this.btnSave.TabIndex = 4;
            this.btnSave.Text = "保存";
            this.btnSave.UseVisualStyleBackColor = true;
            this.btnSave.Click += new System.EventHandler(this.btnSave_Click);
            // 
            // btnReset
            // 
            this.btnReset.Location = new System.Drawing.Point(328, 208);
            this.btnReset.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.btnReset.Name = "btnReset";
            this.btnReset.Size = new System.Drawing.Size(56, 18);
            this.btnReset.TabIndex = 5;
            this.btnReset.Text = "重置";
            this.btnReset.UseVisualStyleBackColor = true;
            this.btnReset.Click += new System.EventHandler(this.btnReset_Click);
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(6, 161);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(53, 12);
            this.label11.TabIndex = 22;
            this.label11.Text = "死    区";
            // 
            // numTiltDzone
            // 
            this.numTiltDzone.Location = new System.Drawing.Point(63, 161);
            this.numTiltDzone.Name = "numTiltDzone";
            this.numTiltDzone.Size = new System.Drawing.Size(45, 21);
            this.numTiltDzone.TabIndex = 23;
            // 
            // testTiltDzone
            // 
            this.testTiltDzone.AutoSize = true;
            this.testTiltDzone.Location = new System.Drawing.Point(118, 163);
            this.testTiltDzone.Margin = new System.Windows.Forms.Padding(2);
            this.testTiltDzone.Name = "testTiltDzone";
            this.testTiltDzone.Size = new System.Drawing.Size(15, 14);
            this.testTiltDzone.TabIndex = 24;
            this.testTiltDzone.UseVisualStyleBackColor = true;
            // 
            // ServoPanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.btnReset);
            this.Controls.Add(this.btnSave);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.groupBox2);
            this.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.Name = "ServoPanel";
            this.Size = new System.Drawing.Size(391, 246);
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numTiltAcc)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numTiltNinty)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numTiltMin)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numTiltMax)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numTiltCen)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numPanAcc)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numPanMax)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numPanCen)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numTiltDzone)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.CheckBox testTiltAcc;
        private System.Windows.Forms.CheckBox testTiltNinty;
        private System.Windows.Forms.CheckBox testTiltMin;
        private System.Windows.Forms.CheckBox testTiltMax;
        private System.Windows.Forms.CheckBox testTiltCen;
        private System.Windows.Forms.NumericUpDown numTiltAcc;
        private System.Windows.Forms.NumericUpDown numTiltNinty;
        private System.Windows.Forms.NumericUpDown numTiltMin;
        private System.Windows.Forms.NumericUpDown numTiltMax;
        private System.Windows.Forms.NumericUpDown numTiltCen;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.CheckBox testPanAcc;
        private System.Windows.Forms.CheckBox testPanMax;
        private System.Windows.Forms.CheckBox testPanCen;
        private System.Windows.Forms.NumericUpDown numPanAcc;
        private System.Windows.Forms.NumericUpDown numPanMax;
        private System.Windows.Forms.NumericUpDown numPanCen;
        private System.Windows.Forms.CheckBox cbPanRev;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Button btnSave;
        private System.Windows.Forms.Button btnReset;
        private System.Windows.Forms.CheckBox testTiltDzone;
        private System.Windows.Forms.NumericUpDown numTiltDzone;
        private System.Windows.Forms.Label label11;
    }
}
