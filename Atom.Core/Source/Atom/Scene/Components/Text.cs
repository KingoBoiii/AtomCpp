using Atom.Bindings;

namespace Atom
{

    [NativeComponent("Atom::Text")]
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

        public float Kerning
        {
            get
            {
                InternalCalls.Text_GetKerning(Entity.Id, out var kerning);
                return kerning;
            }
            set
            {
                InternalCalls.Text_SetKerning(Entity.Id, ref value);
            }
        }

        public float LineSpacing
        {
            get
            {
                InternalCalls.Text_GetLineSpacing(Entity.Id, out var lineSpacing);
                return lineSpacing;
            }
            set
            {
                InternalCalls.Text_SetLineSpacing(Entity.Id, ref value);
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
