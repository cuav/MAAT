using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.IO.Ports;
using System.Runtime.InteropServices;
using System.Threading;
using System.Windows.Forms;

namespace MegaTracker
{

    public class ControlLink
    {
        //public 
        SerialPort port;
        ControlMessage msg = new ControlMessage();
        int msg_state = 0;
        int dlen = 0;
        byte checksum = 0;
        byte out_seq = 0;

        CommandType expected;
        byte expected_seq = 0;
        bool got_expected = false;

        public class MessageReceivedEventArgs : EventArgs
        {
            public ControlMessage msg;
            public MessageReceivedEventArgs(ControlMessage msg) { this.msg = msg; }
        }

        public SerialPort Port
        {
            get { return port; }
            set
            {
                if (port != null)
                    port.DataReceived -= OnDataReceived;
                if (value != null)
                    value.DataReceived += OnDataReceived;
                port = value;
            }
        }

        public int Timeout { get; set; }

        public void SendMessage(CommandType cmd, byte[] data)
        {
            if (port.IsOpen)
            {
                port.Write(new byte[] { (byte)'#', (byte)(data.Length + 2), out_seq, (byte)cmd }, 0, 4);
                byte checksum = (byte)(cmd + data.Length + 2 + out_seq);
                port.Write(data, 0, data.Length);
                foreach (var b in data)
                    checksum += b;
                port.Write(new byte[] { checksum }, 0, 1);
                out_seq++;
            }
        }

        /*
        public void SendAck()
        {
            if (port.IsOpen)
                port.Write(new byte[] { ACK_MARK }, 0, 1);
        }

        public void SendNAck()
        {
            if (port.IsOpen)
                port.Write(new byte[] { NACK_MARK }, 0, 1);
        }
         */

        public event EventHandler<MessageReceivedEventArgs> MessageReceived;

        void OnDataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            while (port.BytesToRead > 0)
            {
                byte ch = (byte)port.ReadByte();

                switch (msg_state)
                {
                    case 0:
                        if (ch == (byte)'#')
                        {
                            msg_state++;
                            checksum = 0;
                            dlen = 0;
                        }
                        else
                            Console.Write((char)ch);
                        break;
                    case 1:
                        if (ch < 2)
                        {
                            msg_state = 0;
                        }
                        else if (ch <= 66)
                        {
                            msg.len = (byte)(ch - 2);
                            checksum += ch;
                            msg_state++;
                        }
                        else
                        {
                            msg_state = 5;
                            msg.len = ch;
                        }
                        break;
                    case 2:
                        msg.seq = ch;
                        checksum += ch;
                        msg_state++;
                        break;
                    case 3:
                        msg.cmd = ch;
                        checksum += ch;
                        msg_state++;
                        break;
                    case 4:
                        if (dlen < msg.len)
                        {
                            msg.data[dlen++] = ch;
                            checksum += ch;
                        }
                        else
                        {
                            Console.WriteLine("msg recv " + ((CommandType)msg.cmd).ToString());
                            msg_state = 0;
                            msg.len += 2;
                            if (checksum == ch && MessageReceived != null)
                            {
                                MessageReceived(this, new MessageReceivedEventArgs(msg));
                            }
                            OnMessageReceived();
                        }
                        break;
                    case 5:
                        if (dlen++ == msg.len)
                        {
                            msg_state = 0;
                        }
                        break;
                }
            }

            //uart_putc('0' + msg_state);
        }


        public byte[] GetBytes(object str)
        {
            int size = Marshal.SizeOf(str);
            byte[] arr = new byte[size];
            IntPtr ptr = Marshal.AllocHGlobal(size);

            Marshal.StructureToPtr(str, ptr, true);
            Marshal.Copy(ptr, arr, 0, size);
            Marshal.FreeHGlobal(ptr);

            return arr;
        }

        public ControlMessage Call(CommandType cmd, byte[] data, CommandType expected = CommandType.CMD_ACK)
        {
            ControlMessage msg = null;

            if (Port.IsOpen)
            {

                got_expected = false;
                this.expected = expected;
                expected_seq = out_seq;
                SendMessage(cmd, data);

                DateTime timeout = DateTime.Now.AddMilliseconds(Timeout);
                while (!got_expected && DateTime.Now < timeout)
                {
                    Thread.Sleep(100);
                    Application.DoEvents();
                }
                if (got_expected && DateTime.Now < timeout)
                    msg = this.msg;
            }
            return msg;
        }

        public void OnMessageReceived()
        {
            if (expected == CommandType.CMD_ACK)
            {
                Console.WriteLine("ACK SEQ {0} EXP {1}", msg.data[0], expected_seq);
                if ((msg.cmd == (byte)CommandType.CMD_ACK || msg.cmd == (byte)CommandType.CMD_NACK)
                 && msg.data[0] == expected_seq)
                    got_expected = true;
            }
            else if (msg.cmd == (byte)expected)
                got_expected = true;
        }

        public T FromBytes<T>(byte[] arr)
        {
            object obj = Activator.CreateInstance(typeof(T));
            int size = Marshal.SizeOf(obj);
            IntPtr ptr = Marshal.AllocHGlobal(size);
            Marshal.Copy(arr, 0, ptr, size);
            obj = Marshal.PtrToStructure(ptr, typeof(T));
            Marshal.FreeHGlobal(ptr);
            return (T)obj;
        }
    }
}
