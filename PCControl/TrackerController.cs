using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;
using System.Threading;
using ArdupilotMega;
using System.IO;

namespace MegaTracker
{
    public partial class TrackerController : Form
    {
        ControlLink clink = new ControlLink();
        OSDPage osd = new OSDPage();
        ServoPage srv = new ServoPage();
        MAVLink mavlink = new MAVLink();

        BinaryReader playbackfile = null;

        int lon, lat, alt;

        public TrackerController()
        {
            InitializeComponent();

            servoPanel1.Link = clink;
            clink.Timeout = 3000;
            clink.MessageReceived += OnCtrlMsgReceived;
            serialPort2.DataReceived += serialPort2_DataReceived;
        }

        void serialPort2_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            Console.Write(serialPort2.ReadExisting());
            //throw new NotImplementedException();
        }

        private void tabPage2_Click(object sender, EventArgs e)
        {

        }

        private void label5_Click(object sender, EventArgs e)
        {

        }

        private void tbPanCen_Scroll(object sender, EventArgs e)
        {

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }





        private void button5_Click(object sender, EventArgs e)
        {

        }

        private void button7_Click(object sender, EventArgs e)
        {

        }

        private void tabPage1_Click(object sender, EventArgs e)
        {

        }

        private void cbPort_DropDown(object sender, EventArgs e)
        {
            string text = cbPort.Text;
            cbPort.Items.Clear();
            cbPort.Items.AddRange(SerialPort.GetPortNames());
            if (cbPort.Items.IndexOf(text) >= 0)
                cbPort.Text = text;
        }



        private void TrackerController_Load(object sender, EventArgs e)
        {
            serialPort1.BaudRate = 115200;
            clink.Port = serialPort1;
            ServoPage sv = new ServoPage();
            //sv.paint(glcd1);
            osd.paint(glcd1);
            //FlyModePage fm = new FlyModePage();
            //fm.paint(glcd1);
            /*glcd1.DrawChar(5, 5, 0x81);
            glcd1.DrawChar(10, 10, (byte)'>');
            glcd1.DrawChar(10, 20, 0xf0);
             */
            //glcd1.DrawChar(0, 0, 0x80);
            //srv.paint(glcd1);
            /*for (int i = 0; i < 0x10; i++)
                for (int j = 0; j < 6; j++)
                    glcd1.DrawChar(i * 6, j * 8, (byte)(i + j * 0x10 + 0x20));*/
        }

        private void btnConnect_Click(object sender, EventArgs e)
        {
            if (serialPort1.IsOpen)
            {
                serialPort1.Close();
                btnConnect.Text = "连接";
            }
            else
            {
                serialPort1.PortName = cbPort.Text;
                serialPort1.BaudRate = 115200;
                try
                {
                    serialPort1.Open();
                    //serialPort1.DtrEnable = true;
                    serialPort1.DtrEnable = true;
                    //Thread.Sleep(100);
                    serialPort1.DtrEnable = false;
                    btnConnect.Text = "断开";
                    Console.WriteLine("DTR Enable");

                    /*
                    for (int i = 0; i < 3; i++)
                    {
                        clink.SendMessage(CommandType.DEV_READY, new byte[] { });
                        Thread.Sleep(50);
                    }*/
                    Console.WriteLine("Conn Done");
                }
                catch
                {
                    MessageBox.Show("打开端口" + cbPort.Text + "失败", "错误");
                }
            }
        }

        private void textBox5_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox6_TextChanged(object sender, EventArgs e)
        {

        }

        private void label6_Click(object sender, EventArgs e)
        {

        }

        private void radioButton8_CheckedChanged(object sender, EventArgs e)
        {
            System.Threading.Timer t = new System.Threading.Timer(null, null, 0, 0);
            //t.
        }


        private void OnCtrlMsgReceived(object sender, ControlLink.MessageReceivedEventArgs arg)
        {
            //GLCD glcd1 = new GLCD();
            //if (arg.msg.type == (byte)MessageType.MT_CMD)
            {
                Invoke(new MethodInvoker(delegate
                    {
                        switch ((CommandType)arg.msg.cmd)
                        {
                            case CommandType.LCD_CLEAR_SCREEN:
                                glcd1.ClearScreen();
                                break;
                            case CommandType.LCD_DRAW_LINE:
                                {
                                    DrawLineMsgData d = clink.FromBytes<DrawLineMsgData>(arg.msg.data);
                                    glcd1.DrawLine(d.x1, d.y1, d.x2, d.y2);
                                }
                                break;
                            case CommandType.LCD_DRAW_RECT:
                                {
                                    DrawRectMsgData d = clink.FromBytes<DrawRectMsgData>(arg.msg.data);
                                    glcd1.DrawRectange(d.x, d.y, d.width, d.height);
                                }
                                break;
                            case CommandType.LCD_FILL_RECT:
                                {
                                    FillRectMsgData d = clink.FromBytes<FillRectMsgData>(arg.msg.data);
                                    glcd1.FillRectange(d.x, d.y, d.width, d.height, d.color == 0 ? Color.White : Color.Blue);
                                }
                                break;
                            case CommandType.LCD_DRAW_STRING:
                                {
                                    DrawStringMsgData d = clink.FromBytes<DrawStringMsgData>(arg.msg.data);
                                    unsafe
                                    {
                                        glcd1.DrawString(d.x, d.y, d.str);

                                        Console.Write("{0},{1},", d.x, d.y);
                                        for (byte* b = d.str; *b != 0; b++)
                                            Console.Write((char)*b);
                                        Console.WriteLine();
                                    }
                                }
                                break;
                            case CommandType.LCD_DRAW_HPARALLEL:
                                {
                                    DrawHParallelMsgData d = clink.FromBytes<DrawHParallelMsgData>(arg.msg.data);
                                    glcd1.DrawHParallel(d.x, d.y, d.width, d.interval, d.count);
                                }
                                break;
                            case CommandType.LCD_DRAW_VPARALLEL:
                                {
                                    DrawVParallelMsgData d = clink.FromBytes<DrawVParallelMsgData>(arg.msg.data);
                                    glcd1.DrawVParallel(d.x, d.y, d.height, d.interval, d.count);
                                }
                                break;
                            case CommandType.LCD_DRAW_VLINE:
                                {
                                    DrawVLineMsgData d = clink.FromBytes<DrawVLineMsgData>(arg.msg.data);
                                    glcd1.DrawVLine(d.x, d.y, d.height);
                                }
                                break;
                            case CommandType.LCD_DRAW_HLINE:
                                {
                                    DrawHLineMsgData d = clink.FromBytes<DrawHLineMsgData>(arg.msg.data);
                                    glcd1.DrawHLine(d.x, d.y, d.width);
                                }
                                break;


                        }
                        glcd1.Invalidate();
                    }));
            }
        }

        private void tabControl1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (tabControl1.SelectedTab == tabPageServo)
            {
                //                if (serialPort1.IsOpen)
                if (clink.Call(CommandType.TRACKER_STOP, new byte[] { }) != null)
                    servoPanel1.LoadSetting();
            }
        }

        private void CtrlBtn_Click(object sender, EventArgs e)
        {
            //KEY_UP = 1,	KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_ENTER, KEY_S1, KEY_S2
            byte key_code = 0;
            if (object.ReferenceEquals(sender, btnUp))
                key_code = 1;
            else if (object.ReferenceEquals(sender, btnDown))
                key_code = 2;
            else if (object.ReferenceEquals(sender, btnLeft))
                key_code = 4;
            else if (object.ReferenceEquals(sender, btnRight))
                key_code = 8;
            //else if (object.ReferenceEquals(sender, btnEnter))
            //key_code = 0x10;
            else if (object.ReferenceEquals(sender, btnS))
                key_code = 0x20;
            else if (object.ReferenceEquals(sender, btnClear))
                key_code = 0x10;

            clink.SendMessage(CommandType.KEY_PRESS, new byte[] { key_code });
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            clink.SendMessage(CommandType.DEV_READY, new byte[] { });
        }

        private void btnPointTo_Click(object sender, EventArgs e)
        {
            if (mavTimer.Enabled)
            {
                mavTimer.Enabled = false;
                btnPointTo.Text = "对准";
                angle = -1;
            }
            else
            {
                mavTimer.Enabled = true;
                btnPointTo.Text = "停止";
            }
        }

        int angle = -1;
        bool mavbusy = false;
        private void mavTimer_Tick(object sender, EventArgs e)
        {
            if (mavbusy)
                return;

            mavbusy = true;
            mavlink.sysid = 7;
            mavlink.compid = 1;

            MAVLink.mavlink_heartbeat_t hb = new MAVLink.mavlink_heartbeat_t();
            hb.autopilot = (byte)(MAVLink.MAV_AUTOPILOT.ARDUPILOTMEGA);
            //hb.system_status = MAVLink.MAV
            mavlink.sendPacket(hb);
            MAVLink.mavlink_gps_raw_int_t g = new MAVLink.mavlink_gps_raw_int_t();


            lock (this)
            {
                if (angle >= 0)
                {
                    //   lat = (int)(5000 * Math.Cos(angle / 2 / Math.PI));
                    //   lon = (int)(5000 * Math.Sin(angle / 2 / Math.PI));
                    double lat2, lon2;
                    getDstPoint(0.0f, 0.0, 500, (angle * Math.PI / 180), out lat2, out lon2);
                    lat = (int)(lat2 * 1.0e7);
                    lon = (int)(lon2 * 1.0e7);

                    double a2 = getBearing(0.0, 0.0, lat2, lon2);
                    Console.WriteLine("angle {0} {1}", angle, a2);

                    angle += 2;
                    if (angle > 360)
                        angle -= 360;
                }
                else if (angle >= -5)
                {
                    lat = lon = 0;
                    angle--;
                }
                else
                    angle = 0;

                //   int.TryParse(tbxLat.Text, out lat);
                //   int.TryParse(tbxLon.Text, out lon);
                //   int.TryParse(tbxAlt.Text, out alt);

                g.lat = lat;
                g.lon = lon;
                g.alt = alt;
            }
            g.fix_type = 3;
            mavlink.sendPacket(g);

            MAVLink.mavlink_vfr_hud_t vfr = new MAVLink.mavlink_vfr_hud_t();
            vfr.airspeed = 10;
            vfr.groundspeed = 10;
            vfr.alt = alt / 1000.0f;
            vfr.heading = (short)angle;
            vfr.throttle = 10;
            mavlink.sendPacket(vfr);

            mavbusy = false;
            //if (serialPort2.BytesToRead > 0)
            //{
            //    //Console.WriteLine("mavlink read");
            //    Console.Write(serialPort2.ReadExisting());
            //}
        }

        private void cbMavPort_DropDown(object sender, EventArgs e)
        {
            string text = cbPort.Text;
            cbMavPort.Items.Clear();
            cbMavPort.Items.AddRange(SerialPort.GetPortNames());
            if (cbMavPort.Items.IndexOf(text) >= 0)
                cbMavPort.Text = text;
        }

        private void btnConnMav_Click(object sender, EventArgs e)
        {
            if (serialPort2.IsOpen)
            {
                mavTimer.Enabled = false;

                serialPort2.Close();
                btnConnMav.Text = "连接";

                if (playback_timer != null)
                    playback_timer.Change(Timeout.Infinite, Timeout.Infinite);

                btnPlayback.Enabled = false;
                btnPointTo.Enabled = false;

                mavlink.Close();
            }
            else
            {
                serialPort2.PortName = cbMavPort.Text;
                serialPort2.BaudRate = 115200;//57600;
                try
                {
                    //serialPort2.DtrEnable = false;
                    
                    

                    serialPort2.Open();
                    btnConnMav.Text = "断开";

                    serialPort2.DtrEnable = false;
                    Thread.Sleep(100);
                    serialPort2.DtrEnable = true;

                    mavlink.BaseStream = serialPort2;
                    mavlink.Open();

                    btnPointTo.Enabled = true;
                    if (playbackfile != null)
                        btnPlayback.Enabled = true;
                }
                catch
                {
                    MessageBox.Show("打开端口" + cbMavPort.Text + "失败", "错误");
                }
            }
        }

        private void btnUpdatePos_Click(object sender, EventArgs e)
        {
            lock (this)
            {
                lat = (int)(double.Parse(tbxLat.Text) * 1.0e7);
                lon = (int)(double.Parse(tbxLon.Text) * 1.0e7);
                alt = (int)(double.Parse(tbxAlt.Text) * 1.0e3);
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {

        }

        private void btnPlayback_Click(object sender, EventArgs e)
        {
            var dlg = new OpenFileDialog();
            if (playbackfile != null)
            {
                playbacktime = DateTime.Now;
                //playbackTimer.Enabled = true;
                
                
                playback_timer =  new System.Threading.Timer(playback_thread);
                playback_timer.Change(0, 100);
                /*
                DateTime d;
                byte[] packet;
                StreamWriter sw = new StreamWriter("log.txt");
                try
                {
                    while (true)
                    {

                        MAVLink.readlogPacketMavlink(playback, out packet, out d);

                        if (packet[3] == MAVLink.MAVLINK_MSG_ID_GPS_RAW_INT)
                        {
                            var gps = MavlinkUtil.ByteArrayToStructure<MAVLink.mavlink_gps_raw_int_t>(packet);
                            if (gps.fix_type >= 2)
                            {
                                Console.WriteLine("{0}\t{1}\t{2}\t{3}", d, gps.lat, gps.lon, gps.alt);
                                sw.WriteLine("{0}\t{1}\t{2}\t{3}", d, gps.lat, gps.lon, gps.alt);
                            }
                        }
                    }
                }
                catch (EndOfStreamException)
                {
                }
                playback.Close();
                sw.Close();*/
            }
        }


        DateTime playbacktime;
        DateTime logstarttime;
        DateTime playstarttime;
        System.Threading.Timer playback_timer;
        byte[] packet;


        private void playback_thread(object state)
        {
            while (DateTime.Now >= playbacktime)
            {
                if (packet != null && packet[3] == 1 && serialPort2.IsOpen)
                    serialPort2.Write(packet, 0, packet.Length);

                DateTime d;

                MAVLink.readlogPacketMavlink(playbackfile, out packet, out d);
                if (logstarttime.Year == 1)
                {
                    logstarttime = d;
                    playbacktime = playstarttime = DateTime.Now;
                }
                else
                {
                    var span = d - logstarttime;
                    if (span.TotalMinutes > 5)
                        continue;
                    else
                        playbacktime = playstarttime + new TimeSpan(span.Ticks / 5);
                }

                /*if (packet != null)
                {
                    if (packet[3] == 1)
                    {
                        Console.Write("{0} ", packet[5]);
                        if (packet[5] == 0)
                        {
                            var hb = MavlinkUtil.ByteArrayToStructure<ArdupilotMega.MAVLink.mavlink_heartbeat_t>(packet);
                            Console.WriteLine("\r\nmode {0}", hb.custom_mode);
                        }
                    }
                }*/
            }
        }


        private void playbackTimer_Tick(object sender, EventArgs e)
        {
            
        }

        public static void getDstPoint(double lat1, double lon1, int distance, double bearing,
            out double lat2, out double lon2)
        {
            double R = 6371000.0;
            lat1 *= Math.PI / 180;
            lon1 *= Math.PI / 180;
            lat2 = Math.Asin(Math.Sin(lat1) * Math.Cos(distance / R)
                + Math.Cos(lat1) * Math.Sin(distance / R) * Math.Cos(bearing));
            lon2 = lon1 + Math.Atan2(Math.Sin(bearing) * Math.Sin(distance / R) * Math.Cos(lat1),
                Math.Cos(distance / R) - Math.Sin(lat1) * Math.Sin(lat2));

            lat2 *= 180 / Math.PI;
            lon2 *= 180 / Math.PI;
        }

        public double getBearing(double home_lat, double home_lon, double lat1, double lon1)
        {
            double rads = Math.Abs(home_lat) * 0.0174532925f;
            double scaleLongDown = Math.Cos(rads);
            double scaleLongUp = 1.0f / Math.Cos(rads);

            //DST to Home
            //rel_coord.x = (lat - home_lat) * 111319.5;
            //rel_coord.y = (lon - home_lon) * 111319.5 * scaleLongDown;
            //rel_coord.z = -alt;


            //dstlat = fabs(rel_coord.x);//fabs(home_lat - lat) * 111319.5;
            //dstlon = fabs(rel_coord.y);//fabs(home_lon - lon) * 111319.5 * scaleLongDown;
            //home_dist = (uint32_t)sqrt(dstlat * dstlat + dstlon * dstlon);


            //DIR to Home
            double dstlon = (home_lon - lon); //OffSet_X
            double dstlat = (home_lat - lat) * scaleLongUp; //OffSet Y
            double bearing = 90 + (Math.Atan2(dstlat, -dstlon) * 57.295775); //absolut home direction
            if (bearing < 0) bearing += 360;//normalization
            /*dir = bearing;

            bearing = bearing - 180;//absolut return direction
            if (bearing < 0) bearing += 360;//normalization
            bearing = bearing - heading;//relative home direction
            if (bearing < 0) bearing += 360; //normalization
            home_dir = bearing;
             */
            //        home_dir = round((float)(bearing/360.0f) * 16.0f) + 1;//array of arrows =)
            //        if(home_direction > 16) home_direction = 0;
            return bearing;
        }

        private void btnOpenTlog_Click(object sender, EventArgs e)
        {
            var dlg = new OpenFileDialog();
            if (dlg.ShowDialog() == DialogResult.OK)
            {
                playbackfile = new BinaryReader(new FileStream(dlg.FileName, FileMode.Open));

                if (serialPort2.IsOpen)
                    btnPlayback.Enabled = true;
            }
        }
    }
}
