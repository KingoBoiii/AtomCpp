namespace Atom
{
    
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
    }

}
