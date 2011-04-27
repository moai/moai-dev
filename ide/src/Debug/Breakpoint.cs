using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MOAI.Debug
{
    public class Breakpoint
    {
        private string p_SourceFile;
        private int p_SourceLine;

        /// <summary>
        /// Creates a new Breakpoint object that represents a debugging
        /// breakpoint.
        /// </summary>
        /// <param name="file">The file which the breakpoint should be placed in.</param>
        /// <param name="line">The line number on which to break.</param>
        public Breakpoint(string file, int line)
        {
            this.p_SourceFile = file;
            this.p_SourceLine = line;
        }

        /// <summary>
        /// The file which the breakpoint is associated with.
        /// </summary>
        public string SourceFile
        {
            get
            {
                return this.p_SourceFile;
            }
        }

        /// <summary>
        /// The line on which this breakpoint will be triggered.
        /// </summary>
        public int SourceLine
        {
            get
            {
                return this.p_SourceLine;
            }
        }
    }
}
