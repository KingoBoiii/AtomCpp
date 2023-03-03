using System;

namespace Atom
{

    public static class Application
    {
        public static uint Width
        {
            get
            {
                InternalCalls.Application_GetWidth(out var width);
                return width;
            }
        }

        public static uint Height
        {
            get
            {
                InternalCalls.Application_GetHeight(out var height);
                return height;
            }
        }
    }

}
