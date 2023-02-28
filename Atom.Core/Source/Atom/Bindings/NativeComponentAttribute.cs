using System;

namespace Atom.Bindings
{

    [AttributeUsage(AttributeTargets.Class | AttributeTargets.Struct)]
    internal class NativeComponentAttribute : Attribute
    {
        public string NativeClassName { get; set; }

        public NativeComponentAttribute(string nativeClassName)
        {
            NativeClassName = nativeClassName;
        }
    }

}
