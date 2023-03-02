using System;

namespace Atom
{

    [AttributeUsage(AttributeTargets.Field)]
    public class VisibleInEditorAttribute : Attribute
    {
        public VisibleInEditorAttribute()
        {
        }
    }

}
