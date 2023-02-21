using Atom;
namespace Sandbox
{

    internal class Camera : Entity
    {
        public Entity OtherEntity;

        public float DistanceFromPlayer = 5.0f;
        
        private void OnUpdate(float deltaTime)
        {
            Entity player = FindEntityByName("Player");
            if(player is null)
            {
                Log.Error("Failed to retreive player!");
                return;
            }

            //Player player = FindEntityByName("Player").As<Player>();

            Transform.Position = new Vector3(player.Transform.Position.X, player.Transform.Position.Y, DistanceFromPlayer);
        }
    }

}
