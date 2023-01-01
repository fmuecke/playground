using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Text;
using System.Windows.Forms;
using Microsoft.Win32;
using Ini;

namespace EISwiz
{
    public partial class EISwiz : Form
    {
        private RegistryKey regKey;
        private string appDir = "Test Project";
        private string appDefaultDir;
        private string appDefaultPath;
        private string licenseFileName;
        private string outputFileName = "setup.bat";
        private string toolsDirName = "~EIS";
        private string mainDir = "c:\\tmp\\MAINDIR";
        private StreamWriter outputFile;
        private string regKeyName = "SOFTWARE\\EISwiz";
        private FormSettings settingsDlg;
        private FormPackerSettings packerSettingsDlg;
        private FormAbout aboutDlg;
        private FormRegVals regValsDlg;

        public EISwiz()
        {
            InitializeComponent();

            settingsDlg = new FormSettings();
            packerSettingsDlg = new FormPackerSettings();
            regValsDlg = new FormRegVals();
            //this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen; //center window
                        
            // get registry values
            try
            {
                regKey = Registry.LocalMachine.OpenSubKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\WinRAR.exe");
                packerSettingsDlg.setWinRARexe(regKey.GetValue("").ToString()); //WinRAR.exe path
                packerSettingsDlg.setWinRARpath(regKey.GetValue("Path").ToString()); //WinRAR folder
                regKey.Close();

                regKey = Registry.CurrentUser.CreateSubKey(regKeyName);
                settingsDlg.setOutputDir(regKey.GetValue("OutputDir").ToString());
                textBox_app_name.Text = regKey.GetValue("Application").ToString();
                appDir = regKey.GetValue("ApplicationDir").ToString();
                textBox_app_dir.Text = appDir;
                textBox_app_publisher.Text = regKey.GetValue("Publisher").ToString();
                textBox_app_contact.Text = regKey.GetValue("Contact").ToString();
                textBox_app_infoURL.Text = regKey.GetValue("Info URL").ToString();
                appDefaultDir = regKey.GetValue("Default Folder").ToString();
                textBox_default_dir.Text = appDefaultDir;
                appDefaultPath = regKey.GetValue("Default Path").ToString();
                textBox_default_path.Text = appDefaultPath;
                licenseFileName = regKey.GetValue("LicenseFile").ToString();
                textBox_license.Text = licenseFileName;
                checkBox_license.Checked = regKey.GetValue("ShowLicenseFile").Equals("True");
                packerSettingsDlg.setEncrypt(regKey.GetValue("Encrypt").Equals("True"));
                packerSettingsDlg.setPassword(regKey.GetValue("EncryptWith").ToString());
                packerSettingsDlg.setCompressionLevel(regKey.GetValue("CompressionLevel").ToString());
                packerSettingsDlg.setDictionarySize(regKey.GetValue("Dictionary").ToString());
                packerSettingsDlg.setVolumeSize(regKey.GetValue("VolumeSize").ToString());
                packerSettingsDlg.setSolidArchives(regKey.GetValue("SolidArchives").Equals("True"));
                settingsDlg.setOutputDir(regKey.GetValue("OutputDir").ToString());

            }
            catch (Exception)
            {
                //throw;
            }

            // disable Shortcut groupboxes
            groupBox_SCD.Enabled = checkBox_SCD.Checked;
            groupBox_SCSM.Enabled = checkBox_SCSM.Checked;

            // preselect values on packer tab
            //comboBox_Packer_compression.SelectedIndex = comboBox_Packer_compression.Items.Count - 1;
            //comboBox_Packer_dictionary.SelectedIndex = comboBox_Packer_dictionary.Items.Count - 1;
            //comboBox_Packer_volume_size.SelectedIndex = comboBox_Packer_volume_size.Items.Count - 1;
            //@TODO:speichern und auslesen mit registry!!

            // preselect values on registry tab
            listView_RegAdd.ContextMenuStrip = contextMenuRegAdd;
            listView_RegDel.ContextMenuStrip = contextMenuRegDel;
        }
        /* retrieves registry values to add */
        private string getRegValsAddStr()
        {
            string regValsAddStr = "";
            int i = 0;

            while (i < listView_RegAdd.Items.Count)
            {
                String hkey = listView_RegAdd.Items[i].SubItems[0].Text;
                String subkey = listView_RegAdd.Items[i].SubItems[1].Text;
                String key = listView_RegAdd.Items[i].SubItems[2].Text;
                String type = "REG_" + listView_RegAdd.Items[i].SubItems[3].Text;
                String value = listView_RegAdd.Items[i].SubItems[4].Text;
                regValsAddStr += "" + toolsDirName + "\\regtool.exe ADD " + hkey + " \"" + subkey + "\" \"" + key + "\" " + type + " \"" + value + "\">nul\n";
                i++;
            }
            return regValsAddStr;
        }
        /* retrieves registry values to del */
        private string getRegValsDelStr()
        {
            string regValsDelStr = "";
            int i = 0;
            while (i < listView_RegDel.Items.Count)
            {
                string hkey = listView_RegDel.Items[i].SubItems[0].Text;
                string subkey = listView_RegDel.Items[i].SubItems[1].Text;
                regValsDelStr += "echo \"" + toolsDirName + "\\regtool.exe delkey " + hkey + " \"" + subkey + "\\\">>\"%windir%\\%UNINSTFILE%\"\n";
                i++;
            }
            return regValsDelStr;
        }
        /* copies the setup files to the program directory */
        private bool copyFiles()
        {
            string dir = appDir + "\\" + toolsDirName;
            try
            {
                Directory.CreateDirectory(dir);
                File.Copy(mainDir + "\\_tools\\regtool.exe", dir + "\\regtool.exe", true);
                File.Copy(mainDir + "\\_tools\\scgen.exe", dir + "\\scgen.exe", true);
                File.Copy(mainDir + "\\_tools\\uneis.exe", dir + "\\uneis.exe", true);
                return true;
            }
            catch (Exception)
            {
                //throw;
                MessageBox.Show("Error copying setup files!", "build error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }
        }
        /* creates the setup script */
        private bool createSetupScript()
        {
            outputFile = new StreamWriter(appDir + "\\" + outputFileName);
            string output = "@echo off\n" +
                ":**************************************************\n" +
                ": Generated by EISwiz (c) F.Muecke 2006\n" +
                ": Do not modify unless you know what you are doing!\n" +
                ":**************************************************\n" +
                "\n" +
                "rem Version 1.21.00 (2006.01.29 Florian Muecke)\n" +
                "\n" +
                "rem change settings below - do not use chars like <,>,$,&\n" +
                "rem       [...........................................] max title length\n" +
                "set TITLE=" + textBox_app_name.Text + "\n" +
                "set PUBLISHER=" + textBox_app_publisher.Text + "\n" +
                "set CONTACT=" + textBox_app_contact.Text + "\n" +
                "set INFOURL=" + textBox_app_infoURL.Text + "\n" +
                "set UNINSTFILE=DEL" + textBox_app_name.Text + ".bat\n" +
                "set UNINSTICON=\n" +//@TODO:
                "set LNKNAME=\n" +
                "set LNKTARGET=\n" +
                "set LNKARGS=\n" +
                "set WORKINGDIR=\n" +
                "\n" +
                "goto init\n" +
                ":regadd\n" +
                "\n" +
                "rem registry values to add\n" +
                "\n" + getRegValsAddStr() + "\n" +
                "goto regadd_done\n" +
                ":regdel\n" +
                "\n" +
                "rem registry values to delete\n" +
                "\n" + getRegValsDelStr() + "\n" +
                "goto regdel_done \n" +
                "\n" +
                ":init\n" +
                "if not exist " + toolsDirName + "\\regtool.exe goto noregtool\n" +
                "if not exist " + toolsDirName + "\\scgen.exe goto noscgen\n" +
                "if not exist " + toolsDirName + "\\env.bat goto noenv\n" +
                "call " + toolsDirName + "\\env.bat\n" +
                "goto DoIt\n" +
                "\n" +
                ":noenv\n" +
                "echo ERROR: unable to create environment variables\n" +
                "goto the_end\n" +
                "\n" +
                ":noregtool\n" +
                "echo ERROR: " + toolsDirName + "\\regtool.exe not found\n" +
                "goto the_end\n" +
                "\n" +
                ":noscgen\n" +
                "echo ERROR: " + toolsDirName + "\\scgen.exe not found\n" +
                "goto the_end\n" +
                "\n" +
                ":DoIt\n" +
                "echo;\n" +
                "echo  %TITLE%, Setup\n" +
                "echo;\n" +
                "echo -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n" +
                "echo;\n" +
                "if not exist " + toolsDirName + "\\decomp.bat goto nodecomp\n" +
                "echo  * extracting sound files (this could take a while)\n" +
                "call " + toolsDirName + "\\decomp.bat\n" +
                "echo;\n" +
                "\n" +
                ":nodecomp\n" +
                "echo  * setting registry entries...\n" +
                "\n" +
                "goto regadd\n" +
                ":regadd_done\n" +
                "\n" +
                "if %SHORTCUTS% == 0 goto no_shortcuts\n" +
                "\n" +
                "echo;\n" +
                "echo  * creating shortcuts\n" +
                "if %SHORTCUTS% == 1 goto smenu_shortcut\n" +
                "if %SHORTCUTS% == 3 goto smenu_shortcut\n" +
                "if %SHORTCUTS% == 2 goto desk_shortcut\n" +
                "goto no_shortcuts\n" +
                "\n" +
                ":smenu_shortcut\n" +
                "" + toolsDirName + "\\scgen.exe \"%SMENU%\\%LNKNAME%.lnk\" \"%cd%\\%LNKTARGET%\" \"%LNKARGS%\" \"%cd%\\%WORKINGDIR%\">nul\n" +
                "\n" +
                "if not %SHORTCUTS% == 3 goto no_shortcuts\n" +
                "\n" +
                ":desk_shortcut\n" +
                "" + toolsDirName + "\\scgen.exe \"%DESKTOP%\\%LNKNAME%.lnk\" \"%cd%\\%LNKTARGET%\" \"%LNKARGS%\" \"%cd%\\%WORKINGDIR%\">nul\n" +
                "\n" +
                ":no_shortcuts\n" +
                "echo;\n" +
                "echo  * creating uninstall information\n" +
                "" + toolsDirName + "\\regtool.exe add HKLM \"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\%TITLE%\" \"DisplayName\" REG_SZ \"%TITLE%\">nul\n" +
                "" + toolsDirName + "\\regtool.exe add HKLM \"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\%TITLE%\" \"UninstallString\" REG_SZ \"%WINDIR%\\uneis.exe %TITLE%\">nul\n" +
                "" + toolsDirName + "\\regtool.exe add HKLM \"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\%TITLE%\" \"UnEisFile\" REG_SZ \"%WINDIR%\\%UNINSTFILE%\">nul\n" +
                "" + toolsDirName + "\\regtool.exe add HKLM \"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\%TITLE%\" \"Publisher\" REG_SZ \"%PUBLISHER%\">nul\n" +
                "" + toolsDirName + "\\regtool.exe add HKLM \"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\%TITLE%\" \"Contact\" REG_SZ \"%CONTACT%\">nul\n" +
                "" + toolsDirName + "\\regtool.exe add HKLM \"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\%TITLE%\" \"DisplayIcon\" REG_SZ \"%CD%\\%UNINSTICON%,-0\">nul\n" +
                "" + toolsDirName + "\\regtool.exe add HKLM \"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\%TITLE%\" \"UnEisFile\" REG_SZ \"%WINDIR%\\%UNINSTFILE%\">nul\n" +
                "" + toolsDirName + "\\regtool.exe add HKLM \"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\%TITLE%\" \"URLInfoAbout\" REG_SZ \"%INFOURL%\">nul\n" +
                "copy " + toolsDirName + "\\regtool.exe %windir%>nul\n" +
                "copy " + toolsDirName + "\\uneis.exe %windir%>nul\n" +
                "echo @echo off>\"%windir%\\%UNINSTFILE%\"\n" +
                "echo echo;>>\"%windir%\\%UNINSTFILE%\"\n" +
                "echo echo;>>\"%windir%\\%UNINSTFILE%\"\n" +
                "echo echo  %TITLE%, Uninstall>>\"%windir%\\%UNINSTFILE%\"\n" +
                "echo echo;>>\"%windir%\\%UNINSTFILE%\"\n" +
                "echo echo -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=->>\"%windir%\\%UNINSTFILE%\"\n" +
                "echo echo;>>\"%windir%\\%UNINSTFILE%\"\n" +
                "echo echo  * removing registry entries>>\"%windir%\\%UNINSTFILE%\"\n" +
                "echo echo;>>\"%windir%\\%UNINSTFILE%\"\n" +
                "goto regdel\n" +
                "\n" +
                ":regdel_done\n" +
                "echo echo;>>\"%windir%\\%UNINSTFILE%\"\n" +
                "echo echo  * removing uninstall entries>>\"%windir%\\%UNINSTFILE%\"\n" +
                "echo echo;>>\"%windir%\\%UNINSTFILE%\"\n" +
                "echo \"%windir%\\regtool.exe\" delkey HKLM \"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\%TITLE%\">>\"%windir%\\%UNINSTFILE%\"\n" +
                "echo echo;>>\"%windir%\\%UNINSTFILE%\"\n" +
                "echo echo  * removing files and folders>>\"%windir%\\%UNINSTFILE%\"\n" +
                "echo echo;>>\"%windir%\\%UNINSTFILE%\"\n" +
                "\n" +
                "if \"%OS%\"==\"Windows_NT\" goto WINNT\n" +
                "echo deltree /Y \"%CD%\">>\"%windir%\\%UNINSTFILE%\"\n" +
                "goto finished\n" +
                "\n" +
                ":WINNT\n" +
                "echo rd /S /Q \"%CD%\">>\"%windir%\\%UNINSTFILE%\"\n" +
                "\n" +
                ":finished\n" +
                "echo echo;>>\"%windir%\\%UNINSTFILE%\"\n" +
                "echo if exist \"%DESKTOP%\\%LNKNAME%.lnk\" del \"%DESKTOP%\\%LNKNAME%.lnk\">>\"%windir%\\%UNINSTFILE%\"\n" +
                "echo if exist \"%SMENU%\\%LNKNAME%.lnk\" del \"%SMENU%\\%LNKNAME%.lnk\">>\"%windir%\\%UNINSTFILE%\"\n" +
                "echo echo -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=->>\"%windir%\\%UNINSTFILE%\"\n" +
                "echo echo;>>\"%windir%\\%UNINSTFILE%\"\n" +
                "echo echo  * fertig!>>\"%windir%\\%UNINSTFILE%\"\n" +
                "if NOT \"%OS%\"==\"Windows_NT\" echo cls>>\"%windir%\\%UNINSTFILE%\"\n" +
                "echo;\n" +
                "echo  * done!\n" +
                "echo;\n" +
                "echo -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n" +
                "echo;\n" +
                "echo  Have fun with this fine application!\n" +
                "echo;\n" +
                "echo;\n" +
                "if \"%OS%\"==\"Windows_NT\" goto the_end\n" +
                "echo installation complete>\"%cd%\\" + toolsDirName + "\\done\"\n" +
                "exit\n" +
                ":the_end\n";
            outputFile.Write(output, 0, output.Length);
            outputFile.Close();
            return true; //@todo: return false!!
        }

        /* calculates the size of a dir (with subdirs) */
        private long getDirSize(DirectoryInfo d)
        {
            long Size = 0;
            // Add file sizes.
            FileInfo[] fis = d.GetFiles();
            foreach (FileInfo fi in fis)
            {
                Size += fi.Length;
            }
            // Add subdirectory sizes.
            DirectoryInfo[] dis = d.GetDirectories();
            foreach (DirectoryInfo di in dis)
            {
                Size += getDirSize(di);
            }
            return (Size);
        }
        
        private bool build()
        {
            /* create ini for installer */
            long size = getDirSize(new DirectoryInfo(appDir)); //get size of app directory
            Ini.IniFile iniFile = new IniFile(mainDir + "\\_build\\RES\\eis.ini");
            iniFile.IniWriteValue("Easy Installation System", "TITLE", textBox_app_name.Text);
            iniFile.IniWriteValue("Easy Installation System", "PATH", textBox_default_path.Text);
            iniFile.IniWriteValue("Easy Installation System", "DIR", textBox_default_dir.Text);
            iniFile.IniWriteValue("Easy Installation System", "FILE", ""); // TODO: dateinamen!!
            iniFile.IniWriteValue("Easy Installation System", "SIZE", size.ToString());
            iniFile.IniWriteValue("Easy Installation System", "PATCH", "patch.eis");
            int sCuts = 0;
            if (checkBox_SCD.Checked) sCuts += 1;
            if (checkBox_SCSM.Checked) sCuts += 2;
            iniFile.IniWriteValue("Easy Installation System", "SHORTCUTS", sCuts.ToString());
            System.IO.Directory.SetCurrentDirectory(mainDir + "\\_build");
            System.Diagnostics.Process.Start("fullBuild.bat");
            File.Copy(mainDir + "\\_build\\setup.exe", settingsDlg.getOutputDir()+"\\setup.exe", true);
            return true;
        }

        private bool pack()
        {
            //TODO: check if old archive exists
            //Directory.CreateDirectory(settingsDlg.getOutputDir() + "\\EIS-" + textBox_app_name.Text); - not needed right now
            System.Diagnostics.Process.Start(packerSettingsDlg.getWinRARexe(), 
                "a -r -s -m"+packerSettingsDlg.getCompressionLevelId()+" -md"+packerSettingsDlg.getDictionarySize() +  //params
                " \"" + settingsDlg.getOutputDir() + "\\eisdata.rar\"" + //archive
                " \"" + appDir + "\\*.*\"");  //files
            return true;
        }

        private void button_browseForLicense_Click(object sender, EventArgs e)
        {
            openLicenseDialog.InitialDirectory = licenseFileName;
            if (openLicenseDialog.ShowDialog() == DialogResult.OK) 
                licenseFileName = openLicenseDialog.FileName;
            textBox_license.Text = licenseFileName;
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            bool enabled = this.checkBox_license.Checked;
            button_browseForLicense.Enabled = enabled;
            label_license.Enabled = enabled;
            textBox_license.Enabled = enabled;
        }

        /* default path browse button */
        private void button_default_path_browse_Click(object sender, EventArgs e)
        {
            folderBrowserDialog_DefaultPath.SelectedPath = appDefaultPath;
            if (folderBrowserDialog_DefaultPath.ShowDialog() == DialogResult.OK)
                appDefaultPath = folderBrowserDialog_DefaultPath.SelectedPath;
            textBox_default_path.Text = appDefaultPath;
        }

        /* application path browse button */
        private void button_app_browse_Click(object sender, EventArgs e)
        {
            folderBrowserDialog_AppDir.SelectedPath = appDir;
            if (folderBrowserDialog_AppDir.ShowDialog() == DialogResult.OK)
                appDir = folderBrowserDialog_AppDir.SelectedPath;
            textBox_app_dir.Text = appDir;
        }
        /* default dir (copy value) button */
        private void button_default_dir_Click(object sender, EventArgs e)
        {
            appDefaultDir = textBox_app_name.Text;
            textBox_default_dir.Text = appDefaultDir;
        }

        private void button_SCD_name_Click(object sender, EventArgs e)
        {
            textBox_SCD_name.Text = textBox_app_name.Text;
        }

        private void button_SCSM_name_Click(object sender, EventArgs e)
        {
            textBox_SCSM_name.Text = textBox_app_name.Text;
        }

        private void checkBox_SCD_CheckedChanged(object sender, EventArgs e)
        {
            groupBox_SCD.Enabled = checkBox_SCD.Checked;
        }

        private void checkBox_SCSM_CheckedChanged(object sender, EventArgs e)
        {
            groupBox_SCSM.Enabled = checkBox_SCSM.Checked;
        }

        private void button_SCD_browse_Click(object sender, EventArgs e)
        {
            openSCDialog.Reset();
            openSCDialog.InitialDirectory = textBox_app_dir.Text;
            openSCDialog.ShowDialog();
            if (openSCDialog.FileName.StartsWith(textBox_app_dir.Text))
                textBox_SCD_file.Text = openSCDialog.FileName.Substring(textBox_app_dir.Text.Length+1);
            else
                textBox_SCD_file.Text = openSCDialog.FileName;
            
        }

        private void button_SCSM_browse_Click(object sender, EventArgs e)
        {
            openSCDialog.Reset();
            openSCDialog.InitialDirectory = textBox_app_dir.Text;
            openSCDialog.ShowDialog();
            if (openSCDialog.FileName.StartsWith(textBox_app_dir.Text))
                textBox_SCSM_file.Text = openSCDialog.FileName.Substring(textBox_app_dir.Text.Length + 1);
            else
                textBox_SCSM_file.Text = openSCDialog.FileName;

        }
       
        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            regKey.SetValue("OutputDir", settingsDlg.getOutputDir());
            regKey.SetValue("Application", textBox_app_name.Text);
            regKey.SetValue("ApplicationDir", appDir);
            regKey.SetValue("Publisher", textBox_app_publisher.Text);
            regKey.SetValue("Contact", textBox_app_contact.Text);
            regKey.SetValue("Info URL", textBox_app_infoURL.Text);
            appDefaultDir = textBox_default_dir.Text;
            regKey.SetValue("Default Folder", appDefaultDir);
            appDefaultPath = textBox_default_path.Text;
            regKey.SetValue("Default Path", appDefaultPath);
            regKey.SetValue("LicenseFile", licenseFileName);
            regKey.SetValue("ShowLicenseFile", checkBox_license.Checked.ToString());
            regKey.SetValue("Encrypt", packerSettingsDlg.getEncrypt().ToString());
            regKey.SetValue("CompressionLevel", packerSettingsDlg.getCompressionLevel());
            regKey.SetValue("Dictionary", packerSettingsDlg.getDictionarySize());
            regKey.SetValue("VolumeSize", packerSettingsDlg.getVolumeSize());
            regKey.SetValue("SolidArchives", packerSettingsDlg.getSolidArchives().ToString());
            regKey.SetValue("EncryptWith", packerSettingsDlg.getPassword());

            regKey.Close();
            settingsDlg.Dispose();
            settingsDlg.Close();
            packerSettingsDlg.Dispose();
            packerSettingsDlg.Close();
            regValsDlg.Dispose();
            regValsDlg.Close();
            this.Dispose();
            this.Close();

        }

        private void MenuItem_buildAll_Click(object sender, EventArgs e)
        {
            if (MessageBox.Show(this, "Accept all settings and proceed?", "ready to build?", MessageBoxButtons.YesNo) == DialogResult.Yes)
            {
                // get registry add strings
                // get registry del strings
                
            }
        }

        private void optionsToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            if (settingsDlg.ShowDialog() == DialogResult.OK)
            {
                //update values
            }
        }

        private void MenuItem_buildSettings_Click(object sender, EventArgs e)
        {
            if (packerSettingsDlg.ShowDialog() == DialogResult.OK)
            {
                //update values
            }

        }

        private void MenuItem_About_Click(object sender, EventArgs e)
        {
            aboutDlg = new FormAbout();
            aboutDlg.ShowDialog();
            aboutDlg.Dispose();
            aboutDlg.Close();
        }

        private void saveFileAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            IniFile ini = new IniFile("C:\\test.ini");
            ini.IniWriteValue("Application", "Name", textBox_app_name.Text);
            ini.IniWriteValue("Application", "Folder", textBox_app_dir.Text);
            ini.IniWriteValue("Application", "Publisher", textBox_app_publisher.Text);
            ini.IniWriteValue("Application", "Contact", textBox_app_contact.Text);
            ini.IniWriteValue("Application", "InfoURL", textBox_app_infoURL.Text);
        }

        private void MenuItem_buildInstaller_Click(object sender, EventArgs e)
        {
            copyFiles();
            createSetupScript();
            build();
        }

        private void removeValuesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            while (listView_RegAdd.SelectedItems.Count > 0) listView_RegAdd.SelectedItems[0].Remove();
        }

        private void toolStripRegDelRemove_Click(object sender, EventArgs e)
        {
            while (listView_RegDel.SelectedItems.Count > 0) listView_RegDel.SelectedItems[0].Remove();
        }

        private void toolStripRegAddAdd_Click(object sender, EventArgs e)
        {
            regValsDlg.setAddMode(true);
            if (regValsDlg.ShowDialog() == DialogResult.OK)
            {
                string[] items = { regValsDlg.hkey, regValsDlg.subkey, regValsDlg.key, 
                    regValsDlg.type, regValsDlg.value };
                listView_RegAdd.Items.Add(new ListViewItem(items));
            }
    
        }

        private void toolStripRegDelAdd_Click(object sender, EventArgs e)
        {
            regValsDlg.setAddMode(false);
            if (regValsDlg.ShowDialog() == DialogResult.OK)
            {
                string[] items = { regValsDlg.hkey, regValsDlg.subkey };
                listView_RegDel.Items.Add(new ListViewItem(items));
            }

        }

        private void MenuItem_CreateArchives_Click(object sender, EventArgs e)
        {
            if (MessageBox.Show(this, "Proceed creating archive files? (this could take a while...)", "ready to build?", MessageBoxButtons.YesNo) == DialogResult.Yes)
            {
                pack();
            }

        }

    }
}