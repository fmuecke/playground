using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace EISwiz
{
    public partial class FormPackerSettings : Form
    {
        private string winRARexe;
        private string winRARpath;
        private bool encrypt;
        private bool showPass;
        private string password;
        private string compressionLevel;
        private string dictionarySize;
        private string volumeSize;
        private bool solidArchives;

        public FormPackerSettings()
        {
            InitializeComponent();
            openRARexeDialog.CheckFileExists = true;
        }

        public void setWinRARexe(String exe)
        {
            winRARexe = exe;
            textBox_Packer_exe.Text = winRARexe;
        }
        public String getWinRARexe() {
            return winRARexe;
        }
        public void setWinRARpath(String path)
        {
            winRARpath = path;
        }
        public String getWinRARpath()
        {
            return winRARpath;
        }
        public void setEncrypt(bool enc)
        {
            encrypt = enc;
            checkBox_Paker_encrypt.Checked = enc;
        }
        public bool getEncrypt()
        {
            return encrypt;
        }
        public void setShowPass(bool show)
        {
            showPass = show;
            checkBox_Packer_encrypt_showpw.Checked = show;
        }
        public bool getShowPass()
        {
            return showPass;
        }
        public void setPassword(string pass)
        {
            password = pass;
            textBox_Packer_encrypt.Text = pass;
        }
        public string getPassword()
        {
            return password;
        }

        public void setCompressionLevel(string val)
        {
            compressionLevel = val;
            comboBox_Packer_compression.SelectedItem = val;
        }
        public string getCompressionLevel()
        {
            return compressionLevel;
        }
        public int getCompressionLevelId()
        {
            return comboBox_Packer_compression.SelectedIndex;
        }
        
        public void setDictionarySize(string val)
        {
            dictionarySize = val;
            comboBox_Packer_dictionary.SelectedItem = val;
        }
        public string getDictionarySize()
        {
            return dictionarySize;
        }
        public void setVolumeSize(string val)
        {
            volumeSize = val;
            comboBox_Packer_volume_size.SelectedItem = val;
        }
        public string getVolumeSize()
        {
            return volumeSize;
        }
        public void setSolidArchives(bool val)
        {
            solidArchives = val;
            checkBox_Packer_solid.Checked = val;
        }
        public bool getSolidArchives()
        {
            return solidArchives;
        }


        /* WinRAR exe browse button */
        private void button_WinRARexe_browse_Click(object sender, EventArgs e)
        {
            openRARexeDialog.InitialDirectory = winRARpath;
            if (openRARexeDialog.ShowDialog() == DialogResult.OK)
                textBox_Packer_exe.Text = openRARexeDialog.FileName;
        }

        private void button_OK_Click(object sender, EventArgs e)
        {
            /* save all values */
            winRARexe = textBox_Packer_exe.Text;
            compressionLevel = comboBox_Packer_compression.SelectedItem.ToString();
            dictionarySize = comboBox_Packer_dictionary.SelectedItem.ToString();
            volumeSize = comboBox_Packer_volume_size.SelectedItem.ToString();
            solidArchives = checkBox_Packer_solid.Checked;
            encrypt = checkBox_Paker_encrypt.Checked;
            password = textBox_Packer_encrypt.Text;
            //winRARpath = ... Pfad setzen für Reg??
            this.Hide();
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            /* discard all values */
            textBox_Packer_exe.Text = winRARexe;
            comboBox_Packer_compression.SelectedItem = compressionLevel;
            comboBox_Packer_dictionary.SelectedItem = dictionarySize;
            comboBox_Packer_volume_size.SelectedItem = volumeSize;
            checkBox_Packer_solid.Checked = solidArchives;
            checkBox_Paker_encrypt.Checked = encrypt;
            textBox_Packer_encrypt.Text = password;
            this.Hide();
        }

        private void checkBox_Paker_encrypt_CheckedChanged(object sender, EventArgs e)
        {
            bool enabled = checkBox_Paker_encrypt.Checked;
            textBox_Packer_encrypt.Enabled = enabled;
            checkBox_Packer_encrypt_showpw.Enabled = enabled;
            label_Packer_encrypt.Enabled = enabled;
        }

        private void checkBox_Packer_encrypt_showpw_CheckedChanged(object sender, EventArgs e)
        {
            textBox_Packer_encrypt.UseSystemPasswordChar = !checkBox_Packer_encrypt_showpw.Checked;
        }

    }
}