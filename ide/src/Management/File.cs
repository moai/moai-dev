using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MOAI.Collections;
using System.IO;
using System.Drawing;

namespace MOAI.Management
{
    public class File : System.Windows.Forms.TreeNode
    {
        private Project p_Project = null;
        private FileInfo p_FileInfo = null;

        /// <summary>
        /// A protected constructor so that derived classes such as Folder only have
        /// to provide the project as an argument to the constructor.
        /// </summary>
        /// <param name="p">The project that owns this folder.</param>
        protected File(Project p)
        {
            this.p_Project = p;
        }

        /// <summary>
        /// Creates a new File object based on an relative path and a Project object.
        /// </summary>
        /// <param name="p">The project that owns this folder.</param>
        /// <param name="parent">The parent director (the directory the project file is located in).</param>
        /// <param name="relpath">The relative path to the file.</param>
        public File(Project p, string parent, string relpath)
        {
            this.p_Project = p;
            this.p_FileInfo = new FileInfo(Path.Combine(parent, relpath));
        }

        /// <summary>
        /// Associates this file with a tree node.
        /// </summary>
        /// <param name="node"></param>
        public virtual void Associate(System.Windows.Forms.TreeNode node)
        {
            // Set properties.
            this.Text = this.ToString();
            this.ImageKey = Associations.GetImageKey(this.p_FileInfo.Extension.Substring(1));
            this.SelectedImageKey = this.ImageKey;

            // Add this file to the node.
            node.Nodes.Add(this);
        }

        /// <summary>
        /// The FileInfo object that represents this file on-disk.
        /// </summary>
        public FileInfo FileInfo
        {
            get
            {
                return this.p_FileInfo;
            }
        }

        /// <summary>
        /// The project that owns this file.
        /// </summary>
        public Project Project
        {
            get
            {
                return this.p_Project;
            }
        }

        /// <summary>
        /// Overrides the ToString() function which is used by the tree view
        /// so that it displays the file name.
        /// </summary>
        public override string ToString()
        {
            return this.FileInfo.Name;
        }
    }
}
