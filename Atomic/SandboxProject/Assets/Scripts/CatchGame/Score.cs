using Atom;

namespace Sandbox.CatchGame
{

    internal class Score : Entity
    {
        // TODO: This should be private, but still visible in the editor
        public int ScoreValue = 0;

        private void OnCreate()
        {
            ScoreValue = 0;
        }

        public void AddScore(int value)
        {
            ScoreValue += value;
            Log.Info("Scored! Score: {0}", ScoreValue);
        }
    }
    
}
