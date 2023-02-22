using Atom;

namespace Sandbox.CatchGame
{
    
    internal class Player : Entity
    {
        public float Speed;
        public int Lives;

        private Score _score;
        private Rigidbody2D _rb;

        private void OnCreate()
        {
            _score = FindEntityByName("Score").As<Score>();
            _rb = GetComponent<Rigidbody2D>();

            Log.Info("CatchGame.Player.OnCreate({0})", Name);
            OnCollision2DEnter += OnCollision2DEnterFunc;
        }

        private void OnDestroy()
        {
            Log.Info("CatchGame.Player.OnDestroy({0})", Name);
            OnCollision2DEnter -= OnCollision2DEnterFunc;
        }

        private void OnUpdate(float deltaTime)
        {
            // TODO: Since Rigidbody takes ownership of Transform, we need a API for Rigidbody to move!
            var position = _rb.Position;

            if(Input.IsKeyDown(KeyCode.A))
            {
                position.X -= Speed * deltaTime;
            }
            else if (Input.IsKeyDown(KeyCode.D))
            {
                position.X += Speed * deltaTime;
            }

            _rb.Position = position;
        }

        private void OnCollision2DEnterFunc(Entity entity)
        {
            var catchItem = entity.As<Catch>();
            if (catchItem == default)
            {
                return;
            }

            _score.AddScore(1);
            Log.Info("Collide with item to Catch!");
        }
    }

}
