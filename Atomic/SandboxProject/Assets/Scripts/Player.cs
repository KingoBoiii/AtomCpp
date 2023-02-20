using Atom;

namespace Sandbox
{

    // TODO: Move build files into cache directory?

    internal class Player : EntityBase, IEntity
    {
        public float Test = 1.0f;
        public EntityBase OtherEntity;

        public Vector3 Position;
        public char Character;
        public string PlayerName;

        public float Speed = 2.5f;
        public float Time;

        private BasicRenderer m_Renderer;

        public void OnCreate()
        {
            Log.Info("Player.OnCreate({0}, {1})", Id, Name);
            Log.Info("Has Transform Component: {0}", HasComponent<Transform>());

            m_Renderer = GetComponent<BasicRenderer>();
            m_Renderer.Color = new Color(1.0f, 0.0f, 1.0f, 1.0f);
        }

        public void OnUpdate(float deltaTime)
        {
            Time += deltaTime;

            //Log.Info("Player.OnUpdate({0})", deltaTime);
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
