#include <JuceHeader.h>

class IndustrialLookAndFeel : public juce::LookAndFeel_V4
{
public:
    IndustrialLookAndFeel()
    {
        const auto background = juce::Colour (0xff121416);
        const auto panel = juce::Colour (0xff1c2025);
        const auto outline = juce::Colour (0xff343b44);
        const auto accent = juce::Colour (0xff31c7b8);
        const auto text = juce::Colour (0xffe6e8eb);
        const auto textMuted = juce::Colour (0xff9aa3ad);

        setColour (juce::ResizableWindow::backgroundColourId, background);
        setColour (juce::TabbedButtonBar::tabOutlineColourId, outline);
        setColour (juce::TabbedButtonBar::frontOutlineColourId, accent);
        setColour (juce::TabbedButtonBar::tabTextColourId, textMuted);
        setColour (juce::TabbedButtonBar::frontTextColourId, text);
        setColour (juce::TabbedComponent::backgroundColourId, panel);
        setColour (juce::TextEditor::backgroundColourId, panel);
        setColour (juce::TextEditor::textColourId, text);
        setColour (juce::TextEditor::highlightColourId, accent.withAlpha (0.35f));
        setColour (juce::Label::textColourId, text);
        setColour (juce::ToggleButton::textColourId, text);
    }
};

class DeviceTab : public juce::Component
{
public:
    DeviceTab (juce::AudioDeviceManager& manager)
        : selector (manager, 1, 2, 0, 0, false, false, true, false)
    {
        addAndMakeVisible (selector);
    }

    void resized() override
    {
        selector.setBounds (getLocalBounds().reduced (12));
    }

private:
    juce::AudioDeviceSelectorComponent selector;
};

class NetworkTab : public juce::Component
{
public:
    NetworkTab()
    {
        title.setText ("NETWORK", juce::dontSendNotification);
        title.setJustificationType (juce::Justification::left);
        addAndMakeVisible (title);

        ipLabel.setText ("Target IP", juce::dontSendNotification);
        portLabel.setText ("Port", juce::dontSendNotification);
        addAndMakeVisible (ipLabel);
        addAndMakeVisible (portLabel);

        ipEditor.setText ("192.168.0.10");
        portEditor.setText ("9000");
        addAndMakeVisible (ipEditor);
        addAndMakeVisible (portEditor);

        sendToggle.setButtonText ("Send");
        addAndMakeVisible (sendToggle);
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced (16);
        title.setBounds (area.removeFromTop (24));

        auto row = area.removeFromTop (28);
        ipLabel.setBounds (row.removeFromLeft (80));
        ipEditor.setBounds (row.removeFromLeft (220));

        row = area.removeFromTop (28).withTrimmedTop (8);
        portLabel.setBounds (row.removeFromLeft (80));
        portEditor.setBounds (row.removeFromLeft (120));

        area.removeFromTop (12);
        sendToggle.setBounds (area.removeFromTop (24));
    }

private:
    juce::Label title;
    juce::Label ipLabel;
    juce::Label portLabel;
    juce::TextEditor ipEditor;
    juce::TextEditor portEditor;
    juce::ToggleButton sendToggle;
};

class StatusTab : public juce::Component
{
public:
    StatusTab()
    {
        title.setText ("STATUS", juce::dontSendNotification);
        title.setJustificationType (juce::Justification::left);
        addAndMakeVisible (title);

        statusLabel.setText ("Disconnected", juce::dontSendNotification);
        statusLabel.setJustificationType (juce::Justification::left);
        addAndMakeVisible (statusLabel);
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced (16);
        title.setBounds (area.removeFromTop (24));
        statusLabel.setBounds (area.removeFromTop (24).withTrimmedTop (8));
    }

private:
    juce::Label title;
    juce::Label statusLabel;
};

class MainComponent : public juce::Component
{
public:
    MainComponent()
        : tabs (juce::TabbedButtonBar::TabsAtTop)
    {
        juce::LookAndFeel::setDefaultLookAndFeel (&lookAndFeel);
        setLookAndFeel (&lookAndFeel);
        deviceManager.initialise (2, 0, nullptr, true);

        tabs.addTab ("Device", juce::Colours::black, new DeviceTab (deviceManager), true);
        tabs.addTab ("Network", juce::Colours::black, new NetworkTab(), true);
        tabs.addTab ("Status", juce::Colours::black, new StatusTab(), true);

        tabs.setTabBarDepth (32);
        addAndMakeVisible (tabs);
        setSize (720, 420);
    }

    ~MainComponent() override
    {
        setLookAndFeel (nullptr);
        juce::LookAndFeel::setDefaultLookAndFeel (nullptr);
    }

    void paint (juce::Graphics& g) override
    {
        g.fillAll (findColour (juce::ResizableWindow::backgroundColourId));
    }

    void resized() override
    {
        tabs.setBounds (getLocalBounds().reduced (12));
    }

private:
    IndustrialLookAndFeel lookAndFeel;
    juce::AudioDeviceManager deviceManager;
    juce::TabbedComponent tabs;
};

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
            
            setContentOwned (new MainComponent(), true);
            
            centreWithSize (720, 420); // Window size
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