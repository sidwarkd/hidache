using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using Microsoft.Win32.SafeHandles;

namespace USBComm
{
    public class HIDCommObject
    {
        private string devicePathName;
        private short productID;
        private short vendorID;
        private SafeFileHandle _HIDHandle;

        public HIDCommObject(short productID, short vendorID)
        {
            this.productID = productID;
            this.vendorID = vendorID;
            devicePathName = String.Empty;
            IsHIDConnected();  // This call sets the devicePathName.  Not the best way to do this but works for now.
        }
        
        public bool IsHIDConnected()
        {

            // Purpose    : Uses a series of API calls to locate a HID-class device
            //            ; by its Vendor ID and Product ID.
            // Returns    : True if the device is detected, False if not detected.

            bool FoundAttachedDevices = false;
            bool foundDevice = false;
            string[] DevicePathName = new string[128];
            System.Guid HidGuid;
            int MemberIndex = 0;
            bool Result = false;
            
            FileIO.SECURITY_ATTRIBUTES Security = new USBComm.FileIO.SECURITY_ATTRIBUTES();
            Hid tempHid = new Hid();

            try
            {

                HidGuid = Guid.Empty;
                foundDevice = false;

                // Values for the SECURITY_ATTRIBUTES structure:
                Security.lpSecurityDescriptor = 0;
                Security.bInheritHandle = System.Convert.ToInt32(true);
                Security.nLength = Marshal.SizeOf(Security);

                /*
                  API function: 'HidD_GetHidGuid
                  Purpose: Retrieves the interface class GUID for the HID class.
                  Accepts: 'A System.Guid object for storing the GUID.
                  */

                Hid.HidD_GetHidGuid(ref HidGuid);

                // Fill an array with the device path names of all attached HIDs.
                FoundAttachedDevices = DeviceManagement.FindDeviceFromGuid(HidGuid, ref DevicePathName);
                

                // If there is at least one HID, attempt to read the Vendor ID and Product ID
                // of each device until there is a match or all devices have been examined.

                if (FoundAttachedDevices)
                {
                    MemberIndex = 0;
                    do
                    {
                        // ***
                        // API function:
                        // CreateFile
                        // Purpose:
                        // Retrieves a handle to a device.
                        // Accepts:
                        // A device path name returned by SetupDiGetDeviceInterfaceDetail
                        // The type of access requested (read/write).
                        // FILE_SHARE attributes to allow other processes to access the device while this handle is open.
                        // A Security structure. Using Null for this may cause problems under Windows XP.
                        // A creation disposition value. Use OPEN_EXISTING for devices.
                        // Flags and attributes for files. Not used for devices.
                        // Handle to a template file. Not used.
                        // Returns: a handle that enables reading and writing to the device.
                        // ***

                        //GRV - this works and is a true copy of Axelson
                        _HIDHandle = FileIO.CreateFile
                                    (DevicePathName[MemberIndex],
                                     0,
                                     FileIO.FILE_SHARE_READ | FileIO.FILE_SHARE_WRITE,
                                     IntPtr.Zero,
                                     FileIO.OPEN_EXISTING,
                                     0,
                                     0);

                        if (!_HIDHandle.IsInvalid)
                        {

                            // The returned handle is valid,
                            // so find out if this is the device we're looking for.

                            // Set the Size property of DeviceAttributes to the number of bytes in the structure.
                            //tempHid.DeviceAttributes.Size = tempHid.DeviceAttributes.ToString().Length;
                            tempHid.DeviceAttributes.Size = Marshal.SizeOf(tempHid.DeviceAttributes);

                            // ***
                            // API function:
                            // HidD_GetAttributes
                            // Purpose:
                            // Retrieves a HIDD_ATTRIBUTES structure containing the Vendor ID,
                            // Product ID, and Product Version Number for a device.
                            // Accepts:
                            // A handle returned by CreateFile.
                            // A pointer to receive a HIDD_ATTRIBUTES structure.
                            // Returns:
                            // True on success, False on failure.
                            // ***

                            Result = Hid.HidD_GetAttributes(_HIDHandle, ref tempHid.DeviceAttributes);

                            if (Result)
                            {
                                // Find out if the device matches the one we're looking for.
                                if ((tempHid.DeviceAttributes.VendorID == vendorID) &
                                    (tempHid.DeviceAttributes.ProductID == productID))
                                {
                                    // Save the DevicePathName so OnDeviceChange() knows which name is my device.
                                    devicePathName = DevicePathName[MemberIndex];
                                    return true; 
                                }
                                else
                                {
                                    foundDevice = false;
                                }
                            }
                            else
                            {
                                // There was a problem in retrieving the information.
                                foundDevice = false;
                            }                            
                        }

                        // Keep looking until we find the device or there are no more left to examine.
                        MemberIndex = MemberIndex + 1;

                    } while (!((foundDevice == true) | (MemberIndex == DevicePathName.Length))); //GRV - +1 deleted per Axelson
                }// end if(DeviceFound)
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.Fail(ex.Message);
            }
            return foundDevice;
        }

        // This function will simply send a command to the device without expecting a result
        public bool SendCommandToDevice(DeviceCommand command)
        {
            SafeFileHandle _HIDWriteHandle;
            bool result = false;
            Hid.OutputReportViaInterruptTransfer report = new Hid.OutputReportViaInterruptTransfer();
            FileIO.SECURITY_ATTRIBUTES Security = new USBComm.FileIO.SECURITY_ATTRIBUTES();
            if (devicePathName != String.Empty) // We have a valid device path to use in CreateFile
            {
                _HIDWriteHandle = FileIO.CreateFile
                                        (devicePathName,
                                         FileIO.GENERIC_WRITE,
                                         FileIO.FILE_SHARE_READ | FileIO.FILE_SHARE_WRITE,
                                         IntPtr.Zero,
                                         FileIO.OPEN_EXISTING,
                                         0,
                                         0);

                if (!_HIDWriteHandle.IsInvalid)
                {
                    try
                    {
                        result = report.Write(command.GetByteArray(), _HIDWriteHandle);
                    }
                    finally
                    {
                        // Close the open handle
                        _HIDWriteHandle.Close();
                    }
                }
                else //Invalid handle returned from CreateFile
                {
                    return false;
                }
            }
            else  // We don't have a valid device path.  The device is probably not connected
            {
                return false;
            }
            return true;
        }

        // This function will send a command to the device and retrieve the response
        public bool SendCommandAndGetResponseFromDevice(ref DeviceCommand command)
        {
            SafeFileHandle _HIDReadHandle, _HIDWriteHandle;
            bool result = false;
            bool foundMyDevice = false;
            Hid.OutputReportViaInterruptTransfer outreport = new Hid.OutputReportViaInterruptTransfer();
            Hid.InputReportViaInterruptTransfer report = new Hid.InputReportViaInterruptTransfer();
            FileIO.SECURITY_ATTRIBUTES Security = new USBComm.FileIO.SECURITY_ATTRIBUTES();
            if (devicePathName != String.Empty) // We have a valid device path to use in CreateFile
            {
                _HIDReadHandle = FileIO.CreateFile
                                        (devicePathName,
                                         FileIO.GENERIC_READ,
                                         FileIO.FILE_SHARE_READ | FileIO.FILE_SHARE_WRITE,
                                         IntPtr.Zero,
                                         FileIO.OPEN_EXISTING,
                                         FileIO.FILE_FLAG_OVERLAPPED,
                                         0);

                _HIDWriteHandle = FileIO.CreateFile
                                        (devicePathName,
                                         FileIO.GENERIC_WRITE,
                                         FileIO.FILE_SHARE_READ | FileIO.FILE_SHARE_WRITE,
                                         IntPtr.Zero,
                                         FileIO.OPEN_EXISTING,
                                         0,
                                         0);

                if (!_HIDReadHandle.IsInvalid && !_HIDWriteHandle.IsInvalid)
                {
                    try
                    {
                        Hid hidDevice = new Hid();
                        IntPtr ppData = (IntPtr)(-1);

                        // Get the input report size
                        Hid.HidD_GetPreparsedData(_HIDHandle, ref ppData);
                        Hid.HidP_GetCaps(ppData, ref hidDevice.Capabilities);
                        Hid.HidD_FreePreparsedData(ppData);

                        byte[] readBuffer = new Byte[hidDevice.Capabilities.OutputReportByteLength];
                        outreport.Write(command.GetByteArray(), _HIDWriteHandle);
                        report.Read(_HIDHandle, _HIDReadHandle, _HIDWriteHandle, ref foundMyDevice, ref readBuffer, ref result);
                        Array.Copy(readBuffer, readBuffer.Length - command.DataSize, command.Data, 0, command.DataSize);
                    }
                    finally
                    {
                        // Close the open handles
                        _HIDReadHandle.Close();
                        _HIDWriteHandle.Close();
                    }
                }
                else //Invalid handle returned from CreateFile
                {
                    return false;
                }
            }
            else  // We don't have a valid device path.  The device is probably not connected
            {
                return false;
            }
            return true;
        }
    }
}
