using Atom.Bindings;
using System;

namespace Atom
{

    public delegate void OnCollision2DEnter(Entity entity);
    public delegate void OnCollision2DExit(Entity entity);

    [NativeComponent("Atom::BoxCollider2D")]
    public class BoxCollider2D : ComponentBase
    {
        //public event OnCollision2DEnter OnCollision2DEnter;
        //public event OnCollision2DExit OnCollision2DExit;
        internal event Action<Entity> OnCollision2DEnter;
        internal event Action<Entity> OnCollision2DExit;

        public void AddOnCollision2DEnterCallback(Action<Entity> callback)
        {
            OnCollision2DEnter += callback;
        }

        public void AddOnCollision2DExitCallback(Action<Entity> callback)
        {
            OnCollision2DExit += callback;
        }

        internal void InvokeOnCollision2DEnter(ulong entityId)
        {
            OnCollision2DEnter?.Invoke(new Entity(entityId));
        }

        internal void InvokeOnCollision2DExit(ulong entityId)
        {
            OnCollision2DExit?.Invoke(new Entity(entityId));
        }
    }

}
