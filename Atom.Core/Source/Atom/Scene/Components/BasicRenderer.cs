using Atom.Bindings;

namespace Atom
{

    [NativeComponent("Atom::BasicRenderer")]
    public class BasicRenderer : ComponentBase
    {
        public Color Color
        {
            get
            {
                InternalCalls.BasicRenderer_GetColor(Entity.Id, out var color);
                return color;
            }
            set
            {
                InternalCalls.BasicRenderer_SetColor(Entity.Id, ref value);
            }
        }
    }

}
