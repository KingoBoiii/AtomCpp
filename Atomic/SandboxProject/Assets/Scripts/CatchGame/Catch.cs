using Atom;
using System.Runtime.Remoting.Metadata;

namespace Sandbox.CatchGame
{

    internal class Catch : Entity
    {
        private Vector2 StartingPosition = new Vector2(0.0f, 6.0f);

        private bool _hitPlayer = false;
        private Rigidbody2D _rb;

        private void OnCreate()
        {
            _rb = GetComponent<Rigidbody2D>();

            Log.Info("CatchGame.Catch.OnCreate({0})", Name);
            //OnCollision2DEnter += OnCollision2DEnterFunc;
        }

        private void OnDestroy()
        {
            Log.Info("CatchGame.Catch.OnCreate({0})", Name);
            //OnCollision2DEnter -= OnCollision2DEnterFunc;
        }

        private void OnUpdate(float deltaTime)
        {
            if (_hitPlayer)
            {
                _rb.Position = StartingPosition;

                _hitPlayer = false;
            }
            Log.Info("Rigidbody Position: ({0}, {1})", _rb.Position.X, _rb.Position.Y);
        }

        //private void OnCollision2DEnterFunc(Entity entity)
        //{
        //    var player = entity.As<Player>();
        //    if (player == default)
        //    {
        //        return;
        //    }
            
        //    _hitPlayer = true;
        //    Log.Info("Collide with Player!");
        //}
    }

}
