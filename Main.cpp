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
        setColour (juce::TextEditor::outlineColourId, outline);
        setColour (juce::TextEditor::focusedOutlineColourId, accent);
        setColour (juce::Label::textColourId, text);
        setColour (juce::ToggleButton::textColourId, text);
        setColour (juce::GroupComponent::outlineColourId, outline);
        setColour (juce::GroupComponent::textColourId, textMuted);
        setColour (juce::ComboBox::backgroundColourId, panel);
        setColour (juce::ComboBox::textColourId, text);
        setColour (juce::ComboBox::outlineColourId, outline);
        setColour (juce::ComboBox::arrowColourId, accent);
    }
};

class DeviceTab : public juce::Component
{
public:
    DeviceTab (juce::AudioDeviceManager& manager)
        : selector (manager, 1, 2, 0, 0, false, false, true, false)
    {
        title.setText ("DEVICE", juce::dontSendNotification);
        title.setJustificationType (juce::Justification::left);
        title.setFont (juce::Font (14.0f, juce::Font::bold));
        addAndMakeVisible (title);

        inputGroup.setText ("INPUT CONFIG");
        addAndMakeVisible (inputGroup);

        channelLabel.setText ("Channel", juce::dontSendNotification);
        channelLabel.setFont (juce::Font (12.0f));
        addAndMakeVisible (channelLabel);

        channelBox.addItem ("Ch 1", 1);
        channelBox.addItem ("Ch 2", 2);
        channelBox.setSelectedId (1);
        addAndMakeVisible (channelBox);

        addAndMakeVisible (selector);
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced (16);
        title.setBounds (area.removeFromTop (24));

        auto content = area;
        auto rightPanel = content.removeFromRight (220);

        selector.setBounds (content.reduced (4));

        inputGroup.setBounds (rightPanel);
        auto groupArea = rightPanel.reduced (12);
        channelLabel.setBounds (groupArea.removeFromTop (20));
        channelBox.setBounds (groupArea.removeFromTop (24));
    }

private:
    juce::Label title;
    juce::GroupComponent inputGroup;
    juce::Label channelLabel;
    juce::ComboBox channelBox;
    juce::AudioDeviceSelectorComponent selector;
};

class NetworkTab : public juce::Component
{
public:
    NetworkTab()
    {
        title.setText ("NETWORK", juce::dontSendNotification);
        title.setJustificationType (juce::Justification::left);
        title.setFont (juce::Font (14.0f, juce::Font::bold));
        addAndMakeVisible (title);

        networkGroup.setText ("TARGET");
        addAndMakeVisible (networkGroup);

        ipLabel.setText ("Target IP", juce::dontSendNotification);
        portLabel.setText ("Port", juce::dontSendNotification);
        ipLabel.setFont (juce::Font (12.0f));
        portLabel.setFont (juce::Font (12.0f));
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

        networkGroup.setBounds (area.removeFromTop (130));
        auto groupArea = networkGroup.getBounds().reduced (12);
        auto row = groupArea.removeFromTop (28);
        ipLabel.setBounds (row.removeFromLeft (80));
        ipEditor.setBounds (row.removeFromLeft (220));

        row = groupArea.removeFromTop (28).withTrimmedTop (8);
        portLabel.setBounds (row.removeFromLeft (80));
        portEditor.setBounds (row.removeFromLeft (120));

        area.removeFromTop (12);
        sendToggle.setBounds (area.removeFromTop (24));
    }

private:
    juce::Label title;
    juce::GroupComponent networkGroup;
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
        title.setFont (juce::Font (14.0f, juce::Font::bold));
        addAndMakeVisible (title);

        stateGroup.setText ("STATE");
        addAndMakeVisible (stateGroup);

        statusLabel.setText ("Disconnected", juce::dontSendNotification);
        statusLabel.setJustificationType (juce::Justification::left);
        statusLabel.setFont (juce::Font (16.0f, juce::Font::bold));
        addAndMakeVisible (statusLabel);
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced (16);
        title.setBounds (area.removeFromTop (24));

        stateGroup.setBounds (area.removeFromTop (120));
        auto groupArea = stateGroup.getBounds().reduced (12);
        statusLabel.setBounds (groupArea.removeFromTop (24));
    }

private:
    juce::Label title;
    juce::GroupComponent stateGroup;
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