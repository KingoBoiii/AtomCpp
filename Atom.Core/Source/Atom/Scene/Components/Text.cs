using System;

namespace Atom
{

    public class Text : ComponentBase
    {
        public string TextString
        {
            get
            {
                InternalCalls.Text_GetTextString(Entity.Id, out var text);
                return text;
            }
            set
            {
                InternalCalls.Text_SetTextString(Entity.Id, value);
            }
        }

        public Color Color
        {
            get
            {
                InternalCalls.Text_GetColor(Entity.Id, out var color);
                return color;
            }
            set
            {
                InternalCalls.Text_SetColor(Entity.Id, ref value);
            }
        }
    }

}
