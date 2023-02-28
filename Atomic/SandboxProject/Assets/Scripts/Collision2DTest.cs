using Atom;

namespace Sandbox
{

    internal class Collision2DTest : Entity
    {
        public void OnCreate()
        {
            Log.Info("Collision2DTest.OnCreate({0}, {1})", Id, Name);
            Log.Info("Has Transform Component: {0}", HasComponent<Transform>());

            //OnCollision2DEnter += OnCollision2DEnterFunc;
            //OnCollision2DExit += OnCollision2DExitFunc;
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
