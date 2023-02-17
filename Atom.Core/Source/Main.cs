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
            Log.Info("Main constructor!");

            Log.Trace("Trace");
            Log.Information("Info");
            Log.Warning("Warn");
            Log.Error("Error");
        }

        public void PrintMessage()
        {
            Log.Trace("Hello world from C#!");
        }

        public void PrintCustomMessage(string message)
        {
            Log.Information($"C# says: {message}");
        }
    }

}