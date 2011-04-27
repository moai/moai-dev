using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using DockPanelSuite;
using MOAI.Management;

namespace MOAI.Designers
{
    public class Designer : DockContent
    {
        private MOAI.Manager p_Manager = null;
        private string p_Path = null;
        private MOAI.Management.File p_File = null;
        private bool p_CanSave = false;

        public Designer()
        {
        }

        /// <summary>
        /// Creates a new Designer base object.
        /// </summary>
        /// <param name="manager">The main MOAI manager object.</param>
        /// <param name="file">The associated file.</param>
        public Designer(MOAI.Manager manager, File file)
        {
            this.p_Manager = manager;
            this.p_File = file;
            if (this.p_File != null)
                this.TabText = this.p_File.FileInfo.Name;
            this.p_Manager.IDEWindow.ActiveTabChanged += (sender, e) =>
                {
                    this.OnTabChanged();
                };
            this.p_Manager.IDEWindow.ResizeEnd += (sender, e) =>
                {
                    this.OnResize();
                };
        }

        /// <summary>
        /// This function is called after the IDE has finished resizing itself, or the
        /// total size of this dock content has otherwise changed.
        /// </summary>
        protected virtual void OnResize() { }

        /// <summary>
        /// This function is called after the active tab has changed.
        /// </summary>
        protected virtual void OnTabChanged() { }

        /// <summary>
        /// This function is called after the file that this designer owns has been saved to disk.
        /// </summary>
        public virtual void OnSaveFile() { }

        /// <summary>
        /// The path to the file that this editor is currently editing.
        /// </summary>
        public string Path
        {
            get
            {
                return this.p_Path;
            }
            protected set
            {
                this.p_Path = value;
            }
        }

        /// <summary>
        /// The File object that represents the file that this editor is currently editing.
        /// Only valid if the Path points to a file that is currently in the project.
        /// </summary>
        public MOAI.Management.File File
        {
            get
            {
                return this.p_File;
            }
            protected set
            {
                this.p_File = value;
            }
        }

        /// <summary>
        /// Whether the save operation is valid on this editor.
        /// </summary>
        public bool CanSave
        {
            get
            {
                return this.p_CanSave;
            }
            protected set
            {
                this.p_CanSave = value;
            }
        }
    }
}
