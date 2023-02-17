using System;
using System.Runtime.CompilerServices;

namespace Atom
{

    internal static class InternalCalls
    {
        #region Entity

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool Entity_HasComponent(ulong entityId, Type componentType);

        #endregion

        #region Identifier

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Identifier_GetName(ulong entityId, out string name);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Identifier_SetName(ulong entityId, ref string name);

        #endregion

        #region Transform

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Transform_GetPosition(ulong entityId, out Vector3 position);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Transform_SetPosition(ulong entityId, ref Vector3 position);

        #endregion

        #region Input

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool Input_IsKeyDown(KeyCode keycode);

        #endregion

        #region Log

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Log_LogMessage(Log.LogLevel level, string formattedMessage);

        #region Obsolete

        [MethodImpl(MethodImplOptions.InternalCall)]
        [Obsolete]
        internal static extern void Log_Trace(string message);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [Obsolete]
        internal static extern void Log_Information(string message);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [Obsolete]
        internal static extern void Log_Warning(string message);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [Obsolete]
        internal static extern void Log_Error(string message);

        #endregion

        #endregion
    }

}
