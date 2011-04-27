using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using MOAI.Management;

namespace MOAI.Designers.Code
{
    public class CodeEditor : ScintillaNet.Scintilla
    {
        private bool p_CanSyntaxCheck = true;
        private Timer c_SyntaxLimitTimer = new Timer();
        private Cache.Manager m_Manager = null;

        public event EventHandler SyntaxCheckRequested;

        public CodeEditor(Cache.Manager manager, bool highlight)
            : base()
        {
            this.m_Manager = manager;

            // Configure the basic settings.
            this.CallTip.BackColor = System.Drawing.SystemColors.Window;
            this.Dock = System.Windows.Forms.DockStyle.Fill;
            this.LineWrap.Mode = ScintillaNet.WrapMode.Word;
            this.Location = new System.Drawing.Point(0, 0);
            this.LongLines.EdgeColumn = 160;
            this.LongLines.EdgeMode = ScintillaNet.EdgeMode.Line;
            this.Margins.Margin2.Width = 16;
            this.Name = "scintillaEditor";
            this.Size = new System.Drawing.Size(553, 534);
            this.Styles.BraceBad.BackColor = System.Drawing.SystemColors.Window;
            this.Styles.BraceBad.FontName = "Courier New";
            this.Styles.BraceBad.Size = 8.25F;
            this.Styles.BraceLight.BackColor = System.Drawing.SystemColors.Window;
            this.Styles.BraceLight.FontName = "Courier New";
            this.Styles.BraceLight.Size = 8.25F;
            this.Styles.CallTip.BackColor = System.Drawing.SystemColors.Window;
            this.Styles.CallTip.FontName = "Courier New";
            this.Styles.ControlChar.BackColor = System.Drawing.SystemColors.Window;
            this.Styles.ControlChar.FontName = "Courier New";
            this.Styles.ControlChar.Size = 8.25F;
            this.Styles.Default.BackColor = System.Drawing.SystemColors.Window;
            this.Styles.Default.CharacterSet = ScintillaNet.CharacterSet.Ansi;
            this.Styles.Default.FontName = "Courier New";
            this.Styles.Default.Size = 8.25F;
            this.Styles.IndentGuide.BackColor = System.Drawing.SystemColors.Window;
            this.Styles.IndentGuide.FontName = "Courier New";
            this.Styles.IndentGuide.Size = 8.25F;
            this.Styles.LastPredefined.BackColor = System.Drawing.SystemColors.Window;
            this.Styles.LastPredefined.FontName = "Courier New";
            this.Styles.LastPredefined.Size = 8.25F;
            this.Styles.LineNumber.FontName = "Courier New";
            this.Styles.LineNumber.Size = 8.25F;
            this.Styles.Max.BackColor = System.Drawing.SystemColors.Window;
            this.Styles.Max.FontName = "Courier New";
            this.Styles.Max.Size = 8.25F;
            this.TabIndex = 0;

            // Set up the margins.
            this.Margins.Left = 5;
            this.Margins[0].Width = 45;
            this.Margins[2].Width = 16;

            // Set the dwell time.
            this.NativeInterface.SetMouseDwellTime(200);

            // Set up the lexer.
            if (highlight)
                this.ConfigurationManager.Language = "lua";
            else
                this.ConfigurationManager.Language = "null";
            this.Styles.LineNumber.BackColor = Color.White;
            this.NativeInterface.SetFoldMarginColour(true, 0xE6F0FA); // Color.FromArgb(250, 240, 230)
            this.NativeInterface.SetFoldMarginHiColour(true, 0xF5F5F5); // Color.FromArgb(245, 245, 245)

            // Configure autocomplete.
            this.CharAdded += new EventHandler<ScintillaNet.CharAddedEventArgs>(CodeEditor_CharAdded);
            this.AutoComplete.RegisterImages(Program.Manager.CacheManager.AutoComplete.Images, Color.Black);

            // Configure keywords.
            this.SetKeywords(LexerKeywordGroupConstants.LUA_KEYWORDS,
                "and break catch do else elseif " +
                "end false for function if " +
                "in is local nil not or repeat return " +
                "then throw true try until when while");
            this.SetKeywords(LexerKeywordGroupConstants.LUA_BASIC_FUNCTIONS,
                "_G _VERSION assert collectgarbage " +
                "dofile error getfenv getmetatable " +
                "ipairs load loadfile loadstring " +
                "module next pairs pcall print " +
                "rawequal rawget rawset require " +
                "select setfenv setmetatable tonumber " +
                "tostring type unpack xpcall");
            /*this.SetKeywords(LexerKeywordGroupConstants.LUA_BASIC_CLASSES,
                "Math String Table Coroutine Debug IO OS Package");

            
                // Math
                "math.abs math.acos math.asin math.atan math.atan2 math.ceil math.cos " +
                "math.cosh math.deg math.exp math.floor math.fmod math.frexp math.huge " +
                "math.ldexp math.log math.log10 math.max math.min math.modf math.pi " +
                "math.pow math.rad math.random math.randomseed math.sin math.sinh " +
                "math.sqrt math.tan math.tanh " +
                // String
                "string.byte string.char string.dump string.find string.format " +
                "string.gmatch string.gsub string.len string.lower string.match " +
                "string.rep string.reverse string.sub string.upper " +
                // Table
                "table.concat table.insert table.maxn table.remove table.sort"
                     
            this.SetKeywords(LexerKeywordGroupConstants.LUA_COROUTINE_IO_SYSTEM,
                // Coroutine
                "coroutine.create coroutine.resume coroutine.running coroutine.status " +
                "coroutine.wrap coroutine.yield " +
                // Debug
                "debug.debug debug.getfenv debug.gethook debug.getinfo debug.getlocal " +
                "debug.getmetatable debug.getregistry debug.getupvalue debug.setfenv " +
                "debug.sethook debug.setlocal debug.setmetatable debug.setupvalue " +
                "debug.traceback " +
                // IO
                "io.close io.flush io.input io.lines io.open io.output io.popen " +
                "io.read io.stderr io.stdin io.stdout io.tmpfile io.type io.write " +
                // OS
                "os.clock os.date os.difftime os.execute os.exit os.getenv os.remove " +
                "os.rename os.setlocale os.time os.tmpname " +
                // Package
                "package.cpath package.loaded package.loaders package.loadlib " +
                "package.path package.preload package.seeall");
            
            this.SetKeywords(LexerKeywordGroupConstants.LUA_ROKET3D_CLASSES, string.Join(" ", this.m_Manager.DefinedClasses.AllClasses.ToArray()));*/
            this.SetKeywords(LexerKeywordGroupConstants.LUA_CLASS_KEYWORDS,
                // Class keywords
                "class inherits context visibility private protected public static instance");

            // Configure colours.
            this.SetStyle(LexerStyleConstants.SCE_LUA_DEFAULT, Color.FromArgb(0, 0, 0));
            this.SetStyle(LexerStyleConstants.SCE_LUA_COMMENT, Color.FromArgb(0, 128, 0));
            this.SetStyle(LexerStyleConstants.SCE_LUA_COMMENTLINE, Color.FromArgb(0, 128, 0));
            this.SetStyle(LexerStyleConstants.SCE_LUA_COMMENTDOC, Color.FromArgb(0, 128, 0));
            this.SetStyle(LexerStyleConstants.SCE_LUA_NUMBER, Color.FromArgb(0, 0, 153));
            this.SetStyle(LexerStyleConstants.SCE_LUA_STRING, Color.FromArgb(163, 21, 21));
            this.SetStyle(LexerStyleConstants.SCE_LUA_CHARACTER, Color.FromArgb(163, 21, 21));
            this.SetStyle(LexerStyleConstants.SCE_LUA_LITERALSTRING, Color.FromArgb(163, 21, 21));
            this.SetStyle(LexerStyleConstants.SCE_LUA_OPERATOR, Color.FromArgb(0, 0, 0));
            this.SetStyle(LexerStyleConstants.SCE_LUA_IDENTIFIER, Color.FromArgb(0, 0, 0));
            this.SetStyle(LexerStyleConstants.SCE_LUA_WORD_KEYWORDS, Color.FromArgb(21, 24, 255));
            this.SetStyle(LexerStyleConstants.SCE_LUA_WORD_BASIC_FUNCTIONS, Color.FromArgb(21, 24, 255));
            this.SetStyle(LexerStyleConstants.SCE_LUA_WORD_BASIC_CLASSES, Color.FromArgb(0, 128, 128));
            this.SetStyle(LexerStyleConstants.SCE_LUA_WORD_UNUSED, Color.FromArgb(21, 24, 255));
            this.SetStyle(LexerStyleConstants.SCE_LUA_WORD_MOAI_CLASSES, Color.FromArgb(0, 128, 128));
            this.SetStyle(LexerStyleConstants.SCE_LUA_WORD_CLASS_KEYWORDS, Color.FromArgb(160, 0, 200));

            // Not yet configured.
            this.SetStyle(LexerStyleConstants.SCE_LUA_PREPROCESSOR, Color.FromArgb(127, 127, 0));
            this.SetStyle(LexerStyleConstants.SCE_LUA_STRINGEOL, Color.FromArgb(163, 21, 21));

            // Configure indicators.
            this.Indicators[0].Style = ScintillaNet.IndicatorStyle.Squiggle;
            this.Indicators[0].Color = Color.Red;

            if (highlight)
            {
                // Start the timer so that we get proper syntax checking after the lexer
                // has finished lexing the text (unfortunatly there is no event for when
                // the lexer has finished parsing the text).
                this.c_SyntaxLimitTimer.Interval = 500;
                this.c_SyntaxLimitTimer.Start();
                this.c_SyntaxLimitTimer.Tick += new EventHandler(c_SyntaxLimitTimer_Tick);

                // Configure other events.
                this.KeyUp += new KeyEventHandler(CodeEditor_KeyUp);
            }
            else
            {
                // Disable syntax checking.
                this.p_CanSyntaxCheck = false;
            }
        }

        /// <summary>
        /// This event is raised when the syntax timer ticks.
        /// </summary>
        /// <param name="sender">The sender of the event.</param>
        /// <param name="e">The event information.</param>
        private void c_SyntaxLimitTimer_Tick(object sender, EventArgs e)
        {
            this.p_CanSyntaxCheck = true;
            this.c_SyntaxLimitTimer.Stop();

            if (this.SyntaxCheckRequested != null)
                this.SyntaxCheckRequested(this, new EventArgs());
        }

        /// <summary>
        /// This event is raised when a key is released.
        /// </summary>
        /// <param name="sender">The sender of the event.</param>
        /// <param name="e">The event information.</param>
        private void CodeEditor_KeyUp(object sender, KeyEventArgs e)
        {
            this.p_CanSyntaxCheck = false;
            if (this.SyntaxCheckRequested != null)
                this.SyntaxCheckRequested(this, new EventArgs());

            this.c_SyntaxLimitTimer.Stop();
            this.c_SyntaxLimitTimer.Start();

            // Check the last character.
            int lci = this.Selection.Start - 1;
            if (lci < 0 || lci > this.Text.Length) return;
            char lc = this.Text[lci];
            if ((lc == ' ' || lc == '\t' || lc == '\n' || lc == '\r') && e.KeyCode == Keys.ShiftKey)
            {
                // Show global list.
                this.AutoComplete.List = Program.Manager.CacheManager.AutoComplete.GetList(null);
                //this.AutoComplete.Show();
            }
        }

        void CodeEditor_CharAdded(object sender, ScintillaNet.CharAddedEventArgs e)
        {
            // Check the last non-space character.
            int lci = this.Selection.Start - 1;
            char lc = this.Text[lci];
            while (lci >= 0)
            {
                if (lc != ' ' && lc != '\t' && lc != '\n' && lc != '\r')
                    break;

                lc = this.Text[lci];
                lci -= 1;
            }
            if (lci == -1)
                return;

            if (lc == '.')
            {
                // Scan back.
                int b = this.Selection.Start - 1;
                int i = b;
                string s = "";
                char c = this.Text[i];
                while (true)
                {
                    if (c != '.' && (c < 'a' || c > 'z') && (c < 'A' || c > 'Z'))
                        break;

                    if (c == '.')
                    {
                        s = s.Insert(0, this.Text.Substring(i + 1, b - i));
                        b = i;
                    }

                    i -= 1;
                    c = this.Text[i];
                }
                if (b != i)
                {
                    s = s.Insert(0, this.Text.Substring(i + 1, b - i));
                    b = i;
                }
                if (s.Length == 0) return;
                else s = s.Substring(0, s.Length - 1);

                this.AutoComplete.List = Program.Manager.CacheManager.AutoComplete.GetList(s);
                //this.AutoComplete.Show();
            }
            else if (lc == '=')
            {
                // Show global list.
                this.AutoComplete.List = Program.Manager.CacheManager.AutoComplete.GetList(null);
                //this.AutoComplete.Show();
            }
        }

        /// <summary>
        /// Sets the foreground color for a set of keywords.
        /// </summary>
        /// <param name="style">The enumeration indicating the type of keyword.</param>
        /// <param name="forecolor">The foreground color for the keyword.</param>
        private void SetStyle(LexerStyleConstants style, Color forecolor)
        {
            this.Styles[(Int32)style].ForeColor = forecolor;
            this.Styles[(Int32)style].BackColor = Color.Transparent;
        }

        /// <summary>
        /// Sets the keywords for a specific set of keywords.
        /// </summary>
        /// <param name="group">The enumeration indicating the type of keyword.</param>
        /// <param name="keywordslist">A string containing all of the keywords.</param>
        private void SetKeywords(LexerKeywordGroupConstants group, string keywordslist)
        {
            this.Lexing.SetKeywords((Int32)group, keywordslist);
        }

        /// <summary>
        /// Instructs the editor to update it's list of keywords with those in the cache.
        /// This function should be called whenever a new class is defined or modified.
        /// </summary>
        public void UpdateClassKeywords()
        {
            //this.SetKeywords(LexerKeywordGroupConstants.LUA_ROKET3D_CLASSES,
            //    string.Join(" ", this.m_Manager.DefinedClasses.AllClasses.ToArray()));
        }

        /// <summary>
        /// Whether the editor can perform syntax checking at this time.
        /// </summary>
        public bool CanSyntaxCheck
        {
            get
            {
                return this.p_CanSyntaxCheck;
            }
        }
    }
}