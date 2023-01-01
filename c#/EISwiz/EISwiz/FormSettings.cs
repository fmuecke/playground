using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace EISwiz
{
    public partial class FormSettings : Form
    {
        private String outputDir;
        public FormSettings()
        {
            InitializeComponent();
        }

        public String getOutputDir()
        {
            return textBox_OutputDir.Text; 
        }
        public void setOutputDir(String dir) 
        {
            outputDir = dir;
            textBox_OutputDir.Text = outputDir;
        }

        /* output dir browse button */
        private void button_outputdir_browse_Click(object sender, EventArgs e)
        {
            folderBrowserDialog_OutputDir.SelectedPath = textBox_OutputDir.Text;
            if (folderBrowserDialog_OutputDir.ShowDialog() == DialogResult.OK)
                textBox_OutputDir.Text = folderBrowserDialog_OutputDir.SelectedPath;
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            /* discard all values */
            textBox_OutputDir.Text = outputDir;
            this.Hide();
        }

        private void button_OK_Click(object sender, EventArgs e)
        {
            /* save values */
            outputDir = textBox_OutputDir.Text;
            this.Hide();
        }

    }
}