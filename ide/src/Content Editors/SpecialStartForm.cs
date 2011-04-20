using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Roket3D.ContentEditors;

namespace Roket3D.ContentEditors
{
    public partial class SpecialStartForm : EditorBase
    {
        public SpecialStartForm()
        {
            InitializeComponent();

            this.webBrowser.Url = new System.Uri(Program.ROOT_PATH + "/Start Page/index.htm", System.UriKind.Absolute);
        }

        private void webBrowser_Navigating(object sender, WebBrowserNavigatingEventArgs e)
        {
            Uri uri = e.Url;

            if (uri.ToString().StartsWith("about:"))
            {
                Dictionary<String, String> query = this.GetDictionaryFromQuery(uri.Query);

                switch (uri.LocalPath)
                {
                    case "solution":
                        if (query["mode"] == "new")
                        {
                            if (Program.MainWindow.CurrentSolution.HasLoaded)
                            {
                                MessageBox.Show(
                                    "You have already loaded a solution.  If you wish to create a new solution, close the current one first.",
                                    "Solution Already Loaded",
                                    MessageBoxButtons.OK,
                                    MessageBoxIcon.Error);
                                e.Cancel = true;
                                return;
                            }

                            NewSolutionForm nsf = new NewSolutionForm();
                            if (nsf.ShowDialog() == DialogResult.OK)
                            {
                                NewSolutionForm.HandleNewProjectCreation(nsf);
                            }
                        }
                        else if (query["mode"] == "open")
                        {
                            if (Program.MainWindow.CurrentSolution.HasLoaded)
                            {
                                MessageBox.Show(
                                    "You have already loaded a solution.  If you wish to load this solution, close the current one first.",
                                    "Solution Already Loaded",
                                    MessageBoxButtons.OK,
                                    MessageBoxIcon.Error);
                                e.Cancel = true;
                                return;
                            }

                            if (query.ContainsKey("uri"))
                            {
                                // directly load the solution.
                                String filename = query["uri"];
                                if (System.IO.File.Exists(filename))
                                {
                                    Program.MainWindow.CurrentSolution.Load(filename);
                                    Program.MainWindow.SolutionExplorer.ReloadTree();
                                    Program.MainWindow.SendMenuEvent(MenuEvent.SOLUTION_OPEN);
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
                                ofd.Filter = "Roket3D Solutions|*.rsln|Roket3D Projects|*.rproj";
                                if (ofd.ShowDialog() == DialogResult.OK)
                                {
                                    String filename = ofd.FileName;
                                    Program.MainWindow.CurrentSolution.Load(filename);
                                    Program.MainWindow.SolutionExplorer.ReloadTree();
                                    Program.MainWindow.SendMenuEvent(MenuEvent.SOLUTION_OPEN);
                                }
                            }
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

                e.Cancel = true;
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
