﻿using Atom;

namespace Sandbox
{

    // TODO: Move build files into cache directory?

    internal class Player : Entity
    {
        //public float Test = 1.0f;
        public Entity OtherEntity;

        public Vector3 Position;
        public char Character = 'A';
        [VisibleInEditor] private bool Enabled = true;
        public string PlayerName;

        //public float Speed = 2.5f;
        public float Speed;

        [HiddenFromEditor] public float Time;
        [VisibleInEditor] private float m_Time = 1.0f;

        private BasicRenderer m_Renderer;

        public void OnCreate()
        {
            Log.Info("Player.OnCreate({0}, {1})", Id, Name);
            Log.Info("Has Transform Component: {0}", HasComponent<Transform>());

            m_Renderer = GetComponent<BasicRenderer>();
            m_Renderer.Color = new Color(1.0f, 0.0f, 1.0f, 1.0f);
        }

        public void OnUpdate(float deltaTime)
        {
            Time += deltaTime;

            //Log.Info("Player.OnUpdate({0})", deltaTime);
            Vector3 position = Transform.Position;
            if (Input.IsKeyDown(KeyCode.A))
            {
                position.X -= Speed * deltaTime;
            }
            else if (Input.IsKeyDown(KeyCode.D))
            {
                position.X += Speed * deltaTime;
            }

            if (Input.IsKeyDown(KeyCode.W))
            {
                position.Y += Speed * deltaTime;
            }
            else if (Input.IsKeyDown(KeyCode.S))
            {
                position.Y -= Speed * deltaTime;
            }
            Transform.Position = position;

            Camera camera = FindEntityByName("Camera").As<Camera>();
            if (camera != null) 
            {
                if (Input.IsKeyDown(KeyCode.Q))
                {
                    camera.DistanceFromPlayer += Speed * 2.0f * deltaTime;
                }
                else if (Input.IsKeyDown(KeyCode.E))
                {
                    camera.DistanceFromPlayer -= Speed * 2.0f * deltaTime;
                }
            }
        }

        public void OnDestroy()
        {
            Log.Info("Player.OnDestroy({0}, {1})", Id, Name);
        }
    }

}
