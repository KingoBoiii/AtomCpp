using System;
using System.Runtime.CompilerServices;

namespace Atom
{

    public class Main
    {
        public float FloatVar = 5.0f;

        public Main()
        {
            Console.WriteLine("Main constructor!");

            CppFunction();
            NativeLog("Atom Game Engine is awesome!", 235);
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
    }

}