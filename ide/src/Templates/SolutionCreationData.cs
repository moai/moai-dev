using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MOAI.Templates
{
    public struct SolutionCreationData
    {
        public string Path;
        public string Name;
        public Boolean SolutionDirUsed;
        public string SolutionDirName;
        public BaseTemplate Template;

        public SolutionCreationData(string path, string name, Boolean solutionDirUsed, string solutionDirName, BaseTemplate template)
        {
            this.Path = path;
            this.Name = name;
            this.SolutionDirUsed = solutionDirUsed;
            this.SolutionDirName = solutionDirName;
            this.Template = template;
        }
    }
}
