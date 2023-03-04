using Atom.Bindings;

namespace Atom
{

    public enum RigidbodyType
    {
        Static = 0,
        Dynamic = 1,
        Kinematic = 2
    }

    [NativeComponent("Atom::Rigidbody2D")]
    public class Rigidbody2D : ComponentBase
    {
        public Vector2 Position
        {
            get
            {
                InternalCalls.Rigidbody2D_GetPosition(Entity.Id, out var position);
                return position;
            }
            set
            {
                InternalCalls.Rigidbody2D_SetPosition(Entity.Id, ref value);
            }
        }

        public RigidbodyType BodyType
        {
            get
            {
                InternalCalls.Rigidbody2D_GetPhysicsBodyType(Entity.Id, out var rigidbodyType);
                return rigidbodyType;
            }
            set
            {
                InternalCalls.Rigidbody2D_SetPhysicsBodyType(Entity.Id, value);
            }
        }

        public void SetLinearVelocity(Vector2 velocity)
        {
            InternalCalls.Rigidbody2D_SetLinearVelocity(Entity.Id, ref velocity);
        }
    }

}
