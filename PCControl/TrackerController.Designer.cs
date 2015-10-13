namespace MegaTracker
{
    partial class TrackerController
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

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.btnS = new System.Windows.Forms.Button();
            this.btnClear = new System.Windows.Forms.Button();
            this.btnDown = new System.Windows.Forms.Button();
            this.btnRight = new System.Windows.Forms.Button();
            this.btnLeft = new System.Windows.Forms.Button();
            this.btnUp = new System.Windows.Forms.Button();
            this.label11 = new System.Windows.Forms.Label();
            this.btnConnect = new System.Windows.Forms.Button();
            this.cbPort = new System.Windows.Forms.ComboBox();
            this.tabPageServo = new System.Windows.Forms.TabPage();
            this.btnServoReset = new System.Windows.Forms.Button();
            this.btnServoSave = new System.Windows.Forms.Button();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            this.btnPlayback = new System.Windows.Forms.Button();
            this.btnUpdatePos = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.btnConnMav = new System.Windows.Forms.Button();
            this.cbMavPort = new System.Windows.Forms.ComboBox();
            this.label14 = new System.Windows.Forms.Label();
            this.label13 = new System.Windows.Forms.Label();
            this.label12 = new System.Windows.Forms.Label();
            this.tbxAlt = new System.Windows.Forms.TextBox();
            this.textBox5 = new System.Windows.Forms.TextBox();
            this.button2 = new System.Windows.Forms.Button();
            this.btnPointTo = new System.Windows.Forms.Button();
            this.tbxLat = new System.Windows.Forms.TextBox();
            this.tbxLon = new System.Windows.Forms.TextBox();
            this.label10 = new System.Windows.Forms.Label();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.mavTimer = new System.Windows.Forms.Timer(this.components);
            this.serialPort2 = new System.IO.Ports.SerialPort(this.components);
            this.btnOpenTlog = new System.Windows.Forms.Button();
            this.glcd1 = new MegaTracker.GLCD();
            this.servoPanel1 = new MegaTracker.ServoPanel();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPageServo.SuspendLayout();
            this.tabPage3.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPageServo);
            this.tabControl1.Controls.Add(this.tabPage3);
            this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl1.Location = new System.Drawing.Point(0, 0);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(452, 310);
            this.tabControl1.TabIndex = 0;
            this.tabControl1.SelectedIndexChanged += new System.EventHandler(this.tabControl1_SelectedIndexChanged);
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.btnS);
            this.tabPage1.Controls.Add(this.glcd1);
            this.tabPage1.Controls.Add(this.btnClear);
            this.tabPage1.Controls.Add(this.btnDown);
            this.tabPage1.Controls.Add(this.btnRight);
            this.tabPage1.Controls.Add(this.btnLeft);
            this.tabPage1.Controls.Add(this.btnUp);
            this.tabPage1.Controls.Add(this.label11);
            this.tabPage1.Controls.Add(this.btnConnect);
            this.tabPage1.Controls.Add(this.cbPort);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(444, 284);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "液晶";
            this.tabPage1.UseVisualStyleBackColor = true;
            this.tabPage1.Click += new System.EventHandler(this.tabPage1_Click);
            // 
            // btnS
            // 
            this.btnS.Location = new System.Drawing.Point(285, 153);
            this.btnS.Name = "btnS";
            this.btnS.Size = new System.Drawing.Size(25, 25);
            this.btnS.TabIndex = 11;
            this.btnS.Text = "E";
            this.btnS.UseVisualStyleBackColor = true;
            this.btnS.Click += new System.EventHandler(this.CtrlBtn_Click);
            // 
            // btnClear
            // 
            this.btnClear.Location = new System.Drawing.Point(343, 153);
            this.btnClear.Name = "btnClear";
            this.btnClear.Size = new System.Drawing.Size(25, 25);
            this.btnClear.TabIndex = 9;
            this.btnClear.Text = "C";
            this.btnClear.UseVisualStyleBackColor = true;
            this.btnClear.Click += new System.EventHandler(this.CtrlBtn_Click);
            // 
            // btnDown
            // 
            this.btnDown.Location = new System.Drawing.Point(312, 109);
            this.btnDown.Name = "btnDown";
            this.btnDown.Size = new System.Drawing.Size(25, 25);
            this.btnDown.TabIndex = 7;
            this.btnDown.Text = "D";
            this.btnDown.UseVisualStyleBackColor = true;
            this.btnDown.Click += new System.EventHandler(this.CtrlBtn_Click);
            // 
            // btnRight
            // 
            this.btnRight.Location = new System.Drawing.Point(343, 78);
            this.btnRight.Name = "btnRight";
            this.btnRight.Size = new System.Drawing.Size(25, 25);
            this.btnRight.TabIndex = 6;
            this.btnRight.Text = "R";
            this.btnRight.UseVisualStyleBackColor = true;
            this.btnRight.Click += new System.EventHandler(this.CtrlBtn_Click);
            // 
            // btnLeft
            // 
            this.btnLeft.Location = new System.Drawing.Point(283, 78);
            this.btnLeft.Name = "btnLeft";
            this.btnLeft.Size = new System.Drawing.Size(25, 25);
            this.btnLeft.TabIndex = 5;
            this.btnLeft.Text = "L";
            this.btnLeft.UseVisualStyleBackColor = true;
            this.btnLeft.Click += new System.EventHandler(this.CtrlBtn_Click);
            // 
            // btnUp
            // 
            this.btnUp.Location = new System.Drawing.Point(312, 48);
            this.btnUp.Name = "btnUp";
            this.btnUp.Size = new System.Drawing.Size(25, 25);
            this.btnUp.TabIndex = 4;
            this.btnUp.Text = "U";
            this.btnUp.UseVisualStyleBackColor = true;
            this.btnUp.Click += new System.EventHandler(this.CtrlBtn_Click);
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(14, 264);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(29, 12);
            this.label11.TabIndex = 2;
            this.label11.Text = "端口";
            // 
            // btnConnect
            // 
            this.btnConnect.Location = new System.Drawing.Point(172, 254);
            this.btnConnect.Name = "btnConnect";
            this.btnConnect.Size = new System.Drawing.Size(75, 23);
            this.btnConnect.TabIndex = 1;
            this.btnConnect.Text = "连接";
            this.btnConnect.UseVisualStyleBackColor = true;
            this.btnConnect.Click += new System.EventHandler(this.btnConnect_Click);
            // 
            // cbPort
            // 
            this.cbPort.FormattingEnabled = true;
            this.cbPort.Location = new System.Drawing.Point(44, 256);
            this.cbPort.Name = "cbPort";
            this.cbPort.Size = new System.Drawing.Size(121, 20);
            this.cbPort.TabIndex = 0;
            this.cbPort.DropDown += new System.EventHandler(this.cbPort_DropDown);
            // 
            // tabPageServo
            // 
            this.tabPageServo.Controls.Add(this.servoPanel1);
            this.tabPageServo.Controls.Add(this.btnServoReset);
            this.tabPageServo.Controls.Add(this.btnServoSave);
            this.tabPageServo.Location = new System.Drawing.Point(4, 22);
            this.tabPageServo.Name = "tabPageServo";
            this.tabPageServo.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageServo.Size = new System.Drawing.Size(444, 284);
            this.tabPageServo.TabIndex = 1;
            this.tabPageServo.Text = "舵机";
            this.tabPageServo.UseVisualStyleBackColor = true;
            this.tabPageServo.Click += new System.EventHandler(this.tabPage2_Click);
            // 
            // btnServoReset
            // 
            this.btnServoReset.Location = new System.Drawing.Point(386, 332);
            this.btnServoReset.Name = "btnServoReset";
            this.btnServoReset.Size = new System.Drawing.Size(75, 23);
            this.btnServoReset.TabIndex = 3;
            this.btnServoReset.Text = "重置";
            this.btnServoReset.UseVisualStyleBackColor = true;
            // 
            // btnServoSave
            // 
            this.btnServoSave.Location = new System.Drawing.Point(306, 332);
            this.btnServoSave.Name = "btnServoSave";
            this.btnServoSave.Size = new System.Drawing.Size(75, 23);
            this.btnServoSave.TabIndex = 2;
            this.btnServoSave.Text = "保存";
            this.btnServoSave.UseVisualStyleBackColor = true;
            // 
            // tabPage3
            // 
            this.tabPage3.Controls.Add(this.btnOpenTlog);
            this.tabPage3.Controls.Add(this.btnPlayback);
            this.tabPage3.Controls.Add(this.btnUpdatePos);
            this.tabPage3.Controls.Add(this.label2);
            this.tabPage3.Controls.Add(this.btnConnMav);
            this.tabPage3.Controls.Add(this.cbMavPort);
            this.tabPage3.Controls.Add(this.label14);
            this.tabPage3.Controls.Add(this.label13);
            this.tabPage3.Controls.Add(this.label12);
            this.tabPage3.Controls.Add(this.tbxAlt);
            this.tabPage3.Controls.Add(this.textBox5);
            this.tabPage3.Controls.Add(this.button2);
            this.tabPage3.Controls.Add(this.btnPointTo);
            this.tabPage3.Controls.Add(this.tbxLat);
            this.tabPage3.Controls.Add(this.tbxLon);
            this.tabPage3.Controls.Add(this.label10);
            this.tabPage3.Controls.Add(this.textBox2);
            this.tabPage3.Controls.Add(this.textBox1);
            this.tabPage3.Controls.Add(this.label1);
            this.tabPage3.Location = new System.Drawing.Point(4, 22);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage3.Size = new System.Drawing.Size(444, 284);
            this.tabPage3.TabIndex = 2;
            this.tabPage3.Text = "测试";
            this.tabPage3.UseVisualStyleBackColor = true;
            // 
            // btnPlayback
            // 
            this.btnPlayback.Enabled = false;
            this.btnPlayback.Location = new System.Drawing.Point(132, 146);
            this.btnPlayback.Name = "btnPlayback";
            this.btnPlayback.Size = new System.Drawing.Size(75, 23);
            this.btnPlayback.TabIndex = 17;
            this.btnPlayback.Text = "回放";
            this.btnPlayback.UseVisualStyleBackColor = true;
            this.btnPlayback.Click += new System.EventHandler(this.btnPlayback_Click);
            // 
            // btnUpdatePos
            // 
            this.btnUpdatePos.Location = new System.Drawing.Point(317, 62);
            this.btnUpdatePos.Name = "btnUpdatePos";
            this.btnUpdatePos.Size = new System.Drawing.Size(75, 23);
            this.btnUpdatePos.TabIndex = 16;
            this.btnUpdatePos.Text = "更新";
            this.btnUpdatePos.UseVisualStyleBackColor = true;
            this.btnUpdatePos.Click += new System.EventHandler(this.btnUpdatePos_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(22, 257);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(29, 12);
            this.label2.TabIndex = 15;
            this.label2.Text = "端口";
            // 
            // btnConnMav
            // 
            this.btnConnMav.Location = new System.Drawing.Point(180, 247);
            this.btnConnMav.Name = "btnConnMav";
            this.btnConnMav.Size = new System.Drawing.Size(75, 23);
            this.btnConnMav.TabIndex = 14;
            this.btnConnMav.Text = "连接";
            this.btnConnMav.UseVisualStyleBackColor = true;
            this.btnConnMav.Click += new System.EventHandler(this.btnConnMav_Click);
            // 
            // cbMavPort
            // 
            this.cbMavPort.FormattingEnabled = true;
            this.cbMavPort.Location = new System.Drawing.Point(52, 249);
            this.cbMavPort.Name = "cbMavPort";
            this.cbMavPort.Size = new System.Drawing.Size(121, 20);
            this.cbMavPort.TabIndex = 13;
            this.cbMavPort.DropDown += new System.EventHandler(this.cbMavPort_DropDown);
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(257, 11);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(29, 12);
            this.label14.TabIndex = 12;
            this.label14.Text = "高度";
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(178, 11);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(29, 12);
            this.label13.TabIndex = 11;
            this.label13.Text = "纬度";
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(97, 11);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(29, 12);
            this.label12.TabIndex = 10;
            this.label12.Text = "经度";
            // 
            // tbxAlt
            // 
            this.tbxAlt.Location = new System.Drawing.Point(235, 65);
            this.tbxAlt.Name = "tbxAlt";
            this.tbxAlt.Size = new System.Drawing.Size(76, 21);
            this.tbxAlt.TabIndex = 9;
            this.tbxAlt.TextChanged += new System.EventHandler(this.textBox6_TextChanged);
            // 
            // textBox5
            // 
            this.textBox5.Location = new System.Drawing.Point(235, 26);
            this.textBox5.Name = "textBox5";
            this.textBox5.Size = new System.Drawing.Size(76, 21);
            this.textBox5.TabIndex = 8;
            this.textBox5.TextChanged += new System.EventHandler(this.textBox5_TextChanged);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(317, 28);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(75, 23);
            this.button2.TabIndex = 7;
            this.button2.Text = "设置";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // btnPointTo
            // 
            this.btnPointTo.Enabled = false;
            this.btnPointTo.Location = new System.Drawing.Point(261, 247);
            this.btnPointTo.Name = "btnPointTo";
            this.btnPointTo.Size = new System.Drawing.Size(56, 23);
            this.btnPointTo.TabIndex = 6;
            this.btnPointTo.Text = "对准";
            this.btnPointTo.UseVisualStyleBackColor = true;
            this.btnPointTo.Click += new System.EventHandler(this.btnPointTo_Click);
            // 
            // tbxLat
            // 
            this.tbxLat.Location = new System.Drawing.Point(153, 65);
            this.tbxLat.Name = "tbxLat";
            this.tbxLat.Size = new System.Drawing.Size(76, 21);
            this.tbxLat.TabIndex = 5;
            // 
            // tbxLon
            // 
            this.tbxLon.Location = new System.Drawing.Point(71, 65);
            this.tbxLon.Name = "tbxLon";
            this.tbxLon.Size = new System.Drawing.Size(76, 21);
            this.tbxLon.TabIndex = 4;
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(21, 68);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(35, 12);
            this.label10.TabIndex = 3;
            this.label10.Text = "plane";
            // 
            // textBox2
            // 
            this.textBox2.Location = new System.Drawing.Point(153, 26);
            this.textBox2.Name = "textBox2";
            this.textBox2.Size = new System.Drawing.Size(76, 21);
            this.textBox2.TabIndex = 2;
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(71, 26);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(76, 21);
            this.textBox1.TabIndex = 1;
            this.textBox1.TextChanged += new System.EventHandler(this.textBox1_TextChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(21, 29);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(29, 12);
            this.label1.TabIndex = 0;
            this.label1.Text = "home";
            // 
            // timer1
            // 
            this.timer1.Interval = 5000;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // mavTimer
            // 
            this.mavTimer.Interval = 200;
            this.mavTimer.Tick += new System.EventHandler(this.mavTimer_Tick);
            // 
            // btnOpenTlog
            // 
            this.btnOpenTlog.Location = new System.Drawing.Point(51, 146);
            this.btnOpenTlog.Name = "btnOpenTlog";
            this.btnOpenTlog.Size = new System.Drawing.Size(75, 23);
            this.btnOpenTlog.TabIndex = 18;
            this.btnOpenTlog.Text = "打开tlog";
            this.btnOpenTlog.UseVisualStyleBackColor = true;
            this.btnOpenTlog.Click += new System.EventHandler(this.btnOpenTlog_Click);
            // 
            // glcd1
            // 
            this.glcd1.Location = new System.Drawing.Point(15, 18);
            this.glcd1.Margin = new System.Windows.Forms.Padding(4);
            this.glcd1.Name = "glcd1";
            this.glcd1.Size = new System.Drawing.Size(244, 209);
            this.glcd1.TabIndex = 10;
            // 
            // servoPanel1
            // 
            this.servoPanel1.Link = null;
            this.servoPanel1.Location = new System.Drawing.Point(21, 21);
            this.servoPanel1.Margin = new System.Windows.Forms.Padding(2);
            this.servoPanel1.Name = "servoPanel1";
            this.servoPanel1.Size = new System.Drawing.Size(391, 246);
            this.servoPanel1.TabIndex = 4;
            // 
            // TrackerController
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(452, 310);
            this.Controls.Add(this.tabControl1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "TrackerController";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.TrackerController_Load);
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            this.tabPageServo.ResumeLayout(false);
            this.tabPage3.ResumeLayout(false);
            this.tabPage3.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPageServo;
        private System.Windows.Forms.Button btnServoReset;
        private System.Windows.Forms.Button btnServoSave;
        private System.Windows.Forms.TabPage tabPage3;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btnPointTo;
        private System.Windows.Forms.TextBox tbxLat;
        private System.Windows.Forms.TextBox tbxLon;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Button btnConnect;
        private System.Windows.Forms.ComboBox cbPort;
        private System.IO.Ports.SerialPort serialPort1;
        //private GLCD glcd1;
        private System.Windows.Forms.Button btnLeft;
        private System.Windows.Forms.Button btnUp;
        private System.Windows.Forms.Button btnDown;
        private System.Windows.Forms.Button btnRight;
        private System.Windows.Forms.Button btnClear;
        private System.Windows.Forms.TextBox textBox5;
        private System.Windows.Forms.TextBox tbxAlt;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.Label label12;
        private GLCD glcd1;
        private ServoPanel servoPanel1;
        private System.Windows.Forms.Button btnS;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.Timer mavTimer;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button btnConnMav;
        private System.Windows.Forms.ComboBox cbMavPort;
        private System.IO.Ports.SerialPort serialPort2;
        private System.Windows.Forms.Button btnUpdatePos;
        private System.Windows.Forms.Button btnPlayback;
        private System.Windows.Forms.Button btnOpenTlog;
        
    }
}

