namespace Atom
{

    public delegate void OnCollision2DEnter(Entity entity);
    public delegate void OnCollision2DExit(Entity entity);

    public class Entity
    {
        public event OnCollision2DEnter OnCollision2DEnter;
        public event OnCollision2DExit OnCollision2DExit;

        protected Entity() { Id = 0; }

        internal Entity(ulong id)
        {
            Id = id;
        }

        public ulong Id { get; }

        public string Name
        {
            get { return GetComponent<Identifier>().Name; }
            set { GetComponent<Identifier>().Name = value; }
        }

        public Transform Transform
        {
            get
            {
                return GetComponent<Transform>();
            }
        }

        #region Physics Test

        internal void OnCollision2DEnter_Internal(ulong entityId)
        {
            OnCollision2DEnter?.Invoke(new Entity(entityId));
        }

        internal void OnCollision2DExit_Internal(ulong entityId)
        {
            OnCollision2DExit?.Invoke(new Entity(entityId));
        }

        #endregion

        public bool HasComponent<TComponent>() where TComponent : ComponentBase
        {
            var componentType = typeof(TComponent);
            return InternalCalls.Entity_HasComponent(Id, componentType);
        }

        public TComponent GetComponent<TComponent>() where TComponent : ComponentBase, new()
        {
            if (!HasComponent<TComponent>())
            {
                return default;
            }

            return new TComponent { Entity = this };
        }

        public Entity FindEntityByName(string name)
        {
            InternalCalls.Entity_FindEntityByName(name, out var entityId);

            if (entityId == 0)
            {
                return default;
            }

            return new Entity(entityId);
        }

        public T As<T>() where T : Entity, new()
        {
            InternalCalls.Entity_GetScriptInstance(Id, out var instance);
            return instance as T;
        }
    }

}
