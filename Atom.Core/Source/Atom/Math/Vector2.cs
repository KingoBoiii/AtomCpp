using System;
using System.IO;
using System.Runtime.InteropServices;

namespace Atom
{

    [StructLayout(LayoutKind.Sequential)]
    public struct Vector2 : IEquatable<Vector2>
    {
        public Vector2(float scalar)
        {
            X = scalar;
            Y = scalar;
        }

        public Vector2(float x, float y)
        {
            X = x;
            Y = y;
        }

        public float X { get; set; }
        public float Y { get; set; }

        public static Vector2 Zero => new Vector2(0.0f);
        public static Vector2 One => new Vector2(1.0f);

        public static Vector2 PositiveXAxis => new Vector2(1.0f, 0.0f);
        public static Vector2 NegativeXAxis => new Vector2(-1.0f, 0.0f);

        public static Vector2 PositiveYAxis => new Vector2(0.0f, 1.0f);
        public static Vector2 NegativeYAxis => new Vector2(0.0f, -1.0f);

        public float Dot(Vector2 other)
        {
            return (this.X * other.X) + (this.Y * other.Y); 
        }

        public float Distance(Vector2 other)
        {
            return (this - other).Magnitude();
        }

        public float Magnitude()
        {
            return (float)Math.Sqrt(this.X * this.X + this.Y * this.Y);
        }

        public Vector2 Normalized()
        {
            float mag = Magnitude();
            return new Vector2(this.X / mag, this.Y / mag);
        }

        #region Operators

        public static Vector2 operator +(Vector2 left, Vector2 right) => Add(left, right);
        public static Vector2 operator +(Vector2 left, float scalar) => AddScalar(left, scalar);
        public static Vector2 operator -(Vector2 left, Vector2 right) => Subtract(left, right);
        public static Vector2 operator -(Vector2 left, float scalar) => SubtractScalar(left, scalar);
        public static Vector2 operator /(Vector2 left, Vector2 right) => Divide(left, right);
        public static Vector2 operator /(Vector2 left, float scalar) => DivideScalar(left, scalar);
        public static Vector2 operator *(Vector2 left, Vector2 right) => Multiply(left, right);
        public static Vector2 operator *(Vector2 left, float scalar) => MultiplyScalar(left, scalar);

        public static bool operator !=(Vector2 left, Vector2 right) => !(left == right);
        public static bool operator ==(Vector2 left, Vector2 right) => Equals(left, right);

        #endregion

        #region Internal Vector2 Methods

        private static Vector2 Add(Vector2 left, Vector2 right)
        {
            return new Vector2(left.X + right.X, left.Y + right.Y);
        }

        private static Vector2 AddScalar(Vector2 vector, float scalar)
        {
            return new Vector2(vector.X + scalar, vector.Y + scalar);
        }

        private static Vector2 Subtract(Vector2 left, Vector2 right)
        {
            return new Vector2(left.X - right.X, left.Y - right.Y);
        }

        private static Vector2 SubtractScalar(Vector2 vector, float scalar)
        {
            return new Vector2(vector.X - scalar, vector.Y - scalar);
        }

        private static Vector2 Divide(Vector2 left, Vector2 right)
        {
            return new Vector2(left.X / right.X, left.Y / right.Y);
        }

        private static Vector2 DivideScalar(Vector2 vector, float scalar)
        {
            return new Vector2(vector.X / scalar, vector.Y / scalar);
        }

        private static Vector2 Multiply(Vector2 left, Vector2 right)
        {
            return new Vector2(left.X * right.Y, left.Y * right.Y);
        }

        private static Vector2 MultiplyScalar(Vector2 vector, float scalar)
        {
            return new Vector2(vector.X * scalar, vector.Y * scalar);
        }

        #endregion

        #region Overrides

        public bool Equals(Vector2 other)
        {
            return (this.X == other.X) && (this.Y == other.Y);
        }

        public override bool Equals(object obj)
        {
            if (!(obj is Vector2 vector))
            {
                return false;
            }

            return this.Equals(vector);
        }

        public override int GetHashCode()
        {
            unchecked
            {
                int hash = 17;
                hash = hash * 23 + this.X.GetHashCode();
                hash = hash * 23 + this.Y.GetHashCode();
                return hash;
            }
        }

        public override string ToString()
        {
            return $"Vector2({X}, {Y})";
        }

        #endregion
    }

}
