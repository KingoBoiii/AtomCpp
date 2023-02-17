namespace Atom
{

    public static class Log
    {
        public static void Trace(string message)
        {
            InternalCalls.Log_Trace(message);
        }

        public static void Info(string message) => Information(message);

        public static void Information(string message)
        {
            InternalCalls.Log_Information(message);
        }

        public static void Warning(string message)
        {
            InternalCalls.Log_Warning(message);
        }

        public static void Error(string message)
        {
            InternalCalls.Log_Error(message);
        }
    }

}
