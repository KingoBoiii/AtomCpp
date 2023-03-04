using System;
using System.Runtime.InteropServices;

namespace Atom
{
    // C# 
    // struct (value type) -> stack allocated
    // class (ref type) -> heap allocated (+ GC)
    [StructLayout(LayoutKind.Sequential)]
    public struct Vector3
    {
        public Vector3(float scalar)
        {
            X = scalar;
            Y = scalar;
            Z = scalar;
        }

        public Vector3(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        public Vector3(Vector2 vector, float z)
        {
            X = vector.X;
            Y = vector.Y;
            Z = z;
        }

        public float X { get; set; }
        public float Y { get; set; }
        public float Z { get; set; }

        public Vector2 XY => new Vector2(X, Y);

        public static Vector3 Zero => new Vector3(0.0f);
        public static Vector3 One => new Vector3(1.0f);

        public static Vector3 PositiveXAxis => new Vector3(1.0f, 0.0f, 0.0f);
        public static Vector3 NegativeXAxis => new Vector3(-1.0f, 0.0f, 0.0f);

        public static Vector3 PositiveYAxis => new Vector3(0.0f, 1.0f, 0.0f);
        public static Vector3 NegativeYAxis => new Vector3(0.0f, -1.0f, 0.0f);

        public static Vector3 PositiveZAxis => new Vector3(0.0f, 0.0f, 1.0f);
        public static Vector3 NegativeZAxis => new Vector3(0.0f, 0.0f, -1.0f);

        public float Dot(Vector3 other)
        {
            return (this.X * other.X) + (this.Y * other.Y) + (this.Z * other.Z);
        }

        public float Distance(Vector3 other)
        {
            return (this - other).Magnitude();
        }

        public float Magnitude()
        {
            return (float)Math.Sqrt(this.X * this.X + this.Y * this.Y + this.Z * this.Z);
        }

        public Vector3 Normalized()
        {
            float mag = Magnitude();
            return new Vector3(this.X / mag, this.Y / mag, this.Z / mag);
        }

        #region Operators

        public static Vector3 operator +(Vector3 left, Vector3 right) => Add(left, right);
        public static Vector3 operator +(Vector3 left, float scalar) => AddScalar(left, scalar);
        public static Vector3 operator -(Vector3 left, Vector3 right) => Subtract(left, right);
        public static Vector3 operator -(Vector3 left, float scalar) => SubtractScalar(left, scalar);
        public static Vector3 operator /(Vector3 left, Vector3 right) => Divide(left, right);
        public static Vector3 operator /(Vector3 left, float scalar) => DivideScalar(left, scalar);
        public static Vector3 operator *(Vector3 left, Vector3 right) => Multiply(left, right);
        public static Vector3 operator *(Vector3 left, float scalar) => MultiplyScalar(left, scalar);

        public static bool operator !=(Vector3 left, Vector3 right) => !(left == right);
        public static bool operator ==(Vector3 left, Vector3 right) => Equals(left, right);

        #endregion

        #region Internal Vector2 Methods

        private static Vector3 Add(Vector3 left, Vector3 right)
        {
            return new Vector3(left.X + right.X, left.Y + right.Y, left.Z + right.Z);
        }

        private static Vector3 AddScalar(Vector3 vector, float scalar)
        {
            return new Vector3(vector.X + scalar, vector.Y + scalar, vector.Z + scalar);
        }

        private static Vector3 Subtract(Vector3 left, Vector3 right)
        {
            return new Vector3(left.X - right.X, left.Y - right.Y, left.Z - right.Z);
        }

        private static Vector3 SubtractScalar(Vector3 vector, float scalar)
        {
            return new Vector3(vector.X - scalar, vector.Y - scalar, vector.Z - scalar);
        }

        private static Vector3 Divide(Vector3 left, Vector3 right)
        {
            return new Vector3(left.X / right.X, left.Y / right.Y, left.Z / right.Z);
        }

        private static Vector3 DivideScalar(Vector3 vector, float scalar)
        {
            return new Vector3(vector.X / scalar, vector.Y / scalar, vector.Z / scalar);
        }

        private static Vector3 Multiply(Vector3 left, Vector3 right)
        {
            return new Vector3(left.X * right.Y, left.Y * right.Y, left.Z * right.Z);
        }

        private static Vector3 MultiplyScalar(Vector3 vector, float scalar)
        {
            return new Vector3(vector.X * scalar, vector.Y * scalar, vector.Z * scalar);
        }

        #endregion

        #region Overrides

        public bool Equals(Vector3 other)
        {
            return (this.X == other.X) && (this.Y == other.Y) && (this.Z == other.Z);
        }

        public override bool Equals(object obj)
        {
            if (!(obj is Vector3 vector))
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
                hash = hash * 23 + this.Z.GetHashCode();
                return hash;
            }
        }

        public override string ToString()
        {
            return $"Vector3({X}, {Y}, {Z})";
        }

        #endregion
    }

}