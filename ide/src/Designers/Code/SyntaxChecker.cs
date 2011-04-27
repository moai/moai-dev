using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using ScintillaNet;

namespace MOAI.Designers.Code
{
    public class SyntaxChecker
    {
        static public List<LuaError> Check(String text, Scintilla editor)
        {
            List<LuaError> errorList = new List<LuaError>();
            errorList.AddRange(CheckBasic(text, editor));
            errorList.AddRange(CheckBlocks(text, editor));
            return errorList;
        }

        static public List<LuaError> CheckBasic(String text, Scintilla editor)
        {
            // Define error regexes and return list.
            List<LuaDetector> errorRegexes = new List<LuaDetector>();
            List<LuaError> errorList = new List<LuaError>();

            /////// BEGIN REGEXES ///////
            errorRegexes.Add(new MalformedNumberDetector());
            errorRegexes.Add(new InvalidIdentifierDetector());
            errorRegexes.Add(new InvalidIdentifierConstructionDetector());
            errorRegexes.Add(new InvalidPropertyDetector());
            errorRegexes.Add(new InvalidEOLStringDetector());
            //////// END REGEXES ////////

            // Now perform the actual syntax check.
            foreach (LuaDetector r in errorRegexes)
            {
                errorList.AddRange(r.GetError(text, editor));
            }

            return errorList;
        }

        static public List<LuaError> CheckBlocks(String text, Scintilla editor)
        {
            // Temporarily disabling block checking due to
            // http://code.google.com/p/roket3d/issues/detail?id=3.
            return new List<LuaError>();

            text += "\r\n"; // add a newline at the end

            // Define block stack and return list.
            Stack<LuaBlock> blockStack = new Stack<LuaBlock>();
            List<LuaError> errorList = new List<LuaError>();

            // Define our block start/end regex.
            Regex blockStartEnd = new Regex(
                "(^|[+\\-\\*/%\\^#=><\\(\\)\\{\\}\\[\\];:,\\.\\ \\\r\n\t])(?<underline>(" +
                "(?<start>function)|" +
                "(?<start>if)[\\ \\(\r\n](.|[\r\n\\ \t])*?[\\ \\)\r\n]then|" +
                "(?<start>while)[\\ \\(\r\n](.|[\r\n\\ \t])*?[\\ \\)\r\n]do|" +
                "(?<start>repeat)|" +
                "(?<end>until)[\\ \\(\r\n](.|[\r\n\\ \t])*?[\\ \\)\r\n]|" +
                "(?<end>end)|" +
                "(?<start>for)[\\ \\(\r\n](.|[\r\n\\ \t])*?[\\ \\)\r\n]do|" +
                "(?<start>elseif)[\\ \\(\r\n](.|[\r\n\\ \t])*?[\\ \\)\r\n]then" +
                "))([+\\-\\*/%\\^#=><\\(\\)\\{\\}\\[\\];:,\\.\\ \\\r\n\t]|$)");
            MatchCollection linearBlocks = blockStartEnd.Matches(text);
            foreach (Match block in linearBlocks)
            {
                // Get the start or end block.
                Group start = block.Groups["start"];
                Group underline = block.Groups["underline"];
                Group end = block.Groups["end"];
                if (editor.Styles.GetStyleAt(underline.Index) != (byte)LexerStyleConstants.SCE_LUA_STRING &&
                    editor.Styles.GetStyleAt(underline.Index) != (byte)LexerStyleConstants.SCE_LUA_LITERALSTRING &&
                    editor.Styles.GetStyleAt(underline.Index) != (byte)LexerStyleConstants.SCE_LUA_STRINGEOL &&
                    editor.Styles.GetStyleAt(underline.Index) != (byte)LexerStyleConstants.SCE_LUA_COMMENT &&
                    editor.Styles.GetStyleAt(underline.Index) != (byte)LexerStyleConstants.SCE_LUA_COMMENTLINE &&
                    editor.Styles.GetStyleAt(underline.Index) != (byte)LexerStyleConstants.SCE_LUA_COMMENTDOC)
                {
                    if (start.Value != "")
                    {
                        // This is a start block.
                        blockStack.Push(new LuaBlock(underline.Value, underline.Index, underline.Length));
                    }
                    else if (end.Value != "")
                    {
                        // This is an end block.
                        try
                        {
                            LuaBlock startBlock = blockStack.Pop();
                            if (end.Value == "until")
                            {
                                // Only valid start block is a repeat.
                                if (startBlock.Name != "repeat")
                                {
                                    KeyValuePair<Int32, Int32> lineColumn = SyntaxChecker.LineColumnBeforeIndex(text, end.Index);
                                    errorList.Add(new LuaError(
                                        lineColumn.Key,
                                        lineColumn.Value,
                                        end.Index,
                                        end.Length,
                                        String.Format("Only repeat blocks can be terminated with 'until'.", end.Value)
                                        ));
                                }
                            }
                            else
                            {
                                // Make sure the start block is not a repeat.
                                if (startBlock.Name == "repeat")
                                {
                                    KeyValuePair<Int32, Int32> lineColumn = SyntaxChecker.LineColumnBeforeIndex(text, end.Index);
                                    errorList.Add(new LuaError(
                                        lineColumn.Key,
                                        lineColumn.Value,
                                        end.Index,
                                        end.Length,
                                        String.Format("Repeat blocks must be terminated with 'until'.", end.Value)
                                        ));
                                }
                            }
                        }
                        catch (InvalidOperationException)
                        {
                            KeyValuePair<Int32, Int32> lineColumn = SyntaxChecker.LineColumnBeforeIndex(text, end.Index);
                            errorList.Add(new LuaError(
                                lineColumn.Key,
                                lineColumn.Value,
                                end.Index,
                                end.Length,
                                String.Format("There is no matching start of block to \"{0}\".", end.Value.Replace("\r", " ").Replace("\n", " "))
                                ));
                        }
                    }
                }
            }

            if (blockStack.Count > 0)
            {
                // Go through an mark an error at the start of each block.
                foreach (LuaBlock block in blockStack)
                {
                    KeyValuePair<Int32, Int32> lineColumn = SyntaxChecker.LineColumnBeforeIndex(text, block.Index);
                    errorList.Add(new LuaError(
                        lineColumn.Key,
                        lineColumn.Value,
                        block.Index,
                        block.Length,
                        String.Format("There is no matching end of block to \"{0}\".", block.Name.Replace("\r", " ").Replace("\n", " "))
                        ));
                }
            }

            return errorList;
        }

        static public KeyValuePair<Int32, Int32> LineColumnBeforeIndex(String text, Int32 index)
        {
            // First normalise the line endings for the text string.
            Regex lineFixMac = new Regex("\r(?<end>[^\n])", RegexOptions.Multiline);
            Regex lineFixLinux = new Regex("(?<start>[^\r])\n", RegexOptions.Multiline);
            String textNew = lineFixMac.Replace(
                                lineFixLinux.Replace(
                                    text,
                                    "${start}\r\n"
                                    ),
                                "\r\n${end}"
                                );

            // Since we just modified the string, we need to increase the index by
            // the number of characters the normalization added.
            index += textNew.Length - text.Length;

            // Now count the number of lines before the specified index.
            String pattern = "\r\n"; // @"\bSethi\b" for word pattern 
            String input = textNew.Substring(0, index);
            Regex r = new Regex(pattern);
            MatchCollection matches = r.Matches(input);

            Int32 line = matches.Count + 1;

            // Calculate column.
            Int32 column = 0;
            if (matches.Count - 1 >= 0)
            {
                column = index - matches[matches.Count - 1].Index - 2; // subtract 2 because the Index is the start of the last \r\n..
            }

            return new KeyValuePair<Int32, Int32>(line, column);
        }
    }

    enum LexerStyleConstants
    {
        SCE_LUA_DEFAULT = 0,
        SCE_LUA_COMMENT = 1,
        SCE_LUA_COMMENTLINE = 2,
        SCE_LUA_COMMENTDOC = 3,
        SCE_LUA_NUMBER = 4,
        SCE_LUA_WORD_KEYWORDS = 5,
        SCE_LUA_STRING = 6,
        SCE_LUA_CHARACTER = 7,
        SCE_LUA_LITERALSTRING = 8,
        SCE_LUA_PREPROCESSOR = 9,
        SCE_LUA_OPERATOR = 10,
        SCE_LUA_IDENTIFIER = 11,
        SCE_LUA_STRINGEOL = 12,
        SCE_LUA_WORD_BASIC_FUNCTIONS = 13,
        SCE_LUA_WORD_BASIC_CLASSES = 14,
        SCE_LUA_WORD_UNUSED = 15,
        SCE_LUA_WORD_MOAI_CLASSES = 16,
        SCE_LUA_WORD_CLASS_KEYWORDS = 17,
        SCE_LUA_WORD7 = 18,
        SCE_LUA_WORD8 = 19
    }

    enum LexerKeywordGroupConstants
    {
        LUA_KEYWORDS = 0,
        LUA_BASIC_FUNCTIONS = 1,
        LUA_BASIC_CLASSES = 2,
        LUA_UNUSED = 3,
        LUA_ROKET3D_CLASSES = 4,
        LUA_CLASS_KEYWORDS = 5,
        LUA_USER3 = 6,
        LUA_USER4 = 7
    }

    #region Error Detection Classes

    // Detects '1var'
    public partial class InvalidIdentifierDetector : LuaRegexDetector
    {
        public InvalidIdentifierDetector()
        {
            rule = new Regex("[+\\-\\*/%\\^#=><\\(\\)\\{\\}\\[\\];:,\\.\\ \\\r\n](?<identifier>[^a-zA-Z_][0-9]+[a-zA-Z_]+)", RegexOptions.Multiline);
            errorMsg = "\"{0}\" is an invalid identifier.";
            //indexOffset = -1;
        }
    }

    // Detects '1.0.0'
    public partial class MalformedNumberDetector : LuaRegexDetector
    {
        public MalformedNumberDetector()
        {
            rule = new Regex("[+\\-\\*/%\\^#=><\\(\\)\\{\\}\\[\\];:,\\.\\ \\\r\n][^a-zA-Z_](?<identifier>[0-9]+\\.([0-9]+\\.)+([0-9]+)?)", RegexOptions.Multiline);
            errorMsg = "\"{0}\" is a malformed number.";
        }
    }

    // Detects 'identifier anotherone'
    public partial class InvalidIdentifierConstructionDetector : LuaRegexDetector
    {
        public InvalidIdentifierConstructionDetector()
        {
            rule = new Regex("[+\\-\\*/%\\^#=><\\(\\)\\{\\}\\[\\];:,\\.\\ \\\r\n](?<identifier>(?<first>[a-zA-Z_]+)\\ (?<second>[a-zA-Z_]+))", RegexOptions.Multiline);
            errorMsg = "\"{0}\" is an invalid identifier construction.  Identifiers can not contain spaces, and the first component is not considered a Lua keyword.";
        }

        override public List<LuaError> GetError(String text, Scintilla editor)
        {
            List<LuaError> errorList = new List<LuaError>();
            MatchCollection mc = this.rule.Matches(text);
            foreach (Match m in mc)
            {
                // We use the scintilla editor object to determine whether
                // the match is actually inside a string.
                Int32 errorIndex = m.Groups["identifier"].Index;
                Int32 errorLength = m.Groups["identifier"].Length;
                if (editor.Styles.GetStyleAt(errorIndex) != (byte)LexerStyleConstants.SCE_LUA_STRING &&
                    editor.Styles.GetStyleAt(errorIndex) != (byte)LexerStyleConstants.SCE_LUA_LITERALSTRING &&
                    editor.Styles.GetStyleAt(errorIndex) != (byte)LexerStyleConstants.SCE_LUA_STRINGEOL &&
                    editor.Styles.GetStyleAt(errorIndex) != (byte)LexerStyleConstants.SCE_LUA_COMMENT &&
                    editor.Styles.GetStyleAt(errorIndex) != (byte)LexerStyleConstants.SCE_LUA_COMMENTLINE &&
                    editor.Styles.GetStyleAt(errorIndex) != (byte)LexerStyleConstants.SCE_LUA_COMMENTDOC)
                {
                    // We must do additional checks to make sure the first and second groups are
                    // not Lua keywords.
                    String[] keywords = {"and", "break", "do", "else", "elseif",
                                                "end", "false", "for", "function", "if",
                                                "in", "is", "local", "nil", "not", "or",
                                                "repeat", "return", "then", "true", "until",
                                                "while"};
                    List<String> keywordsList = keywords.ToList<String>();

                    if (keywordsList.IndexOf(m.Groups["first"].Value) != -1 ||
                        keywordsList.IndexOf(m.Groups["second"].Value) != -1)
                    {
                        // Skip this error because it actually involves a keyword.
                        continue;
                    }

                    KeyValuePair<Int32, Int32> lineColumn = SyntaxChecker.LineColumnBeforeIndex(text, errorIndex);
                    errorList.Add(new LuaError(
                        lineColumn.Key,
                        lineColumn.Value,
                        errorIndex,
                        errorIndex + indexOffset,
                        errorLength,
                        String.Format(this.errorMsg, m.Groups["identifier"].Value.Trim())
                        ));
                }
            }
            return errorList;
        }
    }

    // Detects 'v1.5'
    public partial class InvalidPropertyDetector : LuaRegexDetector
    {
        public InvalidPropertyDetector()
        {
            rule = new Regex("[+\\-\\*/%\\^#=><\\(\\)\\{\\}\\[\\];:,\\.\\ \\\r\n](?<identifier>[a-zA-Z_]+?[a-zA-Z_0-9]*?\\.[0-9]*)[^a-zA-Z_]+", RegexOptions.Multiline);
            errorMsg = "\"{0}\" is an invalid property request.  Properties must be valid identifiers.";
        }
    }

    // Detects '"some string<newline>'
    public partial class InvalidEOLStringDetector : LuaDetector
    {
        public InvalidEOLStringDetector()
        {
            errorMsg = "\"{0}\" does not have a terminating quote.  Use [[ ... ]] for multline quotes.";
        }

        override public List<LuaError> GetError(String text, Scintilla editor)
        {
            // Search the editor's characters for SCE_LUA_STRINGEOL.
            Boolean insideInvalidEOL = false;
            String textBuffer = "";
            Int32 errorIndex = 0;
            List<LuaError> errorList = new List<LuaError>();
            for (Int32 currentIndex = 0; currentIndex < text.Length; currentIndex += 1)
            {
                Byte characterStyle = editor.Styles.GetStyleAt(currentIndex);
                if (characterStyle == (byte)LexerStyleConstants.SCE_LUA_STRINGEOL)
                {
                    if (insideInvalidEOL)
                    {
                        textBuffer += text[currentIndex];
                    }
                    else
                    {
                        insideInvalidEOL = true;
                        errorIndex = currentIndex;
                        textBuffer = ""; // by not setting this to the character, we omit the leading " which
                        // is normally included in the STRINGEOL section.
                    }
                }
                else if (insideInvalidEOL)
                {
                    KeyValuePair<Int32, Int32> lineColumn = SyntaxChecker.LineColumnBeforeIndex(text, errorIndex);
                    errorList.Add(new LuaError(
                        lineColumn.Key,
                        lineColumn.Value,
                        errorIndex,
                        textBuffer.Length,
                        String.Format(this.errorMsg, textBuffer.Trim())
                        ));
                    insideInvalidEOL = false;
                    errorIndex = 0;
                    textBuffer = "";
                }
            }
            return errorList;
        }
    }

    #endregion

    #region Base Classes
    public partial class LuaRegexDetector : LuaDetector
    {
        public Regex rule = null;
        public List<Regex> rules = null;
        public Int32 indexOffset = 0;

        public LuaRegexDetector() { }

        override public List<LuaError> GetError(String text, Scintilla editor)
        {
            List<LuaError> errorList = new List<LuaError>();
            if (this.rules == null)
            {
                errorList.AddRange(RunRegex(this.rule, text, editor));
            }
            else
            {
                foreach (Regex r in this.rules)
                {
                    errorList.AddRange(RunRegex(r, text, editor));
                }
            }
            return errorList;
        }

        private List<LuaError> RunRegex(Regex r, String text, Scintilla editor)
        {
            List<LuaError> errorList = new List<LuaError>();
            MatchCollection mc = r.Matches(text);
            foreach (Match m in mc)
            {
                // We use the scintilla editor object to determine whether
                // the match is actually inside a string.
                Int32 errorIndex = m.Groups["identifier"].Index;
                Int32 errorLength = m.Groups["identifier"].Length;
                if (editor.Styles.GetStyleAt(errorIndex) != (byte)LexerStyleConstants.SCE_LUA_STRING &&
                    editor.Styles.GetStyleAt(errorIndex) != (byte)LexerStyleConstants.SCE_LUA_LITERALSTRING &&
                    editor.Styles.GetStyleAt(errorIndex) != (byte)LexerStyleConstants.SCE_LUA_STRINGEOL &&
                    editor.Styles.GetStyleAt(errorIndex) != (byte)LexerStyleConstants.SCE_LUA_COMMENT &&
                    editor.Styles.GetStyleAt(errorIndex) != (byte)LexerStyleConstants.SCE_LUA_COMMENTLINE &&
                    editor.Styles.GetStyleAt(errorIndex) != (byte)LexerStyleConstants.SCE_LUA_COMMENTDOC)
                {
                    KeyValuePair<Int32, Int32> lineColumn = SyntaxChecker.LineColumnBeforeIndex(text, errorIndex);
                    errorList.Add(new LuaError(
                        lineColumn.Key,
                        lineColumn.Value,
                        errorIndex,
                        errorIndex + indexOffset,
                        errorLength,
                        String.Format(this.errorMsg, m.Groups["identifier"].Value.Trim())
                        ));
                }
            }
            return errorList;
        }
    }

    public abstract class LuaDetector
    {
        public String errorMsg;

        public LuaDetector() { }

        abstract public List<LuaError> GetError(String text, Scintilla editor);
    }

    public class LuaError
    {
        public Int32 Line;
        public Int32 Column;
        public Int32 Index;
        public Int32 IndicatorIndex;
        public Int32 Length;
        public String ErrorMsg;

        public LuaError(Int32 Line, Int32 Column, Int32 Index, Int32 Length, String ErrorMsg)
        {
            this.Line = Line;
            this.Column = Column;
            this.Index = Index;
            this.IndicatorIndex = Index;
            this.Length = Length;
            this.ErrorMsg = ErrorMsg;
        }

        public LuaError(Int32 Line, Int32 Column, Int32 Index, Int32 IndicatorIndex, Int32 Length, String ErrorMsg)
        {
            this.Line = Line;
            this.Column = Column;
            this.Index = Index;
            this.IndicatorIndex = IndicatorIndex;
            this.Length = Length;
            this.ErrorMsg = ErrorMsg;
        }
    }

    public class LuaBlock
    {
        public String Name;
        public Int32 Index;
        public Int32 Length;

        public LuaBlock(String Name, Int32 Index, Int32 Length)
        {
            this.Name = Name;
            this.Index = Index;
            this.Length = Length;
        }
    }

    #endregion
}
