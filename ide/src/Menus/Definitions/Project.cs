using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MOAI.Management;

namespace MOAI.Menus.Definitions.Project
{
    class New : Action
    {
        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = Properties.Resources.project_create;
            this.Text = "New Project";
            this.Enabled = false;
        }
    }

    class AddFile : Action
    {
        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = Properties.Resources.file_add;
            this.Text = "Add New Item";
            this.Enabled = false;
        }
    }

    class AddModel : Action
    {
        public File TargetDirectory = null;

        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = Properties.Resources.model_add;
            this.Text = "Add Model...";
            this.Enabled = false;
        }

        public override void OnActivate()
        {
            /*FileTreeNode ftn = new FileTreeNode();
            if (TargetDirectory != null)
                ftn.fileRef = TargetDirectory;
            else
                // TODO: Make this check the solution based on what file is currently has focus.
                ftn.projectRef = Program.MainWindow.CurrentSolution.Projects[0];
            Creation.AddModel(ftn);*/
        }
    }

    class AddImage : Action
    {
        public File TargetDirectory = null;

        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = Properties.Resources.image_add;
            this.Text = "Add Image...";
            this.Enabled = false;
        }

        public override void OnActivate()
        {
            /*FileTreeNode ftn = new FileTreeNode();
            if (TargetDirectory != null)
                ftn.fileRef = TargetDirectory;
            else
                // TODO: Make this check the solution based on what file is currently has focus.
                ftn.projectRef = Program.MainWindow.CurrentSolution.Projects[0];
            Creation.AddImage(ftn);*/
        }
    }

    class AddAudio : Action
    {
        public File TargetDirectory = null;

        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = Properties.Resources.audio_add;
            this.Text = "Add Audio...";
            this.Enabled = false;
        }
    }

    class AddTemplate : Action
    {
        public File TargetDirectory = null;

        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = null; //Properties.Resources.template_add;
            this.Text = "Add Template...";
            this.Enabled = false;
        }
    }

    class AddArea : Action
    {
        public File TargetDirectory = null;

        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = Properties.Resources.area_add;
            this.Text = "Add Area...";
            this.Enabled = false;
        }
    }

    class AddWorld : Action
    {
        public File TargetDirectory = null;

        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = Properties.Resources.world_add;
            this.Text = "Add World...";
            this.Enabled = false;
        }
    }

    class AddClass : Action
    {
        public File TargetDirectory = null;

        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = Properties.Resources.class_add;
            this.Text = "Add Class...";
            this.Enabled = false;
        }
    }

    class AddScript : Action
    {
        public File TargetDirectory = null;

        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = Properties.Resources.script_add;
            this.Text = "Add Script...";
            this.Enabled = false;
        }
    }

    class AddNewItem : Action
    {
        public File TargetDirectory = null;

        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = null;
            this.Text = "Add New Item";
            this.Enabled = false;
        }
    }

    class AddExistingItem : Action
    {
        public File TargetDirectory = null;

        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = null;
            this.Text = "Add Existing Item";
            this.Enabled = false;
        }
    }

    class AddReference : Action
    {
        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = Properties.Resources.reference;
            this.Text = "Add Reference...";
            this.Enabled = false;
        }
    }

    class ProjProperties : Action
    {
        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = null;
            this.Text = "Project Properties";
            this.Enabled = false;
        }
    }

    class Build : Action
    {
        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = null;
            this.Text = "Build MyProject";
            this.Enabled = false;
        }

        public override void OnActivate()
        {
            /*
            Compilation.ProjectBuilder pb = new Compilation.ProjectBuilder(Program.MainWindow.CurrentSolution.Projects[0], Program.MainWindow.BuildOutput);
            pb.Build(Roket3D.Compilation.BuildMode.DEBUG);
            */
        }
    }

    class Rebuild : Action
    {
        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = null;
            this.Text = "Rebuild MyProject";
            this.Enabled = false;
        }
    }

    class Clean : Action
    {
        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = null;
            this.Text = "Clean MyProject";
            this.Enabled = false;
        }
    }
}
