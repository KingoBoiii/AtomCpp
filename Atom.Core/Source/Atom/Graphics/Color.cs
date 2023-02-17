using System.Runtime.InteropServices;

namespace Atom
{

    [StructLayout(LayoutKind.Sequential)]
    public struct Color
    {
        public Color(float r, float g, float b, float a)
        {
            R = r;
            G = g;
            B = b;
            A = a;
        }

        public float R { get; set; }
        public float G { get; set; }
        public float B { get; set; }
        public float A { get; set; }
    }

}
