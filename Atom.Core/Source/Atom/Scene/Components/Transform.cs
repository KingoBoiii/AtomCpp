using Atom.Bindings;

namespace Atom
{

    [NativeComponent("Atom::Transform")]
    public class Transform : ComponentBase
    {
        public Vector3 Position
        {
            get
            {
                InternalCalls.Transform_GetPosition(Entity.Id, out var position);
                return position;
            }
            set
            {
                InternalCalls.Transform_SetPosition(Entity.Id, ref value);
            }
        }
    }

}
