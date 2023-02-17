using System.Runtime.CompilerServices;

namespace Atom
{

    internal static class InternalCalls
    {
        #region Log

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void Log_Trace(string message);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void Log_Information(string message);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void Log_Warning(string message);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void Log_Error(string message);

        #endregion
    }

}
