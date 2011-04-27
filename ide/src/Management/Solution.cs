using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MOAI.Tools;
using System.IO;
using System.Xml;

namespace MOAI.Management
{
    public class Solution
    {
        private List<Project> p_Projects = new List<Project>();
        private FileInfo p_SolutionInfo = null;

        /// <summary>
        /// Creates a new instance of the Solution class that is not associated
        /// with any on-disk solution.
        /// </summary>
        public Solution()
        {
            this.p_SolutionInfo = null;
        }

        /// <summary>
        /// Loads a new instance of the Solution class from a file on disk.
        /// </summary>
        /// <param name="file">The solution file to be loaded.</param>
        public Solution(FileInfo file)
        {
            this.p_SolutionInfo = file;

            // TODO: Read the solution data from the file.
        }

        #region Disk Operations

        /// <summary>
        /// Creates a new solution on disk with the specified name in the
        /// specified directory.  The resulting location of the file will
        /// be path\name.msln
        /// </summary>
        /// <param name="name">The name of the solution.</param>
        /// <param name="path">The path to the solution.</param>
        public static Solution Create(string name, string path)
        {
            // Create the directory if it does not exist.
            if (!Directory.Exists(path))
                Directory.CreateDirectory(path);

            // Create a new empty instance.
            Solution s = new Solution();

            // Associate a FileInfo instance with it.
            s.p_SolutionInfo = new FileInfo(Path.Combine(path, name + ".msln"));

            // Request that the solution be saved.
            s.Save();

            // Return the new solution.
            return s;
        }

        /// <summary>
        /// Saves the solution file to disk; this solution must have a
        /// solution file associated with it in order to save it to disk.
        /// </summary>
        public void Save()
        {
            if (this.p_SolutionInfo == null)
                throw new InvalidOperationException();

            // Configure the settings for the XmlWriter.
            XmlWriterSettings settings = new XmlWriterSettings();
            settings.Encoding = Encoding.UTF8;
            settings.Indent = true;

            // Create the new XmlWriter.
            XmlWriter writer = XmlWriter.Create(this.p_SolutionInfo.FullName, settings);

            // Generate the XML from the project data.
            writer.WriteStartElement("Solution");

            foreach (Project p in this.p_Projects)
            {
                string rel = PathHelpers.GetRelativePath(this.p_SolutionInfo.DirectoryName, p.ProjectInfo.DirectoryName) + p.ProjectInfo.Name;
                writer.WriteStartElement("Project");
                writer.WriteString(rel);
                writer.WriteEndElement();
            }

            writer.WriteEndElement();
            writer.Close();
        }

        #endregion

        /// <summary>
        /// Converts the solution to a tree node for representation in the Solution Explorer.
        /// </summary>
        /// <returns>A new TreeNode.</returns>
        public System.Windows.Forms.TreeNode ToTreeNode()
        {
            System.Windows.Forms.TreeNode node = new System.Windows.Forms.TreeNode("Solution");
            foreach (Project p in this.p_Projects)
                node.Nodes.Add(p.ToTreeNode());
            return node;
        }

        /// <summary>
        /// Returns a list of projects within this solution.
        /// </summary>
        public List<Project> Projects
        {
            get
            {
                return this.p_Projects;
            }
            set
            {
                this.p_Projects = value;
            }
        }

        /// <summary>
        /// The FileInfo object that represents the on-disk solution file for
        /// this solution.
        /// </summary>
        public FileInfo SolutionInfo
        {
            get
            {
                return this.p_SolutionInfo;
            }
        }
    }
}
