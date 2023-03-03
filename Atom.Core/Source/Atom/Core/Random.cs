using System;

namespace Atom
{

    public static class Random
    {
        private static System.Random s_Random = new System.Random(Guid.NewGuid().GetHashCode());

        /// <summary>
        /// Sets a seed
        /// </summary>
        /// <param name="seed">Integer between <see cref="int.MinValue"/> and <see cref="int.MaxValue"/></param>
        public static void SetSeed(int seed)
        {
            s_Random = new System.Random(seed);
        }

        /// <returns>Random integer between 0 and <see cref="int.MaxValue"/></returns>
        public static int Int()
        {
            return s_Random.Next();
        }

        /// <param name="maxValue">Exclusive max value</param>
        /// <returns>Random integer between 0 and <paramref name="maxValue"/> (exclusive)</returns>
        public static int Int(int maxValue)
        {
            return s_Random.Next(maxValue);
        }

        /// <param name="maxValue">Inclusive max value</param>
        /// <returns>Random integer between 0 and <paramref name="maxValue"/> (inclusive)</returns>
        public static int IntInclusive(int maxValue)
        {
            return s_Random.Next(maxValue + 1);
        }

        /// <param name="minValue">Inclusive min value</param>
        /// <param name="maxValue">Exclusive max value</param>
        /// <returns>Random integer between <paramref name="minValue"/> and <paramref name="maxValue"/> (exclusive)</returns>
        public static int Int(int minValue, int maxValue)
        {
            return s_Random.Next(minValue, maxValue);
        }

        /// <param name="minValue">Inclusive min value</param>
        /// <param name="maxValue">Inclusive max value</param>
        /// <returns>Random integer between <paramref name="minValue"/> and <paramref name="maxValue"/> (inclusive)</returns>
        public static int IntInclusive(int minValue, int maxValue)
        {
            return s_Random.Next(minValue, maxValue + 1);
        }
    }

}
