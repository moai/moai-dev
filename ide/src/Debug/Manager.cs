using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.IO;
using System.Windows.Forms;
using Roket3D.Tools;

namespace Roket3D.Debug
{
    public class Manager
    {
        private Roket3D.Manager p_Parent = null;
        private OutputTool m_OutputTool = null;
        private bool m_Running = false;

        /// <summary>
        /// Creates a new Manager class for managing debugging.
        /// </summary>
        /// <param name="parent">The main Roket3D manager which owns this debugging manager.</param>
        public Manager(Roket3D.Manager parent)
        {
            this.p_Parent = parent;
        }

        /// <summary>
        /// Runs the specified project with debugging.
        /// </summary>
        /// <param name="project">The project to run under the debugger.</param>
        public bool Run(Roket3D.Management.Project project)
        {
            if (this.m_Running)
            {
                // Can't run.
                return false;
            }

            this.m_OutputTool = this.p_Parent.ToolsManager.Get(typeof(OutputTool)) as OutputTool;
            if (this.m_OutputTool != null)
                this.m_OutputTool.ClearLog();

            Process proc = new Process();
            /*if (File.Exists("C:\\Windows\\system32\\vsjitdebugger.exe"))
            {
                proc.StartInfo.FileName = "C:\\Windows\\system32\\vsjitdebugger.exe";
                proc.StartInfo.Arguments = '"' + Path.Combine(Program.Manager.Settings["RootPath"], "Engines\\Win32\\Debug\\Roket3D.exe") + '"';
            }
            else*/
                proc.StartInfo.FileName = Path.Combine(Program.Manager.Settings["RootPath"], "Engines\\Win32\\Debug\\Roket3D.exe");

            proc.StartInfo.WorkingDirectory = project.ProjectInfo.Directory.FullName;
            proc.StartInfo.UseShellExecute = false;
            proc.StartInfo.RedirectStandardOutput = true;
            proc.OutputDataReceived += new DataReceivedEventHandler(proc_OutputDataReceived);
            proc.EnableRaisingEvents = true;
            proc.Exited += new EventHandler(proc_Exited);

            proc.Start();
            proc.BeginOutputReadLine();
            this.m_Running = true;
            return true;
        }

        /// <summary>
        /// This event is raised when the game has exited during debugging.
        /// </summary>
        /// <param name="sender">The sender of the event.</param>
        /// <param name="e">The event information.</param>
        void proc_Exited(object sender, EventArgs e)
        {
            this.m_Running = false;
        }

        /// <summary>
        /// The event is raised when the game or engine outputs to standard output
        /// and it's been redirected.
        /// </summary>
        /// <param name="sender">The sender of the event.</param>
        /// <param name="e">The event information.</param>
        void proc_OutputDataReceived(object sender, DataReceivedEventArgs e)
        {
            if (this.m_OutputTool != null && e.Data != null)
            {
                Roket3D.Manager.VoidLambda lambda = () =>
                    {
                        this.m_OutputTool.AddLogEntry(e.Data);
                    };
                this.m_OutputTool.Invoke(lambda);
            }
        }

        /// <summary>
        /// The main Roket3D manager that owns this debugging manager.
        /// </summary>
        public Roket3D.Manager Parent
        {
            get
            {
                return this.p_Parent;
            }
        }
    }
}
