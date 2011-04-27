namespace MOAI.Tools
{
    partial class ConsoleTool
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ConsoleTool));
            this.c_ConsoleTextBox = new System.Windows.Forms.TextBox();
            this.c_InputPanel = new System.Windows.Forms.Panel();
            this.c_CommandTextBox = new System.Windows.Forms.TextBox();
            this.c_PromptLabel = new System.Windows.Forms.Label();
            this.c_InputPanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // c_ConsoleTextBox
            // 
            this.c_ConsoleTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.c_ConsoleTextBox.BackColor = System.Drawing.Color.Black;
            this.c_ConsoleTextBox.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.c_ConsoleTextBox.Font = new System.Drawing.Font("Courier New", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.c_ConsoleTextBox.ForeColor = System.Drawing.Color.White;
            this.c_ConsoleTextBox.Location = new System.Drawing.Point(5, 1);
            this.c_ConsoleTextBox.Multiline = true;
            this.c_ConsoleTextBox.Name = "c_ConsoleTextBox";
            this.c_ConsoleTextBox.ReadOnly = true;
            this.c_ConsoleTextBox.Size = new System.Drawing.Size(533, 236);
            this.c_ConsoleTextBox.TabIndex = 0;
            this.c_ConsoleTextBox.Text = "Welcome to the MOAI Lua Console.\r\nType Lua statements to execute them.\r\nReturn" +
                " Lua values to have them outputted.\r\n\r\n-----------------------------";
            // 
            // c_InputPanel
            // 
            this.c_InputPanel.BackColor = System.Drawing.Color.Black;
            this.c_InputPanel.Controls.Add(this.c_CommandTextBox);
            this.c_InputPanel.Controls.Add(this.c_PromptLabel);
            this.c_InputPanel.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.c_InputPanel.Location = new System.Drawing.Point(0, 237);
            this.c_InputPanel.Name = "c_InputPanel";
            this.c_InputPanel.Size = new System.Drawing.Size(538, 19);
            this.c_InputPanel.TabIndex = 1;
            // 
            // c_CommandTextBox
            // 
            this.c_CommandTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.c_CommandTextBox.BackColor = System.Drawing.Color.Black;
            this.c_CommandTextBox.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.c_CommandTextBox.Font = new System.Drawing.Font("Courier New", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.c_CommandTextBox.ForeColor = System.Drawing.Color.White;
            this.c_CommandTextBox.Location = new System.Drawing.Point(19, 2);
            this.c_CommandTextBox.Name = "c_CommandTextBox";
            this.c_CommandTextBox.Size = new System.Drawing.Size(516, 14);
            this.c_CommandTextBox.TabIndex = 0;
            this.c_CommandTextBox.KeyDown += new System.Windows.Forms.KeyEventHandler(this.commandTextBox_KeyDown);
            // 
            // c_PromptLabel
            // 
            this.c_PromptLabel.AutoSize = true;
            this.c_PromptLabel.Font = new System.Drawing.Font("Courier New", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.c_PromptLabel.ForeColor = System.Drawing.Color.White;
            this.c_PromptLabel.Location = new System.Drawing.Point(2, 1);
            this.c_PromptLabel.Name = "c_PromptLabel";
            this.c_PromptLabel.Size = new System.Drawing.Size(21, 15);
            this.c_PromptLabel.TabIndex = 1;
            this.c_PromptLabel.Text = "> ";
            // 
            // ToolConsole
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.Black;
            this.ClientSize = new System.Drawing.Size(538, 256);
            this.Controls.Add(this.c_InputPanel);
            this.Controls.Add(this.c_ConsoleTextBox);
            this.DockAreas = ((DockPanelSuite.DockAreas)(((((DockPanelSuite.DockAreas.Float | DockPanelSuite.DockAreas.DockLeft)
                        | DockPanelSuite.DockAreas.DockRight)
                        | DockPanelSuite.DockAreas.DockTop)
                        | DockPanelSuite.DockAreas.DockBottom)));
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "ToolConsole";
            this.TabText = "Lua Console";
            this.Text = "Lua Console";
            this.Load += new System.EventHandler(this.ToolConsole_Load);
            this.Enter += new System.EventHandler(this.ToolConsole_Enter);
            this.c_InputPanel.ResumeLayout(false);
            this.c_InputPanel.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox c_ConsoleTextBox;
        private System.Windows.Forms.Panel c_InputPanel;
        private System.Windows.Forms.Label c_PromptLabel;
        private System.Windows.Forms.TextBox c_CommandTextBox;
    }
}