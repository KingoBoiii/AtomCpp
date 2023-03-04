using Atom;

namespace Sandbox.Examples.CatchGame
{

    internal class Spawner : Entity
    {
        [VisibleInEditor] private int XSpawnRange = 5;
        [VisibleInEditor] private float TimeBetweenSpawns = 0.3f;
        [VisibleInEditor] private float m_CurrentTime = 0.0f;

        private static int m_EnemyIndex = 0;

        protected override void Start()
        {
            m_CurrentTime = TimeBetweenSpawns;
        }

        protected override void Update(float deltaTime)
        {
            m_CurrentTime -= deltaTime;
            if (m_CurrentTime <= 0.0f)
            {
                SpawnEnemy();
                m_CurrentTime = TimeBetweenSpawns;
            }
        }

        private void SpawnEnemy()
        {
            var enemy = Scene.CreateEntity($"Enemy{m_EnemyIndex++}");
            var transform = enemy.GetComponent<Transform>();

            float randomXOffset = (float)Random.IntInclusive(-XSpawnRange, XSpawnRange);
            transform.Position = new Vector3(Transform.Position.X + randomXOffset, Transform.Position.Y, 0.0f);

            var renderer = enemy.AddComponent<BasicRenderer>();
            renderer.Color = new Color
            {
                R = (float)Random.IntInclusive(255) / 255.0f,
                G = (float)Random.IntInclusive(255) / 255.0f,
                B = (float)Random.IntInclusive(255) / 255.0f,
                A = 1.0f
            };

            var collider = enemy.AddComponent<BoxCollider2D>();
            collider.AddOnCollision2DEnterCallback((entity) => Log.Info("Hitted entity {0} ({1})", entity.Name, entity.Id));

            var rb2d = enemy.AddComponent<Rigidbody2D>();
            rb2d.BodyType = RigidbodyType.Dynamic;
        }
    }

}
