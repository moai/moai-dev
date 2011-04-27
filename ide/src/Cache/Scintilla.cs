using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MOAI.Cache
{
    public class Scintilla
    {
        private Stack<ScintillaNet.Scintilla> p_Scintillas = null;

        /// <summary>
        /// Initalizes a Scintilla caching object.
        /// </summary>
        public Scintilla()
        {
            this.p_Scintillas = new Stack<ScintillaNet.Scintilla>();
        }

        /// <summary>
        /// Initalizes the cache to contain a specified amount of scintilla editors.
        /// The callback onProgress is called after each editor is created.
        /// </summary>
        /// <param name="amount"></param>
        /// <param name="onProgress"></param>
        public void InitialCache(int amount, ProgressCallback onProgress)
        {
            for (int i = 0; i < amount; i += 1)
            {
                this.p_Scintillas.Push(new ScintillaNet.Scintilla());
                onProgress(i + 1);
            }
        }

        /// <summary>
        /// Takes a scintilla editor from the cache and returns it.
        /// </summary>
        /// <returns>A new scintilla editor.</returns>
        public ScintillaNet.Scintilla Take()
        {
            if (this.p_Scintillas.Count == 0)
                return new ScintillaNet.Scintilla();
            else
                return this.p_Scintillas.Pop();
        }

        /// <summary>
        /// Returns a scintilla editor to the cache once it has been used.
        /// </summary>
        /// <param name="scintilla"></param>
        public void Return(ScintillaNet.Scintilla scintilla)
        {
            this.p_Scintillas.Push(scintilla);
        }
    }
}
