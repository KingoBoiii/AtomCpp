using Atom;

namespace Sandbox.Examples.CatchGame
{

    internal class Ground : Entity
    {
        private BoxCollider2D m_Collider;

        protected override void Start()
        {
            m_Collider = GetComponent<BoxCollider2D>();
            m_Collider.AddOnCollision2DEnterCallback((entity) =>
            {
                if (!entity.Name.Contains("Enemy"))
                {
                    return;
                }

                Scene.DestroyEntity(entity);
                Log.Info("Enemy ({0}, {1}) hit the ground", entity.Name, entity.Id);
            });
        }
    }

}
