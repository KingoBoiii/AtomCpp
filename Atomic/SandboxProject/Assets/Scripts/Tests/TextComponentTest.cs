using Atom;

namespace Sandbox.Tests
{

    internal class TextComponentTest : Entity
    {
        [VisibleInEditor] private float m_Time = 346.523f;
        [VisibleInEditor] private bool Enabled = true;
        [VisibleInEditor] private bool Disabled;

        private TextRenderer _text;

        protected override void Start()
        {
            //base.Start();
            Log.Info("{0}.Start({1}) - Enabled: {2}", nameof(TextComponentTest), Id, Enabled);

            Name = "Dynamic Text - C#";

            _text = GetComponent<TextRenderer>();

            var oldTextString = _text.Text;
            Log.Info("Current TextString: {0}", oldTextString);

            _text.Text = "Hello world! This is written from C#!";
            _text.Color = new Color(0.443f, 0.8254f, 0.8675f, 1.0f);

            var newTextString = _text.Text;
            Log.Info("New TextString: {0}", newTextString);
        }
        
        protected override void Destroy()
        {
            //base.Destroy();
            Log.Info("{0}.Destroy({1}) - Enabled: {2}", nameof(TextComponentTest), Id, Enabled);
        }

#if false
        private void OnCreate()
        {
            Log.Info("Enabled: {}", Enabled);

            Name = "Dynamic Text - C#";

            _text = GetComponent<TextRenderer>();

            var oldTextString = _text.Text;
            Log.Info("Current TextString: {0}", oldTextString);

            _text.Text = "Hello world! This is written from C#!";
            _text.Color = new Color(0.443f, 0.8254f, 0.8675f, 1.0f);

            var newTextString = _text.Text;
            Log.Info("New TextString: {0}", newTextString);
        }
#endif
    }

}
