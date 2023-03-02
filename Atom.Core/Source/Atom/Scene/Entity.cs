using System.Collections.Generic;
using System;

namespace Atom
{

    public class Entity
    {
        private Dictionary<Type, ComponentBase> _componentCache = new Dictionary<Type, ComponentBase>();

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

        protected virtual void Start()
        {
#if ATOM_DEBUG
            Log.Info("Entity.Start({0})", Id);
#endif
        }

        protected virtual void Update(float deltaTime)
        {
#if ATOM_DEBUG
            Log.Info("Entity.Update({0})", deltaTime);
#endif
        }

        protected virtual void Destroy()
        {
#if ATOM_DEBUG
            Log.Info("Entity.Destroy({0})", Id);
#endif
        }

        #region Physics Test

        internal void OnCollision2DEnter_Internal(ulong entityId)
        {
            if (!HasComponent<BoxCollider2D>())
            {
                Log.Warn("Entity {0} has no BoxCollider2D component", Name);
                return;
            }

            var boxCollider2D = GetComponent<BoxCollider2D>();
            boxCollider2D.InvokeOnCollision2DEnter(entityId);
        }

        internal void OnCollision2DExit_Internal(ulong entityId)
        {
            if (!HasComponent<BoxCollider2D>())
            {
                Log.Warn("Entity {0} has no BoxCollider2D component", Name);
                return;
            }

            var boxCollider2D = GetComponent<BoxCollider2D>();
            boxCollider2D.InvokeOnCollision2DExit(entityId);
        }

        #endregion

        public bool HasComponent<TComponent>() where TComponent : ComponentBase
        {
            var componentType = typeof(TComponent);
            return InternalCalls.Entity_HasComponent(Id, componentType);
        }

        public TComponent GetComponent<TComponent>() where TComponent : ComponentBase, new()
        {
            Type componentType = typeof(TComponent);

            if (!HasComponent<TComponent>())
            {
                if (_componentCache.ContainsKey(componentType))
                    _componentCache.Remove(componentType);

                return null;
            }

            if (!_componentCache.ContainsKey(componentType))
            {
                TComponent component = new TComponent { Entity = this };
                _componentCache.Add(componentType, component);
                return component;
            }

            return _componentCache[componentType] as TComponent;
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
