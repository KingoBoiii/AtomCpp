using System;

namespace Atom
{

    public class Main
    {
        public float FloatVar = 5.0f;

        public Main()
        {
            Console.WriteLine("Main constructor!");
        }

        public void PrintMessage()
        {
            Console.WriteLine("Hello world from C#!");
        }

        public void PrintCustomMessage(string message)
        {
            Console.WriteLine($"C# says: {message}");
        }

    }

}