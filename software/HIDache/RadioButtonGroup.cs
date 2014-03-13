using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace HIDache
{
    // The RadioButtonGroup allows us to bind a group of RadioButton objects to a single value and based on that value
    // one of the RadionButton objects will be selected.  For this program, we want to bind the device mode to a certain
    // RadioButton.

    public partial class RadioButtonGroup : UserControl
    {
        private object stateObject;
        public delegate void StateChangedEventHandler(object sender, RadioButtonGroupEventArgs e);
        public event StateChangedEventHandler StateChanged; 

        public RadioButtonGroup()
        {
            InitializeComponent();
        }

        public RadioButtonGroup(RadioButton[] buttons)
        {
            foreach(RadioButton rb in buttons)
            {
                AddRadioButton(rb);
            }
        }

        protected virtual void OnStateChanged(object state) 
        { 
            StateChangedEventHandler handler = this.StateChanged; 
            if (handler != null) 
                handler(this, new RadioButtonGroupEventArgs(state)); 
        }

        public void AddRadioButton(RadioButton b)
        {
            if (b.Tag != null)
            {
                b.CheckedChanged += new EventHandler(radio_CheckedChanged);
                layoutPanel.Controls.Add(b);
            }
            else
                throw new InvalidOperationException("The Tag property on the RadioButton cannot be null.");
        }

        public void AddRadioButton(string text, object tag)
        {
            if (tag != null)
            {
                RadioButton b = new RadioButton();
                b.Text = text;
                b.Tag = tag;
                b.CheckedChanged += new EventHandler(radio_CheckedChanged);
                layoutPanel.Controls.Add(b);
            }
            else
                throw new InvalidOperationException("The Tag property on the RadioButton cannot be null.");
        }

        void radio_CheckedChanged(object sender, EventArgs e)
        {
            stateObject = ((RadioButton)sender).Tag;

            //Notify any attached handlers
            OnStateChanged(stateObject);
            this.Validate();
        }

        // This is the bindable object that will allow us to bind the device modes to the radio group control
        [Bindable(true)]
        public object StateObject
        {
            get { return stateObject; }
            set 
            { 
                stateObject = value;
                foreach (RadioButton rb in layoutPanel.Controls)
                {
                    if (rb.Tag.Equals(stateObject))
                        rb.Checked = true;
                }
            }
        }
    }
    // RadioButtonGroupEventArgs Definition
    public class RadioButtonGroupEventArgs : EventArgs
    {
        public readonly object state;

        public RadioButtonGroupEventArgs(object state)
        {
            this.state = state;
        }
    } 
}
