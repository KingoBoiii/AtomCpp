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
            position.X += Speed * deltaTime;
            Position = position;
        }

        public void OnDestroy()
        {
            Log.Info($"Player.OnDestroy({Id})");
        }
    }

}
