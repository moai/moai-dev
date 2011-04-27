using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using DockPanelSuite;

namespace MOAI.Tools
{
    public partial class ConsoleTool : Tool
    {
        // TODO: Run the LuaConsole and lua operations in another thread so that we can cancel it if the user accidently runs
        //       an infinite loop.
        //private LuaConsole myConsole = null;
        private List<String> commandList = new List<String>();
        private Int32 currentCommand = 0;

        public ConsoleTool()
        {
            InitializeComponent();
        }

        public override DockState DefaultState
        {
            get
            {
                return DockState.DockBottom;
            }
        }

        private void ToolConsole_Load(object sender, EventArgs e)
        {
            //myConsole = new LuaConsole(this.c_ConsoleTextBox);
        }

        private void ToolConsole_Enter(object sender, EventArgs e)
        {
            c_CommandTextBox.Focus();
        }

        private void commandTextBox_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                // Interpret it as command execute.
                //myConsole.HandleCommand(c_CommandTextBox.Text);
                commandList.Add(c_CommandTextBox.Text);
                currentCommand = commandList.Count; // set the current command to the last
                c_ConsoleTextBox.Select(c_ConsoleTextBox.Text.Length - 1, 0);
                c_ConsoleTextBox.ScrollToCaret();
                c_CommandTextBox.Text = "";
                c_CommandTextBox.Focus();
                e.SuppressKeyPress = true;
            }
            if (e.KeyCode == Keys.Up)
            {
                // Previous command.
                if (currentCommand > 0)
                {
                    currentCommand -= 1;
                    c_CommandTextBox.Text = commandList[currentCommand];
                }
                e.SuppressKeyPress = true;
            }
            if (e.KeyCode == Keys.Down)
            {
                // Previous command.
                if (currentCommand < commandList.Count - 1)
                {
                    currentCommand += 1;
                    c_CommandTextBox.Text = commandList[currentCommand];
                }
                else if (currentCommand == commandList.Count - 1)
                {
                    currentCommand += 1;
                    c_CommandTextBox.Text = "";
                }
                e.SuppressKeyPress = true;
            }
        }
    }
}
