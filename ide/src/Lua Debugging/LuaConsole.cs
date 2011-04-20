using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;
using LuaInterface;
using System.Windows.Forms;
using System.Text.RegularExpressions;

namespace Roket3D.LuaDebugging
{
    class LuaConsole
    {
        public Lua pLuaVM = new Lua();
        private TextBox outputArea = null;

        public LuaConsole(TextBox log)
        {
            this.outputArea = log;

            // Override the print function.
            Type luaConsoleType = this.GetType();
            MethodInfo mInfo = luaConsoleType.GetMethod("print");
            this.pLuaVM.RegisterFunction("print", luaConsoleType, mInfo);
        }

        // DONE: Convert Unix / Mac line breaks and convert to Windows using Regular expressions (because just
        //       substituting \n with \r\n will produce the wrong result if they are already Windows line
        //       endings).
        public void HandleCommand(String command)
        {
            this.outputArea.Text += "\r\n";
            this.outputArea.Text += "> " + command.Trim();
            try
            {
                object[] result = pLuaVM.DoString(command);
                if (result != null && result.Length != 0)
                {
                    foreach (object a in result)
                    {
                        this.outputArea.Text += "\r\n";
                        Regex lineFixMac = new Regex("\r(?<end>[^\n])", RegexOptions.Multiline);
                        Regex lineFixLinux = new Regex("(?<start>[^\r])\n", RegexOptions.Multiline);
                        this.outputArea.Text += lineFixMac.Replace(
                                                    lineFixLinux.Replace(
                                                        a.ToString(),
                                                        "${start}\r\n"
                                                        ),
                                                    "\r\n${end}"
                                                    );
                    }
                }
            }
            catch (Exception ex)
            {
                this.outputArea.Text += "\r\n";
                this.outputArea.Text += ex.Message;
            }
            finally
            {
                // nothing to do here
            }
        }

        public void print()
        {
            this.outputArea.Text += "\r\n";
            this.outputArea.Text += "To inspect objects, use return and they will be printed.\n";
        }
    }
}
