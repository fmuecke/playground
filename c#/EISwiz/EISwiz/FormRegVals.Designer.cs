namespace EISwiz
{
    partial class FormRegVals
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
            this.label14 = new System.Windows.Forms.Label();
            this.label13 = new System.Windows.Forms.Label();
            this.label12 = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.button_RegOK = new System.Windows.Forms.Button();
            this.textBox_RegValue = new System.Windows.Forms.TextBox();
            this.textBox_RegKey = new System.Windows.Forms.TextBox();
            this.textBox_RegSubkeyAdd = new System.Windows.Forms.TextBox();
            this.comboBox_RegType = new System.Windows.Forms.ComboBox();
            this.comboBox_RegHKAdd = new System.Windows.Forms.ComboBox();
            this.button_Cancel = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(12, 121);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(34, 13);
            this.label14.TabIndex = 32;
            this.label14.Text = "Value";
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(12, 94);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(31, 13);
            this.label13.TabIndex = 31;
            this.label13.Text = "Type";
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(12, 68);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(25, 13);
            this.label12.TabIndex = 30;
            this.label12.Text = "Key";
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(12, 42);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(43, 13);
            this.label11.TabIndex = 29;
            this.label11.Text = "Subkey";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(12, 15);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(30, 13);
            this.label10.TabIndex = 28;
            this.label10.Text = "Root";
            // 
            // button_RegOK
            // 
            this.button_RegOK.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.button_RegOK.Location = new System.Drawing.Point(160, 146);
            this.button_RegOK.Name = "button_RegOK";
            this.button_RegOK.Size = new System.Drawing.Size(98, 25);
            this.button_RegOK.TabIndex = 27;
            this.button_RegOK.Text = "OK";
            this.button_RegOK.UseVisualStyleBackColor = true;
            this.button_RegOK.Click += new System.EventHandler(this.button_RegOK_Click);
            // 
            // textBox_RegValue
            // 
            this.textBox_RegValue.Location = new System.Drawing.Point(61, 118);
            this.textBox_RegValue.Name = "textBox_RegValue";
            this.textBox_RegValue.Size = new System.Drawing.Size(301, 20);
            this.textBox_RegValue.TabIndex = 26;
            // 
            // textBox_RegKey
            // 
            this.textBox_RegKey.Location = new System.Drawing.Point(61, 65);
            this.textBox_RegKey.Name = "textBox_RegKey";
            this.textBox_RegKey.Size = new System.Drawing.Size(301, 20);
            this.textBox_RegKey.TabIndex = 25;
            // 
            // textBox_RegSubkeyAdd
            // 
            this.textBox_RegSubkeyAdd.Location = new System.Drawing.Point(61, 39);
            this.textBox_RegSubkeyAdd.Name = "textBox_RegSubkeyAdd";
            this.textBox_RegSubkeyAdd.Size = new System.Drawing.Size(301, 20);
            this.textBox_RegSubkeyAdd.TabIndex = 24;
            // 
            // comboBox_RegType
            // 
            this.comboBox_RegType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox_RegType.FormattingEnabled = true;
            this.comboBox_RegType.Items.AddRange(new object[] {
            "DWORD",
            "SZ",
            "NONE",
            "BINARY"});
            this.comboBox_RegType.Location = new System.Drawing.Point(61, 91);
            this.comboBox_RegType.Name = "comboBox_RegType";
            this.comboBox_RegType.Size = new System.Drawing.Size(79, 21);
            this.comboBox_RegType.TabIndex = 23;
            // 
            // comboBox_RegHKAdd
            // 
            this.comboBox_RegHKAdd.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox_RegHKAdd.FormattingEnabled = true;
            this.comboBox_RegHKAdd.Items.AddRange(new object[] {
            "HKLM",
            "HKCU",
            "HKCR",
            "HKU"});
            this.comboBox_RegHKAdd.Location = new System.Drawing.Point(61, 12);
            this.comboBox_RegHKAdd.Name = "comboBox_RegHKAdd";
            this.comboBox_RegHKAdd.Size = new System.Drawing.Size(55, 21);
            this.comboBox_RegHKAdd.TabIndex = 22;
            // 
            // button_Cancel
            // 
            this.button_Cancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.button_Cancel.Location = new System.Drawing.Point(264, 146);
            this.button_Cancel.Name = "button_Cancel";
            this.button_Cancel.Size = new System.Drawing.Size(98, 25);
            this.button_Cancel.TabIndex = 33;
            this.button_Cancel.Text = "Cancel";
            this.button_Cancel.UseVisualStyleBackColor = true;
            this.button_Cancel.Click += new System.EventHandler(this.button_Cancel_Click);
            // 
            // FormRegVals
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(374, 183);
            this.ControlBox = false;
            this.Controls.Add(this.button_Cancel);
            this.Controls.Add(this.label14);
            this.Controls.Add(this.label13);
            this.Controls.Add(this.label12);
            this.Controls.Add(this.label11);
            this.Controls.Add(this.label10);
            this.Controls.Add(this.button_RegOK);
            this.Controls.Add(this.textBox_RegValue);
            this.Controls.Add(this.textBox_RegKey);
            this.Controls.Add(this.textBox_RegSubkeyAdd);
            this.Controls.Add(this.comboBox_RegType);
            this.Controls.Add(this.comboBox_RegHKAdd);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Name = "FormRegVals";
            this.Opacity = 0.95;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Add registry values";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Button button_RegOK;
        private System.Windows.Forms.TextBox textBox_RegValue;
        private System.Windows.Forms.TextBox textBox_RegKey;
        private System.Windows.Forms.TextBox textBox_RegSubkeyAdd;
        private System.Windows.Forms.ComboBox comboBox_RegType;
        private System.Windows.Forms.ComboBox comboBox_RegHKAdd;
        private System.Windows.Forms.Button button_Cancel;
    }
}