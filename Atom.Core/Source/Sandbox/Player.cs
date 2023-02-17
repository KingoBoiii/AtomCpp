using Atom;

namespace Sandbox
{

    internal class Player : EntityBase, IEntity
    {
        public float Speed { get; set; } = 1.0f;

        public void OnCreate()
        {
            Log.Info($"Player.OnCreate({Id})");
        }

        public void OnUpdate(float deltaTime)
        {
            Log.Info($"Player.OnUpdate({deltaTime})");

            Vector3 position = Position;
            if (Input.IsKeyDown(KeyCode.A))
            {
                position.X -= Speed * deltaTime;
            }
            else if (Input.IsKeyDown(KeyCode.D))
            {
                position.X += Speed * deltaTime;
            }

            if (Input.IsKeyDown(KeyCode.W))
            {
                position.Y += Speed * deltaTime;
            }
            else if (Input.IsKeyDown(KeyCode.S))
            {
                position.Y -= Speed * deltaTime;
            }
            Position = position;
        }

        public void OnDestroy()
        {
            Log.Info($"Player.OnDestroy({Id})");
        }
    }

}
