using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace USBComm
{
    public class DeviceCommand
    {
        //private byte reserved;  // This is the report ID
        //private byte command;
        //private UInt16 address;
        //private byte dataSize;
        //private byte[] data = new byte[58];
        public DeviceCommand(byte command, UInt32 address, byte size, byte[] data)
        {
            Data = new byte[58];
            Command = command;
            Address = address;
            DataSize = size;
            // Only copy the first 58 bytes of data into the Data member.  If there
            // are less bytes then copy everything.
            if (data.Length < 58)
                Array.Copy(data, 0, Data, 0, data.Length);
            else
                Array.Copy(data, 0, Data, 0, 58);
        }

        // Generic constructor
        public DeviceCommand() { }

        // Transforms a DeviceCommand into a byte[] to be sent to the 
        // device.  It will always return an array of length 65.  64
        // bytes for the actual command and 1 byte for the report ID.
        public byte[] GetByteArray()
        {
            byte[] returnArray = new byte[65];
            returnArray[0] = ReportID;
            returnArray[1] = Command;
            returnArray[2] = (byte)Address; // Bits 0-7
            returnArray[3] = (byte)(Address >> 8); // Bits 8-15
            returnArray[4] = (byte)(Address >> 16); // Bits 16-23
            returnArray[5] = (byte)(Address >> 24); // Bits 24-31
            returnArray[6] = DataSize;
            Array.Copy(Data, 0, returnArray, 7, 58);
            return returnArray;
        }


        public byte ReportID { get; set; } // This is the report ID if you have one.  Otherwise, set it to 0.
        public byte Command { get; set; }
        public UInt32 Address { get; set; }
        public byte DataSize { get; set; }
        public byte[] Data { get; set; }
    }
}