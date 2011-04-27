using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Roket3D.Xml;
using System.Threading;
using System.IO;

namespace Roket3D.Compilation
{
    public enum BuildMode
    {
        DEBUG,
        RELEASE
    }

    public class ProjectBuilder
    {
        Project m_Project;
        ToolBuildOutput m_Output;

        public ProjectBuilder(Project Project, ToolBuildOutput Output)
        {
            this.m_Project = Project;
            this.m_Output = Output;
        }

        public void Build(BuildMode Mode)
        {
            Thread bThread = new Thread(new ParameterizedThreadStart(this.BuildThread));
            bThread.Start((Object)Mode);
        }

        void BuildThread(object RawMode)
        {
            BuildMode Mode = (BuildMode)RawMode;

            this.LogClear();
            this.Log("1) Starting build...");
            this.Log("2)   Verifing library files...");
            DirectoryInfo LibraryFolder = new DirectoryInfo(Program.ROOT_PATH + "\\Libraries");
            if (!LibraryFolder.Exists)
            {
                this.Log("X) Libraries support folder not found.  Stop.");
                return;
            }

            // Attempt to get the folder with the engine files in it, depending on the build mode.
            if (Mode == BuildMode.DEBUG)
            {
                try
                {
                    LibraryFolder = LibraryFolder.GetDirectories("Debug")[0];
                }
                catch
                {
                    this.Log("X) Libraries/Debug support folder not found.  Stop.");
                    return;
                }
            }
            else if (Mode == BuildMode.RELEASE)
            {
                try
                {
                    LibraryFolder = LibraryFolder.GetDirectories("Release")[0];
                }
                catch
                {
                    this.Log("X) Libraries/Release support folder not found.  Stop.");
                    return;
                }
            }

            // Now fetch a list of the engine files.
            FileInfo[] SourceFiles = LibraryFolder.GetFiles();
            FileInfo[] DestinationFiles = this.m_Project.GetRootFolder().GetFiles();
            Dictionary<String, FileInfo> DestinationDict = DestinationFiles.ToDictionary(
                new Func<FileInfo,String>(
                    delegate(FileInfo target)
                    {
                        return target.Name;
                    }
                    ));

            foreach (FileInfo sF in SourceFiles)
            {
                Boolean ShouldCopy = false;
                if (DestinationDict.Keys.Contains(sF.Name))
                {
                    // Check last modified.
                    if (DestinationDict[sF.Name].LastWriteTimeUtc < sF.LastWriteTimeUtc)
                    {
                        // Copy.
                        ShouldCopy = true;
                    }
                    else
                    {
                        // Skip.
                    }
                }
                else
                {
                    // Copy.
                    ShouldCopy = true;
                }

                if (ShouldCopy)
                {
                    try
                    {
                        sF.CopyTo(this.m_Project.GetRootFolder() + "\\" + sF.Name, true);
                        this.Log("3)     Copying " + sF.Name + "... success.");
                    }
                    catch
                    {
                        this.Log("3)     Copying " + sF.Name + "... failure.");
                        this.Log("X) Unable to copy required library support file.  Stop.");
                        return;
                    }
                }
            }

            // Finished copying support files.
            this.Log("2)   Library files are up-to-date.");
        }

        void Log(String Message)
        {
            if (this.m_Output.InvokeRequired)
                this.m_Output.Invoke(new LogD(LogP), new object[] { Message });
            else
                this.m_Output.AddLogEntry(Message);
        }

        delegate void LogD(String Message);
        void LogP(String Message)
        {
            this.m_Output.AddLogEntry(Message);
        }

        void LogClear()
        {
            if (this.m_Output.InvokeRequired)
                this.m_Output.Invoke(new LogClearD(LogClearP));
            else
                this.m_Output.ClearLog();
        }

        delegate void LogClearD();
        void LogClearP()
        {
            this.m_Output.ClearLog();
        }
    }
}
