namespace Atom
{

    public interface IEntity
    {
        void OnCreate();
        void OnDestroy();
        void OnUpdate(float deltaTime);
    }

    public class EntityBase
    {
        protected EntityBase() { Id = 0; }

        internal EntityBase(ulong id)
        {
            Id = id;
        }

        public ulong Id { get; }

        public Vector3 Position
        {
            get
            {
                InternalCalls.Entity_GetPosition(Id, out var position);
                return position;
            }
            set
            {
                InternalCalls.Entity_SetPosition(Id, ref value);
            }
        }
    }

}
