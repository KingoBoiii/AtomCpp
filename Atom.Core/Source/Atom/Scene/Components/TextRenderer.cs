using Atom.Bindings;

namespace Atom
{

    [NativeComponent("Atom::TextRenderer")]
    public class TextRenderer : ComponentBase
    {
        public string Text
        {
            get
            {
                InternalCalls.TextRenderer_GetTextString(Entity.Id, out var text);
                return text;
            }
            set
            {
                InternalCalls.TextRenderer_SetTextString(Entity.Id, value);
            }
        }

        public float Kerning
        {
            get
            {
                InternalCalls.TextRenderer_GetKerning(Entity.Id, out var kerning);
                return kerning;
            }
            set
            {
                InternalCalls.TextRenderer_SetKerning(Entity.Id, ref value);
            }
        }

        public float LineSpacing
        {
            get
            {
                InternalCalls.TextRenderer_GetLineSpacing(Entity.Id, out var lineSpacing);
                return lineSpacing;
            }
            set
            {
                InternalCalls.TextRenderer_SetLineSpacing(Entity.Id, ref value);
            }
        }

        public Color Color
        {
            get
            {
                InternalCalls.TextRenderer_GetColor(Entity.Id, out var color);
                return color;
            }
            set
            {
                InternalCalls.TextRenderer_SetColor(Entity.Id, ref value);
            }
        }
    }

}
