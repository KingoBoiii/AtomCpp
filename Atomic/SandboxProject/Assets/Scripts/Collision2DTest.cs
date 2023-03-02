using Atom;

namespace Sandbox
{

    internal class Collision2DTest : Entity
    {
        private BoxCollider2D m_BoxCollider;

        protected override void Start()
        {
            base.Start();
            Log.Info("Has Transform Component: {0}", HasComponent<Transform>());

            m_BoxCollider = GetComponent<BoxCollider2D>();
            m_BoxCollider.AddOnCollision2DEnterCallback(OnCollision2DEnterFunc);
            m_BoxCollider.AddOnCollision2DExitCallback(OnCollision2DExitFunc);
            //m_BoxCollider.OnCollision2DEnter += OnCollision2DEnterFunc;
            //m_BoxCollider.OnCollision2DExit += OnCollision2DExitFunc;
            //OnCollision2DEnter += OnCollision2DEnterFunc;
            //OnCollision2DExit += OnCollision2DExitFunc;
        }

        protected override void Update(float deltaTime)
        {
            //base.Update(deltaTime);
        }

        private void OnCollision2DEnterFunc(Entity entity)
        {
            Log.Info("Collision2DTest.OnCollision2DEnter({0})", entity.Name);
        }

        private void OnCollision2DExitFunc(Entity entity)
        {
            Log.Info("Collision2DTest.OnCollision2DExit({0})", entity.Name);
        }
    }

}
