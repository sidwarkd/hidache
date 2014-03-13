using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using USBComm;

namespace HIDacheObjects
{
    public class HidAcheDevice : INotifyPropertyChanged
    {        
        internal const byte DEVICE_CODE = 0x02; // Used to verify that the attached device really is a HIDAche device
        internal const short VENDOR_ID = 0x04D8;
        internal const short PRODUCT_ID = 0x003C;
        private string version;
        private DeviceMode mode;
        private int interval;
        private MouseMode mouseBehavior;
        private string phrase;
        private bool isConnected;
        private HIDAcheEEPROM eeprom;
        private USBComm.HIDCommObject HIDComm;

        #region Public Members
        public string Version { get { return version; } }
        public DeviceMode Mode { get { return mode; } set { mode = value; this.OnPropertyChanged("Mode"); } }
        public int Interval { get { return interval; } set { interval = value; this.OnPropertyChanged("Interval"); } }
        public MouseMode MouseBehavior { get { return mouseBehavior; } set { mouseBehavior = value; this.OnPropertyChanged("MouseBehavior"); } }
        public string Phrase { get { return phrase; } set { phrase = value; this.OnPropertyChanged("Phrase"); } }
        public bool IsConnected { get { return isConnected; } private set { isConnected = value; this.OnPropertyChanged("IsConnected"); } }
        #endregion

        public HidAcheDevice()
        {
            version = "1.0";
            mode = DeviceMode.Mouse;
            interval = 5;
            mouseBehavior = MouseMode.CircleMotion;
            phrase = "HIDAche";
            isConnected = false;
            HIDComm = new HIDCommObject(PRODUCT_ID, VENDOR_ID);
            eeprom = new HIDAcheEEPROM();
        }

        public HidAcheDevice(DeviceMode deviceMode, int interval, MouseMode mouseMode, string phrase)
        {
            version = "1.0";
            mode = deviceMode;
            this.interval = interval;
            mouseBehavior = mouseMode;
            this.phrase = phrase;
            isConnected = false;
        }

        public override string ToString()
        {
            return String.Format("Version:{0}\nMode:{1}\nInterval:{2}\nMouseBehavior:{3}\nPhrase:{4}\nConnected:{5}", version, mode.ToString(), interval.ToString(), mouseBehavior.ToString(), phrase, isConnected.ToString());
        }

        #region USB Communication

        // The device info is arranged as follows.  The address in EEPROM is given for each.
        // Device Code - 0x01
        // Prank Interval - 0x0A
        // Device Mode - 0x0B
        // Mouse Mode - 0x0C
        // Phrase Length - 0x0D
        // Phrase - 0x0E - 0xFF

        public bool ReadDevice()
        {
            // The EEPROM on the 18F2455 is 256 bytes.  The maximum packet size for
            // transfers is 64 bytes and that includes the overhead of the command.
            // The packet is in the following format:
            // private byte reportID;
            // private byte command;
            // private UInt16 address;
            // private byte dataSize;
            // private byte[] data = new byte[58];
            //
            // The data for the HIDAche device starts at address 0x0A.  This leaves
            // room at the front of EEPROM memory for device data to be stored if we
            // have any.  I like to put a unique device code in the first two bytes of
            // EEPROM so we can verify that we are about to program the correct device.
            // We'll just read in the entire contents of the EEPROM and sort out the 
            // info on the software side.  For this we use the HIDAcheEEPROM object;

            // Build the write command.  0x00F00000 is the starting address.  The F0 tells the bootloader code
            // that we are going to read from EEPROM and the 00 at the end says we will start at address
            // 0x00.  We will reuse this DeviceCommand for all of the transmissions.
            USBComm.DeviceCommand commandPacket = new DeviceCommand((byte)HIDAcheCommand.ReadEEPROM, (Int32)0x00F00000, 50, new byte[58]);
            
            // Read the first 250 bytes
            for (int i = 0; i < 5; i++)
            {
                if (HIDComm.SendCommandAndGetResponseFromDevice(ref commandPacket))
                {
                    Array.Copy(commandPacket.Data, 0, eeprom.Contents, (byte)commandPacket.Address, 50);
                    commandPacket.Address += 50;
                }
                else // command failed
                {
                    System.Diagnostics.Debug.Fail("SendCommandAndGetResponseFromDevice failed in the ReadDevice() function");
                    return false;
                }
            }

            // Read the last 6 bytes
            commandPacket.DataSize = 6;
            if (HIDComm.SendCommandAndGetResponseFromDevice(ref commandPacket))
            {
                Array.Copy(commandPacket.Data, 0, eeprom.Contents, (byte)commandPacket.Address, 6);
            }
            else // command failed
            {
                System.Diagnostics.Debug.Fail("SendCommandAndGetResponseFromDevice failed in the ReadDevice() function");
                return false;
            }

            // Check to make sure we are reading a HIDAche device before continuing
            if (eeprom.DeviceCode == DEVICE_CODE)
            {
                this.Interval = eeprom.Interval;
                this.Mode = (DeviceMode)eeprom.Mode;
                this.MouseBehavior = (MouseMode)eeprom.MouseBehavior;
                this.Phrase = eeprom.Phrase;
            }
            else
            {
                System.Diagnostics.Debug.Fail("SendCommandAndGetResponseFromDevice failed in the ReadDevice() function.  The device code does not match that expected.  Verify that a HIDAche device has been connected.");
                return false;
            }
            
            return true;
            
        }

        // Our HIDAche structure will be anywhere from 4 bytes to 245 bytes
        // depending on mode settings.  We'll determine the size here and
        // write out the information accordingly.  The Send routine will
        // send 64 bytes regardless of the size so we will just zero pad
        // the rest.
        public bool UpdateDevice()
        {
            int bytesRemaining = 0;
            byte[] phraseBytes = ConvertStringToByteArray(phrase);
            
            // Calculate the number of bytes we need to send.  We will always
            // send the top 4 bytes plus the phrase length.
            bytesRemaining = 4 + phraseBytes.Length;
            
            // Build the write command.  0x00F0000A is the starting address.  The F0 tells the bootloader code
            // that we are going to write to EEPROM and the 0A at the end says we will start at address
            // 0x0A.  We will reuse this DeviceCommand for all of the transmissions.
            USBComm.DeviceCommand commandPacket = new DeviceCommand((byte)HIDAcheCommand.WriteEEPROM, (Int32)0x00F0000A, 4, new byte[58]);

            // Send the basic information first.  Then we'll send the phrase if one exists.
            commandPacket.Data[54] = (byte)this.interval;
            commandPacket.Data[55] = (byte)this.mode;
            commandPacket.Data[56] = (byte)this.mouseBehavior;
            commandPacket.Data[57] = (byte)this.phrase.Length;

            if (HIDComm.SendCommandToDevice(commandPacket))
            {
                // Increase the address
                commandPacket.Address += 4;
                bytesRemaining -= 4;
            }
            else // The transmission failed so there is something wrong
            {
                System.Diagnostics.Debug.Fail("SendCommandToDevice failed in the UpdateDevice() method");
                return false;
            }

            while (bytesRemaining != 0)
            {
                // Put the data in the array and send it.
                if (bytesRemaining < 50)
                {
                    commandPacket.DataSize = (byte)bytesRemaining;
                    Array.Copy(phraseBytes, phraseBytes.Length - bytesRemaining, commandPacket.Data, commandPacket.Data.Length - bytesRemaining, bytesRemaining);
                    if (HIDComm.SendCommandToDevice(commandPacket))
                    {
                        // Increase the address
                        commandPacket.Address += (uint)bytesRemaining;
                        bytesRemaining -= bytesRemaining;
                    }
                    else // The transmission failed so there is something wrong
                    {
                        System.Diagnostics.Debug.Fail("SendCommandToDevice failed in the UpdateDevice() method");
                        return false;
                    }
                }
                else
                {
                    commandPacket.DataSize = 50;
                    Array.Copy(phraseBytes, phraseBytes.Length - bytesRemaining, commandPacket.Data, 8, 50);
                    if (HIDComm.SendCommandToDevice(commandPacket))
                    {
                        // Increase the address
                        commandPacket.Address += 50;
                        bytesRemaining -= 50;
                    }
                    else // The transmission failed so there is something wrong
                    {
                        System.Diagnostics.Debug.Fail("SendCommandToDevice failed in the UpdateDevice() method");
                        return false;
                    }
                }
            }
            
            return true;
        }

        // CheckConnectionStatus updates the IsConnected member of the device object.  This member
        // can be bound to the UI to indicate whether the device is connected or not.
        public void CheckConnectionStatus()
        {
            IsConnected = HIDComm.IsHIDConnected();
        }

        private byte[] ConvertStringToByteArray(string stringToConvert)
        {
            System.Text.ASCIIEncoding encoding = new System.Text.ASCIIEncoding();
            byte[] chars = encoding.GetBytes(stringToConvert);
            return chars;            
        }
        
        private enum HIDAcheCommand : byte
        {
            ReadEEPROM =    0x07,
            WriteEEPROM =   0x05
        }
        #endregion

        #region HidAche Enums
        public enum DeviceMode : byte
        {
            Mouse = 0x00,
            Keyboard,
            KeyboardAndMouse
        }

        public enum MouseMode : byte
        {
            CircleMotion = 0x00,
            MoveToBottomLeft,
            MoveToTopRight,
            MoveUp,
            MoveDown,
            MoveLeft,
            MoveRight,
            RandomJump,
            SideToSide
        }
        #endregion

        #region INotifyPropertyChanged Members

        public event PropertyChangedEventHandler PropertyChanged;

        protected virtual void OnPropertyChanged(string propertyName)
        {
            PropertyChangedEventHandler handler = this.PropertyChanged;
            if (handler != null)
                handler(this, new PropertyChangedEventArgs(propertyName));
        }

        #endregion

    }

    internal class HIDAcheEEPROM
    {
        public byte[] Contents { get; set; }
        public byte DeviceCode { get { return Contents[0x00]; } }
        public int Interval { get{ return (int)Contents[0x0A]; } }
        public byte Mode { get{ return Contents[0x0B];} }
        public byte MouseBehavior { get{ return Contents[0x0C]; } }
        public int PhraseLength { get{ return (int)Contents[0x0D]; } }
        public string Phrase { get{ 
                                byte[] phraseBytes = new byte[PhraseLength]; 
                                Array.Copy(Contents, 0x0E, phraseBytes, 0, PhraseLength);
                                return ConvertByteArrayToString(phraseBytes);
                          }}

        public HIDAcheEEPROM()
        {
            Contents = new byte[256];
        }

        private string ConvertByteArrayToString(byte[] bytes)
        {
            System.Text.ASCIIEncoding enc = new System.Text.ASCIIEncoding();
            return enc.GetString(bytes);
        }
    }
}
