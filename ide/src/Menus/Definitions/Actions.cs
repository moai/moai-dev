using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using MOAI.Designers;

namespace MOAI.Menus.Definitions.Actions
{
    class Open : Action
    {
        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = Properties.Resources.actions_open;
            this.Text = "Open File";
            this.Enabled = false;
        }
    }

    class Close : Action
    {
        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = null;
            this.Text = "Close";
            this.Enabled = false;
        }
    }

    class Save : Action
    {
        private Designer m_CurrentEditor = null;

        public override void OnSetSettings()
        {
            this.ItemIcon = Properties.Resources.actions_save;
            this.Text = "Save";
            this.Enabled = false;
            this.Shortcut = System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.S;
        }

        public override void OnActivate()
        {
            m_CurrentEditor.OnSaveFile();
        }

        public override void OnSolutionLoaded()
        {
        }

        public override void OnTabChanged(Designer editor)
        {
            if (editor == null || editor.File == null)
            {
                this.Enabled = false;
                return;
            }
            
            this.Enabled = editor.CanSave;
            this.m_CurrentEditor = editor;
            this.Text = "Save " + editor.File.FileInfo.Name;
        }
    }

    class SaveAs : Action
    {
        private Designer m_CurrentEditor = null;

        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = null;
            this.Text = "Save as...";
            this.Enabled = false;
        }

        public override void OnActivate()
        {
            // TODO: Implement Save As...
        }

        public override void OnTabChanged(Designer editor)
        {
            if (editor == null || editor.File == null)
            {
                this.Enabled = false;
                return;
            }

            this.Enabled = editor.CanSave;
            this.m_CurrentEditor = editor;
            this.Text = "Save " + editor.File.FileInfo.Name + " as...";
            this.Item.Enabled = this.Enabled;
        }
    }

    class SaveAll : Action
    {
        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = Properties.Resources.actions_save_all;
            this.Text = "Save All";
            this.Enabled = false;
        }
    }

    class Exit : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = null;
            this.Text = "Exit";
            this.Enabled = true;
        }

        public override void OnActivate()
        {
            // TODO: Add proper unsaved changes checking etc.. here
            Program.Manager.Stop();
        }

        public override void OnSolutionLoaded()
        {
            this.Enabled = true;
            this.Item.Enabled = this.Enabled;
        }

        public override void OnSolutionUnloaded()
        {
            this.Enabled = true;
            this.Item.Enabled = this.Enabled;
        }
    }

    class Undo : Action
    {
        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = Properties.Resources.actions_undo;
            this.Text = "Undo";
            this.Enabled = false;
        }
    }

    class Redo : Action
    {
        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = Properties.Resources.actions_redo;
            this.Text = "Redo";
            this.Enabled = false;
        }
    }

    class Cut : Action
    {
        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = Properties.Resources.actions_cut;
            this.Text = "Cut";
            this.Enabled = false;
        }
    }

    class Copy : Action
    {
        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = Properties.Resources.actions_copy;
            this.Text = "Copy";
            this.Enabled = false;
        }
    }

    class Paste : Action
    {
        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = Properties.Resources.actions_paste;
            this.Text = "Paste";
            this.Enabled = false;
        }
    }

    class Delete : Action
    {
        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = null;
            this.Text = "Delete";
            this.Enabled = false;
        }
    }

    class SelectAll : Action
    {
        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = null;
            this.Text = "Select All";
            this.Enabled = false;
        }
    }

    class QuickFind : Action
    {
        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = null;
            this.Text = "Quick Find";
            this.Enabled = false;
        }
    }

    class QuickReplace : Action
    {
        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = null;
            this.Text = "Quick Replace";
            this.Enabled = false;
        }
    }

    class FindInFiles : Action
    {
        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = Properties.Resources.actions_find_in_files;
            this.Text = "Find in Files";
            this.Enabled = false;
        }
    }

    class ReplaceInFiles : Action
    {
        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = null;
            this.Text = "Replace in Files";
            this.Enabled = false;
        }
    }

    class GoTo : Action
    {
        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = null;
            this.Text = "Go To...";
            this.Enabled = false;
        }
    }

    class Preferences : Action
    {
        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = null;
            this.Text = "Preferences";
            this.Enabled = false;
        }
    }
}
