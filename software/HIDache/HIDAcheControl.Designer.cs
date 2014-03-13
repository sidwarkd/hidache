namespace HIDache
{
    partial class HIDAcheControl
    {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.lblIsConnected = new System.Windows.Forms.Label();
            this.hidAcheDeviceBindingSource = new System.Windows.Forms.BindingSource(this.components);
            this.gbKeyboardSettings = new System.Windows.Forms.GroupBox();
            this.label1 = new System.Windows.Forms.Label();
            this.tbPhrase = new System.Windows.Forms.TextBox();
            this.gbMouseSettings = new System.Windows.Forms.GroupBox();
            this.rgMouseMode = new HIDache.RadioButtonGroup();
            this.gbDeviceMode = new System.Windows.Forms.GroupBox();
            this.rgDeviceMode = new HIDache.RadioButtonGroup();
            this.lblInterval = new System.Windows.Forms.Label();
            this.tbInterval = new System.Windows.Forms.TextBox();
            this.btnUpdate = new System.Windows.Forms.Button();
            this.pbStatus = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.hidAcheDeviceBindingSource)).BeginInit();
            this.gbKeyboardSettings.SuspendLayout();
            this.gbMouseSettings.SuspendLayout();
            this.gbDeviceMode.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pbStatus)).BeginInit();
            this.SuspendLayout();
            // 
            // lblIsConnected
            // 
            this.lblIsConnected.AutoSize = true;
            this.lblIsConnected.BackColor = System.Drawing.Color.Transparent;
            this.lblIsConnected.DataBindings.Add(new System.Windows.Forms.Binding("Text", this.hidAcheDeviceBindingSource, "IsConnected", true));
            this.lblIsConnected.Font = new System.Drawing.Font("Tahoma", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblIsConnected.ForeColor = System.Drawing.SystemColors.ControlText;
            this.lblIsConnected.Location = new System.Drawing.Point(53, 22);
            this.lblIsConnected.Name = "lblIsConnected";
            this.lblIsConnected.Size = new System.Drawing.Size(231, 29);
            this.lblIsConnected.TabIndex = 6;
            this.lblIsConnected.Text = "Connection Status";
            // 
            // hidAcheDeviceBindingSource
            // 
            this.hidAcheDeviceBindingSource.DataSource = typeof(HIDacheObjects.HidAcheDevice);
            // 
            // gbKeyboardSettings
            // 
            this.gbKeyboardSettings.BackColor = System.Drawing.Color.Transparent;
            this.gbKeyboardSettings.Controls.Add(this.label1);
            this.gbKeyboardSettings.Controls.Add(this.tbPhrase);
            this.gbKeyboardSettings.Font = new System.Drawing.Font("Tahoma", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.gbKeyboardSettings.ForeColor = System.Drawing.SystemColors.ControlText;
            this.gbKeyboardSettings.Location = new System.Drawing.Point(15, 238);
            this.gbKeyboardSettings.Name = "gbKeyboardSettings";
            this.gbKeyboardSettings.Padding = new System.Windows.Forms.Padding(10, 5, 10, 5);
            this.gbKeyboardSettings.Size = new System.Drawing.Size(585, 204);
            this.gbKeyboardSettings.TabIndex = 5;
            this.gbKeyboardSettings.TabStop = false;
            this.gbKeyboardSettings.Text = "Keyboard Settings";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Tahoma", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.ForeColor = System.Drawing.SystemColors.ControlText;
            this.label1.Location = new System.Drawing.Point(13, 21);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(311, 19);
            this.label1.TabIndex = 1;
            this.label1.Text = "Manually enter the phrase (240 char max):";
            // 
            // tbPhrase
            // 
            this.tbPhrase.DataBindings.Add(new System.Windows.Forms.Binding("Text", this.hidAcheDeviceBindingSource, "Phrase", true));
            this.tbPhrase.Location = new System.Drawing.Point(13, 50);
            this.tbPhrase.MaxLength = 240;
            this.tbPhrase.Multiline = true;
            this.tbPhrase.Name = "tbPhrase";
            this.tbPhrase.Size = new System.Drawing.Size(559, 134);
            this.tbPhrase.TabIndex = 0;
            // 
            // gbMouseSettings
            // 
            this.gbMouseSettings.BackColor = System.Drawing.Color.Transparent;
            this.gbMouseSettings.Controls.Add(this.rgMouseMode);
            this.gbMouseSettings.Font = new System.Drawing.Font("Tahoma", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.gbMouseSettings.ForeColor = System.Drawing.SystemColors.ControlText;
            this.gbMouseSettings.Location = new System.Drawing.Point(15, 132);
            this.gbMouseSettings.Name = "gbMouseSettings";
            this.gbMouseSettings.Size = new System.Drawing.Size(585, 86);
            this.gbMouseSettings.TabIndex = 4;
            this.gbMouseSettings.TabStop = false;
            this.gbMouseSettings.Text = "Mouse Settings";
            // 
            // rgMouseMode
            // 
            this.rgMouseMode.AutoSize = true;
            this.rgMouseMode.BackColor = System.Drawing.Color.Transparent;
            this.rgMouseMode.DataBindings.Add(new System.Windows.Forms.Binding("StateObject", this.hidAcheDeviceBindingSource, "MouseBehavior", true));
            this.rgMouseMode.Dock = System.Windows.Forms.DockStyle.Fill;
            this.rgMouseMode.Location = new System.Drawing.Point(3, 19);
            this.rgMouseMode.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.rgMouseMode.Name = "rgMouseMode";
            this.rgMouseMode.Size = new System.Drawing.Size(579, 64);
            this.rgMouseMode.StateObject = null;
            this.rgMouseMode.TabIndex = 0;
            // 
            // gbDeviceMode
            // 
            this.gbDeviceMode.BackColor = System.Drawing.Color.Transparent;
            this.gbDeviceMode.Controls.Add(this.rgDeviceMode);
            this.gbDeviceMode.Font = new System.Drawing.Font("Tahoma", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.gbDeviceMode.ForeColor = System.Drawing.SystemColors.ControlText;
            this.gbDeviceMode.Location = new System.Drawing.Point(15, 57);
            this.gbDeviceMode.Name = "gbDeviceMode";
            this.gbDeviceMode.Size = new System.Drawing.Size(362, 55);
            this.gbDeviceMode.TabIndex = 3;
            this.gbDeviceMode.TabStop = false;
            this.gbDeviceMode.Text = "Device Mode";
            // 
            // rgDeviceMode
            // 
            this.rgDeviceMode.AutoSize = true;
            this.rgDeviceMode.BackColor = System.Drawing.Color.Transparent;
            this.rgDeviceMode.DataBindings.Add(new System.Windows.Forms.Binding("StateObject", this.hidAcheDeviceBindingSource, "Mode", true));
            this.rgDeviceMode.Location = new System.Drawing.Point(10, 23);
            this.rgDeviceMode.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.rgDeviceMode.Name = "rgDeviceMode";
            this.rgDeviceMode.Size = new System.Drawing.Size(345, 23);
            this.rgDeviceMode.StateObject = null;
            this.rgDeviceMode.TabIndex = 0;
            // 
            // lblInterval
            // 
            this.lblInterval.AutoSize = true;
            this.lblInterval.BackColor = System.Drawing.Color.Transparent;
            this.lblInterval.Font = new System.Drawing.Font("Tahoma", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblInterval.ForeColor = System.Drawing.SystemColors.ControlText;
            this.lblInterval.Location = new System.Drawing.Point(383, 74);
            this.lblInterval.Name = "lblInterval";
            this.lblInterval.Size = new System.Drawing.Size(167, 29);
            this.lblInterval.TabIndex = 8;
            this.lblInterval.Text = "Interval (min):";
            // 
            // tbInterval
            // 
            this.tbInterval.DataBindings.Add(new System.Windows.Forms.Binding("Text", this.hidAcheDeviceBindingSource, "Interval", true));
            this.tbInterval.Font = new System.Drawing.Font("Comic Sans MS", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbInterval.Location = new System.Drawing.Point(548, 72);
            this.tbInterval.Name = "tbInterval";
            this.tbInterval.Size = new System.Drawing.Size(52, 37);
            this.tbInterval.TabIndex = 9;
            // 
            // btnUpdate
            // 
            this.btnUpdate.Font = new System.Drawing.Font("Tahoma", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnUpdate.Location = new System.Drawing.Point(226, 448);
            this.btnUpdate.Name = "btnUpdate";
            this.btnUpdate.Size = new System.Drawing.Size(169, 34);
            this.btnUpdate.TabIndex = 10;
            this.btnUpdate.Text = "Update Device";
            this.btnUpdate.UseVisualStyleBackColor = true;
            this.btnUpdate.Click += new System.EventHandler(this.btnUpdate_Click);
            // 
            // pbStatus
            // 
            this.pbStatus.BackColor = System.Drawing.Color.Transparent;
            this.pbStatus.Image = global::HIDache.Properties.Resources.Disconnected;
            this.pbStatus.Location = new System.Drawing.Point(15, 19);
            this.pbStatus.Name = "pbStatus";
            this.pbStatus.Size = new System.Drawing.Size(32, 32);
            this.pbStatus.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
            this.pbStatus.TabIndex = 7;
            this.pbStatus.TabStop = false;
            // 
            // HIDAcheControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.SteelBlue;
            this.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.Controls.Add(this.btnUpdate);
            this.Controls.Add(this.tbInterval);
            this.Controls.Add(this.lblInterval);
            this.Controls.Add(this.pbStatus);
            this.Controls.Add(this.lblIsConnected);
            this.Controls.Add(this.gbKeyboardSettings);
            this.Controls.Add(this.gbMouseSettings);
            this.Controls.Add(this.gbDeviceMode);
            this.DoubleBuffered = true;
            this.Margin = new System.Windows.Forms.Padding(0);
            this.Name = "HIDAcheControl";
            this.Size = new System.Drawing.Size(616, 503);
            ((System.ComponentModel.ISupportInitialize)(this.hidAcheDeviceBindingSource)).EndInit();
            this.gbKeyboardSettings.ResumeLayout(false);
            this.gbKeyboardSettings.PerformLayout();
            this.gbMouseSettings.ResumeLayout(false);
            this.gbMouseSettings.PerformLayout();
            this.gbDeviceMode.ResumeLayout(false);
            this.gbDeviceMode.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pbStatus)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label lblIsConnected;
        private System.Windows.Forms.GroupBox gbKeyboardSettings;
        private System.Windows.Forms.GroupBox gbMouseSettings;
        private System.Windows.Forms.GroupBox gbDeviceMode;
        private System.Windows.Forms.TextBox tbPhrase;
        private System.Windows.Forms.BindingSource hidAcheDeviceBindingSource;
        private System.Windows.Forms.PictureBox pbStatus;
        private System.Windows.Forms.Label lblInterval;
        private System.Windows.Forms.TextBox tbInterval;
        private System.Windows.Forms.Button btnUpdate;
        private RadioButtonGroup rgDeviceMode;
        private RadioButtonGroup rgMouseMode;
        private System.Windows.Forms.Label label1;
    }
}
