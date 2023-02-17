namespace Atom
{

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