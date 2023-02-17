using System;
using System.Runtime.CompilerServices;

namespace Atom
{

    // C# 
    // struct (value type) -> stack allocated
    // class (ref type) -> heap allocated (+ GC)
    public struct Vector3
    {
        public Vector3(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        public float X { get; set; }
        public float Y { get; set; }
        public float Z { get; set; }
    }

    public class Main
    {
        public float FloatVar = 5.0f;

        public Main()
        {
            Console.WriteLine("Main constructor!");

            CppFunction();
            NativeLog("Atom Game Engine is awesome!", 235);

            var pos = new Vector3(5, 2, 1);
            NativeLog_Vector3(ref pos);
        }

        public void PrintMessage()
        {
            Console.WriteLine("Hello world from C#!");
        }

        public void PrintCustomMessage(string message)
        {
            Console.WriteLine($"C# says: {message}");
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern static void CppFunction();

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern static void NativeLog(string text, int parameter);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern static void NativeLog_Vector3(ref Vector3 vector);
    }

}