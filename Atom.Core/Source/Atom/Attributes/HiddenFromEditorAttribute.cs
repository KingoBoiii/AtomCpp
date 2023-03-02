using System;

namespace Atom
{

    [AttributeUsage(AttributeTargets.Field)]
    public class HiddenFromEditorAttribute : Attribute
    {
        public HiddenFromEditorAttribute()
        {
        }
    }

}
