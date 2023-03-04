using Atom;

namespace Sandbox.Examples.CatchGame
{

    internal class Player : Entity
    {
        [VisibleInEditor] private int Score = 0;
        [VisibleInEditor] private float Speed = 500.0f;

        private float m_YPos = 0.0f;

        private Rigidbody2D m_Rigidbody;
        private TextRenderer m_ScoreTextRenderer;

        protected override void Start()
        {
            var collider = GetComponent<BoxCollider2D>();
            collider.AddOnCollision2DEnterCallback((entity) =>
            {
                if (!entity.Name.Contains("Enemy"))
                {
                    return;
                }

                Score++;
                Scene.DestroyEntity(entity);

                UpdateScoreText();
            });

            m_Rigidbody = GetComponent<Rigidbody2D>();
            m_YPos = m_Rigidbody.Position.X;

            var scoreTextEntity = FindEntityByName("ScoreText");
            m_ScoreTextRenderer = scoreTextEntity.GetComponent<TextRenderer>();
            UpdateScoreText();
        }

        private void UpdateScoreText()
        {
            m_ScoreTextRenderer.Text = $"Score: {Score}";
        }

        protected override void Update(float deltaTime)
        {
            var velocity = Vector2.Zero;

            if (Input.IsKeyDown(KeyCode.A))
            {
                velocity.X -= Speed * deltaTime;
            }
            else if (Input.IsKeyDown(KeyCode.D))
            {
                velocity.X += Speed * deltaTime;
            }

            m_Rigidbody.SetLinearVelocity(velocity);
            var position = m_Rigidbody.Position;
            position.Y = m_YPos;
            m_Rigidbody.Position = position;
        }
    }

}
