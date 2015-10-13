using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace MegaTracker
{
    public partial class ServoPanel : UserControl
    {
        public ServoPanel()
        {
            InitializeComponent();
        }

        SetServoParamMsgData panSetting;
        SetServoParamMsgData tiltSetting;

        bool fromuser = true;

        ControlLink link;

        public ControlLink Link
        {
            get
            {
                return link;
            }
            set
            {
                //if (link != null)
                //{
                //    link.MessageReceived -= OnMessageReceived;
                //}
                link = value;
            }
        }

        /*
        public void OnMessageReceived(object sender, ControlLink.MessageReceivedEventArgs arg)
        {
            switch ((CommandType)arg.msg.cmd)
            {
                case CommandType:
                    {
                        SetServoParamMsgData d = link.FromBytes<SetServoParamMsgData>(arg.msg.data);
                        if (d.idx == 0)
                        {
                            panSetting = d;
                        }
                        else if (d.idx == 1)
                        {
                            tiltSetting = d;
                        }

                        fromuser = false;
                        Invoke(new MethodInvoker(delegate
                        {
                            ResetSetting(d);
                        }));
                        fromuser = true;

                        link.SendAck();
                        if (tiltSetting.cen == 0)
                            LoadSetting(1);
                    }
                    break;
            }
        }*/

        public void LoadSetting()
        {
            var msg = link.Call(CommandType.SERVO_GET_SETTING, new byte[] { 0 }, CommandType.SERVO_SET_SETTING);
            if (msg != null && msg.cmd == (byte)CommandType.SERVO_SET_SETTING)
            {
                fromuser = false;
                panSetting = link.FromBytes<SetServoParamMsgData>(msg.data);
                ResetSetting(panSetting);
                msg = link.Call(CommandType.SERVO_GET_SETTING, new byte[] { 1 }, CommandType.SERVO_SET_SETTING);
                if (msg.cmd == (byte)CommandType.SERVO_SET_SETTING)
                {
                    tiltSetting = link.FromBytes<SetServoParamMsgData>(msg.data);
                    ResetSetting(tiltSetting);
                }
                fromuser = true;
            }
            if (msg == null || msg.cmd != (byte)CommandType.SERVO_SET_SETTING)
                MessageBox.Show("获取设置失败");
        }

        public void ResetSetting(SetServoParamMsgData d)
        {
            if (d.idx == 0)
            {
                numPanCen.Value = d.cen;
                numPanMax.Value = d.max;
                numPanAcc.Value = d.acc;
            }
            else if (d.idx == 0)
            {
                numTiltCen.Value = d.cen;
                numTiltMin.Value = d.min;
                numTiltMax.Value = d.max;
                numTiltAcc.Value = d.acc;
                numTiltNinty.Value = d.nty;
            }
        }

        private void numPanCen_ValueChanged(object sender, EventArgs e)
        {
            numPanMax.Minimum = numPanCen.Value;
            if (fromuser)
                SetPanServo(false);
        }

        private void numPanMax_ValueChanged(object sender, EventArgs e)
        {
            numPanCen.Maximum = numPanMax.Value;
            if (fromuser)
                SetPanServo(false);
        }

        private void numPanAcc_ValueChanged(object sender, EventArgs e)
        {
            if (fromuser)
                SetPanServo(false);
        }

        private void numTiltCen_ValueChanged(object sender, EventArgs e)
        {
            numTiltMin.Maximum = numTiltCen.Value;
            numTiltMax.Minimum = numTiltCen.Value;

            if (fromuser)
                SetTiltServo(false);
        }

        private void numTiltMax_ValueChanged(object sender, EventArgs e)
        {
            numTiltCen.Maximum = numTiltMax.Value;
            if (fromuser)
                SetTiltServo(false);
        }

        private void numTiltMin_ValueChanged(object sender, EventArgs e)
        {
            numTiltCen.Minimum = numTiltMin.Value;
            if (fromuser)
                SetTiltServo(false);
        }

        private void numTiltNinty_ValueChanged(object sender, EventArgs e)
        {
            if (fromuser)
                SetTiltServo(false);
        }

        private void numTiltAcc_ValueChanged(object sender, EventArgs e)
        {
            if (fromuser)
                SetTiltServo(false);
        }

        private void cbTiltRev_CheckedChanged(object sender, EventArgs e)
        {
            if (fromuser)
                SetTiltServo(false);
        }

        private void cbPanRev_CheckedChanged(object sender, EventArgs e)
        {
            if (fromuser)
                SetPanServo(false);
        }

        private bool SetPanServo(bool save = false)
        {
            SetServoParamMsgData data = new SetServoParamMsgData();
            data.idx = 0;
            data.cen = (short)(numPanCen.Value);
            data.max = (short)(numPanMax.Value);
            data.min = (short)(2 * data.cen - data.max);

            data.nty = cbPanRev.Checked ? data.max : data.min;
            //data.nty = data.max;
            data.acc = (short)(numPanAcc.Value);
            //data.rev = (sbyte)(cbPanRev.Checked ? -1 : 1);
            data.save = (byte)(save ? 1 : 0);

            var msg = link.Call(CommandType.SERVO_SET_SETTING, link.GetBytes(data), CommandType.CMD_ACK);
            if (msg.cmd == (byte)CommandType.CMD_ACK)
            {

                if (testPanCen.Checked)
                {
                    SetServoAngleMsgData angle = new SetServoAngleMsgData();
                    msg = link.Call(CommandType.SERVO_SET_ANGLE, link.GetBytes(angle));
                }
                else if (testPanMax.Checked)
                {
                    SetServoPwmMsgData pwm = new SetServoPwmMsgData();
                    pwm.pwm = (short)(data.max / 2);
                    msg = link.Call(CommandType.SERVO_SET_PWM, link.GetBytes(pwm));
                }
                else if (testPanAcc.Checked)
                {

                }
            }

            return msg.cmd == (byte)CommandType.CMD_ACK;
        }

        private void SetTiltServo(bool save = false)
        {
            SetServoParamMsgData data = new SetServoParamMsgData();
            data.idx = 1;
            data.cen = (short)(numTiltCen.Value);
            data.max = (short)(numTiltMax.Value);
            data.min = (short)(numTiltMin.Value);
            data.nty = (short)(numTiltNinty.Value);
            data.acc = (short)(numTiltAcc.Value);
            //data.rev = (sbyte)(cbTiltRev.Checked ? -1 : 1);
            data.save = (byte)(save ? 1 : 0);

            var msg = link.Call(CommandType.SERVO_SET_SETTING, link.GetBytes(data), CommandType.CMD_ACK);
            if (msg.cmd == (byte)CommandType.CMD_ACK)
            {

                if (testTiltCen.Checked)
                {
                    SetServoAngleMsgData angle = new SetServoAngleMsgData();
                    angle.idx = 1;
                    msg = link.Call(CommandType.SERVO_SET_ANGLE, link.GetBytes(angle));
                }
                else if (testTiltMax.Checked)
                {
                    SetServoPwmMsgData pwm = new SetServoPwmMsgData();
                    pwm.idx = 1;
                    pwm.pwm = (short)(data.max);
                    msg = link.Call(CommandType.SERVO_SET_PWM, link.GetBytes(pwm));
                }
                else if (testTiltMin.Checked)
                {
                    SetServoPwmMsgData pwm = new SetServoPwmMsgData();
                    pwm.idx = 1;
                    pwm.pwm = (short)(data.min);
                    msg = link.Call(CommandType.SERVO_SET_PWM, link.GetBytes(pwm));
                }
                else if (testTiltNinty.Checked)
                {
                    SetServoAngleMsgData angle = new SetServoAngleMsgData();
                    angle.idx = 1;
                    angle.angle = 90;
                    msg = link.Call(CommandType.SERVO_SET_ANGLE, link.GetBytes(angle));
                }
                else if (testTiltAcc.Checked)
                {

                }
            }
        }

        private void btnReset_Click(object sender, EventArgs e)
        {
            if (panSetting.cen != 0)
            {
                ResetSetting(panSetting);
                SetPanServo(false);
            }
            if (tiltSetting.cen != 0)
            {
                ResetSetting(tiltSetting);
                SetTiltServo(false);
            }
        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            SetPanServo(true);
            SetTiltServo(true);
        }

        private void testPan_CheckChanged(object sender, EventArgs e)
        {
            CheckBox ck = sender as CheckBox;
            if (ck.Checked)
            {
                foreach (var c in new CheckBox[] { testPanCen, testPanMax, testPanAcc })
                    if (!object.ReferenceEquals(ck, c))
                        c.Checked = false;

                SetPanServo();
            }

        }

        private void testTilt_CheckedChanged(object sender, EventArgs e)
        {
            CheckBox ck = sender as CheckBox;
            if (ck.Checked)
            {
                foreach (var c in new CheckBox[] { testTiltCen, testTiltMax, testTiltMin, testTiltAcc, testTiltNinty })
                    if (!object.ReferenceEquals(ck, c))
                        c.Checked = false;

                SetTiltServo();
            }
        }
    }
}
