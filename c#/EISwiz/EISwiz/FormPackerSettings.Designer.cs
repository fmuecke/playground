namespace EISwiz
{
    partial class FormPackerSettings
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

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.groupBox_Packer_general = new System.Windows.Forms.GroupBox();
            this.button_WinRARexe_browse = new System.Windows.Forms.Button();
            this.label10 = new System.Windows.Forms.Label();
            this.textBox_Packer_exe = new System.Windows.Forms.TextBox();
            this.groupBox_Packer_encrypt = new System.Windows.Forms.GroupBox();
            this.label_Packer_encrypt = new System.Windows.Forms.Label();
            this.checkBox_Packer_encrypt_showpw = new System.Windows.Forms.CheckBox();
            this.checkBox_Paker_encrypt = new System.Windows.Forms.CheckBox();
            this.textBox_Packer_encrypt = new System.Windows.Forms.TextBox();
            this.groupBox_Packer_compression = new System.Windows.Forms.GroupBox();
            this.label12 = new System.Windows.Forms.Label();
            this.label13 = new System.Windows.Forms.Label();
            this.comboBox_Packer_compression = new System.Windows.Forms.ComboBox();
            this.comboBox_Packer_dictionary = new System.Windows.Forms.ComboBox();
            this.groupBox_Packer_advanced = new System.Windows.Forms.GroupBox();
            this.comboBox_Packer_volume_size = new System.Windows.Forms.ComboBox();
            this.label14 = new System.Windows.Forms.Label();
            this.checkBox_Packer_solid = new System.Windows.Forms.CheckBox();
            this.buttonCancel = new System.Windows.Forms.Button();
            this.button_OK = new System.Windows.Forms.Button();
            this.openRARexeDialog = new System.Windows.Forms.OpenFileDialog();
            this.groupBox_Packer_general.SuspendLayout();
            this.groupBox_Packer_encrypt.SuspendLayout();
            this.groupBox_Packer_compression.SuspendLayout();
            this.groupBox_Packer_advanced.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox_Packer_general
            // 
            this.groupBox_Packer_general.Controls.Add(this.button_WinRARexe_browse);
            this.groupBox_Packer_general.Controls.Add(this.label10);
            this.groupBox_Packer_general.Controls.Add(this.textBox_Packer_exe);
            this.groupBox_Packer_general.Location = new System.Drawing.Point(12, 12);
            this.groupBox_Packer_general.Name = "groupBox_Packer_general";
            this.groupBox_Packer_general.Size = new System.Drawing.Size(470, 42);
            this.groupBox_Packer_general.TabIndex = 25;
            this.groupBox_Packer_general.TabStop = false;
            this.groupBox_Packer_general.Text = "General";
            // 
            // button_WinRARexe_browse
            // 
            this.button_WinRARexe_browse.Location = new System.Drawing.Point(364, 13);
            this.button_WinRARexe_browse.Name = "button_WinRARexe_browse";
            this.button_WinRARexe_browse.Size = new System.Drawing.Size(98, 20);
            this.button_WinRARexe_browse.TabIndex = 25;
            this.button_WinRARexe_browse.Text = "browse";
            this.button_WinRARexe_browse.UseVisualStyleBackColor = true;
            this.button_WinRARexe_browse.Click += new System.EventHandler(this.button_WinRARexe_browse_Click);
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(6, 16);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(105, 13);
            this.label10.TabIndex = 24;
            this.label10.Text = "WinRAR Executable";
            // 
            // textBox_Packer_exe
            // 
            this.textBox_Packer_exe.Location = new System.Drawing.Point(117, 13);
            this.textBox_Packer_exe.Name = "textBox_Packer_exe";
            this.textBox_Packer_exe.Size = new System.Drawing.Size(241, 20);
            this.textBox_Packer_exe.TabIndex = 23;
            // 
            // groupBox_Packer_encrypt
            // 
            this.groupBox_Packer_encrypt.Controls.Add(this.label_Packer_encrypt);
            this.groupBox_Packer_encrypt.Controls.Add(this.checkBox_Packer_encrypt_showpw);
            this.groupBox_Packer_encrypt.Controls.Add(this.checkBox_Paker_encrypt);
            this.groupBox_Packer_encrypt.Controls.Add(this.textBox_Packer_encrypt);
            this.groupBox_Packer_encrypt.Location = new System.Drawing.Point(12, 60);
            this.groupBox_Packer_encrypt.Name = "groupBox_Packer_encrypt";
            this.groupBox_Packer_encrypt.Size = new System.Drawing.Size(462, 69);
            this.groupBox_Packer_encrypt.TabIndex = 26;
            this.groupBox_Packer_encrypt.TabStop = false;
            this.groupBox_Packer_encrypt.Text = "Encryption";
            // 
            // label_Packer_encrypt
            // 
            this.label_Packer_encrypt.AutoSize = true;
            this.label_Packer_encrypt.Enabled = false;
            this.label_Packer_encrypt.Location = new System.Drawing.Point(6, 46);
            this.label_Packer_encrypt.Name = "label_Packer_encrypt";
            this.label_Packer_encrypt.Size = new System.Drawing.Size(53, 13);
            this.label_Packer_encrypt.TabIndex = 9;
            this.label_Packer_encrypt.Text = "Password";
            // 
            // checkBox_Packer_encrypt_showpw
            // 
            this.checkBox_Packer_encrypt_showpw.AutoSize = true;
            this.checkBox_Packer_encrypt_showpw.Enabled = false;
            this.checkBox_Packer_encrypt_showpw.Location = new System.Drawing.Point(268, 45);
            this.checkBox_Packer_encrypt_showpw.Name = "checkBox_Packer_encrypt_showpw";
            this.checkBox_Packer_encrypt_showpw.Size = new System.Drawing.Size(144, 17);
            this.checkBox_Packer_encrypt_showpw.TabIndex = 8;
            this.checkBox_Packer_encrypt_showpw.Text = "Show cleartext password";
            this.checkBox_Packer_encrypt_showpw.UseVisualStyleBackColor = true;
            this.checkBox_Packer_encrypt_showpw.CheckedChanged += new System.EventHandler(this.checkBox_Packer_encrypt_showpw_CheckedChanged);
            // 
            // checkBox_Paker_encrypt
            // 
            this.checkBox_Paker_encrypt.AutoSize = true;
            this.checkBox_Paker_encrypt.Location = new System.Drawing.Point(6, 20);
            this.checkBox_Paker_encrypt.Name = "checkBox_Paker_encrypt";
            this.checkBox_Paker_encrypt.Size = new System.Drawing.Size(138, 17);
            this.checkBox_Paker_encrypt.TabIndex = 7;
            this.checkBox_Paker_encrypt.Text = "Encrypt files and folders";
            this.checkBox_Paker_encrypt.UseVisualStyleBackColor = true;
            this.checkBox_Paker_encrypt.CheckedChanged += new System.EventHandler(this.checkBox_Paker_encrypt_CheckedChanged);
            // 
            // textBox_Packer_encrypt
            // 
            this.textBox_Packer_encrypt.Enabled = false;
            this.textBox_Packer_encrypt.Location = new System.Drawing.Point(65, 43);
            this.textBox_Packer_encrypt.Name = "textBox_Packer_encrypt";
            this.textBox_Packer_encrypt.Size = new System.Drawing.Size(181, 20);
            this.textBox_Packer_encrypt.TabIndex = 6;
            this.textBox_Packer_encrypt.UseSystemPasswordChar = true;
            // 
            // groupBox_Packer_compression
            // 
            this.groupBox_Packer_compression.Controls.Add(this.label12);
            this.groupBox_Packer_compression.Controls.Add(this.label13);
            this.groupBox_Packer_compression.Controls.Add(this.comboBox_Packer_compression);
            this.groupBox_Packer_compression.Controls.Add(this.comboBox_Packer_dictionary);
            this.groupBox_Packer_compression.Location = new System.Drawing.Point(12, 135);
            this.groupBox_Packer_compression.Name = "groupBox_Packer_compression";
            this.groupBox_Packer_compression.Size = new System.Drawing.Size(111, 116);
            this.groupBox_Packer_compression.TabIndex = 29;
            this.groupBox_Packer_compression.TabStop = false;
            this.groupBox_Packer_compression.Text = "Compression";
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(6, 16);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(92, 13);
            this.label12.TabIndex = 7;
            this.label12.Text = "Compression level";
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(6, 65);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(75, 13);
            this.label13.TabIndex = 8;
            this.label13.Text = "Dictionary size";
            // 
            // comboBox_Packer_compression
            // 
            this.comboBox_Packer_compression.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox_Packer_compression.FormattingEnabled = true;
            this.comboBox_Packer_compression.Items.AddRange(new object[] {
            "0 (store)",
            "1 (fastest)",
            "2 (fast)",
            "3 (normal)",
            "4 (good)",
            "5 (best)"});
            this.comboBox_Packer_compression.Location = new System.Drawing.Point(9, 32);
            this.comboBox_Packer_compression.Name = "comboBox_Packer_compression";
            this.comboBox_Packer_compression.Size = new System.Drawing.Size(92, 21);
            this.comboBox_Packer_compression.TabIndex = 21;
            // 
            // comboBox_Packer_dictionary
            // 
            this.comboBox_Packer_dictionary.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox_Packer_dictionary.FormattingEnabled = true;
            this.comboBox_Packer_dictionary.Items.AddRange(new object[] {
            "64",
            "128",
            "256",
            "512",
            "1024",
            "2048",
            "4096"});
            this.comboBox_Packer_dictionary.Location = new System.Drawing.Point(9, 81);
            this.comboBox_Packer_dictionary.Name = "comboBox_Packer_dictionary";
            this.comboBox_Packer_dictionary.Size = new System.Drawing.Size(92, 21);
            this.comboBox_Packer_dictionary.TabIndex = 20;
            // 
            // groupBox_Packer_advanced
            // 
            this.groupBox_Packer_advanced.Controls.Add(this.comboBox_Packer_volume_size);
            this.groupBox_Packer_advanced.Controls.Add(this.label14);
            this.groupBox_Packer_advanced.Controls.Add(this.checkBox_Packer_solid);
            this.groupBox_Packer_advanced.Location = new System.Drawing.Point(129, 135);
            this.groupBox_Packer_advanced.Name = "groupBox_Packer_advanced";
            this.groupBox_Packer_advanced.Size = new System.Drawing.Size(345, 60);
            this.groupBox_Packer_advanced.TabIndex = 30;
            this.groupBox_Packer_advanced.TabStop = false;
            this.groupBox_Packer_advanced.Text = "Advanced options";
            // 
            // comboBox_Packer_volume_size
            // 
            this.comboBox_Packer_volume_size.FormattingEnabled = true;
            this.comboBox_Packer_volume_size.Items.AddRange(new object[] {
            "1.457.664 - 3.5\"",
            "98.078k - ZIP-100",
            "650m - CD-650M",
            "700m - CD-700M",
            "Autodetect"});
            this.comboBox_Packer_volume_size.Location = new System.Drawing.Point(9, 32);
            this.comboBox_Packer_volume_size.Name = "comboBox_Packer_volume_size";
            this.comboBox_Packer_volume_size.Size = new System.Drawing.Size(120, 21);
            this.comboBox_Packer_volume_size.TabIndex = 26;
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(6, 16);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(112, 13);
            this.label14.TabIndex = 25;
            this.label14.Text = "Split to volumes, bytes";
            // 
            // checkBox_Packer_solid
            // 
            this.checkBox_Packer_solid.AutoSize = true;
            this.checkBox_Packer_solid.Checked = true;
            this.checkBox_Packer_solid.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBox_Packer_solid.Location = new System.Drawing.Point(151, 34);
            this.checkBox_Packer_solid.Name = "checkBox_Packer_solid";
            this.checkBox_Packer_solid.Size = new System.Drawing.Size(124, 17);
            this.checkBox_Packer_solid.TabIndex = 24;
            this.checkBox_Packer_solid.Text = "Create solid archives";
            this.checkBox_Packer_solid.UseVisualStyleBackColor = true;
            // 
            // buttonCancel
            // 
            this.buttonCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.buttonCancel.Location = new System.Drawing.Point(384, 230);
            this.buttonCancel.Name = "buttonCancel";
            this.buttonCancel.Size = new System.Drawing.Size(98, 26);
            this.buttonCancel.TabIndex = 32;
            this.buttonCancel.Text = "Cancel";
            this.buttonCancel.UseVisualStyleBackColor = true;
            this.buttonCancel.Click += new System.EventHandler(this.buttonCancel_Click);
            // 
            // button_OK
            // 
            this.button_OK.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.button_OK.Location = new System.Drawing.Point(280, 230);
            this.button_OK.Name = "button_OK";
            this.button_OK.Size = new System.Drawing.Size(98, 26);
            this.button_OK.TabIndex = 31;
            this.button_OK.Text = "OK";
            this.button_OK.UseVisualStyleBackColor = true;
            this.button_OK.Click += new System.EventHandler(this.button_OK_Click);
            // 
            // openRARexeDialog
            // 
            this.openRARexeDialog.FileName = "WinRAR.exe";
            this.openRARexeDialog.Filter = "WinRAR executable|winrar.exe";
            this.openRARexeDialog.Title = "Select the file you wish to create the shortcut to...";
            // 
            // FormPackerSettings
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(494, 268);
            this.ControlBox = false;
            this.Controls.Add(this.buttonCancel);
            this.Controls.Add(this.button_OK);
            this.Controls.Add(this.groupBox_Packer_advanced);
            this.Controls.Add(this.groupBox_Packer_compression);
            this.Controls.Add(this.groupBox_Packer_encrypt);
            this.Controls.Add(this.groupBox_Packer_general);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Name = "FormPackerSettings";
            this.Opacity = 0.95;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Packer settings";
            this.groupBox_Packer_general.ResumeLayout(false);
            this.groupBox_Packer_general.PerformLayout();
            this.groupBox_Packer_encrypt.ResumeLayout(false);
            this.groupBox_Packer_encrypt.PerformLayout();
            this.groupBox_Packer_compression.ResumeLayout(false);
            this.groupBox_Packer_compression.PerformLayout();
            this.groupBox_Packer_advanced.ResumeLayout(false);
            this.groupBox_Packer_advanced.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox_Packer_general;
        private System.Windows.Forms.Button button_WinRARexe_browse;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.TextBox textBox_Packer_exe;
        private System.Windows.Forms.GroupBox groupBox_Packer_encrypt;
        private System.Windows.Forms.Label label_Packer_encrypt;
        private System.Windows.Forms.CheckBox checkBox_Packer_encrypt_showpw;
        private System.Windows.Forms.CheckBox checkBox_Paker_encrypt;
        private System.Windows.Forms.TextBox textBox_Packer_encrypt;
        private System.Windows.Forms.GroupBox groupBox_Packer_compression;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.ComboBox comboBox_Packer_compression;
        private System.Windows.Forms.ComboBox comboBox_Packer_dictionary;
        private System.Windows.Forms.GroupBox groupBox_Packer_advanced;
        private System.Windows.Forms.ComboBox comboBox_Packer_volume_size;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.CheckBox checkBox_Packer_solid;
        private System.Windows.Forms.Button buttonCancel;
        private System.Windows.Forms.Button button_OK;
        private System.Windows.Forms.OpenFileDialog openRARexeDialog;
    }
}