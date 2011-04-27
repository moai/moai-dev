using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace MOAI
{
    static class Program
    {
        public static Manager Manager = null;

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            Program.Manager = new Manager();
            Program.Manager.Initalize();
            Program.Manager.Start();
            Application.Run();
        }
    }
}
