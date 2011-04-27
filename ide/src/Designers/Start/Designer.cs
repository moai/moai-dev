using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using MOAI.Management;
using MOAI.Tools;

namespace MOAI.Designers.Start
{
    public partial class Designer : MOAI.Designers.Designer
    {
        /// <summary>
        /// Creates a new start page.
        /// </summary>
        /// <param name="manager">The main MOAI manager.</param>
        /// <param name="file">The associated file.</param>
        public Designer(MOAI.Manager manager, File file)
            : base(manager, file)
        {
            InitializeComponent();

            // This editor can never save.
            this.CanSave = false;
            this.File = null;

            this.TabText = "Start Page";
            this.c_WebBrowser.Url = new System.Uri(Program.Manager.Settings["RootPath"] + "/Start Page/index.htm", System.UriKind.Absolute);
        }

        /// <summary>
        /// This function is called after the IDE has finished resizing itself, or the
        /// total size of this dock content has otherwise changed.
        /// </summary>
        protected override void OnResize()
        {
        }

        /// <summary>
        /// This function is called after the active tab has changed.
        /// </summary>
        protected override void OnTabChanged()
        {
        }

        /// <summary>
        /// This function is called after the file that this designer owns has been saved to disk.
        /// </summary>
        public override void OnSaveFile()
        {

        }

        /// <summary>
        /// This event is raised when the user has navigated within the web browser.
        /// </summary>
        /// <param name="sender">The sender of the event.</param>
        /// <param name="e">The navigation event information.</param>
        private void c_WebBrowser_Navigating(object sender, WebBrowserNavigatingEventArgs e)
        {
            Uri uri = e.Url;

            if (uri.ToString().StartsWith("about:"))
            {
                e.Cancel = true;
                Dictionary<String, String> query = this.GetDictionaryFromQuery(uri.Query);

                switch (uri.LocalPath)
                {
                    case "solution":
                        if (query["mode"] == "new")
                        {
                            if (Program.Manager.ActiveSolution != null)
                            {
                                MessageBox.Show(
                                    "You have already loaded a solution.  If you wish to create a new solution, close the current one first.",
                                    "Solution Already Loaded",
                                    MessageBoxButtons.OK,
                                    MessageBoxIcon.Error);
                                return;
                            }

                            NewSolutionForm nsf = new NewSolutionForm();
                            if (nsf.ShowDialog() == DialogResult.OK)
                            {
                                // Request that the template create itself with the data provided.
                                nsf.Result.Template.Create(nsf.Result);
                            }

                            return;
                        }
                        else if (query["mode"] == "open")
                        {
                            if (Program.Manager.ActiveSolution != null)
                            {
                                MessageBox.Show(
                                    "You have already loaded a solution.  If you wish to load this solution, close the current one first.",
                                    "Solution Already Loaded",
                                    MessageBoxButtons.OK,
                                    MessageBoxIcon.Error);
                                return;
                            }

                            if (query.ContainsKey("uri"))
                            {
                                // directly load the solution.
                                string filename = query["uri"];
                                if (System.IO.File.Exists(filename))
                                {
                                    Program.Manager.LoadSolution(filename);
                                }
                                else
                                {
                                    MessageBox.Show(
                                        "The selected solution could not be found on disk.",
                                        "Solution Unavailable",
                                        MessageBoxButtons.OK,
                                        MessageBoxIcon.Error);
                                }
                            }
                            else
                            {
                                OpenFileDialog ofd = new OpenFileDialog();
                                ofd.CheckFileExists = true;
                                ofd.CheckPathExists = true;
                                ofd.RestoreDirectory = true;
                                ofd.Filter = "MOAI Solutions|*.rsln|MOAI Projects|*.rproj";
                                if (ofd.ShowDialog() == DialogResult.OK)
                                {
                                    string filename = ofd.FileName;
                                    Program.Manager.LoadSolution(filename);
                                }
                            }

                            return;
                        }

                        break;
                    case "tutorial":
                        MessageBox.Show(
                            "The selected tutorial is currently unavailable.",
                            "Tutorial Unavailable",
                            MessageBoxButtons.OK,
                            MessageBoxIcon.Error);
                        break;
                }
            }
        }

        private Dictionary<String, String> GetDictionaryFromQuery(String query)
        {
            if (query.StartsWith("?")) query = query.Substring(1);
            String[] keypairs = query.Split('&');
            Dictionary<String, String> result = new Dictionary<String, String>();
            foreach (String keypair in keypairs)
            {
                String[] keyvalue = keypair.Split('=');
                if (keyvalue.Length == 2)
                {
                    result.Add(keyvalue[0], keyvalue[1]);
                }
            }
            return result;
        }
    }
}
