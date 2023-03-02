using Atom;

namespace Sandbox.TopDown2DTest
{

    internal class Player : Entity
    {

        [VisibleInEditor] private float m_Speed = 1.0f;

        private Rigidbody2D rb2d;

        protected override void Start()
        {
            rb2d = GetComponent<Rigidbody2D>();
        }

        protected override void Update(float deltaTime)
        {
            //base.Update(deltaTime);

            //Log.Warn("Player.Update(DeltaTime: {0}): Speed: {1}", deltaTime, m_Speed);

            Vector2 velocity = new Vector2(0.0f, 0.0f);

            if (Input.IsKeyDown(KeyCode.W))
            {
                velocity.Y += m_Speed * deltaTime;
            }
            else if (Input.IsKeyDown(KeyCode.S))
            {
                velocity.Y -= m_Speed * deltaTime;
            }

            if (Input.IsKeyDown(KeyCode.A))
            {
                velocity.X -= m_Speed * deltaTime;
            }
            else if (Input.IsKeyDown(KeyCode.D))
            {
                velocity.X += m_Speed * deltaTime;
            }

            rb2d.SetLinearVelocity(new Vector2(rb2d.Position.X + velocity.X, rb2d.Position.Y + velocity.Y));
        }

    }

}
