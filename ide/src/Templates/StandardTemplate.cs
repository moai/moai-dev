using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using MOAI.Management;
using System.IO;

namespace MOAI.Templates
{
    public class StandardTemplate : BaseTemplate
    {
        public override string TemplateName
        {
            get { return "Standard Game"; }
        }

        public override string TemplateDescription
        {
            get { return "an IDE managed, MOAI project with Lua as the primary scripting language"; }
        }

        public override Bitmap TemplateIcon
        {
            get { return null; }
        }

        /// <summary>
        /// Creates a new solution using the data requested.
        /// </summary>
        /// <param name="data">The solution creation data, usually derived from the user's input in a NewSolutionForm.</param>
        /// <returns>A new solution that can be loaded.</returns>
        public override Solution Create(SolutionCreationData data)
        {
            // Create the new, empty solution on disk.
            Solution s = null;
            if (data.SolutionDirUsed)
                s = Solution.Create(data.SolutionDirName, Path.Combine(data.Path, data.SolutionDirName));
            else
                s = Solution.Create(data.Name, Path.Combine(data.Path, data.Name));

            // Create the new, empty project on disk.
            Project p = null;
            if (data.SolutionDirUsed)
                p = Project.Create(data.Name, Path.Combine(Path.Combine(data.Path, data.SolutionDirName), data.Name));
            else
                p = Project.Create(data.Name, Path.Combine(data.Path, data.Name));

            // Add the project to the solution.
            s.Projects.Add(p);

            // Save the solution back to disk.
            s.Save();

            // Return the new solution.
            return s;
        }
    }
}
