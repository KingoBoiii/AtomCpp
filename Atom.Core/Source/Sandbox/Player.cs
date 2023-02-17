using Atom;

namespace Sandbox
{

    internal class Player : EntityBase, IEntity
    {
        public float Speed { get; set; } = 1.0f;

        private Transform m_Transform;

        public void OnCreate()
        {
            Log.Info("Player.OnCreate({0}, {1})", Id, Name);
            Log.Info("Has Transform Component: {0}", HasComponent<Transform>());

            m_Transform = GetComponent<Transform>();
        }

        public void OnUpdate(float deltaTime)
        {
            Log.Info($"Player.OnUpdate({deltaTime})");

            Vector3 position = Transform.Position;
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
            Transform.Position = position;
        }

        public void OnDestroy()
        {
            Log.Info("Player.OnDestroy({0}, {1})", Id, Name);
        }
    }

}
