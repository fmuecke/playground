using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace EISwiz
{
    public partial class FormRegVals : Form
    {
        public string hkey;
        public string subkey;
        public string key;
        public string type;
        public string value;
        private bool addMode;
        
        public FormRegVals()
        {
            InitializeComponent();
            comboBox_RegHKAdd.SelectedIndex = 0;
            comboBox_RegType.SelectedIndex = 1;
        }

        /* disables some boxes for del mode */
        public void setAddMode(bool val)
        {
            addMode = val;
            textBox_RegKey.Enabled = val;
            textBox_RegValue.Enabled = val;
            comboBox_RegType.Enabled = val;
        }

        private void button_RegOK_Click(object sender, EventArgs e)
        {
            if (textBox_RegSubkeyAdd.Text.Length > 0 || textBox_RegKey.Text.Length > 0 || textBox_RegValue.Text.Length > 0 || 
                (textBox_RegSubkeyAdd.Text.Length > 0 && !addMode ))
            {
                hkey = comboBox_RegHKAdd.SelectedItem.ToString();
                subkey = textBox_RegSubkeyAdd.Text;
                key = textBox_RegKey.Text;
                type = comboBox_RegType.SelectedItem.ToString();
                value = textBox_RegValue.Text;
                Hide();
                DialogResult = DialogResult.OK;
            }
            else
            {
                MessageBox.Show("Please enter some more values!", "too few values", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                DialogResult = DialogResult.None;
            }
        }

        private void button_Cancel_Click(object sender, EventArgs e)
        {
            this.Hide();
        }
    }
}