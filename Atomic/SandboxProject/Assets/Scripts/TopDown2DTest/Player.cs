using Atom;

namespace Sandbox.TopDown2DTest
{

    internal class Player : Entity
    {

        [VisibleInEditor] private float m_Speed = 1.0f;
        [VisibleInEditor] private float m_Time = 0.0f;

        private Rigidbody2D rb2d;

        protected override void Start()
        {
            rb2d = GetComponent<Rigidbody2D>();
        }

        protected override void Update(float deltaTime)
        {
            m_Time += deltaTime;
            //base.Update(deltaTime);

            //Log.Warn("Player.Update(DeltaTime: {0}): Speed: {1}", deltaTime, m_Speed);

            var position = rb2d.Position;

            if (Input.IsKeyDown(KeyCode.W))
            {
                position.Y += m_Speed * deltaTime;
            }
            else if (Input.IsKeyDown(KeyCode.S))
            {
                position.Y -= m_Speed * deltaTime;
            }

            if (Input.IsKeyDown(KeyCode.A))
            {
                position.X -= m_Speed * deltaTime;
            }
            else if (Input.IsKeyDown(KeyCode.D))
            {
                position.X += m_Speed * deltaTime;
            }

            rb2d.Position = position;
        }

    }

}
