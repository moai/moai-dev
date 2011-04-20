using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Roket3D.Tools;

namespace Roket3D.Management
{
    public class Solution
    {
        private List<Project> m_Projects = new List<Project>();

        public System.Windows.Forms.TreeNode ToTreeNode()
        {
            System.Windows.Forms.TreeNode node = new System.Windows.Forms.TreeNode("Solution");
            foreach (Project p in this.m_Projects)
                node.Nodes.Add(p.ToTreeNode());
            return node;
        }

        public List<Project> Projects
        {
            get
            {
                return this.m_Projects;
            }
            set
            {
                this.m_Projects = value;
            }
        }
    }
}
