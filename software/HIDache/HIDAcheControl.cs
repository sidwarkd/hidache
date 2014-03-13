using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using HIDacheObjects;

namespace HIDache
{
    public partial class HIDAcheControl : UserControl
    { 
        public HIDAcheControl()
        {
            InitializeComponent();

            Binding connectionBinding = this.lblIsConnected.DataBindings[0];
            connectionBinding.Format += this.ConvertIsConnectedToString;

            // Add options for the device
            rgDeviceMode.AddRadioButton("Mouse", HIDacheObjects.HidAcheDevice.DeviceMode.Mouse);
            rgDeviceMode.AddRadioButton("Keyboard", HIDacheObjects.HidAcheDevice.DeviceMode.Keyboard);
            rgDeviceMode.AddRadioButton("Both", HIDacheObjects.HidAcheDevice.DeviceMode.KeyboardAndMouse);
            rgDeviceMode.StateChanged += new RadioButtonGroup.StateChangedEventHandler(ModeChange);

            // Add options for mouse mode
            rgMouseMode.AddRadioButton("CircleMotion", HIDacheObjects.HidAcheDevice.MouseMode.CircleMotion);
            rgMouseMode.AddRadioButton("Bottom Left", HIDacheObjects.HidAcheDevice.MouseMode.MoveToBottomLeft);
            rgMouseMode.AddRadioButton("Top Right", HIDacheObjects.HidAcheDevice.MouseMode.MoveToTopRight);
            rgMouseMode.AddRadioButton("Move Up", HIDacheObjects.HidAcheDevice.MouseMode.MoveUp);
            rgMouseMode.AddRadioButton("Move Down", HIDacheObjects.HidAcheDevice.MouseMode.MoveDown);
            rgMouseMode.AddRadioButton("Move Left", HIDacheObjects.HidAcheDevice.MouseMode.MoveLeft);
            rgMouseMode.AddRadioButton("Move Right", HIDacheObjects.HidAcheDevice.MouseMode.MoveRight);
            rgMouseMode.AddRadioButton("Random", HIDacheObjects.HidAcheDevice.MouseMode.RandomJump);
            rgMouseMode.AddRadioButton("Side To Side", HIDacheObjects.HidAcheDevice.MouseMode.SideToSide);
        }

        // This allows us to bind a specific phrase to the device connection status
        private void ConvertIsConnectedToString(object sender, ConvertEventArgs e)
        {
            if ((bool)e.Value)
            {
                e.Value = "HIDAche Device Connected";
                pbStatus.Image = HIDache.Properties.Resources.Connected;
            }
            else
            {
                e.Value = "HIDAche Device Not Connected";
                pbStatus.Image = HIDache.Properties.Resources.Disconnected;
            }
        }

        // The object that we will bind the control to
        public HidAcheDevice HidAcheDevice
        {
            get { return this.hidAcheDeviceBindingSource.DataSource as HidAcheDevice; }
            set{ this.hidAcheDeviceBindingSource.DataSource = value; }
        }

        private void btnUpdate_Click(object sender, EventArgs e)
        {
            if (HidAcheDevice.IsConnected)
            {
                // This MessageBox can be used for debugging to see what is about to be sent
                // to the device.  Otherwise it can be left commented.
                //MessageBox.Show(HidAcheDevice.ToString());
                if (!HidAcheDevice.UpdateDevice())
                    MessageBox.Show("The update was not successful.  Please check the HIDAche hardware and try again.", "Update Failed", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            else
            {
                MessageBox.Show("It doesn't appear that you have a HIDAche device connected.  Check the connection and try again", "Device Not Connected", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        // Depending on what mode is selected we will gray out unavailable sections
        private void ModeChange(object sender, RadioButtonGroupEventArgs e)
        {
            switch ((HIDacheObjects.HidAcheDevice.DeviceMode)e.state)
            {
                case HIDacheObjects.HidAcheDevice.DeviceMode.Mouse:
                    gbKeyboardSettings.Enabled = false;
                    gbMouseSettings.Enabled = true;
                    break;
                case HIDacheObjects.HidAcheDevice.DeviceMode.Keyboard:
                    gbKeyboardSettings.Enabled = true;
                    gbMouseSettings.Enabled = false;
                    break;
                case HIDacheObjects.HidAcheDevice.DeviceMode.KeyboardAndMouse:
                    gbKeyboardSettings.Enabled = true;
                    gbMouseSettings.Enabled = true;
                    break;
            }
        }
    }
}
