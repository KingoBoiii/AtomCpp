using Atom;
namespace Sandbox
{

    internal class Camera : Entity
    {
        public Entity OtherEntity;

        public float DistanceFromPlayer = 5.0f;

        private Player _player;

        private void OnCreate()
        {
            _player = FindEntityByName("Player").As<Player>();
        }

        private void OnUpdate(float deltaTime)
        {
            if(_player is null)
            {
                Log.Error("Failed to retreive player!");
                return;
            }

            Transform.Position = new Vector3(_player.Transform.Position.X, _player.Transform.Position.Y, DistanceFromPlayer);
        }
    }

}
