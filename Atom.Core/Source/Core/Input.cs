using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Atom
{
    
    public static class Input
    {
        public static bool IsKeyDown(KeyCode keycode) => InternalCalls.Input_IsKeyDown(keycode);
    }
    
}
