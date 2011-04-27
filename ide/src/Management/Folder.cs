using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace MOAI.Management
{
    public class Folder : File
    {
        private DirectoryInfo p_FolderInfo = null;
        private List<File> p_Files = null;

        /// <summary>
        /// Creates a new Folder object for representing a folder in the project.
        /// </summary>
        /// <param name="p">The project that owns this folder.</param>
        public Folder(Project p)
            : base(p)
        {
            this.p_Files = new List<File>();
        }

        /// <summary>
        /// Creates a new File object based on an relative path and a Project object.
        /// </summary>
        /// <param name="p">The project that owns this folder.</param>
        /// <param name="parent">The parent director (the directory the project file is located in).</param>
        /// <param name="relpath">The relative path to the file.</param>
        public Folder(Project p, string parent, string relpath)
            : base(p)
        {
            this.p_Files = new List<File>();
            this.p_FolderInfo = new DirectoryInfo(Path.Combine(parent, relpath));
        }

        /// <summary>
        /// Associates this folder with a tree node.
        /// </summary>
        /// <param name="node"></param>
        public override void Associate(System.Windows.Forms.TreeNode node)
        {
            // Set properties.
            this.Text = this.ToString();
            this.ImageKey = Associations.GetImageKey("folder");
            this.SelectedImageKey = this.ImageKey;

            // We also need to loop through the children of this folder
            // to add them to.
            this.Nodes.Clear();
            foreach (File f in this.p_Files)
                f.Associate(this);

            // Add this file to the node.
            node.Nodes.Add(this);
        }

        /// <summary>
        /// The DirectoryInfo object that represents this folder on-disk.
        /// </summary>
        public DirectoryInfo FolderInfo
        {
            get
            {
                return this.p_FolderInfo;
            }
        }

        /// <summary>
        /// A read-only list of the files within the this folder.
        /// </summary>
        public System.Collections.ObjectModel.ReadOnlyCollection<File> Files
        {
            get
            {
                return this.p_Files.AsReadOnly();
            }
        }

        /// <summary>
        /// Adds a file to the list of files contained within this folder.
        /// </summary>
        /// <param name="file"></param>
        public void Add(File file)
        {
            this.p_Files.Add(file);
        }

        /// <summary>
        /// Overrides the ToString() function which is used by the tree view
        /// so that it displays the folder name.
        /// </summary>
        public override string ToString()
        {
            return this.FolderInfo.Name;
        }
    }
}
