using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MOAI.Cache
{
    public class Manager
    {
        private Scintilla p_ScintillaCache = null;
        private AutoComplete p_AutoComplete = null;

        /// <summary>
        /// Creates a new cache manager, which manages all of the sub-caches.
        /// </summary>
        public Manager()
        {
            this.p_ScintillaCache = new Scintilla();
            this.p_AutoComplete = new AutoComplete();
            this.p_AutoComplete.Import();
        }

        /// <summary>
        /// The scintilla caching object.
        /// </summary>
        public Scintilla ScintillaCache
        {
            get
            {
                return this.p_ScintillaCache;
            }
        }

        /// <summary>
        /// The autocomplete caching object.
        /// </summary>
        public AutoComplete AutoComplete
        {
            get
            {
                return this.p_AutoComplete;
            }
        }
    }

    public delegate void ProgressCallback(int progress);
}
