using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MOAI.Menus
{
    class EnumWrapper
    {
        int EnumValue;
        List<String> EnumStringReps;

        public EnumWrapper(Int32 Value, List<String> StringRepresentations)
        {
            this.EnumValue = Value;
            this.EnumStringReps = StringRepresentations;
        }

        public override String ToString()
        {
            try
            {
                return this.EnumStringReps[EnumValue];
            }
            catch
            {
                return "Enum Value: " + this.EnumValue.ToString();
            }
        }
    }
}
