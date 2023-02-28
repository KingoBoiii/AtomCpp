using Atom;

namespace Sandbox.Tests
{

    internal class TextComponentTest : Entity
    {
        private TextRenderer _text;

        private void OnCreate()
        {
            Name = "Dynamic Text - C#";

            _text = GetComponent<TextRenderer>();

            var oldTextString = _text.Text;
            Log.Info("Current TextString: {0}", oldTextString);

            _text.Text = "Hello world! This is written from C#!";
            _text.Color = new Color(0.443f, 0.8254f, 0.8675f, 1.0f);

            var newTextString = _text.Text;
            Log.Info("New TextString: {0}", newTextString);
        }
    }

}
