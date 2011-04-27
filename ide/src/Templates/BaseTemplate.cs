using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using MOAI.Management;

namespace MOAI.Templates
{
    public abstract class BaseTemplate
    {
        public abstract string TemplateName { get; }
        public abstract string TemplateDescription { get; }
        public abstract Bitmap TemplateIcon { get; }

        public abstract Solution Create(SolutionCreationData data);
    }
}
