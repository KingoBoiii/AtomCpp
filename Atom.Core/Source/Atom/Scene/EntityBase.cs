using System;

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

        public bool HasComponent<TComponent>() where TComponent : ComponentBase
        {
            var componentType = typeof(TComponent);
            return InternalCalls.Entity_HasComponent(Id, componentType);
        }

        public TComponent GetComponent<TComponent>() where TComponent : ComponentBase, new()
        {
            if(!HasComponent<TComponent>())
            {
                return default;
            }

            return new TComponent { Entity = this };
        }
    }

}
