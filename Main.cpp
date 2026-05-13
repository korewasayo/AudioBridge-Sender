#include <JuceHeader.h>

// Main application class that inherits from JUCEApplication
class AudioBridgeApplication : public juce::JUCEApplication
{
public:
    AudioBridgeApplication() {}

    const juce::String getApplicationName() override    { return "AudioBridge TX"; }
    const juce::String getApplicationVersion() override { return "1.0.0"; }
    bool moreThanOneInstanceAllowed() override          { return true; }

    // What happens when the app opens
    void initialise (const juce::String& commandLine) override
    {
        mainWindow.reset (new MainWindow (getApplicationName()));
    }

    // What happens when the app closes
    void shutdown() override
    {
        mainWindow = nullptr;
    }

    // The class that draws the window on the screen
    class MainWindow : public juce::DocumentWindow
    {
    public:
        MainWindow (juce::String name)
            : DocumentWindow (name,
                              juce::Desktop::getInstance().getDefaultLookAndFeel()
                                                          .findColour (juce::ResizableWindow::backgroundColourId),
                              DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar (true);
            
            // Create an empty component for now (window background)
            setContentOwned (new juce::Component(), true); 
            
            centreWithSize (400, 300); // Window size
            setVisible (true);
        }

        void closeButtonPressed() override
        {
            // Close the app when clicking the X button
            juce::JUCEApplication::getInstance()->systemRequestedQuit();
        }

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    std::unique_ptr<MainWindow> mainWindow;
};

// JUCE macro that starts the program.
START_JUCE_APPLICATION (AudioBridgeApplication)