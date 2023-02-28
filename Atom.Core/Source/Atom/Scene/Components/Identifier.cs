using Atom.Bindings;

namespace Atom
{

    [NativeComponent("Atom::Identifier")]
    public class Identifier : ComponentBase
    {
        public string Name
        {
            get
            {
                InternalCalls.Identifier_GetName(Entity.Id, out var name);
                return name;
            }
            set
            {
                InternalCalls.Identifier_SetName(Entity.Id, value);
            }
        }
    }

}
