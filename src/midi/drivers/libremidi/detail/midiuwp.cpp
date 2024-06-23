#define NOMINMAX 1
#define WIN32_LEAN_AND_MEAN 1
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include <Windows.Devices.Enumeration.h>
#include <Windows.Devices.Midi.h>
#include <Windows.Foundation.h>
#include <Windows.Storage.Streams.h>
#include <uwphelpers/StorageExtensions.h>
#include <uwphelpers/StorageAsync.h>
#include <iostream>
 
namespace
{  
    using namespace Windows::Foundation;
    using namespace Windows::Devices::Midi;
    using namespace Windows::Devices::Enumeration;
    using namespace Windows::Storage::Streams;

    class observer_winuwp_internal
    {
    public:
        struct port_info
        {
            Platform::String^ id;
            Platform::String^ name;
        };

    private:
        static void OnAdded(Windows::Devices::Enumeration::DeviceWatcher^ sender, Windows::Devices::Enumeration::DeviceInformation^ args) {
        }
        static void OnRemoved(Windows::Devices::Enumeration::DeviceWatcher^ sender, Windows::Devices::Enumeration::DeviceInformationUpdate^ args) {
        }
        static void OnUpdated(Windows::Devices::Enumeration::DeviceWatcher^ sender, Windows::Devices::Enumeration::DeviceInformationUpdate^ args) {
        }
        static void OnEnumerationCompleted(Windows::Devices::Enumeration::DeviceWatcher^ sender, Platform::Object^ args) {

        }

        void initialize(Platform::String^ deviceSelector)
        {
            deviceWatcher_ = DeviceInformation::CreateWatcher(deviceSelector);
            deviceWatcher_->Added += ref new Windows::Foundation::TypedEventHandler<Windows::Devices::Enumeration::DeviceWatcher^, Windows::Devices::Enumeration::DeviceInformation^>(&OnAdded);
            deviceWatcher_->Removed += ref new Windows::Foundation::TypedEventHandler<Windows::Devices::Enumeration::DeviceWatcher^, Windows::Devices::Enumeration::DeviceInformationUpdate^>(&OnRemoved);
            deviceWatcher_->Updated += ref new Windows::Foundation::TypedEventHandler<Windows::Devices::Enumeration::DeviceWatcher^, Windows::Devices::Enumeration::DeviceInformationUpdate^>(&OnUpdated);
            deviceWatcher_->EnumerationCompleted += ref new Windows::Foundation::TypedEventHandler<Windows::Devices::Enumeration::DeviceWatcher^, Platform::Object^>(&OnEnumerationCompleted);

            deviceWatcher_->Start();
        }


        static void OnMessageReceived(Windows::Devices::Midi::MidiInPort^ sender, Windows::Devices::Midi::MidiMessageReceivedEventArgs^ args) {
            const auto& msg = args->Message;

            auto reader = DataReader::FromBuffer(msg->RawData);
            Platform::Array<unsigned char>^ bs = ref new Platform::Array<unsigned char>(2);
            reader->ReadBytes(bs);

            double t = static_cast<double>(msg->Timestamp.Duration);
        }

        bool connected_ = false;
        MidiInPort^ OpenPort(Platform::String^ id)
        {
            MidiInPort^ port_;

            if (id->Length() > 0)
            {
                ExecuteTask(port_, MidiInPort::FromIdAsync(id));
                if (port_)
                {
                    port_->MessageReceived += ref new Windows::Foundation::TypedEventHandler<Windows::Devices::Midi::MidiInPort^, Windows::Devices::Midi::MidiMessageReceivedEventArgs^>(&OnMessageReceived);
                }
            }

            return port_;
        }

        void ClosePort(MidiInPort^ port_)
        {
            if (connected_)
            {
                if (port_)
                {
                    port_->Close();
                }
            }
        }

        Windows::Devices::Midi::MidiInPort^ port_{ nullptr };

        void SendMessage(MidiOutPort^ port_, const unsigned char* message, size_t size)
        {
            if (!connected_)
                return;

            InMemoryRandomAccessStream^ str;
            DataWriter^ rb = ref new DataWriter(str);
            auto value1 = (const uint8_t*)message;
            auto value2 = (const uint8_t*)message + size;
            Platform::Array<unsigned char>^ arr = ref new Platform::Array<unsigned char>(2) {
                (const uint8_t)value1,
                    (const uint8_t)value2
            };

            rb->WriteBytes(arr);
            port_->SendBuffer(rb->DetachBuffer());
        }

    private:
        DeviceWatcher^ deviceWatcher_{ nullptr };
    };
}
