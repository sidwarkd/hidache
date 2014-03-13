using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using HIDacheObjects;
using USBComm;
using Microsoft.Win32.SafeHandles;
using System.Runtime.InteropServices;

namespace HIDache
{
    public partial class MainForm : Form
    {
        private const int WM_DEVICECHANGE = 0x0219; // a change occurred with connected devices
        private const int DBT_DEVICEARRIVAL = 0x8000;  // system detected a new device
        private const int DBT_DEVICEQUERYREMOVE = 0x8001;  // wants to remove, may fail
        private const int DBT_DEVICEQUERYREMOVEFAILED = 0x8002;  // removal aborted
        private const int DBT_DEVICEREMOVEPENDING = 0x8003;  // about to remove, still avail.
        private const int DBT_DEVICEREMOVECOMPLETE = 0x8004;  // device is gone
        private const int DBT_DEVTYP_DEVICEINTERFACE = 0x00000005;  // device interface class

        public MainForm()
        {
            InitializeComponent();

            // Create a new HIDAcheControl control and add it to the form
            HidAcheDevice newDevice = new HidAcheDevice();
            HIDAcheControl hidCtrl = new HIDAcheControl();
            hidCtrl.Name = "hidCtrl";
            hidCtrl.HidAcheDevice = newDevice;
            newDevice.CheckConnectionStatus();
            this.flowLayoutPanel1.Controls.Add(hidCtrl);

            // Register for HID device notifications so we know when there has been a device
            // attached or removed
            USBComm.DeviceManagement.RegisterForHIDDeviceNotifications(this.Handle);
            

            // If a device is connected read in the configuration
            if (newDevice.IsConnected)
            {
                newDevice.ReadDevice();
            }       
        }

        // Because we registered for device notifications above we will recieve a message whenever any USB device
        // is attached or removed.  To handle that we override the WndProc function and check for messages that
        // apply to us.  I've found a defect with this function that I have not yet found a solution for.  The
        // final line of this function base.WndProc(ref m) passes the message along after we've handled it.  This
        // has proven to be a problem since the notification doesn't seem to be passed to other applications until
        // we are done here.  This is especially a problem since the function can be stalled waiting for the user
        // to click Yes or No on the MessageBox.  I have found that if this program is open and the prank device
        // is attached in prank mode that this message interception causes the prank keyboard mode to be sporadic at
        // best.  Since this program should never be open on the machine you intend to prank it isn't a show
        // stopping problem.  However, this program needs to be fixed since it may affect other USB devices 
        // attached to the programming computer. In summary, the prank device will not function properly if this
        // program is open on the computer that the device is attached to.  When I have a solution to this problem
        // I will post the updated code.
        protected override void WndProc(ref Message m)
        {
            switch (m.Msg)
            {
                case WM_DEVICECHANGE:
                    // The WParam value identifies what is occurring.
                    // n = (int)m.WParam;
                    if ((HIDAcheControl)this.flowLayoutPanel1.Controls["hidCtrl"] != null)
                    {
                        // Update the connection status graphic and ask the user if they want to load the configuration if
                        // this change is a connection
                        ((HIDAcheControl)this.flowLayoutPanel1.Controls["hidCtrl"]).HidAcheDevice.CheckConnectionStatus();

                        if ((int)m.WParam == DBT_DEVICEARRIVAL)
                        {
                            HidAcheDevice dev = ((HIDAcheControl)this.flowLayoutPanel1.Controls["hidCtrl"]).HidAcheDevice;
                            if (dev.IsConnected)
                            {
                                if (MessageBox.Show(this, "A HIDAche device has been connected.  Would you like to read the configuration from the device?", "Device Connected", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes)
                                    dev.ReadDevice();
                            }
                        }
                    }
                    break;
            }
            base.WndProc(ref m);
        }
    }
}
