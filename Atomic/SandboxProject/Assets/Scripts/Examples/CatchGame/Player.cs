using Atom;

namespace Sandbox.Examples.CatchGame
{

    internal class Player : Entity
    {
        [VisibleInEditor] private float Speed = 500.0f;

        private Rigidbody2D m_Rigidbody;

        protected override void Start()
        {
            m_Rigidbody = GetComponent<Rigidbody2D>();
        }

        protected override void Update(float deltaTime)
        {
            var velocity = new Vector2(0.0f, 0.0f);

            if (Input.IsKeyDown(KeyCode.A))
            {
                velocity.X -= Speed * deltaTime;
            }
            else if (Input.IsKeyDown(KeyCode.D))
            {
                velocity.X += Speed * deltaTime;
            }

            Log.Info("Velocity: {0}", velocity);
            m_Rigidbody.SetLinearVelocity(velocity);
        }
    }

}
