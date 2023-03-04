using Atom;

namespace Sandbox.Examples.CatchGame
{

    internal class Ground : Entity
    {
        [VisibleInEditor] private int Lives = 3;

        private BoxCollider2D m_Collider;
        private TextRenderer m_LivesTextRenderer;

        protected override void Start()
        {
            m_Collider = GetComponent<BoxCollider2D>();
            m_Collider.AddOnCollision2DEnterCallback((entity) =>
            {
                if (!entity.Name.Contains("Enemy"))
                {
                    return;
                }

                Scene.DestroyEntity(entity);

                Lives--;
                UpdateLivesText();
                //Log.Info("Enemy ({0}, {1}) hit the ground", entity.Name, entity.Id);
            });

            var livesTextEntity = FindEntityByName("LivesText");
            m_LivesTextRenderer = livesTextEntity.GetComponent<TextRenderer>();
        }

        private void UpdateLivesText()
        {
            m_LivesTextRenderer.Text = $"Lives: {Lives}";
        }
    }

}
