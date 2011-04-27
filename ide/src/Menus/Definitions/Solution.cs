using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using MOAI.Tools;

namespace MOAI.Menus.Definitions.Solution
{
    class New : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = Properties.Resources.solution_create;
            this.Text = "New Solution";
            this.Enabled = true;
        }

        public override void OnSolutionLoaded()
        {
            this.Enabled = false;
            this.Item.Enabled = this.Enabled;
        }

        public override void OnSolutionUnloaded()
        {
            this.Enabled = true;
            this.Item.Enabled = this.Enabled;
        }

        public override void OnActivate()
        {
            NewSolutionForm nsf = new NewSolutionForm();
            if (nsf.ShowDialog() == DialogResult.OK)
            {
                // Request that the template create itself with the data provided.
                Management.Solution s = nsf.Result.Template.Create(nsf.Result);
                Program.Manager.LoadSolution(s.SolutionInfo.FullName);
            }
        }
    }

    class Open : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = Properties.Resources.solution_open;
            this.Text = "Open Solution";
            this.Enabled = true;
        }

        public override void OnSolutionLoaded()
        {
            this.Enabled = false;
            this.Item.Enabled = this.Enabled;
        }

        public override void OnSolutionUnloaded()
        {
            this.Enabled = true;
            this.Item.Enabled = this.Enabled;
        }

        public override void OnActivate()
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.CheckFileExists = true;
            ofd.CheckPathExists = true;
            ofd.RestoreDirectory = true;
            ofd.Filter = "MOAI Solutions|*.msln|MOAI Projects|*.mproj";
            if (ofd.ShowDialog() == DialogResult.OK)
            {
                String filename = ofd.FileName;
                Program.Manager.LoadSolution(filename);
            }
        }
    }

    class Close : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = null;
            this.Text = "Close Solution";
            this.Enabled = false;
        }

        public override void OnActivate()
        {
            // TODO: Implement asking whether to save changes here.
            Program.Manager.UnloadSolution();
        }
    }

    class StartWithDebug : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = Properties.Resources.debug;
            this.Text = "Start with Debugging";
            this.Enabled = false;
        }

        public override void OnActivate()
        {
            Program.Manager.DebugManager.Run(Program.Manager.ActiveProject);
        }
    }

    class StartWithoutDebug : Action
    {
        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = Properties.Resources.debug_without;
            this.Text = "Start without Debugging";
            this.Enabled = false;
        }
    }
}
